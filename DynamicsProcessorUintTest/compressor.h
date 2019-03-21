#pragma once
/**
*	The Implement of the compressor
*
*/
#include <math.h>
#include <stdlib.h>

typedef struct {
	// interface properties
	float threshold;			// Threshold	dB		[-50, 0]
	float ratio;				// ratio		none	[1, 50]
	float kneeWidth;			// KneeWidth	dB		[0, 20]
	float attackTime;			// attack time	seconds	[0,	4]
	float releaseTime;			// release time	seconds	[0, 4]
	float makeUpGain;			// makeup gain	dB		[-10,24] || TBD.

	// private parameters
	size_t samplerate;
	float gc;				// gain computer
	float gs;				// gain smoothing
	float alphaA;			// attack time factor
	float alphaR;			// release time factor

	float x_sc;				// the static characteristic of input x_dB
	bool isInitialized;
}
Compressor;

static inline void setCompressor(Compressor* obj, float threshold, float ratio, float kneeWidth, float attackTime, float releaseTime, float makeUpGain, float samplerate) {

	obj->threshold = threshold;
	obj->ratio = ratio;
	obj->kneeWidth = kneeWidth;
	obj->attackTime = attackTime;
	obj->releaseTime = releaseTime;
	obj->makeUpGain = makeUpGain;
	obj->samplerate = samplerate;

	//
	obj->alphaA = expf(-logf(9.0f) / (double)(obj->samplerate * obj->attackTime));
	obj->alphaR = expf(-logf(9.0f) / (double)(obj->samplerate * obj->releaseTime));

	if (!obj->isInitialized)
	{
		obj->gc = 0;
		obj->gs = 0;
		obj->x_sc = 0;
	}
}
static inline Compressor* createCompressor(float threshold, float ratio, float kneeWidth, float attackTime, float releaseTime, float makeUpGain, float samplerate) {
	Compressor* obj = (Compressor*)malloc(sizeof(Compressor));
	if (!obj)
	{
		return NULL;
	}
	obj->isInitialized = false;
	setCompressor(obj, threshold, ratio, kneeWidth, attackTime, releaseTime, makeUpGain, samplerate);
	obj->isInitialized = true;
	return obj;
}
float runCompressor(Compressor* obj, float input, float& output) {
	//	input:	linear input data
	//	output:	linear output data

	//------- Convert input to dB -------//
	// Peak version level detector :
	float inputGain = 20 * log10(fabs(input) + 0.00001);	// the gain of input sample in dB |x_dB|

	//------- Running gain computer -------//
	if (inputGain < obj->threshold - obj->kneeWidth / 2.0f)
	{
		obj->x_sc = inputGain;
	}
	else if (inputGain > obj->threshold + obj->kneeWidth / 2.0f)
	{
		obj->x_sc = obj->threshold + (inputGain - obj->threshold) / obj->ratio;	// compressor

	}
	else
	{
		if (obj->kneeWidth == 0)
		{
			obj->x_sc = obj->threshold + (inputGain - obj->threshold) / obj->ratio;
		}
		else
		{
			float tmp2 = inputGain - obj->threshold + obj->kneeWidth / 2.0f;	// for compressor
			obj->x_sc = inputGain + (1.0f / obj->ratio - 1)*(tmp2 * tmp2) / (2.0f * obj->kneeWidth); // compressor
		}
	}

	obj->gc = obj->x_sc - inputGain;


	//------- Running gain smoothing -------//
	if (obj->gc < obj->gs)
	{
		obj->gs = obj->alphaA * obj->gs + (1.0f - obj->alphaA)*obj->gc;
	}
	else
	{
		obj->gs = obj->alphaR * obj->gs + (1.0f - obj->alphaR)*obj->gc;
	}


	//------- Running Makeup Gain -------//
	// TBD.
	//obj->makeUpGain = (-x_sc)_{ x_dB = 0 }
	float gainMadeUp = obj->gs + obj->makeUpGain;


	//------- Applying gain -------//
	float linearGain = powf(10.0, gainMadeUp / 20.0f);
	output = input * linearGain;

	return output;

}
static inline int freeCompressor(Compressor *obj) {
	if (!obj)
	{
		return -1;
	}
	free(obj);
	return 0;
}



