#include "audio.h"
#include <thread>
#include <future>
#include <cstdlib>
#include <thread>
#include <vector>

namespace audio {
	std::vector<std::thread> _threads;

	void playthread(std::string filename) {
#ifdef __APPLE__
			std::string cmdline = "afplay ";
#else
			std::string cmdline = "aplay -q ";
#endif
			cmdline += filename;
			cmdline += " > /dev/null 2>&1";
			system(cmdline.c_str());
	}

	void play(std::string filename) {
		_threads.push_back(std::thread(playthread, filename));
	}

}
