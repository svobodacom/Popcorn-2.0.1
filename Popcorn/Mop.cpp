#include "Mop.h"

// AsMop
//------------------------------------------------------------------------------------------------------------
AsMop::~AsMop()
{
	for (auto *indicator : Mop_Indicators)
		delete indicator;

	Mop_Indicators.erase(Mop_Indicators.begin(), Mop_Indicators.end() );

	for (auto *cylinder : Mop_Cylinders)
		delete cylinder;

	Mop_Cylinders.erase(Mop_Cylinders.begin(), Mop_Cylinders.end() );
}
//------------------------------------------------------------------------------------------------------------
AsMop::AsMop()
: Mop_State(EMop_State::Idle), Mopping_Is_Done(false), Y_Pos(0), Max_Y_Pos(0), Lifting_Height(0), Start_Tick(0)
{
	int i;
	int x_pos, y_pos;
	AMop_Indicator *indicator;
	AMop_Cylinder *cylinder;

	for (i = 0; i < 10; i++)
	{
		indicator = new AMop_Indicator(AsConfig::Level_X_Offset + 1 + i * 19, AsConfig::Level_Y_Offset + 1, i * 80);
		Mop_Indicators.push_back(indicator);
	}

	for (i = 0; i < 4; i++)
	{
		x_pos = AsConfig::Level_X_Offset + Width / 2 - 6 - i;
		y_pos = AsConfig::Level_Y_Offset + 7 + i * 5;

		cylinder = new AMop_Cylinder(x_pos, y_pos, 13 + i * 2, AMop_Cylinder::Max_Cylinder_Height[i]);
		Mop_Cylinders.push_back(cylinder);
	}
}
//------------------------------------------------------------------------------------------------------------
void AsMop::Act_Lifting(bool lift_up)
{
	int time_offset;
	double ratio;

	time_offset = AsConfig::Current_Timer_Tick - Start_Tick;

	if (time_offset <= Lifting_Timeout)
	{
		ratio = (double)time_offset / (double)Lifting_Timeout;

		if (lift_up)
			ratio = 1.0 - ratio;

		Max_Y_Pos = AsConfig::Max_Y_Pos + (int)( (double)Lifting_Height * ratio);
		Set_Mop();
	}
	else
	{
		if (lift_up)
		{
			Mop_State = EMop_State::Cleaning;
			Start_Tick = AsConfig::Current_Timer_Tick;
		}
		else
			Mop_State = EMop_State::Descend_Done;
	}
}
//------------------------------------------------------------------------------------------------------------
void AsMop::Act()
{
	int time_offset;
	double ratio;

	if (Mop_State == EMop_State::Idle || Mop_State == EMop_State::Clean_Done)
		return;

	Prev_Mop_Rect = Mop_Rect;

	time_offset = AsConfig::Current_Timer_Tick - Start_Tick;

	switch (Mop_State)
	{
	case EMop_State::Ascending:
		Act_Lifting(true);  // Поднимаем сложенную швабру
		break;


	case EMop_State::Cleaning:
		if (time_offset > Expansion_Timeout)
			Mop_State = EMop_State::Clean_Done;
		break;


	case EMop_State::Showing:
		if (time_offset > Expansion_Timeout)
		{
			Mop_State = EMop_State::Descending;
			Start_Tick = AsConfig::Current_Timer_Tick;
		}
		break;


	case EMop_State::Descending:
		Act_Lifting(false);  // Опускаем сложенную швабру
		break;


	case EMop_State::Descend_Done:
		Mop_State = EMop_State::Idle;
		Mopping_Is_Done = true;
		break;


	default:
		AsConfig::Throw();
	}

	if (Mop_State == EMop_State::Cleaning || Mop_State == EMop_State::Showing)
	{
		time_offset = AsConfig::Current_Timer_Tick - Start_Tick;

		if (time_offset <= Expansion_Timeout)
		{
			ratio = (double)time_offset / (double)Expansion_Timeout;

			if (Mop_State == EMop_State::Showing)
				ratio = 1.0 - ratio;

			for (auto *cylinder : Mop_Cylinders)
				cylinder->Set_Height_For(ratio);

			Set_Mop();
		}
	}

	for (auto *indicator : Mop_Indicators)
		indicator->Act();

	AsTools::Invalidate_Rect(Mop_Rect);
	AsTools::Invalidate_Rect(Prev_Mop_Rect);
}
//------------------------------------------------------------------------------------------------------------
void AsMop::Clear(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect;

	if (Mop_State == EMop_State::Idle)
		return;

	if (! IntersectRect(&intersection_rect, &paint_area, &Prev_Mop_Rect) )
		return;

	AsTools::Rect(hdc, Prev_Mop_Rect, AsConfig::BG_Color);

	for (auto *cylinder : Mop_Cylinders)
		cylinder->Clear(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
void AsMop::Draw(HDC hdc, RECT &paint_area)
{
	if (Mop_State == EMop_State::Idle)
		return;

	AsTools::Rect(hdc, AsConfig::Level_X_Offset, Y_Pos, Width, Height, AsConfig::Red_Color);

	for (auto *indicator : Mop_Indicators)
		indicator->Draw(hdc, paint_area);

	for (auto *cylinder : Mop_Cylinders)
		cylinder->Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
bool AsMop::Is_Finished()
{
	return false;  // Заглушка, пока не используется
}
//------------------------------------------------------------------------------------------------------------
void AsMop::Activate(bool cleaning)
{
	if (cleaning)
	{
		Y_Pos = 172;
		Mop_State = EMop_State::Ascending;
		Mopping_Is_Done = false;

		Lifting_Height = Get_Cylinders_Height() + Height;
		Max_Y_Pos = AsConfig::Max_Y_Pos + Lifting_Height;
	}
	else
		Mop_State = EMop_State::Showing;

	Start_Tick = AsConfig::Current_Timer_Tick;
	Set_Mop();
}
//------------------------------------------------------------------------------------------------------------
void AsMop::Clean_Area(HDC hdc)
{
	RECT rect;

	if (Mop_State == EMop_State::Idle)
		return;

	rect = Mop_Rect;
	rect.bottom = AsConfig::Max_Y_Pos * AsConfig::Global_Scale;

	AsTools::Rect(hdc, rect, AsConfig::BG_Color);
}
//------------------------------------------------------------------------------------------------------------
bool AsMop::Is_Mopping_Done()
{
	return Mopping_Is_Done;
}
//------------------------------------------------------------------------------------------------------------
bool AsMop::Is_Cleaning_Done()
{
	if (Mop_State == EMop_State::Clean_Done)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
int AsMop::Get_Cylinders_Height()
{
	int total_cylinder_height = 0;

	for (auto *cylinder : Mop_Cylinders)
		total_cylinder_height += cylinder->Get_Height();

	return total_cylinder_height;
}
//------------------------------------------------------------------------------------------------------------
void AsMop::Set_Mop()
{
	int i;
	int curr_y_pos = 0;
	const int scale = AsConfig::Global_Scale;

	Y_Pos = Max_Y_Pos - Get_Cylinders_Height() - Height + 1;

	for (auto *indicator : Mop_Indicators)
		indicator->Set_Y_Pos(Y_Pos + 1);

	for (i = 0; i < (int)Mop_Cylinders.size(); i++)
	{
		Mop_Cylinders[i]->Set_Y_Pos(Y_Pos + Height + curr_y_pos);

		curr_y_pos += Mop_Cylinders[i]->Get_Height();
	}

	Mop_Rect.left = AsConfig::Level_X_Offset * scale;
	Mop_Rect.top = Y_Pos * scale;
	Mop_Rect.right = Mop_Rect.left + Width * scale;
	Mop_Rect.bottom = Mop_Rect.top + Height * scale;
}
//------------------------------------------------------------------------------------------------------------
