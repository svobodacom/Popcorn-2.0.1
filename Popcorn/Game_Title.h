#pragma once

#include "Final_Letter.h"

//------------------------------------------------------------------------------------------------------------
enum class EGame_Title_State: unsigned char
{
	Idle,

	Game_Over_Descent,
	Game_Over_Show,
	Game_Over_Destroy,

	Game_Won_Descent,
	Game_Won_Animate,

	Finished
};
//------------------------------------------------------------------------------------------------------------
class AsGame_Title: public AGraphics_Object
{
public:
	~AsGame_Title();
	AsGame_Title();

	virtual void Act();
	virtual void Clear(HDC hdc, RECT &paint_area);
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	void Show(bool game_over);
	bool Is_Visible();

private:
	void Destroy_Letters(int curr_tick);
	void Animate_Game_Won();

	EGame_Title_State Game_Title_State;
	int Start_Tick;
	int Destroy_Index;
	RECT Title_Rect, Prev_Title_Rect;

	std::vector<AFinal_Letter *> Title_Letters;

	static const int Descent_Timeout = AsConfig::FPS * 6;  // Спускается за 6 секунд
	static const int Game_Over_Showing_Timeout = AsConfig::FPS * 2;
	static const int Game_Won_Animate_Timeout = AsConfig::FPS * 30;
	static const int Destroy_Delay = AsConfig::FPS / 2;
	static const int Height = 32;
	static const double Low_Y_Pos;
};
//------------------------------------------------------------------------------------------------------------
