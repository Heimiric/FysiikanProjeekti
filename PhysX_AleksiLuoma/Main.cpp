#include <Windows.h>
#include <vector>
#include "Ball.h"
#include "Box.h"

physx::PxPhysics* mPhysics = nullptr;
physx::PxScene* mScene = nullptr;
physx::PxReal mTimeStep = 1.f / 60.f;
physx::PxReal mGravity = 9.81f;
osg::Group* root;

void initPhysX()
{
	static physx::PxDefaultErrorCallback gDefaultErrorCallback;
	static physx::PxDefaultAllocator gDefaultAllocatorCallback;
	static physx::PxSimulationFilterShader gDefaultFilterShader = physx::PxDefaultSimulationFilterShader;
	physx::PxFoundation *mFoundation = nullptr;

	// create foundation object with default error and allocator callbacks.
	mFoundation = PxCreateFoundation(
		PX_PHYSICS_VERSION,
		gDefaultAllocatorCallback,
		gDefaultErrorCallback);

	// create Physics object with the created foundation and with a 'default' scale tolerance.
	mPhysics = PxCreatePhysics(
		PX_PHYSICS_VERSION,
		*mFoundation,
		physx::PxTolerancesScale());

	PxInitExtensions(*mPhysics);

	//Scene
	physx::PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -mGravity, 0.0f);

	if (!sceneDesc.cpuDispatcher)
	{
		physx::PxDefaultCpuDispatcher* mCpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
		sceneDesc.cpuDispatcher = mCpuDispatcher;
	}
	if (!sceneDesc.filterShader)
	{
		sceneDesc.filterShader = gDefaultFilterShader;
	}

	mScene = mPhysics->createScene(sceneDesc);

	/*//Shutting down
	mPhysics->release();
	mFoundation->release();
	mScene->release();*/
}

void stepPhysX()
{
	mScene->simulate(mTimeStep);
	while (!mScene->fetchResults())
	{
		//can do something
	}
}

void setObjects(std::vector<Ball*> &balls, std::vector<Box*> &boxes)
{
	float y = 2.5f;
	float x = 30.f;
	float z = -75.f;
	float col = 0.f;
	for (int i = 0; i < 60; i++)
	{
		if (x >= -30.f)
		{
			x--;
			z -= 2;
			col += 10.f;
		}
		else
		{
			x = 30.f;
			z = -50.f;
			col = 0;
		}

		Box* temp = new Box(root, mPhysics, mScene);
		temp->setPosition(x, -25+y*i, z);
		temp->setColor(0.f, col / 255.f, 1.f);
		temp->setRestitution(0.75);
		temp->setMass(20.f);
		temp->setDimensions(1.f, 1.f, 1.f);
		boxes.push_back(temp);
	}
	x = -30.f;
	z = -75.f;
	for (int i = 0; i < 60; i++)
	{
		if (x <= 30.f)
		{
			x++;
			z -= 2;
			col += 7.5f;
		}
		else
		{
			x = -30.f;
			z = -50.f;
			col = 0.f;
		}

		Ball* temp = new Ball(root, mPhysics, mScene);
		temp->setPosition(x, -25+y*i, z);
		temp->setColor(1.f, col / 255.f, 0.f);
		temp->setRestitution(0.5f);
		temp->setMass(50.f);
		temp->setRadius(1.f);
		balls.push_back(temp);
	}
}

int main()
{
	///PHYSX
	initPhysX();

	//Plane
	physx::PxMaterial* planeMaterial = mPhysics->createMaterial(0.9f, 0.1f, 1.0f);
	physx::PxTransform pose = physx::PxTransform(physx::PxVec3(0.0f, -25.0f, 0.0f), physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0.0f, 0.0f, 1.0f)));
	physx::PxRigidStatic* plane = mPhysics->createRigidStatic(pose);
	physx::PxShape* shape = plane->createShape(physx::PxPlaneGeometry(), *planeMaterial);
	mScene->addActor(*plane);

	///OSG
	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);

	// Declare a group to act as root node of a scene :
	root = new osg::Group();
	
	//// Create ballz & boxez
	std::vector<Ball*> balls;

	std::vector<Box*> boxes;

	setObjects(balls, boxes);
	////

	root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	viewer.setSceneData(root);

	/*viewer.setCameraManipulator(new osgGA::TrackballManipulator());
	osg::Vec3d eyePos, lookAt, up;
	eyePos = osg::Vec3d(-200.f, -100.f, -10.f);
	lookAt = osg::Vec3d(0.f, 0.f, 0.f);
	up = osg::Vec3d(0.f, -1.f, 0.f);
	viewer.getCameraManipulator()->setHomePosition(eyePos, lookAt, up);*/
	viewer.realize();
	///

	//Loop
	while (!viewer.done())
	{
		stepPhysX();
		for (int i = 0; i < balls.size(); i++)
		{
			balls[i]->update();
		}
		for (int i = 0; i < boxes.size(); i++)
		{
			boxes[i]->update();
		}
		viewer.frame();
	}

	return 0;
}