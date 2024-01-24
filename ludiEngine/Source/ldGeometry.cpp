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

#include "..\HEADERS\ldGeometry.h"

float get_distance_from_point_to_tri(Vec3f *pt,
									Vec3f *pos1,Vec3f *pos2,Vec3f *pos3,
									float *pfSParam=NULL,float *pfTParam=NULL)
{
    Vec3f kDiff = (*pos1) - (*pt);
    float fA00 = pos2->norm_squared();
    float fA01 = dot_product(pos2,pos3);
    float fA11 = pos3->norm_squared();
    float fB0 = dot_product(&kDiff,pos2);
    float fB1 = dot_product(&kDiff,pos3);
    float fC = kDiff.norm_squared();
    float fDet = fabs(fA00*fA11-fA01*fA01);
    float fS = fA01*fB1-fA11*fB0;
    float fT = fA01*fB0-fA00*fB1;
    float fSqrDist;

    if ( fS + fT <= fDet )
    {
        if ( fS < 0.0f )
        {
            if ( fT < 0.0f )  // region 4
            {
                if ( fB0 < 0.0f )
                {
                    fT = 0.0f;
                    if ( -fB0 >= fA00 )
                    {
                        fS = 1.0f;
                        fSqrDist = fA00+2.0f*fB0+fC;
                    }
                    else
                    {
                        fS = -fB0/fA00;
                        fSqrDist = fB0*fS+fC;
                    }
                }
                else
                {
                    fS = 0.0f;
                    if ( fB1 >= 0.0f )
                    {
                        fT = 0.0f;
                        fSqrDist = fC;
                    }
                    else if ( -fB1 >= fA11 )
                    {
                        fT = 1.0f;
                        fSqrDist = fA11+2.0f*fB1+fC;
                    }
                    else
                    {
                        fT = -fB1/fA11;
                        fSqrDist = fB1*fT+fC;
                    }
                }
            }
            else  // region 3
            {
                fS = 0.0f;
                if ( fB1 >= 0.0f )
                {
                    fT = 0.0f;
                    fSqrDist = fC;
                }
                else if ( -fB1 >= fA11 )
                {
                    fT = 1.0f;
                    fSqrDist = fA11+2.0f*fB1+fC;
                }
                else
                {
                    fT = -fB1/fA11;
                    fSqrDist = fB1*fT+fC;
                }
            }
        }
        else if ( fT < 0.0f )  // region 5
        {
            fT = 0.0f;
            if ( fB0 >= 0.0f )
            {
                fS = 0.0f;
                fSqrDist = fC;
            }
            else if ( -fB0 >= fA00 )
            {
                fS = 1.0f;
                fSqrDist = fA00+2.0f*fB0+fC;
            }
            else
            {
                fS = -fB0/fA00;
                fSqrDist = fB0*fS+fC;
            }
        }
        else  // region 0
        {
            // minimum at interior point
            float fInvDet = 1.0f/fDet;
            fS *= fInvDet;
            fT *= fInvDet;
            fSqrDist = fS*(fA00*fS+fA01*fT+2.0f*fB0) +
                fT*(fA01*fS+fA11*fT+2.0f*fB1)+fC;
        }
    }
    else
    {
        float fTmp0, fTmp1, fNumer, fDenom;

        if ( fS < 0.0f )  // region 2
        {
            fTmp0 = fA01 + fB0;
            fTmp1 = fA11 + fB1;
            if ( fTmp1 > fTmp0 )
            {
                fNumer = fTmp1 - fTmp0;
                fDenom = fA00-2.0f*fA01+fA11;
                if ( fNumer >= fDenom )
                {
                    fS = 1.0f;
                    fT = 0.0f;
                    fSqrDist = fA00+2.0f*fB0+fC;
                }
                else
                {
                    fS = fNumer/fDenom;
                    fT = 1.0f - fS;
                    fSqrDist = fS*(fA00*fS+fA01*fT+2.0f*fB0) +
                        fT*(fA01*fS+fA11*fT+2.0f*fB1)+fC;
                }
            }
            else
            {
                fS = 0.0f;
                if ( fTmp1 <= 0.0f )
                {
                    fT = 1.0f;
                    fSqrDist = fA11+2.0f*fB1+fC;
                }
                else if ( fB1 >= 0.0f )
                {
                    fT = 0.0f;
                    fSqrDist = fC;
                }
                else
                {
                    fT = -fB1/fA11;
                    fSqrDist = fB1*fT+fC;
                }
            }
        }
        else if ( fT < 0.0f )  // region 6
        {
            fTmp0 = fA01 + fB1;
            fTmp1 = fA00 + fB0;
            if ( fTmp1 > fTmp0 )
            {
                fNumer = fTmp1 - fTmp0;
                fDenom = fA00-2.0f*fA01+fA11;
                if ( fNumer >= fDenom )
                {
                    fT = 1.0f;
                    fS = 0.0f;
                    fSqrDist = fA11+2.0f*fB1+fC;
                }
                else
                {
                    fT = fNumer/fDenom;
                    fS = 1.0f - fT;
                    fSqrDist = fS*(fA00*fS+fA01*fT+2.0f*fB0) +
                        fT*(fA01*fS+fA11*fT+2.0f*fB1)+fC;
                }
            }
            else
            {
                fT = 0.0f;
                if ( fTmp1 <= 0.0f )
                {
                    fS = 1.0f;
                    fSqrDist = fA00+2.0f*fB0+fC;
                }
                else if ( fB0 >= 0.0f )
                {
                    fS = 0.0f;
                    fSqrDist = fC;
                }
                else
                {
                    fS = -fB0/fA00;
                    fSqrDist = fB0*fS+fC;
                }
            }
        }
        else  // region 1
        {
            fNumer = fA11 + fB1 - fA01 - fB0;
            if ( fNumer <= 0.0f )
            {
                fS = 0.0f;
                fT = 1.0f;
                fSqrDist = fA11+2.0f*fB1+fC;
            }
            else
            {
                fDenom = fA00-2.0f*fA01+fA11;
                if ( fNumer >= fDenom )
                {
                    fS = 1.0f;
                    fT = 0.0f;
                    fSqrDist = fA00+2.0f*fB0+fC;
                }
                else
                {
                    fS = fNumer/fDenom;
                    fT = 1.0f - fS;
                    fSqrDist = fS*(fA00*fS+fA01*fT+2.0f*fB0) +
                        fT*(fA01*fS+fA11*fT+2.0f*fB1)+fC;
                }
            }
        }
    }

    if ( pfSParam )
        *pfSParam = fS;

    if ( pfTParam )
        *pfTParam = fT;

    return fabs(fSqrDist);
}

