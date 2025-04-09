#pragma once
#include "standard_libraries.h"
#include "NLP.h"
#include "rhymes.h"


// ��������� ��� �������� ������ � ��������� ���������
struct WordData
{
    string word; // ������ � ����� ������
    string part_of_speech; // ������ � ����������� � ����� ����
    vector <int> sentence_counter; // ���������� ��� �������� ������ �����������, � ������� ����������� �����
    vector <string> rhymed_words; // ������ ��� �������� ����, � �������� ��������� �����
    int amount = 0; // ���������� ������ ������� ����� � ������
    int rhymed_amount = 0; // ���������� ����, � ������� ��������� ������ �����
};

// ��������� 06.04.2025
// ������� ������� UTF-8 ������ �� ��������� Unicode-�������
std::vector<std::string> utf8Split(const std::string& str) {
	std::vector<std::string> result;
	size_t i = 0;

	while (i < str.size()) {
		unsigned char c = str[i];
		size_t char_len = 0;

		if ((c & 0b10000000) == 0) {
			char_len = 1; // ASCII
		}
		else if ((c & 0b11100000) == 0b11000000) {
			char_len = 2;
		}
		else if ((c & 0b11110000) == 0b11100000) {
			char_len = 3;
		}
		else if ((c & 0b11111000) == 0b11110000) {
			char_len = 4;
		}
		else {
			// �������� UTF-8 ����
			char_len = 1;
		}

		if (i + char_len > str.size()) {
			char_len = 1; // ������������
		}

		result.emplace_back(str.substr(i, char_len));
		i += char_len;
	}

	return result;
}

// ������� ��� ������� ���������� �������-�����������
int damerauLevenshteinDistance(const string& ending_1, const string& ending_2) {
	int len_1 = ending_1.size();
	int len_2 = ending_2.size();

	// ������� ��� �������� ������������� �����������
	vector <vector<int>> dist(len_1 + 1, vector<int>(len_2 + 1));

	// ������������� ������ ������ � ������� �������
	for (int i = 0; i <= len_1; ++i) dist[i][0] = i;
	for (int j = 0; j <= len_2; ++j) dist[0][j] = j;

	// �������� ���� ��� ���������� ����������
	for (int i = 1; i <= len_1; ++i) {
		for (int j = 1; j <= len_2; ++j) {
			int cost = (ending_1[i - 1] == ending_2[j - 1]) ? 0 : 1;

			// ������� �� ��������, ������� ��� ������
			dist[i][j] = min({
				dist[i - 1][j] + 1,       // ��������
				dist[i][j - 1] + 1,       // �������
				dist[i - 1][j - 1] + cost // ������
				});

			// ������������ ��������
			if (i > 1 && j > 1 && ending_1[i - 1] == ending_2[j - 2] && ending_1[i - 2] == ending_2[j - 1]) {
				dist[i][j] = min(dist[i][j], dist[i - 2][j - 2] + 1); // ������������
			}
		}
	}
	return dist[len_1][len_2];
}

// ������� ��� ���������� ��������� ����� (��������, ��������� 4 �������)
std::string getSuffix(const std::string& word, int suffix_length = 8) {
	std::vector<std::string> letters = utf8Split(word);

	std::string suffix;
	int start = (letters.size() > suffix_length) ? letters.size() - suffix_length : 0;

	for (int i = start; i < letters.size(); ++i) {
		suffix += letters[i];
	}

	return suffix;
}

// ������� �������� ������� ��������� � ��������� ���������
bool existence_of_participle(vector<WordData>& data, string& comparing_participle)
{
	for (const WordData& check : data)
	{
		if (comparing_participle == check.word)
			return true;
	}
	return false;
};

// ������� ��� �������� ����� (��������� ��������� ���� ����)
bool areWordsRhymed(const string& word_1, const string& word_2, int threshold = 8) {

	// ���������� ��������� ����
	string suffix1 = getSuffix(word_1);
	string suffix2 = getSuffix(word_2);

	// ���������� ���������� �������-����������� ����� �����������
	int distance = damerauLevenshteinDistance(suffix1, suffix2);

	// ���� ���������� ������ ������, ����� ��������� ������������
	return distance <= threshold;
}

// ������� �������� ������� ���������� ����������� ���������
bool existenceRhymedParticiples(WordData& candidate, const string& word)
{

	for (const string& check_participle : candidate.rhymed_words)
	{
		if (word == check_participle)
			return true;
	}
	return false;
}

