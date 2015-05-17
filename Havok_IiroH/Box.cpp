#include "Box.h"


Box::Box(hkpRigidBodyCinfo& cInfo, const osg::Vec3& halfExtents)
{
	_dsHalfExtents = halfExtents;
	_rbHalfExtents = hkVector4(halfExtents.x(), halfExtents.y(), halfExtents.z());
	initShape(cInfo);
}

Box::Box(hkpRigidBodyCinfo& cInfo, const osg::Vec3& dsHalfExtents, const hkVector4& rbHalfExtents)
{
	_dsHalfExtents = dsHalfExtents;
	_rbHalfExtents = rbHalfExtents;

	initShape(cInfo);
}

void Box::initShape(hkpRigidBodyCinfo& cInfo)
{
	_displayBox = new osg::Box();
	_displayBox->setHalfLengths(_dsHalfExtents);

	hkpBoxShape* boxShape = new hkpBoxShape(_rbHalfExtents, 0);
	cInfo.m_shape = boxShape;

	//massat kaatuu
	//hkMassProperties massProps;
	//hkpInertiaTensorComputer::computeBoxVolumeMassProperties(_rbHalfExtents, cInfo.m_mass, massProps);

	//cInfo.setMassProperties(massProps);

	Shape::init(_displayBox, cInfo);
}