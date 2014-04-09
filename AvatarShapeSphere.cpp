#include "AvatarShapeSphere.h"


AvatarShapeSphere::AvatarShapeSphere(Vec3d position, Vec4d color)
{
	enabled = true;

	tp_position = new PositionAttitudeTransform();
	tp_position->setPosition(position);
	
	g_shape = new Geode();
	tp_position->addChild(g_shape);

	sphere = new Sphere(Vec3d(0,0,0),.5);
	sd_shape = new ShapeDrawable(sphere);
	g_shape->addDrawable(sd_shape);
	setColor(color);
}

AvatarShapeSphere::AvatarShapeSphere(void)
{
}


AvatarShapeSphere::~AvatarShapeSphere(void)
{
}

int AvatarShapeSphere::onClick(int currentState) {
		return AVATAR_EDITOR_SHAPE_SPHERE;
}
