#pragma once

#include "Config.h"

//------------------------------------------------------------------------------------------------------------
enum class ELaser_Beam_State: unsigned char
{
	Disabled,
	Active,
	Stopping,
	Cleanup
};
//------------------------------------------------------------------------------------------------------------
class ALaser_Beam: public AGame_Object
{
public:
	ALaser_Beam();

	virtual void Begin_Movement();
	virtual void Finish_Movement();
	virtual void Advance(double max_speed);
	virtual double Get_Speed();

	virtual void Act();
	virtual void Clear(HDC hdc, RECT &paint_area);
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	void Set_At(double x_pos, double y_pos);
	bool Is_Active();
	void Disable();

	static AHit_Checker_List Hit_Checker_List;

private:
	void Redraw_Beam();

	ELaser_Beam_State Laser_Beam_State;
	double X_Pos, Y_Pos;
	double Speed;
	RECT Beam_Rect, Prev_Beam_Rect;

	static const int Width = 1;
	static const int Height = 3;
};
//------------------------------------------------------------------------------------------------------------
