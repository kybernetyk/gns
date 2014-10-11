#include "nmea0183.h"
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <math.h>
#include <fstream>

namespace str {
	std::vector<std::string> &split(const std::string &s, const char delim, std::vector<std::string> &outVec) {
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			outVec.push_back(item);
		}
		return outVec;
	}
	
	std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		split(s, delim, elems);
		return elems;
	}
}

namespace nmea {
	bool packetHasCoords(const Packet &p) {
			return p.type == Packet::Type::GPGGA ||
	   				 p.type == Packet::Type::GPRMC;
	}

	bool packetHasUTC(const Packet &p) {
		return p.type == Packet::Type::GPGGA ||
					 p.type == Packet::Type::GPRMC;
	}

	bool packetHasHeading(const Packet &p) {
		return p.type == Packet::Type::GPRMC;
	}

	bool packetHasSpeed(const Packet &p) {
		return packetHasHeading(p);
	}

	std::vector<std::string> tokenizeSentence(const std::string &sentence) {
		return str::split(sentence, ',');
	}

	Packet::Type typeFromString(const std::string &s) {
		if (s == "$GPGGA") {
			return Packet::Type::GPGGA;
		}

		if (s == "$GPRMC") {
			return Packet::Type::GPRMC;
		}

		return Packet::Type::UNKNOWN;
	}

	std::string strFromType(Packet::Type type) {
		//oh well ... initializer lists seem to kill my compiler here
		std::map<Packet::Type, std::string> m;
		m[Packet::Type::GPGGA] = "GPGGA";
		m[Packet::Type::GPRMC] = "GPRMC";
		m[Packet::Type::UNKNOWN] = "UNKNOWN";
		return m.at(type);
	}

	void fillPacket_GPRMC(const std::vector<std::string> &v, Packet &p) {
		if (v.size() < 9) {
			return;
		}
		p.utc_timestamp = v[1]; 
		p.coords.lat = atof(v[3].c_str());
		p.coords.lat = p.coords.lat / 100.0;
		float degs = floor(p.coords.lat);
		p.coords.lat = (100.0 * (p.coords.lat - degs)) / 60.0;
		p.coords.lat += degs;
		if (v[4] == "S") {
			p.coords.lat = 0.0 - p.coords.lat;
		}
		
		p.coords.lon = atof(v[5].c_str());
		p.coords.lon = p.coords.lon / 100.0;
		degs = floor(p.coords.lon);
		p.coords.lon = (100.0 * (p.coords.lon - degs)) / 60.0;
		p.coords.lon += degs;
		if (v[6] == "W") {
			p.coords.lon = 0.0 - p.coords.lon;
		}

		p.speed = atof(v[7].c_str());
		p.heading = atof(v[8].c_str());
	}

	Packet makePacket(const std::vector<std::string> &v) {
		Packet p;
		p.type = typeFromString(v[0]);
		switch (p.type){
		case Packet::Type::GPRMC:
			fillPacket_GPRMC(v, p);
			break;
		case Packet::Type::UNKNOWN:
		default:
			//abort();
			break;
		}
		return p;
	}

	Packet parseSentence(const std::string &sentence) {
		auto elems = tokenizeSentence(sentence);
		auto p = makePacket(elems);
		return p;
	}
	
	void printPacket(const Packet &p) {
		//ugly hack :3
		const Packet *pp = &p;
		printf("Nmea Packet @ %p:\n", (void*)pp);
		printf("\tType: %s\n", strFromType(p.type).c_str());
		if (packetHasCoords(p)) {
			printf("\tLat: %f\n\tLon: %f\n", p.coords.lat, p.coords.lon);
		}
		if (packetHasHeading(p)) {
			printf("\tHeading: %f\n", p.heading);
		}
		if (packetHasSpeed(p)) {
			printf("\tSpeed: %f\n", p.speed);
		}
		if (packetHasUTC(p)) {
			printf("\tUTC: %s\n", p.utc_timestamp.c_str());
		}
	}

}
#if 0
int main(int argc, char *argv[]) {
//	auto p = nmea::parseSentence("$GPRMC,171804.000,A,5111.8502,N,00626.1751,E,0.07,46.02,280913,,,A*58");
//	auto p = nmea::parseSentence("$GPGGA,082804.683,5205.9421,N,00506.4368,E,1,03,3.0,0.3,M,,,,0000*01");
//	nmea::printPacket(p);
	std::ifstream infile("/dev/ttyUSB0");
	std::string line;
	while (std::getline(infile, line)) {
		if (line[0] != '$') {
			continue;
		}
		auto p = nmea::parseSentence(line);
		if (p.type == nmea::Packet::Type::GPRMC) {
			printf("%s\n", line.c_str());
			nmea::printPacket(p);
		}
	}
	return 0;
}
#endif
