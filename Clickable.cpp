#include "Clickable.h"


Clickable::Clickable(void)
{
	box = 0;
	sphere = 0;
	cone = 0;
	cylinder = 0;
}


Clickable::~Clickable(void)
{
}


void Clickable::setColor(Vec4d newColor) {
	if(sd_shape != 0) {
		sd_shape->setColor(newColor);
	}
}
Vec4d Clickable::getColor() {
	if(sd_shape != 0) {
		return sd_shape->getColor();
	}
	else return Vec4d(1,1,1,1);
}

bool Clickable::isHighlighted(BoundingBox cursorBB, Matrixd worldMatrix) {
	BoundingBox myBB;
	
	/*
	, cursorMatrix
	world->computeLocalToWorldMatrix(worldMatrix,NULL);
	
	tp_cursor->computeLocalToWorldMatrix(cursorMatrix,NULL);
	cursorMatrix.postMult(worldMatrix);
	
	Vec3 cursorPosition = cursorMatrix.getTrans();
	
	cursorBB = g_cursor->getBoundingBox();
	cursorBB.set(	cursorBB.xMin() + cursorPosition.x(),
					cursorBB.yMin() + cursorPosition.y(),
					cursorBB.zMin() + cursorPosition.z(),
					cursorBB.xMax() + cursorPosition.x(),
					cursorBB.yMax() + cursorPosition.y(),
					cursorBB.zMax() + cursorPosition.z());
*/
	Matrixd shapeMatrix;
	Vec3 shapePosition;
	
	myBB = g_shape->getBoundingBox();
		
	tp_position->computeLocalToWorldMatrix(shapeMatrix,NULL);
	shapeMatrix.postMult(worldMatrix);
		
	shapePosition = shapeMatrix.getTrans();

	myBB.set(	myBB.xMin() + shapePosition.x(),
				myBB.yMin() + shapePosition.y(),
				myBB.zMin() + shapePosition.z(),
				myBB.xMax() + shapePosition.x(),
				myBB.yMax() + shapePosition.y(),
				myBB.zMax() + shapePosition.z());

	return cursorBB.intersects(myBB);
}

void Clickable::changeDimensions(Vec3 lpos, Vec3 rpos) {
	float xmin = min(lpos.x(),rpos.x());
	float xmax = max(lpos.x(),rpos.x());
	float ymin = min(lpos.y(),rpos.y());
	float ymax = max(lpos.y(),rpos.y());
	float zmin = min(lpos.z(),rpos.z());
	float zmax = max(lpos.z(),rpos.z());


	if(cone != 0 || cylinder != 0) tp_position->setScale(Vec3((xmax-xmin),(zmax-zmin),(ymax-ymin)));
	else tp_position->setScale(Vec3((xmax-xmin),(ymax-ymin),(zmax-zmin)));
	/*
	if(cone != 0) {	
		float maxDimension = max(xmax-xmin,ymax-ymin);
		maxDimension = max(maxDimension,zmax-zmin);
		cone->setRadius(maxDimension/2.0);
	}
	else if(box != 0) {
		box->setHalfLengths(Vec3((xmax-xmin)/2.0,(ymax-ymin)/2.0,(zmax-zmin)/2.0));
		box->setHalfLengths(Vec3(x,y,z));
	}
	else if(cylinder != 0) {
		cylinder->setHeight(ymax-ymin);
		float maxDimension = max(xmax-xmin,zmax-zmin);
		cylinder->setRadius(maxDimension);
	}
	else if(sphere != 0) {
		float maxDimension = max(xmax-xmin,ymax-ymin);
		maxDimension = max(maxDimension,zmax-zmin);
		sphere->setRadius(maxDimension/2.0);
	}*/
}

void Clickable::rotate(Quat quat) {
	tp_position->setAttitude(quat);
}

void Clickable::translate(Vec3 pos) {
	tp_position->setPosition(pos);
}

void Clickable::changeToCube() {
	tp_position = new PositionAttitudeTransform();
	
	g_shape = new Geode();
	tp_position->addChild(g_shape);
	
	box = new Box(Vec3d(0,0,0),1,1,1);
	sd_shape = new ShapeDrawable(box);
	g_shape->addDrawable(sd_shape);
	
	sphere = 0;
	cone = 0;
	cylinder = 0;
}

void Clickable::changeToCylinder() {
	tp_position = new PositionAttitudeTransform();
	
	g_shape = new Geode();
	tp_position->addChild(g_shape);

	cylinder = new Cylinder(Vec3d(0,0,0),.5,1);
	sd_shape = new ShapeDrawable(cylinder);
	g_shape->addDrawable(sd_shape);
	rotate(Quat(sqrt(.5),0.0,0.0,sqrt(.5)));
	
	box = 0;
	sphere = 0;
	cone = 0;
}

void Clickable::changeToCone() {
	tp_position = new PositionAttitudeTransform();
	
	g_shape = new Geode();
	tp_position->addChild(g_shape);

	cone = new Cone(Vec3d(0,0,0),.5,1);
	sd_shape = new ShapeDrawable(cone);
	g_shape->addDrawable(sd_shape);
	rotate(Quat(-sqrt(.5),0.0,0.0,sqrt(.5)));
	
	box = 0;
	sphere = 0;
	cylinder = 0;
}

void Clickable::changeToSphere() {
	tp_position = new PositionAttitudeTransform();
	
	g_shape = new Geode();
	tp_position->addChild(g_shape);
	
	sphere = new Sphere(Vec3d(0,0,0),.5);
	sd_shape = new ShapeDrawable(sphere);
	g_shape->addDrawable(sd_shape);
	
	box = 0;
	cone = 0;
	cylinder = 0;
}

void Clickable::setSphereRadius(float radius) {
	sphere->setRadius(radius);
}

void Clickable::setBoxHalfLengths(float x, float y, float z) {
	box->setHalfLengths(Vec3(x,y,z));
}