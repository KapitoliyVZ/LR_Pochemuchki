// ���������� � �������� ���� �������� � ���������
#ifndef HEADER
#define HEADER

#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <conio.h>
#include <cstdlib>
#include <iomanip>
#include <time.h>
#include <algorithm>
#include <windows.h>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <regex>
#include <utility>

using namespace std;

/// ������� � ��������� ASCII
const int ESC_button = 27;   // 'esc'
const int ENTER_button = 13; // 'enter'
const int STAR_button = 42;  // '*'
const int SLASH_button = 47; // '/'
const int button_1 = 49;     // '1'
const int button_2 = 50;     // '2'
const int button_3 = 51;     // '3'
const int button_4 = 52;     // '4'
const int button_5 = 53;     // '5'
const int button_6 = 54;     // '6'
const int button_7 = 55;     // '7'
const int button_8 = 56;     // '8'

/// ������� ���������
const int FINISH_button = STAR_button;   // ������ ���������� ���������
const int BACK_button = SLASH_button;    // ������ �������� � ���������� ����
const int CONFIRM_button = ENTER_button; // ������ ������������� ������

const string extension_txt = ".txt";           // ��������� ������ ���������� .txt
const size_t len_txt = extension_txt.length(); // ����� ��������� ������ ����������
const size_t MAX_FILENAME_LENGTH = 255;

ifstream file_input;  // ���� ��� ������
ofstream file_output; // ���� �� ������

// ��������� ��� �������� ���������� � �������
struct VerbInfo
{
    string word;        // ����� (������)
    size_t sentenceNum; // ����� �����������
    size_t wordNum;     // ����� ����� � �����������
};

void start_menu(); // ������� ���� ���������� ������

bool make_verb_endings();      // ������� �������� ����� � ����������� ��� ��������
void fill_vector_by_endings(); // ������� ���������� ������� ��������� �������� �� �����

bool make_input();                                                                  // ������� ��� ������ ���� �������� ����� �� ������
pair<vector<vector<string>>, string> fill_vector_by_text();                         // ������� ��� �������� ����� ������ � ������
vector<VerbInfo> findVerbsInSentences(vector<vector<string>>, vector<set<string>>); // ������� ��� ������ �������� � ������
bool isVerb(string, vector<set<string>>);                                           // ��������, �������� �� ����� �������� �� ������ ���������
map<string, vector<VerbInfo>> findRhymingVerbs(vector<VerbInfo>);                   // ������� ���������� ���� ��������
bool make_output(map<string, vector<VerbInfo>>, string, vector<vector<string>>, string);    // ������� ������ ���� ��� �������� ����� �� ������

#endif