#include "Explosion.h"

// AExplosion
//------------------------------------------------------------------------------------------------------------
AExplosion::AExplosion()
: Explosive_Balls(Explosive_Balls_Count)
{
}
//------------------------------------------------------------------------------------------------------------
void AExplosion::Start_Explosion(RECT &area_rect)
{
	int half_width, half_height;
	int x_pos, y_pos;
	int x_offset, y_offset;
	int size, half_size, rest_size;
	int time_offset;
	bool is_red;

	half_width = (area_rect.right - area_rect.left) / 2;
	half_height = (area_rect.bottom - area_rect.top) / 2;
	x_pos = area_rect.left + half_width;
	y_pos = area_rect.top + half_height;

	half_size = half_width;

	if (half_height < half_size)
		half_size = half_height;

	for (auto &ball : Explosive_Balls)
	{
		x_offset = AsTools::Rand(half_width) - half_width / 2;
		y_offset = AsTools::Rand(half_height) - half_height / 2;

		rest_size = half_size - (int)sqrt(x_offset * x_offset + y_offset * y_offset);

		size = AsTools::Rand(rest_size / 2) + rest_size / 2;

		if (size < AsConfig::Global_Scale)
			size = AsConfig::Global_Scale;

		time_offset = AsTools::Rand(AsConfig::FPS * 3 / 2);

		if (AsTools::Rand(2) == 0)
			is_red = true;
		else
			is_red = false;

		ball.Explode(x_pos + x_offset, y_pos + y_offset, size * 2, is_red, time_offset, 10);
	}
}
//------------------------------------------------------------------------------------------------------------
bool AExplosion::Act_On_Explosion()
{
	bool destroing_is_finished = true;

	for (auto &ball : Explosive_Balls)
	{
		ball.Act();

		destroing_is_finished &= ball.Is_Finished();
	}

	return destroing_is_finished;
}
//------------------------------------------------------------------------------------------------------------
void AExplosion::Draw_Explosion(HDC hdc, RECT &paint_area)
{
	for (auto &ball : Explosive_Balls)
		ball.Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
