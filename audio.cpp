#include "audio.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <codec.h>
#include <vorbisfile.h>
#include <iostream>

using namespace std;
using namespace bazinga;

const double PI = 3.141592653589793238463;

// Vorbis file
OggVorbis_File vf;
int iseof=0;
int current_section;
FILE *input;
double rate = 1;
int16_t prev_audio[8];

double sinc (double x, double T) {
	return x==0?1/T:sin(x)/x;
}

double hs (double t, double T) {
	return sinc(t/T, T);
}

double analogAudioAt (double t, double T, int16_t *audio, int len) {
	double r = 0;

	int s = floor(t-8);
	int e = ceil(t+8);

	if (s < 0)
		s = 0;
	if (e > len)
		e = len;

	for (int i=s;i<e;i++) {
		if (i < 0)
			r += double(double(prev_audio[i+8])*hs(t-T*i, T));
		else
			r += double(double(audio[i])*hs(t-T*i, T));
	}

	return r/PI;
}

// Ported from BASIC (from Ron Nicholson)
double resamp(double x, int16_t *indat, int alim, double fmax, double fsr, int wnwdth) {
	int i,j;
	double r_g,r_w,r_a,r_snc,r_y;

	r_g = 2 * fmax / fsr; // Calc gain correction factor
	r_y = 0;

	for (i = -wnwdth/2;i<(wnwdth/2);i++) { // For 1 window width
		j = int(x + i); // Calc input sample index
	    // calculate von Hann Window. Scale and calculate Sinc
		r_w = 0.5 - 0.5 * cos(2*PI*(0.5 + (j - x)/wnwdth));
		r_a = 2*PI*(j - x)*fmax/fsr;
		r_snc = r_a!=0?r_snc = sin(r_a)/r_a:1;
	
		if ((j >= 0) && (j < alim)) {
		  r_y   = r_y + r_g * r_w * r_snc * indat[j];
		}
	}

	return r_y;                 // Return new filtered sample
}

void audio::fill_buffer (void* udata, Uint8* stream, int len) {
	int full_len = len*rate;
	
	char *data = new char[full_len];

	double volume = 1;

	if(!iseof){
		long total_ret = 0;
		long ret;

		while (total_ret < full_len) {
			ret = ov_read(&vf, (char*)(data+total_ret), (full_len-total_ret), 0, 2, 1, &current_section);
			total_ret += ret;

			if (ret == 0) {
				iseof=1;
				volume = 0;
				break;
			} else if (ret < 0) {
				// Stream error
			} else {
				// Nothing else, I think
			}
		}
	}

	// Apply audio filters (possibily written in Lua?)
	/*
	int16_t *newstream = (int16_t*)stream;
	int newstreamlen = len/2;

	for (int i=0;i<newstreamlen;i++) {
		newstream[i] = newstream[i]*volume; // Volume
	}
	*/

	int16_t *newstream = (int16_t*)stream;
	int newstreamlen = len/2;

	for (int i=0;i<newstreamlen;i++) {
		double p = double(i)*rate;
		int j = int(floor(p));
		//int k = int(ceil(p));
		//newstream[i] = double(((int16_t*)data)[j])*double(p-j)+double(((int16_t*)data)[k])*double(k-p);
		//newstream[i] = analogAudioAt(p, 1, (int16_t*)data, full_len/2);
		newstream[i] = ((int16_t*)data)[j]*0;
		//newstream[i] = resamp(p, (int16_t*)data, full_len/2, 0.1, 10, 32);
	}

	delete data;
}

void audio::init () {
	SDL_AudioSpec wanted;

	wanted.freq = 44100;
	wanted.format = AUDIO_S16;
	wanted.channels = 2;
	wanted.samples = 1024;
	wanted.callback = audio::fill_buffer;
	wanted.userdata = NULL;

	input = fopen("assets/audio/backinblack.ogg", "rb");

	if(ov_open_callbacks(input, &vf, NULL, 0, OV_CALLBACKS_NOCLOSE) < 0) {
		fprintf(stderr,"Input does not appear to be an Ogg bitstream.\n");
		exit(1);
	}
	
	{
    char **ptr=ov_comment(&vf,-1)->user_comments;
    vorbis_info *vi=ov_info(&vf,-1);
    while(*ptr){
      //cout << *ptr << endl;
      ++ptr;
    }
    cout << "bazinga: audio: "<< "bitstream " << vi->channels << " channel(s), " << vi->rate << "Hz" << endl;
 
 	/*
    fprintf(stderr,"\nDecoded length: %ld samples\n",
            (long)ov_pcm_total(&vf,-1));
    fprintf(stderr,"Encoded by: %s\n\n",ov_comment(&vf,-1)->vendor);
	*/
	
	rate = double(vi->rate)/double(wanted.freq);
   	//rate = double(wanted.freq)/double(vi->rate);
   	//rate = 1;
  	}
 

	// Open audio device
	if (SDL_OpenAudio(&wanted, NULL) < 0) {
		cout << "bazinga: audio: couldn't open audio: " << SDL_GetError() << endl;
		return;
	} else {
		cout << "bazinga: audio: init()" << endl;
		cout << "bazinga: audio: format: " << (int)wanted.format << endl;
		cout << "bazinga: audio: freq: " << wanted.freq << endl;
		cout << "bazinga: audio: channels: " << (int)wanted.channels << endl;
		cout << "bazinga: audio: samples: " << wanted.samples << endl;
	}

	SDL_PauseAudio(0);
}

void audio::deinit () {
	SDL_PauseAudio(1);
	SDL_CloseAudio();
	ov_clear(&vf);
}