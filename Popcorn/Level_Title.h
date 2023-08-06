#pragma once

#include "Label.h"

//------------------------------------------------------------------------------------------------------------
enum class ELevel_Title_State: unsigned char
{
	Missing,
	Showing,
	Hiding
};
//------------------------------------------------------------------------------------------------------------
class AsLevel_Title: public AGraphics_Object
{
public:
	AsLevel_Title();

	virtual void Act();
	virtual void Clear(HDC hdc, RECT &paint_area);
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	void Show(int level_number);
	void Hide();

private:
	ELevel_Title_State Level_Title_State;
	RECT Title_Rect;
	ALabel Level_Name, Level_Number;

	static const int Width = 96;
	static const int Height = 14;
	static const int X_Pos = AsConfig::Max_X_Pos / 2 - Width / 2;
	static const int Y_Pos = 150;
};
//------------------------------------------------------------------------------------------------------------
