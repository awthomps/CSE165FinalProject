#ifndef _AVATAR_H_
#define _AVATAR_H_


#define ANIMATION_ROTATION .01
#define NUM_ANIMATIONS 30

#include <Windows.h>
//#include <osgViewer/Viewer>
//#include <osgViewer/ViewerEventHandlers>
#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
//#include <osg/Material>
//#include <osg/StateSet>
//#include <osg/Projection>
//#include <osg/CameraNode>
//#include <osgGA/NodeTrackerManipulator>
//#include <osg/Timer>

//ours:
#include "Clickable.h"
#include "AvatarBodyPart.h"

#include <vector>

using namespace osg;

class Avatar {
public:
Avatar();
	/*
osg::PositionAttitudeTransform* getAvatar_Body();
PositionAttitudeTransform* getAvatar_RightWing();
PositionAttitudeTransform* getAvatar_LeftWing();*/
PositionAttitudeTransform* getRoot() {return avatar_root;}
void createDefaultAvatar();
void updateBodyPartPATs();
void convertCursorToAvatarLocal(PositionAttitudeTransform * position, Clickable* currBodyPart);
void computeBoundingBox();
void saveWings();
void resetWings();
void animateWings();

vector<Clickable *> bodyParts;
BoundingBox bb;
Clickable *c_body, *c_l_wing, *c_r_wing;
PositionAttitudeTransform *t_l_wing_root, *t_r_wing_root;

private:
PositionAttitudeTransform *avatar_root, *t_body, *t_l_wing, *t_r_wing;
Quat lOriginal, rOriginal;
int animationPos;
bool up;

};

#endif