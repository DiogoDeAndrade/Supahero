// ldCompositeMesh.h: interface for the ldCompositeMesh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDCOMPOSITEMESH_H__5D3B0DE8_CC20_4F9E_B1FB_114B50D96BE5__INCLUDED_)
#define AFX_LDCOMPOSITEMESH_H__5D3B0DE8_CC20_4F9E_B1FB_114B50D96BE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../LudiEngine/Headers/ldMesh.h"
#include "../LudiEngine/Headers/ldMaterial.h"

#include <vector>
typedef std::vector<ldMesh*>		ldMeshes;
typedef std::vector<ldMaterial*>	ldMaterials;
typedef std::vector<char*>			ldNames;

class ldCompositeMesh : public ldMesh  
{
public:
	ldCompositeMesh();
	virtual ~ldCompositeMesh();

	// This method isn't to be used, as none of the attributes of the base class... This class
	// only derives from ldMesh for convenience, not as a good programming practice
	virtual void	close();

	inline	unsigned long	add_mesh(char *name,ldMesh *mesh) { char *tmp=strdup(name); names.push_back(name); meshes.push_back(mesh); return meshes.size()-1; }
	inline	unsigned long	add_material(ldMaterial *material) { materials.push_back(material); return materials.size()-1; }

	// Draw the meshes
	virtual void	draw();
	virtual void	draw(unsigned long i);
	virtual void	draw(char *name);

	virtual unsigned long	get_vertex_count();
	virtual unsigned long	get_index_count();
	virtual unsigned long	get_triangle_count();

protected:
	ldMeshes	meshes;
	ldMaterials	materials;
	ldNames		names;
};

#endif // !defined(AFX_LDCOMPOSITEMESH_H__5D3B0DE8_CC20_4F9E_B1FB_114B50D96BE5__INCLUDED_)
