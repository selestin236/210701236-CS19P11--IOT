#define DIGITAL_PIN 2  
#define LED_PIN 13    
#include <DHT.h>

#define DHT_PIN 15    // Pin connected to the DHT sensor
#define DHT_TYPE DHT11 
DHT dht(DHT_PIN, DHT_TYPE); // Type of DHT sensor, DHT11 or DHT22
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Mi 11X";
const char* password = "pravimi11x";
const char* botToken = "7056168314:AAEPJKB_ykzJ9DG2avSFgjAyt_GKZSM0Rgs";
const char* chatId = "1859638363";

void setup() {
  Serial.begin(115200); //baud
  pinMode(DIGITAL_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  dht.begin(); // Initialize DHT sensor

  // Connect to Wi-Fi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  // Read digital value from sensor
  int sensorValue = digitalRead(DIGITAL_PIN);

  // Read temperature value in Celsius
  float temperature = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(temperature)) {
    Serial.println("Failed to read temperature from DHT sensor!");
    delay(2000);
    return;
  }

  // Print temperature value to serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  // Print sensor value
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);

  // Check if sensor value is 1 or temperature is greater than 30°C
  if (sensorValue >= 1 || temperature >= 34) {
    // Send alert message to Telegram
    sendTelegramMessage("Emergency Send the Rescue Team ");
  }

  delay(5000); // Wait for 5 seconds before taking next sensor reading
}

void sendTelegramMessage(String message) {
  HTTPClient http;

  // Compose the URL for the Telegram Bot API
  String url = "https://api.telegram.org/bot" + String(botToken) + "/sendMessage";

  // Compose the JSON payload
  String payload = "{\"chat_id\":\"" + String(chatId) + "\",\"text\":\"" + message + "\"}";

  // Make POST request to the Telegram Bot API
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(payload);

  // Check for response
  if (httpResponseCode > 0) {
    Serial.print("Telegram message sent. Response code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("Error sending message. HTTP error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}
