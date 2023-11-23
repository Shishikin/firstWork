// work1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

// #define INFINITY
#define MANY 100000000110
#define MAX 10

struct Time
{
    std::string time = "";
};

struct Data
{
    std::string data = "";
};

struct Value
{
    int n;
    std::vector <double> value;
    // конструктор
    Value(int n_) : n(n_)
    {
        for (int i = 0; i < n_; ++i)
        {
            value.push_back(0);
        }
    }
    Value()  
    {
        n = MAX;
        for (int i = 0; i < MAX; ++i)
        {
            value.push_back(5);
        }
    }
};

struct StringValue
{
    int n;
    std::vector <std::string> stringValue;
    // конструктор
    StringValue(int n_) : n(n_)
    {
        std::string a = "";
        for (int i = 0; i < n_; ++i)
        {
            stringValue.push_back(a);
        }
    }
    StringValue()
    {
        n = MAX;
        std::string a = "";
        for (int i = 0; i < MAX; ++i)
        {
            stringValue.push_back(a);
        }
    }
};


// промежуточная структура все данные в виде строк
// поля имеют смысл, хотя мне кажется это лишнее
struct Test : public Time, public Data, public StringValue 
{ 
    Test(int n_) : StringValue(n_)
    {
    }
};

struct TimeDate : public Data, public Time, public Value 
{
    TimeDate(int n_): Value(n_)
    {}
};

struct Days : public Value
{
    std::vector <int> number;        // вектор знаменателей среднего значения 
    Days(int n_) : Value(n_)
    {
        for (int i = 0; i < n_; ++i)
        {
            number.push_back(0);
        }
    }    
    Days() : Value()
    {
        for (int i = 0; i < MAX; ++i)
        {
            number.push_back(0);
        }
    }
};

// в зависимости от счётчика (count) прибавляемк строке символ
void ChoosePluseStringOfTest(char a, struct Test& testStruct, int count )
{
    if (count == 0)
    {
        testStruct.data.push_back(a);
    }
    else
    {
        if (count == 1)
        {
            testStruct.time.push_back(a);
        }
        else
        {
            testStruct.stringValue[count - 2].push_back(a);
        }
    }
}

// заполнение промежуточной структуры вектора осмысленными значениями
void FillstructTest(int number, struct Test& testStruct, std::string test)
{
    int count = 0;
    for (auto& a : test)
    {
        if (a == ',')
        {
            a = '.';
        }
        if (a == ' ' || a == '\t')
        {
            count++;            // увелечение счётчика
        }
        else
        {
            ChoosePluseStringOfTest(a, testStruct, count);
        }
    }
}

// перводит точку в запятую
std::string PointToComma(std::string str)
{
    std::string resultString;
    for (auto& a : str)
    {
        if (a == '.')
        {
            a = ',';
        }
        resultString.push_back(a);
    }
    return resultString;
}

// перевод сроки к double
double StringToDouble(std::string str)
{
    // если строчка пустая возвращаем значение Many
    if (str == "")
    {
        //        return INFINITY;
        return MANY;
    }
    else
    {
        for (auto& a : str)
        {
            return std::stod(str);
        }
    }
}

void PrintStructStringValue(StringValue& testStruct)
{
    for (auto& a : testStruct.stringValue)
    {
        std::cout << '\t' << a;
    }
    std::cout << '\n';
}

void PrintStructValue(Value& testStruct)
{
    for (auto& a : testStruct.value)
    {
        std::cout << '\t' << a;
    }
    std::cout << '\n';
}

void PrintStructData(Data& testStruct)
{
    std::cout << testStruct.data << '\t';
}

void PrintStructTime(Time& testStruct)
{
    std::cout << testStruct.time;
}


void PrintStructTest(Test& testStruct)
{
    PrintStructData(testStruct);
    PrintStructTime(testStruct);
    PrintStructStringValue(testStruct);
}


void TestToTimeDate(Test& test, TimeDate& timeDate)
{
    timeDate.data = test.data;
    timeDate.time = test.time;
    int count = 0;
    for (auto& a : test.stringValue)
    {
        timeDate.value[count] = StringToDouble(test.stringValue[count]);
        count++;
    }
}

