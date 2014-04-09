#include "Avatar.h"

using namespace osg;


Avatar::Avatar() {
	bodyParts.clear();
}

void Avatar::createDefaultAvatar() {
	avatar_root = new PositionAttitudeTransform();

	//body
	//c_body = new AvatarBodyPart(Vec3(0,0,0),Vec4(.85,.1,.1,1.0));
	c_body = new AvatarBodyPart();
	c_body->changeToSphere();
	c_body->setSphereRadius(1.5);
	c_body->setColor(Vec4(.85,.1,.1,1.0));
	c_body->enable();
	avatar_root->addChild(c_body->getPat());
	bodyParts.push_back(c_body);

	//left eye
	//Clickable * l_eye = new AvatarBodyPart(Vec3(0.5,0.6,1.3),Vec4(0.2,0.2,0.2,1.0));
	Clickable * l_eye = new AvatarBodyPart();
	l_eye->changeToSphere();
	l_eye->setSphereRadius(0.2);
	l_eye->translate(Vec3(0.5,0.6,1.3));
	l_eye->setColor(Vec4(0.2,0.2,0.2,1.0));
	l_eye->enable();
	avatar_root->addChild(l_eye->getPat());
	bodyParts.push_back(l_eye);

	//right eye
	//Clickable * r_eye = new AvatarBodyPart(Vec3(-0.5,0.6,1.3),Vec4(0.2,0.2,0.2,1.0));
	Clickable * r_eye = new AvatarBodyPart();
	r_eye->changeToSphere();
	r_eye->setSphereRadius(0.2);
	r_eye->translate(Vec3(-0.5,0.6,1.3));
	r_eye->setColor(Vec4(0.2,0.2,0.2,1.0));
	r_eye->enable();
	avatar_root->addChild(r_eye->getPat());
	bodyParts.push_back(r_eye);

	//left wing
	//c_l_wing = new AvatarBodyPart(Vec3(-3.0,0.0,0.0),Vec4(.6,.2,.2,.1));
	c_l_wing = new AvatarBodyPart();
	c_l_wing->changeToCube();
	c_l_wing->setBoxHalfLengths(1.0,0.25,0.5);
	c_l_wing->translate(Vec3(-3.0,0.0,0.0));
	c_l_wing->setColor(Vec4(.6,.2,.2,.1));
	c_l_wing->enable();
	//avatar_root->addChild(c_l_wing->getPat());
	bodyParts.push_back(c_l_wing);
	t_l_wing_root = new PositionAttitudeTransform();
	t_l_wing_root->addChild(c_l_wing->getPat());
	avatar_root->addChild(t_l_wing_root);


	//right wing
	//c_r_wing = new AvatarBodyPart(Vec3(3.0,0.0,0.0),Vec4(.6,.2,.2,.1));
	c_r_wing = new AvatarBodyPart();
	c_r_wing->changeToCube();
	c_r_wing->setBoxHalfLengths(1.0,0.25,0.5);
	c_r_wing->translate(Vec3(3.0,0.0,0.0));
	c_r_wing->setColor(Vec4(.6,.2,.2,.1));
	c_r_wing->enable();
	//avatar_root->addChild(c_r_wing->getPat());
	bodyParts.push_back(c_r_wing);
	t_r_wing_root = new PositionAttitudeTransform();
	t_r_wing_root->addChild(c_r_wing->getPat());
	avatar_root->addChild(t_r_wing_root);
}
/*
PositionAttitudeTransform* Avatar::getAvatar_Body() {
	return t_body;
}

PositionAttitudeTransform* Avatar::getAvatar_RightWing() {
	return t_r_wing;
}

PositionAttitudeTransform* Avatar::getAvatar_LeftWing() {
	return t_l_wing;
}
*/

void Avatar::updateBodyPartPATs() {
	Matrixd rootMatrix, myMatrix;
	avatar_root->computeLocalToWorldMatrix(rootMatrix,NULL);	

	for(unsigned int i = 0; i < bodyParts.size(); ++i) {
		bodyParts[i]->getPat()->computeLocalToWorldMatrix(myMatrix,NULL);
		myMatrix.postMult(rootMatrix);
		bodyParts[i]->getPat()->setAttitude(myMatrix.getRotate());
		bodyParts[i]->getPat()->setPosition(myMatrix.getTrans());
	}
}