float is_ray_intersecting_segment(Vec3f *ray_origin,Vec3f *ray_dir,
								  Vec3f *segment_origin,Vec3f *segment_dir,
							      float *pfRayP=NULL,float *pfSegP=NULL)
{
    Vec3f kDiff = (*ray_origin) - (*segment_origin);
	Vec3f inv_kDiff=-kDiff;
    float fA00 = ray_dir->norm_squared();
	Vec3f inv_ray=-(*ray_dir);
    float fA01 = dot_product(&inv_ray,segment_dir);
    float fA11 = segment_dir->norm_squared();
    float fB0 = dot_product(&kDiff,ray_dir);
    float fC = kDiff.norm_squared();
    float fDet = fabs(fA00*fA11-fA01*fA01);
    float fB1, fS, fT, fSqrDist, fTmp;

    if ( fDet >= 0.0000001f )
    {
        // ray and segment are not parallel
        fB1 = dot_product(&inv_kDiff,segment_dir);
        fS = fA01*fB1-fA11*fB0;
        fT = fA01*fB0-fA00*fB1;

        if ( fS >= 0.0f )
        {
            if ( fT >= 0.0f )
            {
                if ( fT <= fDet )  // region 0
                {
                    // minimum at interior points of ray and segment
                    float fInvDet = 1.0f/fDet;
                    fS *= fInvDet;
                    fT *= fInvDet;
                    fSqrDist = fS*(fA00*fS+fA01*fT+2.0f*fB0) +
                        fT*(fA01*fS+fA11*fT+2.0f*fB1)+fC;
                }
                else  // region 1
                {
                    fT = 1.0f;
                    if ( fB0 >= -fA01 )
                    {
                        fS = 0.0f;
                        fSqrDist = fA11+2.0f*fB1+fC;
                    }
                    else
                    {
                        fTmp = fA01 + fB0;
                        fS = -fTmp/fA00;
                        fSqrDist = fTmp*fS+fA11+2.0f*fB1+fC;
                    }
                }
            }
            else  // region 5
            {
                fT = 0.0f;
                if ( fB0 >= 0.0f )
                {
                    fS = 0.0f;
                    fSqrDist = fC;
                }
                else
                {
                    fS = -fB0/fA00;
                    fSqrDist = fB0*fS+fC;
                }
            }
        }
        else
        {
            if ( fT <= 0.0f )  // region 4
            {
                if ( fB0 < 0.0f )
                {
                    fS = -fB0/fA00;
                    fT = 0.0f;
                    fSqrDist = fB0*fS+fC;
                }
                else
                {
                    fS = 0.0f;
                    if ( fB1 >= 0.0f )
                    {
                        fT = 0.0f;
                        fSqrDist = fC;
                    }
                    else if ( -fB1 >= fA11 )
                    {
                        fT = 1.0f;
                        fSqrDist = fA11+2.0f*fB1+fC;
                    }
                    else
                    {
                        fT = -fB1/fA11;
                        fSqrDist = fB1*fT+fC;
                    }
                }
            }
            else if ( fT <= fDet )  // region 3
            {
                fS = 0.0f;
                if ( fB1 >= 0.0f )
                {
                    fT = 0.0f;
                    fSqrDist = fC;
                }
                else if ( -fB1 >= fA11 )
                {
                    fT = 1.0f;
                    fSqrDist = fA11+2.0f*fB1+fC;
                }
                else
                {
                    fT = -fB1/fA11;
                    fSqrDist = fB1*fT+fC;
                }
            }
            else  // region 2
            {
                fTmp = fA01+fB0;
                if ( fTmp < 0.0f )
                {
                    fS = -fTmp/fA00;
                    fT = 1.0f;
                    fSqrDist = fTmp*fS+fA11+2.0f*fB1+fC;
                }
                else
                {
                    fS = 0.0f;
                    if ( fB1 >= 0.0f )
                    {
                        fT = 0.0f;
                        fSqrDist = fC;
                    }
                    else if ( -fB1 >= fA11 )
                    {
                        fT = 1.0f;
                        fSqrDist = fA11+2.0f*fB1+fC;
                    }
                    else
                    {
                        fT = -fB1/fA11;
                        fSqrDist = fB1*fT+fC;
                    }
                }
            }
        }
    }
    else
    {
        // ray and segment are parallel
        if ( fA01 > 0.0f )
        {
            // opposite direction vectors
            fT = 0.0f;
            if ( fB0 >= 0.0f )
            {
                fS = 0.0f;
                fSqrDist = fC;
            }
            else
            {
                fS = -fB0/fA00;
                fSqrDist = fB0*fS+fC;
            }
        }
        else
        {
            // same direction vectors
            fB1 = dot_product(&inv_kDiff,segment_dir);
            fT = 1.0f;
            fTmp = fA01+fB0;
            if ( fTmp >= 0.0f )
            {
                fS = 0.0f;
                fSqrDist = fA11+2.0f*fB1+fC;
            }
            else
            {
                fS = -fTmp/fA00;
                fSqrDist = fTmp*fS+fA11+2.0f*fB1+fC;
            }
        }
    }

    if ( pfRayP )
        *pfRayP = fS;

    if ( pfSegP )
        *pfSegP = fT;

    return fabs(fSqrDist);
}

