#include "Explosive_Ball.h"

// AExplosive_Ball
//------------------------------------------------------------------------------------------------------------
AExplosive_Ball::AExplosive_Ball()
: Explosive_Ball_State(EExplosive_Ball_State::Idle), Is_Red(false), X_Pos(0), Y_Pos(0), Max_Size(0), Step_Count(0),
  Start_Expanding_Tick(0), Start_Fading_Tick(0), Time_Offset(0), Size(0.0), Size_Step(0.0), Ball_Rect{}
{
}
//------------------------------------------------------------------------------------------------------------
void AExplosive_Ball::Act()
{
	switch (Explosive_Ball_State)
	{
	case EExplosive_Ball_State::Idle:
		break;


	case EExplosive_Ball_State::Charging:
		if (AsConfig::Current_Timer_Tick >= Start_Expanding_Tick)
			Explosive_Ball_State = EExplosive_Ball_State::Expanding;
		break;


	case EExplosive_Ball_State::Expanding:
		Size += Size_Step;

		if (Size >= Max_Size)
		{
			Explosive_Ball_State = EExplosive_Ball_State::Fading;
			Start_Fading_Tick = AsConfig::Current_Timer_Tick;
		}
		else
			Update_Ball_Rect();

		break;


	case EExplosive_Ball_State::Fading:
		if (AsConfig::Current_Timer_Tick > Start_Fading_Tick + Fading_Timeout)
			Explosive_Ball_State = EExplosive_Ball_State::Idle;
		break;
	}
}
//------------------------------------------------------------------------------------------------------------
void AExplosive_Ball::Clear(HDC hdc, RECT &paint_area)
{
	// Заглушка, не используется
}
//------------------------------------------------------------------------------------------------------------
void AExplosive_Ball::Draw(HDC hdc, RECT &paint_area)
{
	int curr_timeout;
	int color_index;
	double ratio;
	const AColor *color;

	switch (Explosive_Ball_State)
	{
	case EExplosive_Ball_State::Idle:
	case EExplosive_Ball_State::Charging:
		break;


	case EExplosive_Ball_State::Expanding:
		if (Is_Red)
			color = &AsConfig::Explosion_Red_Color;
		else
			color = &AsConfig::Explosion_Blue_Color;

		AsTools::Ellipse(hdc, Ball_Rect, *color);
		break;


	case EExplosive_Ball_State::Fading:
		curr_timeout = AsConfig::Current_Timer_Tick - Start_Fading_Tick;

		if (curr_timeout > Fading_Timeout)
			curr_timeout = Fading_Timeout;

		if (curr_timeout == Fading_Timeout)
			AsTools::Ellipse(hdc, Ball_Rect, AsConfig::BG_Color);
		else
		{
			ratio = (double)curr_timeout / (double)Fading_Timeout;
			color_index = (int)round(ratio * (double)(AsConfig::Max_Brick_Fade_Step - 1) );

			if (Is_Red)
				color = AsConfig::Fading_Red_Brick_Colors.Get_Color(color_index);
			else
				color = AsConfig::Fading_Blue_Brick_Colors.Get_Color(color_index);

			AsTools::Ellipse(hdc, Ball_Rect, *color);
		}
		break;
	}
}
//------------------------------------------------------------------------------------------------------------
bool AExplosive_Ball::Is_Finished()
{
	if (Explosive_Ball_State == EExplosive_Ball_State::Idle)
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
void AExplosive_Ball::Explode(int x_pos, int y_pos, int size, bool is_red, int time_offset, int step_count)
{
	X_Pos = x_pos;
	Y_Pos = y_pos;
	Max_Size = size;
	Size = 0.0;
	Time_Offset = time_offset;
	Step_Count = step_count;
	Is_Red = is_red;

	Start_Expanding_Tick = AsConfig::Current_Timer_Tick + Time_Offset;
	Explosive_Ball_State = EExplosive_Ball_State::Charging;

	Size_Step = (double)Max_Size / (double)Step_Count;

	Update_Ball_Rect();
}
//------------------------------------------------------------------------------------------------------------
void AExplosive_Ball::Update_Ball_Rect()
{
	Ball_Rect.left = X_Pos - (int)(Size / 2.0);
	Ball_Rect.top = Y_Pos - (int)(Size / 2.0);
	Ball_Rect.right = Ball_Rect.left + (int)Size;
	Ball_Rect.bottom = Ball_Rect.top + (int)Size;
}
//------------------------------------------------------------------------------------------------------------
