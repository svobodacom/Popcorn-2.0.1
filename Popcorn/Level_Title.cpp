#include "Level_Title.h"

// AsLevel_Title
//------------------------------------------------------------------------------------------------------------
AsLevel_Title::AsLevel_Title()
: Level_Title_State(ELevel_Title_State::Missing), Level_Name(X_Pos, Y_Pos, 72, Height, AsConfig::Name_Font, AsConfig::Blue_Color),
  Level_Number(X_Pos + Width - 32, Y_Pos, 32, Height, AsConfig::Name_Font, AsConfig::White_Color)
{
	const int scale = AsConfig::Global_Scale;

	Level_Name.Content = L"УРОВЕНЬ";

	Title_Rect.left = X_Pos * scale;
	Title_Rect.top = Y_Pos * scale;
	Title_Rect.right = Title_Rect.left + Width * scale;
	Title_Rect.bottom = Title_Rect.top + Height * scale;
}
//------------------------------------------------------------------------------------------------------------
void AsLevel_Title::Act()
{
	// Заглушка, пока не используется
}
//------------------------------------------------------------------------------------------------------------
void AsLevel_Title::Clear(HDC hdc, RECT &paint_area)
{
	if (Level_Title_State == ELevel_Title_State::Missing)
		return;

	AsTools::Rect(hdc, Title_Rect, AsConfig::BG_Color);
}
//------------------------------------------------------------------------------------------------------------
void AsLevel_Title::Draw(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect;

	if (Level_Title_State != ELevel_Title_State::Showing)
		return;

	if (! IntersectRect(&intersection_rect, &paint_area, &Title_Rect) )
		return;

	AsTools::Rect(hdc, Title_Rect, AsConfig::Red_Color);

	Level_Name.Draw(hdc);
	Level_Number.Draw(hdc);
}
//------------------------------------------------------------------------------------------------------------
bool AsLevel_Title::Is_Finished()
{
	return false;  // Заглушка, пока не используется
}
//------------------------------------------------------------------------------------------------------------
void AsLevel_Title::Show(int level_number)
{
	Level_Number.Content.Clear();
	Level_Number.Content.Append(level_number, 2);

	Level_Title_State = ELevel_Title_State::Showing;

	AsTools::Invalidate_Rect(Title_Rect);
}
//------------------------------------------------------------------------------------------------------------
void AsLevel_Title::Hide()
{
	Level_Title_State = ELevel_Title_State::Hiding;

	AsTools::Invalidate_Rect(Title_Rect);
}
//------------------------------------------------------------------------------------------------------------
