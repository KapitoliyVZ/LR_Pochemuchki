#include "general_search.h"
#include "file_working.h"
#include "UI_const.h"

using namespace std;

// �������-����������� ��� ��������� � �������� ����, ������, �����������
void unite_functions(vector<WordData> &rhymes_data,     // ����� ������� ��� �������� ������ � ��������� ������ �� ������ ����
                     vector<vector<string>> &sentences, // ����� ������� ������ � ������������ �������������
                     const string &word_to_compare,     // ����� ����� ��� ������ ����
                     bitset<8> &button_flags)           // ����� ������ ��� ������ ����
{
    sentences = splitTextIntoSentences(); // ���������� ������ �� �����������

    if (!sentences.empty())                                                    // ���� ����� ������, ������ �� ������
        deal_with_words(button_flags, sentences, word_to_compare, rhymes_data); // ������ � ������� ������ ����
}

// vector<WordData> unite_functions(bitset<8> &button_flags, const string &word_to_compare)
//{
//     vector<vector<string>> sentences = splitTextIntoSentences(); // ���������� ������ �� �����������
//
//     vector<WordData> rhymes_data; // ������ ��� �������� ������ � ��������� ���� �� ������ ����
//
//     if (!sentences.empty())                                                    // ���� ������ ������, ������ �� ������
//         deal_with_words(button_flags, sentences, word_to_compare, rhymes_data); // ������ � ������� ������ ����
//
//     return rhymes_data; // ���������� ������ � �������
// }
