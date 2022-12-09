My goal was to create a means around the use of elevated privileges to transform  a bike with a speed sensor, held by a trainer stand, into a joystick (with throttle axis).  Previously, I wrote Linux software that required the use of “sudo” (elevated privileges) to do the same.   I think one benefits of my venture is that Windows 10 users may be able to use the software I authored with their bike trainers.  I only have a laptop with Windows 8.  (I am not enthusiastic about procuring a Windows 10 computer. The Uno Wifi R2 does not work with Windows 8)  I will readily admit that riding a bike trainer is boring.  Also, I think, playing flight simulators [i. e. FlightGear] for an  hour straight is also boring.  Combining them makes a palatable past time for me.  [I join the ranks of one-time Ohio residents who were unlicensed pilots who enjoy cycling.  I enjoy planning the next days workout and the variability of that work out from real-world weather conditions.]

To use the software, you need the following: 
	Leonardo Arduino
	Uno Wifi R2  Arduino
 	Speed sensor mounted to the rear wheel of your bicycle (I use a Wahoo Speed Sensor)
	Bike
	Trainer Stand
 	Computer with monitor equipped close to your bike on a trainer stand
	3 jumper wires  to conned the SDA, SCL, GND pins of the Arduinos
	mouse and keyboard you can operate from your bicycle trainer
	USB cables
	Sweat guards for your bike (optional)

This is a long and expensive list of equipment; but I do not know anyone that does not ride a bike trainer in front a monitor.  (My Mom once watched a monitor while walking on a tread mill in the basement.)  The next most expensive thing is a computer.  I conveniently located my desktop computer adjacent to that monitor, close to my bike trainer stand.

I used my knowledge of thermodynamics (and help from a high school junior to solve a differential equation) to account for  the lost resistance of the trainer stand due to use from a bike.  (Imagine a trainer stand as a tank of water – the water level represents its resistance (or lack of) of the trainer.  That tank includes a small hole.  Riding the bike on the trainer stand is like pouring water back into that tank with its hole.  Eventually the water level will be constant.)  

If you opt us my software, I would enjoy hearing from you.   I think the best way to get in touch with me through forum.flightgear.org or by commenting on my github.com posting.  I endured a mild diffuse axonal injury whle cycling in Ohio, so my response may not be quick.   If you also have a traumatic brain injury, feel free to comment about that also.


Leonardo: “https://www.amazon.com/KEYESTUDIO-Leonardo-Development-Board-Arduino/dp/B0786LJQ8K/ref=sr_1_1_sspa?crid=1QHUY74RYKTIJ&keywords=Leonardo+Arduino&qid=1670592791&sprefix=leonardo+arduino%2Caps%2C74&sr=8-1-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEzTVA4MjRUVUlBVUlWJmVuY3J5cHRlZElkPUEwNTkwNjQzM0VIMUpCMTZJVkUxQSZlbmNyeXB0ZWRBZElkPUEwNzUzNjA4MzVQVDRQRjcySkg5QiZ3aWRnZXROYW1lPXNwX2F0ZiZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU=”

Uno Wifi R2: “https://www.amazon.com/Arduino-UNO-WiFi-REV2-ABX00021/dp/B07MK598QV/ref=sr_1_2?crid=9XDCT4RNWX3Z&keywords=uno+wifi+rv2+Arduino&qid=1670592855&sprefix=uno+wifi+rv2+arduino%2Caps%2C65&sr=8-2”

Wahoo Speed Sensor: “https://www.amazon.com/Wahoo-Cycling-Cadence-Sensor-Bluetooth/dp/B01DIE7LUG/ref=sr_1_1_sspa?crid=2RSJPNBK0Q5Y7&keywords=wahoo%2Bspeed%2Bsensor&qid=1670592911&sprefix=wahoo%2Bspeed%2Bsensor%2Caps%2C102&sr=8-1-spons&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUE1VTRMTFFGUU5SRFEmZW5jcnlwdGVkSWQ9QTA2OTQxMzdHN1FCU1ZDUUkwUUMmZW5jcnlwdGVkQWRJZD1BMDM4Mzg2OVdYVzNPWkVBRFQzMCZ3aWRnZXROYW1lPXNwX2F0ZiZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU&th=1”





