#pragma once
#include <string>

namespace nmea {
	struct GeoCoords {
		float lat;
		float lon;
	};

	struct Packet{
		enum Type {
			UNKNOWN,
			GPRMC,
			GPGGA,

		} type;

		std::string utc_timestamp;

		GeoCoords coords;
		float speed;
		float heading;
	};
	

	bool packetHasHeading(const Packet &p);
	Packet parseSentence(const std::string &sentence);
}
