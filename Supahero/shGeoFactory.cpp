// shGeoFactory.cpp: implementation of the shGeoFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "shGeoFactory.h"
#include "ldCompositeMesh.h"
#include "../LudiEngine/Headers/ldGeoFilter.h"
#include "../LudiEngine/Headers/ldGfx.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

shGeoFactory::shGeoFactory()
{

}

shGeoFactory::~shGeoFactory()
{

}

ldMesh* shGeoFactory::game_char(float height,long text_head,long text_body)
{
	ldGeoFilter	filter;
	Vec3f		vec1;
	Quaternion	ori1;

	ldCompositeMesh *stone=new ldCompositeMesh;
	
	// Build head (mesh & material)
	ldMesh *head_mesh=game_char_head(height*0.15f);
	filter.scale(head_mesh,4.0f,4.0f,4.0f);
	filter.move(head_mesh,0.0f,height*0.925f*4.0f,0.0f);
	head_mesh->close();

	ldMaterial *head_material=GFX->new_material();
	char buffer[128];
	sprintf((char*)&buffer,"textures/head%02li.png",text_head);
	head_material->set_texture(0,get_texture((char*)&buffer));
	head_material->set_diffuse_color(1.0f,1.0f,1.0f,1.0f);
	head_material->set_color_blend_op(0,LDBLEND_MODULATE2X,LDBLENDARG_TEXTURE,LDBLENDARG_DIFFUSE);
	head_material->set_alpha_blend_op(0,LDBLEND_DISABLE,LDBLENDARG_DIFFUSE,LDBLENDARG_DIFFUSE);
	// Build body (mesh & material)
	ldMesh *body_mesh=game_char_body(height*0.85);
	filter.scale(body_mesh,4.0f,4.0f,4.0f);
	body_mesh->close();
	ldMaterial *body_material=GFX->new_material();
	sprintf((char*)&buffer,"textures/body%02li.png",text_body);
	body_material->set_texture(0,get_texture((char*)&buffer));
	body_material->set_diffuse_color(1.0f,1.0f,1.0f,1.0f);
	body_material->set_color_blend_op(0,LDBLEND_MODULATE2X,LDBLENDARG_TEXTURE,LDBLENDARG_DIFFUSE);
	body_material->set_alpha_blend_op(0,LDBLEND_DISABLE,LDBLENDARG_DIFFUSE,LDBLENDARG_DIFFUSE);
	// Build arms (mesh)
	// Upper left arm
/*	ldMesh *upper_left_arm_mesh=stone_arm(0.15*scale);
	upper_left_arm_mesh->close();
	// Lower left arm
	ldMesh *lower_left_arm_mesh=stone_arm(0.10*scale);
	lower_left_arm_mesh->close();
	// Upper right arm
	ldMesh *upper_right_arm_mesh=stone_arm(0.15*scale);
	upper_right_arm_mesh->close();
	// Lower right arm
	ldMesh *lower_right_arm_mesh=stone_arm(0.10*scale);
	lower_right_arm_mesh->close();
*/
	// Composite everything
	stone->add_mesh("head",head_mesh); stone->add_material(head_material);
	stone->add_mesh("body",body_mesh); stone->add_material(body_material);
//	stone->add_mesh("upper_left_arm",upper_left_arm_mesh); stone->add_material(body_material);
//	stone->add_mesh("lower_left_arm",lower_left_arm_mesh); stone->add_material(body_material);
//	stone->add_mesh("upper_right_arm",upper_right_arm_mesh); stone->add_material(body_material);
//	stone->add_mesh("lower_right_arm",lower_right_arm_mesh); stone->add_material(body_material);

	return stone;
}

ldMesh*	shGeoFactory::game_char_head(float size)
{
	set_collidable(true);
	set_dynamic(false);
	set_resolution(0.5f,0.5f,0.5f);
	set_size(-size/2.0f,-size/2.0f,-size/2.0f,size/2.0f,size/1.5f,size/2.0f);
	set_texturing(1);

	ldMesh *spheroid=ldGeoFactory::spheroid();
	ldGeoFilter filter;
	filter.convert_to_indexed(spheroid);

	return spheroid;
}

ldMesh*	shGeoFactory::game_char_body(float size)
{
	set_collidable(true);
	set_dynamic(false);
	set_resolution(0.01f,0.01f,0.01f);
	set_size(-size/4.0f,0.0f,-size/10.0f,size/4.0f,size,size/10.0f);
	set_texturing(1);

	ldMesh *prism=ldGeoFactory::prism(4,true);
	ldGeoFilter filter;
	filter.convert_to_indexed(prism);

	return prism;
}

