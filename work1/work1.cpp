// work1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>       


#ifdef _WIN32
#include <windows.h>
#endif

#define MANY 100000110

double StringToDouble(std::string& str)
{
    // если строчка пустая возвращаем значение Many
    return str == "" ? MANY : std::stod(str);
}

// структура для хранения времени и даты
struct DateTime
{
    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int minute = 0;

    // оператор меньше требуется для std::map
    bool operator <(const DateTime& other) const
    {
        return year < other.year
            || ((year == other.year) && (month < other.month))
            || ((year == other.year) && (month == other.month) && (day < other.day))
            || (((year == other.year) && (month == other.month) && (day == other.day)) && (hour < other.hour))
            || ((((year == other.year) && (month == other.month) && (day == other.day)) && (hour == other.hour)) && (minute < other.minute));
    }

    // оператор вывода
    friend std::ostream& operator << (std::ostream& out, const DateTime& dateTime)
    {
        out << dateTime.year << '.' << dateTime.month << '.' << dateTime.day << '\t';
        out << dateTime.hour << ':' << dateTime.minute;
        return out;
    }

    void GetlineToInt(int& value, std::stringstream& inDate, char delimiter)
    {
        std::string str = "";
        getline(inDate, str, delimiter);
        value = std::stoi(str);
    }

    void InputFileText(std::string& strDate, std::string& strTime)
    {

        std::stringstream inDate(strDate);
        std::stringstream inTime(strTime);
        GetlineToInt(day, inDate, '.');
        GetlineToInt(month, inDate, '.');
        GetlineToInt(year, inDate, '.');                
        GetlineToInt(hour, inTime, ':');
        GetlineToInt(minute, inTime, ':');
    }

};


struct DateTimeValue
{
    DateTime dateTime;
    std::vector <double> value;

    friend std::ostream& operator << (std::ostream& out, const DateTimeValue& dateTimeValue)
    {
        out << dateTimeValue.dateTime;
        for (auto a : dateTimeValue.value)
        {
            if (a != MANY)
            {
                out << '\t' << a;
            }
            else
            {
                out << '\t' << "";
            }
        }
        return out;
    }

    void InputFileText(std::string& strInput)
    {
        strInput.push_back('\t');
        std::string str = "";
        std::string strDate;
        std::string strTime;
        std::stringstream instr(strInput);
        getline(instr, strDate, '\t');
        getline(instr, strTime, '\t');
        dateTime.InputFileText(strDate, strTime);
        while (getline(instr, str, '\t'))
        {
            value.push_back(StringToDouble(str));
        }
    }

};


// показания метеостанций структура для передачи в map
struct Days
{
    std::vector <double> value;
    std::vector <int> number;        // вектор знаменателей среднего значения 
    Days(int n_)
    {
        for (int i = 0; i < n_; ++i)
        {
            value.push_back(0);
            number.push_back(0);
        }
    }
    Days()
    {
        value.push_back(0);
        number.push_back(0);
    }
    friend std::ostream& operator << (std::ostream& out, const Days& dateTimeValue)
    {
        for (auto a : dateTimeValue.value)
        {
            if (a != MANY)
            {
                out << a << '\t';
            }
            else
            {
                out << '\t' << "";
            }
        }
        return out;
    }
};


//функция для выведения std::map < DateTime, double> в консоль
void PrintDateTimeAndOneDouble(std::map < DateTime, double> wec)
{
    for (auto& a : wec)
    {
        std::cout << a.first << '\t';
        std::cout << a.second << '\n';
    }
}

char CommaToPoint(char a)
{
    return a == ',' ? '.' : a;
}

char SpaceToTab(char a)
{
    return a == ' ' ? '\t' : a;
}

void FormatInputFileLine(std::string& str)
{
    for (auto& a : str)
    {
        a = CommaToPoint(a);
        a = SpaceToTab(a);
    }
}

// функция хранит значение
void PushDateTimeValueToMapDays(std::map <DateTime, Days>& days, Days day, const DateTimeValue& a)
{
    if (days.count(a.dateTime))
    {
        for (int i = 0; i < a.value.size(); ++i)
        {
            if (std::abs(a.value[i] - MANY) > 10000)
            {
                days[a.dateTime].value[i] = days[a.dateTime].value[i] + a.value[i];
                days[a.dateTime].number[i] = days[a.dateTime].number[i] + 1;
            }
        }
    }
    else
    {
        for (int i = 0; i < a.value.size(); ++i)
        {
            if (std::abs(MANY - a.value[i]) < 10000)
            {
                day.number[i] = 0;
                day.value[i] = 0;

            }
            else
            {
                day.value[i] = a.value[i];
                day.number[i] = 1;
            }
        }
        days.insert({ a.dateTime, day });
    }
}

