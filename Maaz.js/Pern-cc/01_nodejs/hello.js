import http from 'http';
// import os from 'os';

console.log("This is hello.js file");

// console.log("User Info:", os.userInfo());
// console.log("System Uptime (in seconds):", os.uptime());
// console.log("Current Working Directory:", process.cwd());
// console.log("Node.js Version:", process.version);
// console.log("Platform:", os.platform());

const server = http.createServer((req, res) => {
    res.writeHead(200, {'Content-Type': 'text/plain'});
    res.end('Hello, World!\n');
});

server.on("message", msg => {
    if (msg === "shutdown") {
        server.close(() => {
            console.log("Server is shutting down...");
        });
    }
});

server.listen(3000, () => console.log("Server is running at port http://localhost:3000"));