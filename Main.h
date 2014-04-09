#ifndef MAIN_H
#define MAIN_H

#define HYDRA_TO_WORLD 0.1
#define HYDRA_TO_SCALE 0.01
#define HYDRA_Y_OFFSET 20
#define HYDRA_Z_OFFSET 20

#define GOAL_COLOR Vec4d(1.0,1.0,0.0,1.0)
#define GOAL_RADIUS 2
#define VALID_PROXIMITY 4.0
#define CIELING_OF_WORLD 30
#define FLOOR_OF_WORLD -30
#define MAX_WORLD_SIZE 400
#define MAX_Y_OFFSET 28
#define DIVISIONS 9
#define CAMERA_DISTANCE_RATIO 20
#define MOVE_SPEED 0.2
#define TURN_RATIO 0.03


//Begin includes:
//std:
#include <Windows.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <cstdlib>
#include <ctime>

//OSG:
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include <osg/Material>
#include <osg/StateSet>
#include <osg/Projection>
#include <osg/NodeVisitor>
#include <osg/CameraNode>
#include <osgGA/NodeTrackerManipulator>
#include <osg/Timer>
#include <osgShadow/ShadowMap>
#include <osgShadow/ShadowedScene>
#include <osg/StateSet>
#include <osg/LightSource>
#include <osg/Light>

//BULLET:
#include "btBulletDynamicsCommon.h"


//HYDRA:
#include <sixense.h>

//OURS:
#include "Avatar.h"
#include "Clickable.h"
#include "TitleTextButton.h"
#include "AvatarCreationButton.h"
#include "StartGameButton.h"
#include "ReturnToMainMenuButton.h"
#include "CreateShapeButton.h"
#include "GoToGameButton.h"
#include "StaticInstructions.h"
#include "DynamicInstructions.h"
#include "AvatarShapeSphere.h"
#include "AvatarShapeCube.h"
#include "AvatarShapeCone.h"
#include "AvatarShapeCylinder.h"
#include "AvatarBodyPart.h"
//#include "ColorTriangle.h"
#include "CubeColorChanger.h"
#include "RotateAvatarButton.h"
#include "States.h"
#include "Pipe.h"
#include "HistoryTracker.h"


//Begin namespaces:
using namespace std;
using namespace osg;
using namespace osgShadow;

//BEGIN PROTOTYPES:
void tester();
void createAvatar();
void setupCursor();
void updateControllerInput();
void updateCursor();
void checkForSelection(vector<Clickable*> currVec);
void checkForClick();
void init();
void initMainMenu();
void initAvatarEditor();
void initGame();
void initGameMenu();
void setupMainMenu();
void setupAvatarEditor();
//void setupGame();
//void setupGameMenu();
void switchMegaState();
void modifyCurrClickableSize();
void rotateCurrClickable();
void initGoal();
void setNewGoalPosition();
void setupNewPipeWorld();
void checkForGoalIntersection();
bool checkForLossConditions();
void cameraFollow();
void turnAvatar();
void applyFlap();
void initArrow();
void updateArrow();


//state machines:
void megaStateMachine();
void mainMenuStateMachine();
void avatarEditorStateMachine();
void gameStateMachine();
void gameMenuStateMachine();


//
Avatar avatar;
Camera *camera;
PositionAttitudeTransform *junk1, *junk2;
PositionAttitudeTransform* world, *tp_cursor, *mainMenuWorld, *avatarEditorWorld, *gameWorld, *gameMenuWorld;
Geode *g_cursor;
Clickable * highlighted;
Vec4d highlightedColor;
int currentState;
Light * light;

//Controller Data:
sixenseControllerData *leftControl, *rightControl;
bool l_trigger_release, r_trigger_release;

vector<Clickable *> mainMenuVec, avatarEditorVec, gameVec, gameMenuVec;

//some globals for avatar editor
vector<Clickable *> shapeButtons, colorChangers;
DynamicInstructions *dynamicInstructions_global;
Clickable *chooseShapeButton, *returnToMainButton, *startGameButton, *rotateButton, *currClickable, *bodyPartBeingModified;//, *colorTriangle_global;
bool isLeftWing, isRightWing;


//Game stuff:
vector<Pipe *> pipes;
Geode * g_goal;
PositionAttitudeTransform * t_goal, *t_Arrow, *t_Arrow_rot;
int scoreForNextPoint, totalScore;
double cameraDistance;
btDiscreteDynamicsWorld * dynamicsWorld;
btCollisionShape* avatarPhysics;
btRigidBody* avatarRigidBody;
HistoryTracker history;

#endif