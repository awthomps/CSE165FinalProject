#pragma once

#define COLOR_RED 0
#define COLOR_GREEN 1
#define COLOR_BLUE 2
#define MAX_COLOR_VALUE 255.0
#define HIGHLIGHT_VEC Vec3d(0,0.2,0)

#include "clickable.h"

class CubeColorChanger :
	public Clickable
{
public:
	CubeColorChanger(void);
	CubeColorChanger(Vec3d position, int color);
	~CubeColorChanger(void);


	virtual bool isColorChanger() {return true;}
	void hasHighlight();
	void loseHighlight();
	Vec4d handleHighlighted(double delta, Vec4 objectColor);
	
	

private:
	double colorValue;
	int myColor;
};

