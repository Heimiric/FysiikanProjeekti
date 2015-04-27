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


	//Creating sphere
	osg::Geode *sphereGeode = maker.makeGeodeWithShape(new osg::Sphere(osg::Vec3(0, 0, 0), 1.0f), osg::Vec4(1, 0, 0, 1));
	btCollisionShape* fallShape = new btSphereShape(1);

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
	fallShape->calculateLocalInertia(mass, fallInertia);

	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);

	btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
	dynamicsWorld->addRigidBody(fallRigidBody);

	osg::PositionAttitudeTransform *transform = new osg::PositionAttitudeTransform;
	transform->addChild(sphereGeode);
	root->addChild(transform);


	root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);


	btTransform trans;
	fallRigidBody->getMotionState()->getWorldTransform(trans);

	transform->setPosition(osg::Vec3(0, 0, trans.getOrigin().getY()));


	fallRigidBody->applyImpulse(btVector3(0, 10, 2), btVector3(0, 0, 0));
	fallRigidBody->setRestitution(0.5);
	groundRigidBody->setRestitution(1.0);


	viewer.setSceneData(root);
	viewer.setCameraManipulator(new osgGA::TrackballManipulator());
	viewer.realize();

	bool doneOnce = false;

	while (!viewer.done())
	{
		dynamicsWorld->stepSimulation(1 / 60.f, 10);


		if (handler->_spawnObject)
		{
			osg::Vec3 camRotation(0, 0, 0);
			osg::Vec3 camPos(10,0,0);

			handler->spawn(dynamicsWorld, root, camPos, camRotation);

		}

		handler->updateObjects();
		fallRigidBody->getMotionState()->getWorldTransform(trans);

		if (!doneOnce)
		{
			
			doneOnce = true;
		}

		viewer.frame();
	}

	return 0;
}