#include "Platform_Glue.h"

// AsPlatform_Glue
const double AsPlatform_Glue::Max_Glue_Spot_Height_Ratio = 1.0;
const double AsPlatform_Glue::Min_Glue_Spot_Height_Ratio = 0.4;
const double AsPlatform_Glue::Glue_Spot_Height_Ratio_Step = 0.05;
//------------------------------------------------------------------------------------------------------------
AsPlatform_Glue::AsPlatform_Glue(AsPlatform_State &platform_state)
	: Glue_Spot_Height_Ratio(0.0), Platform_State(&platform_state)
{
}
//------------------------------------------------------------------------------------------------------------
bool AsPlatform_Glue::Act(AsBall_Set *ball_set, EPlatform_State &next_state)
{
	next_state = EPlatform_State::Unknown;

	switch (Platform_State->Glue)
	{
	case EPlatform_Transformation::Init:
		if (Glue_Spot_Height_Ratio < Max_Glue_Spot_Height_Ratio)
			Glue_Spot_Height_Ratio += Glue_Spot_Height_Ratio_Step;
		else
			Platform_State->Glue = EPlatform_Transformation::Active;

		return true;


	case EPlatform_Transformation::Active:
		break;


	case EPlatform_Transformation::Finalize:
		if (Glue_Spot_Height_Ratio > Min_Glue_Spot_Height_Ratio)
		{
			Glue_Spot_Height_Ratio -= Glue_Spot_Height_Ratio_Step;

			while (ball_set->Release_Next_Ball() )
			{
			}
		}
		else
		{
			Platform_State->Glue = EPlatform_Transformation::Unknown;
			next_state = Platform_State->Set_State(EPlatform_Substate_Regular::Normal);
		}

		return true;


	default:
		AsConfig::Throw();
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_Glue::Draw_State(HDC hdc, double x_pos)
{// Рисуем платформу с растекающимся клеем

	HRGN region;
	RECT glue_rect;

	glue_rect.left = (int)( (x_pos + 5.0) * AsConfig::D_Global_Scale);
	glue_rect.top = (AsConfig::Platform_Y_Pos + 1) * AsConfig::Global_Scale;
	glue_rect.right = glue_rect.left + AsConfig::Platform_Normal_Inner_Width * AsConfig::Global_Scale;
	glue_rect.bottom = glue_rect.top + (AsConfig::Platform_Height - 2) * AsConfig::Global_Scale;

	region = CreateRectRgnIndirect(&glue_rect);
	SelectClipRgn(hdc, region);

	AsConfig::BG_Color.Select(hdc);
	Draw_Glue_Spot(hdc, x_pos, 0, 9, 5);
	Draw_Glue_Spot(hdc, x_pos, 6, 6, 5);
	Draw_Glue_Spot(hdc, x_pos, 9, 9, 6);

	AsConfig::White_Color.Select(hdc);
	Draw_Glue_Spot(hdc, x_pos, 0, 9, 4);
	Draw_Glue_Spot(hdc, x_pos, 6, 6, 4);
	Draw_Glue_Spot(hdc, x_pos, 9, 9, 5);

	SelectClipRgn(hdc, 0);
	DeleteObject(region);
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_Glue::Reset()
{
	Glue_Spot_Height_Ratio = Min_Glue_Spot_Height_Ratio;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_Glue::Draw_Glue_Spot(HDC hdc, double x_pos, int x_offset, int width, int height)
{// Рисуем пятно клея

	RECT spot_rect;
	int platform_top = (AsConfig::Platform_Y_Pos + 1) * AsConfig::Global_Scale;
	int spot_height = (int)( (double)height * AsConfig::D_Global_Scale * Glue_Spot_Height_Ratio);

	// Рисуем полуэллипс как "пятно" клея
	spot_rect.left = (int)( (x_pos + 5.0 + (double)x_offset) * AsConfig::D_Global_Scale);
	spot_rect.top = platform_top - spot_height;
	spot_rect.right = spot_rect.left + width * AsConfig::Global_Scale;
	spot_rect.bottom = platform_top + spot_height - AsConfig::Global_Scale;

	Chord(hdc, spot_rect.left, spot_rect.top, spot_rect.right - 1, spot_rect.bottom - 1,  spot_rect.left, platform_top - 1, spot_rect.right - 1, platform_top - 1);
}
//------------------------------------------------------------------------------------------------------------
