

# CppSamples
Small cpp Examples


# CppSamples
Small cpp Examples


https://www.tutorialspoint.com/cplusplus/cpp_questions_answers.htm

C++ Interview Questions 

https://www.tutorialspoint.com/cplusplus/cpp_interview_questions.htm



# Optimization

### for cycle 

(*) ATTENTION ... Do NOT Use if you have to check size of the Array every time

```
// Works // Old but Gold
std::vector vec;

for(int i=0; i < vec.size(); ++i)
{
  int b = vec[i];
}

```

```
// A bit faster // Micro optimisation
for(int i=0, ilen = vec.size(); i < ilen; ++i)
{
  int b = vec[i];
}
```




### operator <

```
// Works // Old but Gold
bool compareFunc(const Param& lhs, const Param& rhs)
{
  bool res = false;
  if (lhs.param_1 < rhs.param_1)     { res = true; } else if(lhs.param_1 == rhs.param_1)
  {  if (lhs.param_2 < rhs.param_2 ) { res = true; } else if(lhs.param_2 == rhs.param_2)
    {  ...
      {
        res = lhs.param_N < rhs.param_N;
      }
    }
  }
  return res;
}
```

```
// Better
bool compareFunc(const Param& lhs, const Param& rhs)
{
  bool res = false;
  for (int i = 0, ilen = lhs.size(); i < ilen; i++)
  {
    if (lhsStr[i] < rhsStr[i])
    {
      res = true;
      break;
    }
    else if (lhsStr[i] == rhsStr[i])
      continue;
    else
      break;
  }
  return res;
}
```

```
// The Best
bool compareFunc(const Param& lhs, const Param& rhs)
{
  bool res = false;
  for (int i = 0, ilen = lhsStr.size(); i < ilen; i++)
  {
    if (lhsStr[i] == rhsStr[i])
      continue;

    res = lhsStr[i] < rhsStr[i];
    break;
  }
  return res;
}
```


### MUTABLE Relevant example (mutable mutex)

```
class MyClass : public QObject
{
  Q_OBJECT
public:
  MyClass();
  ~MyClass();
  state getState() const;
  
private:
  mutable QMutex _internalLock;
  States _state;
};
  
MyClass::MyClass(QObject* parent) : _internalLock {}
{
}

state MyClass::getState() const
{
  QMutexLocker locker(&_internalLock);
  return _state;
}
```


### Initialization over constructor with parameters

- Allows to write less code inside the body of the function
- const variables can be initialized in constructor and not inside the body of the class
- References can be passed only like that and not any other way

```
// Bad Solution

class MyClass
{
public:
  MyClass(int a, double d, SubClass& s)
  {
    _a = a;
    _d = 0.1; // (*) IMpossible here
    _s = s; // ???
  };
  
private:
  int _a;
  const double _d; // = 0.1; // (**) possible but not so good
  Subclass _s;
};
```

```
// Reason Solution and Best Strategy

class MyClass
{
public:
  MyClass(int a, double d, SubClass& s) : _a(a), _d(0.1), _s(s)
  { };
  
private:
  int _a;
  const double _d;
  Subclass _s;
};
```


### Problems

- access violation
- devision by zero
- out of range of array / memory


### Pure WinAPI solutions


### Lambda

```

[=](float a, float b) {
            return (std::abs(a) < std::abs(b));
        } // end of lambda expression

[](float a, float b) {
            return (std::abs(a) < std::abs(b));
        } // end of lambda expression

[&](float a, float b) {
            return (std::abs(a) < std::abs(b));
        } // end of lambda expression

[this](float a, float b) {
            return (std::abs(a) < std::abs(b));
        } // end of lambda expression

[this&](float a, float b) {
            return (std::abs(a) < std::abs(b));
        } // end of lambda expression

```

+ ЗАМЫКАНИЯ



https://github.com/tumagonx/portabat


# QT to STD replacements

```
#include <vector>
#include <algorithm>

	template <class T> bool containsForStdVector ( std::vector<T> & v, T isThisIn)
	{
		//if (!v.contains(uaBrowseFolderNodeId.identifier.numeric)) // QT_VER
		return std::find(v.begin(), v.end(), isThisIn) != v.end();
	}
  
```


