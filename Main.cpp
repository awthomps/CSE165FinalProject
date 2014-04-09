#include "Main.h"


int main() {
	if(sixenseInit() == SIXENSE_SUCCESS) {
		cout<< "Sixense Initialized!" <<endl;
		//initialize controllers
	leftControl = (sixenseControllerData*) malloc(sizeof(sixenseControllerData));
	rightControl = (sixenseControllerData*) malloc(sizeof(sixenseControllerData));
	}
	else cout<<"What? why didn't sixense get initialized?" << endl;
	
	//Creating the viewer	
	osg::ref_ptr<osgViewer::Viewer> viewer(new osgViewer::Viewer);
	// Window size. Comment this out if you want full screen.
	viewer->setUpViewInWindow(100,100,800,600);

	//Creating the root node
	osg::ref_ptr<osg::Group> root (new osg::Group);

	world = new PositionAttitudeTransform();
	

	//SHADOW JUNK
	ShadowMap * shadowMap = new ShadowMap();
	ShadowedScene * shadowScene = new ShadowedScene(shadowMap);
	shadowScene->addChild(world);
	root->addChild(shadowScene);
	

	
	// set root node to viewer
	viewer->setSceneData( root.get() ); 

	// Windows size handler
	viewer->addEventHandler(new osgViewer::WindowSizeHandler);

	// Get camera from viewer
	camera = viewer->getCamera();

	// Get light from viewer
	light = viewer->getLight();
	light->setPosition(Vec4(0,0,CIELING_OF_WORLD,0.0));
	light->setDirection(Vec3(0.2,-1,0));
	light->setAmbient(Vec4d(0.5,0.5,0.5,1.0));
	shadowMap->setLight(light);

	LightSource * lightSource = new LightSource();
	lightSource->setLight(light);
	lightSource->setLocalStateSetModes(StateAttribute::ON);
	shadowScene->addChild(lightSource);

	

	//setup globals:
	init();

	// The main loop
	viewer->realize();

	while(!viewer->done()) {
		updateControllerInput();
		updateCursor();
		megaStateMachine();
		//cout << "light " << viewer->getLight()->getDirection()[0] << " " << viewer->getLight()->getDirection()[1] << " " << viewer->getLight()->getDirection()[2] << endl;

		viewer->frame(); // render frame
	}

	if(sixenseExit() == SIXENSE_SUCCESS) {
		cout<< "Sixense Exited!" <<endl;
	}
	else cout<<"Well I guess it failed on exiting?" << endl;
	return 0;
}

void setupCursor() {
	tp_cursor = new PositionAttitudeTransform();
	g_cursor = new Geode();
	Sphere * cursor_sphere = new osg::Sphere();
	ShapeDrawable * cursor_sd = new osg::ShapeDrawable(cursor_sphere);
	//cursor_sd->setColor(Vec4d(0.439, 0.545, 0.565, 1.0));
	cursor_sd->setColor(Vec4d(0.8, 0.8, 0.8, 1.0));
	cursor_sphere->setRadius(0.3);
	g_cursor->addDrawable(cursor_sd);

	tp_cursor->addChild(g_cursor);
	//world->addChild(tp_cursor);
}

void updateCursor() {
	if((currentState & MEGA_MASK) == GAME_S)
		return;

	double floor = -5;
	Vec3d newPos;

	/*
	if(HYDRA_TO_WORLD*rightControl->pos[1] < floor) {
		newPos = Vec3d(HYDRA_TO_WORLD*rightControl->pos[0],floor,HYDRA_TO_WORLD*rightControl->pos[2]- HYDRA_Z_OFFSET);
	}*/
	newPos = Vec3d(HYDRA_TO_WORLD*rightControl->pos[0],HYDRA_TO_WORLD*rightControl->pos[1] - HYDRA_Y_OFFSET,HYDRA_TO_WORLD*rightControl->pos[2] - HYDRA_Z_OFFSET);

	//tp_cursor->setPosition(newPos);
	//calculate world position of newPos 
	Matrixd worldMatrix, cursorMatrix; 
	world->computeLocalToWorldMatrix(worldMatrix,NULL); 
	tp_cursor->computeLocalToWorldMatrix(cursorMatrix,NULL); 
	cursorMatrix.postMult(worldMatrix); cursorMatrix.invert(cursorMatrix); 
	newPos = cursorMatrix.preMult(newPos); 
	tp_cursor->setPosition((newPos + tp_cursor->getPosition()) * .5);
}


void updateControllerInput()
{
	static double l_trigger_val_last = 0.0;
	static double r_trigger_val_last = 0.0;

	if((sixenseGetNewestData(0,leftControl) == SIXENSE_SUCCESS) && (sixenseGetNewestData(1,rightControl) == SIXENSE_SUCCESS))
	{
		/*
		cout<<"position:"<<endl;
		cout<< "left (" << (int) leftControl->pos[0] << ", "<< (int) leftControl->pos[1] << ", "<< (int) leftControl->pos[2] << ") "<<endl;
		cout<< "right? (" << (int) rightControl->pos[0] << ", "<< (int) rightControl->pos[1] << ", "<< (int) rightControl->pos[2] << ")"<<endl;
		*/

		if(l_trigger_val_last >0.0 && leftControl->trigger == 0.0) l_trigger_release = true;
		else l_trigger_release = false;

		if(r_trigger_val_last >0.0 && rightControl->trigger == 0.0) r_trigger_release = true;
		else r_trigger_release = false;

		l_trigger_val_last = leftControl->trigger;
		r_trigger_val_last = rightControl->trigger;
		//cout<< "l_trigger: " << l_trigger_release << endl;
		//cout<< "r_trigger: " << r_trigger_release << endl;
	}
	else
	{
		cout<<"Not getting controller data!" <<endl;
	}
}




