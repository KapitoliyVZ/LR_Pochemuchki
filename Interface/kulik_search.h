#pragma once
#include "standard_libraries.h"

// ������� ��� ����������� ����� ������� � UTF-8
int utf8CharLength(unsigned char byte) {
    if ((byte & 0x80) == 0) return 1;       // ASCII ������ (1 ����)
    if ((byte & 0xE0) == 0xC0) return 2;   // 2-�������� ������
    if ((byte & 0xF0) == 0xE0) return 3;   // 3-�������� ������
    if ((byte & 0xF8) == 0xF0) return 4;   // 4-�������� ������
    return 0; // ������������ ����
}


// ������� �������� ��������
void removeSpacesFromSentences(vector<vector<string>>& sentences) {
	if (sentences.empty()) return; // ���� ������ ������, ������ �� ������
    for (auto& sentence : sentences) {
		if (sentence.empty()) continue; // ���� ����������� ������, ���������� ���
        for (auto& word : sentence) {
            // ������� ��� ������� �� ������
            word.erase(remove(word.begin(), word.end(), ' '), word.end());
        }
    }
}

// ������� ��� ����������� ������ (����� � ����� ���������� ��������)
vector<string> tokenizeTextUTF8(const string& text) {
    vector<string> tokens;
    size_t i = 0;

    while (i < text.size()) {
        int char_len = utf8CharLength(static_cast<unsigned char>(text[i]));
        if (char_len == 0) {
            cerr << "������������ ������������������ ���� � ������� " << i << endl;
            break;
        }

        string utf8_char = text.substr(i, char_len);

        // ���� ��� ���� ����������, ��������� ��������
        if (ispunct(static_cast<unsigned char>(utf8_char[0])) || utf8_char == " ") {
            tokens.push_back(utf8_char);
        }
        else {
            // ���� ��� ����� �����, ���������� � ���������� ������
            if (!tokens.empty() && !ispunct(static_cast<unsigned char>(tokens.back()[0]))) {
                tokens.back() += utf8_char;
            }
            else {
                tokens.push_back(utf8_char);
            }
        }

        i += char_len;
    }

    return tokens;
}

// ������� ���������� ������ �� �����������
vector<vector<string>> splitTextIntoSentences(string str_text) 
{
    vector<vector<string>> sentences;

    // ����������� ������
    vector<string> tokens = tokenizeTextUTF8(str_text);

    // ���������� �� �����������
    vector<string> current_sentence;
    bool inside_quotes = false;

    // ������ ������ ����������, ������� ���������� ����� �����������
    const vector<string> sentence_endings = {
        ".", "!", "?", "...", "?!", "!?", "!...", "?..."
    };

    for (size_t i = 0; i < tokens.size(); ++i) {
        const auto& token = tokens[i];

        // �������� �� �������
        if (token == "\"" || token == "�") {
            inside_quotes = !inside_quotes;
        }
        else if (token == "\"" || token == "�") {
            inside_quotes = !inside_quotes;
        }

        // ��������� ����� � ������� �����������
        if (!token.empty()) {
            current_sentence.push_back(token);
        }

        // �������� ����� �����������, ���� �� ��� �������
        if (!inside_quotes &&
            (find(sentence_endings.begin(), sentence_endings.end(), token) != sentence_endings.end())) {

            // ���������, ���� �� ����� �������� ������ ������� ��� ������ �������, ����������� � �������� �����������
            while (i + 1 < tokens.size() &&
                (tokens[i + 1] == "\"" || tokens[i + 1] == "�" || tokens[i + 1] == "�")) {
                current_sentence.push_back(tokens[++i]);
            }

            // ���������, �� �������� �� ������� ����� ������ ��������������� �����
            if (i + 1 < tokens.size() &&
                find(sentence_endings.begin(), sentence_endings.end(), tokens[i + 1]) != sentence_endings.end()) {
                current_sentence.push_back(tokens[++i]);
            }

            // ��������� ����������� � ������, ���� ��� �� ������
            if (!current_sentence.empty()) {
                sentences.push_back(current_sentence);
            }

            // ������� ������� �����������
            current_sentence.clear();
        }
    }

    // ��������� ���������� ������ (���� ��� ����)
    if (!current_sentence.empty()) {
        sentences.push_back(current_sentence);
    }

    removeSpacesFromSentences(sentences);

	// ������� ��������� �������, ���� �� ������
	if (!sentences.empty())
        sentences.pop_back();

    return sentences;
}