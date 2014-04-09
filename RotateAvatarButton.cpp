#include "RotateAvatarButton.h"


RotateAvatarButton::RotateAvatarButton(Vec3d position, Vec4d color)
{
	enabled = true;

	titleText = DEFAULT_BUTTON_TEXT_ROTATE;

	tp_position = new PositionAttitudeTransform();
	tp_position->setPosition(position);

	t_text = new Text();
	t_text->setText(titleText);
	t_text->setCharacterSize(0.25);

	g_shape = new Geode();
	g_shape->addDrawable(t_text);
	tp_position->addChild(g_shape);

	box = new Box(Vec3d(0.85,0,-1),2.6,1,1);
	sd_shape = new ShapeDrawable(box);
	g_shape->addDrawable(sd_shape);
	setColor(color);
}

RotateAvatarButton::RotateAvatarButton(void)
{
}


RotateAvatarButton::~RotateAvatarButton(void)
{
}

int RotateAvatarButton::onClick(int currentState) {
		return AVATAR_EDITOR_ROTATE_SHAPE_PROLOGUE;
}