//Initialization:
void init() {
	//set up camera
	//camera setup
	Vec3d cam_eye = Vec3d(0,0,20);
	Vec3d cam_center = Vec3d(0,0,0);
	Vec3d cam_up = osg::Vec3d(0.0,1.0,0.0);
	camera->setViewMatrixAsLookAt(cam_eye,cam_center,cam_up);

	l_trigger_release, l_trigger_release;

	//avatar stuff
	avatar.createDefaultAvatar();

	setupCursor();
	/*
	junk1 = new PositionAttitudeTransform();
	junk2 = new PositionAttitudeTransform();
	junk1->addChild(junk2);
	junk2->addChild(junk1);
	junk1->addChild(avatar.getAvatar_Body());
	junk1->addChild(tp_cursor);
	*/
	//initialize state machine
	currentState = MAIN_MENU_S;

	//setup the first state menu:
	setupMainMenu();
	//setupAvatarEditor();

	/*
	//init other mega states
	initAvatarEditor();
	initGame();
	initGameMenu();*/
	bodyPartBeingModified = 0;
	isLeftWing = false;
	isRightWing = false;
}


/*
 * Menus/Worlds setup:
 */

void initMainMenu() {
	//setup main menu world
	mainMenuWorld = new PositionAttitudeTransform();

	avatar.getRoot()->setAttitude(Quat(0.0,0.0,0.0,1.0));

	//init vector array of clickables
	mainMenuVec = vector<Clickable *>();
	mainMenuVec.clear();

	//add avatar to world:
	//world->addChild(avatar.getAvatar_Body());
	
	//clickables
	Vec3d titlePosition = Vec3d(-7,3,0);
	Clickable * title = new TitleTextButton(titlePosition);
	mainMenuVec.push_back(title);

	Vec3d avatarCreationButtonPos = Vec3d(-3.8,-3.2,0);
	Clickable * avatarCreationButton = new AvatarCreationButton(avatarCreationButtonPos,Vec4d(0.0,1.0,0.0,1.0));
	mainMenuVec.push_back(avatarCreationButton);
	
	Vec3d startGameButtonPos = Vec3d(-3.8,-4.7,0);
	Clickable * startGameButton = new StartGameButton(startGameButtonPos,Vec4d(0.0,1.0,1.0,1.0));
	mainMenuVec.push_back(startGameButton);
	//end clickables

	mainMenuWorld->addChild(tp_cursor);
	mainMenuWorld->addChild(avatar.getRoot());
	/*
	for(unsigned int i = 0; i < avatar.bodyParts.size(); ++i) {
		//mainMenuWorld->addChild(avatar.bodyParts[i]->getPat());
		mainMenuVec.push_back(avatar.bodyParts[i]);
	}*/
	
	for(unsigned int i = 0; i < mainMenuVec.size(); ++i) {
		mainMenuWorld->addChild(mainMenuVec[i]->getPat());
		//junk1->addChild(mainMenuVec[i]->getPat());
	}

	for(unsigned int i = 0; i < avatar.bodyParts.size(); ++i) {
		avatar.bodyParts[i]->disable();
	}
}

void setupMainMenu() {
	initMainMenu();
	Group * root = world->getParent(0);

	/*
	if(mainMenuWorld->getNumChildren() <= 0) {
		mainMenuWorld->addChild(tp_cursor);
		mainMenuWorld->addChild(avatar.getAvatar_Body());
		for(unsigned int i = 0; i < mainMenuVec.size(); ++i) {
			mainMenuWorld->addChild(mainMenuVec[i]->getPat());
		}
	}*/

	root->addChild(mainMenuWorld);

	root->removeChild(world);
	world = mainMenuWorld;
}

