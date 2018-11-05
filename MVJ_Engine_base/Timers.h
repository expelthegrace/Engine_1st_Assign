#pragma once

#include "SDL.h"

class Timer
{
public:
	Timer();
	~Timer();

	void Start() {
		inici = SDL_GetTicks();
		final = -1;
		running = true;
	}

	float Read() {
		return SDL_GetTicks() - inici;
	}

	float Stop(){
		if (running) {
			running = false;
			return final = (SDL_GetTicks() - inici) / 1000;
		}
		else return final;
	}

private:
	bool running = false;
	float inici, final;
};

class AccurateTimer
{
public:
	AccurateTimer();
	~AccurateTimer();

	void Start() {
		inici = SDL_GetPerformanceCounter();
		final = -1;
		running = true;
		freq = SDL_GetPerformanceFrequency();
	}

	double Read() {
		return SDL_GetPerformanceCounter() - inici;
	}

	double Stop() {

		if (running) {
			running = false;
			return final = (SDL_GetPerformanceCounter() - inici) / freq;
		}
		else return final;
	}

private:
	bool running = false;
	double inici, final;
	double freq;
};

