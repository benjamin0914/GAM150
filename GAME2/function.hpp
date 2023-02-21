#pragma once

struct AABB
{
	//AEVec2	c; // center
	//float  r[2]; // holds half width and half height

	AEVec2	min;
	AEVec2	max;
};

bool CollisionIntersection_RectRect(const AABB& aabb1, const AEVec2& vel1,
	const AABB& aabb2, const AEVec2& vel2);

float EaseInCubic(float start, float end, float value);
float EaseOutCubic(float start, float end, float value);

	// Use this to check if mouse position is inside a rectangle box
	// Does not check for clicks
bool MouseClick(float pos_x, float pos_y, float width, float height, s32 cursor_x, s32 cursor_y);