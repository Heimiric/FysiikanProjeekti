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

#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

class WorldObject
{
	friend class InputHandler;

public:
	WorldObject(osg::Shape* shape, float size, btTransform* startState, btDiscreteDynamicsWorld *world, osg::Group *root);
	void updateRender();
	virtual ~WorldObject();
protected:

	void initializeDraw(osg::Shape* shape, osg::Group *root);
	void initializePhysics(btTransform *startState, btDynamicsWorld *world);
	osg::ShapeDrawable *_drawable;
	osg::Shape *_shape;
	osg::Geode *_geode;
	btCollisionShape *_collisionShape;
	btScalar _mass;
	btVector3 _inertia;
	btDefaultMotionState *_startState;
	btTransform *_currentTransform;
	btRigidBody::btRigidBodyConstructionInfo *_constructionInfo;
	btRigidBody *_rigidBody;
	osg::PositionAttitudeTransform *_drawableTransform;
	float _size;

};

