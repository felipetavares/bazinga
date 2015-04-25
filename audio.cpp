#include "cache.h"
#include "audio.h"
#include "console.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <codec.h>
#include <iostream>

using namespace std;
using namespace bazinga;

/*
	Defines M_PI if not defined by the math lib
*/
#ifndef M_PI
#define M_PI 3.141592653589793238463
#endif

/*
	Variables used by the audio module.
	For more info, see audio.h
*/
size_t audio::maxMemorySize;
unsigned int audio::samples;
unsigned long audio::frequency;
vector <audio::AudioFile*> audio::playing;

audio::AudioFile::~AudioFile () {
}

audio::AudioData::AudioData (Path fileName):
  section(0), error(false) {
  if (fs::fileExists(fileName)) {
    this->fileName = fileName;
    input = fopen(fileName.getPath().c_str(), "rb");

    if(ov_open_callbacks(input, &oggFile, NULL, 0, OV_CALLBACKS_NOCLOSE) < 0) {
			console << LINEINFO << "input does not appear to be an OGG bitstream" << outline;
			error = true;
			return;
		}

    // Print comments to console
	  char **ptr = ov_comment(&oggFile,-1)->user_comments;
	  vorbis_info *oggInfo = ov_info(&oggFile,-1);
	  while(*ptr) {
	    console << LINEINFO << *ptr << outline;
	    ++ptr;
	  }

		console << LINEINFO << "chanels(s)=" << oggInfo->channels << outline;
		console << LINEINFO << (long)ov_pcm_total(&oggFile,-1) << outline;
		console << LINEINFO << "encoded by " << ov_comment(&oggFile, -1)->vendor << outline;

		// Divide frequencies (see audio::AudioFile::getRate() in audio.h)
		rate = double(oggInfo->rate)/double(audio::frequency);

    bool eofReached = false;

		while (!eofReached && !error) {
      // audio::samples is 16 bit, we want 8 bit
  		int len = 2*audio::samples;
  		int full_len = len*rate;
  		int8_t *data = new int8_t[full_len];

  		long ret;
  		long total_ret = 0;

  		while (total_ret < full_len) {
  			ret = ov_read(&oggFile, (char*)(data+total_ret), (full_len-total_ret), 0, 2, 1, &section);
  			total_ret += ret;

  			if (ret == 0) {
  				eofReached = true;
          break;
  			} else if (ret < 0) {
  				error = true;
  			}
  		}

      if (eofReached && (total_ret < full_len)) {
        for (int i=total_ret;i<full_len;i++) {
          data[i] = 0;
        }
      }

  		samples.push_back((int16_t*)data);
    }
  } else {
    error = true;
  }
}

audio::AudioData::~AudioData () {
}

unsigned int audio::AudioData::getSize () {
  return samples.size();
}

int16_t *audio::AudioData::samplesAt (unsigned int position) {
  return samples[position];
}

double audio::AudioData::getRate () {
  return rate;
}

audio::MemoryAudioFile::MemoryAudioFile (Path fileName):
  position(0), fileName(fileName) {
  audioData = cache::getAudio(fileName);
}

audio::MemoryAudioFile::~MemoryAudioFile () {
}

void audio::MemoryAudioFile::update() {
}

int16_t *audio::MemoryAudioFile::nextSamples() {
  if (position < audioData->getSize()) {
    return audioData->samplesAt(position++);
  } else {
    return NULL;
  }
}

void audio::MemoryAudioFile::freeSamples() {
}

double audio::MemoryAudioFile::getRate () {
  return audioData->getRate();
}

bool audio::MemoryAudioFile::end () {
  return position >= audioData->getSize();
}

audio::DiskAudioFile::DiskAudioFile (Path fileName):
 	section(0), eofReached(false), error(false),
	preload(16) {
	if (fs::fileExists(fileName)) {
		this->fileName = fileName;
		// "rb" because ya'know... Windows.
		input = fopen(fileName.getPath().c_str(), "rb");

		// Try to open the file
		if(ov_open_callbacks(input, &oggFile, NULL, 0, OV_CALLBACKS_NOCLOSE) < 0) {
			console << LINEINFO << "input does not appear to be an OGG bitstream" << outline;
			error = true;
			return;
		}

		// Print comments to console
	  char **ptr = ov_comment(&oggFile,-1)->user_comments;
	  vorbis_info *oggInfo = ov_info(&oggFile,-1);
	  while(*ptr) {
	    console << LINEINFO << *ptr << outline;
	    ++ptr;
	  }

		console << LINEINFO << "chanels(s)=" << oggInfo->channels << outline;
		console << LINEINFO << (long)ov_pcm_total(&oggFile,-1) << outline;
		console << LINEINFO << "encoded by " << ov_comment(&oggFile, -1)->vendor << outline;

		// Divide frequencies (see audio::AudioFile::getRate() in audio.h)
		rate = double(oggInfo->rate)/double(audio::frequency);

		for (int i=0;i<preload;i++)
			readSamples();
	} else {
		error = true;
	}
}

