#pragma once

#include "shapeTransform.h"
#include <osg/Node>

class RigidBodyCallBack : public osg::NodeCallback
{
public:
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nodeVisitor)
	{
		ShapeTransform* shapeTransform = dynamic_cast<ShapeTransform*>(node->getParent(0));

		if(shapeTransform)
		{
			shapeTransform->updateTransform();
		}

		traverse(node, nodeVisitor);
	}
};

