#pragma once

#include "Gate.h"
#include "Level.h"

//------------------------------------------------------------------------------------------------------------
class AsBorder: public AHit_Checker, public AGame_Object
{
public:
	~AsBorder();
	AsBorder();

	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall_Object *ball);

	virtual void Begin_Movement();
	virtual void Finish_Movement();
	virtual void Advance(double max_speed);
	virtual double Get_Speed();

	virtual void Act();
	virtual void Clear(HDC hdc, RECT &paint_area);
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	void Redraw_Floor();
	void Open_Gate(int gate_index, bool short_open);
	int Long_Open_Gate();
	bool Is_Gate_Opened(int gate_index);
	bool Is_Gate_Closed(int gate_index);
	void Get_Gate_Pos(int gate_index, int &gate_x_pos, int &gate_y_pos);

private:
	void Draw_Element(HDC hdc, RECT &paint_area, int x, int y, bool top_border);
	void Draw_Floor(HDC hdc, RECT &paint_area);

	bool Clear_Floor;
	RECT Floor_Rect;
	std::vector<AGate *> Gates;
};
//------------------------------------------------------------------------------------------------------------
