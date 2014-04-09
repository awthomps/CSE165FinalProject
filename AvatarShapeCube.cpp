#include "AvatarShapeCube.h"


AvatarShapeCube::AvatarShapeCube(Vec3d position, Vec4d color)
{
	enabled = true;

	tp_position = new PositionAttitudeTransform();
	tp_position->setPosition(position);
	
	g_shape = new Geode();
	tp_position->addChild(g_shape);

	box = new Box(Vec3d(0,0,0),1,1,1);
	sd_shape = new ShapeDrawable(box);
	g_shape->addDrawable(sd_shape);
	setColor(color);
}

AvatarShapeCube::AvatarShapeCube(void)
{
}


AvatarShapeCube::~AvatarShapeCube(void)
{
}

int AvatarShapeCube::onClick(int currentState) {
		return AVATAR_EDITOR_SHAPE_BOX;
}
