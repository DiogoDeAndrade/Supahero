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

// ldBoundingSphere.cpp: implementation of the ldBoundingSphere class.
//
//////////////////////////////////////////////////////////////////////

#include "..\HEADERS\ldBoundingSphere.h"
#include "..\HEADERS\ldUtils.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ldBoundingSphere::ldBoundingSphere()
{
	set_center(0.0f,0.0f,0.0f);
	set_radius(0.0f);
}

ldBoundingSphere::ldBoundingSphere(float r)
{
	set_center(0.0f,0.0f,0.0f);
	set_radius(r);
}

ldBoundingSphere::ldBoundingSphere(Vec3f *center,float r)
{
	set_center(center);
	set_radius(r);
}

ldBoundingSphere::ldBoundingSphere(float x,float y,float z,float r)
{
	set_center(x,y,z);
	set_radius(r);
}

ldBoundingSphere::~ldBoundingSphere()
{

}

bool ldBoundingSphere::is_inside(Vec3f *pt)
{
	if (_radius==0.0f) return false;

	Vec3f vec=(*pt)-_center;

	if (vec.norm_squared()<=(_radius*_radius)) return true;

	return false;
}

bool ldBoundingSphere::is_colliding(ldBoundingSphere *sphere)
{
	if (_radius==0.0f) return false;

	Vec3f vec;

	sphere->get_center(&vec);
	float r=sphere->get_radius();

	vec=vec-_center;

	if (vec.norm_squared()<=Sqr(_radius+r)) 
		return true;

	return false;
}

