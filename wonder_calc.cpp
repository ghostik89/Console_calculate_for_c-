//UnitFour.cpp
//Calculator
#define _USE_MATH_DEFINES  //для использования математических констант

#include <cmath>      //для математических функций
#include <iostream>   //стандартный ввод и вывод
#include <cstdlib>    //для функции exit()
#include <cctype>     // для функции isdigit()
#include <cstring>    //для функций со строками

using namespace std;

void eatspaces(char* str);			//функция удаления пробелов
double expr(char* str);				//функция вычисляющая выражение
double term(char* str, int& index);	//фунция для анализа элемента
double number(char* str, int& index);//функция, распознающая число
char* extract(char* str, int& index);//функция для извлечение подстроки
void error (int index);              //указатель на ошибки
double mathematic(char* op, double value); //для вычисления косинуса, синуса и т.д.

const double degree = 180.0 / M_PI; //градусы
const int MAX(80);					//максимальная длина выражения, включая '/0'

char input[MAX]; 
int inputIndex(0);
int main()
{	
	char buffer[MAX] = {0};//область вычисляемого выражения
	
	cout<<endl
		<<"Welcome!\n Enter clear line, if you want to exit\n";
	
	for(;;)
	{
		cin.getline(buffer,sizeof buffer);//читать входную строку
		eatspaces(buffer);
		strcpy_s(input, buffer);
		inputIndex = 0;
		
		if(!buffer[0]) 
			return 0;//пустая строка -  признак завершения работы
		
		cout<<"\t = "<<expr(buffer)<<endl;//вывести значение выражения
	}
}

void eatspaces(char* str)
{
	int i(0);
	int j(0); //индексы куда копировать и откуда копировать

	while((*(str + i) = *(str + j++)) != '\0')  // Loop while character
                                               // copied is not \0
    if(*(str + i) != ' ')                    // Increment i as long as
         i++;                                  // character is not a space
	return;
}

double expr(char* str)
{
	double value(0.0);//здесь хранится результат
	int index(0);//текущая позиция символа

	value = term(str, index);// получить первый элелмент
	
	for(;;)
	{
		switch(*(str + index++))//выбрать дейстие на основе текущего символа
		{
		  case'\0': 
			  return value;//найлен конец строки, поэтому возвращаем то, что имеем
		  case '+': 
			value += term(str,index);//найден знак + сложение следующего элемента
            break;
		  case '-':
			  value -= term(str, index);//найден знак - => вычесть
			  break;
		  default:
			  cout<<endl
				  <<"Agh!&# Error!\n";
			  error(index);
			  exit(1);
		}
	}
}

double term(char* str, int& index)//функция, вычисляющая значение элемента
{
	double value(0.0);//здесь накапливается значение результата
	
	value = number(str, index);

	while(true)
	{
		if(*(str + index) == '*')//если это знак умножения, умножить на следующее число
			value *= number(str, ++index);
		
		else if(*(str + index) == '/') //если знак деления, то разделить на следующее число
			value /= number(str, ++index);

		else if(*(str + index) == '^')//возведение в степень доделать!!!
			value = pow(value, number(str, ++index));
		
		else break;
	}
	return value;
}

double number(char* str, int& index)
{
	double value(0.0);//хранит результирующее значение

	char op[6];
	int ip = 0;
	while(index < 6 && isalpha(*(str + index)))
		op[ip++] = *(str + index++);
	op[ip] = '\0';

	if(*(str + index) == '(')
	{
		int oldIndex = inputIndex;
		inputIndex += index + 1;
		char* psubstr(nullptr);   //указатель выражения-подстроки
		psubstr = extract(str, ++index);//извлечь строку в скобках
		value = expr(psubstr);//получить значение выражения-подстроки

		if(op[0])
			value = mathematic(op, value);

		delete[]psubstr;
		inputIndex = oldIndex;
		return value;//вернуть значение подстроки
	}
	
	if(!isdigit(*(str + index)))//если цифры нет
	{
		cout<<endl
			<<"Aghr!&# There's an error!\n";
		error(index);
		exit(1);
	}
	while(isdigit(*(str + index)))//цикл накапливает ведущие цифры
		value = 10*value + (*(str + index++) - '0');
	
	if (*(str+index) != '.')//если не цифра. то проверяем на десятичную точку
		return value;
	
	double factor(1.0);//множитель для десятичных разрядов
	while(isdigit(*(str + (++index))))
	{
		factor *= 0.1;//умeньшить в 10 раз
		value = value + (*(str + index)- '0')*factor;//добавить десятичную позицию
	}
	return value;
}

//функция для извлечения подстроки, заключенной
//в скобки
char* extract(char* str, int& index)
{
	char buffer[MAX];   //временное пространство для подстроки
	char* pstr(nullptr);//указатель на новую строку для возраста
	int numL(0);        //счетчик найденных левых скобок
	int bufindex(index);//сохранить начальное значение index

	do
	{
		buffer[index - bufindex] = *(str + index);
		switch(buffer[index - bufindex])
		{
	        case ')':
				if(0 == numL)
				{
					buffer[index - bufindex] = '\0'; //заменить ')'
					++index;                         //на '\0'
					pstr = new char [index - bufindex];
					if(!pstr)
					{
						cout<<"Program broken :'(\n";
						exit(1);
					}
					strcpy_s(pstr, index - bufindex, buffer);//копировать подстроку в новую память

					return pstr;
				}
				else numL--;
				break;

		    case '(': numL++;
					break;
		}

	}while(*(str + index++) != '\0');

	error(0);
	cout << "Program broken :'(\n";
	exit(1);
}

void error(int index)
{
	cout << input << endl;
	for(int i = 0;i < inputIndex + index;i++)
		cout <<' ';
	cout << '^' << endl;
}

double mathematic(char* op, double value)
{
	if(!_stricmp(op,"sin"))			//если найден в строке sin
		return sin(value);
	else if(!_stricmp(op,"sind"))
		return sin(value/degree);
	else if(!_stricmp(op,"cos"))
		return cos(value);
	else if(!_stricmp(op,"cosd"))
		return cos(value/degree);
	else if(!_stricmp(op,"tg"))
		return tan(value);
	else if(!_stricmp(op,"tgd"))
		return tan(value/degree);
}