#include "Library.h"
#include "Header.h"

vector<set<string>> verbEndings = {}; // ������ ��������� ��� ������������� �������

int main()
{
    setlocale(LC_ALL, "Russian");

    start_menu();
    return 0;
}

// ������� ���������� ����
void start_menu()
{
    bool flag_cycle = false; // true - ����� �� �����

    do
    {
        flag_cycle = false;
        char user_answer; // �������, �������� �������������

        system("cls");
        cout << setw(50) << "WElCOME\n"
             << setw(20) << " ��������� ������������� ��� ������ ����������� �������� �� �������� ������. \n"
             << setw(20) << " ������� ������������ �� ������ ���������, ������������ � ����� 'VerbEndings.txt' � �������� ���������.\n"
             << setw(20) << " ������� ���� � ������� ������ ����� ������ '.txt.' \n\n"
             << setw(20) << " ��������� ���� ���� ������������ � ����� ���� � ������ ����� ��������� ����� + (Rhymes).\n"
             << setw(20) << " ������� ����� � ��������� ��������� ����������q ������������ � ����� ���� � ������ ����� ��������� ������ + (NumbText).\n"
             << setw(20) << " ��� ��������� �������� ����� � ������ ������������� ����� ��������� ������������� ����������� ��������� ��������� ������.\n\n"
             << setw(10) << " �� ������ ���� � ������������ ���� ����������� ��������������� ��������� ��������� ���:\n"
             << setw(10) << "'ENTER'- �������/����������� �������� � ������� � ��������� ����.\n"
             << setw(10) << "  '/'  - ��������� ����� � ����������� ���� ��� ���������� ��������.\n"
             << setw(10) << "  '*'  - ��������� ���������.\n\n\n"
             << setw(70) << "������� ����� ������� ��� ������ ���������.";
        user_answer = _getch();

        if (user_answer != FINISH_button) // ���� ��-�������-���������, ��
            flag_cycle = make_input();
        else
            flag_cycle = true;

    } while (!flag_cycle);
}

// ������� �������� ����� � ����������� ��� ��������
bool make_verb_endings()
{
    file_input.open("VerbEndings.txt");
    if (!file_input.is_open())
    {
        cout << "������!: ���� ��������� ������� ����������� � �������� �������. �������� ��� � ���������� �����. \n";
        _getch();
        return false;
    }

    fill_vector_by_endings(); // ������� ���� ����� � ������
    file_input.close();
    return true;
}

// ������� ���������� ������� ��������� �������� �� �����
void fill_vector_by_endings()
{
    // vector<set<string>> all_endings; // ��������� ������ ��� �������� ���������
    string line, word;
    bool flag = false;
    vector<string> words;
    // ������ ����� ������ �� �����
    while (!file_input.eof())
    {
        getline(file_input, line); // ������ �� ����� �����
        if (line[0] == '1' and flag == false)
        {
            flag = true;
        }
        line.erase(0, 3);
        if (flag)
        {
            stringstream ss(line);
            while (ss >> word)
            {

                // ������� ����� � ����� �����, ���� ��� ����
                if (!word.empty() && (word.back() == ',' || word.back() == '.'))
                    word.erase(word.size() - 1);

                words.push_back(word); // ��������� ����� � �����������
                set<string> bobobo;
                bobobo.insert(word);
                verbEndings.push_back(bobobo); // ��������� ����������� � ����� ������
            }
        }
    }
}

// ������� ��� ������ ���� �������� ����� �� ������
bool make_input()
{
    bool flag_cycle = false; // true - ����� �� �����
    do
    {
        system("cls");

        if (!make_verb_endings()) // �������� ������� ����� ���������
            return false;

        // ���� ����� �����
        string input_file_name = entering_name_file("input"); // (input - ������� ���� �� ������, output - �������� ���� �� ������)

        if (input_file_name[0] == FINISH_button)    // ���� ��� ����� ����� ����� ���� ������ "���������", ��
            return true;                            // ����� �� ������
        else if (input_file_name[0] == BACK_button) // ���� ��� ����� ����� ����� ���� ������ "�����", ��
            return false;                           // ��������� ����

        // ������ ������ �������� �����
        pair<vector<vector<string>>, string> text = fill_vector_by_text();
        vector<vector<string>> text_in_vector = text.first; // ����� � ��������� ������
        string text_in_string = text.second;                // ����� � ��������� ������

        // ��������� �������
        vector<VerbInfo> foundVerbs = findVerbsInSentences(text_in_vector, verbEndings);

        // ��������� ������� ����
        map<string, vector<VerbInfo>> all_rhymes = findRhymingVerbs(foundVerbs);

        // ����� ������� ��� �������� ������
        if (make_output(all_rhymes, input_file_name, text_in_vector, text_in_string))
            return true;

    } while (!flag_cycle);
}

