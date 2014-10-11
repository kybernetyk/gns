#include "termbox/src/termbox.h"
#include "main_screen.h"
#include <time.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include "nmea_device.h"
#include "audio.h"
#include "config.h"

float g_fps = 0.0;

timeval timeval_subtract (timeval x, timeval y) {
	if (x.tv_usec < y.tv_usec) {
			 int nsec = (y.tv_usec - x.tv_usec) / 1000000 + 1;
			 y.tv_usec -= 1000000 * nsec;
			 y.tv_sec += nsec;
	 }
	 if (x.tv_usec - y.tv_usec > 1000000) {
			 int nsec = (x.tv_usec - y.tv_usec) / 1000000;
			 y.tv_usec += 1000000 * nsec;
			 y.tv_sec -= nsec;
	 }
	
	 timeval result;
		 /* Compute the time remaining to wait.
				tv_usec is certainly positive. */
	 result.tv_sec = x.tv_sec - y.tv_sec;
	 result.tv_usec = x.tv_usec - y.tv_usec;
	 return result;
}

int main(int argc, char **argv) {
	nmea::Device gps_device;
	if (!gps_device.initWithPathAndPreferredPacketType(DEVICE_PATH, nmea::Packet::Type::GPRMC)) {
		fprintf(stderr, "couldn't init %s!\nrun as root plox\n", DEVICE_PATH);
		//return 23;
	}

	int ret = tb_init();
	if (ret) {
		fprintf(stderr, "tb_init() failed with error code %d\n", ret);
		return 1;
	}
	
	main_screen::Screen ms;
//	ms.setDestination(51.20242,6.410733);
	ms.setDestination(DEST_LAT, DEST_LON);
	ms.draw();

	tb_event ev;
	bool cont = true; //continue execution? 

	//fps
	float fps = 30;
	long delay_millisecs = (1.0 / fps) * 1000;
	long delay_microsecs = delay_millisecs * 1000;

	//timing
	timeval time_diff;
	timeval timer;
	timeval last_timer;
	gettimeofday(&last_timer, NULL);

	std::thread gps_update_thread([&gps_device, &cont]() {
			for (;;) {
				gps_device.update();
				usleep(1000); //this_thread::sleep
				if (!cont) {
					break;
				}
			}
	});
	gps_update_thread.detach();

	while (cont) {
		if (tb_peek_event(&ev, delay_millisecs) > 0) {
			switch (ev.type) {
				case TB_EVENT_KEY:
					switch (ev.key) {
						case TB_KEY_ESC:
							cont = false;
							break;
					}
					switch (ev.ch) {
						case 'q':
							cont = false;
							break;
					}
					break;
				default:
					break;
			}
			ms.handleEvent(ev);
		}

		//update screen if fps threshold reached
		gettimeofday(&timer, NULL);
		time_diff = timeval_subtract(timer, last_timer);
		if ((time_diff.tv_usec + time_diff.tv_sec * 1e6) >= delay_microsecs) { 			
			gettimeofday(&last_timer, NULL);
			g_fps = (1.0 / (time_diff.tv_usec + time_diff.tv_sec * 1e6)) * 1e6;

			auto p = gps_device.mostCurrentPacket().get();
			ms.setLocation(p.coords.lat, p.coords.lon);
			ms.setHeading(p.heading);
			ms.setSpeed(p.speed*0.514444444444f); //convert to m/s
			ms.draw();
		}
	}

	tb_shutdown();
}

