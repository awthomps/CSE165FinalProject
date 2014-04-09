#pragma once


#define DEFAULT_BUTTON_TEXT_3 "Left trigger to cancel        Right trigger to select"

#include "clickable.h"

using namespace std;
using namespace osg;
using namespace osgText;

class StaticInstructions :
	public Clickable
{
public:
	StaticInstructions(Vec3d position, Vec4d color);
	StaticInstructions(void);
	~StaticInstructions(void);
	virtual int onClick(int currentState);

private:
	string titleText;
	Text * t_text;
};

