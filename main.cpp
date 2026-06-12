#include <iostream>
#include <cstring>
#include <cctype>
#include <cstdio>
#include "stockexchange.h"
#include "datetime.h"


using namespace DateTime;

const char* skipSpaces(const char* ptr) {
    while (*ptr == ' '  && *ptr != '\0') {
        ptr++;
    }
    return ptr;
}

bool isValidDate(int day, int month, int year) {

    if (month < 1 || month > 12) {
        std::cout << "Month value is invalid: " << month << "\n";
        return false;
    }
    
    if (day < 1 || day > 31) {
        std::cout << "Day value is invalid: " << day << "\n";
        return false;
    }

    if (month == 2) {
        bool is_leap_year = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if ((!is_leap_year && day > 28) || (is_leap_year && day > 29)) {
            std::cout << "Day value is invalid: " << day << "\n";
            return false;
        }
    }

    return true;
}

bool isValidTime(int hours, int minutes, int seconds) {
    if (hours < 0 || hours > 23) {
        return false;
    }
    if (minutes < 0 || minutes > 59) {
        return false;
    }
    if (seconds < 0 || seconds > 59) {
        return false;
    }
    return true;
}

bool parseDateTime(const char* arr, datetime& dt) {
    
    int day;
    int month;
    int year;
    int hours;
    int minutes;
    int seconds;
    sscanf(arr, "%d.%d.%d %d:%d:%d", &day, &month, &year, &hours, &minutes, &seconds);

    char date[11] = { 0 };
    char time[9] = { 0 };
    int i_date = 0;
    int i_time = 0;
    bool parsing_date = true;

    // Заполним массивы date и time для дальнейшего поиска ошибок
    for (int i = 0; arr[i] != '\0' && arr[i] != ','; ++i) {
        if (arr[i] == ' ' && parsing_date) {
            parsing_date = false;
            continue;
        }

        if (parsing_date) {
            if (i_date < sizeof(date) - 1) {
                date[i_date++] = arr[i];
            }
        }
        else {
            if (i_time < sizeof(time) - 1) {
                time[i_time++] = arr[i];
            }
        }
    }
    date[i_date] = '\0';
    time[i_time] = '\0';

    // Посчитаем количество точек, если их < 2, выведем ошибку 
    int point_count = 0;
    for (int i = 0; date[i] != '\0'; i++) {
        if (date[i] == '.') {
            ++point_count;
        }
    }

    if (point_count != 2) {
        std::cout << "Wrong date format: " << date << "\n";
        return false;
    }
    
    // Посчитаем количество двоеточий, если их < 2, выведем ошибку 
    int doublePoint_count = 0;
    for (int i = 0; time[i] != '\0'; i++) {
        if (time[i] == ':') {
            ++doublePoint_count;
        }
    }
    if (doublePoint_count != 2) {
        std::cout << "Wrong time format: " << time << "\n";
        return false;
    }

    if (!isValidDate(day, month, year)) {
        return false;
    }

    if (!isValidTime(hours, minutes, seconds)) {
        char time[9];
        time[0] = static_cast<char>((hours) / 10 + '0');
        time[1] = static_cast<char>((hours) % 10 + '0');
        time[2] = ':';

        time[3] = static_cast<char>((minutes) / 10 + '0');
        time[4] = static_cast<char>((minutes) % 10 + '0');
        time[5] = ':';

        time[6] = static_cast<char>((seconds) / 10 + '0');
        time[7] = static_cast<char>((seconds) % 10 + '0');
        time[8] = '\0';

        std::cout << "Time value is invalid: " << time << "\n";
        return false;
    }

    dt = datetime(year, month, day, hours, minutes, seconds);
    return true;
}

struct OrderStruct {
    DateTime::datetime dt;
    char ticker[16];
    bool is_buy = false;
    int amount;
    double price;
};

bool parseOrder(const char* command, OrderStruct& order) {
    const char* ptr = command + 3;
    ptr = skipSpaces(ptr);

    // Парсим дату и время
    if (!parseDateTime(ptr, order.dt)) {
        return false;
    }

    // Считываем до запятой и пропускаем лишние пробелы
    while (*ptr != ',' && *ptr != '\0') {
        ptr++;
    }
    if (*ptr != ',') {
        return false;
    }
    ptr++;
    ptr = skipSpaces(ptr);

    // Парсим тикер
    int tickerSize = 0;
    while (*ptr != ',' && *ptr != '\0' && tickerSize < sizeof(order.ticker) - 1) {
        order.ticker[tickerSize++] = *ptr++;
    }
    order.ticker[tickerSize] = '\0';

    // Считываем до запятой и пропускаем лишние пробелы
    while (tickerSize > 0 && (isspace(order.ticker[tickerSize - 1]) != 0)) {
        order.ticker[--tickerSize] = '\0';
    }
    if (*ptr != ',') {
        return false;
    }
    ptr++;
    ptr = skipSpaces(ptr);

    // Парсим тип заявки
    char type[6] = {};
    int typeSize = 0;
    while (*ptr != ',' && *ptr != '\0' && typeSize < sizeof(type) - 1) {
        if (isspace(*ptr)==0) {
            type[typeSize++] = static_cast<char>(tolower(*ptr));
        }
        ptr++;
    }
    type[typeSize] = '\0';


    if (strcmp(type, "buy") == 0) {
        order.is_buy = true;
    }
    else if (strcmp(type, "sell") == 0) {
        order.is_buy = false;
    }
    else {
        std::cout << "Unknown command: " << type << '\n';
        return false;
    }

    // Считываем до запятой и пропускаем лишние пробелы
    while (*ptr != ',' && *ptr != '\0') {
        if (isspace(*ptr)==0) {
            return false;
        }
        ptr++;
    }
    if (*ptr != ',') {
        return false;
    }
    ptr++;
    ptr = skipSpaces(ptr);

    // Парсим количество
    order.amount = 0;
    while (*ptr >= '0' && *ptr <= '9' && *ptr != '\0') {
        order.amount = order.amount*10 + (*ptr++ - '0');
    }

    if (*ptr == '\0') {
        std::cout << "Unknown command" << '\n';
    }
    ptr = skipSpaces(ptr);
    if (*ptr != ',') {
        return false;
    }
    ptr++;
    ptr = skipSpaces(ptr);
    if (*ptr == '\0') {
        std::cout << "Unknown command" << '\n';
    }

    // Парсим цену
    order.price = 0.0;
    if (*ptr != 0) {
        order.price = std::atof(ptr);
    }

    return true;
}

