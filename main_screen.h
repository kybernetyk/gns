#pragma once
#include "nmea0183.h"
#include "audio.h"
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

		void setHeading(float deg) {
			m_heading = deg;
		}

		void setSpeed(float speed) {
			m_speed = speed;
		}
	protected:
		void play_sound(float cur_head, float desired_head);

	private:
		audio::Player m_player;
		nmea::GeoCoords m_location;
		float m_heading;
		float m_lastHeading = 0.0;
		float m_speed;
		float m_lastDistance = 0.0;
		float m_distanceTravelled = 0.0; //for signal sound playback
		float m_distanceTravelledTotal = 0.0f; //total distance travelled this session
		nmea::GeoCoords m_destination;
	};

};
