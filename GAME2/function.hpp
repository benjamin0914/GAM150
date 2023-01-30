#pragma once


float EaseInCubic(float start, float end, float value);
float EaseOutCubic(float start, float end, float value);

	// Use this to check if mouse position is inside a rectangle box
	// Does not check for clicks
bool MouseClick(float pos_x, float pos_y, float width, float height, s32 cursor_x, s32 cursor_y);
