#pragma once

#include "Shape.h"
#include <map>

class ShapeManager
{
public:
	ShapeManager(hkpWorld* physicsWorld, osg::ref_ptr<osg::Group> root);
	~ShapeManager();
	
	void registerShape(osg::ref_ptr<Shape> shape);
private:
	int _nextID;
	std::map<int, osg::ref_ptr<Shape>> _shapes;

	hkpWorld* _physicsWorld;

	osg::ref_ptr<osg::Group> _root;
};