```
#include <random>
#include <iostream>
#include <sstream>


		unsigned int random_char()
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(0, 255);
			return dis(gen);
		}

		std::string generate_hex(const unsigned int len)
		{
			std::stringstream ss;
			for (unsigned int i = 0; i < len; i++)
			{
				const auto rc = random_char();
				std::stringstream hexstream;
				hexstream << std::hex << rc;
				auto hex = hexstream.str();
				ss << (hex.length() < 2 ? '0' + hex : hex);
			}
			return ss.str();
		}

		void toUpper(std::string& s)
		{
			std::transform(s.begin(), s.end(), s.begin(), toupper);
		}

		void toLower(std::string& s)
		{
			std::transform(s.begin(), s.end(), s.begin(), tolower);
		}

		std::string uuidStdGenerate()
		{
			std::string strUUID = generate_hex(32 / 2);
			toUpper(strUUID);
			return strUUID;
		}
```

# Debug stuff by writing to a file with QT

```
	
	QString filename = "C:\\folder\\test.txt";
	QFile inFile(filename);
	inFile.open(QIODevice::ReadOnly | QIODevice::Text);
	QByteArray inputData = inFile.readAll();
	inFile.close();

	QFile file(filename);
	file.open(QIODevice::WriteOnly);
	QTextStream out(&file);
	out << inputData;
	out << "Thomas M. Disch: " << 334 << endl;
	file.close();

	/*
		QFile _file(filename);
		_file.cre
		QTextStream out(&_file);
		QString str = "123\n";
		out << str << endl;
	*/
```

# Виртуальные функции и деструктор

- Вритуальные методы в Конструкторе
При вызове виртуальных методов из коснутруктора используется **ПОЗДНЕЕ СВЯЗЫВАНИЕ**, как следствие мы не сможем вызвать . 

- Вритуальные методы в Деструкторе
При вызове виртуальных методов из деструктора компилятор использует не позднее, а **РАННЕЕ СВЯЗЫВАНИЕ**. 

https://habr.com/ru/post/64280/

# Исключения в контрукторах и деструкторах

- Исключени в Конструкторе
Если исключение не обработано, то c логической точки зрения разрушается объект, который еще не создан, а с технической, так как он еще не создан, то и деструктор этого объекта не будет вызван.
Если конструктор класса завершает работу исключением, значит он не завершает свою работу — следовательно объект не будет создан. Из-за этого могут возникать утечки памяти, т.к. для не полностью сконструированных объектов не будет вызван деструктор. Из-за этого распространено мнение, что конструктор никогда не должен вырабатывать исключения, однако это не так — утечки памяти возникнут не во всех случаях.

Стандарт языка С++ гарантирует, что если исключение возникнет в конструкторе, то памяти из под членов-данных класса будет освобождена корректно вызовом деструктора — т.е. если вы используете идиому RAII [2], то проблем не будет. Часто для этого достаточно использовать std::vector/std::string вместо старых массивов и строк, и умные указатели вместо обычных [3]. Если же вы продолжите использовать сырые указатели и динамически выделять память — нужно будет очень тщательно следить за ней, например в следующем фрагменте кода нет утечки, т.к. исключение будет выработано только если память не будет выделена [4]:


- Исключени в Деструкторе
Необработанное исключение в деструкторе вызовет размотку стека, что может привести к тому, что деструктор не выполнится полностью и объект полноценно не очстится из памяти +прочее неконтролируемое поведение. 

+ Нельзя бросать исключения во время обработки другого исключения (когда происходит раскрутка стека) — это приведет к аварийному завершению работы программы (фактически вызову abort()).

https://pro-prof.com/forums/topic/constructor_destructor_exceptions


# lvalue и rvalue

- Lvalue (locator value) представляет собой объект, который занимает идентифицируемое место в памяти (например, имеет адрес).

int var;
var = 4;

- Rvalue (locator value) это выражение, которое не представляет собой объект, который занимает идентифицируемое место в памяти. (оно, не имеет адрес).

