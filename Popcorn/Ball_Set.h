#pragma once

#include "Ball.h"

//------------------------------------------------------------------------------------------------------------
class AsBall_Set: public AGame_Objects_Set
{
public:
	AsBall_Set();

	virtual void Act();

	void Release_From_Platform(double platform_x_pos);
	bool Release_Next_Ball();
	void Set_On_Platform(double platform_x_pos);
	void Disable_All_Balls();
	bool All_Balls_Are_Lost();
	void Set_For_Test();
	bool Is_Test_Finished();
	void Triple_Balls();
	void Inverse_Balls();
	void Accelerate();
	void Reset_Speed();
	void On_Platform_Advance(double direction, double platform_speed, double max_speed);

private:
	void Turn_Tripled_Ball(ABall *ball, bool add_min_angle);
	virtual bool Get_Next_Game_Object(int &index, AGame_Object **game_obj);

	std::vector<ABall> Balls;
};
//------------------------------------------------------------------------------------------------------------
