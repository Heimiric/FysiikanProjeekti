#include "WorldObject.h"


WorldObject::WorldObject(osg::Shape* shape, float size, btTransform *startState, btDiscreteDynamicsWorld* world, osg::Group *root)
{
	_size = size;
	initializeDraw(shape, root);
	initializePhysics(startState, world);
}

void WorldObject::initializeDraw(osg::Shape* shape, osg::Group *root)
{
	_drawable = new osg::ShapeDrawable(shape);
	_shape = shape;
	_drawable->setColor(osg::Vec4(1,1,1,1));
	_geode = new osg::Geode;
	_geode->addDrawable(_drawable);

	_drawableTransform = new osg::PositionAttitudeTransform;
	_drawableTransform->addChild(_geode);
	root->addChild(_drawableTransform);

}

void WorldObject::initializePhysics(btTransform* startState, btDynamicsWorld *world)
{

	std::string shapeString = _shape->className();

	if (shapeString == "Sphere")
	{
		_collisionShape = new btSphereShape(_size);
	}

	if (shapeString == "Box")
	{
		_collisionShape = new btBoxShape(btVector3(_size*0.5f, _size*0.5f, _size*0.5f));
	}

	_mass = _size;
	_inertia = btVector3(0.1, 0.1, 0.1);
	_collisionShape->calculateLocalInertia(_mass,_inertia);


	_startState = new btDefaultMotionState(*startState);
	_constructionInfo = new btRigidBody::btRigidBodyConstructionInfo(_mass, _startState, _collisionShape, _inertia);
	_rigidBody = new btRigidBody(*_constructionInfo);
	world->addRigidBody(_rigidBody);

	_rigidBody->setRestitution(0.5);
	_currentTransform = &_rigidBody->getWorldTransform();
	btVector3 ble(0, 0, 0);
	

	_rigidBody->applyImpulse(btVector3(-_currentTransform->getOrigin()), ble);
}

void WorldObject::updateRender()
{
	_currentTransform = &_rigidBody->getWorldTransform();

	_drawableTransform->setPosition(osg::Vec3(_currentTransform->getOrigin().getZ(),
									_currentTransform->getOrigin().getX(),
									_currentTransform->getOrigin().getY()));


	_drawableTransform->setAttitude(osg::Quat(_currentTransform->getRotation().z(),
												_currentTransform->getRotation().y(),
												_currentTransform->getRotation().x(), _currentTransform->getRotation().w()));
}

WorldObject::~WorldObject()
{
}
