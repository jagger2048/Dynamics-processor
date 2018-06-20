#pragma once
#include <iostream>
//#include <vector.h>
struct noise_gate_buff {
	double fs = 48000;						// sample rate(Hz)
	double attack_time = 0.05;				// attack time(seconds)
	double release_time = 0.02;				// release time(seconds)
	double hold_time = 0.0003 * fs;			// hold time(points)
	double threshold = -24;					// threshold(dB)

	double t_lin = pow(10, threshold / 20);
	double at = exp(-log(9) / (fs * attack_time));		// attack time smoothing coefficient
	double ar = exp(-log(9) / (fs * release_time));		// release time smoothing coefficient

	double counter_a = 0;					// hold counter for attack time
	double counter_r = 0;					// hold counter for release time

											//double x_abs = 0;				
	double x_env = 0;
	//double gca = 0;
	double gs[2] = { 0 };
};

double noiseGate(double audio_in, noise_gate_buff &NG) {
	double x_abs = abs(audio_in);
	// envelop detector  at = 0.02;rt = 0.01;  0.9977 = exp(-log(9) / (fs * 0.02));
	NG.x_env = 0.9977 * NG.x_env + (1 - 0.9553) * ((x_abs - NG.x_env > 0) ? x_abs - NG.x_env : 0);

	double gca;
	if (NG.x_env < NG.t_lin) {
		gca = 0;
	}
	else
		gca = 1;

	if (gca < NG.gs[0]) {
		// attack mode
		NG.counter_r = 0;
		if (++NG.counter_a < NG.hold_time) {
			// hold mode
			NG.gs[0] = NG.gs[1];
		}
		else {
			NG.gs[0] = NG.at * NG.gs[1] + (1 - NG.at) * gca;
		}
		NG.gs[1] = NG.gs[0];

	}
	else {
		// release mode
		NG.counter_a = 0;
		if (++NG.counter_r < NG.hold_time) {
			NG.gs[0] = NG.gs[1];
		}
		else {
			NG.gs[0] = NG.ar * NG.gs[1] + (1 - NG.ar) * gca;
		}
		NG.gs[1] = NG.gs[0];
	}
	// apply gain
	return (NG.gs[0] * audio_in);

}