bool ldBoundingSphere::is_colliding(Vec3f *pos1,Vec3f *pos2,Vec3f *pos3,float *s,float *t)
{
	if (_radius==0.0f) return false;

	Vec3f	pt1=*pos1;
	Vec3f	pt2=*pos2; pt2=pt2-pt1;
	Vec3f	pt3=*pos3; pt3=pt3-pt1;

	float fSqrDist;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Adapted from source code from Dave H. Eberly (http://www.magic-software.com/License/free.pdf)
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	Vec3f	kDiff=pt1-_center;
	float	fA00=pt2.norm_squared();
	float	fA01=dot_product(pt2,pt3);
	float	fA11=pt3.norm_squared();
	float	fB0=dot_product(&kDiff,&pt2);
	float	fB1=dot_product(&kDiff,&pt3);
	float	fC=kDiff.norm_squared();
	float	fDet=fabs(fA00*fA11-fA01*fA01);
	float	fS=fA01*fB1-fA11*fB0;
	float	fT=fA01*fB0-fA00*fB1;

    if (fS+fT<=fDet)
    {
        if (fS<0.0f)
        {
            if (fT<0.0f)  // region 4
            {
                if (fB0<0.0f)
                {
                    fT=0.0f;
                    if (-fB0>=fA00)
                    {
                        fS=1.0f;
                        fSqrDist=fA00+2.0f*fB0+fC;
                    }
                    else
                    {
                        fS=-fB0/fA00;
                        fSqrDist=fB0*fS+fC;
                    }
                }
                else
                {
                    fS=0.0f;
                    if (fB1>=0.0f)
                    {
                        fT=0.0f;
                        fSqrDist=fC;
                    }
                    else if (-fB1>=fA11)
                    {
                        fT=1.0f;
                        fSqrDist=fA11+2.0f*fB1+fC;
                    }
                    else
                    {
                        fT=-fB1/fA11;
                        fSqrDist=fB1*fT+fC;
                    }
                }
            }
            else  // region 3
            {
                fS=0.0f;
                if (fB1>=0.0f)
                {
                    fT=0.0f;
                    fSqrDist=fC;
                }
                else if (-fB1>=fA11)
                {
                    fT=1.0f;
                    fSqrDist=fA11+2.0f*fB1+fC;
                }
                else
                {
                    fT=-fB1/fA11;
                    fSqrDist=fB1*fT+fC;
                }
            }
        }
        else if (fT<0.0f)  // region 5
        {
            fT=0.0f;
            if (fB0>=0.0f)
            {
                fS=0.0f;
                fSqrDist=fC;
            }
            else if (-fB0>=fA00)
            {
                fS=1.0f;
                fSqrDist=fA00+2.0f*fB0+fC;
            }
            else
            {
                fS=-fB0/fA00;
                fSqrDist=fB0*fS+fC;
            }
        }
        else  // region 0
        {
            // minimum at interior point
            float fInvDet=1.0f/fDet;
            fS*=fInvDet;
            fT*=fInvDet;
            fSqrDist=fS*(fA00*fS+fA01*fT+2.0f*fB0)+
					 fT*(fA01*fS+fA11*fT+2.0f*fB1)+fC;
        }
    }
    else
    {
        float fTmp0,fTmp1,fNumer,fDenom;

        if (fS<0.0f)  // region 2
        {
            fTmp0=fA01+fB0;
            fTmp1=fA11+fB1;
            if (fTmp1>fTmp0)
            {
                fNumer=fTmp1-fTmp0;
                fDenom=fA00-2.0f*fA01+fA11;
                if (fNumer>=fDenom)
                {
                    fS=1.0f;
                    fT=0.0f;
                    fSqrDist=fA00+2.0f*fB0+fC;
                }
                else
                {
                    fS=fNumer/fDenom;
                    fT=1.0f-fS;
                    fSqrDist=fS*(fA00*fS+fA01*fT+2.0f*fB0)+
							 fT*(fA01*fS+fA11*fT+2.0f*fB1)+fC;
                }
            }
            else
            {
                fS=0.0f;
                if (fTmp1<=0.0f)
                {
                    fT=1.0f;
                    fSqrDist=fA11+2.0f*fB1+fC;
                }
                else if (fB1>=0.0f)
                {
                    fT=0.0f;
                    fSqrDist=fC;
                }
                else
                {
                    fT=-fB1/fA11;
                    fSqrDist=fB1*fT+fC;
                }
            }
        }
        else if (fT<0.0f)  // region 6
        {
            fTmp0=fA01+fB1;
            fTmp1=fA00+fB0;
            if (fTmp1>fTmp0)
            {
                fNumer=fTmp1-fTmp0;
                fDenom=fA00-2.0f*fA01+fA11;
                if (fNumer>=fDenom)
                {
                    fT=1.0f;
                    fS=0.0f;
                    fSqrDist=fA11+2.0f*fB1+fC;
                }
                else
                {
                    fT=fNumer/fDenom;
                    fS=1.0f-fT;
                    fSqrDist=fS*(fA00*fS+fA01*fT+2.0f*fB0)+
							 fT*(fA01*fS+fA11*fT+2.0f*fB1)+fC;
                }
            }
            else
            {
                fT=0.0f;
                if (fTmp1<=0.0f)
                {
                    fS=1.0f;
                    fSqrDist=fA00+2.0f*fB0+fC;
                }
                else if (fB0>=0.0f)
                {
                    fS=0.0f;
                    fSqrDist=fC;
                }
                else
                {
                    fS=-fB0/fA00;
                    fSqrDist=fB0*fS+fC;
                }
            }
        }
        else  // region 1
        {
            fNumer=fA11+fB1-fA01-fB0;
            if (fNumer<=0.0f)
            {
                fS=0.0f;
                fT=1.0f;
                fSqrDist=fA11+2.0f*fB1+fC;
            }
            else
            {
                fDenom=fA00-2.0f*fA01+fA11;
                if (fNumer>=fDenom)
                {
                    fS=1.0f;
                    fT=0.0f;
                    fSqrDist=fA00+2.0f*fB0+fC;
                }
                else
                {
                    fS=fNumer/fDenom;
                    fT=1.0f - fS;
                    fSqrDist=fS*(fA00*fS+fA01*fT+2.0f*fB0)+
							 fT*(fA01*fS+fA11*fT+2.0f*fB1)+fC;
                }
            }
        }
    }

    if (s) *s=fS;
    if (t) *t=fT;

    fSqrDist=fabs(fSqrDist);
	
	if (fSqrDist<Sqr(_radius)) return true;

	return false;
}

bool ldBoundingSphere::get_ray_intersection(Vec3f *origin,Vec3f *dir,float *t)
{
	if (_radius==0.0f) return false;

	float b,c,b2,ac4;
	float deltaX,deltaY,deltaZ;
	float t1,t2;

	dir->normalize();

	if (dir->norm_squared==0) return false;

	deltaX=(origin->x-_center.x); 
	deltaY=(origin->y-_center.y); 
	deltaZ=(origin->z-_center.z); 

	b=2*(deltaX*(dir->x)+deltaY*(dir->y)+deltaZ*(dir->z));
	b2=Sqr(b);
	c=Sqr(deltaX)+Sqr(deltaY)+Sqr(deltaZ)-Sqr(_radius);
	ac4=4*c;
	if (b2>=ac4)
	{
		t1=float((-b+sqrt(b2-ac4))/2.0f);
		t2=float((-b-sqrt(b2-ac4))/2.0f);
		if ((t1<0.0f) && (t2<0.0f)) return false;

		if (t)
		{
			if (t2>0.0f) *t=t2;
			else *t=t1;
		}
		return true;
	}
	
	return false;
}
