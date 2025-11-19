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
SMTPSession smtp;
#define RECIPIENT_EMAIL "g004642115@atu.ie"
#define AUTHOR_EMAIL "esp32rj@gmail.com"
#define AUTHOR_PASSWORD "bcho nulq vuvz rixe"
#define SMTP_PORT 465
#define SMTP_HOST "smtp.gmail.com"

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
  DHT.read(DHT11_PIN);
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
  
  MailClient.networkReconnect(true);
  smtp.debug(1);
  smtp.callback(smtpCallback);

  Session_Config config;

  config.server.host_name = SMTP_HOST;
  config.server.port = SMTP_PORT;
  config.login.email = AUTHOR_EMAIL;
  config.login.password = AUTHOR_PASSWORD;

  config.login.user_domain = F("127.0.0.1");
  config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
  config.time.gmt_offset = 0;
  config.time.day_light_offset = 0;

  SMTP_Message message;

  message.sender.name = F("ESP Mail");
  message.sender.email = AUTHOR_EMAIL;
  message.subject = F("Test sending message as embedded files");
  message.addRecipient(F("Admin"), RECIPIENT_EMAIL);

  message.html.content = F("<span style=\"color:#0055ff;\">This is html message</span>");

  message.html.charSet = F("utf-8");

  message.html.transfer_encoding = Content_Transfer_Encoding::enc_qp;

  message.html.embed.enable = true;
  
  message.html.embed.filename = F("test.html");

  message.html.embed.type = esp_mail_smtp_embed_message_type_attachment;

    message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;

  message.addHeader(F("Message-ID: <abcde.fghij@gmail.com>"));

  
  if (!smtp.connect(&config))
  {
    MailClient.printf("Connection error, Status Code: %d, Error Code: %d, Reason: %s\n", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
    return;
  }

  if (!smtp.isLoggedIn())
  {
    Serial.println("Error, Not yet logged in.");
  }
  else
  {
    if (smtp.isAuthenticated())
      Serial.println("Successfully logged in.");
    else
      Serial.println("Connected with no Auth.");
  }

  if (!MailClient.sendMail(&smtp, &message))
    MailClient.printf("Error, Status Code: %d, Error Code: %d, Reason: %s\n", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());

  MailClient.printf("Free Heap: %d\n", MailClient.getFreeHeap());

  message.text.content = F("This is simple plain text message");
  message.text.charSet = F("utf-8");
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_base64;
  message.text.embed.enable = true;
  message.text.embed.filename = F("test.txt");
  message.text.embed.type = esp_mail_smtp_embed_message_type_inline;

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

void smtpCallback(SMTP_Status status)
{
  /* Print the current status */
  Serial.println(status.info());

  /* Print the sending result */
  if (status.success())
  {
    // MailClient.printf used in the examples is for format printing via debug Serial port
    // that works for all supported Arduino platform SDKs e.g. SAMD, ESP32 and ESP8266.
    // In ESP8266 and ESP32, you can use Serial.printf directly.

    Serial.println("----------------");
    MailClient.printf("Message sent success: %d\n", status.completedCount());
    MailClient.printf("Message sent failed: %d\n", status.failedCount());
    Serial.println("----------------\n");

    for (size_t i = 0; i < smtp.sendingResult.size(); i++)
    {
      /* Get the result item */
      SMTP_Result result = smtp.sendingResult.getItem(i);

      // In case, ESP32, ESP8266 and SAMD device, the timestamp get from result.timestamp should be valid if
      // your device time was synched with NTP server.
      // Other devices may show invalid timestamp as the device time was not set i.e. it will show Jan 1, 1970.
      // You can call smtp.setSystemTime(xxx) to set device time manually. Where xxx is timestamp (seconds since Jan 1, 1970)

      MailClient.printf("Message No: %d\n", i + 1);
      MailClient.printf("Status: %s\n", result.completed ? "success" : "failed");
      MailClient.printf("Date/Time: %s\n", MailClient.Time.getDateTimeString(result.timestamp, "%B %d, %Y %H:%M:%S").c_str());
      MailClient.printf("Recipient: %s\n", result.recipients.c_str());
      MailClient.printf("Subject: %s\n", result.subject.c_str());
    }
    Serial.println("----------------\n");

    // You need to clear sending result as the memory usage will grow up.
    smtp.sendingResult.clear();
  }
}
