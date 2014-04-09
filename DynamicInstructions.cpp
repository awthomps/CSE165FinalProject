#include "DynamicInstructions.h"


DynamicInstructions::DynamicInstructions(Vec3d position, Vec4d color)
{
	enabled = false;

	titleText = DEFAULT_BUTTON_TEXT_4;

	tp_position = new PositionAttitudeTransform();
	tp_position->setPosition(position);

	t_text = new Text();
	t_text->setText(titleText);
	t_text->setCharacterSize(0.25);

	g_shape = new Geode();
	g_shape->addDrawable(t_text);
	tp_position->addChild(g_shape);

	box = new Box(Vec3d(6.7,0,-1),14.2,1.2,.2);
	sd_shape = new ShapeDrawable(box);
	g_shape->addDrawable(sd_shape);
	setColor(color);
}

DynamicInstructions::DynamicInstructions(void)
{
}


DynamicInstructions::~DynamicInstructions(void)
{
}

int DynamicInstructions::onClick(int currentState) {
		return GAME_S;
}

void DynamicInstructions::changeText(string newText) {
	titleText = newText;
	t_text->setText(newText);
}