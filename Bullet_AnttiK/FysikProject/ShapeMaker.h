#pragma once

#include <Windows.h>
#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/PositionAttitudeTransform>
#include <osgGA/TrackballManipulator>
#include <osg/Drawable>
#include <osg/Shape>
#include <osg/Object>
#include <osg/ShapeDrawable>

class ShapeMaker
{
public:
	ShapeMaker();
	~ShapeMaker();
	osg::Geode* makeGeodeWithShape(osg::Shape* shape, osg::Vec4 color)
	{
		osg::ShapeDrawable *drawable = new osg::ShapeDrawable(shape);
		drawable->setColor(color);
		_drawables.push_back(drawable);
		osg::Geode *geode = new osg::Geode;
		geode->addDrawable(drawable);
		return geode;
	}

	std::vector<osg::ShapeDrawable*>_drawables;
	std::vector<osg::Shape*>_shapes;
};

