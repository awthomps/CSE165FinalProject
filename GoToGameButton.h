#pragma once


#define DEFAULT_BUTTON_TEXT_2 "Go to\nGame"

#include "clickable.h"

using namespace std;
using namespace osg;
using namespace osgText;

class GoToGameButton :
	public Clickable
{
public:
	GoToGameButton(Vec3d position, Vec4d color);
	GoToGameButton(void);
	~GoToGameButton(void);
	virtual int onClick(int currentState);

private:
	string titleText;
	Text * t_text;
};

