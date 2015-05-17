#pragma once
#include <Windows.h>
#include <iostream>
#include <osgGA/GUIEventHandler>
#include "../FysikProject/WorldObject.h"
#include <random>

class InputHandler : public osgGA::GUIEventHandler
{

public:
	InputHandler(){};
	~InputHandler(){};

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&);
	virtual void accept(osgGA::GUIEventHandlerVisitor& v)   { v.visit(*this); };
	void spawn(btDiscreteDynamicsWorld *world, osg::Group *root, osg::Matrixd camMatrix);
	void updateObjects();
	bool _spawnObject = false;
private:
	std::vector<WorldObject*>_objects;
	void calcViewTransformation(osg::Matrixd camMatrix);
	btTransform _viewTransform;
};
	
