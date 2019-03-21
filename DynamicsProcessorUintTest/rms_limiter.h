#pragma once
/**
*	The Implement of the Rms Limiter
*
*/
#include <math.h>
#include <stdlib.h>

typedef struct {
	// interface properties
	float threshold;			// Threshold	dB		[50, 0]
	float kneeWidth;			// KneeWidth	dB		[0, 20]
	float attackTime;			// attack time	seconds	[0,	4]
	float releaseTime;			// release time	seconds	[0, 4]
	float makeUpGain;			// makeup gain	dB		[-10,24]

	// private parameters
	size_t samplerate;
	float gc;				// gain computer
	float gs;				// gain smoothing
	float alphaA;			// attack time factor
	float alphaR;			// release time factor

	float x_sc;				// the static characteristic of input x_dB
	float rms;
	bool isInitialized;
}
RmsLimiter;

static inline void setRmsLimiter(RmsLimiter* obj, float threshold,float kneeWidth, float attackTime, float releaseTime, float makeUpGain, float samplerate) {

	obj->threshold = threshold;
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
		obj->rms = 0.001;
	}
}

static inline RmsLimiter* createRmsLimiter(float threshold, float kneeWidth, float attackTime, float releaseTime, float makeUpGain, float samplerate) {
	RmsLimiter* obj = (RmsLimiter*)malloc(sizeof(RmsLimiter));
	if (!obj)
	{
		return NULL;
	}
	obj->isInitialized = false;
	setRmsLimiter(obj, threshold, kneeWidth, attackTime, releaseTime, makeUpGain, samplerate);
	return obj;
}
float runRmsLimiter(RmsLimiter* obj, float input, float& output) {
	//	input:	linear input data
	//	output:	linear output data

	//------- Convert input to dB -------//
	// Note that we should use the rms detector instead of peak detector
	// Rms version formula : y(n) = alpha*y(n-1) + (1-alpha)*x(n)^2
	obj->rms = obj->alphaA * obj->rms + (1 - obj->alphaA) * input * input;
	float inputGain = 10*log10(obj->rms); 

	//------- Running gain computer -------//
	if (inputGain < obj->threshold - obj->kneeWidth / 2.0f)
	{
		obj->x_sc = inputGain;
	}
	else if (inputGain > obj->threshold + obj->kneeWidth / 2.0f)
	{
		obj->x_sc = obj->threshold;	// limiter 

	}
	//else if (!obj->kneeWidth)
	else
	{
		if (obj->kneeWidth == 0 )
		{
			obj->x_sc = obj->threshold;
		}
		else
		{
			float tmp2 = inputGain - obj->threshold + obj->kneeWidth / 2.0f;
			obj->x_sc = inputGain - tmp2 * tmp2 / (2.0f * obj->kneeWidth);
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
static inline int freeRmsLimiter(RmsLimiter *obj) {
	if (!obj)
	{
		return -1;
	}
	free(obj);
	return 0;
}
