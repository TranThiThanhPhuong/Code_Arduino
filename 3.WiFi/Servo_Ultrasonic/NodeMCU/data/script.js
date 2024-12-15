let url = `ws://${window.location.hostname}/ws`;
// let url = `ws://192.168.4.1/ws`;
let sensorValue = document.getElementById("sensorValue");
let degree = document.getElementById("degree");
let websocket;

// This is called when the page finished loading
function init() {
  wsConnect(url);
}

// Call this to connect to the WebSockets server
function wsConnect(url) {
  console.log("Trying to open a WebSocket connection...");
  // create websocket instance
  websocket = new WebSocket(url);

  // Assign callbacks
  websocket.onopen = onOpen;
  websocket.onclose = onClose;
  websocket.onmessage = onMessage;
  websocket.onerror = onError;
}

// Called when a WebSockets connection is established with the server
function onOpen(event) {
  console.log("Connected");
}

// Called when a WebSockets connection is closed
function onClose(event) {
  // Log connection state
  console.log("Disconnected");
  // Try to reconnect after a few seconds
  setTimeout(function () {
    wsConnect(url);
  }, 2000);
}

// Called when a WebSockets connection is closed
function onMessage(event) {
  // Print out our received message
  console.log("Received: " + event.data);
  message_JSON = JSON.parse(event.data);
  sensorValue.textContent = message_JSON["sensor_value"] + " cm";
}

// Called when a WebSocket error occurs
function onError(event) {
  console.log("ERROR: " + event.data);
}

// Sends a message to the server (and prints it to the console)
function doSend(message) {
  console.log("Sending: " + message);
  websocket.send(message);
}

function controlServo() {
  let data = {
    degree: parseInt(degree.value),
  };
  doSend(JSON.stringify(data));
}

// Call the init function as soon as the page loads
window.addEventListener("load", init, false);
