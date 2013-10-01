#include "audio.h"
#include <thread>
#include <future>
#include <cstdlib>
#include <thread>
#include <vector>

namespace audio {
	std::vector<std::thread> _threads;

	void playthread(std::string filename) {
			std::string cmdline = "aplay -q ";
			cmdline += filename;
			cmdline += " > /dev/null 2>&1";
			system(cmdline.c_str());
	}

	void play(std::string filename) {
		_threads.push_back(std::thread(playthread, filename));
	}

}
