#ifndef BAZINGA_AUDIO_H
#define BAZINGA_AUDIO_H

#include <SDL.h>

namespace bazinga {
	namespace audio {
		void fill_buffer (void*, Uint8*, int);

		void init();
		void deinit();
	}
}

#endif /* BAZINGA_AUDIO_H */