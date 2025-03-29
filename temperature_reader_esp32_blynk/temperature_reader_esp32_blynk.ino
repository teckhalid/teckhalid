#define BLYNK_TEMPLATE_ID "Your_Template_ID"
#define BLYNK_TEMPLATE_NAME "Esp32 temperature app"
#define BLYNK_AUTH_TOKEN "Your_Blynk_Auth_Token"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

// WiFi Credentials (Replace with your WiFi details)
const char *ssid = "Your_WiFi_SSID";      
const char *password = "Your_WiFi_Password";  

#define DHTPIN 4  // GPIO pin where DHT11 is connected
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

float tempThreshold = 39.0;  // Set temperature threshold
bool alertSent = false;  // Prevents multiple alerts

void sendTemperature() {
  float temp = dht.readTemperature();  // Read temperature
  
  if (!isnan(temp)) {
    Blynk.virtualWrite(V0, temp);  // Send temperature to Blynk
    
    // Check if temperature exceeds the threshold
    if (temp > tempThreshold && !alertSent) {
      Blynk.logEvent("high_temperature", String("High Temperature: ") + temp + "°C");
      alertSent = true;  // Prevent multiple alerts
    } 
    else if (temp <= tempThreshold) {
      alertSent = false;  // Reset alert when temperature drops
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Connecting to WiFi...");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);

  Serial.println("Connected to WiFi!");
  Serial.println("Connecting to Blynk...");

  while (!Blynk.connected()) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nConnected to Blynk!");
  dht.begin();
  timer.setInterval(2000L, sendTemperature);  // Check temperature every 2 seconds
} 

void loop() {
  Blynk.run();
  timer.run();
}
