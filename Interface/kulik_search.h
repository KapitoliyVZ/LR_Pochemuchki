#pragma once
#include "standard_libraries.h"

// данный заголовочный файл предназначен дл€ реализации разбиени€ вход€щего текста на вектор векторов предложений

// ‘ункци€ дл€ удалени€ символов переноса строки из текста
void removeNewlines(string& text) {
	text.erase(remove(text.begin(), text.end(), '\n'), text.end());
}

vector<vector<string>> parse_sentences(string text)   //разделение текста на предложени€
{
	vector<vector<string>> sentences = {};
	string line = "";
	string word = "";
	bool quote_is_open = false; //флаг открыти€ кавычек
	vector<string> current_sentence;
	removeNewlines(text);

	line = text;

	for (int i = 0; i < line.size(); i++)
	{
		if (line[i] != '.' && line[i] != '!' && line[i] != '?' && line[i] != 'Е')
		{
			if (line[i] == ',' || line[i] == ':')  //если встретилась зап€та€ или двоеточие
			{
				if (word != "")
				{
					current_sentence.push_back(word);
					word = "";
				}
				word += line[i];
				current_sentence.push_back(word);
				word = "";
				continue;
			}

			if (line[i] == 'Ђ')  //если встретились открывающие кавычки
			{
				quote_is_open = true;
				word += line[i];
				current_sentence.push_back(word);
				word = "";
				continue;
			}

			if (line[i] == 'ї')  //если встретились закрывающие кавычки
			{
				quote_is_open = false;

				//если после кавычек идет тире со словами автора с маленькой буквы, продолжаем предложение. 
				//("ѕривет!" - сказал он.)
				if ((i < line.size() - 4) && (line[i + 2] == 'Ц' && (line[i + 4] >= 'а' && line[i + 4] <= '€')) || (line[i + 1] == ',') || (line[i + 1] == '.'))
				{
					if (word != "")
					{
						current_sentence.push_back(word);
						word = "";
					}
					word += line[i];
					current_sentence.push_back(word);
					word = "";
					continue;
				}

				//проверка, не был ли перед кавычками завершающий знак предложени€; 
				//если он был, то тогда записываем завершающие кавычки и завершаем предложение; (ѕример: "ѕривет!")
				if (line[i - 1] == '?!' || line[i - 1] == '!' || line[i - 1] == '?' || line[i - 1] == 'Е' || line[i - 1] == '.')
				{
					if (word != "")
					{
						current_sentence.push_back(word);
						word = "";
					}
					word += line[i];
					current_sentence.push_back(word);
					sentences.push_back(current_sentence);
					current_sentence.clear();
					word = "";
					continue;
				}
			}

			if (line[i] == ' ') //если встретилс€ пробел
			{
				if (word != "")
				{
					current_sentence.push_back(word);
					word = "";
				}
			}
			else  //если буква
			{
				word += line[i];
			}
		}


		else   //если встретилс€ завершающий знак предложени€
		{
			if (quote_is_open == true)  //продолжаем записывать предложение, если кавычки ќ“ –џ“џ, но встретилс€ завершающий знак предложени€  (ѕример:  "я пришел!")         
			{
				if (word != "")
				{
					current_sentence.push_back(word);
					word = "";
				}
				word += line[i];
				current_sentence.push_back(word);
				word = "";
				continue;
			}

			if (line[i] == 'Е' && i < line.size() - 2)  //если троеточие встретилось внутри предложени€, не в конце строки, то продолжаем записывать (ѕример: “ихо... хорошо)
			{
				if (line[i + 2] >= 'а' && line[i + 2] <= '€') //если буква через пробел маленька€, то это одно предложение 
				{
					if (word != "")
					{
						current_sentence.push_back(word);
						word = "";
					}
					word += line[i];
					current_sentence.push_back(word);
					word = "";
					continue;
				}

			}

			//если точка встретилась внутри скобок, продолжаем записывать  
			if (line[i] == '.' && i < line.size() - 1)
			{
				if (line[i + 1] == ')')    //если следующий после точки символ ) , то записываем скобку последним символом в текущее предложение              
				{
					if (word != "")
					{
						current_sentence.push_back(word);
						word = "";
					}
					word += line[i];
					current_sentence.push_back(word);
					word = "";
					word += line[i + 1];
					word = "";
					current_sentence.push_back(word);
					sentences.push_back(current_sentence);
					current_sentence.clear();
					i++;      //увеличиваем самосто€тельно счетчик, так как записали сразу скобку, идущую после точки
					continue;
				}

			}

			//дл€ диалога: если после ?; ...; !; . идет тире со словами автора с маленькой буквы, продолжаем предложение
			if ((line[i] == '!' || line[i] == '?' || line[i] == '.' || line[i] == 'Е') && (i < line.size() - 4))
			{
				if (line[i + 2] == 'Ц' && (line[i + 4] >= 'а' && line[i + 4] <= '€'))   //проверка, что через пробел идет "-", а потом через пробел маленька€ буква (ѕример: - ѕривет! - сказала она.)
				{
					if (word != "")
					{
						current_sentence.push_back(word);
						word = "";
					}
					word += line[i];
					current_sentence.push_back(word);
					word = "";
					continue;
				}
			}

			if (word != "")
			{
				current_sentence.push_back(word);
				word = "";
			}
			word += line[i];
			current_sentence.push_back(word);
			sentences.push_back(current_sentence);
			current_sentence.clear();
			word = "";
		}
	}

	return sentences;
}