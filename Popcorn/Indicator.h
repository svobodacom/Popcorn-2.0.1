#pragma once

#include "Config.h"

//------------------------------------------------------------------------------------------------------------
class AIndicator: public AGraphics_Object
{
public:
	AIndicator(EMessage_Type message_type, int x_pos, int y_pos);

	virtual void Act();
	virtual void Clear(HDC hdc, RECT &paint_area);
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	void Cancel();
	void Restart();
	void Reset();

private:
	EMessage_Type Message_Type;
	bool Need_To_Notify;
	int X_Pos, Y_Pos;
	int End_Tick;
	RECT Indicator_Rect;

	static const int Width = 12;
	static const int Height = 30;
	static const int Inner_Width = Width - 2;
	static const int Inner_Height = Height - 2;
	static const int Indicator_Timeout = AsConfig::FPS * 50;  // 50 секунд
};
//------------------------------------------------------------------------------------------------------------
