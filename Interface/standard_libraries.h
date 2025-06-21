#pragma once

// данный заголовочный файл содержит все основные библиотеки, которые используются для реализации заполнения вектора структур с содержательной информацией

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
#include <fstream>
#include <conio.h>
#include <stdio.h>
#include <commdlg.h>
#include <iomanip>
#include <cctype>
#include <ctime>
#include <cstdarg>
#include <clocale> 
#include <cstdlib>  // Для wcstombs (функция преобразования для стандарта С++ 20)
#include <chrono>
#include <thread>
#include <regex>
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