void initAvatarEditor() {
	//setup main menu world
	avatarEditorWorld = new PositionAttitudeTransform();

	avatarEditorVec = vector<Clickable *>();
	avatarEditorVec.clear();
	shapeButtons = vector<Clickable *>();
	shapeButtons.clear();
	colorChangers = vector<Clickable *>();
	colorChangers.clear();

	avatarEditorWorld->addChild(tp_cursor);
	avatarEditorWorld->addChild(avatar.getRoot());
	/*
	for(unsigned int i = 0; i < avatar.bodyParts.size(); ++i) {
		//avatarEditorWorld->addChild(avatar.bodyParts[i]->getPat());
		avatarEditorVec.push_back(avatar.bodyParts[i]);
	}*/

	//clickables
	Vec3d returnToMainMenuPos = Vec3d(-7,4.5,0);
	Clickable * returnToMainMenuButton = new ReturnToMainMenuButton(returnToMainMenuPos,Vec4d(0.0,1.0,0.0,1.0));
	avatarEditorVec.push_back(returnToMainMenuButton);
	returnToMainButton = returnToMainMenuButton;
	
	Clickable * rotateAvatarButton = new RotateAvatarButton(Vec3d(-3,4.5,0.0),Vec4d(0.7,0.3,0.9,1.0));
	avatarEditorVec.push_back(rotateAvatarButton);
	rotateButton = rotateAvatarButton;
		
	Vec3d createShapeButtonPos = Vec3d(1,4.5,0);
	Clickable * createShapeButton = new CreateShapeButton(createShapeButtonPos,Vec4d(0.0,1.0,1.0,1.0));
	avatarEditorVec.push_back(createShapeButton);
	chooseShapeButton = createShapeButton;
		
	Vec3d GoToGameButtonPos = Vec3d(5,4.5,0);
	Clickable * goToGameButton = new GoToGameButton(GoToGameButtonPos,Vec4d(1.0,0.0,1.0,1.0));
	avatarEditorVec.push_back(goToGameButton);
	startGameButton = goToGameButton;
		
	Vec3d staticInstructionsPos = Vec3d(-6.8,-5.0,0);
	Clickable * staticInstructions = new StaticInstructions(staticInstructionsPos,Vec4d(1.0,1.0,1.0,1.0));
	avatarEditorVec.push_back(staticInstructions);
		
	Vec3d dynamicInstructionsPos = Vec3d(-6.8,-3.9,0);
	Clickable * dynamicInstructions = new DynamicInstructions(dynamicInstructionsPos,Vec4d(1.0,1.0,1.0,1.0));
	avatarEditorVec.push_back(dynamicInstructions);
	dynamicInstructions_global = (DynamicInstructions*)dynamicInstructions;

	Vec3 avatarShapeSpherePos = Vec3(-3.0,0.0,0.0);
	Clickable * avatarShapeSphere = new AvatarShapeSphere(avatarShapeSpherePos,Vec4d(.5,.5,.5,1.0));
	avatarEditorVec.push_back(avatarShapeSphere);
	shapeButtons.push_back(avatarShapeSphere);
	avatarShapeSphere->goAway();

	Vec3 avatarShapeCubePos = Vec3(-1.0,0.0,0.0);
	Clickable * avatarShapeCube = new AvatarShapeCube(avatarShapeCubePos,Vec4d(.5,.5,.5,1.0));
	avatarEditorVec.push_back(avatarShapeCube);
	shapeButtons.push_back(avatarShapeCube);
	avatarShapeCube->goAway();

	Vec3 avatarShapeConePos = Vec3(1.0,-.3,0.0);
	Clickable * avatarShapeCone = new AvatarShapeCone(avatarShapeConePos,Vec4d(.5,.5,.5,1.0));
	avatarShapeCone->rotate(Quat(-sqrt(.5),0.0,0.0,sqrt(.5)));
	avatarEditorVec.push_back(avatarShapeCone);
	shapeButtons.push_back(avatarShapeCone);
	avatarShapeCone->goAway();

	Vec3 avatarShapeCylinderPos = Vec3(3.0,0.0,0.0);
	Clickable * avatarShapeCylinder = new AvatarShapeCylinder(avatarShapeCylinderPos,Vec4d(.5,.5,.5,1.0));
	avatarShapeCylinder->rotate(Quat(sqrt(.5),0.0,0.0,sqrt(.5)));
	avatarEditorVec.push_back(avatarShapeCylinder);
	shapeButtons.push_back(avatarShapeCylinder);
	avatarShapeCylinder->goAway();

	/*
	Clickable *colorTriangle = new ColorTriangle(Vec3(5,0,0),Vec4(0,0,0,0));
	avatarEditorVec.push_back(colorTriangle);
	colorTriangle->goAway();
	colorTriangle_global = colorTriangle;*/
	Clickable * colorChanger_r = new CubeColorChanger(Vec3d(5,0,0),0);
	avatarEditorVec.push_back(colorChanger_r);
	colorChangers.push_back(colorChanger_r);
	colorChanger_r->goAway();

	Clickable * colorChanger_g = new CubeColorChanger(Vec3d(4,0,0),1);
	avatarEditorVec.push_back(colorChanger_g);
	colorChangers.push_back(colorChanger_g);
	colorChanger_g->goAway();

	Clickable * colorChanger_b = new CubeColorChanger(Vec3d(3,0,0),2);
	avatarEditorVec.push_back(colorChanger_b);
	colorChangers.push_back(colorChanger_b);
	colorChanger_b->goAway();

	//end clickables

	for(unsigned int i = 0; i < avatarEditorVec.size(); ++i) {
		avatarEditorWorld->addChild(avatarEditorVec[i]->getPat());
		//junk1->addChild(mainMenuVec[i]->getPat());
	}
	for(unsigned int i = 0; i < avatar.bodyParts.size(); ++i) {
		avatar.bodyParts[i]->enable();
	}
}

void setupAvatarEditor() {
	initAvatarEditor();

	world->getParent(0)->addChild(avatarEditorWorld);
	world->getParent(0)->removeChild(world);
	world = avatarEditorWorld;
}

/*
void initGame() {
	gameVec = vector<Clickable *>();
	gameVec.clear();
}

void setupGame() {
	world->removeChildren(0,world->getNumChildren());

	//world->addChild(avatar.getAvatar_Body());
	world->addChild(avatar.getRoot());
	for(unsigned int i = 0; i < gameVec.size(); ++i) {
		world->addChild(gameVec[i]->getPat());
	}
}
*/

void initGameMenu() {
	gameMenuVec = vector<Clickable *>();
	gameMenuVec.clear();
}

void setupGameMenu() {
	world->removeChildren(0,world->getNumChildren());

	world->addChild(tp_cursor);
	for(unsigned int i = 0; i < gameMenuVec.size(); ++i) {
		world->addChild(gameMenuVec[i]->getPat());
	}
}

/*
 * State Machines:
 */
void megaStateMachine(){
	if((currentState & MEGA_MASK) == MAIN_MENU_S) {
		mainMenuStateMachine();
	}
	else if((currentState & MEGA_MASK) == AVATAR_EDITOR_S) {
		avatarEditorStateMachine();
	}
	else if((currentState & MEGA_MASK) == GAME_S) {
		gameStateMachine();
	}
	else if((currentState & MEGA_MASK) == GAME_MENU_S) {
		gameMenuStateMachine();
	}
}

