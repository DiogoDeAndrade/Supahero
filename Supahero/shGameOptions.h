// shGameOptions.h: interface for the shGameOptions class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHGAMEOPTIONS_H__00FAEE5D_3F80_4362_938E_A76B0427B449__INCLUDED_)
#define AFX_SHGAMEOPTIONS_H__00FAEE5D_3F80_4362_938E_A76B0427B449__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class shGameOptions  
{
public:
	shGameOptions();
	virtual ~shGameOptions();

	inline	float	get_camera_move_speed() { return camera_move_speed; }

protected:
	float	camera_move_speed;
};

#endif // !defined(AFX_SHGAMEOPTIONS_H__00FAEE5D_3F80_4362_938E_A76B0427B449__INCLUDED_)
