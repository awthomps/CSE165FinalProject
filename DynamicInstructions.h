#pragma once


#define DEFAULT_BUTTON_TEXT_4 "Select body part to edit\nOr make new body part"

#include "clickable.h"

using namespace std;
using namespace osg;
using namespace osgText;

class DynamicInstructions :
	public Clickable
{
public:
	DynamicInstructions(Vec3d position, Vec4d color);
	DynamicInstructions(void);
	~DynamicInstructions(void);
	virtual int onClick(int currentState);
	void changeText(string newText);

private:
	string titleText;
	Text * t_text;
};

