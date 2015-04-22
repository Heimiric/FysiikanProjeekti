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

#include <PxPhysicsAPI.h>

#pragma comment(lib, "PhysX3_x64.lib")
#pragma comment(lib, "PhysX3Common_x64.lib")
#pragma comment(lib, "PhysX3Extensions.lib")
#pragma comment(lib, "PhysXProfileSDK")

static physx::PxDefaultAllocator gDefaultAllocatorCallback;
static physx::PxDefaultErrorCallback gDefaultErrorCallback;
static physx::PxSimulationFilterShader gDefaultFilterShader = physx::PxDefaultSimulationFilterShader;

physx::PxScene* gScene = NULL;

void initializePhysX()
{
	physx::PxFoundation* physXFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);

	physx::PxPhysics* gPhysicisSDK = PxCreateBasePhysics(PX_PHYSICS_VERSION, *physXFoundation, physx::PxTolerancesScale());

	PxInitExtensions(*gPhysicisSDK);

	//Scene
	physx::PxSceneDesc sceneDesc(gPhysicisSDK->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);

	if (!sceneDesc.cpuDispatcher)
	{
		physx::PxDefaultCpuDispatcher* mCpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
		sceneDesc.cpuDispatcher = mCpuDispatcher;
	}

	if (!sceneDesc.filterShader)
		sceneDesc.filterShader = gDefaultFilterShader;

	gScene = gPhysicisSDK->createScene(sceneDesc);
}

int main()
{
	/*osgViewer::GraphicsWindow window;
	window.setWindowName("Physics! It's science bitch!");
	window.setWindowRectangle(10, 10, 800, 600);*/

	initializePhysX();

	osgViewer::Viewer viewer;
	viewer.setUpViewOnSingleScreen();
	osg::Group* root = new osg::Group();
	osg::Geode* pyramidGeode = new osg::Geode();
	osg::Geometry* pyramidGeometry = new osg::Geometry();

	pyramidGeode->addDrawable(pyramidGeometry);
	root->addChild(pyramidGeode);

	osg::Vec3Array* pyramidVertices = new osg::Vec3Array;
	pyramidVertices->push_back(osg::Vec3(0, 0, 0));			//FL
	pyramidVertices->push_back(osg::Vec3(10, 0, 0));		//FR
	pyramidVertices->push_back(osg::Vec3(10, 10, 0));		//BR
	pyramidVertices->push_back(osg::Vec3(0, 10, 0));		//BL
	pyramidVertices->push_back(osg::Vec3(5, 5, 10));		//P

	pyramidGeometry->setVertexArray(pyramidVertices);

	osg::DrawElementsUInt* pyramidBase = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	pyramidBase->push_back(3);
	pyramidBase->push_back(2);
	pyramidBase->push_back(1);
	pyramidBase->push_back(0);
	pyramidGeometry->addPrimitiveSet(pyramidBase);

	osg::DrawElementsUInt* pyramidFaceOne = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	pyramidFaceOne->push_back(0);
	pyramidFaceOne->push_back(1);
	pyramidFaceOne->push_back(4);
	pyramidGeometry->addPrimitiveSet(pyramidFaceOne);

	osg::DrawElementsUInt* pyramidFaceTwo = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	pyramidFaceTwo->push_back(1);
	pyramidFaceTwo->push_back(2);
	pyramidFaceTwo->push_back(4);
	pyramidGeometry->addPrimitiveSet(pyramidFaceTwo);
	
	osg::DrawElementsUInt* pyramidFaceThree = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	pyramidFaceThree->push_back(2);
	pyramidFaceThree->push_back(3);
	pyramidFaceThree->push_back(4);
	pyramidGeometry->addPrimitiveSet(pyramidFaceThree);
	
	osg::DrawElementsUInt* pyramidFaceFour = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	pyramidFaceFour->push_back(3);
	pyramidFaceFour->push_back(0);
	pyramidFaceFour->push_back(4);
	pyramidGeometry->addPrimitiveSet(pyramidFaceFour);

	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	colors->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
	colors->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
	colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	colors->push_back(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));

	pyramidGeometry->setColorArray(colors);
	pyramidGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	osg::PositionAttitudeTransform* pyramidTwoXForm = new osg::PositionAttitudeTransform();

	root->addChild(pyramidTwoXForm);
	pyramidTwoXForm->addChild(pyramidGeode);

	osg::Vec3 pyramidTwoPosition(15, 0, 0);
	pyramidTwoXForm->setPosition(pyramidTwoPosition);

	root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	viewer.setSceneData(root);

	viewer.setCameraManipulator(new osgGA::TrackballManipulator());
	viewer.realize();

	while (!viewer.done())
	{
		viewer.frame();
	}

	return 0;
}