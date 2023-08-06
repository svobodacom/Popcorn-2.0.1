#include "Platform_State.h"

// AsPlatform_State
//------------------------------------------------------------------------------------------------------------
AsPlatform_State::AsPlatform_State()
: Current_State(EPlatform_State::Regular), Next_State(EPlatform_State::Unknown), Regular(EPlatform_Substate_Regular::Missing),
  Meltdown(EPlatform_Substate_Meltdown::Unknown), Rolling(EPlatform_Substate_Rolling::Unknown),
  Glue(EPlatform_Transformation::Unknown), Expanding(EPlatform_Transformation::Unknown),
  Laser(EPlatform_Transformation::Unknown), Moving(EPlatform_Moving_State::Stop)
{
}
//------------------------------------------------------------------------------------------------------------
AsPlatform_State::operator EPlatform_State() const
{
	return Current_State;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_State::operator = (EPlatform_State new_state)
{
	Current_State = new_state;
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform_State::Set_Next_State(EPlatform_State next_state)
{
	if (next_state == Current_State)
		return;

	switch (Current_State)
	{
	case EPlatform_State::Regular:
		AsConfig::Throw();  // Обычное состояние "само" не заканчивается, переключение в другое состояние должно быть явным
		break;

	case EPlatform_State::Meltdown:
		return;  // Игнорируем любое следующее состояние после Meltdown, т.к. после него должен рестартовать игровой процесс

	case EPlatform_State::Rolling:
		AsConfig::Throw();  // Состояние Rolling "само" должно переключаться в следующее
		break;

	case EPlatform_State::Glue:
		Glue = EPlatform_Transformation::Finalize;
		break;

	case EPlatform_State::Expanding:
		Expanding = EPlatform_Transformation::Finalize;
		break;

	case EPlatform_State::Laser:
		Laser = EPlatform_Transformation::Finalize;
		break;

	default:
		AsConfig::Throw();
	}

	Next_State = next_state;
}
//------------------------------------------------------------------------------------------------------------
EPlatform_State AsPlatform_State::Get_Next_State()
{
	return Next_State;
}
//------------------------------------------------------------------------------------------------------------
EPlatform_State AsPlatform_State::Set_State(EPlatform_Substate_Regular new_regular_state)
{
	EPlatform_Transformation *transformation_state = 0;

	if (Current_State == EPlatform_State::Regular && Regular == new_regular_state)
		return EPlatform_State::Unknown;

	if (new_regular_state == EPlatform_Substate_Regular::Normal)
	{
		switch (Current_State)
		{
		case EPlatform_State::Glue:
			transformation_state = &Glue;
			break;

		case EPlatform_State::Expanding:
			transformation_state = &Expanding;
			break;

		case EPlatform_State::Laser:
			transformation_state = &Laser;
			break;
		}

		if (transformation_state != 0)
		{
			if (*transformation_state == EPlatform_Transformation::Unknown)
				return Set_Next_Or_Regular_State(new_regular_state);  // Финализация состояния закончилась
			else
				*transformation_state = EPlatform_Transformation::Finalize;  // Запускаем финализацию состояния

			return EPlatform_State::Unknown;
		}
	}

	Current_State = EPlatform_State::Regular;
	Regular = new_regular_state;

	return EPlatform_State::Unknown;
}
//------------------------------------------------------------------------------------------------------------
EPlatform_State AsPlatform_State::Set_Next_Or_Regular_State(EPlatform_Substate_Regular new_regular_state)
{// Возврат: если не Unknown, то надо перейти в это новое состояние

	EPlatform_State next_state;

	Current_State = EPlatform_State::Regular;

	// Если есть отложенное состояние, то переведём в него, а не в Regular
	next_state = Get_Next_State();

	if (next_state == EPlatform_State::Unknown)
		Regular = new_regular_state;

	Next_State = EPlatform_State::Unknown;

	return next_state;
}
//------------------------------------------------------------------------------------------------------------
