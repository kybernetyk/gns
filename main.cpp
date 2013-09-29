#include "termbox/src/termbox.h"
#include "main_screen.h"
#include <time.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/stat.h>

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
	int ret = tb_init();
	if (ret) {
		fprintf(stderr, "tb_init() failed with error code %d\n", ret);
		return 1;
	}
	main_screen::Screen ms;

	ms.draw();

	tb_event ev;
	bool cont = true;

	float fps = 10;
	long delay_millisecs = (1.0 / fps) * 1000;
	long delay_microsecs = delay_millisecs * 1000;
	
	timeval time_diff;
	timeval timer;
	timeval last_timer;
	gettimeofday(&last_timer, NULL);
	
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
					ms.handleEvent(ev);
					break;
				default:
					ms.handleEvent(ev);
					break;
			}
		}

		gettimeofday(&timer, NULL);
		time_diff = timeval_subtract(timer, last_timer);
		if ((time_diff.tv_usec + time_diff.tv_sec * 1e6) >= delay_microsecs) { 			
			gettimeofday(&last_timer, NULL);
			g_fps = (1.0 / (time_diff.tv_usec + time_diff.tv_sec * 1e6)) * 1e6;
			ms.draw();
		}
	}

	tb_shutdown();
}