4 = var;       // ERROR!
(var + 1) = 4; // ERROR!

https://habr.com/ru/post/348198/

# Copy Constructor

- при передаче объекта класса в функцию, как параметра по значению (а не по ссылке);
- при возвращении из функции объекта класса, как результата её работы;
- при инициализации  одного объекта класса другим объектом этого класса.

# Что такое таблица виртуальных функций 

https://tproger.ru/problems/working-of-the-virtual-functions-in-c-plus-plus/

Виртуальная функция определяется vtable (виртуальной таблицей). Если какая-либо функция класса объявлена как виртуальная, создастся vtable, которая хранит адреса виртуальных функций этого класса. Для всех таких классов компилятор добавляет скрытую переменную vptr, которая указывает на vtable. Если виртуальная функция не переопределена в производном классе, vtable производного класса хранит адрес функции в родительском классе. Таблица vtable используется для получения доступа к адресу при вызове виртуальной функции. Механизм vtable позволяет реализовать динамическое связывание в C++.


Когда мы связываем объект производного класса с указателем базового класса, переменная vptr указывает на vtable производного класса. Это присвоение гарантирует, что будет вызвана нужная виртуальная функция.


# ТИПЫ С/CPP/WIN char, bool и BOOL
```
int sz = 0;
sz = sizeof(char); // = 1
sz = sizeof(wchar_t); // = 2
sz = sizeof(TCHAR); // = 2 typedef wchar_t WCHAR;

sz = sizeof(bool); 
sz = sizeof(BOOL); // typedef int BOOL;
```

# extern C
```
extern "C" {
#include <foo.h>
}

extern "C" {
    void foo() { }
}

 U __gxx_personality_v0
0000000000000000 T foo
```
вы получаете с рычага. Имя функции " foo "в объектном файле-это просто" foo", и у него нет всей причудливой информации о типе, которая исходит из искажения имени.

The extern "C" {} construct указывает компилятору не выполнять искажение имен, объявленных в фигурных скобках. Обычно компилятор C++ "улучшает" имена функций, чтобы они кодировали информацию о типе аргументов и возвращаемое значение; это называется исковеркали имя. Элемент extern "C" конструкция предотвращает искажение.

он обычно используется, когда код C++ должен вызывать библиотеку языка C. Он также может использоваться при предоставлении функции C++ (из DLL, для примеру) для клиентов c.


# extern C

вы должны использовать extern " C " в любое время, когда вы включаете функции определения заголовка, находящиеся в файле, скомпилированном компилятором C, используемом в файле C++. (Многие стандартные библиотеки C могут включать эту проверку в свои заголовки, чтобы упростить ее для разработчика)

например, если у вас есть проект с 3 файлами, util.c, util.ч, а основного.cpp и как .с и. файлы cpp компилируются с помощью компилятора C++ (g++, cc и т. д.), Тогда он действительно не нужен и может даже вызвать ошибки компоновщика. Если ваш процесс сборки использует обычный компилятор C для util.c, тогда вам нужно будет использовать extern "C" при включении util.з.

происходит то, что C++ кодирует параметры функции в ее имени. Вот как работает перегрузка функций. Все, что обычно происходит с функцией C, - это добавление подчеркивания ("_") к началу имени. Без использования extern "C" компоновщик будет искать функцию с именем DoSomething@@int@float () когда фактическое имя функции это _DoSomething () или просто DoSomething ().

использование extern "C" решает вышеуказанную проблему, сообщая компилятору C++, что он должен искать функцию, которая следует соглашению об именовании C, а не C++.


-----------------------------------------------
### Q: Какие типы умных указателей вы знаете ?
A: unique_ptr, shared_ptr, weak_ptr 

