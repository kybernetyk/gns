#include "audio.h"
#include <thread>
#include <future>
#include <cstdlib>
#include <thread>
#include <vector>
#include <deque>
#include <unistd.h>

namespace audio {
	void Player::doWork() {
		m_queueMutex.lock();
		if (m_playQueue.size() == 0) {
			m_queueMutex.unlock();
			return;
		}
		std::string filename = m_playQueue.front();
		m_playQueue.pop_front();
		m_queueMutex.unlock();
#ifdef __APPLE__
				std::string cmdline = "afplay ";
#else
				std::string cmdline = "aplay -q ";
#endif
				cmdline += filename;
				cmdline += " > /dev/null 2>&1";
				system(cmdline.c_str());
	}
	
	Player::Player() {
		m_keepWorking.store(true);
		m_playThread = std::thread([this]() {
			while (this->m_keepWorking.load()) {
				usleep(1000);
				doWork();
			}
		});
	}
	
	Player::~Player() {
		m_keepWorking.store(false);
		m_playThread.join();
	}

	void Player::play(std::string filename) {
		std::lock_guard<std::mutex> lk(m_queueMutex);
		m_playQueue.push_back(filename);
	}
	
}
