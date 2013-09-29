#include "nmea_device.h"
#include <stdio.h>
#define DUMMY_DEVICE 1

namespace nmea {
		bool Device::initWithPathAndPreferredPacketType(std::string path, nmea::Packet::Type preferredPacketType) {
			m_fin = fopen(path.c_str(), "r");
			if (!m_fin) {
				printf("opening '%s' failed!\n", path.c_str());
				return false;
			}
			m_preferredPacketType = preferredPacketType;
			return true;
		}

		Device::~Device() {
			fclose(m_fin);
		}

		//so fucking ghetto
		std::string readline(FILE *f) {
			std::string s;
			for (;;) {
				char c;
				size_t r = fread(&c, 1, 1, f);
				if (r != 1) {
					break;
				}
				if (c == '\n') {
					break;
				}
				s += c;
			}
			return s;
		}
		
		void Device::update() {
			//yes this locking will pretty much lead to single thread execution
			//but who cares? we're doing only 4800 baud anyway
			if (!m_fin) {
				printf("file handle not initialized!\n");
				abort();
				return;
			}
			std::string line;
			int max_line_count = 16; //number of lines to read without hitting the preferred target
			int line_count = 0;
			while (line_count++ < max_line_count) {
				line = readline(m_fin);
				if (line[0] != '$') {
					continue;
				}
				auto p = nmea::parseSentence(line);
				if (p.type == m_preferredPacketType) { 		
					std::lock_guard<std::mutex> lk(m_lock);
					m_cachedPacket = p;	
					return;
				}
			}
		}
		
		nmea::Packet Device::mostCurrentPacket() {
			std::lock_guard<std::mutex> lk(m_lock);
			return m_cachedPacket;
		}
}
