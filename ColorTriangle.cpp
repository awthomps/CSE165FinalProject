#include "ColorTriangle.h"


ColorTriangle::ColorTriangle(Vec3d position, Vec4d color)
{
	enabled = true;

	sd_shape = 0;

	tp_position = new PositionAttitudeTransform();
	tp_position->setPosition(position);

	g_shape = new Geode();
	tp_position->addChild(g_shape);

	Geometry * colorTriangleGeo = new Geometry();
	g_shape->addDrawable(colorTriangleGeo);

	//(not a perfect triangle [fix])
	Vec3Array * colorTriangleVertices = new Vec3Array;
	colorTriangleVertices->push_back(Vec3(0,0,0));
	colorTriangleVertices->push_back(Vec3(4,0,0));
	colorTriangleVertices->push_back(Vec3(2,3,0));
	colorTriangleGeo->setVertexArray(colorTriangleVertices);

	DrawElementsUInt* colorTriangleFace = new DrawElementsUInt( PrimitiveSet::TRIANGLES, 0);
	colorTriangleFace->push_back(0);
	colorTriangleFace->push_back(1);
	colorTriangleFace->push_back(2);
	colorTriangleGeo->addPrimitiveSet(colorTriangleFace);

	Vec4Array * colors = new Vec4Array;
	colors->push_back(Vec4d(0.0,1.0,0.0,1.0));
	colors->push_back(Vec4d(0.0,0.0,1.0,1.0));
	colors->push_back(Vec4d(1.0,0.0,0.0,1.0));
	colorTriangleGeo->setColorArray(colors);
	colorTriangleGeo->setColorBinding(Geometry::BIND_PER_VERTEX);


	//sd_shape

}


ColorTriangle::~ColorTriangle(void)
{
}