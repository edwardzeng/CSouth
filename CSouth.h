#ifndef _CSOUTH_H_
#define _CSOUTH_H_

#include <osgGA/CameraManipulator>
#include <osgViewer/Viewer>
#include <osgUtil/IntersectVisitor>
#include <osg/LineSegment>
#include <osgDB/ReadFile>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Geometry>


class CSouth : public osgGA::CameraManipulator
{
public:
	CSouth();
	virtual void setByMatrix(const osg::Matrixd& matrix){};
	virtual osg::Matrixd getMatrix() const;
	virtual void setByInverseMatrix(const osg::Matrixd& matrix){};
	virtual osg::Matrixd getInverseMatrix() const;
	virtual bool handle ( const osgGA:: GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);
	bool changePosition(osg::Vec3 delta);
	void setStep(int delta_step);
	void setMnode(osg::Node* Node);
private:
	osg::Vec3 m_vPosition;
	osg::Vec3 m_vRotation;
	int m_vStep;
	float m_vRotateStep;
	int m_iLeftX;
	int m_iLeftY;
	bool m_bLeftDown;
	osg::Node * m_node;
};
#endif//_CSOUTH_H_