void mainMenuStateMachine() {
	checkForSelection(mainMenuVec);
	//checkForSelection(avatar.bodyParts);
	checkForClick();
}

void avatarEditorStateMachine() {
	checkForSelection(avatarEditorVec);
	//checkForSelection(avatar.bodyParts);
	checkForClick();

	switch(currentState) {
		case AVATAR_EDITOR_CREATE_SHAPE_PROLOGUE:
			currentState = AVATAR_EDITOR_CREATE_SHAPE;
			//avatar.getAvatar_Body()->setPosition(avatar.getAvatar_Body()->getPosition() + Vec3(0,1000,0));
			for(unsigned int i = 0; i < avatar.bodyParts.size(); ++i) {
				avatar.bodyParts[i]->goAway();
				avatar.bodyParts[i]->disable();
			}
			chooseShapeButton->disable();
			chooseShapeButton->goAway();
			returnToMainButton->disable();
			returnToMainButton->goAway();
			startGameButton->disable();
			startGameButton->goAway();
			rotateButton->disable();
			rotateButton->goAway();
			break;
		case AVATAR_EDITOR_CREATE_SHAPE:
			for(unsigned int i = 0; i < shapeButtons.size(); ++i) {
				shapeButtons[i]->comeBack();
			}
			dynamicInstructions_global->changeText("Select a shape for\nyour new body part");
			currentState = AVATAR_EDITOR_CHOOSE_SHAPE;
			break;
		case AVATAR_EDITOR_CHOOSE_SHAPE:
			if(l_trigger_release) {
				chooseShapeButton->enable();
				chooseShapeButton->comeBack();
				returnToMainButton->enable();
				returnToMainButton->comeBack();
				startGameButton->enable();
				startGameButton->comeBack();
				rotateButton->enable();
				rotateButton->comeBack();
				
				currentState = AVATAR_EDITOR_S;
				dynamicInstructions_global->changeText("Select body part to edit\nOr make new body part");
				//avatar.getAvatar_Body()->setPosition(avatar.getAvatar_Body()->getPosition() - Vec3(0,1000,0));				
				for(unsigned int i = 0; i < avatar.bodyParts.size(); ++i) {
					avatar.bodyParts[i]->comeBack();
					avatar.bodyParts[i]->enable();
				}
				for(unsigned int i = 0; i < shapeButtons.size(); ++i) {
					shapeButtons[i]->goAway();
				}
				break;
			}

			break;
		case AVATAR_EDITOR_SHAPE_BOX:
			currentState = AVATAR_EDITOR_MODIFY_SHAPE_SIZE_PROLOGUE;
			currClickable = new AvatarBodyPart();
			currClickable->changeToCube();
			currClickable->disable();
			currClickable->setColor(Vec4d(colorChangers[0]->getColor()[0],colorChangers[1]->getColor()[1],colorChangers[2]->getColor()[2],1.0));
			//currClickable->setColor(Vec4d(0.8,0.8,0.8,1.0));
			break;
		case AVATAR_EDITOR_SHAPE_CYLINDER:
			currentState = AVATAR_EDITOR_MODIFY_SHAPE_SIZE_PROLOGUE;
			currClickable = new AvatarBodyPart();
			currClickable->changeToCylinder();
			currClickable->disable();
			currClickable->setColor(Vec4d(colorChangers[0]->getColor()[0],colorChangers[1]->getColor()[1],colorChangers[2]->getColor()[2],1.0));
			//currClickable->setColor(Vec4d(0.8,0.8,0.8,1.0));
			break;
		case AVATAR_EDITOR_SHAPE_CONE:
			currentState = AVATAR_EDITOR_MODIFY_SHAPE_SIZE_PROLOGUE;
			currClickable = new AvatarBodyPart();
			currClickable->changeToCone();
			currClickable->disable();
			currClickable->setColor(Vec4d(colorChangers[0]->getColor()[0],colorChangers[1]->getColor()[1],colorChangers[2]->getColor()[2],1.0));
			//currClickable->setColor(Vec4d(0.8,0.8,0.8,1.0));
			break;
		case AVATAR_EDITOR_SHAPE_SPHERE:
			currentState = AVATAR_EDITOR_MODIFY_SHAPE_SIZE_PROLOGUE;
			currClickable = new AvatarBodyPart();
			currClickable->changeToSphere();
			currClickable->disable();
			currClickable->setColor(Vec4d(colorChangers[0]->getColor()[0],colorChangers[1]->getColor()[1],colorChangers[2]->getColor()[2],1.0));
			//currClickable->setColor(Vec4d(0.8,0.8,0.8,1.0));
			break;
		case AVATAR_EDITOR_MODIFY_SHAPE_SIZE_PROLOGUE:
			avatarEditorVec.push_back(currClickable);
			avatarEditorWorld->addChild(avatarEditorVec[avatarEditorVec.size()-1]->getPat());
			for(unsigned int i = 0; i < shapeButtons.size(); ++i) {
				shapeButtons[i]->goAway();
			}
			currentState = AVATAR_EDITOR_MODIFY_SHAPE_SIZE;
			dynamicInstructions_global->changeText("The min and max positions of the Hydra\nControllers represent the extents of the object");
			break;
		case AVATAR_EDITOR_MODIFY_SHAPE_SIZE:
			if(r_trigger_release) {
				currentState = AVATAR_EDITOR_MODIFY_SHAPE_COLOR;
				dynamicInstructions_global->changeText("Modify the object color by selecting a capsule\nAnd pressing up or down on the right controller");
				for(unsigned int i = 0; i < colorChangers.size(); ++i) {
					colorChangers[i]->comeBack();
				}
			}
			else if(l_trigger_release) {
				currentState = AVATAR_EDITOR_CREATE_SHAPE;
				avatarEditorWorld->removeChild(avatarEditorVec[avatarEditorVec.size()-1]->getPat());
				avatarEditorVec.pop_back();
				currClickable = 0;
			}
			
			modifyCurrClickableSize();
			
			break;
		case AVATAR_EDITOR_MODIFY_SHAPE_COLOR:
			if(r_trigger_release) {
				currentState = AVATAR_EDITOR_MODIFY_SHAPE_ROTATE;
				dynamicInstructions_global->changeText("Rotate the right controller into the\nDesired orientation for this object");
				for(unsigned int i = 0; i < colorChangers.size(); ++i) {
					colorChangers[i]->goAway();
				}
			}
			else if(l_trigger_release) {
				currentState = AVATAR_EDITOR_MODIFY_SHAPE_SIZE;
				dynamicInstructions_global->changeText("The min and max positions of the Hydra\nControllers represent the extents of the object");
				for(unsigned int i = 0; i < colorChangers.size(); ++i) {
					colorChangers[i]->goAway();
				}
				break;
			}
			for(unsigned int i = 0; i < colorChangers.size(); ++i) {
					if(highlighted == colorChangers[i]) {
						currClickable->setColor(((CubeColorChanger *)colorChangers[i])->handleHighlighted(rightControl->joystick_y ,currClickable->getColor()));
						break;
					}
				}
			
			break;
		case AVATAR_EDITOR_MODIFY_SHAPE_ROTATE:
			if(r_trigger_release) {
				currentState = AVATAR_EDITOR_MODIFY_SHAPE_PLACE;
				dynamicInstructions_global->changeText("Position and place with the right controller!\nRotate the avatar with the left controller.");
				for(unsigned int i = 0; i < avatar.bodyParts.size(); ++i) {
					if(bodyPartBeingModified != 0) {
						if(avatar.bodyParts[i] == bodyPartBeingModified)
							continue;
					}
					
					avatar.bodyParts[i]->comeBack();
				}
			}
			else if(l_trigger_release) {
				currentState = AVATAR_EDITOR_MODIFY_SHAPE_COLOR;
				dynamicInstructions_global->changeText("Modify the object color by selecting a capsule\nAnd pressing up or down on the right controller");
				for(unsigned int i = 0; i < colorChangers.size(); ++i) {
					colorChangers[i]->comeBack();
				}
				break;
			}

			rotateCurrClickable();

			break;
		case AVATAR_EDITOR_MODIFY_SHAPE_PLACE:
			if(r_trigger_release) {				
				if(bodyPartBeingModified != 0) {
					//if(!isLeftWing && !isRightWing)
						avatar.getRoot()->removeChild(bodyPartBeingModified->getPat());
					//avatar.bodyParts.pop_back();
					vector<Clickable*>::iterator it = avatar.bodyParts.begin();
					vector<Clickable*>::iterator ed = avatar.bodyParts.end();
					while(it != ed) {
						if(*it == bodyPartBeingModified) {
							avatar.bodyParts.erase(it);
							break;
						}
						it++;
					}

					if(isLeftWing) {
						//avatar.t_l_wing_root->removeChild(avatar.c_l_wing->getPat());

						avatar.c_l_wing = bodyPartBeingModified;
						avatar.t_l_wing_root->addChild(bodyPartBeingModified->getPat());
						isLeftWing = false;
					}
					else if(isRightWing) {
						//avatar.t_r_wing_root->removeChild(avatar.c_r_wing->getPat());

						avatar.c_r_wing = bodyPartBeingModified;
						avatar.t_r_wing_root->addChild(bodyPartBeingModified->getPat());
						isRightWing = false;
					}

					bodyPartBeingModified = 0;
				}
								
				currentState = AVATAR_EDITOR_MAIN;

				avatar.convertCursorToAvatarLocal(tp_cursor, currClickable);
				avatar.getRoot()->addChild(currClickable->getPat());
				avatar.bodyParts.push_back(currClickable);
				
				for(unsigned int i = 0; i < avatar.bodyParts.size(); ++i) {
					avatar.bodyParts[i]->enable();
				}
				chooseShapeButton->enable();
				chooseShapeButton->comeBack();
				returnToMainButton->enable();
				returnToMainButton->comeBack();
				startGameButton->enable();
				startGameButton->comeBack();
				rotateButton->enable();
				rotateButton->comeBack();
				dynamicInstructions_global->changeText("Select body part to edit\nOr make new body part");	

				avatarEditorWorld->removeChild(currClickable->getPat());
				avatarEditorVec.pop_back();
				break;
			}
			else if(l_trigger_release) {
				currentState = AVATAR_EDITOR_MODIFY_SHAPE_ROTATE;
				dynamicInstructions_global->changeText("Rotate the right controller into the\nDesired orientation for this object");
				for(unsigned int i = 0; i < avatar.bodyParts.size(); ++i) {
					if(bodyPartBeingModified != 0) {
						if(avatar.bodyParts[i] == bodyPartBeingModified)
							continue;
					}
					
					avatar.bodyParts[i]->goAway();
				}
				break;
			}

			avatar.getRoot()->setAttitude(Quat(leftControl->rot_quat[0],leftControl->rot_quat[1],leftControl->rot_quat[2],leftControl->rot_quat[3]));				
			currClickable->translate(tp_cursor->getPosition());

			break;
		case AVATAR_EDITOR_CHANGE_SHAPE_PROLOGUE:
			bodyPartBeingModified = highlighted;
			if(bodyPartBeingModified == avatar.c_l_wing) {
				isLeftWing = true;
			}
			else if(bodyPartBeingModified == avatar.c_r_wing) {
				isRightWing = true;
			}
			currentState = AVATAR_EDITOR_CREATE_SHAPE_PROLOGUE;
			break;
		case AVATAR_EDITOR_ROTATE_SHAPE_PROLOGUE:
			for(unsigned int i = 0; i < avatar.bodyParts.size(); ++i) {
				avatar.bodyParts[i]->disable();
			}
			chooseShapeButton->disable();
			chooseShapeButton->goAway();
			returnToMainButton->disable();
			returnToMainButton->goAway();
			startGameButton->disable();
			startGameButton->goAway();
			rotateButton->disable();
			rotateButton->goAway();
			
			currentState = AVATAR_EDITOR_ROTATE_SHAPE;
			
			dynamicInstructions_global->changeText("Use the left controller to\nRotate the avatar");
			break;
		case AVATAR_EDITOR_ROTATE_SHAPE:
			if(r_trigger_release || l_trigger_release) {
				for(unsigned int i = 0; i < avatar.bodyParts.size(); ++i) {
					avatar.bodyParts[i]->enable();
				}
				chooseShapeButton->enable();
				chooseShapeButton->comeBack();
				returnToMainButton->enable();
				returnToMainButton->comeBack();
				startGameButton->enable();
				startGameButton->comeBack();
				rotateButton->enable();
				rotateButton->comeBack();
				
				currentState = AVATAR_EDITOR_MAIN;
				break;
			}
			
			avatar.getRoot()->setAttitude(Quat(leftControl->rot_quat[0],leftControl->rot_quat[1],leftControl->rot_quat[2],leftControl->rot_quat[3]));			
			//avatar.updateBodyPartPATs();
			
			/*
			for(unsigned int i = 0; i < avatar.bodyParts.size(); ++i) {
				avatar.bodyParts[i]->rotate(Quat(leftControl->rot_quat[0],leftControl->rot_quat[1],leftControl->rot_quat[2],leftControl->rot_quat[3]));
			}*/
		
			break;
	}
}

