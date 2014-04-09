#include "AvatarBodyPart.h"


AvatarBodyPart::AvatarBodyPart(Vec3d position, Vec4d color)
{
	enabled = false;

	tp_position = new PositionAttitudeTransform();
	tp_position->setPosition(position);
	
	g_shape = new Geode();
	tp_position->addChild(g_shape);

	setColor(color);
}

AvatarBodyPart::AvatarBodyPart(void)
{
}


AvatarBodyPart::~AvatarBodyPart(void)
{
}
