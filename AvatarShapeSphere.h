#pragma once

#include "clickable.h"

using namespace std;
using namespace osg;
using namespace osgText;

class AvatarShapeSphere :
	public Clickable
{
public:
	AvatarShapeSphere(Vec3d position, Vec4d color);
	AvatarShapeSphere(void);
	~AvatarShapeSphere(void);
	virtual int onClick(int currentState);
};

