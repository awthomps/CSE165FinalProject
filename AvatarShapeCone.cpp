#include "AvatarShapeCone.h"


AvatarShapeCone::AvatarShapeCone(Vec3d position, Vec4d color)
{
	enabled = true;

	tp_position = new PositionAttitudeTransform();
	tp_position->setPosition(position);
	
	g_shape = new Geode();
	tp_position->addChild(g_shape);

	cone = new Cone(Vec3d(0,0,0),.5,1);
	sd_shape = new ShapeDrawable(cone);
	g_shape->addDrawable(sd_shape);
	setColor(color);
}

AvatarShapeCone::AvatarShapeCone(void)
{
}


AvatarShapeCone::~AvatarShapeCone(void)
{
}

int AvatarShapeCone::onClick(int currentState) {
		return AVATAR_EDITOR_SHAPE_CONE;
}
