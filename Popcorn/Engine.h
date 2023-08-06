#pragma once

#include "Border.h"
#include "Level.h"
#include "Platform.h"
#include "Ball_Set.h"
#include "Monster_Set.h"
#include "Info_Panel.h"

//------------------------------------------------------------------------------------------------------------
enum class EKey_Type: unsigned char
{
	Left,
	Right,
	Space
};
//------------------------------------------------------------------------------------------------------------
enum class EGame_State: unsigned char
{
	Test_Ball,

	Enter_name,
	Mop_Level,
	Play_Level,
	Lost_Ball,
	Restart_Level,
	Finish_Level,
	Game_Over,
	Game_Won
};
//------------------------------------------------------------------------------------------------------------
class AsEngine
{
public:
	AsEngine();

	void Init_Engine(HWND hwnd);
	void Draw_Frame(HDC hdc, RECT &paint_area);
	int On_Key(EKey_Type key_type, bool key_down);
	int On_Timer();
	void On_Char(wchar_t symbol);

	const int Timer_ID;

private:
	bool Is_Destroying_Complete();
	void Restart_Level();
	void Play_Level();
	void Stop_Play();
	void Game_Over();
	void Game_Won();
	void Advance_Movers();
	void Act();
	void Handle_Message();
	void On_Falling_Letter(AFalling_Letter *falling_letter);
	void Set_Floor_State(bool turn_on);

	EGame_State Game_State;
	bool Got_Name;
	double Rest_Distance;

	AsLevel Level;
	AsPlatform Platform;
	AsBorder Border;
	AsBall_Set Ball_Set;
	AsLaser_Beam_Set Laser_Beam_Set;
	AsMonster_Set Monster_Set;
	AsInfo_Panel Info_Panel;

	std::vector<AGame_Object *> Modules;  // UNO; Главные графические объекты (модули) игры
};
//------------------------------------------------------------------------------------------------------------
