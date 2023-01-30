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

bool MouseClick(float pos_x, float pos_y, float width, float height, s32 cursor_x, s32 cursor_y)
{
	float left, right, bottom, top;
	left = pos_x - width * 2.f;
	right = pos_x + width * 2.f;
	bottom = pos_y - height * 2.f;
	top = pos_y + height * 2.f;

	if (cursor_x < left) return false;
	if (cursor_x > right) return false;
	if (cursor_y < bottom) return false;
	if (cursor_y > top) return false;
	return true;
}