void gameStateMachine() {
	if((leftControl->buttons & SIXENSE_BUTTON_START) == SIXENSE_BUTTON_START) {
		currentState = MAIN_MENU_S;
		//reset camera
		Vec3d cam_eye = Vec3d(0,0,20);
		Vec3d cam_center = Vec3d(0,0,0);
		Vec3d cam_up = osg::Vec3d(0.0,1.0,0.0);
		camera->setViewMatrixAsLookAt(cam_eye,cam_center,cam_up);
		setupCursor();
		switchMegaState();
		return;
	}

	//check if force should be applied
	if(history.addToHistAndCheck(leftControl->pos[1] * HYDRA_TO_WORLD,rightControl->pos[1]) * HYDRA_TO_WORLD) {
		//cout<< "Flap Detected" << endl;
		applyFlap();
	}

	//calculate turn
	turnAvatar();

	//calculate y position:
	dynamicsWorld->stepSimulation(1/60.f,10);
	Vec3d positionWithGravity = avatar.getRoot()->getPosition();
	btTransform trans;
	avatarRigidBody->getMotionState()->getWorldTransform(trans);
	positionWithGravity[1] = trans.getOrigin().getY();
	
		Vec3d newMove = avatar.getRoot()->getAttitude() * Vec3d(0,0,MOVE_SPEED);
	avatar.getRoot()->setPosition(positionWithGravity + newMove);

	cameraFollow();
	checkForGoalIntersection();
	avatar.animateWings();
	if(checkForLossConditions()) return;
	updateArrow();
	//cout << "avatar " << avatar.getRoot()->getPosition()[0] << " " << avatar.getRoot()->getPosition()[1] << " " << avatar.getRoot()->getPosition()[2] << endl;
	//cout << "t_arrow " << t_Arrow->getPosition()[0] << " " << t_Arrow->getPosition()[1] << " " << t_Arrow->getPosition()[2] << endl;
}

