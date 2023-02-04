#include "pch.h"

bool CollisionIntersection_RectRect(const AABB& aabb1, const AEVec2& vel1,
	const AABB& aabb2, const AEVec2& vel2)
{
	UNREFERENCED_PARAMETER(aabb1);
	UNREFERENCED_PARAMETER(vel1);
	UNREFERENCED_PARAMETER(aabb2);
	UNREFERENCED_PARAMETER(vel2);

	// Static collision check	
	if ((aabb1.max.x < aabb2.min.x) || (aabb1.max.y < aabb2.min.y) ||
		(aabb2.max.x < aabb1.min.x) || (aabb2.max.y < aabb1.min.y))
	{

		// Continue with dynamic check
		float tfirst = 0.f;
		float tlast = (float)AEFrameRateControllerGetFrameTime();

		// Set A stationary, let B have combined velocity
		AEVec2 vel_relative_b, vel_a;
		AEVec2 dfirst, dlast;
		AEVec2Set(&vel_relative_b, vel2.x, vel2.y);
		AEVec2Set(&vel_a, vel1.x, vel1.y);
		// Vector B - A to get relative velocity for obj B
		AEVec2Sub(&vel_relative_b, &vel_relative_b, &vel_a);

		dfirst.x = aabb1.min.x - aabb2.max.x;
		dfirst.y = aabb1.min.y - aabb2.max.y;
		dlast.x = aabb1.max.x - aabb2.min.x;
		dlast.y = aabb1.max.y - aabb2.min.y;

		if (vel_relative_b.x < 0)
		{
			// Case 1 x
			if (aabb1.min.x > aabb2.max.x)
			{
				return false;
			}
			// Case 4 x - Revisited
			if (aabb1.max.x < aabb2.min.x)
			{
				tfirst = (dfirst.x / vel_relative_b.x) > tfirst ? (dfirst.x / vel_relative_b.x) : tfirst;
				tlast = (dlast.x / vel_relative_b.x) < tlast ? (dlast.x / vel_relative_b.x) : tlast;
			}
		}
		else if (vel_relative_b.x > 0)
		{
			// Case 3 x
			if (aabb1.max.x < aabb2.min.x)
			{
				return false;
			}
			// Case 2 x - Revisited
			if (aabb1.min.x > aabb2.max.x)
			{
				tfirst = ((dfirst.x / vel_relative_b.x) > tfirst) ? (dfirst.x / vel_relative_b.x) : tfirst;
				tlast = ((dlast.x / vel_relative_b.x) < tlast) ? (dlast.x / vel_relative_b.x) : tlast;
			}
		}



		if (vel_relative_b.y < 0)
		{
			// Case 1 y
			if (aabb1.min.y > aabb2.max.y)
			{
				return false;
			}
			// Case 4 y - Revisited
			if (aabb1.max.y < aabb2.min.y)
			{
				tfirst = (dfirst.y / vel_relative_b.y) > tfirst ? (dfirst.y / vel_relative_b.y) : tfirst;
				tlast = (dlast.y / vel_relative_b.y) < tlast ? (dlast.y / vel_relative_b.y) : tlast;
			}
		}
		else if (vel_relative_b.y > 0)
		{
			// Case 3 y
			if (aabb1.max.y < aabb2.min.y)
			{
				return false;
			}
			// Case 2 y - Revisited
			if (aabb1.min.y > aabb2.max.y)
			{
				tfirst = ((dfirst.y / vel_relative_b.y) > tfirst) ? (dfirst.y / vel_relative_b.y) : tfirst;
				tlast = ((dlast.y / vel_relative_b.y) < tlast) ? (dlast.y / vel_relative_b.y) : tlast;
			}
		}


		// Case 5
		if (tfirst > tlast)
		{
			return false;
		}

	}
	else
	{
		return true;
	}
}

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
