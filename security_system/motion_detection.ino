#include <WiFi.h>
#include <PubSubClient.h>
#include "esp_camera.h"

const char* ssid = "Nenovi_Tenda";
const char* password = "bobi6445bobi6445";

// const char* mqtt_server = "your_rpi_ip";
// const char* topic = "security/camera1/detection";

// WiFiClient espClient;
// PubSubClient client(espClient);

unsigned long last_detection = 0;
const unsigned long cooldown_ms = 5000;

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Wifi connected");
}

// void reconnect() {
//   while (!client.connected()) {
//     if (client.connect("ESP32CAMClient")) {
//       client.publish(topic, "ESP32CAM connected");
//     } else {
//       delay(5000);
//     }
//   }
// }

void setup_camera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 5;
  config.pin_d1 = 18;
  config.pin_d2 = 19;
  config.pin_d3 = 21;
  config.pin_d4 = 36;
  config.pin_d5 = 39;
  config.pin_d6 = 34;
  config.pin_d7 = 35;
  config.pin_xclk = 0;
  config.pin_pclk = 22;
  config.pin_vsync = 25;
  config.pin_href = 23;
  config.pin_sccb_sda = 26;
  config.pin_sccb_scl = 27;
  config.pin_pwdn = 32;
  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_GRAYSCALE;
  config.frame_size = FRAMESIZE_QQVGA;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed: 0x%x", err);
    return;
  }
}

// void send_mqtt_alert() {
//   client.publish(topic, "Person Detected");
// }

void setup() {
  Serial.begin(115200);
  setup_wifi();
  // client.setServer(mqtt_server, 1883);
  // reconnect();
  setup_camera();
}

uint8_t* prev_frame = nullptr;

void loop() {
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Error: Failed to capture frame");
    return;
  }

  int diff_sum = 0;
  if (prev_frame) {
    for (int i = 0; i < fb->len; i++) {
      diff_sum += abs(fb->buf[i] - prev_frame[i]);
    }
  
    Serial.println(diff_sum);
    if (diff_sum > 500000 && millis() - last_detection > cooldown_ms) {
      Serial.println("Motion detected");
      last_detection = millis();
    }
  }

  if (prev_frame) free(prev_frame);
  prev_frame = (uint8_t*)malloc(fb->len);
  memcpy(prev_frame, fb->buf, fb->len);

  esp_camera_fb_return(fb);
  delay(1000);
}
