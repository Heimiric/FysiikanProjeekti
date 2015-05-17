#include "ShapeManager.h"


ShapeManager::ShapeManager(hkpWorld* physicsWorld, osg::ref_ptr<osg::Group> root)
{
	_physicsWorld = physicsWorld;
	_root = root;
	_nextID = 0;

	_shapes = std::map<int, osg::ref_ptr<Shape>>();
}


ShapeManager::~ShapeManager()
{
	_shapes.clear();
}


void ShapeManager::registerShape(osg::ref_ptr<Shape> shape)
{
	_shapes[_nextID] = shape;
	++_nextID;

	_root->addChild(shape->getVisualTransform());

	_physicsWorld->addEntity(shape->getRigidBody())->removeReference();
}