(auto_ptr, scoped_pointer, ... что-то ещё было)
-----------------------------------------------
```
auto_ptr - deprecated - реализовывал разрушающее копирование
vector<auto_ptr> v_ptrs;
<auto_ptr> p_a = v_ptrs[i]; // то что было в v_ptrs[i] теперь в p_a и v_ptrs[i] - невалидный
for(auto ptr : v_ptrs)
{
// на i-том невалидный указатель, свалимся
}

unique_ptr - запрещено копирование через
Приватный оператор присваивания и оператор копирования
, можно использовать только move():
// ПРИМЕР // ???

shared_ptr - ведёт подсчёт ссылок (может полявиться проблема циклических ссылок)

weak_ptr - избавляет от циклических ссылок
```
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

-----------------------------------------------
### Q: Можно ли использовать shared_ptr в нескольких потоках ?
A: Да, но есть ограничения.
``` 
-----------------------------------------------
1. Разные потоки могут использовать один и тот же экземпляр shared_ptr,
но только если они не вызывают не констатнтые методы.
2. Для синхронизации одного экземпляра можно использовать атомарные функции.
3. То есть для увеличения/уменьшения счётчика ссылок нужно использовать
специальные атомарные фции, типа apomic_inctement / atomic_decrement.
``` 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

-----------------------------------------------
### Q: Контейнеры библиотеки STL ?
A:
-----------------------------------------------
``` 
- Последовательные контейнеры
array - статический непрерывный массив 
vector - динамический непрерывный массив
deque - Двусторонняя очередь 
! forward_list - односвязный список 
list - двусвязный список  
- Ассоциативные контейнеры
set - Коллекция уникальных ключей, отсортированная по ключам 
map - Коллекция пар ключ-значение, отсортированная по ключам, ключи являются уникальными 
! multiset - Коллекция ключей, отсортированная по ключам 
! multimap - Коллекция пар ключ-значение, отсортированная по ключам 
- Неупорядоченные ассоциативные контейнеры
unordered_set - Коллекция уникальных ключей, хэш-ключами 
unordered_map - Коллекция пар ключ-значение, хэширован ключи, ключи являются уникальными 
! unordered_multiset - Коллекция ключей, хэш-ключами 
! unordered_multimap - Коллекция пар ключ-значение, хешируется по ключам 
- Адаптеры контейнеров
stack - реализует стек с помощью контейнера (LIFO структуры данных) 
queue - адаптируется контейнера обеспечивают очереди (FIFO структуры данных) 
! priority_queue - адаптация контейнеров для обеспечения приоритета очереди
- Специальные
! tuple
! circular_bufer
``` 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

-----------------------------------------------
### Q: Разница между map и unordnered_map ?
A:
``` 
Внутренняя организация памяти внутри контейнеров.
map - красно-чёрное дерево.
unordnered_map - хеш-таблица
```
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

---------------------------------------------------------------
### Q: Сложность операций всатвки в map ?
A:
``` 
Амортизированное О(1)
Коллизии нет  - О(1)
Коллизии есть - до О(N) формируются Бакеты
``` 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

---------------------------------------------------------------
### Q: Сколько нужно мьютексов для получения dead_lock ?

A: Два

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

---------------------------------------------------------------
### Q: Перечислите проблемы многопоточного программирования ?
A: 
``` 
- гонки потоков
- dead_lock / взаимная блокировка
- доступ к общим ресурсам / памяти
- Синхронизация Большого числа Клиентов / Серверов 
 // TO DO //
- "Проблема обедающих философов"
- "Проблема 10.000 клиентов" или "C10k-problem" - https://ru.wikipedia.org/wiki/C10k
``` 
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

-----------------------------------------------
### Q: Сложность операций с контейнером ?

A: См. Таблицу сложностей операций

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


---------------------------------------------------------------
### Q: Многокомпонентные шаблоны проектирования ?
A: 

http://yarik-raider.blogspot.com/2010/01/mvc.html

https://habr.com/ru/post/215605/

- MVC - Model-View-Controller
- MVP - Model-View-Presenter
- MVVM - Model-View-View Model
- MVPVM - Model-View-Presenter-View Model

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

---------------------------------------------------------------
### Q: Какие особенные контейнеры есть в библиотеке BOOST ?

