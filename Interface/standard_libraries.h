#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <algorithm>
#include <locale>
#include <codecvt>
#include <unordered_map>
#include <bitset>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <stdio.h>
#include "windows.h"
#include <commdlg.h>
#include <iomanip>
#include <cctype>
#include <ctime>
#include <sstream>
#include <cstdarg>
#include <algorithm>
#include <vector>
#include <clocale>  // Для установки ?локали?
#include <cstdlib>  // Для wcstombs (функция преобразования для стандарта С++ 20)
#include <chrono>
#include <thread>
#include <unordered_map>
#include <regex>
#include <conio.h>
#include <cstdio>
#include "windows.h"
#include <commdlg.h>
#include <iomanip>
#include <cctype>
#include <ctime>
#include <sstream>
#include <cstdarg>
#include <algorithm>
#include <vector>
#include <clocale>  // Для установки ?локали?
#include <cstdlib>  // Для wcstombs (функция преобразования для стандарта С++ 20)
#include <chrono>
#include <thread>
#include <unordered_map>
#include <regex>
#include <stdexcept>
using namespace std;


// Функция для подсчета слогов в слове (по количеству гласных)
int countSyllables(const std::string& word) {
	// Массив гласных букв
	const std::string vowels = "аеёиоуыэюяАЕЁИОУЫЭЮЯ";
	int syllables = 0;

	// Перебираем каждый символ в слове и проверяем, является ли он гласным
	for (char c : word) {
		if (vowels.find(c) != std::string::npos) {
			syllables++;
		}
	}
	return syllables;
};