#include "InputHandler.h"

bool InputHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	switch (ea.getEventType())
	{
	case(osgGA::GUIEventAdapter::KEYDOWN) :
	{
		switch (ea.getKey())
		{
		case ' ':
			_spawnObject = true;
			std::cout << " spacebar pressed" << std::endl;
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

void InputHandler::spawn(btDiscreteDynamicsWorld *world, osg::Group *root, osg::Vec3 camPos, osg::Vec3 camRotation)
{
	calcViewTransformation(camPos, camRotation);

	_objects.push_back(new WorldObject(new osg::Sphere(osg::Vec3(0, 0, 0), 1.0), 1.0f,
		&_viewTransform, world, root));
	_spawnObject = false;
}

void InputHandler::calcViewTransformation(osg::Vec3 camPos, osg::Vec3 camRotation)
{
	btVector3 origin(camPos.z(), camPos.x(), camPos.y());
	btQuaternion rotation(camRotation.z(), camRotation.x(), camRotation.y());
	_viewTransform.setOrigin(origin);
	_viewTransform.setRotation(rotation);
}

void InputHandler::updateObjects()
{

	for (int i = 0; i < _objects.size(); i++)
	{
		_objects[i]->updateRender();
	}
}