float get_distance_ray_triangle(Vec3f *ray_origin,Vec3f *ray_dir,
								Vec3f *pos1,Vec3f *pos2,Vec3f *pos3,
							    float *pfRayP=NULL, float *pfTriP0=NULL, float *pfTriP1=NULL)
{
    Vec3f kDiff = *pos1 - *ray_origin;
    float fA00 = ray_dir->norm_squared();
	Vec3f inv_ray=-(*ray_dir);
    float fA01 = dot_product(&inv_ray,pos2);
    float fA02 = dot_product(&inv_ray,pos3);
    float fA11 = pos2->norm_squared();
    float fA12 = dot_product(pos2,pos3);
    float fA22 = dot_product(pos3,pos3);
	Vec3f inv_kDiff=-kDiff;
    float fB0  = dot_product(&inv_kDiff,ray_dir);
    float fB1  = dot_product(&kDiff,pos2);
    float fB2  = dot_product(&kDiff,pos3);
    float fCof00 = fA11*fA22-fA12*fA12;
    float fCof01 = fA02*fA12-fA01*fA22;
    float fCof02 = fA01*fA12-fA02*fA11;
    float fDet = fA00*fCof00+fA01*fCof01+fA02*fCof02;

    Vec3f kTriSeg_origin;
	Vec3f kTriSeg_dir;
    Vec3f kPt;
    float fSqrDist, fSqrDist0, fR, fS, fT, fR0, fS0, fT0;

    if ( fabs(fDet) >= 0.0000001f )
    {
        float fCof11 = fA00*fA22-fA02*fA02;
        float fCof12 = fA02*fA01-fA00*fA12;
        float fCof22 = fA00*fA11-fA01*fA01;
        float fInvDet = 1.0f/fDet;
        float fRhs0 = -fB0*fInvDet;
        float fRhs1 = -fB1*fInvDet;
        float fRhs2 = -fB2*fInvDet;

        fR = fCof00*fRhs0+fCof01*fRhs1+fCof02*fRhs2;
        fS = fCof01*fRhs0+fCof11*fRhs1+fCof12*fRhs2;
        fT = fCof02*fRhs0+fCof12*fRhs1+fCof22*fRhs2;

        if ( fR <= 0.0f )
        {
            if ( fS+fT <= 1.0f )
            {
                if ( fS < 0.0f )
                {
                    if ( fT < 0.0f )  // region 4m
                    {
                        // min on face s=0 or t=0 or r=0
                        kTriSeg_origin = *pos1;
                        kTriSeg_dir = *pos3;
                        fSqrDist = is_ray_intersecting_segment(ray_origin,ray_dir,&kTriSeg_origin,&kTriSeg_dir,&fR,&fT);
                        fS = 0.0f;
                        kTriSeg_origin = *pos1;
                        kTriSeg_dir = *pos2;
                        fSqrDist0 = is_ray_intersecting_segment(ray_origin,ray_dir,&kTriSeg_origin,&kTriSeg_dir,&fR0,&fS0);
                        fT0 = 0.0f;
                        if ( fSqrDist0 < fSqrDist )
                        {
                            fSqrDist = fSqrDist0;
                            fR = fR0;
                            fS = fS0;
                            fT = fT0;
                        }
                        fSqrDist0 = get_distance_from_point_to_tri(ray_origin,pos1,pos2,pos3,&fS0,
                            &fT0);
                        fR0 = 0.0f;
                        if ( fSqrDist0 < fSqrDist )
                        {
                            fSqrDist = fSqrDist0;
                            fR = fR0;
                            fS = fS0;
                            fT = fT0;
                        }
                    }
                    else  // region 3m
                    {
                        // min on face s=0 or r=0
                        kTriSeg_origin = *pos1;
                        kTriSeg_dir = *pos3;
                        fSqrDist = is_ray_intersecting_segment(ray_origin,ray_dir,&kTriSeg_origin,&kTriSeg_dir,&fR,&fT);
                        fS = 0.0f;
                        fSqrDist0 = get_distance_from_point_to_tri(ray_origin,pos1,pos2,pos3,&fS0,
                            &fT0);
                        fR0 = 0.0f;
                        if ( fSqrDist0 < fSqrDist )
                        {
                            fSqrDist = fSqrDist0;
                            fR = fR0;
                            fS = fS0;
                            fT = fT0;
                        }
                    }
                }
                else if ( fT < 0.0f )  // region 5m
                {
                    // min on face t=0 or r=0
                    kTriSeg_origin = *pos1;
                    kTriSeg_dir = *pos2;
                    fSqrDist = is_ray_intersecting_segment(ray_origin,ray_dir,&kTriSeg_origin,&kTriSeg_dir,&fR,&fS);
                    fT = 0.0f;
                    fSqrDist0 = get_distance_from_point_to_tri(ray_origin,pos1,pos2,pos3,&fS0,&fT0);
                    fR0 = 0.0f;
                    if ( fSqrDist0 < fSqrDist )
                    {
                        fSqrDist = fSqrDist0;
                        fR = fR0;
                        fS = fS0;
                        fT = fT0;
                    }
                }
                else  // region 0m
                {
                    // min on face r=0
                    fSqrDist = get_distance_from_point_to_tri(ray_origin,pos1,pos2,pos3,&fS,&fT);
                    fR = 0.0f;
                }
            }
            else
            {
                if ( fS < 0.0f )  // region 2m
                {
                    // min on face s=0 or s+t=1 or r=0
                    kTriSeg_origin = *pos1;
                    kTriSeg_dir = *pos3;
                    fSqrDist = is_ray_intersecting_segment(ray_origin,ray_dir,&kTriSeg_origin,&kTriSeg_dir,&fR,&fT);
                    fS = 0.0f;
                    kTriSeg_origin = (*pos1)+(*pos2);
                    kTriSeg_dir = (*pos3)-(*pos2);
                    fSqrDist0 = is_ray_intersecting_segment(ray_origin,ray_dir,&kTriSeg_origin,&kTriSeg_dir,&fR0,&fT0);
                    fS0 = 1.0f-fT0;
                    if ( fSqrDist0 < fSqrDist )
                    {
                        fSqrDist = fSqrDist0;
                        fR = fR0;
                        fS = fS0;
                        fT = fT0;
                    }
                    fSqrDist0 = get_distance_from_point_to_tri(ray_origin,pos1,pos2,pos3,&fS0,&fT0);
                    fR0 = 0.0f;
                    if ( fSqrDist0 < fSqrDist )
                    {
                        fSqrDist = fSqrDist0;
                        fR = fR0;
                        fS = fS0;
                        fT = fT0;
                    }
                }
                else if ( fT < 0.0f )  // region 6m
                {
                    // min on face t=0 or s+t=1 or r=0
                    kTriSeg_origin = *pos1;
                    kTriSeg_dir = *pos2;
                    fSqrDist = is_ray_intersecting_segment(ray_origin,ray_dir,&kTriSeg_origin,&kTriSeg_dir,&fR,&fS);
                    fT = 0.0f;
                    kTriSeg_origin = (*pos1)+(*pos2);
                    kTriSeg_dir = (*pos3)-(*pos2);
                    fSqrDist0 = is_ray_intersecting_segment(ray_origin,ray_dir,&kTriSeg_origin,&kTriSeg_dir,&fR0,&fT0);
                    fS0 = 1.0f-fT0;
                    if ( fSqrDist0 < fSqrDist )
                    {
                        fSqrDist = fSqrDist0;
                        fR = fR0;
                        fS = fS0;
                        fT = fT0;
                    }
                    fSqrDist0 = get_distance_from_point_to_tri(ray_origin,pos1,pos2,pos3,&fS0,&fT0);
                    fR0 = 0.0f;
                    if ( fSqrDist0 < fSqrDist )
                    {
                        fSqrDist = fSqrDist0;
                        fR = fR0;
                        fS = fS0;
                        fT = fT0;
                    }
                }
                else  // region 1m
                {
                    // min on face s+t=1 or r=0
                    kTriSeg_origin = (*pos1)+(*pos2);
                    kTriSeg_dir = (*pos3)-(*pos2);
                    fSqrDist = is_ray_intersecting_segment(ray_origin,ray_dir,&kTriSeg_origin,&kTriSeg_dir,&fR,&fT);
                    fS = 1.0f-fT;
                    fSqrDist0 = get_distance_from_point_to_tri(ray_origin,pos1,pos2,pos3,&fS0,&fT0);
                    fR0 = 0.0f;
                    if ( fSqrDist0 < fSqrDist )
                    {
                        fSqrDist = fSqrDist0;
                        fR = fR0;
                        fS = fS0;
                        fT = fT0;
                    }
                }
            }
        }
        else  // fR > 0
        {
            if ( fS+fT <= 1.0f )
            {
                if ( fS < 0.0f )
                {
                    if ( fT < 0.0f )  // region 4p
                    {
                        // min on face s=0 or t=0
                        kTriSeg_origin = *pos1;
                        kTriSeg_dir = *pos3;
                        fSqrDist = is_ray_intersecting_segment(ray_origin,ray_dir,&kTriSeg_origin,&kTriSeg_dir,NULL,&fT);
                        fS = 0.0f;
                        kTriSeg_origin = *pos1;
                        kTriSeg_dir = *pos2;
                        fSqrDist0 = is_ray_intersecting_segment(ray_origin,ray_dir,&kTriSeg_origin,&kTriSeg_dir,NULL,&fS0);
                        fT0 = 0.0f;
                        if ( fSqrDist0 < fSqrDist )
                        {
                            fSqrDist = fSqrDist0;
                            fS = fS0;
                            fT = fT0;
                        }
                    }
                    else  // region 3p
                    {
                        // min on face s=0
                        kTriSeg_origin = *pos1;
                        kTriSeg_dir = *pos3;
                        fSqrDist = is_ray_intersecting_segment(ray_origin,ray_dir,&kTriSeg_origin,&kTriSeg_dir,NULL,&fT);
                        fS = 0.0f;
                    }
                }
                else if ( fT < 0.0f )  // region 5p
                {
                    // min on face t=0
                    kTriSeg_origin = *pos1;
                    kTriSeg_dir = *pos2;
                    fSqrDist = is_ray_intersecting_segment(ray_origin,ray_dir,&kTriSeg_origin,&kTriSeg_dir,NULL,&fS);
                    fT = 0.0f;
                }
                else  // region 0p
                {
                    // ray intersects triangle
                    fSqrDist = 0.0f;
                }
            }
            else
            {
                if ( fS < 0.0f )  // region 2p
                {
                    // min on face s=0 or s+t=1
                    kTriSeg_origin = *pos1;
                    kTriSeg_dir = *pos3;
                    fSqrDist = is_ray_intersecting_segment(ray_origin,ray_dir,&kTriSeg_origin,&kTriSeg_dir,NULL,&fT);
                    fS = 0.0f;
                    kTriSeg_origin = (*pos1)+(*pos2);
                    kTriSeg_dir = (*pos3)-(*pos2);
                    fSqrDist0 = is_ray_intersecting_segment(ray_origin,ray_dir,&kTriSeg_origin,&kTriSeg_dir,NULL,&fT0);
                    fS0 = 1.0f-fT0;
                    if ( fSqrDist0 < fSqrDist )
                    {
                        fSqrDist = fSqrDist0;
                        fS = fS0;
                        fT = fT0;
                    }
                }
                else if ( fT < 0.0f )  // region 6p
                {
                    // min on face t=0 or s+t=1
                    kTriSeg_origin = *pos1;
                    kTriSeg_dir = *pos2;
                    fSqrDist = is_ray_intersecting_segment(ray_origin,ray_dir,&kTriSeg_origin,&kTriSeg_dir,NULL,&fS);
                    fT = 0.0f;
                    kTriSeg_origin = (*pos1)+(*pos2);
                    kTriSeg_dir = (*pos3)-(*pos2);
                    fSqrDist0 = is_ray_intersecting_segment(ray_origin,ray_dir,&kTriSeg_origin,&kTriSeg_dir,NULL,&fT0);
                    fS0 = 1.0f-fT0;
                    if ( fSqrDist0 < fSqrDist )
                    {
                        fSqrDist = fSqrDist0;
                        fS = fS0;
                        fT = fT0;
                    }
                }
                else  // region 1p
                {
                    // min on face s+t=1
                    kTriSeg_origin = (*pos1)+(*pos2);
                    kTriSeg_dir = (*pos3)-(*pos2);
                    fSqrDist = is_ray_intersecting_segment(ray_origin,ray_dir,&kTriSeg_origin,&kTriSeg_dir,NULL,&fT);
                    fS = 1.0f-fT;
                }
            }
        }
    }
    else
    {
        // ray and triangle are parallel
        kTriSeg_origin = *pos1;
        kTriSeg_dir = *pos2;
        fSqrDist = is_ray_intersecting_segment(ray_origin,ray_dir,&kTriSeg_origin,&kTriSeg_dir,&fR,&fS);
        fT = 0.0f;

        kTriSeg_dir = *pos3;
        fSqrDist0 = is_ray_intersecting_segment(ray_origin,ray_dir,&kTriSeg_origin,&kTriSeg_dir,&fR0,&fT0);
        fS0 = 0.0f;
        if ( fSqrDist0 < fSqrDist )
        {
            fSqrDist = fSqrDist0;
            fR = fR0;
            fS = fS0;
            fT = fT0;
        }

        kTriSeg_origin = *pos1 + *pos2;
        kTriSeg_dir = *pos3 - *pos2;
        fSqrDist0 = is_ray_intersecting_segment(ray_origin,ray_dir,&kTriSeg_origin,&kTriSeg_dir,&fR0,&fT0);
        fS0 = 1.0f-fT0;
        if ( fSqrDist0 < fSqrDist )
        {
            fSqrDist = fSqrDist0;
            fR = fR0;
            fS = fS0;
            fT = fT0;
        }

        fSqrDist0 = get_distance_from_point_to_tri(ray_origin,pos1,pos2,pos3,&fS0,&fT0);
        fR0 = 0.0f;
        if ( fSqrDist0 < fSqrDist )
        {
            fSqrDist = fSqrDist0;
            fR = fR0;
            fS = fS0;
            fT = fT0;
        }
    }

    if ( pfRayP )
        *pfRayP = fR;

    if ( pfTriP0 )
        *pfTriP0 = fS;

    if ( pfTriP1 )
        *pfTriP1 = fT;

    return fSqrDist;
}

