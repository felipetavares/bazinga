#include "audio.h"
#include <cmath>
#include <iostream>
using namespace std;
using namespace bazinga;

#define M_PI 3.14159265359

void audio::fill_buffer (void* udata, Uint8* stream, int len) {
	static double v = 0;
	double multiplier = 440.0*1024/44100;

	for (int i=0;i<len;i++) {
		stream[i] = sin(v*multiplier)*127;
		v += 2*M_PI/double(len);
	}
}

void audio::init () {
	SDL_AudioSpec wanted;

	wanted.freq = 44100;
	wanted.format = AUDIO_S8;
	wanted.channels = 2;
	wanted.samples = 1024;
	wanted.callback = audio::fill_buffer;
	wanted.userdata = NULL;

	if (SDL_OpenAudio(&wanted, NULL) < 0) {
		cout << "bazinga: audio: couldn't open audio: " << SDL_GetError() << endl;
		return;
	} else {
		cout << "bazinga: audio: init()" << endl;
		cout << "bazinga: audio: format: " << wanted.format << endl;
		cout << "bazinga: audio: freq: " << wanted.freq << endl;
		cout << "bazinga: audio: channels: " << wanted.freq << endl;
		cout << "bazinga: audio: samples: " << wanted.samples << endl;
	}

	SDL_PauseAudio(0);
}

void audio::deinit () {
	SDL_CloseAudio();
}