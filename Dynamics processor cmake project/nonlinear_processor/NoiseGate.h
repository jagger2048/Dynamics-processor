#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <stdio.h>
typedef struct  {
	float samplerate_;		// sample rate(Hz)		48000
	float attackTime_;		// attack time(seconds)	0.05
	float releaseTime_;	// release time(seconds)0.02
	float holdTime_;		// hold time(second)	0.0003
	float threshold_;		// threshold_(dB)		-24
	//======
	float holdTimeCount;	// hold time count (points)
	float linearThreshold;	// linear threshold
	float at;				// attack time smoothing coefficient
	float rt;				// release time smoothing coefficient

	float attactCounter ;					// hold counter for attack time
	float releaseCounter ;					// hold counter for release time
	float x_env ;
	float gca ;
	float gs[2] ;
}
NoiseGate_t;

// New and return a NoiseGate struct poniter
NoiseGate_t * newNoiesGate();

// set the noise gate
// samplerate		sample rate (Hz)		48000
// attackTime		attack time (seconds)	0.05
// releaseTime		release time (seconds)	0.02
// holdTime			hold time (second)		0.0003
// threshold		threshold (dB)			-24
void setNoiseGate(NoiseGate_t * obj, float attackTime, float releaseTime, float threshold, float holdTime, float samplerate);


// new and set the nnoise gate
// samplerate		sample rate (Hz)		48000
// attackTime		attack time (seconds)	0.05
// releaseTime		release time (seconds)	0.02
// holdTime			hold time (second)		0.0003
// threshold		threshold (dB)			-24
NoiseGate_t * createNoiseGate(float attackTime, float releaseTime, float threshold, float samplerate);


// run the noise gate
float runNoiseGate(NoiseGate_t * obj, float in, float* out);


// free the noise gate
int freeNoiseGate(NoiseGate_t * obj);
