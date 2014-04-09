#pragma once


#define DEFAULT_GAME_TEXT "Start Game"

#include "clickable.h"

using namespace std;
using namespace osg;
using namespace osgText;

class StartGameButton :
	public Clickable
{
public:
	StartGameButton(Vec3d position, Vec4d color);
	StartGameButton(void);
	virtual int onClick(int currentState);
	~StartGameButton(void);

private:
	string titleText;
	Text * t_text;
};