audio::DiskAudioFile::~DiskAudioFile () {
	ov_clear(&oggFile);
	fclose(input);

	// Free all remaing chunks
	while (stream.size()) {
		delete stream.front();
    stream.pop();
	}
}

void audio::DiskAudioFile::update () {
	// Fill buffer with new chunks
	while (stream.size() < preload)
		readSamples();
}

int16_t *audio::DiskAudioFile::nextSamples () {
	if (error)
		return NULL;

	if (stream.size()) {
		 auto samples = latestSamples = stream.front();
		stream.pop();
		return samples;
	} else {
		return NULL;
	}
}

void audio::DiskAudioFile::freeSamples () {
  delete latestSamples;
}

double audio::DiskAudioFile::getRate () {
	return rate;
}

bool audio::DiskAudioFile::end () {
	return !stream.size();
}

void audio::DiskAudioFile::readSamples () {
	if (!eofReached && !error) {
		// audio::samples is 16 bit, we want 8 bit
		int len = 2*audio::samples;
		int full_len = len*rate;
		int8_t *data = new int8_t[full_len];

		long ret;
		long total_ret = 0;

		while (total_ret < full_len) {
			ret = ov_read(&oggFile, (char*)(data+total_ret), (full_len-total_ret), 0, 2, 1, &section);
			total_ret += ret;

			if (ret == 0) {
				eofReached = true;
			} else if (ret < 0) {
				error = true;
			}
		}

		stream.push((int16_t*)data);
	}
}

/*
double sinc (double x, double T) {
	return x==0?1/T:sin(PI*x)/(PI*x);
}

double hs (double t, double T) {
	return sinc(t/T, T);
}
*/

double analogAudioAt (double t, double T, int16_t *audio, int len, int side) {
	double r = 0;

	int s = floor(t-1);
	int e = ceil(t+1);

	if (s < 0)
		s = 0;
	if (e >= len)
		e = len-1;

	for (int i=s;i<e;i++) {
			r = audio[i*2+side];//*hs(t-T*i, T);
	}

	return r;
}

void audio::fill_buffer (void* udata, Uint8* stream, int len) {
	// We use signed 16 bits everywhere
	// All the s prefixes come from 'sixteen'
	int16_t *sStream = (int16_t*)stream;
	unsigned int sLen = len/2;

	bool copy = true;

	for (auto& file :playing) {
		auto samples = file->nextSamples();
		auto rate = file->getRate();

		if (!samples)
			continue;

		// Length of one channel
		unsigned int monoSLen = sLen/2;

		for (unsigned int i=0;i<monoSLen;i++) {
			// Position
			double p = double(i)*rate;

			if (copy) {
				sStream[i*2] = analogAudioAt(p, 1.0, samples, audio::samples, 0);
				sStream[i*2+1] = analogAudioAt(p, 1.0, samples, audio::samples, 1);
			} else {
				sStream[i*2] += analogAudioAt(p, 1.0, samples, audio::samples, 0);
				sStream[i*2+1] += analogAudioAt(p, 1.0, samples, audio::samples, 1);
			}
		}

    copy = false;
    file->freeSamples();
	}
}

void audio::play (Path file) {
	playing.push_back(load(file));
}

void audio::init () {
	SDL_AudioSpec wanted;

	wanted.freq = 44100;
	wanted.format = AUDIO_S16;
	wanted.channels = 2;
	wanted.samples = 2048;
	wanted.callback = audio::fill_buffer;
	wanted.userdata = NULL;

	// Open audio device
	if (SDL_OpenAudio(&wanted, NULL) < 0) {
		console << LINEINFO << "bazinga: audio: couldn't open audio: " << SDL_GetError() << outline;
		return;
	} else {
		console << LINEINFO << "bazinga: audio: init()" << outline;
		console << LINEINFO << "bazinga: audio: format: " << (int)wanted.format << outline;
		console << LINEINFO << "bazinga: audio: freq: " << wanted.freq << outline;
		console << LINEINFO << "bazinga: audio: channels: " << (int)wanted.channels << outline;
		console << LINEINFO << "bazinga: audio: samples: " << wanted.samples << outline;
	}

	audio::samples = wanted.samples*2;
	audio::frequency = wanted.freq;
	// max 2mb per audio file in memory
	audio::maxMemorySize = 2 * 1024 * 1024;

	SDL_PauseAudio(0);
}

void audio::update () {
	for (int i=0;i<playing.size();i++) {
		auto file = playing[i];

		file->update();

		if (file->end()) {
			playing.erase(playing.begin()+i);
			delete file;
			i--;
		}
	}
}

void audio::deinit () {
	SDL_PauseAudio(1);
	SDL_CloseAudio();
}

audio::AudioFile* audio::load (Path path) {
	AudioFile* file;

	if (fs::getFileSize(path) > audio::maxMemorySize) {
		file = new DiskAudioFile(path);
	} else {
		file = new MemoryAudioFile(path);
	}

	return file;
}
