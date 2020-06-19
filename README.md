# TTL_sensors-on-ESPHome
customized sensors on ESP Home, triggered by TTL messages of Xiaomi BT gateway.
## Thanks to:
https://github.com/nldroid/CustomP1UartComponent
https://esphome.io/custom/uart.html
## Howto
1. edit the .h file; 
 I have one of Zelkova K1 smart-lock, one of BT temp&humi 2, and two of Flora Sensors (HHCCJCY01). you should modify the codes according to your configurations.
2. edit yaml
 match the content of .h file.
3. upload

## Tested on ESP32 & ESP8266
## Known issues:
- '[E][uart:096]: Reading from UART timed out at byte 0!' on every ttl message.
- timestamp not converted to readable format.
- keyid not converted to readable format.
