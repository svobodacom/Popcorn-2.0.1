#include "Laser_Beam_Set.h"

// AsLaser_Beam_Set
//------------------------------------------------------------------------------------------------------------
AsLaser_Beam_Set::AsLaser_Beam_Set()
: Laser_Beams(Max_Laser_Beam_Count)
{
}
//------------------------------------------------------------------------------------------------------------
void AsLaser_Beam_Set::Fire(double left_gun_x_pos, double right_gun_x_pos)
{
	ALaser_Beam *left_beam = 0, *right_beam = 0;

	for (auto &beam : Laser_Beams)
	{
		if (beam.Is_Active() )
			continue;

		if (left_beam == 0)
			left_beam = &beam;
		else
			if (right_beam == 0)
			{
				right_beam = &beam;
				break;
			}
	}

	if (left_beam == 0 || right_beam == 0)
		AsConfig::Throw();  // Не хватило "свободных" лазерных лучей!

	left_beam->Set_At(left_gun_x_pos, AsConfig::Platform_Y_Pos - 1);
	right_beam->Set_At(right_gun_x_pos, AsConfig::Platform_Y_Pos - 1);
}
//------------------------------------------------------------------------------------------------------------
void AsLaser_Beam_Set::Disable_All()
{
	for (auto &beam : Laser_Beams)
		beam.Disable();
}
//------------------------------------------------------------------------------------------------------------
bool AsLaser_Beam_Set::Get_Next_Game_Object(int &index, AGame_Object **game_obj)
{
	if (index < 0 || index >= (int)Laser_Beams.size() )
		return false;

	*game_obj = &Laser_Beams[index++];

	return true;
}
//------------------------------------------------------------------------------------------------------------
