#include "termbox/src/termbox.h"
#include "main_screen.h"
#include "tbutil.h"
#include "nav.h"
#include <math.h>
#include <cmath>
#include "audio.h"

extern float g_fps;
#define sgn(x) ((x<0)?-1:((x>0)?1:0)) 

namespace main_screen {
	void coords_on_circle(float deg, float r, float &x, float &y) {
		x = sin(math::deg2rad(deg)) * r;
		x *= 1.75; 

		y = cos(math::deg2rad(deg)) * r;
		y *= -1.0;

			if (x > 0.0) {
				x += 0.5;
			}
			if (x < 0.0) {
				x -= 0.5;
			}
			if (y > 0.0) {
				y += 0.5;	
			}
			if (y < 0.0) {
				y -= 0.5;
			}
			x = (int)x;
		y = (int)y;
	}

	void draw_circle(int center_x, int center_y, int radius) {
		float r = radius;
		float cx = center_x;
		float cy = center_y;
		tb_change_cell(cx, cy, '*', TB_GREEN, TB_DEFAULT);
		for (int theta = 0; theta < 360; theta++) {
			float x, y;
			coords_on_circle(theta, r, x, y);
		int ix = cx + x;
			int iy = cy + y;
			
			tb_change_cell(ix, iy, '*', TB_GREEN, TB_DEFAULT);
		}
	}

	int abs(int x) {
		if (x < 0) {
			return x * -1;
		}
		return x;
	}

void line_slow(int x1, int y1, int x2, int y2, uint16_t col) {
  int dx,dy,sdx,sdy,px,py,dxabs,dyabs,i;
  float slope;

  dx=x2-x1;      /* the horizontal distance of the line */
  dy=y2-y1;      /* the vertical distance of the line */
  dxabs=abs(dx);
  dyabs=abs(dy);
  sdx=sgn(dx);
  sdy=sgn(dy);
  if (dxabs>=dyabs) /* the line is more horizontal than vertical */
  {
    slope=(float)dy / (float)dx;
    for(i=0;i!=dx;i+=sdx)
    {
      px=i+x1;
      py=slope*i+y1;
			tb_change_cell(px, py, '*', col, TB_DEFAULT);
    }
  }
  else /* the line is more vertical than horizontal */
  {
    slope=(float)dx / (float)dy;
    for(i=0;i!=dy;i+=sdy)
    {
      px=slope*i+x1;
      py=i+y1;
			tb_change_cell(px, py, '*', col, TB_DEFAULT);
    }
  }
}

	void draw_line_to_heading(int cx, int cy, int r, uint16_t col, float heading) {
		float x, y;
		coords_on_circle(heading, r, x, y);
		line_slow(cx,cy, cx + x, cy + y, col); 
	}

	void draw_rotating_compass(float cur_heading, float desired_heading) {
		float x = 24;
		float y = 30;
		float r = 12;

		draw_circle(x, y, r);
		print_tb("--v--", x-2, y - r - 1, TB_BLACK, TB_WHITE);

		//'rotate' the compass
		float diff = 0.0 - cur_heading;

		float lx, ly;
		coords_on_circle(0.0 + diff, r, lx, ly);
		tb_change_cell(x + lx, y + ly, 'N', TB_WHITE, TB_DEFAULT);
		coords_on_circle(90.0 + diff, r, lx, ly);
		tb_change_cell(x + lx, y + ly, 'E', TB_WHITE, TB_DEFAULT);
		coords_on_circle(180.0 + diff, r, lx, ly);
		tb_change_cell(x + lx, y + ly, 'S', TB_WHITE, TB_DEFAULT);
		coords_on_circle(270.0 + diff, r, lx, ly);
		tb_change_cell(x + lx, y + ly, 'W', TB_WHITE, TB_DEFAULT);

		desired_heading += diff;
		cur_heading += diff;

		draw_line_to_heading(x, y, r, TB_YELLOW, desired_heading) ;
//		coords_on_circle(desired_heading, r, lx, ly);
//		tb_change_cell(x + lx, y + ly, '+', TB_YELLOW, TB_RED);
	}

