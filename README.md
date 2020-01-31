

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
// The Best
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

int sz = 0;
sz = sizeof(char); // = 1
sz = sizeof(wchar_t); // = 2
sz = sizeof(TCHAR); // = 2 typedef wchar_t WCHAR;

sz = sizeof(bool); 
sz = sizeof(BOOL); // typedef int BOOL;

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
вы получаете с рычага. Имя функции " foo "в объектном файле-это просто" foo", и у него нет всей причудливой информации о типе, которая исходит из искажения имени.

The extern "C" {} construct указывает компилятору не выполнять искажение имен, объявленных в фигурных скобках. Обычно компилятор C++ "улучшает" имена функций, чтобы они кодировали информацию о типе аргументов и возвращаемое значение; это называется исковеркали имя. Элемент extern "C" конструкция предотвращает искажение.

он обычно используется, когда код C++ должен вызывать библиотеку языка C. Он также может использоваться при предоставлении функции C++ (из DLL, для примеру) для клиентов c.
```

# extern C
```
вы должны использовать extern " C " в любое время, когда вы включаете функции определения заголовка, находящиеся в файле, скомпилированном компилятором C, используемом в файле C++. (Многие стандартные библиотеки C могут включать эту проверку в свои заголовки, чтобы упростить ее для разработчика)

например, если у вас есть проект с 3 файлами, util.c, util.ч, а основного.cpp и как .с и. файлы cpp компилируются с помощью компилятора C++ (g++, cc и т. д.), Тогда он действительно не нужен и может даже вызвать ошибки компоновщика. Если ваш процесс сборки использует обычный компилятор C для util.c, тогда вам нужно будет использовать extern "C" при включении util.з.

происходит то, что C++ кодирует параметры функции в ее имени. Вот как работает перегрузка функций. Все, что обычно происходит с функцией C, - это добавление подчеркивания ("_") к началу имени. Без использования extern "C" компоновщик будет искать функцию с именем DoSomething@@int@float () когда фактическое имя функции это _DoSomething () или просто DoSomething ().

использование extern "C" решает вышеуказанную проблему, сообщая компилятору C++, что он должен искать функцию, которая следует соглашению об именовании C, а не C++.
```