// �������� ������� ������ ����
vector <WordData> find_rhymes(vector<vector<string>>& words_text_collection, bitset<8> button_flags,vector<string>& word_to_compare)
{

	vector <WordData> data;
	// ������������� ����������, ������� ����� ��������� � �������� ������ data
	WordData candidate;

	int rhyme_counter = 0;

	int same_words_counter = 0;


	// ������� �������� ����
	int variant_of_check = 0;

	// ���������� �������� ���� ������ ���� ��� ������������� �����
	if (button_flags.test(7) == 1 && word_to_compare.empty())
		variant_of_check = 0;

	// ��������� �������� ���� ������ ���� �� ������������ ������
	if (button_flags.test(7) == 1 && !word_to_compare.empty())
		variant_of_check = 1;

	// ������������ �������� ���� ������ ���� ��� ������������� �����
	if (button_flags.test(7) == 0 && word_to_compare.empty())
		variant_of_check = 2;

	// ������������ �������� ���� ������ ���� co ������������ ������
	if (button_flags.test(7) == 0 && !word_to_compare.empty())
		variant_of_check = 3;

	// ����� ���� � ����������� �� ������ ������
	switch (variant_of_check)
	{
		// ��� ������������� ����� + ���������� ��������
	case 0:
		for (int i = 0; i < 6; i++)
		{
			if (!words_text_collection[i].empty())
			{
				for (string& first_word : words_text_collection[i])
				{
					rhyme_counter = 0;
					same_words_counter = 0;
					candidate.word = first_word;
					for (string& second_word : words_text_collection[i])
					{
						// �������� �� ���������� ���������� ����
						if (candidate.word == second_word)
						{
							same_words_counter++;
							continue;
						}

						// �������� �� ������� � ������� �������� ���������
						if (existence_of_participle(data, candidate.word))
							continue;

						// ��������, ��������� �� �����
						if (areWordsRhymed(second_word, candidate.word))
						{

							// �������� ������� ������������ ����� (���� ���������� ���� ���������, �� ��� ������ �������� ��� ��������� ��� � ������)
							if (existenceRhymedParticiples(candidate, second_word))
								continue;

							// ������� ����������� ��������� � ������ ����������� ���������
							candidate.rhymed_words.push_back(second_word);

						}
						
					}

					if (existence_of_participle(data, candidate.word))
						continue;

					candidate.amount = same_words_counter;
					candidate.rhymed_amount = candidate.rhymed_words.size();
					data.push_back(candidate);
					candidate.rhymed_words.clear();
				}
			}

		}
		break;
		// ���������� �������� ���� ���� + ������������ �����
	case 1:

		
		for (int i = 0; i < 6; i++)
		{
			

			if (!word_to_compare[i].empty())
			{
				candidate.word = word_to_compare[i];

				for (string& second_word : words_text_collection[i])
				{
					
					// �������� �� ���������� ���������� ����
					if (candidate.word == second_word)
					{
						same_words_counter++;
						continue;
					}

					// �������� �� ������� � ������� �������� ���������
					if (existence_of_participle(data, candidate.word))
						continue;

					// ��������, ��������� �� �����
					if (areWordsRhymed(second_word, candidate.word))
					{

						// �������� ������� ������������ ����� (���� ���������� ���� ���������, �� ��� ������ �������� ��� ��������� ��� � ������)
						if (existenceRhymedParticiples(candidate, second_word))
							continue;

						// ������� ����������� ��������� � ������ ����������� ���������
						candidate.rhymed_words.push_back(second_word);

					}
				}

				if (existence_of_participle(data, candidate.word))
					continue;

				candidate.amount = same_words_counter;
				candidate.rhymed_amount = candidate.rhymed_words.size();
				data.push_back(candidate);
				candidate.rhymed_words.clear();
				same_words_counter = 0;

			}
		}
		break;

		// ������������ �������� ���� ���� ��� ������������� �����
	case 2: 

		for (int i = 0; i < 6; i++)
		{
			if (!words_text_collection[i].empty())
			{
				for (string& first_word : words_text_collection[i])
				{
					rhyme_counter = 0;
					same_words_counter = 0;
					candidate.word = first_word;

					for (int j = 0; j < 6; j++)
					{
						// ����� ���������� ����
						if (j == i)
						{
							for (string& word : words_text_collection[i])
							{
								if (candidate.word == word)
									same_words_counter++;
							}
							continue;
						}
						
						for (string& second_word : words_text_collection[j])
						{
							

							// �������� �� ������� � ������� �������� ���������
							if (existence_of_participle(data, candidate.word))
								continue;

							// ��������, ��������� �� �����
							if (areWordsRhymed(second_word, candidate.word))
							{

								// �������� ������� ������������ ����� (���� ���������� ���� ���������, �� ��� ������ �������� ��� ��������� ��� � ������)
								if (existenceRhymedParticiples(candidate, second_word))
									continue;

								// ������� ����������� ��������� � ������ ����������� ���������
								candidate.rhymed_words.push_back(second_word);

							}

						

						}

					
					}

					if (existence_of_participle(data, candidate.word))
						continue;

					candidate.amount = same_words_counter;
					candidate.rhymed_amount = candidate.rhymed_words.size();
					data.push_back(candidate);
					candidate.rhymed_words.clear();
				}
			}

		}
		break;
	
	case 3:

		for (int i = 0; i < 6; i++)
		{
			if (!word_to_compare[i].empty())
			{
				candidate.word = word_to_compare[i];
				rhyme_counter = 0;
				same_words_counter = 0;
			
				for (int j = 0; j < 6; j++)
				{
					// ����� ���������� ����
					if (j == i)
					{
						for (string& word : words_text_collection[i])
						{
							if (candidate.word == word)
								same_words_counter++;
						}
						continue;
					}

					for (string& second_word : words_text_collection[j])
					{


						// �������� �� ������� � ������� �������� ���������
						if (existence_of_participle(data, candidate.word))
							continue;

						// ��������, ��������� �� �����
						if (areWordsRhymed(second_word, candidate.word))
						{

							// �������� ������� ������������ ����� (���� ���������� ���� ���������, �� ��� ������ �������� ��� ��������� ��� � ������)
							if (existenceRhymedParticiples(candidate, second_word))
								continue;

							// ������� ����������� ��������� � ������ ����������� ���������
							candidate.rhymed_words.push_back(second_word);

						}



					}


				}

				if (existence_of_participle(data, candidate.word))
					continue;

				candidate.amount = same_words_counter;
				candidate.rhymed_amount = candidate.rhymed_words.size();
				data.push_back(candidate);
				candidate.rhymed_words.clear();
				
			}

		}


		break;
	}


	return data;
};

