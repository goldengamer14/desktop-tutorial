const url = "ws://localhost:3000/";
let socket = new WebSocket(url);

socket.onopen = handleOpen;
socket.onmessage = handleMessage;
socket.onerror = handleError;
socket.onclose = handleClose;

function handleOpen(event) {
    console.log("WebSocket is open now.", event);
    socket.send("Hello Server!");
};

function handleMessage(event) {
    console.log("WebSocket message received:", event);
};

function handleError(error) {
    console.error("WebSocket error observed:", error);
};

function handleClose(event) {
    console.log("WebSocket is closed now.", event);
    socket.close(1000, "Unknown reason");

    console.log("Reconnecting to the WebSocket server...");
    socket = connectWebSocket();
};

function connectWebSocket() {
    const socket = new WebSocket(url);

    new Promise((resolve, reject) => {
        socket.onopen = () => { handleOpen(); resolve(); };
        socket.onmessage = handleMessage;
        socket.onerror = () => { handleError(); reject(socket.readyState); };
        socket.onclose = handleClose;
    }).then(() => {
        console.log("WebSocket Reconnected Successfully! You can send messages now.");
        return socket;
    }).catch((readyState) => {
        console.log("WebSocket is not open yet. Cannot send message.");
        console.log("readyState:", readyState);
        return socket;
    });
}