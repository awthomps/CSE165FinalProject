#pragma once

#define DEFAULT_MAIN_TITLE "Flappy Bird 3D"

#include "clickable.h"


using namespace std;
using namespace osg;
using namespace osgText;

class TitleTextButton :
	public Clickable
{
public:
	TitleTextButton(Vec3d position);
	TitleTextButton(void);
	~TitleTextButton(void);

private:
	string titleText;
	Text * t_text;
};

