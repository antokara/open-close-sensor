# open close sensor

An open/close sensor for windows/doors/etc. using the TMAG5273 Linear 3D Hall-Effect Sensor and the Seeed Studio XIAO SAMD21.

It features anti-tampering detection from external magnetic fields or removing of its cover.

## Compatibility

- sliding/hinged/etc. doors/windows/etc.
- plastic/wood/glass/aluminum/composite material door/windows/frames/etc.
- digital output pin
- 3.7V to 5V
- 50 mA

## calibration mode

1. close the door/window
2. install the sensor on the door/window frame (stationary part)
    1. remove its cover (this will trigger a tamper alert pulse, on the digital output pin)
3. install the magnet on the door/window (moving part)
4. press & hold the calibration button for 10 seconds, until the blue LED turns on
    1. release the button. The blue LED will remain on
5. open the door/window all the way
    1. the yellow LED should turn on and stay on
6. close the door/window all the way
    1. the yellow LED should turn of and stay off
7. optional: repeat steps 5-6 for additional calibration
8. press the calibration button to finish
    1. the blue LED should turn off