void Avatar::convertCursorToAvatarLocal(PositionAttitudeTransform * position, Clickable* currBodyPart) {
	/*
	Matrixd rootMatrix, bodyPartsMatrix;
	avatar_root->computeLocalToWorldMatrix(rootMatrix, NULL);
	currBodyPart->getPat()->computeLocalToWorldMatrix(bodyPartsMatrix, NULL);

	//rootMatrix.postMult(bodyPartsMatrix);
	bodyPartsMatrix.invert_4x4(bodyPartsMatrix);
	//rootMatrix.invert_4x4(rootMatrix);
	Vec3d newPosition = bodyPartsMatrix.postMult(currBodyPart->getPat()->getPosition());
	currBodyPart->getPat()->setPosition(bodyPartsMatrix.getTrans());
	*/
	
	Vec3d newPos = currBodyPart->getPat()->getPosition();
	Matrixd worldMatrix, cursorMatrix; 
	avatar_root->computeLocalToWorldMatrix(worldMatrix,NULL); 
	currBodyPart->getPat()->computeLocalToWorldMatrix(cursorMatrix,NULL);
	//worldMatrix.invert_4x4(worldMatrix);
	newPos = worldMatrix.postMult(newPos);
	currBodyPart->getPat()->setPosition(newPos);
	worldMatrix.invert_4x4(worldMatrix);

	cursorMatrix.makeIdentity();
	cursorMatrix.setRotate(currBodyPart->getPat()->getAttitude());
	worldMatrix.preMult(cursorMatrix);
	currBodyPart->getPat()->setAttitude(worldMatrix.getRotate());
}


void Avatar::computeBoundingBox() {
	bb.set(0,0,0,0,0,0);
	
	for(unsigned int i = 0; i < bodyParts.size(); ++i) {
		BoundingBox bodyPartBox = bodyParts[i]->getGeode()->getBoundingBox();
		Vec3 offset = bodyParts[i]->getPat()->getPosition();
		bodyPartBox.set(	bodyPartBox.xMin() + offset.x(),
							bodyPartBox.yMin() + offset.y(),
							bodyPartBox.zMin() + offset.z(),
							bodyPartBox.xMax() + offset.x(),
							bodyPartBox.yMax() + offset.y(),
							bodyPartBox.zMax() + offset.z());
							
		bb.expandBy(bodyPartBox);
	}
	bb.set(	bb.xMin() + avatar_root->getPosition()[0],
			bb.yMin() + avatar_root->getPosition()[1],
			bb.zMin() + avatar_root->getPosition()[2],
			bb.xMax() + avatar_root->getPosition()[0],
			bb.yMax() + avatar_root->getPosition()[1],
			bb.zMax() + avatar_root->getPosition()[2]);
}

void Avatar::saveWings() {
	lOriginal = c_l_wing->getPat()->getAttitude();//t_l_wing_root->getAttitude();
	rOriginal = c_r_wing->getPat()->getAttitude();//t_r_wing_root->getAttitude();

	//t_l_wing_root->addChild(c_l_wing->getPat());
	//t_r_wing_root->addChild(c_r_wing->getPat());

	animationPos = 0;
	up = true;
}

void Avatar::resetWings() {
	t_l_wing_root->setAttitude(Quat(0,0,0,1));
	t_r_wing_root->setAttitude(Quat(0,0,0,1));
	c_l_wing->getPat()->setAttitude(lOriginal);
	c_r_wing->getPat()->setAttitude(rOriginal);
}

void Avatar::animateWings() {
	//if at the end, switch direcitons
	if((animationPos == NUM_ANIMATIONS/2 + 1) || (animationPos == (-NUM_ANIMATIONS)/2 -1)) up = !up;

	if(up) {
		Matrixd rMat, lMat;
		lMat.makeRotate(-ANIMATION_ROTATION, Vec3(0,0,1));
		rMat.makeRotate(ANIMATION_ROTATION, Vec3(0,0,1));

		Quat lQuat, rQuat;
		lQuat.set(lMat);
		rQuat.set(rMat);

		//c_l_wing->getPat()->setAttitude(c_l_wing->getPat()->getAttitude() * lQuat);
		//c_r_wing->getPat()->setAttitude(c_r_wing->getPat()->getAttitude() * rQuat);
		t_l_wing_root->setAttitude(t_l_wing_root->getAttitude() * lQuat);
		t_r_wing_root->setAttitude(t_r_wing_root->getAttitude() * rQuat);
		animationPos++;
	}
	else {
		Matrixd rMat, lMat;
		lMat.makeRotate(ANIMATION_ROTATION, Vec3(0,0,1));
		rMat.makeRotate(-ANIMATION_ROTATION, Vec3(0,0,1));

		Quat lQuat, rQuat;
		lQuat.set(lMat);
		rQuat.set(rMat);

		//c_l_wing->getPat()->setAttitude(c_l_wing->getPat()->getAttitude() * lQuat);
		//c_r_wing->getPat()->setAttitude(c_r_wing->getPat()->getAttitude() * rQuat);
		t_l_wing_root->setAttitude(t_l_wing_root->getAttitude() * lQuat);
		t_r_wing_root->setAttitude(t_r_wing_root->getAttitude() * rQuat);
		animationPos--;
	}
}