#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

#define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
#define PMTK_SET_NMEA_UPDATE_1HZ  "$PMTK220,1000*1F"
#define PMTK_API_SET_FIX_CTL_1HZ  "$PMTK300,1000,0,0,0,0*1C"
#define PGCMD_ANTENNA "$PGCMD,33,1*6C" 

SoftwareSerial serialPort(5,13);
String ch;

const char* ssid = "Giru Giru";
const char* password = "Mc6785608359";
const char* host = "192.168.0.107";
const int httpPort = 222;

WiFiClient client;

void gpsInit();

void setup() {
  // put your setup code here, to run once:
  serialPort.begin(115200);
  Serial.begin(115200);
  Serial.println("Begin software serial");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected to WiFi");
  gpsInit();
}

void loop() {
  //if (!client.connect(host, httpPort)) {
  //  return;
  //  }
  while (serialPort.available() > 0)
  {
    ch = serialPort.readStringUntil('\n');
    Serial.print("[");
    Serial.print(ch);
    Serial.println("]");
    //client.print(ch);
  }
  //delay(500);
}

void gpsInit() {
  serialPort.begin(9600);
  serialPort.println(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  serialPort.println(PMTK_SET_NMEA_UPDATE_1HZ);
  serialPort.println(PMTK_API_SET_FIX_CTL_1HZ);
  serialPort.println(PGCMD_ANTENNA);
  delay(500);
}

