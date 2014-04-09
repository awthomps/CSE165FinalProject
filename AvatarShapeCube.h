#pragma once

#include "clickable.h"

using namespace std;
using namespace osg;
using namespace osgText;

class AvatarShapeCube :
	public Clickable
{
public:
	AvatarShapeCube(Vec3d position, Vec4d color);
	AvatarShapeCube(void);
	~AvatarShapeCube(void);
	virtual int onClick(int currentState);
};

