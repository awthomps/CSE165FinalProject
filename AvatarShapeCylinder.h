#pragma once

#include "clickable.h"

using namespace std;
using namespace osg;
using namespace osgText;

class AvatarShapeCylinder :
	public Clickable
{
public:
	AvatarShapeCylinder(Vec3d position, Vec4d color);
	AvatarShapeCylinder(void);
	~AvatarShapeCylinder(void);
	virtual int onClick(int currentState);
};

