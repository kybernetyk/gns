#pragma once
#include "nmea0183.h"
#include "termbox/src/termbox.h"

namespace main_screen {
	class Screen {
	public:
		void draw();
		void handleEvent(tb_event event);

		void setDestination(float lat, float lon) {
			m_destination = {.lat = lat, .lon = lon};
		}
		
		void setLocation(float lat, float lon) {
			m_location = {.lat = lat, .lon = lon};
		}
		

	private:
		nmea::GeoCoords m_location;
		nmea::GeoCoords m_destination;
	};

};
