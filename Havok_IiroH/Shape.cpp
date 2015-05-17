#include "shape.h"

Shape::Shape()
{
	_displayShape = NULL;
	_displayShapeDrawable = NULL;
	_rigidBody = NULL;
	_transform = NULL;
}

Shape::~Shape()
{
	delete _rigidBody;
	_rigidBody = 0;
}

void Shape::init(osg::ref_ptr<osg::Shape> displayShape, hkpRigidBodyCinfo& cInfo)
{
	_displayShape = displayShape;
	_displayShapeDrawable = new osg::ShapeDrawable(_displayShape);
	addDrawable(_displayShapeDrawable);

	_rigidBody = new hkpRigidBody(cInfo);

	_transform = new ShapeTransform(this, _rigidBody);

	cInfo.m_shape->removeReference();

	if( cInfo.m_motionType != hkpMotion::MOTION_FIXED ||cInfo.m_motionType != hkpMotion::MOTION_INVALID )
	{
		setUpdateCallback(new RigidBodyCallBack());
	}
}