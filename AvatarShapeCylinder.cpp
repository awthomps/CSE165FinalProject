#include "AvatarShapeCylinder.h"


AvatarShapeCylinder::AvatarShapeCylinder(Vec3d position, Vec4d color)
{
	enabled = true;

	tp_position = new PositionAttitudeTransform();
	tp_position->setPosition(position);
	
	g_shape = new Geode();
	tp_position->addChild(g_shape);

	cylinder = new Cylinder(Vec3d(0,0,0),.5,1);
	sd_shape = new ShapeDrawable(cylinder);
	g_shape->addDrawable(sd_shape);
	setColor(color);
}

AvatarShapeCylinder::AvatarShapeCylinder(void)
{
}


AvatarShapeCylinder::~AvatarShapeCylinder(void)
{
}

int AvatarShapeCylinder::onClick(int currentState) {
		return AVATAR_EDITOR_SHAPE_CYLINDER;
}
