#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
long serial_speed = 115200;
const char* ssid = "your_ssid";
const char* password = "your_password";
const char* mqtt_server = "mqtt_server_ip_address";
int mqtt_port = 1883;
const char* user_name = "mqtt_username"; 
const char* user_password = "mqtt_password";

const char* topic_subscribe = "ha/bedroom/ledlight/switch";
const char* topic_publish = "ha/bedroom/ledlight/status";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[20];
int value = 0;
int ledstate = 0;

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

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Command from MQTT broker is : [");
  Serial.print(topic);
  Serial.print("]");
  int p =(char)payload[0]-'0';
 
  if(p==0) // if get "0" trun off the LED
  {
    digitalWrite(2, LOW); 
    ledstate = 0;
    Serial.println(" Turn Off LED! " );
  } 
  
  if(p==1) // if get "1" trun on the LED
  {
    digitalWrite(2, HIGH); 
    ledstate = 1;
    Serial.println(" Turn On LED! " );
  }
  Serial.println();
} //end callback

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
      client.subscribe(topic_subscribe);
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
  client.setCallback(callback);
  
}
void loop() {
    
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) { // publish status every 2 sec
    lastMsg = now;
    snprintf(msg, sizeof(msg), "%d",ledstate);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(topic_publish, msg);
  }
}
