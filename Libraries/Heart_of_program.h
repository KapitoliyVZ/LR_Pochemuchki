#include "general_search.h"

using namespace std;

vector<WordData> unite_functions(const string file_path, const bitset<8> &button_flags, const string &word_to_compare)
{
    fstream input_file(file_path, ios::out | ios::binary);
    // ����� ������� ��� ������ � ������ (��������, ��������, � ��.)

    vector<vector<string>> sentences = splitTextIntoSentences(input_file); // ���������� ������ �� �����������

    vector<WordData> data; // ������ ��� �������� ������ � ��������� ���� �� ������ ����

    deal_with_words(button_flags, sentences, word_to_compare, data); // ������ � ������� ������ ����

    return data; // ���������� ������ � �������
}
