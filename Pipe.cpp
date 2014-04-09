#include "Pipe.h"


Pipe::Pipe(Vec3d position)
{
	//root
	t_pipe_root = new PositionAttitudeTransform();
	t_pipe_root->setPosition(position);

	double yCenterOfPipe = OFF_FROM_CENTER + (HEIGHT/2.0);
	Vec3d top_position, bot_position;
	top_position = Vec3(0,yCenterOfPipe,0);
	bot_position = Vec3(0,-yCenterOfPipe,0);

	//top pipe
	t_top_pipe = new PositionAttitudeTransform();
	t_top_pipe->setPosition(top_position);
	t_top_pipe->setAttitude(Quat(sqrt(.5),0,0,sqrt(.5)));
	t_pipe_root->addChild(t_top_pipe);

	g_top_pipe = new Geode();
	ShapeDrawable * cylinderT = new ShapeDrawable(new Cylinder(Vec3d(0.0,0.0,0.0),RADIUS,HEIGHT));
	cylinderT->setColor(PIPE_COLOR);
	g_top_pipe->addDrawable(cylinderT);
	t_top_pipe->addChild(g_top_pipe);

	topBB = g_top_pipe->getBoundingBox();
	
	topBB.set(	topBB.xMin() + position[0] + top_position.x(),
				topBB.zMin() + position[1] + top_position.y(),
				topBB.yMin() + position[2] + top_position.z(),
				topBB.xMax() + position[0] + top_position.x(),
				topBB.zMax() + position[1] + top_position.y(),
				topBB.yMax() + position[2] + top_position.z());
	/*topBB.set(	topBB.xMin() + position[0] + top_position.x(),
				topBB.yMin() + position[1] + top_position.y(),
				topBB.zMin() + position[2] + top_position.z(),
				topBB.xMax() + position[0] + top_position.x(),
				topBB.yMax() + position[1] + top_position.y(),
				topBB.zMax() + position[2] + top_position.z());*/

	//bot pipe
	t_bot_pipe = new PositionAttitudeTransform();
	t_bot_pipe->setAttitude(Quat(sqrt(.5),0,0,sqrt(.5)));
	t_bot_pipe->setPosition(bot_position);
	t_pipe_root->addChild(t_bot_pipe);

	g_bot_pipe = new Geode();
	ShapeDrawable * cylinderB = new ShapeDrawable(new Cylinder(Vec3d(0.0,0.0,0.0),RADIUS,HEIGHT));
	cylinderB->setColor(PIPE_COLOR);
	g_bot_pipe->addDrawable(cylinderB);
	t_bot_pipe->addChild(g_bot_pipe);

	botBB = g_bot_pipe->getBoundingBox();
	
	botBB.set(	botBB.xMin() + position[0] + bot_position.x(),
				botBB.zMin() + position[1] + bot_position.y(),
				botBB.yMin() + position[2] + bot_position.z(),
				botBB.xMax() + position[0] + bot_position.x(),
				botBB.zMax() + position[1] + bot_position.y(),
				botBB.yMax() + position[2] + bot_position.z());
				
	/*botBB.set(	botBB.xMin() + position[0] + bot_position.x(),
				botBB.yMin() + position[1] + bot_position.y(),
				botBB.zMin() + position[2] + bot_position.z(),
				botBB.xMax() + position[0] + bot_position.x(),
				botBB.yMax() + position[1] + bot_position.y(),
				botBB.zMax() + position[2] + bot_position.z());*/
	//cout<< botBB.yMin() << " " << botBB.yMax() <<endl;
}

Pipe::Pipe(void)
{
}


Pipe::~Pipe(void)
{
}


bool Pipe::isColliding(BoundingBox avatar) {
	return (avatar.intersects(topBB) || avatar.intersects(botBB));
}
