# RPM-meter

Before the code can be used the CAn library and LCD library need to be installed in the Arduino IDE.
The MCP2515\_DEFAULT\_CLOCK\_FREQUENCY in the MCP2515.h file needs to be changed to $8e6$ instead of the default $16e6$. This file can be found in the location of where the library is installed -> CAN -> src (in my case this is under Documents/Arduino/libraries/CAN/src/). This is the clock speed of the CAN bus transceiver, which is 8 MHz. 

The wiring layout can be found here: https://www.tinkercad.com/things/9uQwcti0zVH?sharecode=c2LFVm6MtAcUe3TGwNbU21Kxy9My0ZWiBvwphViZ2Ck
