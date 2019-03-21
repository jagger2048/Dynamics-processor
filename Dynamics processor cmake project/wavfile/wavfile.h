#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "dr_wav.h"

#ifndef DR_WAV_IMPLEMENTATION
#define DR_WAV_IMPLEMENTATION
#endif // DR_WAV_IMPLEMENTATION

#ifdef __cplusplus
//extern "C" {
#endif // __cplusplus

	// A package of the dr_wav library for easy to use.
	typedef struct 
	{
		unsigned int channels;
		unsigned int sampleRate;
		drwav_uint64 totalPCMFrameCount;	//	nSamples per cahnnels
		drwav_uint64 totalSampleCount;		//	left samples +drwav_open_file_and_read_pcm_frames_f32 ritht samples
		int16_t *pDataS16[2];				//	a pointer point to the S16 format audio data || S16:int16_t[-32768, 32767] is 16 bits PCM data.
											//	pDataS16[0] : left channel, pDataS16[1] : right channel
		float *pDataFloat[2];				//	a pointer point to the Float format audio data || -1 ~ 1
											//	pDataFloat[0] : left channel , pDataFloat[1] : right channel
	}WAV;

	WAV* wavfile_create();
	WAV* wavfile_read(const char* filename);
	
	int wavfile_write_s16(const char* filename, int16_t * const *pDataS16, size_t nSamples, unsigned int nChannels, unsigned int sampleRate);
	int wavfile_write_f32(const char* filename, float * const *pDataFloat, size_t nSamples, unsigned int nChannels, unsigned int sampleRate);
	int wavfile_destory(WAV *handles);




#ifdef __cplusplus
//}
#endif // __cplusplus
