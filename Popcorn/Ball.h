#pragma once

#include "Config.h"

//------------------------------------------------------------------------------------------------------------
class ABall: public AGame_Object, public ABall_Object
{
public:
	ABall();

	virtual void Begin_Movement();
	virtual void Finish_Movement();
	virtual void Advance(double max_speed);
	virtual double Get_Speed();

	virtual void Act();
	virtual void Clear(HDC hdc, RECT &paint_area);
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	virtual double Get_Direction();
	virtual void Set_Direction(double new_direction);
	virtual EBall_State Get_State();
	virtual void Set_State(EBall_State new_state, double x_pos = 0, double y_pos = 0);
	virtual void Reflect(bool from_horizontal);
	virtual void Draw_Teleporting(HDC hdc, int step);
	virtual void Set_On_Parachute(int brick_x, int brick_y);
	virtual void Get_Center(double &x_pos, double &y_pos);
	virtual bool Is_Moving_Up();
	virtual bool Is_Moving_Left();

	void Set_Speed(double new_speed);
	void Set_For_Test();
	bool Is_Test_Finished();
	void Forced_Advance(double direction, double speed, double max_speed);
	void Release();

	int Release_Timer_Tick;  // Значение счётчика времени, после которого надо отпустить прикленненый мячик

	static AHit_Checker_List Hit_Checker_List;

private:
	void Redraw_Ball();
	void Redraw_Parachute();
	void Draw_Parachute(HDC hdc, RECT &paint_area);
	void Clear_Parachute(HDC hdc);
	bool Detect_Hits_Cycling();

	EBall_State Ball_State, Prev_Ball_State;
	double Ball_Direction, Prev_Ball_Direction;
	double Ball_Speed, Prev_Ball_Speed;

	bool Testing_Is_Active;
	int Test_Iteration;
	double Rest_Test_Distance;

	double Center_X_Pos, Center_Y_Pos;

	RECT Ball_Rect, Prev_Ball_Rect;
	RECT Parachute_Rect, Prev_Parachute_Rect;

	int Recent_Hits_Pos, Recent_Hits_Count;
	APoint Recent_Hits[16];

	static const int Parachute_Size = 15;
	static const int On_Platform_Timeout = 10 * AsConfig::FPS;  // Время нахождения на платформе
};
//------------------------------------------------------------------------------------------------------------
