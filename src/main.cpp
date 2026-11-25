#include <M5Unified.h>
#include <WiFi.h>
#include <Preferences.h>
#include <WebServer.h>

Preferences prefs;
WebServer server(80);
String ssid, password;
bool connected = false;
unsigned page = 0;
const char* pages[] = {"HOME","METEO","NETWORK","APRS","SENSOR","HA","RADIO","POWER"};
void draw() {
  auto& d = M5.Display;
  d.fillScreen(TFT_WHITE);
  d.setTextColor(TFT_BLACK, TFT_WHITE);
  d.setTextSize(3);
  d.setCursor(24,24);
  d.println("M5Paper MultiPanel");
  d.setTextSize(2);
  d.println("SYSTEM - development preview");
  d.drawFastHLine(20,100,d.width()-40,TFT_BLACK);
  d.setCursor(24,125);
  d.println(pages[page]);
  d.println(connected ? WiFi.localIP().toString() : "Wi-Fi offline");
  d.printf("Battery: %d%%\n", M5.Power.getBatteryLevel());
  d.println("Touch: left / right to change page");
  d.println("Modules are not implemented yet.");
}
void setup() {
  Serial.begin(115200);
  auto cfg = M5.config();
  M5.begin(cfg);
  M5.Display.setRotation(1);
  M5.Display.setEpdMode(epd_mode_t::epd_quality);
  prefs.begin("multipanel", true);
  ssid = prefs.getString("ssid", "");
  password = prefs.getString("password", "");
  prefs.end();
  WiFi.mode(WIFI_STA);
  if (ssid.length()) {
    WiFi.begin(ssid.c_str(), password.c_str());
    unsigned long start=millis();
    while(WiFi.status()!=WL_CONNECTED && millis()-start<12000) delay(100);
  }
  connected = WiFi.status()==WL_CONNECTED;
  if (connected) {
    server.on("/", HTTP_GET, [](){server.send(200,"text/plain","M5Paper MultiPanel - development preview");});
    server.begin();
  }
  draw();
}
void loop() {
  M5.update();
  if (connected) server.handleClient();
  auto t=M5.Touch.getDetail();
  if(t.wasPressed()) {
    page=(page+(t.x<M5.Display.width()/2 ? 7 : 1))%8;
    draw();
  }
  delay(30);
}
