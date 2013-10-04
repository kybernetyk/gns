#pragma once


#ifdef __APPLE__
	//os x, 2nd usb port
	#define DEVICE_PATH "/dev/cu.PL2303-00001004"
#else
	//linux
	#define DEVICE_PATH "/dev/ttyUSB0"
#endif
	
//destination coordinates
#define DEST_LAT 51.202499f
#define DEST_LON 6.410629f
