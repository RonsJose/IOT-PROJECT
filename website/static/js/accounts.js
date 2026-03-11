function showMessage(message, isError = false) {
    const messageDiv = document.getElementById("message");
    messageDiv.textContent = message;
    messageDiv.className = isError ? "message error" : "message success";
}

function clearMessage() {
    const messageDiv = document.getElementById("message");
    messageDiv.textContent = "";
    messageDiv.className = "message";
}

async function createAccount() {
    clearMessage();

    const full_name = document.getElementById("full_name").value.trim();
    const email = document.getElementById("email").value.trim();

    if (!full_name) {
        showMessage("Full name is required", true);
        return;
    }

    try {
        const response = await fetch("/create-account", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({
                full_name: full_name,
                email: email
            })
        });

        const data = await response.json();

        if (!response.ok) {
            showMessage(data.error || "Failed to create account", true);
            return;
        }

        showMessage(`Account created. User ID: ${data.user_id}`);

        document.getElementById("full_name").value = "";
        document.getElementById("email").value = "";

        loadUsers();
    } catch (error) {
        showMessage("Error creating account", true);
    }
}

async function registerCard() {
    clearMessage();

    const user_id = document.getElementById("card_user_id").value.trim();
    const card_uid = document.getElementById("card_uid").value.trim();
    const card_label = document.getElementById("card_label").value.trim();

    if (!user_id || !card_uid) {
        showMessage("User ID and Card UID are required", true);
        return;
    }

    try {
        const response = await fetch("/register-card", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({
                user_id: user_id,
                card_uid: card_uid,
                card_label: card_label
            })
        });

        const data = await response.json();

        if (!response.ok) {
            showMessage(data.error || "Failed to register card", true);
            return;
        }

        showMessage(data.message || "Card registered");

        document.getElementById("card_user_id").value = "";
        document.getElementById("card_uid").value = "";
        document.getElementById("card_label").value = "";

        loadUsers();
    } catch (error) {
        showMessage("Error registering card", true);
    }
}

async function deleteAccount() {
    clearMessage();

    const user_id = document.getElementById("delete_user_id").value.trim();

    if (!user_id) {
        showMessage("User ID is required", true);
        return;
    }

    try {
        const response = await fetch(`/delete-account/${user_id}`, {
            method: "DELETE"
        });

        const data = await response.json();

        if (!response.ok) {
            showMessage(data.error || "Failed to delete account", true);
            return;
        }

        showMessage(data.message || "Account deleted");

        document.getElementById("delete_user_id").value = "";

        loadUsers();
    } catch (error) {
        showMessage("Error deleting account", true);
    }
}

async function loadUsers() {
    const usersList = document.getElementById("users_list");

    try {
        const response = await fetch("/users");
        const users = await response.json();

        usersList.innerHTML = "";

        if (!users.length) {
            usersList.innerHTML = `<p class="empty-users">No users found.</p>`;
            return;
        }

        users.forEach(user => {
            const div = document.createElement("div");
            div.className = "user-card";

            div.innerHTML = `
                <h3>${user.full_name}</h3>
                <div class="user-row"><strong>User ID:</strong> ${user.user_id}</div>
                <div class="user-row"><strong>Email:</strong> ${user.email || "N/A"}</div>
                <div class="user-row"><strong>Card UID:</strong> ${user.card_uid || "N/A"}</div>
                <div class="user-row"><strong>Card Label:</strong> ${user.card_label || "N/A"}</div>
            `;

            usersList.appendChild(div);
        });
    } catch (error) {
        usersList.innerHTML = `<p class="empty-users">Error loading users.</p>`;
    }
}

window.onload = loadUsers;