// �������� ������� ������ � ������� ������ ����
// TMP VOID
void deal_with_words(bitset<8>& button_flags,const vector<vector<string>>& numbered_sentences, string word_to_compare = " ")
{
	vector<vector<string>> sentences = numbered_sentences;
	
	
    // ���������� ������ ����, ������� ����� �����
    const int amount_of_parts_of_speech = 6;


    //TMP ��������� � ������ ��������� � �������� � ��������� ������ �������
    vector <WordData> data;


    // ������ �������� �� ������ ����� ����
    vector<vector<string>> words_text_collection (amount_of_parts_of_speech);
	
	
	vector<string> comparing_word_part_of_speech;

	comparing_word_part_of_speech = get_comparing_word_part(word_to_compare);
	
	if (!comparing_word_part_of_speech.empty())
	{
		for (int i = 0; i < 6; i++)
		{
			if (!comparing_word_part_of_speech[i].empty())
				button_flags.set(i);
		}
	}
    // ����������� ������ ���� ��� MyStem
    const vector <string> parts_of_speech { "V", "ADV", "A", "S", "����", "�����" };

    // ���������� ������� �������� ����������� ������� ����;
    for (int i = 0; i < 6; i++)
    {
        if (button_flags.test(i))
        {
            words_text_collection[i] = findWordsByPartOfSpeech(sentences, parts_of_speech[i]);
        }

    }

    /*// TMP �������� ������
    for (vector<string>& sentence : sentences)
    {
        for (string& word : sentence)
            cout << word << " ";

        cout << endl;
    };

    for (int i = 0; i < 6; i++)
    {
        if (!words_text_collection[i].empty())
            cout << "part of speeech" + parts_of_speech[i] + ": ";

        for (string& word : words_text_collection[i])
        {
             cout << word + " ";
        }

        if (!words_text_collection[i].empty())
            cout << endl;
    }*/

    // ����� ����
    data = find_rhymes(words_text_collection, button_flags, comparing_word_part_of_speech);


    // �������� ������ TMP
    /*cout << endl << endl;
   
    for (WordData& output : data)
    {
        cout << "�����: " << output.word;
        cout << endl;
        if (output.rhymed_words.size() != 0)
        {
            cout << "rhymed with ";
            for (string& word : output.rhymed_words)
            {
                cout << " " << word << " ,";
            }

        }
        cout << endl;
        cout << "amount of word in the text:  " << output.amount << endl;
        cout << "amount of rhymes: " << output.rhymed_amount << endl << endl;
    }
    */


};