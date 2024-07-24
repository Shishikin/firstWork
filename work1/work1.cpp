#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <cstdint>
#include <climits>

#ifdef _WIN32
#include <windows.h>
#endif

double StringToDouble(std::string& str)
{
    // если строчка пустая возвращаем значение INT_MAX
    return str == "" ? INT_MAX : std::stod(str);
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
        out << dateTime.year << '.' << dateTime.month << '.' << dateTime.day;
        if (!(dateTime.hour >= 24 || dateTime.hour < 0))
        {
            if (dateTime.minute >= 60 || dateTime.minute < 0)
            {
                out << '\t' << dateTime.hour;
            }
            else
            {
                out << '\t' << dateTime.hour << ':' << dateTime.minute;
            }
        }
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
            if (a != INT_MAX)
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
    void Decade();
    void NoTime();
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
            if (a != INT_MAX)
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

void FormatInputFileLine(std::string& str)
{
    std::replace(str.begin(), str.end(), ',', '.');
    std::replace(str.begin(), str.end(), ' ', '\t');
}

// функция хранит значение
void PushDateTimeValueToMapDays(std::map <DateTime, Days>& days, Days day, const DateTimeValue& a)
{
    if (days.count(a.dateTime))
    {
        for (int i = 0; i < a.value.size(); ++i)
        {
            if (std::abs(a.value[i] - INT_MAX) > 10000)
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
            if (std::abs(INT_MAX - a.value[i]) < 10000)
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
                a.second.value[i] = INT_MAX;
            }
        }
    }
}

void DateTimeValue::NoTime()
{
    dateTime.hour = 10000;
    dateTime.minute = 10000;
}

void DateTimeValue::Decade()
{
    dateTime.hour = 10000;
    dateTime.minute = 10000;
    //    a.dateTime.day = a.dateTime.day / 10 < 2 ? a.dateTime.day / 10 + 1 : 2 + 1;
    if (dateTime.day / 10 < 2)
    {
        if (dateTime.day != 10)
        {
            dateTime.day = dateTime.day / 10 * 10 + 1;
        }
        else
        {
            dateTime.day = 1;
        }
    }
    else
    {
        if (dateTime.day != 20)
        {
            dateTime.day = 21;
        }
        else
        {
            dateTime.day = 11;
        }
    }
}

void VectorToMapAverageDays(std::map <DateTime, Days>& days,
    std::vector <DateTimeValue>& dateTimeValueVector,
    void (DateTimeValue::*func_ptr)())   // передача в качестве аргумента указатель на метод класса
{
    Days day = Days(dateTimeValueVector[0].value.size());
    for (auto& a : dateTimeValueVector)
    {
        (a.*func_ptr)();  // использование указателя на метод класса
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
    getline(std::cin, pathFileOutput);
 //   std::cin >> pathFileOutput;
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
    std::cout << "Введите путь к файлу из которого, будет извлекаться информация (он должен быть без кавычек)\n";
    std::getline(std::cin, pathFileInput);


    in.exceptions(std::ifstream::badbit);
    try
    {
        in.open(pathFileInput);
        // Ваш код обработки файла...
    }
    catch (const std::ifstream::failure& ex)
    {
        std::cerr << "Исключение при работе с файлом: " << ex.what() << std::endl;
        return 1;
    }

    std::vector <DateTimeValue> dateTimeValueVector;

    // заполнение данными из файла в vector
    std::string str;
    while (getline(in, str))
    {
        if (str.length() > 8)
        {
            DateTimeValue testDateTimeValue;
            FormatInputFileLine(str);
            testDateTimeValue.InputFileText(str);
            dateTimeValueVector.push_back(testDateTimeValue);
        }
    }

    in.close();

    std::map <DateTime, Days> days;
    VectorToMapAverageDays(days, dateTimeValueVector, &(DateTimeValue::NoTime));

    std::vector <DateTimeValue> daysVector;
    MapToVector(daysVector, days);

    OutputFile(daysVector, "Введите путь к файлу вывода среднесуточных значений (он должен быть без кавычек)");

    std::map <DateTime, Days> decadeMap;
    VectorToMapAverageDays(decadeMap, daysVector, &(DateTimeValue::Decade));
    std::vector <DateTimeValue> decadeVector;
    MapToVector(decadeVector, decadeMap);

    OutputFile(decadeVector, "Введите путь к файлу вывода среднедекадных значений (он должен быть без кавычек)");

    return 0;
}