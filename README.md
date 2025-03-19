# Project_Barchev

#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
using namespace std;

string Rhume_Root(string Root) // принимаем на вход путь к исходному файлу, 
{
    ifstream Text_File(Root); // открываем 

    string File_Extension = ".txt"; // удаляем расширение
    int position = Root.find(File_Extension);
    if (position != -1)
    {
        Root.erase(position);
    }
   
    Root = Root + "_rhymes"; // дописка к названию (значение опционально)
    bool Correct_Name = false;
    int i = 1;
    do
    {
        ifstream Rhume_File(Root); // открываем файл на чтение
        if (Rhume_File) // если он существует, значит нельзя создавать выходной файл с таким именем
        {
            //================================================
            // убираем неудачно добавленную цифру, если она была добавлена
            position = Root.find(" (" + to_string(i-1) + ")");
            if (position != -1)
            {
                Root.erase(position);
            }
            //================================================
            Rhume_File.close(); 
       
            Root = Root + " (" + to_string(i) + ")"; // дописываем число по порядку возрастанию

        }
        else // если файл не открылся, значит его нет. Значит имя подходит для работы
        {
            Correct_Name = true;
        }
        i++; // увеличиваем индекс, если файл с номером
    } while (Correct_Name == false);// пока этот файл открывается, значит он существует, и поэтому его нельзя трогать.

    return Root + File_Extension; // дописываем расширение
}

int main()
{
    //=================================
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "ru");
    system("color F0");
    //=================================
    string jfjfjfj = "C://Users//andre//OneDrive//Рабочий стол//testing//простой файл";
    cout << Rhume_Root(jfjfjfj);
}
