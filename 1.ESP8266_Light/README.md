# ESP8266 Light
## Description
Use ESP-01 module to control led through MQTT protocol.

![image](https://github.com/marcusj1248/HomeAssistantProject/blob/main/1.ESP8266_Light/ESP8266_Light.jpg)
## Home Assistant Configuration.yaml
```
switch:
  - platform: mqtt
    name: "LED Light"
    icon: mdi:lightbulb-outline
    command_topic: "ha/bedroom/ledlight/switch"
    state_topic: "ha/bedroom/ledlight/status"
    qos: 1
    payload_on: "1"
    payload_off: "0"
    state_on: "1"
    state_off: "0"
    optimistic: false
    retain: true
```