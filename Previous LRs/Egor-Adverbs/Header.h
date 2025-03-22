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
// ��� ���������� ��� ���������� ������ ����� ������������ � �������������� ������ (�������������� string � String^).

#include <unordered_set>			// ��� ������������� ���������� ��� �������� ���������� ���������.


using namespace std;
using namespace System;
using namespace msclr::interop;
using namespace System::Windows::Forms;
using namespace System::Drawing;


void inputFromFile(const char* fname, vector<string>& adverb, bool mode);						// ������� ��� ���������� ������ �� ����� � ������ � ��� �������, ���� ��� ������ � ���� ��������������� �����������.
bool determinePartOfSpeech(string word);														// ������� ��� ����������� �������.

void doubleRhymeList(vector<string>& adverb, vector<string>& rhyme);							// ������� ��� ������ ����������� �������.
int countWordPairs(string str);																	// ������� ��� �������� ���������� ��� ���� � ������� ����������� �������.
void doubleRhymeToTextBox(vector<string>& rhyme, RichTextBox^ richTextBox, int* rhymeCount);	// ������� ��� ��������� ���� � ������ �� � RichTextBox � ���������� ����������� �������.
void rhymePairsToFile(vector<string>& rhyme, string filePath, int rhymeCount);					// ������� ������������� ��� ������ ��� ���� �� ������� ����� � ����. 
void highlightWordsInRichTextBox(RichTextBox^ richTextBox, string wordsToHighlight, string colorStr);	// ������� ��� ��������� ����������� ������� �������.
void mergeDuplicates(vector<string>& words);													// ������� ��� �������� ���������� �� ������� �������.


string Utf8ToAnsi(string utf8String);					// ������� ��� �������������� ������ �� ��������� UTF-8 � ��������� ANSI.
string removeConsonants(string input);					// ������� ��� �������� ��������� ���� �� ������.
string checkSymbol(string pas);							// ��������������� ������� ��� �������� ������������ ������ �� ����������� ������� � � ����������� ��������������.
string unification(string str);							// ������� ��� ���������� ������ � ������� ��������.
string numberOrAdverb(string str, string mode);			// ������� ��� ������ ������� ���� ������ �����������, � ������� ��� ����������.


bool isRhyme(string word1, string word2);								// ������� ��� ����������� �������� �������� ����.
bool separation(string word);											// �������  ���������, ������������� �� ������ ���������, ������� ������ ������������ ��� ���������� �����������.
bool isVowel(char c);													// ������� ��� �������� ������� �� �������������� � �������.
bool samePositions(string word1, string word2);							// ������� ��� �������� ���������� ������� ������� ���� � ���� ������.
bool arePair(char a, char b, const vector<pair<char, char>>& pairs);	// ������� ��� ��������, �������� �� ��� ����� �������.
bool checkWordWithSameLength(string word1, string word2, const vector<pair<char, char>>& vow);											// ������� ��� �������� �� �������� �������� ���� � ���������� �������.
bool checkLastThreeLetters(string word1, string word2, const vector<pair<char, char>>& vow, const vector<pair<char, char>>& cons);		// ������� ��� �������� ����������� ��������� ���� ���� ���� ����.


