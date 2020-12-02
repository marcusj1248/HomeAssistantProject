# ESP8266 DHT Sensor
## Description
Use ESP-01 module to read and publish humility and temperature form a DHT22 sensor through MQTT protocol.

![image](https://github.com/marcusj1248/HomeAssistantProject/blob/main/2.ESP8266_DHTSensor/ESP8266_DHTSensor.jpg)

## Home Assistant Configuration.yaml
```
sensor:
  - platform: mqtt
    name: "Temperature"
    state_topic: "ha/bedroom/HTSensor"
    unit_of_measurement: '°C'
    value_template: "{{ value_json.temperature }}"
  - platform: mqtt
    name: "Humidity"
    state_topic: "ha/bedroom/HTSensor"
    unit_of_measurement: '%'
    value_template: "{{ value_json.humidity }}"
```