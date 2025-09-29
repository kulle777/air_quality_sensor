# air_quality_sensor
LoF project at Wapice.

To connect this Iot device to Wifi:
1) Power it
2) It will produce a wifi hotspot that you connect to and setup wifi
3) The device will remain in that wifi until it cant get that wifi. In that case it will make a hotspot again.

What you have to setup:
in the file /main/config.h.example
write your adafruitIO keys.
Remove .example from filename so it becomes config.h