// ������� ��� �������� ����� ������ � ������ (��� ������)
pair<vector<vector<string>>, string> fill_vector_by_text()
{
    vector<vector<string>> text_in_vector; // ��������� ������ ��� �������� ������

    string text_in_string, sentence;

    // ������ ����� ������ �� �����
    getline(file_input, text_in_string, '\0'); // ������ �� ����� �����
    file_input.close();                        // ��������� ����

    //!!! ���������

    // ���������� ���������� ��������� ��� ���������� ������ �� �����, ���������������� � ��������������� �����
    regex sentenceEndRegex("[.!?]+"); // ���������� ��������� ��� ������ ������������ (�����, ��������������� � �������������� �����)
    sregex_token_iterator sentenceIterator(text_in_string.begin(), text_in_string.end(), sentenceEndRegex, -1);
    sregex_token_iterator endIterator;

    // �������� �� ������� �����������
    while (sentenceIterator != endIterator)
    {
        sentence = *sentenceIterator++;
        if (!sentence.empty())
        {
            stringstream wordStream(sentence);
            string word;
            vector<string> words;

            // ������ ���� � �����������
            while (wordStream >> word)
            {
                // ������� ��� �������, ����� ����
                word.erase(remove_if(word.begin(),
                                     word.end(),
                                     [](unsigned char c)
                                     { return !isalpha(c); }),
                           word.end());

                // ���� ����� �������� �������� �����, ��������� ����� � ������
                if (!word.empty())
                    words.push_back(word);
            }
            text_in_vector.push_back(words); // ��������� ����������� � ����� ������
        }
    }

    return {text_in_vector, text_in_string};
}

// ������� ��� ������ �������� � ������
vector<VerbInfo> findVerbsInSentences(vector<vector<string>> text_in_vector, vector<set<string>> verbEndings)
{
    // ������ ��� �������� ���������� � ��������� ��������
    vector<VerbInfo> foundVerbs;

    for (size_t i = 0; i < text_in_vector.size(); ++i)
        for (size_t j = 0; j < text_in_vector[i].size(); ++j)
        {
            const string &word = text_in_vector[i][j];
            if (isVerb(word, verbEndings))
                // ��������� ���������� � ��������� ������� � ������
                foundVerbs.push_back({word, i + 1, j + 1}); // i + 1 � j + 1 ��� 1-����������
        }

    // ����� ��������� �������� �� �������
    // print_FoundVerbs(foundVerbs);

    return foundVerbs;
}

// ��������, �������� �� ����� �������� �� ������ ���������
bool isVerb(string word, vector<set<string>> verbEndings)
{
    // �������:
    // true - ���� ������
    // false - ���� �� ������

    for (const auto &endings : verbEndings)
        for (const auto &ending : endings)
            if (word.length() >= ending.length() && word.compare(word.length() - ending.length(), ending.length(), ending) == 0)
                return true; // ������� ������������, ����� �������� ��������

    return false;
}

