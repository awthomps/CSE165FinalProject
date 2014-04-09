#pragma once


#define DEFAULT_AVATAR_TEXT "Avatar Creation"

#include "clickable.h"

using namespace std;
using namespace osg;
using namespace osgText;

class AvatarCreationButton :
	public Clickable
{
public:
	AvatarCreationButton(Vec3d position, Vec4d color);
	AvatarCreationButton(void);
	~AvatarCreationButton(void);
	virtual int onClick(int currentState);

private:
	string titleText;
	Text * t_text;
};

