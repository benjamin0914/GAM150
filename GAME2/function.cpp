#include "pch.h"

/*
float EaseInCubic(float start, float end, float value)
{
	end -= start;
	return end * value * value * value + start;
}*/

float EaseInCubic(float start, float end, float value)
{
	end -= start;
	return end * value * value * value + start;
}

float EaseOutCubic(float start, float end, float value)
{
	value--;
	end -= start;
	return end * (value * value * value + 1) + start;
}
