#include "nmea_device.h"
#define DUMMY_DEVICE 1

namespace nmea {
		bool Device::initWithPathAndPreferredPacketType(std::string path, nmea::Packet::Type preferredPacketType) {
			m_ifstream.open(path);
			m_preferredPacketType = preferredPacketType;
			return true;
		}
		
		void Device::update() {
#ifdef DUMMY_DEVICE
		auto p = nmea::parseSentence("$GPRMC,171804.000,A,5111.8502,N,00626.1751,E,0.07,46.02,280913,,,A*58");
		std::lock_guard<std::mutex> lk(m_lock);
		m_cachedPacket = p;
#else
			std::string line;
			while (std::getline(m_ifstream, line)) {
				if (line[0] != '$') {
					continue;
				}
		
				auto p = nmea::parseSentence(line);
				if (p.type == m_preferredPacketType) { 		
					std::lock_guard<std::mutex> lk(m_lock);
					m_cachedPacket = p;	
				}
			}
#endif
		}
		
		nmea::Packet Device::mostCurrentPacket() {
			std::lock_guard<std::mutex> lk(m_lock);
			return m_cachedPacket;
		}
}
