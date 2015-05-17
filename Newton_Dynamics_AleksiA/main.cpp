#include <Windows.h>
#include <SDKDDKVer.h>
#include <stdio.h>
#include <tchar.h>

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

#include <newton/Newton.h>
#include <dMath/dMatrix.h>



NewtonBody* CreateBackgroundBody(NewtonWorld* const world)
{
	dFloat points[4][3] =
	{
		{ -100.0f, 0.0f, 100.0f },
		{ 100.0f, 0.0f, 100.0f },
		{ 100.0f, 0.0f, -100.0f },
		{ -100.0f, 0.0f, -100.0f },
	};

	// create a collision tree
	NewtonCollision* const collision = NewtonCreateTreeCollision(world, 0);

	// start building the collision mesh
	NewtonTreeCollisionBeginBuild(collision);

	// add the face one at a time
	NewtonTreeCollisionAddFace(collision, 4, &points[0][0], 3 * sizeof (dFloat), 0);

	// finish building the collision
	NewtonTreeCollisionEndBuild(collision, 1);

	// create a body with a collision and locate at the identity matrix position 
	dMatrix matrix(dGetIdentityMatrix());
	NewtonBody* const body = NewtonCreateDynamicBody(world, collision, &matrix[0][0]);

	//destroy the collision
	NewtonDestroyCollision(collision);
	return body;
}

static void ApplyGravity(const NewtonBody* const body, dFloat timestep, int threadIndex)
{
	// apply gravity
	dFloat mass;
	dFloat Ixx;
	dFloat Iyy;
	dFloat Izz;

	NewtonBodyGetMassMatrix(body, &mass, &Ixx, &Iyy, &Izz);
	dVector gravityForce(0.0f, -9.8f * mass, 0.0f, 0.0f);
	NewtonBodySetForce(body, &gravityForce[0]);
}

NewtonBody* CreateFreeFallBall(NewtonWorld* const world/*, dFloat size, dFloat mass, dFloat x, dFloat y, dFloat z*/)
{
	// create a collision sphere
	NewtonCollision* const collision = NewtonCreateSphere(world, 3.0f, 0, NULL);

	// create a dynamic body with a sphere shape
	dMatrix matrix(dGetIdentityMatrix());
	matrix.m_posit.m_y = 50.0f;
	matrix.m_posit.m_x = 40.0f;
	matrix.m_posit.m_z = 50.0f;
	NewtonBody* const body = NewtonCreateDynamicBody(world, collision, &matrix[0][0]);

	// set the force callback for applying force and torque
	NewtonBodySetForceAndTorqueCallback(body, ApplyGravity);

	// set the mass for body
	dFloat mass = 1.0f;
	NewtonBodySetMassProperties(body, mass, collision);

	// set the linear damping to zero
	NewtonBodySetLinearDamping(body, 0.0f);

	//destroy the collision
	NewtonDestroyCollision(collision);
	return body;

}
NewtonBody* CreateFreeFallBall1(NewtonWorld* const world)
{
	// create a collision sphere
	NewtonCollision* const collision = NewtonCreateSphere(world, 1.0f, 0, NULL);

	// creating dynamic body with a sphere shape
	dMatrix matrix(dGetIdentityMatrix());
	matrix.m_posit.m_y = 50.0f;
	matrix.m_posit.m_x = 50.0f;
	matrix.m_posit.m_z = 50.0f;
	NewtonBody* const body = NewtonCreateDynamicBody(world, collision, &matrix[0][0]);

	// setting force callback for applying force and torque
	NewtonBodySetForceAndTorqueCallback(body, ApplyGravity);

	// setting mass for this body
	dFloat mass = 1.0f;
	NewtonBodySetMassProperties(body, mass, collision);

	// setting linear damping to zero
	NewtonBodySetLinearDamping(body, 0.0f);

	//Destroy Collision
	NewtonDestroyCollision(collision);
	return body;

}

NewtonBody* CreateFreeFallBall2(NewtonWorld* const world)
{
	// create a collision sphere
	NewtonCollision* const collision = NewtonCreateSphere(world, 2.0f, 0, NULL);

	// creating dynamic body with a sphere shape
	dMatrix matrix(dGetIdentityMatrix());
	matrix.m_posit.m_y = 55.0f;
	matrix.m_posit.m_x = 49.0f;
	matrix.m_posit.m_z = 50.0f;
	NewtonBody* const body = NewtonCreateDynamicBody(world, collision, &matrix[0][0]);

	// setting force callback for applying force and torque
	NewtonBodySetForceAndTorqueCallback(body, ApplyGravity);

	// setting mass for this body
	dFloat mass = 6.0f;
	NewtonBodySetMassProperties(body, mass, collision);

	// setting linear damping to zero
	NewtonBodySetLinearDamping(body, 0.0f);

	//Destroy Collision
	NewtonDestroyCollision(collision);
	return body;

}



