# ArdSerialXPlaneUDP
It allows to connect arduino with the x-plane through serial communication and the udp protocol in a bidirectional way.

### Quick Start
1. Purchase and install X-Plane 9, 10 or 11.
2. Download the `XPlaneConnect.zip` file from the latest release on the [releases](https://github.com/nasa/XPlaneConnect/releases) page.
3. Copy the contents of the .zip archive to the plugin directory (`[X-Plane Directory]/Resources/plugins/`)
4. Look at the example code in the arduino folder, you will have to load this code on your arduino board. 
5. Keep your arduino board connected to a serial port
6. Start a flight on the x-plane simulator
7. Launch `python3 service/init.py --port COM3 --baud 9600`

Arduino hardware and X-Plane are connected.

[![Alt text](https://img.youtube.com/vi/1boIcYFqCek/0.jpg)](https://www.youtube.com/watch?v=1boIcYFqCek)

#### Notices

[**XPlaneConnect**](https://github.com/nasa/XPlaneConnect/blob/master/license.pdf)

Copyright (c) 2013-2018 United States Government as represented by the Administrator of the
National Aeronautics and Space Administration. All Rights Reserved.
