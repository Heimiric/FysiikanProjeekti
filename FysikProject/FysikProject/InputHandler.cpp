#include "InputHandler.h"

bool InputHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{

	switch (ea.getEventType())
	{
	case(osgGA::GUIEventAdapter::KEYDOWN) :
	{
		switch (ea.getKey())
		{
		case 's':
			std::srand(time(NULL));
			_spawnObject = true;
			std::cout << " object spawned" << std::endl;
			return false;
			break;
		default:
			return false;
		}
	}
	default:
		return false;
	}
}

void InputHandler::spawn(btDiscreteDynamicsWorld *world, osg::Group *root, osg::Matrixd camMatrix)
{
	calcViewTransformation(camMatrix);


	int box = std::rand() % 2;

	if (box)
	{
		_objects.push_back(new WorldObject(new osg::Box(osg::Vec3(0, 0, 0), 10.0f, 10.0f, 10.0f), 10.0f,
			&_viewTransform, world, root));
	}
	else if (!box)
	{
		_objects.push_back(new WorldObject(new osg::Sphere(osg::Vec3(0, 0, 0), 10.0f), 10.0f,
			&_viewTransform, world, root));
	}
	_spawnObject = false;

}

void InputHandler::calcViewTransformation(osg::Matrixd camMatrix)
{
	float z = camMatrix.getTrans().z();
	float y = camMatrix.getTrans().y();
	float x = camMatrix.getTrans().x();


	osg::Quat camQuat = camMatrix.getRotate();
	float rotZ = camQuat.z();
	float rotX = camQuat.x();
	float rotY = camQuat.y();

	btVector3 pos(y, -z, x);
	btQuaternion rot(rotZ, rotY, rotX);

	_viewTransform.setOrigin(pos);
	_viewTransform.setRotation(rot);

}

void InputHandler::updateObjects()
{

	for (int i = 0; i < _objects.size(); i++)
	{
		_objects[i]->updateRender();
	}
}