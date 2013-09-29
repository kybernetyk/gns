#pragma once
#include <string>
#include <thread>
#include <sstream>
#include <fstream>
#include "nmea0183.h"

namespace nmea {
	class Device {
	public:
		bool initWithPathAndPreferredPacketType(std::string path, nmea::Packet::Type preferredPacketType);
		
		//this will block until a packet of the preferred type is read!
		//preferrably call this from a seperate thread
		void update();
		
		nmea::Packet mostCurrentPacket();
	
	private:
		nmea::Packet m_cachedPacket;
		nmea::Packet::Type m_preferredPacketType;
		std::mutex m_lock;
		std::ifstream m_ifstream;
	};
	
}
