#include "nmea_device.h"
#define DUMMY_DEVICE 1

namespace nmea {
		bool Device::initWithPathAndPreferredPacketType(std::string path, nmea::Packet::Type preferredPacketType) {
			m_ifstream.open(path);
			m_preferredPacketType = preferredPacketType;
			return true;
		}
		
		void Device::update() {
			//yes this locking will pretty much lead to single thread execution
			//but who cares? we're doing only 4800 baud anyway
			std::string line;
			int max_line_count = 255; //number of lines to read without hitting the preferred target
			int line_count = 0;
			while (line_count++ < max_line_count && std::getline(m_ifstream, line)) {
				if (line[0] != '$') {
					continue;
				}
		
				auto p = nmea::parseSentence(line);
				if (p.type == m_preferredPacketType) { 		
					std::lock_guard<std::mutex> lk(m_lock);
					m_cachedPacket = p;	
					break;
				}
			}
		}
		
		nmea::Packet Device::mostCurrentPacket() {
			std::lock_guard<std::mutex> lk(m_lock);
			return m_cachedPacket;
		}
}
