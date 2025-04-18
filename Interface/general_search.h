#pragma once
#include "standard_libraries.h"
#include "NLP.h"
#include "rhymes.h"
#include "kulik_search.h"


// структура для хранения данных о найденном причастии
struct WordData
{
	string word;				  // строка с самим словом
	string part_of_speech;		  // строка с информацией о части речи
	vector<int> sentence_counter; // переменная для хранения номера предложения, в котором встретилось слово
	vector<string> rhymed_words;  // вектор для хранения слов, с которыми рифмуется слово
	int amount = 0;				  // количество встреч данного слова в тексте
	int rhymed_amount = 0;		  // количество слов, с которым рифмуется взятое слово
};

// Функция разбора UTF-8 строки на отдельные Unicode-символы
std::vector<std::string> utf8Split(const std::string& str)
{
	std::vector<std::string> result;
	size_t i = 0;

	while (i < str.size())
	{
		unsigned char c = str[i];
		size_t char_len = 0;

		if ((c & 0b10000000) == 0)
		{
			char_len = 1; // ASCII
		}
		else if ((c & 0b11100000) == 0b11000000)
		{
			char_len = 2;
		}
		else if ((c & 0b11110000) == 0b11100000)
		{
			char_len = 3;
		}
		else if ((c & 0b11111000) == 0b11110000)
		{
			char_len = 4;
		}
		else
		{
			// Неверный UTF-8 байт
			char_len = 1;
		}

		if (i + char_len > str.size())
		{
			char_len = 1; // безопасность
		}

		result.emplace_back(str.substr(i, char_len));
		i += char_len;
	}

	return result;
}

// Функция для расчета расстояния Дамерау-Левенштейна
int damerauLevenshteinDistance(const string& ending_1, const string& ending_2)
{
	int len_1 = ending_1.size();
	int len_2 = ending_2.size();

	// Матрица для хранения промежуточных результатов
	vector<vector<int>> dist(len_1 + 1, vector<int>(len_2 + 1));

	// Инициализация первой строки и первого столбца
	for (int i = 0; i <= len_1; ++i)
		dist[i][0] = i;
	for (int j = 0; j <= len_2; ++j)
		dist[0][j] = j;

	// Основной цикл для вычисления расстояний
	for (int i = 1; i <= len_1; ++i)
	{
		for (int j = 1; j <= len_2; ++j)
		{
			int cost = (ending_1[i - 1] == ending_2[j - 1]) ? 0 : 1;

			// Минимум из удаления, вставки или замены
			dist[i][j] = min({
				dist[i - 1][j] + 1,		  // удаление
				dist[i][j - 1] + 1,		  // вставка
				dist[i - 1][j - 1] + cost // замена
				});

			// Транспозиция символов
			if (i > 1 && j > 1 && ending_1[i - 1] == ending_2[j - 2] && ending_1[i - 2] == ending_2[j - 1])
			{
				dist[i][j] = min(dist[i][j], dist[i - 2][j - 2] + 1); // транспозиция
			}
		}
	}
	return dist[len_1][len_2];
}

// Функция для извлечения окончания слова (например, последние 4 символа)
std::string getSuffix(const std::string& word, int suffix_length = 8)
{
	std::vector<std::string> letters = utf8Split(word);

	std::string suffix;
	int start = (letters.size() > suffix_length) ? letters.size() - suffix_length : 0;

	for (int i = start; i < letters.size(); ++i)
	{
		suffix += letters[i];
	}

	return suffix;
}

// функция проверки наличия причастия в структуре причастий
bool existence_of_participle(vector<WordData>& data, string& comparing_participle)
{
	for (const WordData& check : data)
	{
		if (comparing_participle == check.word)
			return true;
	}
	return false;
};

// Функция для проверки рифмы (сравнение окончаний двух слов)
bool areWordsRhymed(const std::string& word_1, const std::string& word_2)
{
	// Разбиваем слова на Unicode-символы
	std::vector<std::string> letters_1 = utf8Split(word_1);
	std::vector<std::string> letters_2 = utf8Split(word_2);

	int len_1 = letters_1.size();
	int len_2 = letters_2.size();

	// Если оба слова длиннее 4 символов
	if (len_1 > 4 && len_2 > 4)
	{
		int diff_count = 0;

		// Сравниваем последние 4 символа по позициям
		for (int i = 0; i < 4; ++i)
		{
			std::string char1 = letters_1[len_1 - 4 + i];
			std::string char2 = letters_2[len_2 - 4 + i];

			if (char1 != char2)
			{
				diff_count++;
				if (diff_count > 2) return false;
			}
		}
		return true;
	}
	else
	{
		// Берём последние 2 символа
		if (len_1 < 2 || len_2 < 2) return false; // безопасности ради

		std::string suffix1 = letters_1[len_1 - 2] + letters_1[len_1 - 1];
		std::string suffix2 = letters_2[len_2 - 2] + letters_2[len_2 - 1];

		return suffix1 == suffix2;
	}
}

// функция проверки наличия одинаковых рифмованных причастий
bool existenceRhymedParticiples(WordData& candidate, const string& word)
{

	for (const string& check_participle : candidate.rhymed_words)
	{
		if (word == check_participle)
			return true;
	}
	return false;
}

