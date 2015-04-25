#ifndef BAZINGA_AUDIO_H
#define BAZINGA_AUDIO_H

#include "filesystem.h"
#include <vorbisfile.h>
#include <queue>
#include <vector>
#include <SDL.h>

namespace bazinga {
	/*
		Audio Module

		Contains all classes and functions responsible
		for outputting audio.
		It uses signed 16bit audio everywhere internally.
	*/
	namespace audio {
		class AudioFile;

		/*
			Files bigger than this size won't be allowed to live
			on memory (will be loaded as DiskAudioFile instead of
			MemoryAudioFile)

			In bytes.
		*/
		extern size_t maxMemorySize;
		/*
			Number of 16bit samples used in every audio chunk.
		*/
		extern unsigned int samples;
		/*
			Number of samples consumed in one second
		*/
		extern unsigned long frequency;
		/*
			All the audio files currently playing
		*/
		extern vector <AudioFile*> playing;
		/*
			Buffer Filler

			Called every time the audio driver needs more
			audio samples. Mixes audio from all the audio files listed in
			audio::playing.
		*/
		void fill_buffer (void*, Uint8*, int);

		/*
			An audio file representation

			The real work is done by DiskAudioFile and MemoryAudioFile,
			this is just an abstract class to keep all clean.
		*/
		class AudioFile {
		public:
			virtual ~AudioFile();
			/*
				Returns the next chunk of audio samples
				for this file.

				The size of the returned chunk is equal to
				audio::samples.
			*/
			virtual int16_t* nextSamples() = 0;
			/*
				Fills internal buffers as needed
			*/
			virtual void update() = 0;
			/*
				Returns the audio rate:

					file frequency / audio card frequency

				The audio card frequency is stored in audio::frequency.
			*/
			virtual double getRate() = 0;
			/*
				Returns true when the file has reached its end
			*/
			virtual bool end() = 0;
			/*
				Frees the latest samples returned by nextSamples()
			*/
			virtual void freeSamples() = 0;
		};

		/*
			Audio file played directly from the disk

			Best used for long songs. For sound effects
			use MemoryAudioFile instead.
		*/
		class DiskAudioFile: public AudioFile {
			/*
				OGG File used for reading audio samples
			*/
			FILE *input;
			/*
				Contains specific information about the OGG file
			*/
			OggVorbis_File oggFile;
			/*
				Contains the section we are in the ogg file
			*/
			int section;
			/*
				Must contain true if eof has been reached
			*/
			bool eofReached;
			/*
				Path to the file
			*/
			Path fileName;
			/*
				Stream of chunks of samples used to feed the audio
				pipeline (returned to the audio driver using nextSamples())
			*/
			queue <int16_t *> stream;
			/*
				See audio::AudioFile::getRate()
			*/
			double rate;
			/*
				Must be equal to true if we can't read this file for any reason

				When error is set, nextSamples() will return NULL
			*/
			bool error;
			/*
				Number of sample chunks to preload & bufferize
			*/
			unsigned int preload;
			int16_t *latestSamples;
		public:
			DiskAudioFile(Path);
			~DiskAudioFile();

			void update();
			int16_t *nextSamples();
			double getRate();
			bool end();
			void freeSamples();
		private:
			void readSamples();
		};

		/*
			Used to keep all the needed data in a single place.
			Can be used by multiple MemoryAudioFile's to avoid repetition,
			by using the cache::getAudio API.
		*/
		class AudioData {
			/*
				OGG File used for reading audio samples
			*/
			FILE *input;
			/*
				Contains specific information about the OGG file
			*/
			OggVorbis_File oggFile;
			/*
				Contains the section we are in the ogg file
			*/
			int section;
			/*
				Path to the file
			*/
			Path fileName;
			/*
				Contains all the samples in the audio
			*/
			vector <int16_t *> samples;
			/*
				See audio::AudioFile::getRate()
			*/
			double rate = 1;
			/*
				Must be true we get any error reading the file
			*/
			bool error;
		public:
			AudioData(Path);
			~AudioData();

			/*
				Returns the size of the samples vector
			*/
			unsigned int getSize();
			int16_t *samplesAt(unsigned int);
			double getRate();
		};

		/*
			Audio file played from memory after buffering from disk

			Best used for sound effects. For songs, use DiskAudioFile
			instead.
		*/
		class MemoryAudioFile: public AudioFile {
			/*
				Position the the samples vector,
				e.g.: the next vector position
				that will be returned when nextSamples()
				is called
			*/
			unsigned int position;
			Path fileName;
			AudioData* audioData;
		public:
			/*
				Reads the file to samples &
				init all the needed variables
				(rate, position, etc.)
			*/
			MemoryAudioFile(Path);
			~MemoryAudioFile();

			/*
				Does nothing (no need to fill buffers here)
			*/
			void update();
			int16_t *nextSamples();
			double getRate();
			bool end();
			void freeSamples();
		};

		/*
			Initializes the audio module
		*/
		void init();
		/*
			Fills all buffers
		*/
		void update();
		/*
			Clear everything
		*/
		void deinit();
		/*
			Play the file specified by the path
		*/
		void play(Path);
		/*
			Decides if a file should be in disk
			or not and load it as needed
		*/
		AudioFile* load(Path);
	}
}

#endif /* BAZINGA_AUDIO_H */
