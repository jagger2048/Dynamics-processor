#include "NoiseGate.h"
NoiseGate_t* newNoiesGate() {
	NoiseGate_t* obj = (NoiseGate_t*)malloc(sizeof(NoiseGate_t));
	memset(obj, 0, sizeof(NoiseGate_t));
	if (!obj)
	{
		return NULL;
	}
	return obj;
}
void setNoiseGate(NoiseGate_t* obj, float attackTime, float releaseTime, float threshold, float holdTime, float samplerate) {

	obj->attackTime_ = attackTime;
	obj->releaseTime_ = releaseTime;
	obj->threshold_ = threshold;
	obj->samplerate_ = samplerate;
	obj->holdTime_ = holdTime;

	obj->holdTimeCount = obj->holdTime_ * obj->samplerate_;
	obj->linearThreshold = powf(10.0f, obj->threshold_ / 20.0f);
	obj->at = exp(-logf(9) / (obj->samplerate_ * obj->attackTime_));
	obj->rt = exp(-logf(9) / (obj->samplerate_ * obj->releaseTime_));
}
NoiseGate_t* createNoiseGate(float attackTime, float releaseTime, float threshold, float samplerate) {
	NoiseGate_t* obj = newNoiesGate();
	if (!obj) {
		return NULL;
	}
	setNoiseGate(obj, attackTime, releaseTime, threshold, 0.0003, samplerate);
	return obj;
}
float runNoiseGate(NoiseGate_t* obj, float in, float* out) {
	float x_abs = in > 0 ? in : -in;
	// envelop detector  at = 0.02;rt = 0.01;  0.9977 = exp(-log(9) / (samplerate * 0.02));
	//obj->x_env = 0.9977 * obj->x_env + (1 - 0.9553) * ((x_abs - obj->x_env > 0) ? x_abs - obj->x_env : 0);
	obj->x_env = obj->rt * obj->x_env + (1 - obj->at) * ((x_abs - obj->x_env > 0) ? x_abs - obj->x_env : 0);

	if (obj->x_env < obj->linearThreshold)
	{
		obj->gca = 0;
	}
	else
	{
		obj->gca = 1;
	}

	if (obj->gca < obj->gs[0])
	{
		// attack mode
		obj->releaseCounter = 0;
		if (++obj->attactCounter < obj->holdTimeCount)
		{
			// hold mode
			obj->gs[0] = obj->gs[1];
		}
		else
		{
			obj->gs[0] = obj->at * obj->gs[1] + (1 - obj->at) * obj->gca;
		}
		obj->gs[1] = obj->gs[0];

	}
	else {
		// release mode
		obj->attactCounter = 0;
		if (++obj->releaseCounter < obj->holdTimeCount)
		{
			// hold mode
			obj->gs[0] = obj->gs[1];
		}
		else
		{
			obj->gs[0] = obj->rt * obj->gs[1] + (1 - obj->rt) * obj->gca;
		}
		obj->gs[1] = obj->gs[0];
	}
	// apply gain
	*out = obj->gs[0] * in;
	return (obj->gs[0] * in);

}
int freeNoiseGate(NoiseGate_t* obj) {
	if (!obj)
	{
		return -1;
	}
	free(obj);
	return 0;
}