1. Списки
- Массив
- Односвязный список
- Двусвязный список
- Список с пропусками
2. Деревья
- B-дерево
- Двоичное дерево поиска
- AVL-дерево
- Красно-чёрное деревоКуча
3. Графы
- Ориентированный граф
- Ориентированный ациклический граф
- Бинарная диаграмма решений
- Гиперграф
4. Прочие
- Хеш-таблица
- Стек

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@




-----------------------------------------------

# СТАНДАРТНЫЕ ЗАДАЧКИ

-----------------------------------------------
### Q: Переворот числа
A: IN 456   OUT 654
```
int reverse_int(int a)
{
  int b = 0
  int c = 0;
  for(int i = 0; a > 0; i++)
  {
    b = a % 10;
	a = a / 10;
	c = c * 10 + b;
  }
  
  return c;
}
```
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

-----------------------------------------------
### Q: Переворот строки (указатель на char)
A: IN "abcde"   OUT "edcba"
```
void reverse_str(char* str)
{
    int len = strlen(str);
    for(i=0; i < (len/2); i++)
    {
        char temp = str[i];
        str[i] = str[ (len-1) - i];
        str[(len-1) - i] = temp;
    }
}
```
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

-----------------------------------------------
### Q: Переворот строки (string)
A: IN "abcde"   OUT "edcba"
```
// v.1 BAD
#include <algorithm>
#include <string>

void reverse_str(std::string& s)
{
    for ( int i = 0, len = s.length(); i < (len/2); i++ )
    {
		std::swap(s[i], s[(len-1) - i]);
	}
}
```

```
// v.2 GOOD
#include <algorithm>
#include <string>

int main()
{
    std::string foo("foo");
	std::reverse(foo.begin(), foo.end());
}
```
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


-----------------------------------------------
### Q: Сортировка пузырьком (Плохая)
A: IN "246135"   OUT "123456" 
```
// C
void bubble(int* a, int n)
{
  for (int i = n - 1; i > 0; i--)
    {
    for (int j = 0; j < i; j++)
      if (a[j] > a[j + 1])
      {
        int tmp = a[j];
        a[j] = a[j + 1];
        a[j + 1] = tmp;
      }
    }
}
```
```
// C++
template<typename T>
void bubble_sort(T array[], std::size_t size)
{
    for (std::size_t idx_i = 0; idx_i < size - 1; idx_i++)
    {
        for (std::size_t idx_j = 0; idx_j < size - idx_i - 1; idx_j++)
        {
            if (array[idx_j + 1] < array[idx_j])
            {
                std::swap(array[idx_j], array[idx_j + 1]);
            }
        }
    }
}
```
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

-----------------------------------------------
### Q: Сортировка пузырьком (Улучшенная)
A: IN "246135"   OUT "123456" 
```
void bubbleSort(int* arrayPtr, int length_array) // сортировка пузырьком
{
 int temp = 0; // временная переменная для хранения элемента массива
 bool exit = false; // булевая переменная для выхода из цикла, если массив отсортирован
 
 while (!exit) // пока массив не отсортирован
 {
  exit = true;
  for (int int_counter = 0; int_counter < (length_array - 1); int_counter++) // внутренний цикл
    //сортировка пузырьком по возрастанию - знак >
    //сортировка пузырьком по убыванию - знак <
    if (arrayPtr[int_counter] > arrayPtr[int_counter + 1]) // сравниваем два соседних элемента
    {
     // выполняем перестановку элементов массива
     temp = arrayPtr[int_counter];
     arrayPtr[int_counter] = arrayPtr[int_counter + 1];
     arrayPtr[int_counter + 1] = temp;
     exit = false; // на очередной итерации была произведена перестановка элементов
    }
 }
}
```
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


-----------------------------------------------
### Q: Факториал (Итеративный)
A: IN 5   OUT 125 (1*2*3*4*5) 
```
int my_fact(int b)
{
  if(b<0) return 0; // или ОШИБКА !
  
  int c = 1;
  for(int i=1; i<b+1; i++)
  {
    c = c * i;
  }
  return c;
}
```
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