void gameMenuStateMachine() {
}


void checkForSelection(vector<Clickable*> currVec) {
	Matrixd worldMatrix, cursorMatrix;
	world->computeLocalToWorldMatrix(worldMatrix,NULL);
	
	tp_cursor->computeLocalToWorldMatrix(cursorMatrix,NULL);
	cursorMatrix.postMult(worldMatrix);
	
	Vec3 cursorPosition = cursorMatrix.getTrans();
	
	BoundingBox cursorBB = g_cursor->getBoundingBox();
	cursorBB.set(	cursorBB.xMin() + cursorPosition.x(),
					cursorBB.yMin() + cursorPosition.y(),
					cursorBB.zMin() + cursorPosition.z(),
					cursorBB.xMax() + cursorPosition.x(),
					cursorBB.yMax() + cursorPosition.y(),
					cursorBB.zMax() + cursorPosition.z());

	
	for(unsigned int i = 0; i < currVec.size(); ++i) {
		if( currVec[i]->isEnabled() && currVec[i]->isHighlighted(cursorBB, worldMatrix)) {
			if(currVec[i] == highlighted)
				return;

			if(highlighted != 0) {
				if(!highlighted->isColorChanger()) highlighted->setColor(highlightedColor);
				else ((CubeColorChanger *) highlighted)->loseHighlight();
			}
			highlighted = currVec[i];
			//cout<<"Highlited!" << endl;
			highlightedColor = highlighted->getColor();
			Vec4d newColor = Vec4d(highlightedColor[0]/2,highlightedColor[1]/2,highlightedColor[2]/2,highlightedColor[3]);
			if(!highlighted->isColorChanger()) highlighted->setColor(newColor);
			else ((CubeColorChanger *) highlighted)->hasHighlight();
			return;
		}
	}
		
	avatar.getRoot()->computeLocalToWorldMatrix(worldMatrix,NULL);
	if((currentState & MEGA_MASK) == MAIN_MENU_S || (currentState & MEGA_MASK) == AVATAR_EDITOR_S) {
		for(unsigned int i = 0; i < avatar.bodyParts.size(); ++i) {
			if( avatar.bodyParts[i]->isEnabled() && avatar.bodyParts[i]->isHighlighted(cursorBB, worldMatrix)) {
				if(avatar.bodyParts[i] == highlighted)
					return;

				if(highlighted != 0) {
					if(!highlighted->isColorChanger()) highlighted->setColor(highlightedColor);
					else ((CubeColorChanger *) highlighted)->loseHighlight();
				}
				highlighted = avatar.bodyParts[i];
				//cout<<"Highlited!" << endl;
				highlightedColor = highlighted->getColor();
				Vec4d newColor = Vec4d(highlightedColor[0]/2,highlightedColor[1]/2,highlightedColor[2]/2,highlightedColor[3]);
				if(!highlighted->isColorChanger()) highlighted->setColor(newColor);
				else ((CubeColorChanger *) highlighted)->hasHighlight();
				return;
			}
		}
	}
	
	if(highlighted != 0) {
		if(!highlighted->isColorChanger()) highlighted->setColor(highlightedColor);
		else ((CubeColorChanger *) highlighted)->loseHighlight();
		highlighted = 0;
	}
}

