#pragma once

#include "shapeTransform.h"
#include "RigidBodyCallBack.h"
#include <osg/ShapeDrawable>
#include <osg/Geode>


/*
	Bridges shapes between havok & osg
*/
class Shape : public osg::Geode
{
public:

	Shape();
	~Shape();
	hkpRigidBody* getRigidBody() {return _rigidBody;}
	osg::ref_ptr<osg::PositionAttitudeTransform> getVisualTransform() {return _transform->getVisualTransform();}
	const osg::Vec3d& getVisualPosition() { return _transform->getVisualPos(); }
	const hkVector4& getRigidBodyPosition() { return _transform->getRigidBodyPos(); }
	const osg::Quat& getVisualRotation() {  return _transform->getVisualRot(); }
	const hkQuaternion& getRigidBodyRotation() { return _transform->getRigidBodyRot(); }


private:
	osg::ref_ptr<osg::Shape> _displayShape;
	osg::ref_ptr<osg::ShapeDrawable> _displayShapeDrawable;

	hkpRigidBody* _rigidBody;
	osg::ref_ptr<ShapeTransform> _transform; //Transform bridge between havok and osg

protected:
	void init(osg::ref_ptr<osg::Shape> displayShape, hkpRigidBodyCinfo& cInfo);
	virtual void initShape(hkpRigidBodyCinfo& cInfo) = 0;




};

