#pragma once

#include <Common/Base/hkBase.h>
#include <Physics2012\Dynamics\Entity\hkpRigidBody.h>
#include <osg/PositionAttitudeTransform>
#include <iostream>


/*
	Transformation bridge between havok & osg
*/
class ShapeTransform : public osg::PositionAttitudeTransform
{
public:
	ShapeTransform(osg::ref_ptr<osg::Node> node, hkpRigidBody* rigidBody);
	~ShapeTransform(void);

	void updateTransform();

	osg::ref_ptr<osg::PositionAttitudeTransform> getVisualTransform() {return this;}

	const hkVector4& getRigidBodyPos() {return _position;}
	const hkQuaternion getRigidBodyRot() {return _rotation;}

	const osg::Vec3d& getVisualPos() {return this->getPosition();}
	const osg::Quat& getVisualRot() {return this->getAttitude();}

private:
	hkpRigidBody* _rigidBody;

	hkVector4 _position;
	hkQuaternion _rotation;
};

