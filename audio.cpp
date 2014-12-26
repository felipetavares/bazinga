#include "audio.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <codec.h>
#include <vorbisfile.h>
#include <iostream>

#ifdef _WIN32
	#include <io.h>
	#include <fcntl.h>
	#include <windows.h>
#endif

using namespace std;
using namespace bazinga;

#define M_PI 3.14159265359

// Vorbis file
OggVorbis_File vf;
int iseof=0;
int current_section;
FILE *input;

void audio::fill_buffer (void* udata, Uint8* stream, int len) {
	double volume = 1;

	if(!iseof){
		long ret=ov_read(&vf,(char*)stream,len,0,2,1,&current_section);
		if (ret == 0) {
		  /* EOF */
		  iseof=1;
		  volume = 0;
		  cout << "EOF" << endl;
		} else if (ret < 0) {
		  cout << "STREAM ERROR" << endl;
		  /* error in the stream.  Not a problem, just reporting it in
		 case we (the app) cares.  In this case, we don't. */
		} else {
			long newret = 1;
		 	while (ret < len && newret != 0) {
		 		newret=ov_read(&vf,(char*)(stream+ret),(len-ret),0,2,1,&current_section);
		 		ret += newret;

		 		if (newret == 0)
		 			iseof = 1;
		 	}
		  /* we don't bother dealing with sample rate changes, etc, but
		 you'll have to*/
			cout << "RET: " << ret << endl;
		  //fwrite(stream,1,ret,stdout);
		}
	} else {
		volume = 0;
	}

	cout << volume << endl;

	int16_t *newstream = (int16_t*)stream;
	int newstreamlen = len/2;

	for (int i=0;i<newstreamlen;i++) {
		newstream[i] = newstream[i]*volume; // Volume
	}
}

void audio::init () {
	SDL_AudioSpec wanted;

	wanted.freq = 44100;
	wanted.format = AUDIO_S16;
	wanted.channels = 2;
	wanted.samples = 1024;
	wanted.callback = audio::fill_buffer;
	wanted.userdata = NULL;

	input = fopen("assets/audio/ambient1.ogg", "rb");

	if(ov_open_callbacks(input, &vf, NULL, 0, OV_CALLBACKS_NOCLOSE) < 0) {
		fprintf(stderr,"Input does not appear to be an Ogg bitstream.\n");
		exit(1);
	}
	
	{
    char **ptr=ov_comment(&vf,-1)->user_comments;
    vorbis_info *vi=ov_info(&vf,-1);
    while(*ptr){
      fprintf(stderr,"%s\n",*ptr);
      ++ptr;
    }
    cout << "\nBitstream is" << vi->channels << "channel, " << vi->rate << "Hz\n";
 
    fprintf(stderr,"\nDecoded length: %ld samples\n",
            (long)ov_pcm_total(&vf,-1));
    fprintf(stderr,"Encoded by: %s\n\n",ov_comment(&vf,-1)->vendor);
  	}
  

	// Open audio device
	if (SDL_OpenAudio(&wanted, NULL) < 0) {
		cout << "bazinga: audio: couldn't open audio: " << SDL_GetError() << endl;
		return;
	} else {
		cout << "bazinga: audio: init()" << endl;
		cout << "bazinga: audio: format: " << wanted.format << endl;
		cout << "bazinga: audio: freq: " << wanted.freq << endl;
		cout << "bazinga: audio: channels: " << wanted.channels << endl;
		cout << "bazinga: audio: samples: " << wanted.samples << endl;
	}

	SDL_PauseAudio(0);
}

void audio::deinit () {
	ov_clear(&vf);
	SDL_PauseAudio(1);
	SDL_CloseAudio();
}