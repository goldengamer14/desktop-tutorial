import WebSocket, { WebSocketServer } from "ws";
// const WebSocket = require("ws");

WebSocket.Server = WebSocketServer;
console.log(WebSocket);
console.log(WebSocket.Server);

let server = new WebSocket.Server({ port: 3000 });

console.log("Server");
console.log(server);

server.on("connection", socket => {
    console.log("A client connected.");

    socket.on("message", message => {
        const text = message.toString();
        console.log("Received message:", text);

        server.clients.forEach(client => {
            if (client.readyState === WebSocket.OPEN) {
                if (client !== socket) {
                    console.log("Broadcasting message to other clients:", text);
                    client.send(text);
                } else {
                    console.log("Sending Broadcast acknowledgment to sender.");
                    client.send("The message has been Broadcasted to all clients.");
                }
            }
        });
    });

    socket.on("close", () => {
        console.log("A client disconnected.");
    });
});