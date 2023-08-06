#pragma once

#include "Config.h"

//------------------------------------------------------------------------------------------------------------
class AMop_Indicator: public AGraphics_Object
{
public:
	AMop_Indicator(int x_pos, int y_pos, int time_offset);

	virtual void Act();
	virtual void Clear(HDC hdc, RECT &paint_area);
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	void Set_Y_Pos(int y_pos);

private:
	int X_Pos, Y_Pos;
	int Time_Offset;
	const AColor *Current_Color;
	RECT Indicator_Rect;

	static const int Width = 17;
	static const int Height = 5;
	static const int Normal_Timeout = AsConfig::FPS;  // 1 секунда
};
//------------------------------------------------------------------------------------------------------------
