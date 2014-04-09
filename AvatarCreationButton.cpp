#include "AvatarCreationButton.h"


AvatarCreationButton::AvatarCreationButton(Vec3d position, Vec4d color)
{
	enabled = true;

	titleText = DEFAULT_AVATAR_TEXT;

	tp_position = new PositionAttitudeTransform();
	tp_position->setPosition(position);

	t_text = new Text();
	t_text->setText(titleText);
	t_text->setCharacterSize(0.5);

	g_shape = new Geode();
	g_shape->addDrawable(t_text);
	tp_position->addChild(g_shape);

	box = new Box(Vec3d(3.85,0,-1),8,1,1);
	sd_shape = new ShapeDrawable(box);
	g_shape->addDrawable(sd_shape);
	setColor(color);
}

AvatarCreationButton::AvatarCreationButton(void)
{
}


AvatarCreationButton::~AvatarCreationButton(void)
{
}

int AvatarCreationButton::onClick(int currentState) {
	//std::cout << "here" << std::endl;

		return AVATAR_EDITOR_MAIN;
}
