#include <Windows.h>
#include "../FysikProject/FysikProject/ShapeMaker.h"
#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/PositionAttitudeTransform>
#include <osgGA/TrackballManipulator>
#include <osg/Drawable>
#include <osg/Shape>
#include <osg/Object>
#include <osg/ShapeDrawable>
#include <osgGA/GUIEventHandler>
#include "../FysikProject/FysikProject/InputHandler.h"
#include "../FysikProject/FysikProject/WorldObject.h"

#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"


WorldObject *obj;
int main()
{
	ShapeMaker maker;

	InputHandler *handler = new InputHandler();

	osgViewer::Viewer viewer;
	viewer.addEventHandler(handler);

	viewer.setUpViewOnSingleScreen();
	osg::Group* root = new osg::Group();


	// Build the broadphase
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();

	// Set up the collision configuration and dispatcher
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	// The actual physics solver
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	// The world.
	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -9.81, 0));


	//Creating plane
	osg::Geode *planeGeode = maker.makeGeodeWithShape(new osg::Box(osg::Vec3(0, 0, -0.1), 100.f, 100.f, 0.1f), osg::Vec4(0, 0, 1, 1));
	root->addChild(planeGeode);
	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1.f);



	btDefaultMotionState* groundMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));


	btRigidBody::btRigidBodyConstructionInfo
		groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

	dynamicsWorld->addRigidBody(groundRigidBody);

	btDefaultMotionState* fallMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));

	btScalar mass = 1;
	btVector3 fallInertia(0, 0, 0);


	osg::PositionAttitudeTransform *transform = new osg::PositionAttitudeTransform;

	root->addChild(transform);


	root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);


	btTransform trans;
	transform->setPosition(osg::Vec3(0, 0, trans.getOrigin().getY()));

	groundRigidBody->setRestitution(1.0);


	viewer.setSceneData(root);
	viewer.setCameraManipulator(new osgGA::TrackballManipulator());
	viewer.realize();

	bool doneOnce = false;

	osg::Camera *prevCamera = new osg::Camera;


	osgGA::CameraManipulator *camManipul = viewer.getCameraManipulator();
	camManipul->setAutoComputeHomePosition(false);


	osg::Vec3d eye;
	osg::Vec3d center;
	osg::Vec3d up;
	osg::Matrix camMatrix;
	bool spawned = false;

	while (!viewer.done())
	{
		dynamicsWorld->stepSimulation(1 / 30.f, 10);


		if (handler->_spawnObject)
		{
	
			camMatrix = viewer.getCamera()->getViewMatrix();
			camMatrix = camManipul->getMatrix();

			handler->spawn(dynamicsWorld, root, camMatrix);

		}

		handler->updateObjects();

		viewer.frame();


	}

	return 0;
}