bool is_ray_intersecting_tri(Vec3f *ray_origin,Vec3f *ray_dir,
							 Vec3f *pos1,Vec3f *pos2,Vec3f *pos3,
							 Vec3f *point)
{
    float	fRayP;

	Vec3f	pt1=*pos1;
	Vec3f	pt2=*pos2; pt2=pt2-pt1;
	Vec3f	pt3=*pos3; pt3=pt3-pt1;

    if (get_distance_ray_triangle(ray_origin,ray_dir,&pt1,&pt2,&pt3,&fRayP)<=0.0000001f)
    {
		if (point)
		{
			*point=*ray_origin+(*ray_dir)*fRayP;
		}
        return true;
    }

    return false;
}

/* Triangle/triangle intersection test routine,
 * by Tomas Moller, 1997.
 * See article "A Fast Triangle-Triangle Intersection Test",
 * Journal of Graphics Tools, 2(2), 1997
 *
 * Updated June 1999: removed the divisions -- a little faster now!
 * Updated October 1999: added {} to CROSS and SUB macros 
 * Adapted by Diogo de Andrade on April 2000
 *
 *
 */

/* sort so that a<=b */
#define SORT(a,b)    \
		if (a>b)     \
		{            \
			float c; \
			c=a;     \
			a=b;     \
			b=c;     \
		}