int main() {
    DataBase DB;
    DB.initDB();

    char command[128];
    while (true) {
        std::cout << "> ";
        std::cin.getline(command, 128);

        if (std::strcmp(command, "print") == 0) {
            DB.printDB();
        }
        else if (std::strncmp(command, "load", 4) == 0) {
            char DBfilename[16];
            if (sscanf(command + 5, "%15s", DBfilename) == 1) {
                DB.loadDB(DBfilename);
            }
        }
        
        // В основном цикле обработки команд:
        else if (std::strncmp(command, "add", 3) == 0) {
            OrderStruct order;
            if (parseOrder(command, order)) {

                ::ticker newTicker = {};

                //парсим вручную дату и время
                char* dt = newTicker.datetime;
                dt[0] = static_cast<char>((order.dt.day() / 10) + '0');
                dt[1] = static_cast<char>((order.dt.day() % 10) + '0');
                dt[2] = '.';

                dt[3] = static_cast<char>((order.dt.month() / 10) + '0');
                dt[4] = static_cast<char>((order.dt.month() % 10) + '0');
                dt[5] = '.';
                
                dt[6] = static_cast<char>((order.dt.year() / 1000) + '0');
                dt[7] = static_cast<char>(((order.dt.year() / 100) % 10) + '0');
                dt[8] = static_cast<char>(((order.dt.year() / 10) % 10) + '0');
                dt[9] = static_cast<char>((order.dt.year() % 10) + '0');
                dt[10] = ' ';
                
                dt[11] = static_cast<char>((order.dt.hour() / 10) + '0');
                dt[12] = static_cast<char>((order.dt.hour() % 10) + '0');
                dt[13] = ':';
                
                dt[14] = static_cast<char>((order.dt.minute() / 10) + '0');
                dt[15] = static_cast<char>((order.dt.minute() % 10) + '0');
                dt[16] = ':';
                
                dt[17] = static_cast<char>((order.dt.second() / 10) + '0');
                dt[18] = static_cast<char>((order.dt.second() % 10) + '0');
                dt[19] = '\0';

                
                //Скопируем заявку
                int index = 0;
                while(index < sizeof(newTicker.name) - 1 && order.ticker[index] != '\0') {
                    newTicker.name[index] = order.ticker[index];
                    ++index;
                }
                newTicker.name[index] = '\0';

                newTicker.is_buy = order.is_buy;
                newTicker.amount = order.amount;
                newTicker.price = static_cast<float>(order.price);

                DB.add(newTicker);
            }
        }
        else if (std::strncmp(command, "dom", 3) == 0) {
            char tickerName[17];
            if (sscanf(command + 4, "%16s", tickerName) == 1) {
                DB.domTicker(tickerName);
            }
        }
        else if (std::strncmp(command, "export", 6) == 0) {
            char filename[16];
            if (sscanf(command + 7, "%15s", filename) == 1) {
                DB.exportDB(filename);
            }
        }
        else if (std::strncmp(command, "save", 4) == 0) {
            char DBfilename[16];
            if (sscanf(command + 5, "%15s", DBfilename) == 1) {
                DB.saveDB(DBfilename);
            }
        }
        else if (std::strcmp(command, "quit") == 0) {
            break;
        }
        else if (std::strcmp(command, "clear") == 0) {
            DB.clear();
        }
        else if (std::strncmp(command, "delete", 6) == 0) {
            int index;
            if (sscanf(command + 7, "%d", &index) == 1) {
                Node* deletedIndex = DB.findRecord(index);
                if (deletedIndex != nullptr) {
                    DB.deleteID(deletedIndex);
                }
            }
        }
        else {
            int i = 0;
            while (command[i] != ' ') {
                ++i;
            } 
            std::cout << "Unknown command: ";
            for (int j = 0; j < i; ++j) {
                std::cout << command[j];
            }
            std::cout << '\n';
        }
    }

    return 0;
}
