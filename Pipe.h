#pragma once

//Constants/Macros:
#define OFF_FROM_CENTER 8.0
#define HEIGHT 30
#define RADIUS 6
#define PIPE_COLOR Vec4d(0.0,0.9,0.2,1.0)

//std:
#include <Windows.h>
#include <iostream>

//osg:
#include <osg/PositionAttitudeTransform>
#include <osg/ShapeDrawable>
#include <osg/Geode>

using namespace osg;
using namespace std;


class Pipe
{
public:
	Pipe(Vec3d position);
	Pipe(void);
	~Pipe(void);
	
	PositionAttitudeTransform * getRoot() { return t_pipe_root; }
	Vec3d getPosition() { return t_pipe_root->getPosition(); }
	bool isColliding(BoundingBox avatar);


private:
	PositionAttitudeTransform *t_pipe_root, *t_top_pipe, *t_bot_pipe;
	Geode *g_top_pipe, *g_bot_pipe;
	BoundingBox botBB, topBB;
};