/* this edge to edge test is based on Franlin Antonio's gem:
   "Faster Line Segment Intersection", in Graphics Gems III,
   pp. 199-202 */
#define EDGE_EDGE_TEST(V0,U0,U1)                        \
	Bx=U0[i0]-U1[i0];                                   \
	By=U0[i1]-U1[i1];                                   \
	Cx=V0[i0]-U0[i0];                                   \
	Cy=V0[i1]-U0[i1];                                   \
	f=Ay*Bx-Ax*By;                                      \
	d=By*Cx-Bx*Cy;                                      \
	if((f>0 && d>=0 && d<=f) || (f<0 && d<=0 && d>=f))  \
	{                                                   \
		e=Ax*Cy-Ay*Cx;                                  \
		if(f>0)                                         \
		{                                               \
			if(e>=0 && e<=f) return true;             \
		}                                               \
		else                                            \
		{                                               \
			if(e<=0 && e>=f) return true;             \
		}                                               \
	}

#define EDGE_AGAINST_TRI_EDGES(V0,V1,U0,U1,U2)   \
{                                                \
	float Ax,Ay,Bx,By,Cx,Cy,e,d,f;               \
	Ax=V1[i0]-V0[i0];                            \
	Ay=V1[i1]-V0[i1];                            \
	/* test edge U0,U1 against V0,V1 */          \
	EDGE_EDGE_TEST(V0,U0,U1);                    \
	/* test edge U1,U2 against V0,V1 */          \
	EDGE_EDGE_TEST(V0,U1,U2);                    \
	/* test edge U2,U1 against V0,V1 */          \
	EDGE_EDGE_TEST(V0,U2,U0);                    \
}

