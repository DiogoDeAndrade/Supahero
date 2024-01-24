/******************************************************************************************
* AUTHOR: Diogo de Andrade <diogo.andrade@netvisao.pt>                                    *
*                                                                                         *
* This file, and all files in the LudiEngine is released with the 'I don't give a damn'   *
* licence... This means I'm not responsible for anything that may happen due to the use   *
* of this engine, but you can use it in any way you like it...                            *
* I would just appreciate (although it is not mandatory) if some comments on it could be  *
* made, revisions you make yourself, new filters, etc, etc, etc...                        *
* Credit where credit is due...                                                           *
*                                                                                         *
* COPYRIGHT: Copyright 2002, Diogo Andrade. All rights reserved.                          *
******************************************************************************************/
// ldGeoFilter.h: interface for the ldGeoFilter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDGEOFILTER_H__89CE8131_F370_4418_B2CC_64085DFBB8E0__INCLUDED_)
#define AFX_LDGEOFILTER_H__89CE8131_F370_4418_B2CC_64085DFBB8E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ldGeometry.h"
#include "ldQuaternion.h"

class ldMesh;
struct ldVertex;

typedef bool (*ldAcceptanceFunction)(ldVertex *vertex);

#include <vector>
#include <list>

typedef std::list<ldAcceptanceFunction>	ldAcceptanceFunctions;

class ldGeoFilter  
{
protected:

	// Structures for smooth normals (not indexed)
	typedef std::vector<unsigned long>	ldULongVector;

	struct smooth_normals_struct {
		Vec3f			pos;
		Vec3f			normal;
		ldULongVector	vertex;
	};

	typedef std::vector<smooth_normals_struct> smooth_normals_structVector;
	
	// Structures for smooth normals (indexed)

	typedef std::vector<Vec3f> vectorVector;

public:
	ldGeoFilter();
	virtual ~ldGeoFilter();

	// Filters
	void	scale_along_normals(ldMesh *mesh,float min,float max);
	void	scale_texture_coords(ldMesh *mesh,long index,float su,float sv);
	void	convert_to_indexed(ldMesh *mesh);
	void	move(ldMesh *mesh,Vec3f *pos);
	void	move(ldMesh *mesh,float x,float y,float z);
	void	rotate(ldMesh *mesh,Quaternion *ori);
	void	rotate(ldMesh *mesh,float x,float y,float z,float ang);
	void	scale(ldMesh *mesh,float x,float y,float z);
	void	smooth_normals(ldMesh *mesh);

public:
	inline	void	add_acceptance_function(ldAcceptanceFunction func) { _acceptance_functions.push_back(func); }
			void	remove_acceptance_function(ldAcceptanceFunction func);
	inline	void	set_tolerance(float t) { _tolerance=t; }

protected:
	bool	vertex_is_accepted(ldVertex *vertex);

protected:
	ldAcceptanceFunctions	_acceptance_functions;
	float					_tolerance;
};

#endif // !defined(AFX_LDGEOFILTER_H__89CE8131_F370_4418_B2CC_64085DFBB8E0__INCLUDED_)