	void draw_rotating_compass2(float cur_heading, float desired_heading) {
		float x = 24;
		float y = 30;
		float r = 12;

		draw_circle(x, y, r);
//		print_tb("--v--", x-2, y - r - 1, TB_BLACK, TB_WHITE);

		//'rotate' the compass
		float diff = 0.0 - cur_heading;

		float lx, ly;
		coords_on_circle(0.0 + diff, r, lx, ly);
		tb_change_cell(x + lx, y + ly, 'N', TB_WHITE, TB_DEFAULT);
		coords_on_circle(90.0 + diff, r, lx, ly);
		tb_change_cell(x + lx, y + ly, 'E', TB_WHITE, TB_DEFAULT);
		coords_on_circle(180.0 + diff, r, lx, ly);
		tb_change_cell(x + lx, y + ly, 'S', TB_WHITE, TB_DEFAULT);
		coords_on_circle(270.0 + diff, r, lx, ly);
		tb_change_cell(x + lx, y + ly, 'W', TB_WHITE, TB_DEFAULT);

		desired_heading += diff;
		cur_heading += diff;

		draw_line_to_heading(x, y, r, TB_YELLOW, cur_heading) ;
		coords_on_circle(desired_heading, r, lx, ly);
		tb_change_cell(x + lx, y + ly, '+', TB_YELLOW, TB_RED);
		print_tb("DEST", x + lx, y + ly, TB_BLACK, TB_WHITE);
	}

	void draw_compass(float cur_heading, float desired_heading) {
		float x = 70;
		float y = 30;
		float r = 12;
		draw_circle(x, y, r);

		float lx, ly;
		coords_on_circle(0.0, r, lx, ly);
		tb_change_cell(x + lx, y+ ly, 'N', TB_WHITE, TB_DEFAULT);
		coords_on_circle(90.0, r, lx, ly);
		tb_change_cell(x + lx, y + ly, 'E', TB_WHITE, TB_DEFAULT);
		coords_on_circle(180.0, r, lx, ly);
		tb_change_cell(x + lx, y + ly, 'S', TB_WHITE, TB_DEFAULT);
		coords_on_circle(270.0, r, lx, ly);
		tb_change_cell(x + lx, y + ly, 'W', TB_WHITE, TB_DEFAULT);


		draw_line_to_heading(x, y, r, TB_YELLOW, cur_heading) ;
		coords_on_circle(desired_heading, r, lx, ly);
//		tb_change_cell(x + lx, y + ly, '+', TB_YELLOW, TB_RED);
		print_tb("DEST", x + lx, y + ly, TB_BLACK, TB_WHITE);
	}

	void play_sound(float cur_head, float desired_head) {
			//lol who needs math if one can have if?
			//calc a difference that is 0..180 
			float diff = 0.0 - desired_head;
			cur_head += diff;
			if (cur_head < 0.0) {
				cur_head = 360 + cur_head;
			}
			if (cur_head > 180) {
				cur_head = fabs(cur_head - 360);
			}


			if (cur_head < 20) {
				audio::play("beep.wav");
				return;
			}
			if (cur_head < 60) {
				audio::play("ping.wav");
				return;
			}

			if (cur_head > 150) {
				audio::play("bzz.wav");
				return;
			}
			if (cur_head > 120) {
				audio::play("bzz2.wav");
				return;
			}
			//printf_tb(10, 10, TB_WHITE, TB_DEFAULT, "CUR_H: %f", cur_head);
	}

	int h = 0;
	void Screen::draw() {
		tb_clear();
/*		m_heading = h--;
		if (h < 0) {
			h = 359;
		}
*/
		float or_lat = m_location.lat;
		float or_lon = m_location.lon;

		float dest_lat = m_destination.lat; 
		float dest_lon = m_destination.lon; 

		float dist = nav::distance_between(or_lat, or_lon, dest_lat, dest_lon);
		float desired_head = nav::heading_fromto(or_lat, or_lon, dest_lat, dest_lon);

		m_distanceTraveled += fabs(m_lastDistance - dist);
		m_lastDistance = dist;

		bool may_sound = false;
		if (fabs(m_lastHeading - m_heading) > 20.0 ||
				m_distanceTraveled > 20.0) {
			m_lastHeading = m_heading;
			m_distanceTraveled = 0.0;
			may_sound = true;
		}

		print_tb("Press <ESC> or <q> to exit", 0, 1, TB_WHITE, TB_DEFAULT);
		printf_tb(4,2, TB_GREEN, TB_DEFAULT, "curr lat: %f lon: %f", or_lat, or_lon);
		printf_tb(4,3, TB_GREEN, TB_DEFAULT, "dest lat: %f lon: %f", dest_lat, dest_lon);
		printf_tb(4,5, TB_GREEN, TB_DEFAULT, "dist: %f meters", dist);
		printf_tb(4,8, TB_GREEN, TB_DEFAULT, "current head: %f", m_heading);
		printf_tb(4,9, TB_GREEN, TB_DEFAULT, "desired head: %f", desired_head);
		printf_tb(4,11, TB_GREEN, TB_DEFAULT, "current speed: %f knots", m_speed);
		printf_tb(0,0, TB_YELLOW, TB_DEFAULT, "fps: %f", g_fps);


		if (may_sound) {
			play_sound(m_heading, desired_head);
		}


		draw_rotating_compass2(m_heading, desired_head);
		draw_compass(m_heading, desired_head);

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
