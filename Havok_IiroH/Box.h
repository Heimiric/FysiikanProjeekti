#pragma once

#include "Shape.h"
#include <Physics2012\Collide\Shape\Convex\Box\hkpBoxShape.h>
#include <Physics2012\Utilities\Dynamics\Inertia\hkpInertiaTensorComputer.h>

class Box : public Shape
{
public:

	Box(hkpRigidBodyCinfo& cInfo, const osg::Vec3& halfExtents = osg::Vec3(1.0f, 1.0f, 1.0f) );
	Box(hkpRigidBodyCinfo& cInfo, const osg::Vec3& dsHalfExtents, const hkVector4& rbHalfExtents);
	~Box(){}

private:
	osg::ref_ptr<osg::Box> _displayBox;

	osg::Vec3 _dsHalfExtents;
	hkVector4 _rbHalfExtents;

	void initShape(hkpRigidBodyCinfo& cInfo);

};

