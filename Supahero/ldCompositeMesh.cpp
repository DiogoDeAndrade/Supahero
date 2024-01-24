// ldCompositeMesh.cpp: implementation of the ldCompositeMesh class.
//
//////////////////////////////////////////////////////////////////////

#include "ldCompositeMesh.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ldCompositeMesh::ldCompositeMesh()
{

}

ldCompositeMesh::~ldCompositeMesh()
{
	for (unsigned long i=0; i<meshes.size(); i++) 
	{
		delete meshes[i];
	}
	meshes.clear();

	for (i=0; i<materials.size(); i++) 
	{
		for (unsigned long j=0; j<materials.size(); j++)
		{
			if ((i!=j) && (materials[j]==materials[i])) materials[j]=NULL;
		}
		delete materials[i];
		materials[i]=NULL;
	}
	materials.clear();
}

void ldCompositeMesh::close()
{
}

void ldCompositeMesh::draw()
{
	for (unsigned long i=0; i<meshes.size(); i++) draw(i);
}

void ldCompositeMesh::draw(unsigned long i)
{
	if (materials[i]) materials[i]->set();
	if (meshes[i]) meshes[i]->draw();
}

void ldCompositeMesh::draw(char *name)
{
	for (unsigned long i=0; i<meshes.size(); i++)
	{
		if (!stricmp(name,names[i]))
		{
			if (materials[i]) materials[i]->set();
			if (meshes[i]) meshes[i]->draw();
			return;
		}
	}
}

unsigned long ldCompositeMesh::get_vertex_count() 
{ 
	unsigned long count=0;
	for (unsigned long i=0; i<meshes.size(); i++) count+=meshes[i]->get_vertex_count();

	return count;
}

unsigned long ldCompositeMesh::get_index_count() 
{ 
	unsigned long count=0;
	for (unsigned long i=0; i<meshes.size(); i++) count+=meshes[i]->get_index_count();

	return count;
}

unsigned long ldCompositeMesh::get_triangle_count() 
{ 
	unsigned long count=0;
	for (unsigned long i=0; i<meshes.size(); i++) count+=meshes[i]->get_triangle_count();

	return count;
}
