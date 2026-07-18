const fixedHeaders = {
    "Content-Type": "application/json",
    "X-API-Key": "secret-123"
};

const adminHeaders = {
    ...fixedHeaders,
    "role": "admin"
};

async function testGet() {
    // Get all users
    await fetch("http://localhost:3000/user")
        .then(res => res.json())
        .then(data => console.log(data))
        .catch(err => console.error(err));

    // Get user with ID 4
    await fetch("http://localhost:3000/user/4")
        .then(res => res.json())
        .then(data => console.log(data))
        .catch(err => console.error(err));

    // Get user with Name "Elena"
    await fetch("http://localhost:3000/user?name=Elena")
        .then(res => res.json())
        .then(data => console.log(data))
        .catch(err => console.error(err));
}

async function testPost() {
    // Valid POST request
    await fetch("http://localhost:3000/user", {
        method: "POST",
        body: JSON.stringify({
            "name": "Avil",
            "email": "avil@example.com"
        }),
        headers: fixedHeaders
    }).then(res => res.json())
        .then(data => console.log(data))
        .catch(err => console.error(err));

    // Invalid POST request (invalid api-key)
    await fetch("http://localhost:3000/user", {
        method: "POST",
        headers: {
            "Content-Type": "application/json",
            "X-API-Key": "invalid-key",
            "role": "admin"
        },
        body: JSON.stringify({
            "name": "Adwaita",
            "email": "adwaita@example.com"
        })
    }).then(res => res.json())
        .then(data => console.log(data))
        .catch(err => console.error(err));
}

async function testPut() {
    // Valid PUT request
    await fetch("http://localhost:3000/user/1", {
        method: "PUT",
        headers: adminHeaders,
        body: JSON.stringify({
            "name": "Cavil",
            "email": "cavil@example.com"
        })
    }).then(res => res.json())
        .then(data => console.log(data))
        .catch(err => console.error(err));

    // Invalid PUT request (Admin role required)
    await fetch("http://localhost:3000/user/1", {
        method: "PUT",
        headers: fixedHeaders,
        body: JSON.stringify({
            "name": "Cavil",
            "email": "cavil@example.com"
        })
    }).then(res => res.json())
        .then(data => console.log(data))
        .catch(err => console.error(err));

    // Invalid PUT request (non-existent user)
    await fetch("http://localhost:3000/user/999", {
        method: "PUT",
        headers: adminHeaders,
        body: JSON.stringify({
            "name": "Cavil",
            "email": "cavil@example.com"
        })
    }).then(res => res.json())
        .then(data => console.log(data))
        .catch(err => console.error(err));

    // Invalid PUT request (invalid api-key)
    await fetch("http://localhost:3000/user/1", {
        method: "PUT",
        headers: {
            "Content-Type": "application/json",
            "X-API-Key": "invalid-key",
            "role": "admin"
        },
        body: JSON.stringify({
            "name": "Johnson",
            "email": "johnson@example.com"
        })
    }).then(res => res.json())
        .then(data => console.log(data))
        .catch(err => console.error(err));
}

async function testDelete() {
    // Valid DELETE request
    await fetch("http://localhost:3000/user/2", {
        method: "DELETE",
        headers: adminHeaders
    }).then(res => res.json())
        .then(data => console.log(data))
        .catch(err => console.error(err));

    // Invalid DELETE request (Admin role required)
    await fetch("http://localhost:3000/user/1", {
        method: "DELETE",
        headers: fixedHeaders
    }).then(res => res.json())
        .then(data => console.log(data))
        .catch(err => console.error(err));

    // Invalid DELETE request (deleted user)
    await fetch("http://localhost:3000/user/2", {
        method: "DELETE",
        headers: adminHeaders
    }).then(res => res.json())
        .then(data => console.log(data))
        .catch(err => console.error(err));

    // Invalid DELETE request (invalid api-key)
    await fetch("http://localhost:3000/user/3", {
        method: "DELETE",
        headers: {
            "Content-Type": "application/json",
            "X-API-Key": "invalid-key",
            "role": "admin"
        }
    }).then(res => res.json())
        .then(data => console.log(data))
        .catch(err => console.error(err));
}

const main = async function () {
    console.log("\n\ntestGet\n\n");
    await testGet();

    console.log("\n\ntestPost\n\n");
    await testPost();

    console.log("\n\ntestPut\n\n");
    await testPut();

    console.log("\n\ntestDelete\n\n");
    await testDelete();
}();