// ������� ���������� ���� ��������
map<string, vector<VerbInfo>> findRhymingVerbs(vector<VerbInfo> foundVerbs)
{
    map<string, vector<VerbInfo>> rhymeGroups; // ������ ��������������� �������� �� ����������
    vector<VerbInfo> line;
    vector<string> all_verbs;
    set<string> processedVerbs;
    // �������� �� ������� ������ ���������
    // ��� ������� ��������� � ������
    int end_len = 3;

    for (const auto &verb : foundVerbs)
    {
        end_len = 3;
        if (verb.word.length() >= end_len)
        {
            string ending = verb.word.substr(verb.word.length() - end_len, end_len);

            if (ending[2] == '�' or ending[1] == '�')
            {
                ending.erase(0, 1);
                end_len -= 1;
            }
            // cout << ending << endl;
            if (processedVerbs.find(verb.word) != processedVerbs.end())
                continue;

            vector<VerbInfo> group; // ������ �������� � ����� � ��� �� ����������
            for (const auto &candidate : foundVerbs)
            {
                // ���������, ��������� �� ��������� � ��� �� ������ ��� ���������
                if (candidate.word.length() >= end_len &&
                    candidate.word.substr(candidate.word.length() - end_len, end_len) == ending &&
                    processedVerbs.find(candidate.word) == processedVerbs.end())
                {
                    group.push_back(candidate);
                    processedVerbs.insert(candidate.word); // �������� ������ ��� ������������
                }
            }

            // ��������� ������ � �����, ���� ������� ����������
            if (!group.empty())
            {
                rhymeGroups[ending] = group;
            }
        }
    }
    // _getch();
    return rhymeGroups;
}

// ������� ������ ���� ��� �������� ����� �� ������
bool make_output(map<string, vector<VerbInfo>> all_rhymes, string input_file_name, vector<vector<string>> text_in_vector, string text_in_string)
{
    system("cls");

    // ����� ������� ��������������� ������������ ����� ����
    pair<bool, string> result_rhymes = auto_name_OutputFile(input_file_name, "rhymes");
    bool succes_opening_rhymes = result_rhymes.first; // ���� ���������� �������� ����� ����
    string file_name_rhymes = result_rhymes.second;   // ��� ��������� ����� ����

    if (succes_opening_rhymes)
    {
        output_rhymes_in_file(all_rhymes); // ������ ���� � ����
        cout << "���� ���� ������� ��������� � ����� '" << file_name_rhymes << "'.\n";
    }

    // ����� ������� ��������������� ������������ ����� �����
    pair<bool, string> result_text = auto_name_OutputFile(input_file_name, "text");
    bool success_opening_text = result_text.first; // ���� ���������� �������� ����� ������
    string file_name_text = result_text.second;    // ��� ��������� ����� ������

    if (success_opening_text)
    {
        // output_newText_in_file(text_in_vector); // ������ ������-������ � ����
        output_newText_in_file(text_in_string); // ������ ������-������ � ����
        cout << "��������������� ����� �������� � ����� '" << file_name_text << "'.\n\n";
    }

    cout << "������� ����� ������, ����� ������ ����� ����������� �������� � ������ �����.\n"
         << "'*'  - ��������� ���������.\n";

    char user_answer = _getch(); // �������, �������� �������������

    if (user_answer == FINISH_button) // ���� �������-���������, ��
        return true;                  // ����� �� ���������
    else                              // �����
        return false;                 // ��������� ���� ���������
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// (�����) ������� ��� ������ �� ������� ���� ���� �� ������� � ��������� ������ ����������� � ������ ����� � �����������
void print_text_vector(vector<vector<string>> text_in_vector)
{
    for (size_t i = 0; i < text_in_vector.size(); ++i)
    {
        cout << "����������� " << i + 1 << ":" << endl;
        for (size_t j = 0; j < text_in_vector[i].size(); ++j)
        {
            cout << "����� " << j + 1 << ": " << text_in_vector[i][j] << endl;
        }
    }
    _getch();
}

// (�����) ������� ��� ������ �� ������� ��������� ��������
void print_FoundVerbs(vector<VerbInfo> foundVerbs)
{
    int count = 1;
    cout << "��������� �������:" << endl;
    for (const auto &verbInfo : foundVerbs)
    {
        cout << count << ") " << verbInfo.word << " (����������� " << verbInfo.sentenceNum
             << ", ����� " << verbInfo.wordNum << ")" << endl;
        count++;
    }
    _getch();
}

// (�����) ������� ��� ������ �� ������� ������ ����� �� �������
void print_text_string(string text_in_string)
{
    // string buffer = "";
    // getline(file_input, buffer); // ������ ������ �� �����
    // cout << buffer;

    // // ������ ����� ���������
    // while (getline(file_input, buffer))
    //     cout << buffer << endl; // ������� ������ ������ �� �����

    cout << text_in_string;
    _getch();
}