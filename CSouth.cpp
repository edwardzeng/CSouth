#include "CSouth.h"

CSouth::CSouth()
{
	m_vPosition = osg::Vec3(0, 0, 5.0);
	m_vRotation = osg::Vec3(osg::PI_2, 0, 0);
	m_vStep =1.0;
	m_vRotateStep=0.01;
	m_bLeftDown =false;
	m_node =0;
}

osg::Matrixd CSouth::getMatrix() const
{
	osg::Matrixd mat;
	mat.makeTranslate(m_vPosition);
	return osg::Matrixd::rotate(m_vRotation[0],osg::X_AXIS,m_vRotation[1], osg::Y_AXIS, m_vRotation[2], osg::Z_AXIS)*mat;
}

osg::Matrixd CSouth::getInverseMatrix()const
{
	osg::Matrixd mat;
	mat.makeTranslate(m_vPosition);
	return osg::Matrixd::inverse(osg::Matrixd::rotate(m_vRotation[0],osg::X_AXIS,m_vRotation[1], osg::Y_AXIS, m_vRotation[2], osg::Z_AXIS)*mat);
}

bool CSouth::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us)
{
	osgViewer::Viewer* view =dynamic_cast <osgViewer::Viewer* >(&us);
	switch(ea.getEventType())
	{
	case osgGA::GUIEventAdapter::KEYDOWN:
		{
			if(ea.getKey()=='w'|| ea.getKey()=='W')
			{
				changePosition(osg::Vec3d(m_vStep * cosf(osg::PI_2 + m_vRotation._v[2]), m_vStep * sinf(osg::PI_2 + m_vRotation._v[2]), 0));
				std::cout<<"key w"<<std::endl;
			}
			
			else if(ea.getKey()=='s' || ea.getKey()=='S')
			{
				changePosition(osg::Vec3d(-m_vStep * cosf(osg::PI_2 + m_vRotation._v[2]), -m_vStep * sinf(osg::PI_2 + m_vRotation._v[2]), 0));
			}
			else if(ea.getKey()=='a' || ea.getKey()=='A')
			{
				changePosition(osg::Vec3d(-m_vStep * sinf(osg::PI_2 + m_vRotation._v[2]), m_vStep * cosf(osg::PI_2 + m_vRotation._v[2]), 0));
			}
			else if(ea.getKey()=='d' || ea.getKey()=='D')
			{
				changePosition(osg::Vec3d(m_vStep * sinf(osg::PI_2 + m_vRotation._v[2]), -m_vStep * cosf(osg::PI_2 + m_vRotation._v[2]), 0));
			}
			else if(ea.getKey()=='+')
			{
				setStep(1);
			}
			else if(ea.getKey()=='-')
			{
				setStep(-1);
			}
			else if(ea.getKey()==osgGA::GUIEventAdapter::KEY_Home)
			{
				changePosition(osg::Vec3(0, 0, 1.0));
			}
			else if(ea.getKey()==osgGA::GUIEventAdapter::KEY_End)
			{
				changePosition(osg::Vec3(0, 0, -1.0));
			}
		}
		break;
	case osgGA::GUIEventAdapter::PUSH:
		{
			if(ea.getButton()== osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
			{
				m_iLeftX = ea.getX();
				m_iLeftY = ea.getY();
				m_bLeftDown =true;
				std::cout<<"left button pushed"<<std::endl;
			}
			return false;
		}
		break;
	case osgGA::GUIEventAdapter::DRAG:
		{
			if(m_bLeftDown)
			{
				int delX=ea.getX() -m_iLeftX;
				int delY=ea.getY() -m_iLeftY;
				m_vRotation[2] -= osg::DegreesToRadians(m_vRotateStep*delX);
				m_vRotation[0] += osg::DegreesToRadians(m_vRotateStep*delY);
				if(m_vRotation[0] <= 0)
				{
					m_vRotation[0]=0;
				}
				if(m_vRotation[0] >= osg::PI)
				{
					m_vRotation[0]=osg::PI;
				}
				std::cout<<"mouse drag"<<std::endl;
			}
		}
		break;
	case osgGA::GUIEventAdapter::RELEASE:
		{
			if(ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
			{
				m_bLeftDown =false;
				std::cout<<"release button"<<std::endl;
			}
		}
		break;
	default:
		break;
	}
	return false;
}

void CSouth::setStep(int delta_step)
{
	m_vStep += delta_step;
	if(m_vStep <=0)
		m_vStep =0;
}

bool CSouth::changePosition (osg::Vec3 delta)
{
	if(m_node)
	{
		osg::Vec3 newPos = m_vPosition + delta;
		osg::Vec3 start = osg::Vec3(newPos.x(), newPos.y(), newPos.z() +80);
		osg::Vec3 end = osg::Vec3(newPos.x(), newPos.y(),newPos.z() -1000);
		osg::ref_ptr <osgUtil::IntersectionVisitor > iv = new osgUtil::IntersectionVisitor;
		osg::ref_ptr <osgUtil::LineSegmentIntersector > ls=new osgUtil::LineSegmentIntersector(start,end);
		osgUtil::LineSegmentIntersector::Intersections itersections;

		long height =newPos.z();
		iv->setIntersector(ls.get());
		m_node->accept(*(iv.get()));
		if(ls->containsIntersections())
		{
			itersections =ls->getIntersections();
			osgUtil::LineSegmentIntersector:: Intersections::iterator iter =itersections.begin();
			height=iter->getLocalIntersectPoint().z()+5;
		}
		else
		{
			return false;
			std::cout<<"no intersections"<<std::endl;
		}
		osg::Vec3 start2 = osg::Vec3(m_vPosition.x(),m_vPosition.y(),height);
		osg::Vec3 end2 = osg::Vec3(newPos.x(),newPos.y(),height);
		osgUtil::IntersectVisitor iv2;
		osg::ref_ptr< osg::LineSegment > line = new osg::LineSegment(start2,end2);

		iv2.addLineSegment (line.get());
		m_node->accept(iv2);
		if(!iv2.hits())
		{
			m_vPosition += delta;
			m_vPosition.set(m_vPosition.x(),m_vPosition.y(),m_vPosition.z());
		}
		else
		{
			return false;
		}		
	}
	else
	{
			m_vPosition += delta;
	}
	return true;
}
void CSouth::setMnode(osg::Node* temp)
{
	m_node =temp;
}