void AverageMapDays(std::map <DateTime, Days>& days)
{
    for (auto& a : days)
    {
        for (int i = 0; i < a.second.value.size(); ++i)
        {
            if (a.second.number[i] != 0)
            {
                a.second.value[i] = a.second.value[i] / a.second.number[i];
                a.second.number[i] = 0;
            }
            else
            {
                a.second.value[i] = MANY;
            }
        }
    }
}

DateTimeValue NoTime(DateTimeValue a)
{
    a.dateTime.hour = 10000;
    a.dateTime.minute = 10000;
    return a;
}

DateTimeValue Decade(DateTimeValue a)
{
    a.dateTime.hour = 10000;
    a.dateTime.minute = 10000;
//    a.dateTime.day = a.dateTime.day / 10 < 2 ? a.dateTime.day / 10 + 1 : 2 + 1;
    if (a.dateTime.day / 10 < 2 )
    {
        if (a.dateTime.day  != 10)
        {
            a.dateTime.day = a.dateTime.day / 10 * 10 + 1;
        } 
        else
        {
            a.dateTime.day = 1;
        }
    }
    else
    {
        if (a.dateTime.day != 20)
        {
            a.dateTime.day = 21;
        }
        else
        {
            a.dateTime.day = 11;
        }
    }
    return a;
}

void VectorToMapAverageDays(std::map <DateTime, Days>& days,
           std::vector <DateTimeValue>& dateTimeValueVector,
    DateTimeValue(*func_ptr)(DateTimeValue))
{
    Days day = Days(dateTimeValueVector[0].value.size());
    for (auto& a : dateTimeValueVector)
    {
        a = func_ptr(a);
        PushDateTimeValueToMapDays(days, day, a);
    }
    AverageMapDays(days);
}

void MapToVector(std::vector <DateTimeValue>& daysVector, std::map <DateTime, Days> days)
{
    DateTimeValue  day;
    for (auto& a : days)
    {
        day.dateTime = a.first;
        day.value = a.second.value;
        daysVector.push_back(day);
    }
}

// открытие файла для вывода
void OutputFile(std::vector <DateTimeValue> daysVector, std::string message)
{
    std::ofstream out;
    std::string pathFileOutput;
    std::cout << message << '\n';
    std::cin >> pathFileOutput;
    out.open(pathFileOutput);
    for (auto& a : daysVector)
    {
        out << a << '\n';
    }
    out.close();
}

int main()
{
#ifdef _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#endif

    // открытия файла ввода
    std::ifstream in;
    std::string pathFileInput;
    std::cout << "InputFile\n";
    std::cin >> pathFileInput;


    in.exceptions(std::ifstream::badbit);
    try
    {
        in.open(pathFileInput);
        if (!in.is_open())
        {
            std::cout << "Ошибка при открытии с файлом";
            return 1;
        }
    }
    catch (const std::ifstream::failure& ex)
    {
        std::cout << "Ошибка при открытии файла";
        std::cout << ex.what();
        std::cout << ex.code();
        return 1;
    }

    std::vector <DateTimeValue> dateTimeValueVector;
    
    // заполнение данными из файла в vector
    std::string str;
    while (!in.eof())
    {
        getline(in, str);
        if (str != "")
        {
            DateTimeValue testDateTimeValue;
            FormatInputFileLine(str);
            testDateTimeValue.InputFileText(str);
            dateTimeValueVector.push_back(testDateTimeValue);
        }
    }

    in.close();

    std::map <DateTime, Days> days;
    VectorToMapAverageDays(days, dateTimeValueVector, &NoTime);

    std::vector <DateTimeValue> daysVector;
    MapToVector(daysVector, days);

    OutputFile(daysVector, "OutputFileDays");
    
    std::map <DateTime, Days> decadeMap;
    VectorToMapAverageDays(decadeMap, daysVector, &Decade);
    std::vector <DateTimeValue> decadeVector;
    MapToVector(decadeVector, decadeMap);

    OutputFile(decadeVector, "OutputFileDecade");
       
    return 0;
}




// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
