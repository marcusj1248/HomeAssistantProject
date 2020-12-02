#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <DHT.h>

#define DHTPIN 0     // Digital pin connected to the DHT sensor
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);

// Update these with values suitable for your network.
long serial_speed = 115200;
const char* ssid = "your_ssid";
const char* password = "your_password";
const char* mqtt_server = "mqtt_server_ip_address";
int mqtt_port = 1883;
const char* user_name = "mqtt_username"; 
const char* user_password = "mqtt_password";

// topic which want to publish
const char* topic_publish = "ha/bedroom/HTSensor";

//Timer timer;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];


void setup_wifi() {
  delay(10);
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW); 
  pinMode(0, INPUT);
  // connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(),user_name,user_password)) {
      Serial.println("connected");
      //client.subscribe(topic_subscribe);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(serial_speed);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  dht.begin();
}
void loop() {
    
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // Check if any reads failed and exit early.
    if (isnan(h) || isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);
    
    snprintf(msg, sizeof(msg), "{\"humidity\":\"%.0f\",\"temperature\":\"%.1f\"}", h, t);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(topic_publish, msg);

    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("°C "));
    Serial.print(F("Heat index: "));
    Serial.print(hic);
    Serial.print(F("°C \n"));

  }
}