// основная функция поиска рифм
vector<WordData> find_rhymes(vector<vector<string>>& words_text_collection, bitset<8> button_flags, vector<string>& word_to_compare)
{

	vector<WordData> data;
	// промежуточная переменная, которая будет добавлена в итоговый вектор data
	WordData candidate;

	int rhyme_counter = 0;

	int same_words_counter = 0;

	// вариант проверки рифм
	int variant_of_check = 0;

	for (string& word : word_to_compare)
	{
		// однородная проверка всех частей речи без сравниваемого слова
		if (button_flags.test(7) == 1 && word.empty()) {
			variant_of_check = 0;
			break;
		}


		// однородня проверка всех частей речи со сравниваемым словом
		if (button_flags.test(7) == 1 && !word.empty())
		{
			variant_of_check = 1;
			break;
		}


		// неоднородная проверка всех частей речи без сравниваемого слова
		if (button_flags.test(7) == 0 && word.empty())
		{
			variant_of_check = 2;
			break;
		}


		// неоднородная проверка всех частей речи co сравниваемым словом
		if (button_flags.test(7) == 0 && !word.empty())
		{
			variant_of_check = 3;
			break;
		}
	}


	// поиск рифм в зависимости от режима работы
	switch (variant_of_check)
	{
		// нет сравниваемого слова + однородная проверка
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
						// проверка на количество одинаковых слов
						if (candidate.word == second_word)
						{
							same_words_counter++;
							continue;
						}

						// проверка на наличие в векторе структур причастий
						if (existence_of_participle(data, candidate.word))
							continue;

						// проверка, рифмуются ли слова
						if (areWordsRhymed(second_word, candidate.word))
						{

							// проверка наличия рифмованного слова (если одинаковых слов несколько, то нет смысла заносить его несколько раз в вектор)
							if (existenceRhymedParticiples(candidate, second_word))
								continue;

							// заносим рифмованное причастие в вектор рифмованных причастий
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
		// однородная проверка всех слов + сравниваемое слово
	case 1:

		for (int i = 0; i < 6; i++)
		{

			if (!word_to_compare[i].empty())
			{
				candidate.word = word_to_compare[i];

				for (string& second_word : words_text_collection[i])
				{

					// проверка на количество одинаковых слов
					if (candidate.word == second_word)
					{
						same_words_counter++;
						continue;
					}

					// проверка на наличие в векторе структур причастий
					if (existence_of_participle(data, candidate.word))
						continue;

					// проверка, рифмуются ли слова
					if (areWordsRhymed(second_word, candidate.word))
					{

						// проверка наличия рифмованного слова (если одинаковых слов несколько, то нет смысла заносить его несколько раз в вектор)
						if (existenceRhymedParticiples(candidate, second_word))
							continue;

						// заносим рифмованное причастие в вектор рифмованных причастий
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

		// неоднородная проверка всех слов без сравниваемого слова
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
						// поиск одинаковых слов
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

							// проверка на наличие в векторе структур причастий
							if (existence_of_participle(data, candidate.word))
								continue;

							// проверка, рифмуются ли слова
							if (areWordsRhymed(second_word, candidate.word))
							{

								// проверка наличия рифмованного слова (если одинаковых слов несколько, то нет смысла заносить его несколько раз в вектор)
								if (existenceRhymedParticiples(candidate, second_word))
									continue;

								// заносим рифмованное причастие в вектор рифмованных причастий
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
					// поиск одинаковых слов
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

						// проверка на наличие в векторе структур причастий
						if (existence_of_participle(data, candidate.word))
							continue;

						// проверка, рифмуются ли слова
						if (areWordsRhymed(second_word, candidate.word))
						{

							// проверка наличия рифмованного слова (если одинаковых слов несколько, то нет смысла заносить его несколько раз в вектор)
							if (existenceRhymedParticiples(candidate, second_word))
								continue;

							// заносим рифмованное причастие в вектор рифмованных причастий
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

// основная функция работы с рифмами частей речи
void deal_with_words(bitset<8>& button_flags, const vector<vector<string>>& numbered_sentences, string word_to_compare, vector<WordData>& data)
{
	vector<vector<string>> sentences = numbered_sentences;

	// количество частей речи, которые можно найти
	const int amount_of_parts_of_speech = 6;

	// вектор векторов на каждую часть речи
	vector<vector<string>> words_text_collection(amount_of_parts_of_speech);

	vector<string> comparing_word_part_of_speech;

	if (!word_to_compare.empty())
	{
		comparing_word_part_of_speech = get_comparing_word_part(word_to_compare);

		for (string& word : comparing_word_part_of_speech)
		{
			if (!word.empty())
			{
				for (int i = 0; i < 6; i++)
				{
					if (!comparing_word_part_of_speech[i].empty())
						button_flags.set(i);
				}
			}
		}
	}

	// обозначения частей речи для MyStem
	const vector<string> parts_of_speech{ "V", "ADV", "A", "S", "прич", "деепр" };

	// заполнение вектора векторов конкретными частями речи;
	for (int i = 0; i < 6; i++)
	{
		if (button_flags.test(i))
		{
			words_text_collection[i] = findWordsByPartOfSpeech(sentences, parts_of_speech[i]);
		}
	}

	// поиск рифм
	data = find_rhymes(words_text_collection, button_flags, comparing_word_part_of_speech);

};