int main()
{

	NewtonWorld* const world = NewtonCreate();
	NewtonBody* const background = CreateBackgroundBody(world);
	NewtonBody* const freeFallBall = CreateFreeFallBall(world);
	NewtonBody* const freeFallBall1 = CreateFreeFallBall1(world);
	NewtonBody* const freeFallBall2 = CreateFreeFallBall2(world);

	NewtonInvalidateCache(world);

	osgViewer::Viewer viewer;
	viewer.setUpViewOnSingleScreen();
	osg::Group* root = new osg::Group();
	osg::Geode* floorGeode = new osg::Geode();
	osg::Geometry* floorGeometry = new osg::Geometry();

	floorGeode->addDrawable(floorGeometry);
	root->addChild(floorGeode);

	osg::Sphere* unitSphere = new osg::Sphere(osg::Vec3(0, 0, 0), 3.0f);
	osg::Sphere* unitSphere1 = new osg::Sphere(osg::Vec3(0, 0, 0), 1.0f);
	osg::Sphere* unitSphere2 = new osg::Sphere(osg::Vec3(0, 0, 0), 2.0f);
	osg::ShapeDrawable* unitSphereDrawable = new osg::ShapeDrawable(unitSphere);
	osg::ShapeDrawable* unitSphereDrawable1 = new osg::ShapeDrawable(unitSphere1);
	osg::ShapeDrawable* unitSphereDrawable2 = new osg::ShapeDrawable(unitSphere2);

	
	osg::Vec3Array* floorVertices = new osg::Vec3Array;
	floorVertices->push_back(osg::Vec3(0, 0, 0));			//FL
	floorVertices->push_back(osg::Vec3(100, 0, 0));		//FR
	floorVertices->push_back(osg::Vec3(100, 100, 0));		//BR
	floorVertices->push_back(osg::Vec3(0, 100, 0));		//BL
	
	floorGeometry->setVertexArray(floorVertices);

	osg::DrawElementsUInt* floorBase = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	floorBase->push_back(3);
	floorBase->push_back(2);
	floorBase->push_back(1);
	floorBase->push_back(0);
	floorGeometry->addPrimitiveSet(floorBase);

	

	osg::Vec4Array* colors = new osg::Vec4Array;

	colors->push_back(osg::Vec4(0.0f, 1.0f, 1.0f, 1.0f));

	floorGeometry->setColorArray(colors);
	floorGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);



	root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);

	viewer.setSceneData(root);

	viewer.setCameraManipulator(new osgGA::TrackballManipulator());
	viewer.realize();

	osg::PositionAttitudeTransform* sphereXForm =
		new osg::PositionAttitudeTransform();

	osg::PositionAttitudeTransform* sphereXForm1 =
		new osg::PositionAttitudeTransform();

	osg::PositionAttitudeTransform* sphereXForm2 =
		new osg::PositionAttitudeTransform();

	osg::Geode* unitSphereGeode = new osg::Geode();
	osg::Geode* unitSphereGeode1 = new osg::Geode();
	osg::Geode* unitSphereGeode2 = new osg::Geode();
	root->addChild(sphereXForm);
	root->addChild(sphereXForm1);
	root->addChild(sphereXForm2);

	dMatrix matrix;
	dMatrix matrix1;
	dMatrix matrix2;
	sphereXForm->addChild(unitSphereGeode);
	sphereXForm1->addChild(unitSphereGeode1);
	sphereXForm2->addChild(unitSphereGeode2);

	unitSphereGeode->addDrawable(unitSphereDrawable);
	sphereXForm->addChild(unitSphereGeode);
	unitSphereGeode1->addDrawable(unitSphereDrawable1);
	sphereXForm1->addChild(unitSphereGeode1);
	unitSphereGeode2->addDrawable(unitSphereDrawable2);
	sphereXForm2->addChild(unitSphereGeode2);
	
		while (!viewer.done())
	{
			viewer.frame();
			
			
				NewtonUpdate(world, 1.0f / 60.f);



				NewtonBodyGetMatrix(freeFallBall, &matrix[0][0]);
				NewtonBodyGetMatrix(freeFallBall1, &matrix1[0][0]);
				NewtonBodyGetMatrix(freeFallBall2, &matrix2[0][0]);

				osg::Vec3 pos = osg::Vec3(matrix.m_posit.m_x, matrix.m_posit.m_z, matrix.m_posit.m_y);
				osg::Vec3 pos1 = osg::Vec3(matrix1.m_posit.m_x, matrix1.m_posit.m_z, matrix1.m_posit.m_y);
				osg::Vec3 pos2 = osg::Vec3(matrix2.m_posit.m_x, matrix2.m_posit.m_z, matrix2.m_posit.m_y);
				sphereXForm->setPosition(pos);
				sphereXForm1->setPosition(pos1);	
				sphereXForm2->setPosition(pos2);
	}

	return 0;
}


