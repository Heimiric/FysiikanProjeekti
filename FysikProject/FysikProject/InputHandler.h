#pragma once
#include <Windows.h>
#include <iostream>
#include <osgGA/GUIEventHandler>
#include "../FysikProject/WorldObject.h"

class InputHandler : public osgGA::GUIEventHandler
{

public:
	InputHandler(){};
	~InputHandler(){};

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&);
	virtual void accept(osgGA::GUIEventHandlerVisitor& v)   { v.visit(*this); };
	void spawn(btDiscreteDynamicsWorld *world, osg::Group *root, osg::Vec3 camPosition, osg::Vec3 camRotation);
	void updateObjects();
	bool _spawnObject = false;
private:
	std::vector<WorldObject*>_objects;
	void calcViewTransformation(osg::Vec3 camPosition, osg::Vec3 camRotation);
	btTransform _viewTransform;
};
	
