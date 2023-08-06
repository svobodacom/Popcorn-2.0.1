#pragma once

#include "resource.h"
#include "Engine.h"

//------------------------------------------------------------------------------------------------------------
class AsFrame_DC
{
public:
	~AsFrame_DC();
	AsFrame_DC();

	HDC Get_DC(HWND hwnd, HDC hdc);

	int Width, Height;

private:
	HDC DC;
	HBITMAP Bitmap;
};
//------------------------------------------------------------------------------------------------------------
class AsMain_Window
{
public:
	AsMain_Window();

	int APIENTRY Main(HINSTANCE instance, int command_show);

private:
	ATOM Register_Class();
	BOOL Init_Instance(int command_show);
	void On_Paint(HWND hwnd);

	static const int Max_String_Size = 100;

	AsFrame_DC Frame_DC;
	AsEngine Engine;
	HINSTANCE Instance;
	AString Title, Window_Class_Name;

	static LRESULT CALLBACK Window_Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	static AsMain_Window *Self;
};
//------------------------------------------------------------------------------------------------------------
