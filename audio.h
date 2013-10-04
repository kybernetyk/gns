#pragma once
#include <string>
#include <deque>
#include <thread>
#include <atomic>

namespace audio {
	class Player {
		public:
			Player();
			void play(std::string filename);
			~Player();
			
		protected:
			void doWork();

		private:
			std::thread m_playThread;
			std::mutex m_queueMutex;
			std::deque<std::string> m_playQueue;	
			std::atomic_bool m_keepWorking;
	};
}
