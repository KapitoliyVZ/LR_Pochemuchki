#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE
#include "MyForm.h"
#include "Header.h"


using namespace ProjectXXX;
[STAThread]
int WINAPI	WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//// Инициализация консоли
	/*AllocConsole();
	freopen("CONOUT$", "w", stdout);*/

	// Настройка кодировки консоли
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew MyForm);
	setlocale(LC_ALL, "Rus");
	return 0;
}


