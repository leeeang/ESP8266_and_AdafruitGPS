#include <ESP8266WiFi.h>
#include <Adafruit_GPS.h>

Adafruit_GPS GPSModule(&Serial1);

const char* ssid = "Giru Giru";
const char* password = "Mc6785608359";
const char* host = "192.168.0.107";
const int httpPort = 222;
char GPS_coordinate;

const byte ledPin = LED_BUILTIN;
volatile byte state = LOW;

void blinking();
void GPS_commands();

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  //Serial.begin(115200);

  //------------------------------------------------------------------
  // interrupt sequences
  noInterrupts();       //
  timer0_isr_init();    //initializing isr
  timer0_attachInterrupt(blinking);   //calling blinking isr function
  timer0_write(ESP.getCycleCount() + 40000000L);  //increment by 80MHz
  interrupts();
  //------------------------------------------------------------------

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //Serial.print("Connected to WiFi");
  GPS_commands();
}

void loop() {
  WiFiClient client;
  if (!client.connect(host, httpPort)) {
    //Serial.println("Connection failed");
    return;
  }
  //client.print("Print from TCP");
  client.print(GPS_coordinate);
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      //Serial.println("Client timeout");
      client.stop();
      return;
    }
  }
  delay(100);
}

void GPS_commands() {
  GPSModule.begin(9600);
  GPSModule.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPSModule.sendCommand(PMTK_SET_NMEA_UPDATE_5HZ);
  GPSModule.sendCommand(PMTK_API_SET_FIX_CTL_5HZ);
  GPSModule.sendCommand(PGCMD_ANTENNA);
}

void blinking() {
  digitalWrite(LED_BUILTIN, state);
  GPS_coordinate = GPS.read();

  //Serial.println("State: "+(String)state);
  state = !state;
  timer0_write(ESP.getCycleCount() + 40000000L);
}

