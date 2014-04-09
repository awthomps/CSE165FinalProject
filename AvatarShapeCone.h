#pragma once

#include "clickable.h"

using namespace std;
using namespace osg;
using namespace osgText;

class AvatarShapeCone :
	public Clickable
{
public:
	AvatarShapeCone(Vec3d position, Vec4d color);
	AvatarShapeCone(void);
	~AvatarShapeCone(void);
	virtual int onClick(int currentState);
};

