// Popcorn.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Main.h"

// AsFrame_DC
//------------------------------------------------------------------------------------------------------------
AsFrame_DC::~AsFrame_DC()
{
	if (Bitmap != 0)
		DeleteObject(Bitmap);

	if (DC != 0)
		DeleteObject(DC);
}
//------------------------------------------------------------------------------------------------------------
AsFrame_DC::AsFrame_DC()
: Width(0), Height(0), DC(0), Bitmap(0)
{
}
//------------------------------------------------------------------------------------------------------------
HDC AsFrame_DC::Get_DC(HWND hwnd, HDC hdc)
{
	int dc_width, dc_height;
	RECT rect;

	GetClientRect(hwnd, &rect);

	dc_width = rect.right - rect.left;
	dc_height = rect.bottom - rect.top;

	if (dc_width != Width && dc_height != Height)
	{
		if (Bitmap != 0)
			DeleteObject(Bitmap);

		if (DC != 0)
			DeleteObject(DC);

		Width = dc_width;
		Height = dc_height;

		DC = CreateCompatibleDC(hdc);
		Bitmap = CreateCompatibleBitmap(hdc, Width, Height);
		SelectObject(DC, Bitmap);

		++rect.right;
		++rect.bottom;

		AsTools::Rect(DC, rect, AsConfig::BG_Color);
	}

	return DC;
}
//------------------------------------------------------------------------------------------------------------




// Global Variables:
AsMain_Window Main_Window;
//------------------------------------------------------------------------------------------------------------
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	return Main_Window.Main(hInstance, nCmdShow);
}
//------------------------------------------------------------------------------------------------------------




// AsMain_Window
AsMain_Window *AsMain_Window::Self = 0;
//------------------------------------------------------------------------------------------------------------
AsMain_Window::AsMain_Window()
: Instance(0)
{
	Self = this;
}
//------------------------------------------------------------------------------------------------------------
int APIENTRY AsMain_Window::Main(HINSTANCE instance, int command_show)
{
	HACCEL accel_table;
	MSG msg;
	wchar_t str_buf[Max_String_Size];

	Instance = instance; // Store instance handle in our global variable

	// Initialize global strings
	LoadStringW(Instance, IDS_APP_TITLE, str_buf, Max_String_Size);
	Title = str_buf;

	LoadStringW(Instance, IDC_POPCORN, str_buf, Max_String_Size);
	Window_Class_Name = str_buf;

	Register_Class();

	// Perform application initialization:
	if (! Init_Instance(command_show))
		return FALSE;

	accel_table = LoadAccelerators(Instance, MAKEINTRESOURCE(IDC_POPCORN));


	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, accel_table, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}
//------------------------------------------------------------------------------------------------------------
ATOM AsMain_Window::Register_Class()
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = Window_Proc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = Instance;
	wcex.hIcon = LoadIcon(Instance, MAKEINTRESOURCE(IDI_POPCORN));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = AsConfig::BG_Color.Get_Brush();
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_POPCORN);
	wcex.lpszClassName = Window_Class_Name.Get_Content();
	wcex.hIconSm = LoadIcon(Instance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}
//------------------------------------------------------------------------------------------------------------
BOOL AsMain_Window::Init_Instance(int command_show)
{
	HWND hwnd;
	RECT window_rect;

	window_rect.left = 0;
	window_rect.top = 0;
	window_rect.right = 320 * 3;
	window_rect.bottom = 200 * 3;

	AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW - WS_THICKFRAME, TRUE);

	hwnd = CreateWindowW(Window_Class_Name.Get_Content(), Title.Get_Content(), WS_OVERLAPPEDWINDOW - WS_THICKFRAME, 0, 0, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top, 0, 0, Instance, 0);

	if (hwnd == 0)
		return FALSE;

	Engine.Init_Engine(hwnd);

	ShowWindow(hwnd, command_show);
	UpdateWindow(hwnd);

	return TRUE;
}
//------------------------------------------------------------------------------------------------------------
void AsMain_Window::On_Paint(HWND hwnd)
{
	HDC hdc, frame_dc;
	PAINTSTRUCT ps;

	hdc = BeginPaint(hwnd, &ps);
	frame_dc = Frame_DC.Get_DC(hwnd, hdc);
	Engine.Draw_Frame(frame_dc, ps.rcPaint);

	BitBlt(hdc, 0, 0, Frame_DC.Width, Frame_DC.Height, frame_dc, 0, 0, SRCCOPY);

	EndPaint(hwnd, &ps);
}
//------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK AsMain_Window::Window_Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(Self->Instance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	break;


	case WM_PAINT:
		Self->On_Paint(hWnd);
	break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;


	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			return Self->Engine.On_Key(EKey_Type::Left, true);

		case VK_RIGHT:
			return Self->Engine.On_Key(EKey_Type::Right, true);

		case VK_SPACE:
			return Self->Engine.On_Key(EKey_Type::Space, true);
		}
		break;


	case WM_KEYUP:
		switch (wParam)
		{
		case VK_LEFT:
			return Self->Engine.On_Key(EKey_Type::Left, false);

		case VK_RIGHT:
			return Self->Engine.On_Key(EKey_Type::Right, false);

		case VK_SPACE:
			return Self->Engine.On_Key(EKey_Type::Space, false);
		}
		break;


	case WM_CHAR:
		Self->Engine.On_Char(wParam);
		break;


	case WM_TIMER:
		if (wParam == Self->Engine.Timer_ID)
			return Self->Engine.On_Timer();
		break;


	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