void checkForClick() {
	if(highlighted != 0 && (r_trigger_release)) {
		int oldMegaState = currentState & MEGA_MASK;
	
		currentState = highlighted->onClick(currentState);
		
		if((currentState & MEGA_MASK) != oldMegaState)
			switchMegaState();
	}
}

void switchMegaState() {
	highlighted = 0;
	avatar.getRoot()->setPosition(Vec3d(0,0,0));
	avatar.getRoot()->setAttitude(Quat(0,0,0,1));

	//camera setup
	Vec3d cam_eye = Vec3d(0,0,20);
	Vec3d cam_center = Vec3d(0,0,0);
	Vec3d cam_up = osg::Vec3d(0.0,1.0,0.0);
	camera->setViewMatrixAsLookAt(cam_eye,cam_center,cam_up);

	if((currentState & MEGA_MASK) == MAIN_MENU_S) {
		setupMainMenu();
	}
	else if((currentState & MEGA_MASK) == AVATAR_EDITOR_S) {
		
		setupAvatarEditor();
	}
	else if((currentState & MEGA_MASK) == GAME_S) {
		//setupGame();
	//cout << "here" << endl;
		setupNewPipeWorld();
	}
	else if((currentState & MEGA_MASK) == GAME_MENU_S) {
		setupGameMenu();
	}
}

void modifyCurrClickableSize() {
	if(currClickable == 0)
		return;

	Vec3 rPos = Vec3d(HYDRA_TO_SCALE*rightControl->pos[0],HYDRA_TO_SCALE*rightControl->pos[1],HYDRA_TO_SCALE*rightControl->pos[2]);
	Vec3 lPos = Vec3d(HYDRA_TO_SCALE*leftControl->pos[0],HYDRA_TO_SCALE*leftControl->pos[1],HYDRA_TO_SCALE*leftControl->pos[2]);
	currClickable->changeDimensions(lPos,rPos);
}

void rotateCurrClickable() {
	currClickable->rotate(Quat(rightControl->rot_quat[0],rightControl->rot_quat[1],rightControl->rot_quat[2],rightControl->rot_quat[3]));
}

void setupNewPipeWorld() {

	//init controller y position history:
	history = HistoryTracker();

	gameWorld = new PositionAttitudeTransform();
	gameWorld->addChild(avatar.getRoot());
	
	avatar.computeBoundingBox();
	cameraDistance = (avatar.bb.zMax() - avatar.bb.zMin())/2 * CAMERA_DISTANCE_RATIO ;

	//camera setup
	Vec3d cam_eye = Vec3d(0,0,-cameraDistance);
	Vec3d cam_center = Vec3d(0,0,0);
	Vec3d cam_up = osg::Vec3d(0.0,1.0,0.0);
	camera->setViewMatrixAsLookAt(cam_eye,cam_center,cam_up);

	//save avatar wings
	avatar.saveWings();

	pipes = vector<Pipe*>();
	pipes.clear();
	double increment = MAX_WORLD_SIZE/DIVISIONS;
	double half_length = MAX_WORLD_SIZE/2;
	srand(std::time(0)); //current time as seed for random generator
	
	for(double x = 1.0; x < DIVISIONS - 1.0; ++x) {
		for(double z = 1.0; z < DIVISIONS -1.0; ++z) {
			//random number between -MAX_Y_OFFSET and MAX_Y_OFFSET
			int y = rand();
			y = y % (((MAX_Y_OFFSET * 2) + 1) - MAX_Y_OFFSET);
			Pipe * currPipe = new Pipe(Vec3d(x*increment - half_length, y, z*increment - half_length));
			//cout<< "Pipe x: " << x*increment - half_length <<" y: " << y << " z: " << z*increment - half_length << endl;
			pipes.push_back(currPipe);
			gameWorld->addChild(currPipe->getRoot());
		}
	}

	initGoal();
	initArrow();

	world->getParent(0)->addChild(gameWorld);
	world->getParent(0)->removeChild(world);
	world = gameWorld;

	//bulletInit
	// Build the broadphase
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();

    // Set up the collision configuration and dispatcher
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

    // The actual physics solver
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

    // The world.
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0,-15,0));
	avatarPhysics = new btBoxShape(btVector3(.5,.5,.5));

	btDefaultMotionState* fallMotionState =
            new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
    btScalar mass = 1;
    btVector3 fallInertia(0,0,0);
    avatarPhysics->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,avatarPhysics,fallInertia);
    avatarRigidBody = new btRigidBody(fallRigidBodyCI);
    dynamicsWorld->addRigidBody(avatarRigidBody);

}

