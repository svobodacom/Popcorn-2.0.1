#include "Mop_Indicator.h"

// AMop_Indicator
//------------------------------------------------------------------------------------------------------------
AMop_Indicator::AMop_Indicator(int x_pos, int y_pos, int time_offset)
	: X_Pos(x_pos), Y_Pos(y_pos), Time_Offset(time_offset), Current_Color(&AsConfig::Red_Color)
{
	Set_Y_Pos(Y_Pos);
}
//------------------------------------------------------------------------------------------------------------
void AMop_Indicator::Act()
{
	int total_timeout = Normal_Timeout + AsConfig::Max_Mop_Indicator_Fade_Step;
	int current_tick = (AsConfig::Current_Timer_Tick + Time_Offset) % total_timeout;
	int current_offset;

	if (current_tick < Normal_Timeout)
		Current_Color = &AsConfig::Red_Color;
	else
	{
		current_offset = current_tick - Normal_Timeout;

		if (current_offset < 0 || current_offset >= AsConfig::Max_Mop_Indicator_Fade_Step)
			AsConfig::Throw();

		Current_Color = AsConfig::Fading_Blue_Mop_Indicator_Colors.Get_Color(current_offset);
	}

	AsTools::Invalidate_Rect(Indicator_Rect);
}
//------------------------------------------------------------------------------------------------------------
void AMop_Indicator::Clear(HDC hdc, RECT &paint_area)
{
	// Заглушка, пока не используется
}
//------------------------------------------------------------------------------------------------------------
void AMop_Indicator::Draw(HDC hdc, RECT &paint_area)
{
	const int scale = AsConfig::Global_Scale;
	RECT intersection_rect;

	if (! IntersectRect(&intersection_rect, &paint_area, &Indicator_Rect) )
		return;

	AsTools::Rect(hdc, Indicator_Rect, *Current_Color);

	// Рамка индикатора
	AsConfig::Highlight_Color.Select_Pen(hdc);
	MoveToEx(hdc, X_Pos * scale, (Y_Pos + Height) * scale, 0);
	LineTo(hdc, X_Pos * scale, Y_Pos * scale);
	LineTo(hdc, (X_Pos + Width) * scale, Y_Pos * scale);

	AsConfig::Shadow_Color.Select_Pen(hdc);
	MoveToEx(hdc, (X_Pos + Width) * scale, Y_Pos * scale, 0);
	LineTo(hdc, (X_Pos + Width) * scale, (Y_Pos + Height) * scale);
	LineTo(hdc, X_Pos * scale, (Y_Pos + Height) * scale);
}
//------------------------------------------------------------------------------------------------------------
bool AMop_Indicator::Is_Finished()
{
	return false;  // Заглушка, пока не используется
}
//------------------------------------------------------------------------------------------------------------
void AMop_Indicator::Set_Y_Pos(int y_pos)
{
	const int scale = AsConfig::Global_Scale;

	Y_Pos = y_pos;

	Indicator_Rect.left = X_Pos * scale;
	Indicator_Rect.top = Y_Pos * scale;
	Indicator_Rect.right = Indicator_Rect.left + Width * scale;
	Indicator_Rect.bottom = Indicator_Rect.top + Height * scale;
}
//------------------------------------------------------------------------------------------------------------
