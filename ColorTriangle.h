#pragma once
#include "clickable.h"
class ColorTriangle :
	public Clickable
{
public:
	ColorTriangle(Vec3d position, Vec4d color);
	~ColorTriangle(void);

	Vec4d currentColor(PositionAttitudeTransform * cursor);
};