ldMesh *shGeoFactory::landscape(float size,float bumps)
{
	set_collidable(true);
	set_dynamic(false);
	set_resolution(0.01f,0.01f,0.01f);
	set_size(-size/2.0f,0.0f,-size/2.0f,size/2.0f,30.0f,size/2.0f);
	set_texturing(1);

	ldGeoFilter filter;
	ldMesh *landscape=xz_quad();
	filter.convert_to_indexed(landscape);
	filter.scale_texture_coords(landscape,0,5.0f,5.0f);
	filter.scale_along_normals(landscape,0.0f,bumps);
	landscape->close();

	return landscape;
}

ldMesh *shGeoFactory::enemy(long i)
{
	ldCompositeMesh		*enm;
	ldGeoFilter			filter;
	ldMaterial			*enemy_material;

	set_collidable(true);
	set_dynamic(false);
	set_resolution(0.01f,0.01f,0.01f);
	set_size(-2.5f,0.0f,-2.5f,2.5f,5.0f,2.5f);
	set_texturing(0);

	enm=new ldCompositeMesh;

	switch (i)
	{
		case 0:
			{
				set_size(-2.5f,0.0f,-2.5f,2.5f,10.0f,2.5f);
				ldMesh *pyr=pyramid(5,true);
				filter.convert_to_indexed(pyr);
				pyr->close();

				enemy_material=GFX->new_material();
				enemy_material->set_diffuse_color(0.0f,0.0f,1.0f,1.0f);
				enemy_material->set_color_blend_op(0,LDBLEND_ARG2,LDBLENDARG_TEXTURE,LDBLENDARG_DIFFUSE);
				enemy_material->set_alpha_blend_op(0,LDBLEND_DISABLE,LDBLENDARG_DIFFUSE,LDBLENDARG_DIFFUSE);
				enm->add_mesh("enemy",pyr); enm->add_material(enemy_material);
			}
			break;
		case 1:
			{
				set_size(-2.5f,0.0f,-2.5f,2.5f,5.0f,2.5f);
				ldMesh *pyr1=pyramid(3,true);
				ldMesh *pyr2=pyramid(3,true);
				ldMesh *pyr3=pyramid(3,true);
				ldMesh *pyr4=pyramid(3,true);
				ldMesh *pyr5=pyramid(3,true);
				ldMesh *pyr6=pyramid(3,true);
				filter.convert_to_indexed(pyr1);
				filter.convert_to_indexed(pyr2);
				filter.convert_to_indexed(pyr3);
				filter.convert_to_indexed(pyr4);
				filter.convert_to_indexed(pyr5);
				filter.convert_to_indexed(pyr6);
				filter.rotate(pyr2,0.0f,0.0f,1.0f,180.0f); filter.move(pyr2,0.0f,10.0f,0.0f);
				filter.rotate(pyr3,0.0f,0.0f,1.0f,90.0f); filter.move(pyr3,5.0f,5.0f,0.0f);
				filter.rotate(pyr4,0.0f,0.0f,1.0f,-90.0f); filter.move(pyr4,-5.0f,5.0f,0.0f);
				filter.rotate(pyr5,1.0f,0.0f,0.0f,90.0f); filter.move(pyr5,0.0f,5.0f,-5.0f);
				filter.rotate(pyr6,1.0f,0.0f,0.0f,-90.0f); filter.move(pyr6,0.0f,5.0f,5.0f);
				pyr1->close();
				pyr2->close();
				pyr3->close();
				pyr4->close();
				pyr5->close();
				pyr6->close();

				enemy_material=GFX->new_material();
				enemy_material->set_diffuse_color(0.0f,1.0f,0.0f,1.0f);
				enemy_material->set_color_blend_op(0,LDBLEND_ARG2,LDBLENDARG_TEXTURE,LDBLENDARG_DIFFUSE);
				enemy_material->set_alpha_blend_op(0,LDBLEND_DISABLE,LDBLENDARG_DIFFUSE,LDBLENDARG_DIFFUSE);
				
				enm->add_mesh("enemy",pyr1); enm->add_material(enemy_material);
				enm->add_mesh("enemy",pyr2); enm->add_material(enemy_material);
				enm->add_mesh("enemy",pyr3); enm->add_material(enemy_material);
				enm->add_mesh("enemy",pyr4); enm->add_material(enemy_material);
				enm->add_mesh("enemy",pyr5); enm->add_material(enemy_material);
				enm->add_mesh("enemy",pyr6); enm->add_material(enemy_material);
			}
			break;
		case 2:
			{
				set_resolution(0.02f,0.02f,0.02f);
				set_size(-2.5f,0.0f,-2.5f,2.5f,5.0f,2.5f);
				ldMesh *sph=spheroid();
				filter.convert_to_indexed(sph);
				filter.scale_along_normals(sph,1.0f,6.0f);
				filter.scale(sph,1.0f,0.6f,0.6f);
				sph->close();

				enemy_material=GFX->new_material();
				enemy_material->set_diffuse_color(1.0f,0.0f,0.0f,0.5f);
				enemy_material->set_color_blend_op(0,LDBLEND_ARG2,LDBLENDARG_TEXTURE,LDBLENDARG_DIFFUSE);
				enemy_material->set_alpha_blend_op(0,LDBLEND_DISABLE,LDBLENDARG_DIFFUSE,LDBLENDARG_DIFFUSE);
				enm->add_mesh("enemy",sph); enm->add_material(enemy_material);
			}
			break;
		case 3:
			{
				set_size(-2.0f,3.0f,-2.0f,2.0f,7.0f,2.0f);
				ldMesh *sph=spheroid();
				filter.convert_to_indexed(sph);
				sph->close();

				set_size(-4.0f,0.0f,-4.0f,4.0f,10.0f,4.0f);
				ldMesh *cyl=prism(16,true);
				filter.scale_along_normals(cyl,1.5f,1.5f);
				cyl->close();

				enemy_material=GFX->new_material();
				enemy_material->set_diffuse_color(1.0f,0.0f,0.0f,1.0f);
				enemy_material->set_color_blend_op(0,LDBLEND_ARG2,LDBLENDARG_TEXTURE,LDBLENDARG_DIFFUSE);
				enemy_material->set_alpha_blend_op(0,LDBLEND_DISABLE,LDBLENDARG_DIFFUSE,LDBLENDARG_DIFFUSE);
				enm->add_mesh("enemy",sph); enm->add_material(enemy_material);
				enemy_material=GFX->new_material();
				enemy_material->set_diffuse_color(0.0f,0.0f,0.0f,1.0f);
				enemy_material->set_color_blend_op(0,LDBLEND_ARG2,LDBLENDARG_TEXTURE,LDBLENDARG_DIFFUSE);
				enemy_material->set_alpha_blend_op(0,LDBLEND_DISABLE,LDBLENDARG_DIFFUSE,LDBLENDARG_DIFFUSE);
				enm->add_mesh("enemy",cyl); enm->add_material(enemy_material);
			}
			break;
	}

	return enm;
}

