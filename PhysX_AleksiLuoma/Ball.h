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
#include <osg/ShapeDrawable>

#include <PxPhysicsAPI.h>

#pragma comment(lib, "PhysX3DEBUG_x86.lib")
#pragma comment(lib, "PhysX3CommonDEBUG_x86.lib")
#pragma comment(lib, "PhysX3ExtensionsDEBUG.lib")

class Ball
{
public:
	Ball(osg::Group* root, physx::PxPhysics* physics, physx::PxScene* scene);
	~Ball();

	void update();
	void setPosition(float x, float y, float z);
	void setRadius(float r);
	void setColor(float r, float g, float b, float a = 1.0f);
	void setRestitution(float r);
	void setDensity(float d);
	void setMass(float m);
	void setForce(physx::PxVec3 f);
	void setTorque(physx::PxVec3 t);

private:
	//PhysX
	void physXInit(physx::PxPhysics* physics, physx::PxScene* scene);

	physx::PxMaterial* _pxMaterial;
	physx::PxReal _pxDensity;
	physx::PxTransform _pxTransform;
	physx::PxSphereGeometry _pxGeometry;
	physx::PxRigidDynamic* _pxActor;

	//OSG
	void osgInit(osg::Group* root);

	osg::Sphere* _osgSphere;
	osg::ShapeDrawable* _osgShape;
	osg::PositionAttitudeTransform* _osgTransform;
	osg::Geode* _osgGeode;
};