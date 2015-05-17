
#include "HavokCore.h"
#include "ShapeManager.h"
#include "Box.h"

#include <Windows.h>
#include <iostream>

//OpenSceneGraph includes
#include <osg/Node>
#include <osg/Group>

#include <osg/Geometry>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/PositionAttitudeTransform>
#include <osgGA/TrackballManipulator>


ShapeManager* shapeMngr;

void addFallingBox(float yPos, float friction, float restitution);
void createGroundBox();


int HK_CALL main(int argc, const char** argv)
{
	HavokCore* havok = new HavokCore(true);
	osg::ref_ptr<osg::Group> root = new osg::Group();

	shapeMngr = new ShapeManager(havok->getWorld(), root);

	root->setCullingActive(false);

	havok->getWorld()->lock();

	createGroundBox();

	std::cout << "---------------------" << std::endl;
	std::cout << "Give number of boxes: ";
	int numBoxes;
	std::cin >> numBoxes;
	float restitution, friction;
	std::cout << std::endl;
	std::cout << "Restitution: ";
	std::cin >> restitution;
	std::cout << std::endl;
	std::cout << "Friction: ";
	std::cin >> friction;

	for (int i = 0; i < numBoxes; i++)
	{
		addFallingBox((i*3 + 20), friction, restitution);
	}

	havok->getWorld()->unlock();

	//Set lighting
	root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);

	// osg display
	osgViewer::Viewer viewer;
	viewer.setUpViewOnSingleScreen();
	viewer.setSceneData(root);

	viewer.setCameraManipulator(new osgGA::TrackballManipulator());
	osg::Vec3d eyePos, lookAt, up;
	eyePos = osg::Vec3(56.0f, 58.0f, 150.0f);
	lookAt = osg::Vec3(0.0f, 0.0f, 0.0f);
	up = osg::Vec3(0.0f, 1.0f, 0.0f);

	viewer.getCameraManipulator()->setHomePosition(eyePos, lookAt, up);
	viewer.home();

	viewer.setUpViewInWindow(0, 0, 800, 600);
	viewer.realize();

	hkStopwatch stopWatch;
	stopWatch.start();
	hkReal lastTime = stopWatch.getElapsedSeconds();

	const int updateFrequency = 60.0f;
	const hkReal timeStep = 1.0f / hkReal(updateFrequency);
		
	
	while (!viewer.done())
	{
		havok->stepSimulation(timeStep);

		while (stopWatch.getElapsedSeconds() < lastTime + timeStep);
		lastTime += timeStep;

		viewer.frame();
	}

	
	delete havok;

	return 0;
}


void addFallingBox(float yPos, float friction, float restitution)
{
	osg::Vec3 halfExtents(0.5f, 1.0f, 1.5f);

	hkpRigidBodyCinfo cInfo;
	cInfo.m_position = hkVector4(0.f, yPos, 0.f);
	cInfo.m_motionType = hkpMotion::MOTION_DYNAMIC;
	cInfo.m_restitution = hkReal(restitution);
	cInfo.m_friction = (friction);

	Box* box = new Box(cInfo, halfExtents);
	shapeMngr->registerShape(box);
}

void createGroundBox()
{
	osg::Vec3 halfExtents(100.f, 2.f, 100.f);

	hkpRigidBodyCinfo cInfo;
	cInfo.m_position = hkVector4(0.f, 0.f, 0.f);
	cInfo.m_motionType = hkpMotion::MOTION_FIXED;

	Box* ground = new Box(cInfo, halfExtents);
	shapeMngr->registerShape(ground);

}