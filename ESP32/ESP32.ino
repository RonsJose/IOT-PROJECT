#include <WiFi.h>
#include <NetworkClient.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include "webpagecode.h"
#include "WifiCred.h"
#include <DFRobot_DHT11.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include "ThingSpeak.h"
#include "API.h"
#include <ESP_Mail_Client.h>

AsyncWebServer server(80);

#define PERIOD 1000
PulseOximeter pox;
int previous = 0;
float heartRate = 0;
float spo2 = 0;
DFRobot_DHT11 DHT;
#define DHT11_PIN 27
const int TRIG_PIN = 25;
const int ECHO_PIN = 14;
long duration;
float cms, inches;
unsigned long channelNum=5;
WiFiClient client;
#define UPLOAD_PERIOD 10000
int uploadLast=0;
TaskHandle_t Task1;
#define emailSenderAccount    "esp32rj@gmail.com"
#define emailSenderPassword   "bcho nulq vuvz rixe"
#define smtpServer            "smtp.gmail.com"
#define smtpServerPort        465
#define emailSubject          "Testing"
#define emailRecipient        "g004642115@atu.ie"

SMTPSession smtp;

void smtpCallback(SMTP_Status status);

void Task1Code(void *pvParameters)
{
  for(;;)
  {
    int now= millis();
    if(now-uploadLast>UPLOAD_PERIOD)
  {
  ThingSpeak.setField(1,DHT.temperature);
  ThingSpeak.setField(2,DHT.humidity);
  ThingSpeak.setField(3,heartRate);
  ThingSpeak.setField(4,spo2);
  ThingSpeak.setField(5,cms);

  int a = ThingSpeak.writeFields(channelNum,ApiKey);
  if(a==200)
  {
    Serial.println("Good");
  }
  else
  {
    Serial.println("Bad");
  }
  uploadLast=now;

  }
  vTaskDelay(100/portTICK_PERIOD_MS);

    }
  }

String getTemp() {
   DHT.read(DHT11_PIN);
  if(isnan(DHT.temperature))
  {
    Serial.println("Failed to get reading from DHT T");
    return "--";
  }
  else  
  {
     Serial.print("temp:");
    Serial.print(DHT.temperature);
    return String(DHT.temperature) + " °C";
  }
}

String getHumid(){
  DHT.read(DHT11_PIN);
  if(isnan(DHT.humidity))
  {
    Serial.println("Failed to get reading from DHT H");
    return "--";
  }
else 
{
  Serial.print("  humi:");
  Serial.println(DHT.humidity);
  return String(DHT.humidity) + " %";
}
}

String getDis(){
digitalWrite (TRIG_PIN, LOW);
delayMicroseconds (5) ;
digitalWrite (TRIG_PIN, HIGH);
delayMicroseconds (10) ;
digitalWrite (TRIG_PIN, LOW);
duration = pulseIn (ECHO_PIN, HIGH) ;
cms = (duration/2) / 29.1;
if (isnan(cms))
{
  Serial.println("Failed to get reading from HCRS04");
  return "--";
}
else
{
return String(cms) + " Cm";
}

}

String getHeartRate() {
  if(isnan(heartRate))
  {
    Serial.println("Failed to get reading from heart click heart");
    return "--";
  }
  else
  {
    return String(heartRate, 1) + " bpm";
  }
  
}

String getSpO2() {
  if(isnan(spo2))
  {
    Serial.println("Failed to get reading from heart click spo2");
    return "--";
  }
  else
  {
    return String(spo2, 1) + " %";
  }
  
}


void handleRoot(AsyncWebServerRequest *request) {
    String page = String(homePage);
    page.replace("%TEMPERATURE%", getTemp());
    page.replace("%HUMIDITY%", getHumid());
    page.replace("%DISTANCE%", getDis());
    page.replace("%HEARTRATE%", getHeartRate());
    page.replace("%SPO2%", getSpO2());
    request->send(200, "text/html", page);
}

void handleNotFound(AsyncWebServerRequest *request)  {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += request->url();
  message += "\nMethod: ";
  message += (request->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += request->params();
  message += "\n";
  for (uint8_t i = 0; i < request->params(); i++) 
  {
    const AsyncWebParameter* p = request->getParam(i);
    message += " " + p->name() + ": " + p->value() + "\n";
  }
 request->send(404, "text/plain", message);
}

void setup(void) {
  Serial.begin(115200);
  ThingSpeak.begin(client);
  pinMode (TRIG_PIN, OUTPUT);
  pinMode (ECHO_PIN, INPUT);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("iot")) {
    Serial.println("MDNS responder started");
  }
  
    if (!pox.begin()) {
    Serial.println("FAILED to initialize MAX30100!");
    while (1);
  } else {
    Serial.println("MAX30100 initialized successfully!");
  }

  xTaskCreatePinnedToCore(
    Task1Code,
    "Task 1",
    2000,
    NULL,
    2,
    &Task1,
    0);

  server.on("/", HTTP_GET, handleRoot);

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", getTemp().c_str());
  });

  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", getHumid().c_str());
  });

  server.on("/distance", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain",getDis().c_str());
  });

  server.on("/heartRate", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", getHeartRate().c_str());
  });
  server.on("/spO2", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", getSpO2().c_str());
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  sendEmail();
}



void loop(void) {
   pox.update();

  if (millis() - previous > PERIOD) {
    heartRate = pox.getHeartRate();
    spo2 = pox.getSpO2(); 
    Serial.printf("Heart Rate: %.1f bpm | SpO2: %.1f%%\n", heartRate, spo2);
    previous = millis();
  }
  
}

void sendEmail( void ) {
  smtp.debug(1);
  smtp.callback(smtpCallback);
  Session_Config config;
  
  config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
  config.time.gmt_offset = 0;
  config.time.day_light_offset = 1;

  /* Set the session config */
  config.server.host_name = smtpServer;
  config.server.port = smtpServerPort;
  config.login.email = emailSenderAccount;
  config.login.password = emailSenderPassword;
  config.login.user_domain = "";

  SMTP_Message message;

  message.enable.chunking = true;

  message.sender.name = "Test";
  message.sender.email = emailSenderAccount;

  message.subject = emailSubject;
  message.addRecipient("rons", emailRecipient);

  String htmlMsg = "<h2>Testing if this works.</h2>";
  message.html.content = htmlMsg.c_str();
  message.html.charSet = "utf-8";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_qp;

  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_normal;
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;


  if (!smtp.connect(&config))
    return;
  
  if (!MailClient.sendMail(&smtp, &message, true))
    Serial.println("Error sending Email, " + smtp.errorReason());
}

void smtpCallback(SMTP_Status status)
{
  Serial.println(status.info());

  if (status.success())
  {
    Serial.println("----------------");
    MailClient.printf("Message sent success: %d\n", status.completedCount());
    MailClient.printf("Message sent failed: %d\n", status.failedCount());
    Serial.println("----------------\n");

    for (size_t i = 0; i < smtp.sendingResult.size(); i++)
    {
      SMTP_Result result = smtp.sendingResult.getItem(i);

      MailClient.printf("Message No: %d\n", i + 1);
      MailClient.printf("Status: %s\n", result.completed ? "success" : "failed");
      MailClient.printf("Date/Time: %s\n", MailClient.Time.getDateTimeString(result.timestamp, "%B %d, %Y %H:%M:%S").c_str());
      MailClient.printf("Recipient: %s\n", result.recipients.c_str());
      MailClient.printf("Subject: %s\n", result.subject.c_str());
    }
    Serial.println("----------------\n");

    smtp.sendingResult.clear();
  }
}