-----------------------------------------------
### Q: Факториал (Рекурсивный)
A: IN 5   OUT 125 (1*2*3*4*5) 
```
int my_fact(int num)  // вычисление факториала числа num
{
  if (num <= 1)
    return 1;  // если число не больше 1, возвращаем 1
  else
    return (num * fact(num - 1));  // рекурсивный вызов для числа на 1 меньше
}
```
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


-----------------------------------------------
### Q: Возведение в степень (Итеративно) (Доп.)
A: IN 2^4   OUT 16 (2*2*2*2)
```
int my_pow(int x, unsigned int power) // num^pow
{
  if (power == 0) return 1;
  if (power == 1) return x;
  int n = 15;
  while ((power <<= 1) >= 0) n--;
  long tmp = x;
  while (--n > 0)
    tmp = tmp * tmp * (((power <<= 1) < 0) ? x : 1);
  return tmp;
}
```

-----------------------------------------------
### Q: Возведение в степень (Итеративно) (без учёта отрицательных тепеней)
A: IN 2^4   OUT 16 (2*2*2*2)
```
int my_pow(int n, unsigned int p) // num^pow
{
  int c = 1;
  for(int i=0; i<p; i++)
    c = n * c;
	
  return c;
}
```
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

-----------------------------------------------
### Q: Возведение в степень (Итеративно) (с учётом отрицательных степеней)
A: IN 2^4    OUT 16 (2*2*2*2)

A: IN 2^-2   OUT 0  (1/4 = 1/(2*2))
```
int my_pow(int n, int p) // num^pow
{
  if(p<0) // 2^(-2) = 1/(2^2)
    p=(-1)*p;
   
  int c = 1;
  for(int i=0; i<p; i++)
  {
    c = n * c;
  }

  if(p<0) // 2^(-2) = 1/(2^2)
     c = 1 / c;

  return c;
}
```
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


-----------------------------------------------
### Q: Возведение в степень (Улучшенный по скорости)
A: IN 2^6    OUT 64 (2*2*2*2*2*2)

A: IN 2^5    OUT 32 (2*2*2*2*2)
```
int my_pow(int n, int p) // num^pow
{
  if(p<0) // 2^(-2) = 1/(2^2)
    p=(-1)*p;

  int c = 1;
  if(p % 2 == 0) // степень чётная => возводим до половины и полвинное умножаем на само себя
  {
    for(int i=0; i<p/2; i++)
    {
      c = n * c;
    }
    c = c * c;
  }
  else // степень не чётная
  {
    for(int i=0; i<p; i++)
    {
      c = n * c;
    }
  }

  if(p<0) // 2^(-2) = 1/(2^2)
     c = 1 / c;

  return c;
}
```
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

-----------------------------------------------
### Q: Возведение в степень (Рекурсивно)
A: IN 2^4   OUT 16 (2*2*2*2)
```
double my_pow(double x, int N)
{
  if(N < 0)
    return (1 / my_pow(x, -N));
  else if(N == 0)
    return 1;
  else if(N % 2 == 0)
  {
    double p = my_pow(x, N / 2);
    return (p * p);
  }
  else
    return ( my_pow(x, N – 1) * x );
}
```
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

-----------------------------------------------
### Q: (1) atio "своими руками" 
A: IN строка str="345"  OUT в число типа int n=435
```
int myAtio(std::string a = "456")
{
  int c = 0;
  int len = a.size();
  
  for(int i=0; i < len; i++)
  {
    c = c*10;
	switch(a[i])
	{
	  case '0': c=c+0; break;
	  case '1': c=c+1; break;
	  case '2': c=c+2; break;
	  case '3': c=c+3; break;
	  case '4': c=c+4; break;
	  case '5': c=c+5; break;
	  case '6': c=c+6; break;
	  case '7': c=c+7; break;
	  case '8': c=c+8; break;
	  case '9': c=c+9; break;
	}
  }
  return c;
}
```

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

-----------------------------------------------
### Q: (2) atio "своими руками" 
A: IN строка str="345"  OUT в число типа int n=435
```
int myAtio(std::string a = "456")
{
  int c = 0;
  int len = a.size();
  for(int i=0; i < len; i++)
  {
    c = c * 10;
	c = (a[i] - '0') + c;
  }
  return c;
}
```
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

