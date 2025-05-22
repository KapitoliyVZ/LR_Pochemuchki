#include "general_search.h"
#include "file_working.h"
#include "UI_const.h"
#include "output_file_working.h"

using namespace std;

// �������-����������� ��� ��������� � �������� ����, ������, �����������
void unite_functions(vector<WordData>& wordData,        // ����� ������� ��� �������� ������ � ��������� ������ �� ������ ����
    vector<vector<string>>& sentences, // ����� ������� ������ � ������������ �������������
    const string str_sentences,        // ������ � ������ 
    const string& word_to_compare,     // ����� ����� ��� ������ ����
    bitset<8>& button_flags)           // ����� ������ ��� ������ ����
{

    // sentences = splitTextIntoSentences(str_sentences); // ���������� ������ �� �����������
    sentences = parse_sentences(str_sentences);

    if (!sentences.empty())                                                  // ���� ����� ������, ������ �� ������
        deal_with_words(button_flags, sentences, word_to_compare, wordData); // ������ � ������� ������ ����

    // ��������� ��������� �����������
    addSentenceNumbers(sentences);

    // ��������� ���������� � wordData � ��������� �����������
    searchWordInSentences(wordData, sentences);

}


