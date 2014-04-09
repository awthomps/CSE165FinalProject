#include "CubeColorChanger.h"


CubeColorChanger::CubeColorChanger(Vec3d position, int color)
{
	enabled = true;
	tp_position = new PositionAttitudeTransform();
	tp_position->setPosition(position);
	tp_position->setAttitude(Quat(sqrt(.5),0,0,(sqrt(.5))));
	
	g_shape = new Geode();
	tp_position->addChild(g_shape);

	capsule = new Capsule(Vec3d(0,0,0),.5,1);
	sd_shape = new ShapeDrawable(capsule);
	g_shape->addDrawable(sd_shape);

	myColor = color;
	switch(myColor)
	{
	case COLOR_RED:
		setColor(Vec4d(0.8,0.0,0.0,1.0));
		break;
	case COLOR_GREEN:
		setColor(Vec4d(0.0,0.8,0.0,1.0));
		break;
	case COLOR_BLUE:
		setColor(Vec4d(0.0,0.0,0.8,1.0));
		break;
	}

	colorValue = MAX_COLOR_VALUE * 0.8;
}

CubeColorChanger::CubeColorChanger(void)
{
}


CubeColorChanger::~CubeColorChanger(void)
{
}

Vec4d CubeColorChanger::handleHighlighted(double delta, Vec4 objectColor) {
	if(colorValue + delta >= MAX_COLOR_VALUE) colorValue = MAX_COLOR_VALUE;
	else if(colorValue + delta <= 0.0) colorValue = 0;
	else colorValue += delta;

	Vec4d colorToSet;
	switch(myColor)
	{
	case COLOR_RED:
		colorToSet = Vec4d(colorValue/MAX_COLOR_VALUE,0.0,0.0,1.0);
		objectColor[0] = colorValue/MAX_COLOR_VALUE;
		break;
	case COLOR_GREEN:
		colorToSet = Vec4d(0.0,colorValue/MAX_COLOR_VALUE,0.0,1.0);
		objectColor[1] = colorValue/MAX_COLOR_VALUE;
		break;
	case COLOR_BLUE:
		colorToSet = Vec4d(0.0,0.0,colorValue/MAX_COLOR_VALUE,1.0);
		objectColor[2] = colorValue/MAX_COLOR_VALUE;
		break;
	default:
		colorToSet = Vec4d(1.0,1.0,1.0,1.0);
		break;
	}

	setColor(colorToSet);
	return objectColor;
}


void CubeColorChanger::hasHighlight() {
	tp_position->setPosition(tp_position->getPosition() + HIGHLIGHT_VEC);
}

void CubeColorChanger::loseHighlight() {
	tp_position->setPosition(tp_position->getPosition() - HIGHLIGHT_VEC);
}