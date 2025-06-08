#define BLYNK_TEMPLATE_ID "TMPL4sRG6t46q"
#define BLYNK_TEMPLATE_NAME "Quickstart Device"
#define BLYNK_AUTH_TOKEN "UO8rK-lkfZMlfey_jR3JKo-kSxfTzl9c"

#include <WiFi.h>
#include <esp_camera.h>
#include <WebServer.h>
#include <BlynkSimpleEsp32.h>

// WiFi credentials
const char* ssid = "Beeline_2G_F15FF5";
const char* password = "vKRAJ6WAJu";

// Camera pins for AI Thinker ESP32-CAM
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

WebServer server(80);

void handle_jpg_stream() {
  WiFiClient client = server.client();
  if (!client.connected()) {
    Serial.println("Client not connected");
    Blynk.virtualWrite(V13, "Client not connected");
    return;
  }

  Serial.println("MJPEG stream requested");
  Blynk.virtualWrite(V13, "MJPEG stream requested");
  client.print("HTTP/1.1 200 OK\r\n");
  client.print("Content-Type: multipart/x-mixed-replace; boundary=frame\r\n");
  client.print("Cache-Control: no-cache\r\n");
  client.print("Pragma: no-cache\r\n");
  client.print("\r\n");

  int frame_count = 0;
  while (client.connected()) {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Failed to capture frame");
      Blynk.virtualWrite(V13, "Failed to capture frame");
      client.print("HTTP/1.1 500 Internal Server Error\r\n\r\n");
      client.stop();
      break;
    }
    Serial.printf("Frame %d captured, size: %u bytes\n", ++frame_count, fb->len);

    client.print("--frame\r\n");
    client.print("Content-Type: image/jpeg\r\n");
    client.print("Content-Length: ");
    client.print(fb->len);
    client.print("\r\n\r\n");
    client.write((const char *)fb->buf, fb->len);
    client.print("\r\n");

    esp_camera_fb_return(fb);

    if (!client.available() && !client.connected()) {
      Serial.println("Client disconnected");
      Blynk.virtualWrite(V13, "Client not connected");
      break;
    }
    delay(50);
  }

  Serial.println("MJPEG stream stopped");
  Blynk.virtualWrite(V13, "MJPEG stream stopped");
  client.stop();
}

void handle_jpg() {
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Failed to capture single frame");
    Blynk.virtualWrite(V13, "Failed to capture single frame");
    server.send(500, "text/plain", "Camera capture failed");
    return;
  }
  Serial.printf("Single frame captured, size: %u bytes\n", fb->len);
  server.send_P(200, "image/jpeg", (const char *)fb->buf, fb->len);
  esp_camera_fb_return(fb);
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  // Initialize camera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 10000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 20;
  config.fb_count = 2;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x\n", err);
    Blynk.virtualWrite(V13, "Camera init failed");
    return;
  }
  Serial.println("Camera initialized");
  Blynk.virtualWrite(V13, "Camera initialized");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.println(WiFi.localIP());
  Blynk.virtualWrite(V13, "WiFi connected: " + WiFi.localIP().toString());

  // Start Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);

  // Setup web server
  server.on("/mjpeg/1", HTTP_GET, handle_jpg_stream);
  server.on("/jpg", HTTP_GET, handle_jpg);
  server.begin();
  Serial.println("Server started");
  Blynk.virtualWrite(V13, "Server started");
}

void loop() {
  Blynk.run();
  server.handleClient();
}
