#pragma once

#include "clickable.h"

using namespace std;
using namespace osg;
using namespace osgText;

class AvatarBodyPart :
	public Clickable
{
public:
	AvatarBodyPart(Vec3d position, Vec4d color);
	AvatarBodyPart(void);
	~AvatarBodyPart(void);
	virtual int onClick(int currentState) {return AVATAR_EDITOR_CHANGE_SHAPE_PROLOGUE;}
};

