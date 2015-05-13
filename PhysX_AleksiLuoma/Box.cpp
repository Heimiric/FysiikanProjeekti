#include "Box.h"


Box::Box(osg::Group* root, physx::PxPhysics* physics, physx::PxScene* scene)
{
	physXInit(physics, scene);
	osgInit(root);
}

void Box::update()
{
	_osgTransform->setPosition(osg::Vec3(_pxActor->getGlobalPose().p.x, _pxActor->getGlobalPose().p.y, _pxActor->getGlobalPose().p.z));
	_osgTransform->setAttitude(osg::Quat(_pxActor->getGlobalPose().q.x, _pxActor->getGlobalPose().q.y, _pxActor->getGlobalPose().q.z, _pxActor->getGlobalPose().q.w));
}

void Box::setPosition(float x, float y, float z)
{
	_pxTransform = physx::PxTransform(physx::PxVec3(x, y, z));
	_pxActor->setGlobalPose(_pxTransform);
	_osgTransform->setPosition(osg::Vec3(x, y, z));
}

void Box::setDimensions(float x, float y, float z)
{
	_pxGeometry.halfExtents = physx::PxVec3(x, y, z);
	_osgBox->setHalfLengths(osg::Vec3(x, y, z));
}

void Box::setColor(float r, float g, float b, float a)
{
	_osgShape->setColor(osg::Vec4(r, g, b, a));
}

void Box::setRestitution(float r)
{
	_pxMaterial->setRestitution(r);
}

void Box::setDensity(float d)
{
	_pxDensity = d;
}

void Box::setMass(float m)
{
	_pxActor->setMass(m);
}

void Box::setForce(physx::PxVec3 f)
{
	_pxActor->clearForce();
	_pxActor->addForce(f);
}

void Box::setTorque(physx::PxVec3 t)
{
	_pxActor->clearTorque();
	_pxActor->addTorque(t);
}

void Box::physXInit(physx::PxPhysics* physics, physx::PxScene* scene)
{
	_pxMaterial = physics->createMaterial(0.5f, 0.1f, 1.0f);
	_pxDensity = 1.0f;
	_pxTransform = physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f));
	_pxGeometry = physx::PxBoxGeometry(1.f, 1.f, 1.f);
	_pxActor = PxCreateDynamic(*physics, _pxTransform, _pxGeometry, *_pxMaterial, _pxDensity);
	_pxActor->setAngularDamping(0.1f);
	_pxActor->setLinearDamping(0.01f);
	_pxActor->setMass(1.0f);
	scene->addActor(*_pxActor);
}

void Box::osgInit(osg::Group* root)
{
	_osgBox = new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f), 1.0f, 1.0f, 1.0f);
	_osgShape = new osg::ShapeDrawable(_osgBox);
	_osgShape->setColor(osg::Vec4(0.0f, 0.0f, 0.0f, 1.f));

	_osgTransform = new osg::PositionAttitudeTransform();

	_osgGeode = new osg::Geode();
	root->addChild(_osgTransform);

	_osgTransform->addChild(_osgGeode);
	_osgGeode->addDrawable(_osgShape);
}

Box::~Box()
{
}