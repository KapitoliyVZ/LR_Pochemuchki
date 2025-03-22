#pragma once
//#define _CRT_SECURE_NO_WARNINGS
//#define _CRT_SECURE_NO_DEPRECATE
//#define _CRT_NONSTDC_NO_DEPRECATE
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <cctype>
#include <filesystem>
#include <sstream>

#include <vector>
#include <windows.h> 

#include <msclr/marshal.h>			
#include <msclr/marshal_cppstd.h>	
// Две библиотеки для маршалинга данных между управляемыми и неуправляемыми типами (Преобразования string в String^).

#include <unordered_set>			// Для использования контейнера для хранения уникальных элементов.


using namespace std;
using namespace System;
using namespace msclr::interop;
using namespace System::Windows::Forms;
using namespace System::Drawing;


void inputFromFile(const char* fname, vector<string>& adverb, bool mode);						// Функция для считывания текста из файла и поиска в нем наречий, либо для записи в файл пронумерованных предложений.
bool determinePartOfSpeech(string word);														// Функция для определения наречий.

void doubleRhymeList(vector<string>& adverb, vector<string>& rhyme);							// Функция для поиска рифмующихся наречий.
int countWordPairs(string str);																	// Функция для подсчета количества пар рифм в цепочке рифмующихся наречий.
void doubleRhymeToTextBox(vector<string>& rhyme, RichTextBox^ richTextBox, int* rhymeCount);	// Функция для обработки рифм и вывода их в RichTextBox с выделением рифмующихся наречий.
void rhymePairsToFile(vector<string>& rhyme, string filePath, int rhymeCount);					// Функция предназначена для записи пар рифм из вектора строк в файл. 
void highlightWordsInRichTextBox(RichTextBox^ richTextBox, string wordsToHighlight, string colorStr);	// Функция для выделения рифмующихся наречий цветами.
void mergeDuplicates(vector<string>& words);													// Функция для удаления дубликатов из вектора наречий.


string Utf8ToAnsi(string utf8String);					// Функция для преобразования строки из кодировки UTF-8 в кодировку ANSI.
string removeConsonants(string input);					// Функция для удаления согласных букв из строки.
string checkSymbol(string pas);							// Вспомогательная функция для проверки передаваемой строки на специальные символы и её дальнейшего форматирования.
string unification(string str);							// Функция для приведения строки к нижнему регистру.
string numberOrAdverb(string str, string mode);			// Функция для вывода наречия либо номера предложения, в котором оно содержится.


bool isRhyme(string word1, string word2);								// Функция для определения взаимной рифмовки слов.
bool separation(string word);											// Функция  проверяет, заканчивается ли строка символами, которые обычно используются для разделения предложений.
bool isVowel(char c);													// Функция для проверки символа на принадлежность к гласным.
bool samePositions(string word1, string word2);							// Функция для проверки совпадения позиций гласных букв в двух словах.
bool arePair(char a, char b, const vector<pair<char, char>>& pairs);	// Функция для проверки, являются ли две буквы парными.
bool checkWordWithSameLength(string word1, string word2, const vector<pair<char, char>>& vow);											// Функция для проверки на взаимную рифмовку слов с одинаковой длинной.
bool checkLastThreeLetters(string word1, string word2, const vector<pair<char, char>>& vow, const vector<pair<char, char>>& cons);		// Функция для проверки созвучности последних трех букв двух слов.


