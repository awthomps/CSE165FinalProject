#include "TitleTextButton.h"


TitleTextButton::TitleTextButton(Vec3d position)
{
	enabled = false;

	titleText = DEFAULT_MAIN_TITLE;

	tp_position = new PositionAttitudeTransform();
	tp_position->setPosition(position);

	t_text = new Text();
	t_text->setText(titleText);
	t_text->setCharacterSize(1);

	g_shape = new Geode();
	g_shape->addDrawable(t_text);
	tp_position->addChild(g_shape);
}

TitleTextButton::TitleTextButton(void)
{
}


TitleTextButton::~TitleTextButton(void)
{
}
