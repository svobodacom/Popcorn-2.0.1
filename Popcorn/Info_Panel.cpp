#include "Info_Panel.h"

// AsInfo_Panel
int AsInfo_Panel::Score = 0;
RECT AsInfo_Panel::Logo_Rect;
RECT AsInfo_Panel::Data_Rect;
//------------------------------------------------------------------------------------------------------------
AsInfo_Panel::AsInfo_Panel()
: Expecting_User_Name(true), Extra_Lives_Count(AsConfig::Initial_Life_Count), Start_Tick(0), Dark_Blue(0, 170, 170), Dark_Red(151, 0, 0),
  Letter_P(EBrick_Type::Blue, ELetter_Type::P, 214 * AsConfig::Global_Scale + 1, 153 * AsConfig::Global_Scale),
  Letter_G(EBrick_Type::Blue, ELetter_Type::G, 256 * AsConfig::Global_Scale, 153 * AsConfig::Global_Scale),
  Letter_M(EBrick_Type::Blue, ELetter_Type::M, 297 * AsConfig::Global_Scale - 1, 153 * AsConfig::Global_Scale),
  Floor_Indicator(EMessage_Type::Floor_Is_Over, Score_X + 8, Score_Y + Indicator_Y_Offset),
  Monster_Indicator(EMessage_Type::Unfreeze_Monsters, Score_X + 90, Score_Y + Indicator_Y_Offset),
  Player_Name_Label(Score_X + 5, Score_Y + 5, Score_Width - 2 * 5, 18, AsConfig::Name_Font, AsConfig::Blue_Color),
  Score_Label(Score_X + 5, Score_Y + 5 + Score_Value_Offset, Score_Width - 2 * 5, 18, AsConfig::Score_Font, AsConfig::White_Color)
{
	const int scale = AsConfig::Global_Scale;

	//Choose_Font();

	Logo_Rect.left = Score_X * scale;
	Logo_Rect.top = 5 * scale;
	Logo_Rect.right = Logo_Rect.left + 104 * scale;
	Logo_Rect.bottom = Logo_Rect.top + 100 * scale;

	Data_Rect.left = Score_X * scale;
	Data_Rect.top = Score_Y * scale;
	Data_Rect.right = Data_Rect.left + Score_Width * scale;
	Data_Rect.bottom = Data_Rect.top + Score_Height * scale;

	Letter_P.Show_Full_Size();
	Letter_G.Show_Full_Size();
	Letter_M.Show_Full_Size();
}
//------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Begin_Movement()
{
	// Заглушка, пока не используется
}
//------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Finish_Movement()
{
	// Заглушка, пока не используется
}
//------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Advance(double max_speed)
{
	// Заглушка, пока не используется
}
//------------------------------------------------------------------------------------------------------------
double AsInfo_Panel::Get_Speed()
{
	return 0.0;  // Заглушка, пока не используется
}
//------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Act()
{
	int curr_tick;

	Floor_Indicator.Act();
	Monster_Indicator.Act();

	if (Expecting_User_Name)
	{
		curr_tick = AsConfig::Current_Timer_Tick - Start_Tick;

		if (curr_tick > Blink_Timeout)
		{
			Start_Tick = AsConfig::Current_Timer_Tick;

			if (Player_Name_Label.Content.Get_Length() == 0)
				Player_Name_Label.Content = L"ВВЕДИТЕ ИМЯ";
			else
				Player_Name_Label.Content = L"";

			AsTools::Invalidate_Rect(Data_Rect);
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Clear(HDC hdc, RECT &paint_area)
{
	// Заглушка, т.к. очистка не нужна (индикатор при рисовании полностью себя перерисовывает)
}
//------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Draw(HDC hdc, RECT &paint_area)
{
	const int scale = AsConfig::Global_Scale;
	const wchar_t *pop_str = L"POP";
	const wchar_t *corn_str = L"CORN";
	RECT intersection_rect;

	// 1. Логотип
	if (IntersectRect(&intersection_rect, &paint_area, &Logo_Rect) )
	{
		AsTools::Rect(hdc, 211, 5, 104, 100, AsConfig::Blue_Color);

		SetBkMode(hdc, TRANSPARENT);

		// 1.1. "POP"
		AsConfig::Logo_Pop_Font.Select(hdc);
		SetTextColor(hdc, AsConfig::BG_Color.Get_RGB() );
		TextOut(hdc, (Logo_X_Pos + Shadow_X_Offset) * scale, (Logo_Y_Pos + Shadow_Y_Offset) * scale, pop_str, wcslen(pop_str) );

		SetTextColor(hdc, AsConfig::Red_Color.Get_RGB() );
		TextOut(hdc, Logo_X_Pos * scale, Logo_Y_Pos * scale, pop_str, wcslen(pop_str) );

		// 1.2. "CORN"
		AsConfig::Logo_Corn_Font.Select(hdc);
		SetTextColor(hdc, AsConfig::BG_Color.Get_RGB() );
		TextOut(hdc, (Logo_X_Pos + Shadow_X_Offset - 5) * scale, (Logo_Y_Pos + Shadow_Y_Offset + 48) * scale, corn_str, wcslen(corn_str) );

		SetTextColor(hdc, AsConfig::Red_Color.Get_RGB() );
		TextOut(hdc, (Logo_X_Pos - 5) * scale, (Logo_Y_Pos + 48) * scale, corn_str, wcslen(corn_str) );
	}


	// 2. Таблица счёта
	if (IntersectRect(&intersection_rect, &paint_area, &Data_Rect) )
	{
		// 2.1. Рамка
		AsTools::Rect(hdc, Score_X, Score_Y, Score_Width, 2, Dark_Red);
		AsTools::Rect(hdc, Score_X, Score_Y + Score_Height - 2, Score_Width, 2, Dark_Red);
		AsTools::Rect(hdc, Score_X, Score_Y, 2, Score_Height, Dark_Red);
		AsTools::Rect(hdc, Score_X + Score_Width - 2, Score_Y, 2, Score_Height, Dark_Red);

		AsTools::Rect(hdc, Score_X + 2, Score_Y + 2, Score_Width - 4, Score_Height - 4, Dark_Blue);

		// 2.2. Бордюр
		AsConfig::Highlight_Color.Select_Pen(hdc);
		MoveToEx(hdc, (Score_X + 2) * scale, (Score_Y + Score_Height - 2) * scale, 0);
		LineTo(hdc, (Score_X + 2) * scale, (Score_Y + 2) * scale);
		LineTo(hdc, (Score_X + Score_Width - 2) * scale, (Score_Y + 2) * scale);

		AsConfig::Shadow_Color.Select_Pen(hdc);
		MoveToEx(hdc, (Score_X + Score_Width - 2) * scale, (Score_Y + 2) * scale, 0);
		LineTo(hdc, (Score_X + Score_Width - 2) * scale, (Score_Y + Score_Height - 2) * scale);
		LineTo(hdc, (Score_X + 2) * scale, (Score_Y + Score_Height - 2) * scale);

		// 2.3. Имя игрока
		AsTools::Rect(hdc, Player_Name_Label.Content_Rect, Dark_Red);  // Выводим плашку фона

		Player_Name_Label.Draw(hdc);

		// 3. Счёт игрока
		AsTools::Rect(hdc, Score_Label.Content_Rect, Dark_Red);  // Выводим плашку фона

		Score_Label.Content = L"SCORE:";
		Score_Label.Content.Append(Score, 6);
		Score_Label.Draw(hdc);

		// 4. Буквы индикаторов
		Letter_P.Draw(hdc, paint_area);
		Letter_G.Draw(hdc, paint_area);
		Letter_M.Draw(hdc, paint_area);

		// 5. Индикаторы
		Floor_Indicator.Draw(hdc, paint_area);
		Monster_Indicator.Draw(hdc, paint_area);

		// 6. Дополнительные жизни
		Show_Extra_Lives(hdc);
	}
}
//------------------------------------------------------------------------------------------------------------
bool AsInfo_Panel::Is_Finished()
{
	return false;  // Заглушка, пока не используется
}
//------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Init()
{
	
}
//------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Increase_Life_Count()
{
	if (Extra_Lives_Count < AsConfig::Max_Life_Count)
	{
		++Extra_Lives_Count;
		AsTools::Invalidate_Rect(Data_Rect);
	}
}
//------------------------------------------------------------------------------------------------------------
bool AsInfo_Panel::Decrease_Life_Count()
{
	if (Extra_Lives_Count == 0)
		return false;

	--Extra_Lives_Count;
	AsTools::Invalidate_Rect(Data_Rect);

	return true;
}
//------------------------------------------------------------------------------------------------------------
bool AsInfo_Panel::Edit_Player_Name(wchar_t symbol)
{
	if (Expecting_User_Name)
	{
		Player_Name_Label.Content = L"";
		Expecting_User_Name = false;
	}

	return Player_Name_Label.Append(symbol);
}
//------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Update_Score(EScore_Event_Type event_type, EBrick_Type brick_type)
{
	switch (event_type)
	{
	case EScore_Event_Type::Hit_Brick:
		switch (brick_type)
		{
		case EBrick_Type::Red:
		case EBrick_Type::Blue:
			Score += 20;
			break;

		case EBrick_Type::Multihit_1:
			Score += 100;
			break;

		case EBrick_Type::Multihit_2:
			Score += 50;
			break;

		case EBrick_Type::Multihit_3:
			Score += 30;
			break;

		case EBrick_Type::Multihit_4:
			Score += 20;
			break;

		case EBrick_Type::Parachute:
			Score += 50;
			break;

		case EBrick_Type::Teleport:
			Score += 25;
			break;

		case EBrick_Type::Ad:
			Score += 33;
			break;
		}
		break;


	case EScore_Event_Type::Hit_Monster:
		Score += 37;
		break;


	case EScore_Event_Type::Catch_Letter:
		Score += 23;
		break;


	default:
		AsConfig::Throw();
	}

	AsTools::Invalidate_Rect(Data_Rect);
}
//------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Choose_Font()
{
	CHOOSEFONT cf{};
	LOGFONT lf{};

	cf.lStructSize = sizeof(CHOOSEFONT);
	cf.lpLogFont = &lf;
	cf.Flags = CF_SCREENFONTS;
	cf.nFontType = SCREEN_FONTTYPE;

	ChooseFont(&cf);
}
//------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Show_Extra_Lives(HDC hdc)
{
	int i, j;
	int lives_to_draw = Extra_Lives_Count;

	AsTools::Rect(hdc, Score_X + 27, Score_Y + Indicator_Y_Offset, 56, 30, AsConfig::Teleport_Portal_Color);

	for (j = 0; j < 3; j++)
		for (i = 0; i < 4; i++)
		{
			if (lives_to_draw <= 0)
				break;

			Draw_Extra_Life(hdc, 27 + 6 + j * 16, 57 + i * 7);

			--lives_to_draw;
		}
}
//------------------------------------------------------------------------------------------------------------
void AsInfo_Panel::Draw_Extra_Life(HDC hdc, int x_pos, int y_pos)
{
	const int scale = AsConfig::Global_Scale;
	RECT rect;

	rect.left = (Score_X + x_pos) * scale;
	rect.top = (Score_Y + y_pos) * scale;
	rect.right = rect.left + 4 * scale;
	rect.bottom = rect.top + 5 * scale;

	AsTools::Ellipse(hdc, rect, AsConfig::Red_Color);

	rect.left += 8 * scale;
	rect.right += 8 * scale;

	AsTools::Ellipse(hdc, rect, AsConfig::Red_Color);

	rect.left = (Score_X + x_pos + 2) * scale;
	rect.top = (Score_Y + y_pos + 1) * scale;
	rect.right = rect.left + 8 * scale;
	rect.bottom = rect.top + 3 * scale;

	AsConfig::Blue_Color.Select(hdc);
	AsTools::Round_Rect(hdc, rect);
}
//------------------------------------------------------------------------------------------------------------