//------------------------------------------------------------------------------------------------------------
INT_PTR CALLBACK AsMain_Window::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;


	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}

	return (INT_PTR)FALSE;
}
//------------------------------------------------------------------------------------------------------------
/*

V Конец уровня при потере мяча
V 1. Анимация расплавления платформы
V 2. Анимация выкатывания новой
V 3. Анимация расширяющейся платформы
V 4. Пуск мяча пробелом и состояния мяча

V Кирпичи
V 1. Обычные кирпичи (синие или красные)
V 1.1. Могут быть с буквой или без
V 1.2. При попадании в кирпич он исчезает из уровня сразу, и если кирпич был:
V 1.2.1. Без буквы - создаётся активный кирпич
V 1.2.2. С буквой - создаётся падающая буква

V 2. Неразрушаемый кирпич
V 2.1. Анимация при попадании

V 3. Многоразовый кирпич
V 3.1. 4 состояния кирпича (1-4 удара до разрушения)
V 3.2. Переход по состояниям
V 3.3. Анимация приза при разрушении (кирпич исчезает, анимация - остаётся)

V 4. Кирпич с парашютом
V 4.1. Анимация парашюта - как вариант падающей буквы

V 5. Кирпич телепортации
V 5.1. Анимация при захвате и выпуске мяча

V 6. Кирпич рекламы
V 6.1. Анимация фрагмента "рекламы", скрывавшейся за кирпичом


V Взаимодействие особых кирпичей
V 1. Падающая буква - взаимодействует только с платформой
V 2. Активный кирпич - взаимодействует только с шариком


V Действия букв
V 1. Простые:
V 1.1. О ("Отмена") — отмена действия символов К, Ш, П, Л и М.
V 1.2. И ("Инверсия")
V 1.3. С ("Скорость")
V 1.4. М ("Монстры")
V 1.5. Ж ("Жизнь")


V 2. Сложные:
V 2.1. К ("Клей")
V 2.1.1. Новое состояние платформы
V 2.1.2. Анимация растекающегося клея (прямая и обратная)
V 2.1.3. Фиксация шарика (+ новое состояние шарика)
V 2.1.4. Пуск шарика пробелом и по таймауту

V 2.2. Ш ("Шире")
V 2.2.1. Новое состояние платформы (+ изменение размера)
V 2.2.2. Анимация расширяющейся платформы (прямая и обратная)

V 2.3. П ("Пол")
V 2.3.1. Отрисовка пола
V 2.3.2. Шкала пола
V 2.3.3. Взаимодействие пола с мячом
V 2.3.4. Отбитие мяча нижней гранью платформы

V 2.4. Л ("Лазер")
V 2.4.1. Новое состояние платформы
V 2.4.2. Анимация превращения в пушку (прямая и обратная)
V 2.4.3. Стрельба пробелом:
V 2.4.3.1. Обработка пробела особым образом, когда платформа в режиме лазера
V 2.4.3.2. Объекты лазерных лучей - анимация, перемещение и взаимодействие с другими объектами

V 2.5. Т ("Три")
V 2.5.1. Добавить поддержку множества мячиков
V 2.5.2. Вывод множества мячиков
V 2.5.3. Взаимодействие множества мячиков с элементами игры

X 2.6. + (Переход на следующий уровень)
X 2.6.1. Анимация досрочного перехода на следующий уровень
X 2.6.2. Отдельный уровень перехода с особыми стенками (рамкой)

V Движение мячика
V 1. Отскок мячика от рамки
V 2. Попадания в края платформы
V 3. Подсечка мяча платформой
V 4. Попадание в монстра

V Попадание в кирпич
V 1. Попадание в кирпич - состояния кирпича
V 2. Выбивание падающей буквы
V 3. Перехват падающей буквы
V 4. Для каждой буквы - свои действия

V Гейты
V 1. Вывод гейтов
V 2. Анимация открытия/закрытия гейтов
V 3. Состояния гейтов (закрыт, открывается, закрывается)


V Монстры
V 1. Список монстров (глаз, "черепаха", "сатурн", голова, "бубен", аквариум, зажигалка)
V 2. Для каждого - сделать раскадровку
V 3. Перемещение, выбор направления + состояния (живой/пауза/исчезает)
V 4. Взаимодействие с мячом и платформой
V 5. Выход из гейта
V 6. Для каждого - анимация


V Информационная панель
V 1. Логотип
V 2. Индикаторы
V 2.1. Имя игрока
V 2.2 Текущий счёт
V 3. Шкала монстров и пола
V 4. Изменение шкал
V 5. Окно дополнительных жизней
V 6. Учёт игровых действий и отображение на индикаторах


V Игра и уровни
V 1. Список первых 10 уровней
V 2. Состояния игры:
V 2.1. Заставка
V 2.2. Анимация начала уровня
V 2.3. Играем уровень
V 2.4. Потеря жизни
V 2.5. Переход на следующий уровень:
V 2.5.1. Нормальный
X 2.5.2. Досрочный
X 2.6. Окончание игры

V 3. Финальная настройка
V 3.1. Исправляем 1-й уровень
V 3.2. Играем и исправляем ошибки
V 3.3. Компилим в релизе



*/
