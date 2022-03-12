# Manually set the ESP32 Real Time Clock (RTC)

If you want to use the RTC of the ESP 32 but have no WiFi for NTP sync and atomic clock accuracy isn't critical this sketch has the basics. 
SP32Time library by Felix Biego https://github.com/fbiego/ESP32Time

### Important to note that serial monitor input syntax must be exact or it will fail. 
The format is presented when you press the Boot / Flash button (GPIO 0)
The onboard Blue LED will go from Solid to Blinking when waiting for time input

## Example of what you will see

![manual_set_rtc](https://user-images.githubusercontent.com/20883620/158036551-767eb588-d7ba-4e17-8013-64561e4add74.jpg)
