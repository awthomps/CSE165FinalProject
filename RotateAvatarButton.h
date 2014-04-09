#pragma once


#define DEFAULT_BUTTON_TEXT_ROTATE "Rotate\nAvatar"

#include "clickable.h"

using namespace std;
using namespace osg;
using namespace osgText;

class RotateAvatarButton :
	public Clickable
{
public:
	RotateAvatarButton(Vec3d position, Vec4d color);
	RotateAvatarButton(void);
	~RotateAvatarButton(void);
	virtual int onClick(int currentState);

private:
	string titleText;
	Text * t_text;
};

