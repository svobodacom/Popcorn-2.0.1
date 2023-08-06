#include "Engine.h"

// AsEngine
//------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine()
: Timer_ID(WM_USER + 1), Game_State(EGame_State::Enter_name), Got_Name(false), Rest_Distance(0.0), Modules{}
{
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Init_Engine(HWND hwnd)
{// Настройка игры при старте

	int index;

	SYSTEMTIME sys_time;
	FILETIME file_time;

	GetSystemTime(&sys_time);
	SystemTimeToFileTime(&sys_time, &file_time);

	srand(file_time.dwLowDateTime);

	AsConfig::Hwnd = hwnd;

	Level.Init();
	Platform.Init(&Ball_Set, &Laser_Beam_Set);
	Monster_Set.Init(&Border);
	Info_Panel.Init();

	AFalling_Letter::Init();

	ABall::Hit_Checker_List.Add_Hit_Checker(&Border);
	ABall::Hit_Checker_List.Add_Hit_Checker(&Level);
	ABall::Hit_Checker_List.Add_Hit_Checker(&Platform);
	ABall::Hit_Checker_List.Add_Hit_Checker(&Monster_Set);

	ALaser_Beam::Hit_Checker_List.Add_Hit_Checker(&Level);
	ALaser_Beam::Hit_Checker_List.Add_Hit_Checker(&Monster_Set);

	AsPlatform::Hit_Checker_List.Add_Hit_Checker(&Monster_Set);

	Platform.Redraw_Platform();

	SetTimer(AsConfig::Hwnd, Timer_ID, 1000 / AsConfig::FPS, 0);

	// Modules
	index = 0;

	Modules.push_back(&Level);
	Modules.push_back(&Border);
	Modules.push_back(&Platform);
	Modules.push_back(&Ball_Set);
	Modules.push_back(&Laser_Beam_Set);
	Modules.push_back(&Monster_Set);
	Modules.push_back(&Info_Panel);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{// Отрисовка экрана игры

	SetGraphicsMode(hdc, GM_ADVANCED);

	for (auto *curr_module : Modules)
		curr_module->Clear(hdc, paint_area);

	for (auto *curr_module : Modules)
		curr_module->Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Key(EKey_Type key_type, bool key_down)
{
	switch (key_type)
	{
	case EKey_Type::Left:
		Platform.Move(true, key_down);
		break;


	case EKey_Type::Right:
		Platform.Move(false, key_down);
		break;


	case EKey_Type::Space:
		Platform.On_Space_Key(key_down);
		break;
	}

	return 0;
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::On_Timer()
{
	++AsConfig::Current_Timer_Tick;

	switch (Game_State)
	{
	case EGame_State::Test_Ball:
		Ball_Set.Set_For_Test();
		Level.Set_Current_Level(0);
		Game_State = EGame_State::Play_Level;
		break;


	case EGame_State::Enter_name:
		if (Got_Name)
		{
			Level.Mop_Level(1);
			Game_State = EGame_State::Mop_Level;
		}
		break;


	case EGame_State::Mop_Level:
		if (Level.Is_Level_Mopping_Done() )
			Restart_Level();
		break;


	case EGame_State::Play_Level:
		Play_Level();
		break;


	case EGame_State::Lost_Ball:
		if (Is_Destroying_Complete() )
		{
			if (Info_Panel.Decrease_Life_Count() )
				Restart_Level();
			else
				Game_Over();
		}
		break;


	case EGame_State::Restart_Level:
		if (Platform.Has_State(EPlatform_Substate_Regular::Ready) )
		{
			Game_State = EGame_State::Play_Level;
			Ball_Set.Set_On_Platform(Platform.Get_Middle_Pos() );
			Monster_Set.Activate(AsConfig::Max_Monsters_Count);
			Level.Hide_Title();
		}
		break;


	case EGame_State::Finish_Level:
		if (Is_Destroying_Complete() )
		{
			Level.Mop_Next_Level();
			Game_State = EGame_State::Mop_Level;
		}
		break;


	case EGame_State::Game_Over:
		break;  // Не делаем ничего


	case EGame_State::Game_Won:
		if (Is_Destroying_Complete() )
			Game_Won();
		break;
	}

	Act();

	return 0;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::On_Char(wchar_t symbol)
{
	if (! Got_Name)
		Got_Name = Info_Panel.Edit_Player_Name(symbol);
}
//------------------------------------------------------------------------------------------------------------
bool AsEngine::Is_Destroying_Complete()
{
	if (Monster_Set.Are_All_Destroyed() && Platform.Has_State(EPlatform_Substate_Regular::Missing) )
		return true;
	else
		return false;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Restart_Level()
{
	Game_State = EGame_State::Restart_Level;
	Level.Show_Title();
	Border.Open_Gate(7, true);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Play_Level()
{
	Advance_Movers();

	if (Ball_Set.All_Balls_Are_Lost() )
	{// Потеряли все мячики

		Stop_Play();
		Game_State = EGame_State::Lost_Ball;
	}
	else
		Ball_Set.Accelerate();

	if (Ball_Set.Is_Test_Finished() )
		Game_State = EGame_State::Test_Ball;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Stop_Play()
{
	Level.Stop();
	Set_Floor_State(false);
	Monster_Set.Destroy_All();
	Laser_Beam_Set.Disable_All();
	Platform.Set_State(EPlatform_State::Meltdown);
	Info_Panel.Floor_Indicator.Reset();
	Info_Panel.Monster_Indicator.Reset();
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Game_Over()
{
	Level.Game_Title.Show(true);
	Game_State = EGame_State::Game_Over;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Game_Won()
{
	Level.Game_Title.Show(false);
	Game_State = EGame_State::Game_Over;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Advance_Movers()
{
	double curr_speed, max_speed = 0.0;

	// 1. Получаем максимальную скорость движущихся объектов
	for (auto *curr_module : Modules)
	{
		curr_module->Begin_Movement();

		curr_speed = fabs(curr_module->Get_Speed() );

		if (curr_speed > max_speed)
			max_speed = curr_speed;
	}


	// 2. Смещаем все движущиеся объекты
	Rest_Distance += max_speed;

	while (Rest_Distance > 0.0)
	{
		for (auto *curr_module : Modules)
			curr_module->Advance(max_speed);

		Rest_Distance -= AsConfig::Moving_Step_Size;
	}


	// 3. Заканчиваем все движения на этом кадре
	for (auto *curr_module : Modules)
		curr_module->Finish_Movement();
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Act()
{
	int index = 0;
	AFalling_Letter *falling_letter;

	// 1. Выполняем все действия
	for (auto *curr_module : Modules)
		curr_module->Act();

	// 2. Ловим падающие буквы
	while (Level.Get_Next_Falling_Letter(index, &falling_letter) )
	{
		if (Platform.Hit_By(falling_letter) )
			On_Falling_Letter(falling_letter);
	}

	// 3. Рестарт уровня (если надо)
	if (Game_State == EGame_State::Restart_Level)
		if (Border.Is_Gate_Opened(AsConfig::Gates_Count - 1) )
			Platform.Set_State(EPlatform_State::Rolling);

	Handle_Message();
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Handle_Message()
{
	AMessage *message;

	if (AsMessage_Manager::Get_Message(&message) )
	{
		switch (message->Message_Type)
		{
		case EMessage_Type::Floor_Is_Over:
			Set_Floor_State(false);
			delete message;
			break;


		case EMessage_Type::Unfreeze_Monsters:
			Monster_Set.Set_Freeze_State(false);
			break;


		case EMessage_Type::Level_Done:
			Stop_Play();
			Ball_Set.Disable_All_Balls();

			if (Level.Can_Mop_Next_Level() )
				Game_State = EGame_State::Finish_Level;
			else
				Game_State = EGame_State::Game_Won;

			break;

		default:
			AsConfig::Throw();
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::On_Falling_Letter(AFalling_Letter *falling_letter)
{
	switch (falling_letter->Letter_Type)
	{
	case ELetter_Type::O:  // "Отмена"
		Info_Panel.Floor_Indicator.Cancel();
		Info_Panel.Monster_Indicator.Cancel();
		Platform.Set_State(EPlatform_Substate_Regular::Normal);
		break;

	case ELetter_Type::I:  // "Инверсия"
		Ball_Set.Inverse_Balls();
		Platform.Set_State(EPlatform_Substate_Regular::Normal);
		break;

	case ELetter_Type::C:  // "Скорость"
		Ball_Set.Reset_Speed();
		Platform.Set_State(EPlatform_Substate_Regular::Normal);
		break;

	case ELetter_Type::M:  // "Монстры"
		Monster_Set.Set_Freeze_State(true);
		Info_Panel.Monster_Indicator.Restart();  // Отобразить на индикаторе
		break;

	case ELetter_Type::G:  // "Жизнь"
		Info_Panel.Increase_Life_Count();
		Platform.Set_State(EPlatform_Substate_Regular::Normal);
		break;

	case ELetter_Type::K:  // "Клей"
		Platform.Set_State(EPlatform_State::Glue);
		break;

	case ELetter_Type::W:  // "Шире"
		Platform.Set_State(EPlatform_State::Expanding);
		break;

	case ELetter_Type::T:  // "Три"
		Platform.Set_State(EPlatform_Substate_Regular::Normal);
		Ball_Set.Triple_Balls();
		break;

	case ELetter_Type::L:  // "Лазер"
		Platform.Set_State(EPlatform_State::Laser);
		break;

	case ELetter_Type::P:  // "Пол"
		Set_Floor_State(true);
		Info_Panel.Floor_Indicator.Restart();  // Отобразить на индикаторе
		Platform.Set_State(EPlatform_Substate_Regular::Normal);
		break;

	case ELetter_Type::Plus:  // Переход на следующий уровень
		break;  // Обработка "+" не сделана

	default:
		AsConfig::Throw();
	}

	falling_letter->Finalize();

	AsInfo_Panel::Update_Score(EScore_Event_Type::Catch_Letter);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::Set_Floor_State(bool turn_on)
{
	AsConfig::Level_Has_Floor = turn_on;
	Border.Redraw_Floor();
}
//------------------------------------------------------------------------------------------------------------
