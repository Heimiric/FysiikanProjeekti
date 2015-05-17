#include "shapeTransform.h"

ShapeTransform::ShapeTransform(osg::ref_ptr<osg::Node> node, hkpRigidBody* rigidBody)
{
	_rigidBody = rigidBody;

	hkpRigidBodyCinfo cInfo;
	_rigidBody->getCinfo(cInfo);

	//set rigid body position and rotation
	_position = cInfo.m_position;
	_rotation = cInfo.m_rotation;

	//set display shape pos & rot
	this->setPosition(osg::Vec3f(_position(0), _position(1), _position(2)));
	this->setAttitude(osg::Quat(_rotation(0), _rotation(1), _rotation(2), _rotation(3)));

	this->addChild(node);
}


ShapeTransform::~ShapeTransform(void)
{
	delete _rigidBody;
	_rigidBody = 0;
}

void ShapeTransform::updateTransform()
{
	_position = _rigidBody->getPosition();
	_rotation = _rigidBody->getRotation();


	this->setPosition(osg::Vec3f(_position(0), _position(1), _position(2)));
	this->setAttitude(osg::Quat(_rotation(0), _rotation(1), _rotation(2), _rotation(3)));

}