//called in setup Pipe world
void initGoal() {
	//init score and next point's score
	scoreForNextPoint = 0;
	totalScore = 0;
	cout<< "Score: " << totalScore << endl;
	g_goal = new Geode();
	ShapeDrawable * sphere = new ShapeDrawable(new Sphere(Vec3d(0.0,0.0,0.0),GOAL_RADIUS));
	sphere->setColor(GOAL_COLOR);
	g_goal->addDrawable(sphere);
	
	t_goal = new PositionAttitudeTransform();
	t_goal->addChild(g_goal);
	gameWorld->addChild(t_goal);

	setNewGoalPosition();
}

//called by checkForGoalIntersection
void setNewGoalPosition() {
	int nextGoal = rand() % pipes.size();
	t_goal->setPosition(pipes[nextGoal]->getPosition());
	totalScore = totalScore + scoreForNextPoint;
	scoreForNextPoint = (int) (avatar.getRoot()->getPosition() - t_goal->getPosition()).length();
}

//called every frame
void checkForGoalIntersection() {
	//cout<< "Score: " << totalScore << endl;
	//cout<< "Next Goal Worth: " << scoreForNextPoint << endl;
	double distanceToGoal = (avatar.getRoot()->getPosition() - t_goal->getPosition()).length();
	if(distanceToGoal < VALID_PROXIMITY) {
		setNewGoalPosition();
		cout<< "Score: " << totalScore << endl;
		cout<< "Next Goal Worth: " << scoreForNextPoint << endl;
	}
}

bool checkForLossConditions() {
	Vec3d currentPos = avatar.getRoot()->getPosition();
	
	avatar.computeBoundingBox();
	//cout << "min: " << avatar.bb.xMin() << " Max: " << avatar.bb.xMax() << endl;
	for(unsigned int i = 0; i < pipes.size(); ++i) {
		if(pipes[i]->isColliding(avatar.bb)) {
			currentState = MAIN_MENU_S;
			//cout <<"colliding " <<endl;
			setupCursor();
			switchMegaState();
			avatar.resetWings();
			return true;
		}
	}
	return false;
}

void cameraFollow() {

	Quat avatarRotation = avatar.getRoot()->getAttitude();

	Vec3d cam_eye = avatar.getRoot()->getPosition() + (avatarRotation * Vec3d(0,0,-cameraDistance));
	Vec3d cam_center = avatar.getRoot()->getPosition();
	Vec3d cam_up = osg::Vec3d(0.0,1.0,0.0);
	camera->setViewMatrixAsLookAt(cam_eye,cam_center,cam_up);
}

void turnAvatar() {
	float turnBy = (leftControl->trigger - rightControl->trigger) * TURN_RATIO;
	Quat turn = Quat(turnBy,Vec3(0,1,0));
	avatar.getRoot()->setAttitude(avatar.getRoot()->getAttitude() * turn);
}

void applyFlap() {
	avatarRigidBody->applyCentralForce(btVector3(0,2000,0));
}

void initArrow() {
	Geode * g_Arrow = new Geode();
	Cone *cone = new Cone();
	ShapeDrawable * sd_cone = new ShapeDrawable(cone);
	sd_cone->setColor(Vec4d(1.0,1.0,0.0,1.0));
	g_Arrow->addDrawable(sd_cone);
	t_Arrow = new PositionAttitudeTransform();
	
	t_Arrow_rot = new PositionAttitudeTransform();
	t_Arrow->addChild(t_Arrow_rot);
	t_Arrow_rot->setAttitude(Quat(0,1,0,0));
	t_Arrow_rot->addChild(g_Arrow);
	
	t_Arrow->setPosition(Vec3d(avatar.getRoot()->getPosition()[0], avatar.getRoot()->getPosition()[1] + avatar.bb.yMax() + 3, avatar.getRoot()->getPosition()[2]));
	gameWorld->addChild(t_Arrow);
}

void updateArrow() {
	Matrixd mat;
	mat.makeLookAt(avatar.getRoot()->getPosition(), t_goal->getPosition(), Vec3d(0,1,0));
	mat.invert_4x4(mat);
		/*
	Quat quat;
	quat.makeRotate(avatar.getRoot()->getPosition(), t_goal->getPosition());
	t_Arrow->setAttitude(quat);
	*/
	t_Arrow->setAttitude(mat.getRotate());
	t_Arrow->setPosition(Vec3d(avatar.getRoot()->getPosition()[0], avatar.bb.yMax() + 3, avatar.getRoot()->getPosition()[2]));
}