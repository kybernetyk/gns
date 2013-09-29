#include "termbox/src/termbox.h"
#include "main_screen.h"
#include "tbutil.h"
#include "nav.h"

extern float g_fps;

namespace main_screen {
	void Screen::draw() {
		tb_clear();

		float or_lat = m_location.lat;
		float or_lon = m_location.lon;

		float dest_lat = m_destination.lat; 
		float dest_lon = m_destination.lon; 

		float dist = nav::distance_between(or_lat, or_lon, dest_lat, dest_lon);
		float head = nav::heading_fromto(or_lat, or_lon, dest_lat, dest_lon);

		print_tb("Press <ESC> or <q> to exit", 0, 0, TB_WHITE, TB_DEFAULT);
		printf_tb(4,2, TB_GREEN, TB_DEFAULT, "dist: %f", dist);
		printf_tb(4,3, TB_GREEN, TB_DEFAULT, "head:  %f", head);

		printf_tb(4,20, TB_YELLOW, TB_DEFAULT, "fps: %f", g_fps);

		tb_present();
	}

	void Screen::handleEvent(tb_event event) {
		switch (event.type) {
				case TB_EVENT_RESIZE:
					draw();
					break;
		}
	}
}
