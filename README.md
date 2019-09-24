
# PNG Example
Small png Examples


![](Screenshot_51.png)


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
