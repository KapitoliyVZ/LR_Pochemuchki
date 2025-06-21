#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>

// данный заголовочный файл предназначен для вызова MyStem и получения части речи идущего на вход MyStem слова

// получение пути к mystem.exe
std::string getMystemPath() {
    char buffer[MAX_PATH];
    DWORD length = GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    if (length == 0) {
        return "\"mystem.exe\"";  // fallback с кавычками
    }

    std::string fullPath(buffer, length);

    // найдём позицию последнего слэша
    size_t pos = fullPath.find_last_of("\\/");  // ищется последний слэш или обратный слэш для отделения директории от файла
    if (pos == std::string::npos) {
        return "\"mystem.exe\"";  // если слэш не найден — вернуть просто имя исполняемого файла
    }

    // соберём путь и добавим кавычки
    std::string mystemPath = fullPath.substr(0, pos + 1) + "mystem.exe";  // путь к директории + имя исполняемого файла
    return "\"" + mystemPath + "\"";  // оборачивает путь в кавычки, чтобы избежать проблем с пробелами
}

class MystemProcess {
private:
    HANDLE hChildStd_IN_Wr = NULL;      // дескриптор для записи в stdin дочернего процесса
    HANDLE hChildStd_OUT_Rd = NULL;     // дескриптор для чтения из stdout дочернего процесса
    PROCESS_INFORMATION piProcInfo{};   // структура с информацией о созданном процессе

public:
    MystemProcess() {
        SECURITY_ATTRIBUTES saAttr{};
        saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
        saAttr.bInheritHandle = TRUE;  // дескрипторы могут наследоваться дочерним процессом
        saAttr.lpSecurityDescriptor = NULL;

        HANDLE hChildStd_IN_Rd = NULL;
        HANDLE hChildStd_OUT_Wr = NULL;

        // создание пайпов для stdin и stdout
        if (!CreatePipe(&hChildStd_OUT_Rd, &hChildStd_OUT_Wr, &saAttr, 0))
            throw std::runtime_error("Stdout pipe creation failed.");  // пайп для чтения stdout не удалось создать

        if (!SetHandleInformation(hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
            throw std::runtime_error("Stdout read handle inheritance failed.");  // запрет на наследование дескриптора чтения stdout

        if (!CreatePipe(&hChildStd_IN_Rd, &hChildStd_IN_Wr, &saAttr, 0))
            throw std::runtime_error("Stdin pipe creation failed.");  // пайп для записи в stdin не удалось создать

        if (!SetHandleInformation(hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0))
            throw std::runtime_error("Stdin write handle inheritance failed.");  // запрет на наследование дескриптора записи stdin

        STARTUPINFOA siStartInfo{};
        siStartInfo.cb = sizeof(STARTUPINFO);
        siStartInfo.hStdError = hChildStd_OUT_Wr;   // перенаправление stderr дочернего процесса в пайп
        siStartInfo.hStdOutput = hChildStd_OUT_Wr;  // перенаправление stdout дочернего процесса в пайп
        siStartInfo.hStdInput = hChildStd_IN_Rd;    // stdin дочернего процесса будет читаться из пайпа
        siStartInfo.dwFlags |= STARTF_USESTDHANDLES;  // указание использовать перенаправленные потоки

        std::string command = getMystemPath() + " -nig";  // формирование команды запуска MyStem с параметрами

        std::vector<char> cmd(command.begin(), command.end());
        cmd.push_back('\0');  // добавление завершающего нуля для корректной передачи команды в CreateProcess

        if (!CreateProcessA(NULL, cmd.data(), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &siStartInfo, &piProcInfo))
            throw std::runtime_error("Failed to launch mystem.exe process");  // запуск дочернего процесса не удался

        // закрываем ненужные концы пайпов
        CloseHandle(hChildStd_IN_Rd);    // больше не нужен в родительском процессе
        CloseHandle(hChildStd_OUT_Wr);   // родитель не пишет в stdout дочернего процесса
    }

    std::string analyze(const std::string& text) {
        DWORD written;
        std::string input = text + "\n";  // mystem ожидает построчный ввод

        // записываем в пайп
        WriteFile(hChildStd_IN_Wr, input.c_str(), static_cast<DWORD>(input.size()), &written, NULL);  // отправка текста в stdin mystem

        // чтение вывода
        char buffer[4096];
        DWORD read;
        std::string result;

        while (true) {
            BOOL success = ReadFile(hChildStd_OUT_Rd, buffer, sizeof(buffer) - 1, &read, NULL);  // читается из stdout пайпа
            if (!success || read == 0) break;  // выход при ошибке или окончании потока
            buffer[read] = '\0';  // корректное завершение строки
            result += buffer;

            // mystem выводит по строкам. можно остановиться на первой строке.
            if (result.find('\n') != std::string::npos) break;  // останавливаемся после получения первой строки результата
        }

        return result;
    }

    ~MystemProcess() {
        CloseHandle(hChildStd_IN_Wr);      // закрытие дескриптора записи
        CloseHandle(hChildStd_OUT_Rd);     // закрытие дескриптора чтения

        TerminateProcess(piProcInfo.hProcess, 0);  // завершение дочернего процесса
        CloseHandle(piProcInfo.hProcess);          // освобождение дескриптора процесса
        CloseHandle(piProcInfo.hThread);           // освобождение дескриптора потока
    }
};

