#pragma once

#include "Config.h"

//------------------------------------------------------------------------------------------------------------
enum class EBrick_Type: unsigned char
{
	None,

	Red,
	Blue,
	Unbreakable,
	Multihit_1,
	Multihit_2,
	Multihit_3,
	Multihit_4,
	Parachute,
	Teleport,
	Ad,
	Invisible
};
//------------------------------------------------------------------------------------------------------------
enum class EDirection_Type: unsigned char
{
	Left,
	Up,
	Right,
	Down
};
//------------------------------------------------------------------------------------------------------------
class AActive_Brick: public AGraphics_Object
{
public:
	void Get_Level_Pos(int &dest_brick_x, int &dest_brick_y);

protected:
	virtual ~AActive_Brick();
	AActive_Brick(EBrick_Type brick_type, int level_x, int level_y);

	virtual void Clear(HDC hdc, RECT &paint_area);

	double Get_Brick_X_Pos(bool of_center);
	double Get_Brick_Y_Pos(bool of_center);

	EBrick_Type Brick_Type;
	int Level_X, Level_Y;
	RECT Brick_Rect;
};
//------------------------------------------------------------------------------------------------------------
class AActive_Brick_Red_Blue: public AActive_Brick
{
public:
	~AActive_Brick_Red_Blue();
	AActive_Brick_Red_Blue(EBrick_Type brick_type, int level_x, int level_y);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	static void Draw_In_Level(HDC hdc, RECT &brick_rect, EBrick_Type brick_type);

private:
	int Fade_Step;
};
//------------------------------------------------------------------------------------------------------------
class AActive_Brick_Unbreakable: public AActive_Brick
{
public:
	~AActive_Brick_Unbreakable();
	AActive_Brick_Unbreakable(int level_x, int level_y);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	static void Draw_In_Level(HDC hdc, RECT &brick_rect);

private:
	int Animation_Step;
	HRGN Region;

	static const int Max_Animation_Step = 12;
};
//------------------------------------------------------------------------------------------------------------
class AActive_Brick_Multihit: public AActive_Brick
{
public:
	~AActive_Brick_Multihit();
	AActive_Brick_Multihit(int level_x, int level_y);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	static void Draw_In_Level(HDC hdc, RECT &brick_rect, EBrick_Type brick_type);

private:
	static void Draw_Stage(HDC hdc, RECT &brick_rect, int x, int width);

	int Rotation_Step;

	static const int Steps_Per_Turn = 16;
	static const int Max_Rotation_Step = Steps_Per_Turn * 4;
};
//------------------------------------------------------------------------------------------------------------
enum class ETeleport_State: unsigned char
{
	Starting,
	Finishing,
	Done
};
//------------------------------------------------------------------------------------------------------------
class AActive_Brick_Teleport: public AActive_Brick
{
public:
	~AActive_Brick_Teleport();
	AActive_Brick_Teleport(int level_x, int level_y, ABall_Object *ball, AActive_Brick_Teleport *destination_teleport);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	static void Draw_In_Level(HDC hdc, RECT &brick_rect, int step = 0);

	EDirection_Type Release_Direction;

private:
	void Set_Ball(ABall_Object *ball);

	ETeleport_State Teleport_State;
	int Animation_Step;
	ABall_Object *Ball;
	AActive_Brick_Teleport *Destination_Teleport;

	static const int Max_Animation_Step = 12;
};
//------------------------------------------------------------------------------------------------------------
class AAdvertisement: public AGraphics_Object
{
public:
	~AAdvertisement();
	AAdvertisement(int level_x, int level_y, int width, int height);

	virtual void Act();
	virtual void Clear(HDC hdc, RECT &paint_area);
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	void Show_Under_Brick(int level_x, int level_y);
	bool Has_Brick_At(int level_x, int level_y);

private:
	int Level_X, Level_Y;
	int Width, Height;
	RECT Ad_Rect;
	HRGN Empty_Region;
	int Ball_X, Ball_Y;
	int Ball_Width, Ball_Height;
	int Ball_Y_Offset;
	double Falling_Speed, Acceleration_Step;
	double Deformation_Ratio;

	HRGN *Brick_Regions;

	static const int Ball_Size = 12;
	static const int High_Ball_Threshold = 12 * AsConfig::Global_Scale;
	static const int Low_Ball_Threshold = -3 * AsConfig::Global_Scale;
	static const int Deformation_Height = 2 * AsConfig::Global_Scale;
};
//------------------------------------------------------------------------------------------------------------
class AActive_Brick_Ad: public AActive_Brick
{
public:
	~AActive_Brick_Ad();
	AActive_Brick_Ad(int level_x, int level_y, AAdvertisement *advertisement);

	virtual void Act();
	virtual void Draw(HDC hdc, RECT &paint_area);
	virtual bool Is_Finished();

	static void Draw_In_Level(HDC hdc, RECT &brick_rect);

private:
	AAdvertisement *Advertisement;

	static const int Circle_Size = 7;
};
//------------------------------------------------------------------------------------------------------------
