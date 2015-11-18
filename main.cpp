#include "CSouth.h"
#include <iostream>


osg::Node* createFloor(const osg::Vec3& center,float radius)
{
	int numTilesX=10;
	int numTilesY =10;

	float width =2*radius;
	float height=2*radius;

	osg::Vec3 v000(center -osg::Vec3(width*0.5f,height*0.5f,0.0f));
	osg::Vec3 dx(osg::Vec3(width/((float)numTilesX),0.0,0.0f));
	osg::Vec3 dy(osg::Vec3(0.0f,height/((float)numTilesY),0.0f));

	osg::Vec3Array* coords =new osg::Vec3Array;
	int iy;
	for(iy =0;iy<=numTilesY; ++iy)
	{
		for(int ix=0;ix<=numTilesX; ++ix)
		{
			coords->push_back(v000 +dx*(float)ix +dy*(float)iy);
		}
	}


	osg::Vec4Array* colors =new osg::Vec4Array;
	colors->push_back(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
	colors->push_back(osg::Vec4(0.0f,0.0f,0.0f,1.0f));
	int numColors =colors->size();
	int numIndicesPerRow =numTilesX +1;
	osg::UByteArray* coordIndices =new osg::UByteArray;
	osg::UByteArray * colorIndices = new osg::UByteArray;
	for(iy=0;iy <numTilesY; ++iy)
	{
		for(int ix =0;ix <numTilesX;++ix)
		{
			coordIndices->push_back(ix + (iy + 1) * numIndicesPerRow);
			coordIndices->push_back(ix+iy*numIndicesPerRow);
			coordIndices->push_back((ix +1) + iy*numIndicesPerRow);
			coordIndices->push_back((ix +1) + (iy +1)*numIndicesPerRow);
			colorIndices->push_back((ix + iy)% numColors);
		}
	}



	osg::Vec3Array* normals =new osg::Vec3Array;
	normals->push_back(osg::Vec3(0.0f,0.0f,1.0f));
	deprecated_osg::Geometry* geom =new deprecated_osg::Geometry;
	geom->setVertexArray(coords);
	geom->deprecated_osg::Geometry::setVertexIndices(coordIndices);

	geom->setColorArray(colors);
	geom->setColorIndices(colorIndices);
	geom-> setColorBinding(deprecated_osg::Geometry::BIND_PER_PRIMITIVE);

	geom->setNormalArray(normals);
	geom->setNormalBinding(deprecated_osg::Geometry::BIND_OVERALL);

	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,coordIndices->size()));

	osg::Geode* geode =new osg::Geode;
	geode->addDrawable(geom);
	return geode;
}

osg::Geode* CreateStair()
{
	osg::ref_ptr<osg::Geode> gnode = new osg::Geode;
	for(int i=0;i<=20;i++)
	{
		osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0, 50.0+i*2, 1.0+i*2),10.0, 4.0, 0.5));
		gnode->addDrawable(shape.get());
		
	}
	return gnode.release();
}

int main()
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
	osg::ref_ptr<osg::Group> gp =new osg::Group;
	gp->addChild(createFloor(osg::Vec3(0, 0, 0),200));
	gp->addChild(CreateStair());
	viewer->setSceneData(gp);
	CSouth* cs = new CSouth;
	cs->setMnode(gp);
	viewer->setCameraManipulator(cs);
	return viewer->run();
}