-----------------------------------------------
### Q: Из строки с пробелами удалить все пробелы в строке
A:
```
```
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

-----------------------------------------------
### Q: Найти подстроку в строке
A:
```
```
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


-----------------------------------------------
### Q: Факториал на Шаблонах
A:
```
template<unsigned N> struct factorial {
 static const unsigned value = N * factorial<N - 1>::value;
};
template<> struct factorial<0> {
 static const unsigned value = 1;
};

// Пример обращения:
const unsigned f5 = factorial<5>::value; // 120
```
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

-----------------------------------------------
### Q: Биномиальные коэффициенты на шаблонах
A:
```
template<unsigned N, unsigned K>
struct C {
 static const unsigned value =
 factorial<N>::value / factorial<K>::value / factorial<N–
K>::value;
};

// Пример обращения:
const unsigned i = C<5, 2>::value; // 10
```
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


-----------------------------------------------
### Q: Возведение в степень на шаблонах
A:
```
double pow(double x, int N)
{
  if(N < 0) return 1 / pow(x, -N);
  else if(N == 0) return 1;
  else if(N % 2 == 0)
  {
    double p = pow(x, N / 2);
    return p * p;
  }
  else return pow(x, N – 1) * x;
}
```

```
// ШАБЛОННАЯ ВЕРСИЯ

template<int N, typename T>
typename enable_if<(N < 0), T>::type
pow(T x){ return 1 / pow<-N>(x); }
template<int N, typename T>
typename enable_if<(N == 0), T>::type
pow(T x){ return 1; }
template<int N, typename T>
typename enable_if<(N > 0) && (N % 2 == 0), T>::type
pow(T x){ T p = pow<N / 2>(x); return p * p; }
template<int N, typename T>
typename enable_if<(N > 0) && (N % 2 == 1), T>::type
pow(T x){ return pow<N - 1>(x) * x; }

y = pow<3>(x); // Пример вызова
```
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@



# Идиома erase-remove 
Удалить все цифры 8 внутри числового вектора:
```
#include <algorithm>

vec.erase( std::remove( vec.begin(), vec.end(), 8), vec.end() );
```

# Q. Как удалить из VECTOR вектора с числами все 8-ки
A. Воспользоваться Идиомой erase-remove 
```
vec.erase( std::remove( vec.begin(), vec.end(), 8), vec.end() );

vector: { 1 2 3 1 2 3 1 2 3 }
1. вызов remove( v.begin(), v.end(), 2 ) удалит элементы со значением 2 из контейнера v
Удаление происходит по принципу сдвига:
Дошли до двойки: 1 2
Сдвинули тройку на место двойки: 1 3
Ищем дальше, добавляя элементы вектора и ставя по порядку после тройки:
1 3 1 2
Снова встретили двойку, ставим на её место следующий в векторе элемент, не равный двойке
1 3 1 3
И так далее до конца вектора
1 3 1 3 1 3 
Исходный вектор был длины 
Исходный  : 1 2 3 1 2 3 1 2 3
Текущий   : 1 3 1 3 1 3 ? ? ? оставляем на местах сдвинутых элементов невалидные значения вектора
Скорее всего там лежат: 1 2 3 те же значения, что и оставись в оригинальном векторе, но не факт.
remove() после того как сдвинул все элементы, возвращает указатель на ПЕРВЫЙ НЕВАЛИДНЫЙ элемент (то есть на первый ?)
2. вызов erase( первый ? , до конфа вектора ) - удалит из вектора все лишние невалидные значения
и останется только: { 1 3 1 3 1 3 }

```

# Q. Как удалить из LIST списка все числа равные 8-ке
A. вызвать list.remove() и всё.
```
list.remove( list.begin(), list.end(), 8);

list есть собственная реализация удаления объектов, list::remove и она быстрая.
- Элементы там не двигаются, а лишь переставляются ссылки.
- erase вызывать уже не нужно, потому что list::remove уничтожает ненужные элементы сам.
```

