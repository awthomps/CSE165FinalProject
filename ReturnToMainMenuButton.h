#pragma once


#define DEFAULT_BUTTON_TEXT "Return to\nMain Menu"

#include "clickable.h"

using namespace std;
using namespace osg;
using namespace osgText;

class ReturnToMainMenuButton :
	public Clickable
{
public:
	ReturnToMainMenuButton(Vec3d position, Vec4d color);
	ReturnToMainMenuButton(void);
	~ReturnToMainMenuButton(void);
	virtual int onClick(int currentState);

private:
	string titleText;
	Text * t_text;
};