#define POINT_IN_TRI(V0,U0,U1,U2)             \
{                                             \
	float a,b,c,d0,d1,d2;                     \
											  \
	/* is T1 completly inside T2? */          \
	/* check if V0 is inside tri(U0,U1,U2) */ \
	a=U1[i1]-U0[i1];                          \
	b=-(U1[i0]-U0[i0]);                       \
	c=-a*U0[i0]-b*U0[i1];                     \
	d0=a*V0[i0]+b*V0[i1]+c;                   \
											  \
	a=U2[i1]-U1[i1];                          \
	b=-(U2[i0]-U1[i0]);                       \
	c=-a*U1[i0]-b*U1[i1];                     \
	d1=a*V0[i0]+b*V0[i1]+c;                   \
									  		  \
	a=U0[i1]-U2[i1];                          \
	b=-(U0[i0]-U2[i0]);                       \
	c=-a*U2[i0]-b*U2[i1];                     \
	d2=a*V0[i0]+b*V0[i1]+c;                   \
	if (d0*d1>0.0f)                           \
	{                                         \
		if (d0*d2>0.0f) return true;        \
	}                                         \
}

bool coplanar_tri_tri(Vec3f *n,
					  Vec3f *pta1,Vec3f *pta2,Vec3f *pta3,
					  Vec3f *ptb1,Vec3f *ptb2,Vec3f *ptb3)
{
	Vec3f	a;
	short	i0,i1;
	float	V0[3],V1[3],V2[3];
	float	U0[3],U1[3],U2[3];

	V0[0]=pta1->x; V0[1]=pta1->y; V0[2]=pta1->z; 
	V1[0]=pta2->x; V1[1]=pta2->y; V1[2]=pta2->z; 
	V2[0]=pta3->x; V2[1]=pta3->y; V2[2]=pta3->z; 
	U0[0]=ptb1->x; U0[1]=ptb1->y; U0[2]=ptb1->z; 
	U1[0]=ptb2->x; U1[1]=ptb2->y; U1[2]=ptb2->z; 
	U2[0]=ptb3->x; U2[1]=ptb3->y; U2[2]=ptb3->z; 

	/* first project onto an axis-aligned plane, that maximizes the area */
	/* of the triangles, compute indices: i0,i1. */
	a.set(fabs(n->x),fabs(n->y),fabs(n->z));
	if (a.x>a.y)
	{
		if (a.x>a.z)
		{
			i0=1;      /* A[0] is greatest */
			i1=2;
		}
		else
		{
			i0=0;      /* A[2] is greatest */
			i1=1;
		}
	}
	else   /* A[0]<=A[1] */
	{
		if (a.z>a.y)
		{
			i0=0;      /* A[2] is greatest */
			i1=1;
		}
		else
		{
			i0=0;      /* A[1] is greatest */
			i1=2;
		}
	}

	/* test all edges of triangle 1 against the edges of triangle 2 */
	EDGE_AGAINST_TRI_EDGES(V0,V1,U0,U1,U2);
	EDGE_AGAINST_TRI_EDGES(V1,V2,U0,U1,U2);
	EDGE_AGAINST_TRI_EDGES(V2,V0,U0,U1,U2);

	/* finally, test if tri1 is totally contained in tri2 or vice versa */
	POINT_IN_TRI(V0,U0,U1,U2);
	POINT_IN_TRI(U0,V0,V1,V2);

	return false;
}

