#include "Indicator.h"

// AIndicator
//------------------------------------------------------------------------------------------------------------
AIndicator::AIndicator(EMessage_Type message_type, int x_pos, int y_pos)
	: Message_Type(message_type), Need_To_Notify(false), X_Pos(x_pos), Y_Pos(y_pos), End_Tick(0)
{
	const int scale = AsConfig::Global_Scale;

	Indicator_Rect.left = X_Pos * scale;
	Indicator_Rect.top = Y_Pos * scale;
	Indicator_Rect.right = Indicator_Rect.left + Width * scale;
	Indicator_Rect.bottom = Indicator_Rect.top + Height * scale;
}
//------------------------------------------------------------------------------------------------------------
void AIndicator::Act()
{
	if (! Is_Finished() )
		AsTools::Invalidate_Rect(Indicator_Rect);
}
//------------------------------------------------------------------------------------------------------------
void AIndicator::Clear(HDC hdc, RECT &paint_area)
{
	// Заглушка, пока не используется
}
//------------------------------------------------------------------------------------------------------------
void AIndicator::Draw(HDC hdc, RECT &paint_area)
{
	int inner_x_offset = (Width - Inner_Width) / 2;
	int inner_y_offset = (Height - Inner_Height) / 2;
	int curr_height;
	const int scale = AsConfig::Global_Scale;
	double ratio;
	RECT rect;

	AsTools::Rect(hdc, X_Pos, Y_Pos, Width, Height, AsConfig::Teleport_Portal_Color);

	if (End_Tick == 0 || Is_Finished() )
		return;

	ratio = (double)(End_Tick - AsConfig::Current_Timer_Tick) / (double)Indicator_Timeout;

	curr_height = (int)( (double)(Inner_Height * scale) * ratio);

	if (curr_height == 0)
		return;

	rect.left = (X_Pos + inner_x_offset) * scale;
	rect.top = (Y_Pos + inner_y_offset) * scale + (Inner_Height * scale - curr_height);
	rect.right = rect.left + Inner_Width * scale;
	rect.bottom = (Y_Pos + inner_y_offset + Inner_Height) * scale;

	AsTools::Rect(hdc, rect, AsConfig::Red_Color);
}
//------------------------------------------------------------------------------------------------------------
bool AIndicator::Is_Finished()
{
	if (AsConfig::Current_Timer_Tick > End_Tick)
	{
		Cancel();
		return true;
	}
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
void AIndicator::Cancel()
{
	AMessage *message;

	if (Need_To_Notify)
	{
		message = new AMessage(Message_Type);

		AsMessage_Manager::Add_Message(message);

		Need_To_Notify = false;

		Reset();
	}
}
//------------------------------------------------------------------------------------------------------------
void AIndicator::Restart()
{
	End_Tick = AsConfig::Current_Timer_Tick + Indicator_Timeout;
	Need_To_Notify = true;
}
//------------------------------------------------------------------------------------------------------------
void AIndicator::Reset()
{
	End_Tick = 0;
	AsTools::Invalidate_Rect(Indicator_Rect);
}
//------------------------------------------------------------------------------------------------------------
