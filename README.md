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
	mutable QMutex					_internalLock;
};
  
MyClass::MyClass(QObject* parent) :
_internalLock {}
{
}

state MyClass::getState() const
{
	QMutexLocker locker(&_internalLock);
	return _state;
}
```


