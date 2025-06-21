#pragma once

// ������ ������������ ���� �������� ��� �������� ����������, ������� ������������ ��� ���������� ���������� ������� �������� � �������������� �����������

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
#include <cstdlib>  // ��� wcstombs (������� �������������� ��� ��������� �++ 20)
#include <chrono>
#include <thread>
#include <regex>
using namespace std;


// ������� ��� �������� ������ � ����� (�� ���������� �������)
int countSyllables(const std::string& word) {
	// ������ ������� ����
	const std::string vowels = "����������Ũ�������";
	int syllables = 0;

	// ���������� ������ ������ � ����� � ���������, �������� �� �� �������
	for (char c : word) {
		if (vowels.find(c) != std::string::npos) {
			syllables++;
		}
	}
	return syllables;
};