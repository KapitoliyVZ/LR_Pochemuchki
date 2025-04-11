//#pragma once
//#include "standard_libraries.h"
//vector<vector<string>> sentences = {};
//
//void parse_sentences(string& filename, ifstream& in)   //разделение текста на предложения
//{
//	string line = "";
//	string word = "";
//	/*string filename = "text.txt";
//	ifstream in;*/
//	bool quote_is_open = false; //флаг открытия кавычек
//	vector<string> current_sentence;
//
//	if (open_file(filename, in))
//	{
//		while (getline(in, line))
//		{
//			for (int i = 0; i < line.size(); i++)
//			{
//				if (line[i] != '.' && line[i] != '!' && line[i] != '?' && line[i] != '…')
//				{
//					if (line[i] == ',' || line[i] == ':' || line[i] == '(' || line[i] == ')')  //если встретилась запятая, двоеточие или скобки
//					{
//						if (word != "")
//						{
//							current_sentence.push_back(word);
//							word = "";
//						}
//						word += line[i];
//						current_sentence.push_back(word);
//						word = "";
//						continue;
//					}
//
//					if (line[i] == '«')  //если встретились открывающие кавычки
//					{
//						quote_is_open = true;
//						word += line[i];
//						current_sentence.push_back(word);
//						word = "";
//						continue;
//					}
//
//					if (line[i] == '»')  //если встретились закрывающие кавычки
//					{
//						quote_is_open = false;
//
//						//если после кавычек идет тире со словами автора с маленькой буквы, продолжаем предложение. 
//						//("Привет!" - сказал он.)
//						if ((i < line.size() - 4) && (line[i + 2] == '–' && (line[i + 4] >= 'а' && line[i + 4] <= 'я')) || (line[i + 1] == ',') || (line[i + 1] == '.'))
//						{
//							if (word != "")
//							{
//								current_sentence.push_back(word);
//								word = "";
//							}
//							word += line[i];
//							current_sentence.push_back(word);
//							word = "";
//							continue;
//						}
//
//						//проверка, не был ли перед кавычками завершающий знак предложения; 
//						//если он был, то тогда записываем завершающие кавычки и завершаем предложение; (Пример: "Привет!")
//						if (line[i - 1] == '?!' || line[i - 1] == '!' || line[i - 1] == '?' || line[i - 1] == '…' || line[i - 1] == '.')
//						{
//							if (word != "")
//							{
//								current_sentence.push_back(word);
//								word = "";
//							}
//							word += line[i];
//							current_sentence.push_back(word);
//							sentences.push_back(current_sentence);
//							current_sentence.clear();
//							word = "";
//							continue;
//						}
//					}
//
//					if (line[i] == ' ') //если встретился пробел
//					{
//						if (word != "")
//						{
//							current_sentence.push_back(word);
//							word = "";
//						}
//					}
//					else  //если буква
//					{
//						word += line[i];
//					}
//				}
//
//
//				else   //если встретился завершающий знак предложения
//				{
//					if (quote_is_open == true)  //продолжаем записывать предложение, если кавычки ОТКРЫТЫ, но встретился завершающий знак предложения  (Пример:  "Я пришел!")         
//					{
//						if (word != "")
//						{
//							current_sentence.push_back(word);
//							word = "";
//						}
//						word += line[i];
//						current_sentence.push_back(word);
//						word = "";
//						continue;
//					}
//
//					if (line[i] == '…' && i < line.size() - 2)  //если троеточие встретилось внутри предложения, не в конце строки, то продолжаем записывать (Пример: Тихо... хорошо)
//					{
//						if (line[i + 2] >= 'а' && line[i + 2] <= 'я') //если буква через пробел маленькая, то это одно предложение 
//						{
//							if (word != "")
//							{
//								current_sentence.push_back(word);
//								word = "";
//							}
//							word += line[i];
//							current_sentence.push_back(word);
//							word = "";
//							continue;
//						}
//					}
//
//					//для диалога: если после ?; ...; !; . идет тире со словами автора с маленькой буквы, продолжаем предложение
//					if ((line[i] == '!' || line[i] == '?' || line[i] == '.' || line[i] == '…') && (i < line.size() - 4))
//					{
//						if (line[i + 2] == '–' && (line[i + 4] >= 'а' && line[i + 4] <= 'я'))   //проверка, что через пробел идет "-", а потом через пробел маленькая буква (Пример: - Привет! - сказала она.)
//						{
//							if (word != "")
//							{
//								current_sentence.push_back(word);
//								word = "";
//							}
//							word += line[i];
//							current_sentence.push_back(word);
//							word = "";
//							continue;
//						}
//					}
//
//					if (word != "")
//					{
//						current_sentence.push_back(word);
//						word = "";
//					}
//					word += line[i];
//					current_sentence.push_back(word);
//					sentences.push_back(current_sentence);
//					current_sentence.clear();
//					word = "";
//				}
//			}
//		}
//	}
//	in.close();
//}