#define NEWCOMPUTE_INTERVALS(VV0,VV1,VV2,D0,D1,D2,D0D1,D0D2,A,B,C,X0,X1) \
{ \
        if (D0D1>0.0f) \
        { \
                /* here we know that D0D2<=0.0 */ \
            /* that is D0, D1 are on the same side, D2 on the other or on the plane */ \
                A=VV2; B=(VV0-VV2)*D2; C=(VV1-VV2)*D2; X0=D2-D0; X1=D2-D1; \
        } \
        else if (D0D2>0.0f)\
        { \
                /* here we know that d0d1<=0.0 */ \
            A=VV1; B=(VV0-VV1)*D1; C=(VV2-VV1)*D1; X0=D1-D0; X1=D1-D2; \
        } \
        else if (D1*D2>0.0f || D0!=0.0f) \
        { \
                /* here we know that d0d1<=0.0 or that D0!=0.0 */ \
                A=VV0; B=(VV1-VV0)*D0; C=(VV2-VV0)*D0; X0=D0-D1; X1=D0-D2; \
        } \
        else if (D1!=0.0f) \
        { \
                A=VV1; B=(VV0-VV1)*D1; C=(VV2-VV1)*D1; X0=D1-D0; X1=D1-D2; \
        } \
        else if (D2!=0.0f) \
        { \
                A=VV2; B=(VV0-VV2)*D2; C=(VV1-VV2)*D2; X0=D2-D0; X1=D2-D1; \
        } \
        else \
        { \
                /* triangles are coplanar */ \
                return coplanar_tri_tri(&n1,pta1,pta2,pta3,ptb1,ptb2,ptb3); \
        } \
}