void PrintTimeDate(TimeDate& testStruct)
{
    PrintStructData(testStruct);
    PrintStructTime(testStruct);
    PrintStructValue(testStruct);
}

void DoubleValueToDoubleValue(Value& testStruct, Value& result)
{
    int count = 0;
    for (auto& a : testStruct.value)
    {
        result.value[count] = a;
        count++;
    }
}



/*

void PrintDays(Days &testStruct)
{
    std::cout << testStruct.precipitation << '\t';
    std::cout << testStruct.temperature << '\t';
    std::cout << testStruct.humidity << '\t';
    std::cout << testStruct.windSpeed << '\n';
}

void PrintBaseDays(BaseDays testStruct)
{
    std::cout << testStruct.data << '\t';
    std::cout << testStruct.precipitation << '\t';
    std::cout << testStruct.temperature << '\t';
    std::cout << testStruct.humidity << '\t';
    std::cout << testStruct.windSpeed << '\n';
}
*/
int main()
{
    
    int n = 4;  // выбор числа элементов
    std::vector <Test> testStruct;  //создание вектора промежуточных структур
    std::vector <TimeDate> timeDateStruct;
    TimeDate testtimeDateStruct = TimeDate(n);
    Test testingStruct = Test(n);     // структура, которая нужна для того чтобы просто забить вектор
    std::ifstream in;
    std::ofstream out;
    std::ofstream fout;
    fout.open("result2.txt");
    out.open("bigresult.txt");
    std::string nameFile = "onefile.txt";
    in.open(nameFile);
    std::string testString;
    
    if (in.is_open())
    {
        // заполнение вектора промежуточных структур,
        //содержащих все данные в виде строк
        int i = 0;
        while (!in.eof())
        {
            getline(in, testString);
            testStruct.push_back(testingStruct);
            timeDateStruct.push_back(testtimeDateStruct);
            FillstructTest(i, testStruct[i], testString);
            PrintStructTest(testStruct[i]);
            TestToTimeDate(testStruct[i], timeDateStruct[i]);
            PrintTimeDate(timeDateStruct[i]);
            i = i + 1;
        }
        
        std::map <std::string, Days> days;
        Days day = Days(n);
        for (auto& a : timeDateStruct)
        {
            
            if (a.data != "")
            {
                // функцию мне кажется ао доработатьь
                
                DoubleValueToDoubleValue(a, day);
                
                if (days.count(a.data))
                {
 //                   std::cout << "ntr\n"
                    
                    for (int i = 0; i < n; ++i)
                    {
                        if (std::abs(day.value[i] - MANY) > 10000)
                        {                            
                            days[a.data].value[i] = days[a.data].value[i] + day.value[i];
                            days[a.data].number[i] = days[a.data].number[i] + 1;
//                            std::cout << days[a.data].number[i];
                        }
                    }
                    
                }
                else
                {
                    
                    // ошибка если первый элемент имеет значение равное MANY. то он попадает сюда
                    
                    for (int i = 0; i < n; ++i)
                    {
                        if (std::abs(MANY-day.value[i]) < 10000)
                        {

//                            day.number[i] = 1;
                            day.value[i] = 0;
                        }
                        else
                        {
//                            day.value[i] = 0;
                            day.number[i] = 1;
                            std::cout << "!!!" << '\t';
                        }
                        
                    }
                    days.insert({ a.data, day });
//                    PrintStructValue(days[a.data].value);
//                    days[a.data];
                    out << a.data << "\t" << days[a.data].value[0] << "\n";
                }
                
                


            }
            
        }
        
        for (auto& a : days)
        {
            for (int i = 0; i < n; ++i)
            {
                if (a.second.number[i] != 0)
                {
                    a.second.value[i] = a.second.value[i] / a.second.number[i];
                }
                else
                {
                    
                    a.second.number[i] = MANY;
                }
            }
            
            out << a.first;
            StringValue str = StringValue(n);
            for (int i = 0; i < n; ++i)
            {
                str.stringValue[i] = std::to_string(a.second.value[i]);
                str.stringValue[i] = PointToComma(str.stringValue[i]);
                out << '\t' << str.stringValue[i];
            }
            out << '\n';
            
        }
        
        
    }
    else
    {
        std::cout << "error";
    }
    
    std::cout << "Hello World!\n";
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
