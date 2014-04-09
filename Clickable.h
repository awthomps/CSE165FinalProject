#pragma once

//std:
#include <Windows.h>
#include <string>
#include <iostream>
#include <math.h>

//OSG:
#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Material>
#include <osg/StateSet>
#include <osg/Projection>
#include <osg/NodeVisitor>
#include <osgText/Text>

//OURS:
#include "States.h"

using namespace osg;

class Clickable
{
public:
	//Clickable(bool enabled, Vec3d position, Vec4d newColor, Geode * shape);
	Clickable(void);
	~Clickable(void);

	bool isEnabled() { return enabled; }
	virtual void enable() {enabled = true;}
	virtual void disable() {enabled = false;}
	virtual bool isColorChanger() { return false;}
	PositionAttitudeTransform * getPat() { return tp_position;}
	Geode * getGeode() {return g_shape;}
	void setColor(Vec4d newColor);
	Vec4d getColor();
	bool isHighlighted(BoundingBox cursorBB, Matrixd worldMatrix);
	virtual int onClick(int currentState) {return currentState;}
	void changeDimensions(Vec3 lpos, Vec3 rpos);
	void rotate(Quat quat);
	void translate(Vec3 pos);
	void changeToCube();
	void changeToCylinder();
	void changeToCone();
	void changeToSphere();
	void setSphereRadius(float radius);
	void setBoxHalfLengths(float x, float y, float z);
	void goAway() {
		tp_position->setPosition(tp_position->getPosition() + Vec3(0,1000,0));
	}
	void comeBack() {
		tp_position->setPosition(tp_position->getPosition() + Vec3(0,-1000,0));
	}

protected:
	bool enabled;
	PositionAttitudeTransform * tp_position;
	Geode * g_shape;
	ShapeDrawable * sd_shape;
	Box * box;
	Cone * cone;
	Sphere * sphere;
	Cylinder * cylinder;
	Capsule * capsule;
};