bool are_triangles_intersecting(Vec3f *pta1,Vec3f *pta2,Vec3f *pta3,
								Vec3f *ptb1,Vec3f *ptb2,Vec3f *ptb3)
{
	Vec3f e1,e2;
	Vec3f n1,n2;
	float d1,d2;
	float du0,du1,du2,dv0,dv1,dv2;
	Vec3f dir;
	float isect1[2], isect2[2];
	float du0du1,du0du2,dv0dv1,dv0dv2;
	float vp0,vp1,vp2;
	float up0,up1,up2;
	float aa,bb,cc;

	/* compute plane equation of triangle(V0,V1,V2) */
	e1=(*pta2)-(*pta1);
	e2=(*pta3)-(*pta1);
	n1=cross_product(e1,e2);
	Vec3f	inv_n1=-n1;
	d1=dot_product(&inv_n1,pta1);
	/* plane equation 1: N1.X+d1=0 */

	/* put U0,U1,U2 into plane equation 1 to compute signed distances to the plane*/
	du0=dot_product(&n1,ptb1)+d1;
	du1=dot_product(&n1,ptb2)+d1;
	du2=dot_product(&n1,ptb3)+d1;

	/* coplanarity robustness check */
	if (fabs(du0)<0.000001f) du0=0.0f;
	if (fabs(du1)<0.000001f) du1=0.0f;
	if (fabs(du2)<0.000001f) du2=0.0f;

	du0du1=du0*du1;
	du0du2=du0*du2;

	if ((du0du1>0.0f) && (du0du2>0.0f))	/* same sign on all of them + not equal 0 ? */
		return false;					/* no intersection occurs */

	/* compute plane of triangle (U0,U1,U2) */
	e1=(*ptb2)-(*ptb1);
	e2=(*ptb3)-(*ptb1);
	n2=cross_product(e1,e2);
	Vec3f inv_n2=-n2;
	d2=dot_product(&inv_n2,ptb1);
	/* plane equation 2: N2.X+d2=0 */

	/* put V0,V1,V2 into plane equation 2 */
	dv0=dot_product(&n2,pta1)+d2;
	dv1=dot_product(&n2,pta2)+d2;
	dv2=dot_product(&n2,pta3)+d2;

	if (fabs(dv0)<0.000001f) dv0=0.0f;
	if (fabs(dv1)<0.000001f) dv1=0.0f;
	if (fabs(dv2)<0.000001f) dv2=0.0f;

	dv0dv1=dv0*dv1;
	dv0dv2=dv0*dv2;

	if ((dv0dv1>0.0f) && (dv0dv2>0.0f))	/* same sign on all of them + not equal 0 ? */
		return false;					/* no intersection occurs */

	/* compute direction of intersection line */
	dir=cross_product(n1,n2);

	/* compute and index to the largest component of D */
	aa=fabs(dir.x);
	bb=fabs(dir.y);
	cc=fabs(dir.z);
	if (aa>bb)
	{
		if (aa>cc)
		{
		  vp0=pta1->x;
		  vp1=pta2->x;
		  vp2=pta3->x;

		  up0=ptb1->x;
		  up1=ptb2->x;
		  up2=ptb3->x;
		}
		else
		{
		  vp0=pta1->z;
		  vp1=pta2->z;
		  vp2=pta3->z;

		  up0=ptb1->z;
		  up1=ptb1->z;
		  up2=ptb1->z;
		}
	}
	else
	{
		if (bb>cc)
		{
		  vp0=pta1->y;
		  vp1=pta2->y;
		  vp2=pta3->y;

		  up0=ptb1->y;
		  up1=ptb2->y;
		  up2=ptb3->y;
		}
		else
		{
		  vp0=pta1->z;
		  vp1=pta2->z;
		  vp2=pta3->z;

		  up0=ptb1->z;
		  up1=ptb1->z;
		  up2=ptb1->z;
		}
	}

	/* compute interval for triangle 1 */
	float a,b,c,x0,x1;
	NEWCOMPUTE_INTERVALS(vp0,vp1,vp2,dv0,dv1,dv2,dv0dv1,dv0dv2,a,b,c,x0,x1);

	/* compute interval for triangle 2 */
	float d,e,f,y0,y1;
	NEWCOMPUTE_INTERVALS(up0,up1,up2,du0,du1,du2,du0du1,du0du2,d,e,f,y0,y1);

	float xx,yy,xxyy,tmp;
	xx=x0*x1;
	yy=y0*y1;
	xxyy=xx*yy;

	tmp=a*xxyy;
	isect1[0]=tmp+b*x1*yy;
	isect1[1]=tmp+c*x0*yy;

	tmp=d*xxyy;
	isect2[0]=tmp+e*xx*y1;
	isect2[1]=tmp+f*xx*y0;

	SORT(isect1[0],isect1[1]);
	SORT(isect2[0],isect2[1]);

	if (isect1[1]<isect2[0] || isect2[1]<isect1[0]) return false;
	
	return true;
}
