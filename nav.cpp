#include "nav.h"
#include <math.h>
#include <stdio.h>
#include "termbox/src/termbox.h"

namespace math {
	float rad2deg(float rad) {
		return (180.0 * rad / (M_PI));
	}

	float deg2rad(float deg) {
		return (M_PI * deg / 180.0);
	}

	inline float sq(float n) {
		return n * n;
	}
}

namespace nav {
	float distance_between(float lat1, float lon1, float lat2, float lon2) {
		// returns distance in meters between two positions, both specified
		// as signed decimal-degrees latitude and longitude. Uses great-circle
		// distance computation for a hypothised sphere of radius 6372795 meters.
		float delta = math::deg2rad(lon1-lon2);
		float sdlon = sin(delta);
		float cdlon = cos(delta);
		lat1 = math::deg2rad(lat1);
		lat2 = math::deg2rad(lat2);
		float slat1 = sin(lat1);
		float clat1 = cos(lat1);
		float slat2 = sin(lat2);
		float clat2 = cos(lat2);
		delta = (clat1 * slat2) - (slat1 * clat2 * cdlon);
		delta = math::sq(delta);
		delta += math::sq(clat2 * sdlon);
		delta = sqrt(delta);
		float denom = (slat1 * slat2) + (clat1 * clat2 * cdlon);
		delta = atan2(delta, denom);
		return delta * 6372795.0; 
	}

	float heading_fromto(float lat1, float lon1, float lat2, float lon2) {
		// returns heading in degrees (North=0, West=270) from
		// position 1 to position 2, both specified as signed decimal-degrees
		// latitude and longitude.
		float dlon = math::deg2rad(lon2-lon1);
		lat1 = math::deg2rad(lat1);
		lat2 = math::deg2rad(lat2);
		float a1 = sin(dlon) * cos(lat2);
		float a2 = sin(lat1) * cos(lat2) * cos(dlon);
		a2 = cos(lat1) * sin(lat2) - a2;
		a2 = atan2(a1, a2);
		if (a2 < 0.0) {
			a2 += M_PI*2.0;			// poor man's fmod() 
		}
		return math::rad2deg(a2);
	}
}

void print_tb(const char *str, int x, int y, uint16_t fg, uint16_t bg)
{
	while (*str) {
		uint32_t uni;
		str += tb_utf8_char_to_unicode(&uni, str);
		tb_change_cell(x, y, uni, fg, bg);
		x++;
	}
}

void printf_tb(int x, int y, uint16_t fg, uint16_t bg, const char *fmt, ...)
{
	char buf[4096];
	va_list vl;
	va_start(vl, fmt);
	vsnprintf(buf, sizeof(buf), fmt, vl);
	va_end(vl);
	print_tb(buf, x, y, fg, bg);
}


void draw_screen() {
	tb_clear();

	float or_lat =  51.197487;
	float or_lon = 6.436220; 

	float dest_lat = 51.20248;
	float dest_lon = 6.410682;

	float dist = nav::distance_between(or_lat, or_lon, dest_lat, dest_lon);
	float head = nav::heading_fromto(or_lat, or_lon, dest_lat, dest_lon);

//	printf("dist: %f\n", dist);
//	printf("head: %f\n", head);

	print_tb("Press <ESC> or <q> to exit", 0, 0, TB_WHITE, TB_DEFAULT);
	printf_tb(4,2, TB_GREEN, TB_DEFAULT, "dist: %f", dist);
	printf_tb(4,3, TB_GREEN, TB_DEFAULT, "head:  %f", head);

	tb_present();
}

int main(int argc, char **argv) {
	int ret = tb_init();
	if (ret) {
		fprintf(stderr, "tb_init() failed with error code %d\n", ret);
		return 1;
	}

	draw_screen();

	tb_event ev;
	bool cont = true;
	while (cont && tb_poll_event(&ev)) {
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
				case TB_EVENT_RESIZE:
					draw_screen();
					break;
			}
	}

	tb_shutdown();
}

