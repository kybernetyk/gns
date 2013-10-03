#include "nmea_device.h"
#include <stdio.h>
#include <sys/param.h>
#include <sys/filio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>


namespace nmea {
	int config_serial_port(int fd) {
		struct termios options;
		tcgetattr(fd, &options);
		cfmakeraw(&options);
		options.c_cc[VMIN] = 1; // Wait for at least 1 character before returning
		options.c_cc[VTIME] = 2; // Wait 200 milliseconds between bytes before returning from read

		options.c_cflag &= ~PARENB; //no parity
		options.c_cflag &= 1; //1 stop bit
		options.c_cflag &= ~PARENB; /* no parity */
		options.c_cflag &= ~CSTOPB; /* 1 stop bit */
		options.c_cflag &= ~CSIZE;
		options.c_cflag |= CS8 | CLOCAL; /* 8 bits */
		options.c_lflag = ICANON; /* canonical mode */
		options.c_oflag &= ~OPOST; /* raw output */

		// Set baud rate
		cfsetspeed(&options, 4800); 
		int result = tcsetattr(fd, TCSANOW, &options);
		if (result != 0) {
			printf("Unable to set options on %i: %i", fd, result);
		}
		return result;
	}


		bool Device::initWithPathAndPreferredPacketType(std::string path, nmea::Packet::Type preferredPacketType) {
			m_fin = fopen(path.c_str(), "r");
			if (!m_fin) {
				printf("opening '%s' failed!\n", path.c_str());
				return false;
			}
			int fd = fileno(m_fin);
			config_serial_port(fd);
			m_cachedPacket.speed = 0;
			m_cachedPacket.heading = 0;
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
			int max_line_count = 1; //number of lines to read without hitting the preferred target
			int line_count = 0;
			while (line_count++ < max_line_count) {
				line = readline(m_fin);
				if (line[0] != '$') {
					continue;
				}
				auto p = nmea::parseSentence(line);
				if (p.type == m_preferredPacketType) { 		
					std::lock_guard<std::mutex> lk(m_lock);
					auto tp = m_cachedPacket;
					m_cachedPacket = p;	
					if (nmea::packetHasHeading(p) && m_cachedPacket.speed < 4.0) {
						m_cachedPacket.heading = tp.heading;
					}
					return;
				}
			}
		}
		
		nmea::Packet Device::mostCurrentPacket() {
			std::lock_guard<std::mutex> lk(m_lock);
			return m_cachedPacket;
		}
}
