#pragma once


#define DEFAULT_BUTTON_TEXT_1 "Create\nNew Shape"

#include "clickable.h"

using namespace std;
using namespace osg;
using namespace osgText;

class CreateShapeButton :
	public Clickable
{
public:
	CreateShapeButton(Vec3d position, Vec4d color);
	CreateShapeButton(void);
	~CreateShapeButton(void);
	virtual int onClick(int currentState);

private:
	string titleText;
	Text * t_text;
};

