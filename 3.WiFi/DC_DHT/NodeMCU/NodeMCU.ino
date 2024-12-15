#include <FS.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer webServer(80);
AsyncWebSocket webSocket("/ws");

// Network Credentials
#define SSID "102210030"
#define PSWD "ducchung"


// JSON
#include <ArduinoJson.h>
StaticJsonDocument<200> json;
DeserializationError error;
String message;

/****************************************
 * Websocket Event Hander
 */
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;

    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;

    case WS_EVT_DATA:
      {
        AwsFrameInfo *info = (AwsFrameInfo *)arg;
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
          data[len] = 0;
          error = deserializeJson(json, data);
          if (!error) {
            serializeJson(json, Serial);
          }
        }
      }
      break;

    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

/****************************************
 * Main
 */
void setup() {
  Serial.begin(115200);
  // Make sure we can read the file system
  if (!SPIFFS.begin()) {
    Serial.println("Error mounting SPIFFS");
    while (1)
      ;
  }
  WiFi.softAP(SSID, PSWD);
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
  webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    IPAddress remote_ip = request->client()->remoteIP();
    Serial.println("[" + remote_ip.toString() + "] HTTP GET request of " + request->url());
    request->send(SPIFFS, "/index.html", "text/html");
  });
  webServer.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    IPAddress remote_ip = request->client()->remoteIP();
    Serial.println("[" + remote_ip.toString() + "] HTTP GET request of " + request->url());
    request->send(SPIFFS, "/script.js", "text/javascript");
  });
  webServer.onNotFound([](AsyncWebServerRequest *request) {
    IPAddress remote_ip = request->client()->remoteIP();
    Serial.println("[" + remote_ip.toString() + "] HTTP GET request of " + request->url());
    request->send(404, "text/plain", "NOT FOUND");
  });

  webSocket.onEvent(onEvent);
  webServer.addHandler(&webSocket);
  webServer.begin();
}

void loop() {
  if (Serial.available()) {
    error = deserializeJson(json, Serial);
    if (!error) {
      message = "";
      serializeJson(json, message);
      webSocket.textAll(message);
    }
  }

  webSocket.cleanupClients();
}