ldMesh* shGeoFactory::supa(long i)
{
	ldGeoFilter	filter;
	Vec3f		vec1;
	Quaternion	ori1;
	float		height=2.0f;

	ldCompositeMesh *stone=new ldCompositeMesh;
	
	// Build head (mesh & material)
	ldMesh *head_mesh=game_char_head(height*0.15f);
	filter.scale(head_mesh,4.0f,4.0f,4.0f);
	filter.move(head_mesh,0.0f,height*0.925f*4.0f,0.0f);
	head_mesh->close();

	ldMaterial *head_material=GFX->new_material();
	head_material->set_texture(0,get_texture("textures/supa_head.png"));
	head_material->set_diffuse_color(1.0f,1.0f,1.0f,1.0f);
	head_material->set_color_blend_op(0,LDBLEND_MODULATE,LDBLENDARG_TEXTURE,LDBLENDARG_DIFFUSE);
	head_material->set_alpha_blend_op(0,LDBLEND_DISABLE,LDBLENDARG_DIFFUSE,LDBLENDARG_DIFFUSE);
	// Build body (mesh & material)
	ldMesh *body_mesh=game_char_body(height*0.85);
	filter.scale(body_mesh,4.0f,4.0f,4.0f);
	body_mesh->close();
	ldMaterial *body_material=GFX->new_material();
	body_material->set_texture(0,get_texture("textures/supa_body.png"));
	body_material->set_diffuse_color(1.0f,1.0f,1.0f,1.0f);
	body_material->set_color_blend_op(0,LDBLEND_MODULATE,LDBLENDARG_TEXTURE,LDBLENDARG_DIFFUSE);
	body_material->set_alpha_blend_op(0,LDBLEND_DISABLE,LDBLENDARG_DIFFUSE,LDBLENDARG_DIFFUSE);
	// Build arms (mesh)
	// Upper left arm
/*	ldMesh *upper_left_arm_mesh=stone_arm(0.15*scale);
	upper_left_arm_mesh->close();
	// Lower left arm
	ldMesh *lower_left_arm_mesh=stone_arm(0.10*scale);
	lower_left_arm_mesh->close();
	// Upper right arm
	ldMesh *upper_right_arm_mesh=stone_arm(0.15*scale);
	upper_right_arm_mesh->close();
	// Lower right arm
	ldMesh *lower_right_arm_mesh=stone_arm(0.10*scale);
	lower_right_arm_mesh->close();
*/
	// Composite everything
	stone->add_mesh("head",head_mesh); stone->add_material(head_material);
	stone->add_mesh("body",body_mesh); stone->add_material(body_material);
//	stone->add_mesh("upper_left_arm",upper_left_arm_mesh); stone->add_material(body_material);
//	stone->add_mesh("lower_left_arm",lower_left_arm_mesh); stone->add_material(body_material);
//	stone->add_mesh("upper_right_arm",upper_right_arm_mesh); stone->add_material(body_material);
//	stone->add_mesh("lower_right_arm",lower_right_arm_mesh); stone->add_material(body_material);

	return stone;
}
