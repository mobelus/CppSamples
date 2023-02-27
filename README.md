
# QtSamples
Many small samples of qt projects

TODO - https://www.youtube.com/watch?time_continue=347&v=iR8K6ya0ZaY&feature=emb_logo

http://qt.shoutwiki.com/wiki/Closing_a_Qt_Quick_application_from_QML

https://habr.com/ru/post/117996/
https://proglib.io/p/tricky-challenges-cpp
https://techrocks.ru/2019/03/01/popular-interview-questions-on-c-plus-plus/
https://pvs-studio.com/ru/blog/posts/cpp/0722/
https://medium.com/@rodrigues.b.nelson/choosing-wisely-c-containers-and-big-oh-complexity-64f9bd1e7e4c
https://flexiple.com/algorithms/big-o-notation-cheat-sheet/

https://www.cyberforum.ru/cpp-beginners/thread971708.html


### QtSamples
Test


### WIX

https://stackoverflow.com/questions/36671672/how-to-enable-wix-projects-in-visual-studio-2017

https://marketplace.visualstudio.com/items?itemName=RobMensching.WixToolsetVisualStudio2017Extension

https://github.com/wixtoolset/wix3/releases/tag/wix3104rtm

# Почему не стоит использовать qobject_cast<MyClass*>(sender())

- Этот метод помечен qt как deprecated
- В многопоточных системах с sender() есть проблема:
sender() - вовзвращает отправителя - Последний испустивший сигнал объект
И если повезёт, то sender() - вернёт нам того объекта отправителя сигнала какого нам надо
А если не повезёт, то sender() - вернёт нам последнего, который будет СОВСЕМ НЕ тот кого мы ожидаем
```
MyClass *senderMy   = qobject_cast<MyClass*>(sender());
bool    isSenderMy = (senderMy != nullptr);
```

# Шаблоны и Qt / Q_OBJECT + template<class T> 

https://dolzhenko.blogspot.com/2008/10/qt-usage-c-templates-with-qobject.html
	
**PROBLEM**: Using the Meta-Object Compiler (moc):
moc doesn't create moc class (qt meta info for class) for follow class declaration:
```
template <class T>
class SomeWidget : public SomeSubWidget {
 Q_OBJECT
public:
 SomeWidget(QWidget *parent = 0): SomeSubWidget(parent){
 }
 // for instance, wrap T class
protected:
 virtual T* component(QWidget *parent = 0) { return new T(parent); }

 T *widget;
};
```


**Solution**: devide class for two classes - with Q_OBJECT declaration and with template:
create sub class with Q_OBJECT
```
class SomeSubWidget : public QWidget
{
 Q_OBJECT
public:
 SomeSubWidget(QWidget *parent = 0):QWidget(parent){}
};
```

and when inherit it and add template:
```
template <class T> class SomeWidget : public SomeSubWidget
{
public:
 SomeWidget(QWidget *parent = 0): SomeSubWidget(parent) { } // for instance, wrap T class
protected:
 virtual T* component(QWidget *parent = 0) { return new T(parent); }
 T *widget;
};
```

https://stackoverflow.com/questions/4397478/qt-templated-q-object-class

**PROBLEM**: It is not possible to mix template and Q_OBJECT 

**SOLUTION 2**: If you have a subset of types you can list the slots and signals like this:
```
class SignalsSlots : public QObject
{
  Q_OBJECT
  public:
    explicit SignalsSlots(QObject *parent = 0) :
    QObject(parent) { }
  public slots:
    virtual void writeAsync(int value) { }
  signals:
    void readAsynkPolledChanged(int value);
};
...
template <class T>
class Abstraction : public SignalsSlots
{...
```
	
**Pimpl** 
https://stackoverflow.com/questions/12294981/interfaces-with-template-methods


# QScroller::grabGesture + QScrollerProperties
- QScroller::grabGesture(ui->tableView, QScroller::LeftMouseButtonGesture);
- Approach with QScrollerProperties using the scroller obj.
- setTableScrollerNonDragableProperties<QTreeView>(ui->treeViewTable);

```
#include <QTreeView>
#include <QScroller>
#include <QScrollerProperties>

template <class H>
void setTableScrollerNonDragableProperties(H *table)
{
    QScroller *scrol = QScroller::scroller(table);
    // (*) What this part of code does:
    // When you are at the top and scroll down realy fast on the table with your finger gesture
    // When bable comes to the end it will Drag (move a bit under the visible bottom and return to the top again)
    // To make the table just move down to the end and then simply Stop without dragging and stop at the bottom
    // we prepare QScrollerProperties scrprop; with following parameters:

    QScrollerProperties scrprop;
    QVariant overshootPolicy = QVariant::fromValue<QScrollerProperties::OvershootPolicy>(QScrollerProperties::OvershootAlwaysOff);
    scrprop.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, overshootPolicy);
    scrprop.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, overshootPolicy);
    scrprop.setScrollMetric(QScrollerProperties::DragStartDistance, 0.001);
    scrprop.setScrollMetric(QScrollerProperties::DragStartDistance, 0.001);

    scrol->setScrollerProperties(scrprop);

    scrol->grabGesture(table, QScroller::LeftMouseButtonGesture);
}

```

# QString s; s.arg().arg()... <=> QStringList

```
QStringList listProxy;
listProxy.append("Z");
listProxy.append("X");
listProxy.append("Y");
QString funcProxy = "a'%1'b'%2'c'%3'"; // a'Z'b'X'c'Y'
for (int i = 0; i < listProxy.size(); ++i)
    funcProxy.replace(QString("%%1").arg(i + 1), listProxy.at(i));
```

# QMessageBox

### QMessageBox BAD:
```
QMessageBox msg;
    msg.setWindowTitle(tr("Invalid variable value"));
    msg.setText(tr("Unable to convert Variant value to int state"));
    msg.exec();
```

https://doc.qt.io/qt-5/qdialog.html#exec

### QMessageBox GOOD:
```
QMessageBox::warning(nullptr, "Warning", "Macros recording wasn't stopped!\n Stop recording the macros and try again.", "Ok");
QMessageBox::critical(this, "Error", error);
QMessageBox::about(this, tr("About program"), aboutMessage);

```

# qstring arg multiple times in a circle

	QString str;

	// Version 1
	str = "some %5987%239 random %66 text";
	for(int i=0; i<3; ++i)
	{
		str = str.arg( QString::number( (i+1)*10 ) );
		// returns "some 1020 random 30 text"
	}
	
	// Version 2
	str = "some %1 random %2 text %3 a %3 b %1 c %2 d finish";
	for(int i=0; i<3; ++i)
	{
		QString argNum = "%"+QString::number(i+1);
		str = str.arg(argNum).arg( QString::number( (i+1)*10 ) );
		// returns "some 10 random 20 text 30 a 30 b 10 c 20 d finish"
	}


# tr("your cool text") => ???? ?????? ?????

PROBLEM: Sometimes we get **???? ???? ???** on the place of our translated text.
Mostly It happends when we translate from English to Any Unicode Language.

setText(QCoreApplication::translate("Text", text.toLocal8Bit()));

### toLocal8Bit() can cause this problem

Use instead of toLocal8Bit() <---> toUtf8()


# QFile .open() correct error handling file.errorString()

```
QString flName;
QFile fl(flName); // exists / permission denied / other process busy
QString error;
if(fl.exists())
{
    if(fl.open(QIODevice::ReadOnly))
    {
        QByteArray block = fl.read(10);
        fl.write(block);
        fl.close();
    }
    else
    {
        error = QString("File: %1 Error: %2").arg(flName).arg(fl.errorString());
    }
}
else
{
    error = "File does not exist";
}

```


# QDir create for DEBUG

```
QDateTime dt = QDateTime::currentDateTime();
QDir dr; dr.mkdir("C:/Users/Public/test/" + dt.toString("dd-MM-yyyy-hh-mm-ss-z"));
```

# QFile create for DEBUG

```
{
	QDateTime dt = QDateTime::currentDateTime();
	QString sdt = dt.toString("dd_MM_yyyy_hh_mm_ss_z");
	sdt = sdt.append(".txt");

	QString file = "C:/test/_/" + sdt;

	writeStrToFile("1", file);
	writeStrToFile("2", file);
	writeStrToFile("3", file);
}

void writeStrToFile(QString scriptTetx, QString fileName)
{
	std::string s = fileName.toStdString();

	QFile scriptFile(fileName);
	if (scriptFile.open(QIODevice::ReadWrite))
	{
		// Save the script
		QTextStream outputStream(&scriptFile);
		QString code = scriptTetx;
		code = code.append("\n");
		outputStream.seek(outputStream.readAll().size());
		outputStream << code;
		scriptFile.close();
	}
}
```

# QJSEngine - setObjectOwnership

```
	QJSEngine _scriptEngine(this);
	// Register objects for access from script
	QJSValue svApi = _scriptEngine.newQObject(this);
	_scriptEngine.globalObject().setProperty("api", svApi);

	// all cpp objects that were passed to QJSEngine or QQmlEngine will be destroy by context of these object.
	// Without this call destuctor of ScriptInterface will be called 2 times - by EmulatorUiApplication and ScriptEngine;
	QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
```

# QString - smal guide
	
```
    QString s("1234567890");
    s = s.mid(3, s.length() - 3); // all after first 3
    s = s.left(3); // first 3
    s = s.right(3); // last 3
    
```

### SELECT BIG Data from a Large Table CORREC Solution:

```
    //
    // canFetchMore() + fetchMore() approach
    //
    
    while (mModel->canFetchMore())
        mModel->fetchMore();
    int totalRows = mModel->rowCount();
    
    for (int row = 0; row < totalRows; ++row) {
        for (int col = 0; col < mModel.columnCount(); ++col) {
            out << mModel->index(row, col).data().toString() << mFieldDelimeter;
        }
        out << EOL;
    }

```

### SELECT BIG Data from a Large Table BAD Solution:

```
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>


	int rows = model->rowCount();
        int cols = model->columnCount();
        bool modelShowsAllRecords = !mModel->canFetchMore();
        
        if (modelShowsAllRecords) {
            for (int row = 0; row < rowCnt; ++row) {
                for (int col = 0; col < colCnt; ++col) {
                    out << mModel->index(row, col).data().toString() << mFieldDelimeter;
                }
                out << EOL;
            }
        }
        else {
            QSqlQuery query(mModel->query());
            bool      table_ok = query.exec();

            if (!table_ok) {
                QString sqlError("Error from SystemEventLogTable: " + query.lastError().text());
                qDebug() << sqlError;
                return ExportError(QString("Export to file error: %1").arg(sqlError));
            }
            else {
                // Only way to get a row count, size() method in QSqlQuery class
                // does not work for SQLite3
                query.last();
                int rowFullCount = query.at() + 1;
                if (rowCnt != rowFullCount)
                    rowCnt = rowFullCount;
            }

            if (rowCnt != 0) {
                query.first(); // return to first row
                do {
                    for (int col = 0; col < colCnt; ++col) {
                        out << query.value(col).toString() << mFieldDelimeter;
                    }
                    out << EOL;
                } while (query.next());
            }
        }

        return Result<void>::ok();
```
	

### OLD find function version - NO ALGO used


```
findTrainNumberNumberInContainer( QVector<StationScheduleInfo>& vec, int trainNumber, int& countStops)
{
	bool isNumberFound = false;
	for (auto a : vec)
	{
		if (countStops > 0 && !isNumberFound)
			return true;

		isNumberFound = (a.trainNum() == trainNumber);
		if (isNumberFound)
			countStops++;
	}

	return false;
}

```

## NEW find function version - USING  ALGORUTHM

```
	countStops = std::count_if(vec.begin(), vec.end(), [&](StationScheduleInfo elem) { return elem.trainNum() == trainNumber; });
	return countStops > 0;
```


### Center Coordinates for dialog
```
void GetCenterCoordsForDlg(QRect dlgLoginSize)
{
	// QListIterator<QScreen*> it(qApp->screens());
	// QScreen* firstScreen = qApp->screens().at(0);
	// QRect rct = (firstScreen)->geometry();
	// int sumWidth = rct.width();
	// while (it.hasNext())
	// {
	// 	QScreen *singleScreen = it.next();
	// 	sumWidth += singleScreen->geometry().width();
	// }

	QRect resultRect;

	QRect mainWindowRect = this->geometry();
	QListIterator<QScreen*> it(qApp->screens());
	QScreen* firstScreen = qApp->screens().at(0);
	QRect rct = (firstScreen)->geometry();
	int sumWidth = rct.width();
	// Show login dialog upon top-left x coordinate
	//int appWindowXposition = this->x();
	// Show login dialog upon coordinates of the center of the application window
	int appWindowXposition = this->x() + this->width() / 2;
	while (it.hasNext())
	{
		QScreen *singleScreen = it.next();
		QRect screenRect = singleScreen->geometry();
		int lastFoundScreenWidth = screenRect.width();

		if (appWindowXposition < sumWidth)
		{
			auto height = qMin(dlgLoginSize.height(), screenRect.height());
			auto width = qMin(dlgLoginSize.width(), screenRect.width());
			auto x = (screenRect.width() - width) / 2;	// center
			x += (sumWidth - lastFoundScreenWidth);		// center + step to right on all of the screens that we have met before
			auto y = (screenRect.height() - height) / 2;

			resultRect = QRect(x, y, width, height);
			qApp->sendCenterCoordsForDlg(resultRect);
		}
		else
		{
			sumWidth += screenRect.width();
		}
	}

}
```

# BEFORE REVIEW

```

QVariant StationScheduleTableModel::data(const QModelIndex & index, int role) const
{
  if (index.isValid())
  {
    int irow = index.row();
    int cnt = _itemsShown.count();

    if (irow < 0 || irow >= cnt)
    	return QVariant();

    const StationScheduleInfo &item = _itemsShown[irow];

    switch (role)
    {
    case TrainNumberRole:
    {
    	if (irow % _distanceNum == 0)
    		return item.trainNum();
    }
    break;
    case StationIdRole:
    	return item.stationId();
    case StationNameRole:
    	return item.stationName();
    case ArivalTimeRole:
    	return item.arivalTimeString();
    case DepartureTimeRole:
    	return item.departureTimeString();
    default:
    	return QVariant();
    }
  }

  return QVariant();
}

```

# AFTER REVIEW

```
  QVariant result;
  if (index.isValid())
  {
    int irow = index.row();
    int cnt = _itemsShown.count();

    if (irow < 0 || irow >= cnt)
    	return QVariant();

    const StationScheduleInfo &item = _itemsShown[irow];

    switch (role)
    {
    case TrainNumberRole:
    {
    	if (irow % _distanceNum == 0)
    		result = item.trainNum();
    }
    break;
    case StationIdRole:
    	result = item.stationId();
    	break;
    case StationNameRole:
    	result = item.stationName();
    	break;
    case ArivalTimeRole:
    	result = item.arivalTimeString();
    	break;
    case DepartureTimeRole:
    	result = item.departureTimeString();
    	break;
    default: break;
    }
  }

  return result;
```

```
{
    connect(ui->slider, &QSlider::valueChanged, this, &A::onSetValue);
    connect(ui->stepLeftButton, &QPushButton::clicked, this, &A::onMoveStepLeft);
    connect(ui->stepRightButton, &QPushButton::clicked, this, &A::onMoveStepRight);
    ui->slider->setValue(settings->value());
    ui->label->setText(QString::number(settings->laserValue()));

    ui->slider->setSingleStep(STEP);
    ui->slider->setPageStep(STEP);
    ui->slider->setTickInterval(STEP);
}
void A::onSetValue(int v)
{
    int res = std::round(v / STEP) * STEP;
    ui->slider->setValue(res);
    settings->setValue(res);
    ui->label->setText(QString::number(res));
}
void A::onMoveStepLeft()
{
    doMoveStep(true);
}
void A::onMoveStepRight()
{
    doMoveStep(false);
}

void A:doMoveStep(bool isleft)
{
    int v   = ui->slider->value();
    auto mi = ui->slider->minimum();
    auto ma = ui->slider->maximum();
    auto s  = STEP;

    for (int i = mi, l, r; i < ma; i += s) {
        l  = i;
        r = l + s;
        if (v >= l && v <= r) {
            if (l == v && v >= m) {
                v = isleft ? (l - s) : (l + s);
                break;
            }
            else if (r == v && v <= ma) {
                v = isLeft ? (r - s) : (r + s);
                break;
            }
            else if (v > l && v < r) {
                val = isleft ? l : r;
                break;
            }
        }
    }
    ui->slider->setValue(v);
}
```


# **Разработка веб-приложений с использованием QML и Qt для WebAssembly**

 **Установка и настройка.**

Нужно сделать две вещи: настроить компилятор Emscripten и Qt для веб-сборки.

**Emscripten**

на Linux:

- `# fetch Emscripten SDK and install target version`
- `git clone https://github.com/emscripten-core/emsdk.git`
- `cd emsdk`
- `./emsdk install sdk-1.38.30-64bit`
- `./emsdk activate --embedded sdk-1.38.30-64bit`
- `source emsdk_env.sh`

Обратите внимание, что 1.38.30 рекомендуется для сборки, не экспериментируйте с другими сборками, так как у них будут проблемы.

Исходный код `emsdk_env.sh` важен для настройки правильной среды *Emscripten*, поэтому не забывайте запускать ее <u>(каждый раз, когда вы открываете новый терминал</u>)


**Qt WebAssembly**

Это еще проще. Загрузите установщик [Qt со страницы загрузки Qt](https://www.qt.io/download) или откройте существующий [инструмент Qt Maintenance,](https://doc.qt.io/qt-5/gettingstarted.html#updating-or-removing-qt) если он у вас уже установлен.

В обоих случаях вам просто нужно выбрать Qt для WebAssembly, в версии Qt  от 5.13.2.

 

![https://miro.medium.com/max/1633/1*XF9KMNHHaFMSjoUS6FbNpA.jpeg](file:///C:/Users/IGOR~1.SHA/AppData/Local/Temp/msohtmlclip1/01/clip_image004.jpg)

Установка Qt WebAssembly



# Привет WebAssembly

Перейдите к вашему проекту и создайте `build `каталог.  Затем просто вызовите новый `qmake `с поддержкой WebAssembly:

- `cd /path-to-hello-webassembly-project` - преход в дирректорию проекта
- `mkdir build && cd build` - создание папки `build` и переход в неё
- `/path-to-hello-webassembly/5.13.2/wasm_32/bin/qmake ..` - компилирует проект
- `make`

После того, как это будет сделано, вы получите один из файлов в вашем каталоге сборки -  projectname.html. Чтобы запустить его в браузере, нам нужен, как обычно, веб-сервер. Вы можете использовать все, что захотите, для обслуживания этих статических файлов, но есть также удобный инструмент командной строки emrun, который вы можете использовать для этой цели, поскольку он уже доступен как часть среды Emscripten.

Итак, чтобы открыть наше приложение в браузере Firefox, можно сделать:

- `emrun --browser=firefox *.html`
 

**<u>Пример из dev-shaldunov:</u>**

1. Выполняем из /home/user/emsdk:

     `./emsdk activate --embadded sdk-1.38.30-64bit`

2. `source emsdk_env.sh`
3.  Создаем папку `build` в корне проекта: /mywebassembly/build:
4. `/opt/Qt/5.13.2/wasm_32/bin/qmake ..`
5. `make` 

6. Чтобы открыть наше приложение в браузере Firefox, можно сделать:

```
`emrun --browser=firefox *.html`
```

P.S.: Более подробное описание Вы можете найти на: https://itnext.io/developing-web-apps-using-qml-and-qt-for-webassembly-aa84453f2f61
   
 

1) class A;
```
class A {
 ~A();
  A();
  A(const A& other);
  A& coperator= (const A& other)
  A(A&& other);
  A& coperator= (A&& other)
};
```

2) Singleton
```
class A {
public:
  static A& i() {
    static A a;
    return a;
  }
private:
  A() {}
  A(const A& other) = delete;
  A& operator=(constA& other) = delete;
};
```
3) Creation Destruction
```
BA, BB, BC
MA, MB, MC
Z
```

4) tasks:
- class_with_pointer
- bubble_sort
- quick_sort
- binary_search
- my_string
- my_vector
- spin_lock
- thread_pool
- producer_consumer
- DFS - Number_of_islands
- BFS - Binary_tree_level_order_traversal
- 



# CppSamples
Small cpp Examples

Test **Test** *Test* Test

# CppSamples
Small cpp Examples


https://www.tutorialspoint.com/cplusplus/cpp_questions_answers.htm

C++ Questions 

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


### MUTABLE Relevant example (mutable mutex) / MUTABLE пример использования на практике
Ответ: mutable mutex при работе с const-тантными get-методами.
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


### Initialization over constructor with parameters / Инициализация через конструктор с параметрами

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


# UUID UUID UUID UUID UUID

### Разные вопросы с примерами
https://www.bestprog.net/ru/2018/04/02/c-class-constructor-features-of-use-constructors-in-the-classes-the-default-constructor-parameterized-constructors-examples-of-classes-that-contain-constructors_ru/

### undefined behavoiur 
--

### rvalue и lvalue
- lvalue - то, что имеет конкретный адрес в памяти
- rvalue - то, что адреса не имеет, то, что называется временным объектом

### Методы класса определяются неявно / по умолчанию
```class A { };```

Ответ:
- конструктор по умолчанию (без аргументов)
(- деструктор)
- конструктор копирования по умолчанию
- оператор присваивания
- Перемещающий конструктор
- Оператор перемещения (Перемещающий оператор присваивания)
```
То есть объявление class A {} может быть эквивалентно следующему:
  A() {} 
  ~A() {} 
  A(const A& that) {}
  A& operator=(const A& that) {}
  A(A&& that) {} // C++11
  A& operator=(A&& that) {} // C++11
};
```
### Когда конструктор по умолчанию не создается ?

// TO DO //

### В чем опасность проблема конструктора копирования созданного по умолчанию ?
// TO DO  (!) //
- В том, что Он Тупо копируются все поля.
- (!) В случае, если в классе присутствует указатель, то такое конструктор нужно переопределять, как и (Правило трёх / пяти для С++11)

### Какова сигнатура конструктора копии / копирующего конструктора ?
```A(const A& _obj) { this = _obj; }```

### Какова сигнатура оператора присваивания / оператора "равно" ?
```
A& operator=(const A& _obj) 
{
  if (&_obj == this)
    return *this;
	
  A a; a = _obj; return a;
} 
```
### Какова сигнатура перемещающего конструктора / конструктора перемещения ?
``` A(A&& tmp) { ... } // см 2 вопр. ниже``` 

### Какова сигнатура перемещающего оператора / оператора перемещения ?
``` A& operator=(A&& tmp) { ... } // см 1 вопр. ниже ```


### Если В классе есть указатель, что стоит перегрузить ?
До С++11
- Правило **Трёх** / Правило Трех (1. констр, дестр, 2. констр копии 3. оператор равно)

Начиная С++11
- Правило **Пяти** 
(1. констр, дестр, 2. констр копии 3. оператор равно
+4. констр. перемещения +5. оператор перемещения)

```
#include <vector>
class Buffer
{
public:
  Buffer(const std::string& buff)
  :   pBuff(nullptr)
  ,   buffSize(buff.length())
  {
      pBuff = new char[buffSize];
      memcpy(pBuff, buff.c_str(), buffSize);
  }

  ~Buffer(){ destroy(); }

  Buffer(const Buffer& other)
  :   pBuff(nullptr)
  ,   buffSize(other.buffSize)
  {
    pBuff = new char[buffSize];
    memcpy(pBuff, other.pBuff, buffSize);
  }

  Buffer& operator=(const Buffer& other)
  {
    destroy();
    buffSize = other.buffSize;
    pBuff = new char[buffSize];
    memcpy(pBuff, other.pBuff, buffSize);
    return *this;
  }

  Buffer(Buffer&& tmp)
  :   pBuff(tmp.pBuff)
  ,   buffSize(tmp.buffSize)
  {
    tmp.pBuff = nullptr;
  }

  Buffer& operator=(Buffer&& tmp)
  {
    destroy();
    buffSize = tmp.buffSize;
    pBuff = tmp.pBuff;
    tmp.pBuff = nullptr;
    return *this;
  }

private:
  void destroy()
  {
    if (pBuff)
        delete[] pBuff;
  }

  char* pBuff;
  size_t buffSize;
};
```


### Что такое вызов this() ?
https://habr.com/ru/post/152118/

```
struct SomeStruct
{
    private int _i;
    private double _d;

    public SomeStruct(int i)
        : this() // вызываем “конструктор по умолчанию”
    {
        _i = i;
        // Поле _d инициализировано неявно!
    }
}
```

Вызов this() выглядит в точности как вызов конструктора по умолчанию и предотвращает ошибку компиляции за счет того, что он обнуляет (а значит и инициализирует) все поля структуры. На самом же деле вызов this() превращается все в ту же инструкцию initobj, используемую ранее для получения значения по умолчанию экземпляра структуры.

### Есть конструктор с один параметром и тип этого параметра не совпадает с типом класса ? Какая есть особенность у ТАКОГО конструтора с один параметром ?
- Он может неявно вызываться, при инициализации
```
class A
{
private: int m_a;
public:
A{int a} : m_a(a) { }
};

A obj = 2; // неявно вызовится A obj(2);
```
- Чтобы запретить НЕЯВНЫЙ вызов подобный можно использовать ключевое слово EXPLICIT
```
class A
{
private: int m_a;
public:
explicit A{int a} : m_a(a) { }
};

A obj = 2; // ОШИБКА КОМПИЛЯЦИИ
           // из-за explicit так больше делать нельзя!
A obj(2);
```

### Примеры использования особенности у ТАКОГО конструтора с один параметром ?
У НЕГО ключевого слова EXPLICIT нету!

1. ПЛЮС
```
class string
{ string( const char* ) };
```
Что позволяет нам использовать класс стринг в обоих вариантах, что повышает удобство:
```
std::string obj = "abc";
std::string obj( "abc" );
```
Идентичные записи, но всем более удобен первый вариант.

2. Функция принимет на вход тип std::string, a мы можем передать ей на вход CONST char*:
```
void foo(std::string s);
{
}

void main()
{
  const char * str = "abc";
  foo( str );
}
```
В силу неявного преобразования в таком случае получится, что эта конструкция БУДЕТ работать и в этом второе удобство ивпользования такого конструктора:
```
foo( str ); // вызовет по факту foo( std::string s( str ) )
```

### Функция принимет на вход тип std::string, a мы передаём ей на вход char* ? Можно ли передать в параметр функции типа std::string тип char* ?
НЕТ ! В std::string мы можем передать только CONST CHAR *

### Как ещё можно запретить неявное преобразование, кроме слова explicit для конструктора с одним параметром ?
ВАРИАНТ 1: В своём классе нужно добавить в PRIVATE Оператор того типа, который хотиз запретить чтобы он неявно не преобразовывался:
```
class foo
{
public:
 double s;
 double& DBL(){return s;}
private:
 operator double() { } // добавили оператор double() в приват, итог
};
 
foo a;
a.s = 80.9;
double n;
n = a; //ошибочка, приватный приведенец
n = a.DBL(); //ОК, вызов публичного метода
```

ВАРИАНТ 2: Запрещаем неявное преобразование в вызове функции через параметр Шаблона:
```
template <typename T>
void foo(T t, typename std::enable_if<std::is_same<T, uint64_t>::value>::type * = 0)
{
   std::cout << "Only uint64_t.";
}
 
int main()
{
   uint64_t i64 = 100;
   int i = 42;
   double d = 12.3;
   short s = 1;
   long l = 12356;
 
   foo(i64);
   //foo(i);
   //foo(d);
   //foo(s);
   //foo(l);
}
```

### Опеределение каких методов исключает невяное определение конструктора перемещеения / перемещающего оператора присваивания ? Что нужно переопределить и тогда перемещающий оператор пропадает фактически.
Ответ: Переопределение оператора =

// TO DO //

### Безопасно / Можно ли вызывать Виртуальные методы из Конструкторов или Деструкторов
- Можно ли вызывать Виртуальные методы из Конструктора
- Можно ли вызывать Виртуальные методы из Деструктора

- Виртуальность не будет работать в конструкторе и деструкторе, ибо Для Конструкторов, ещё не проинициализируются таблицы виртуальных функций, Для Деструкторов, будут удалены объекты с их таблицами вирт.функций. Плюс если вызвать чисто-виртуальную функцию в конструкторе, то мы можем словить PURE VIRTUAL CALL EXCEPTION.
- Для случая создания объекта наследника, бызовется конструктор базового класса и далее начинаются проблемы:
- Т.к. конструктор Базового класса ещё не отработал полностью, а конструктор наследника вызовется только следующим за базовым, и таблица вирт.фций не проинициализирована полностью, то вызовется не функция наследника, вызов которой мы ожидаем, а вызовется функция Базового класса (если метод НЕ ЧИСТО виртуальный). В результате мы получим НЕОЖИДАЕМОЕ поведение.
- Если вызываемый метод будет ЧИСТО виртуальным (внутри конструктора) мы ещё и можем получить Исключение! PURE VIRTUAL CALL EXCEPTION 

Внутри Конструктора - не работает механизм виртуальных функций, потому что ещё не инициализирована таблица виртуальных функций.

Внутри Деструктора - механизм виртуальных функций может сработать неправильно (не так как было задумано изначально), по причине того, что в таблице виртуальных функций будут содержаться указатели на методы уже уничтоженного класса наследника.

Подробнее: Базовый класс в нём определён конструктор с вызовом виртуальной функциии. Мы наследуемся, переопределяем в наследнике виртуальную функцию. При создании объекта наследника, будет сначала вызван конструктор Базового класса, в нём будет вызвана виртуальная фция переопределённая в наследнике, И ввиду того, что НАСЛЕДНИК НЕДОСФОРМИРОВАН (его конструктор ещё не отрабатывал) то как следствие возможна ошибка. Далее UB (undefined behavior) Вероятнее всего вызовется метод самого базового класса, если тот определён, иначе ... вопрос.

Подробнее: Базовый класс в нём определён деструктор с вызовом виртуальной функциии. Мы наследуемся, переопределяем в наследнике виртуальную функцию. При удалении объекта наследника, будет сначала вызван деструктор Наследника класса, после деструктор Базового класса и уже в нём будет вызвана виртуальная фция переопределённая в наследнике, И ввиду того, что НАСЛЕДНИК УНИЧТОЖЕН (его деструктор уже полностью отработал) то как следствие возможна ошибка. Далее UB (undefined behavior) Вероятнее всего вызовется метод самого базового класса, если тот определён, иначе ... вопрос.

### Как можно создать объект класса по определённому адресу в памяти ? по конкретному адресу в памяти ?
- Использовать оператор  **placement new** 
- Использовать MEMCPY ( Адрес_Куда, Адрес_Откуда, Размер_Скок_Скопировать )
```memcpy( &dst[dstIdx], &src[srcIdx], numElementsToCopy * sizeof( Element ) );```
- ещё как-то побитово копировать

```
new T;      // calls operator new(sizeof(T))
            // (C++17) or operator new(sizeof(T), std::align_val_t(alignof(T))))
new T[5];   // calls operator new[](sizeof(T)*5 + overhead)
            // (C++17) or operator new(sizeof(T)*5+overhead, std::align_val_t(alignof(T))))
new(2,f) T; // calls operator new(sizeof(T), 2, f)
            // (C++17) or operator new(sizeof(T), std::align_val_t(alignof(T)), 2, f)
```
		
```
// buffer on stack 
unsigned char buf[sizeof(int)*2] ; 
// placement new in buf 
int *pInt = new (buf) int(3); 
int *qInt = new (buf + sizeof (int)) int(5);
```

```
// initial value of X 
int X = 10; 
  
cout << "Before placement new :" << endl; 
cout << "X : " << X << endl; 
cout << "&X : " << &X << endl; 
  
// Placement new changes the value of X to 100 
int *mem = new (&X) int(100); 
```

### Для чего нужно ключевое слово mutable ?
Для CONST-тантных методов, которые не могут изменять состояние экземпляра класса и которые и было введено ключевое слово mutable - ибо их он изменять может в отличие от всех сотальных.
- Самый релевантный пример его использования в реальной практике это **mutable mutex** внутри констатных Get-методов класса, которые должны обратиться к некому общему ресурсу внутри класса.

### Для чего нужно ключевое слово static ?
- static-методы класса имеют доступ только к статическим полям класса, т.к.
- static-методы не имеют прямой связи с конкретным экземпляром класса
- и по этой причине static метод в классе не имеет доступа к указателю this 
- inline функции не могут быть помечены как static

### Для чего нужно ключевое слово inline ?
Это такая функция, чье тело подставляется в каждую точ­ку вызова, вместо того, чтобы генерировать код вызова. Нам не всегда гарантируется её встраиваемость:

!!! ЕСТЬ ОГРАНИЧЕНИЯ:
- Некоторые компиляторы не могут компилировать встроенный код для функции, содержащей циклы, а также предложения switch и goto.
- Зачастую бывает нельзя создать рекурсивную inline функцию.
- Запрещается использование статических членов функции.

### Для чего нужно ключевое слово const ?
- Позволяет задать константность объекта (Чтобы не было возможности изменить ОБЪЕКТ)
- Позволяет задать константность указателя (Чтобы не было возможности изменить указатель)
- Константы и аргументы/результаты функций, а так же Позволяет указать, что данный метод не модифицирует члены класса, т.е. сохраняет состояние объекта. Чтобы метод не мог изменять члены класса, ЗА Исключением членов mutable.

- Позволяет задать константность объекта (Чтобы не было возможности изменить ОБЪЕКТ)
```
const int i(1);
int const j(1);
```
- Позволяет задать константность указателя (Чтобы не было возможности изменить указатель)
```
const char * a1 = "12"; a="21";
char const * a2 = "11"; a="22";
char * const a3 = "13"; 
a = "23"; // <-- не работает
*a = 'Y';
const char * const s = "data"; // работает
```
```
int i = 4; 
int* const j(&i); 
int k = 6; 
*j = k; // Ok 
j = &k; // error C3892: 'j' : you cannot assign to a variable that is const
```
- Константы и аргументы/результаты функций, а так же Позволяет указать, что данный метод не модифицирует члены класса, т.е. сохраняет состояние объекта. Чтобы метод не мог изменять члены класса, ЗА Исключением членов mutable.
```
class A {
private:
  int x;
public:
  void f(int a) const {
    x = a; // <-- не работает
  }
};
```

### Для чего используется ключевое слово volatile?

Ответ: Для указания компилятору, что доступ к переменной может осуществляться из мест, неподконтрольных ему. А как следствие, что работу с данной переменной не нужно подвергать разного рода оптимизациям.
```volatile int i = 1; // Независимо от прочего кода, данная переменная не будет оптимизирована.```
Т.е. если volatile присутствует в каком-то условии, которое не меняется со временем, то компилятор может оптимизировать его, чтобы избежать ненужных проверок, при использовании volatile компилятор скорее всего не будет этого делать.
```while (1) 
{
    if(i == 1)
    {
        // Какой-то код не изменяющий i
    }
}
// Если бы volatile отсутствовало, то компилятор мог бы переделать код на что-то аля:
if(i == 1) // Нет необходимости проверять i все время, если и так известно, что оно не изменяется
```

предотвращает неявное преобразование типов при инициализации.


### STD::MOVE
Ответ: Она преобразует любой входной тип к RValue-ссылке ! (std::remove_reference "Очищает" тип от ссылочности вообще и подставляет после тип RVALUE-ссылку return static_cast ... type&&> )
```
template<typename _Tp>
  inline typename std::remove_reference<_Tp>::type&&
  move(_Tp&& __t)
  { return static_cast<typename std::remove_reference<_Tp>::type&&>(__t); }
```

### STD::FORWARD

https://ru.stackoverflow.com/questions/908361/%D0%A7%D0%B5%D0%BC-%D0%BE%D1%82%D0%BB%D0%B8%D1%87%D0%B0%D1%8E%D1%82%D1%81%D1%8F-%D1%84%D1%83%D0%BD%D0%BA%D1%86%D0%B8%D0%B8-move-%D0%B8-forward

Ответ: Она может преобразовать входнорй тип как к LValue-ссылке так и к RValue-ссылке с использованием внутри себя механизм **СКЛЕЙКА ССЫЛОК** 
```
template<class _T>
_T&& forward(typename std::remove_reference<_T>::type& a)
  { return static_cast<_T&&>(a); }
```
Концептуально: Нужна для того, чтобы осуществлять "Проброс типа" того типа колторый нам нужен, через шаблонную функцию так чтобы на выходе этот тип либо в неизменном виде остался. Ибо в результате работы с сылками при работе склейки ссылок мы можем получить не тот тип, на выходе, который мы хотим. Для решения этой проблемы и есть std::forward.

Суть этой идиомы в том, что если ссылка на шаблонизированный тип имеет строго вид T&&, и если в процессе вывода типа компилятор выполняет свертывание ссылок, то в результате свертывания результирующая ссылка может быть выведена как rvalue, так и lvalue.

Нужно для прямая передача **"прямой передачи"** (perfect forwarding), а выглядит эта идиома так:
```
template <typename T>
void f(T&& arg) {
  . . .
  g(std::forward<T>(arg));
}
```

### const_expr 
- не гарантирует выполнения на этапе компиляции
- нужен для просчёта каких-то функций на этапе компиляции
- Если на этапе компиляции const_expr не может быть вычислено, то оно будет вычислено в runtim-е

### const_expr всег да ли вызов таких функци будет на этапе компиляции ?
- Если на этапе компиляции const_expr не может быть вычислено, то оно будет вычислено в runtim-е

### Чем отличается typedef от using как возможность для определения "псевдонима типа" или alias-а 
https://ru.stackoverflow.com/questions/499481/%D0%9E%D1%82%D0%BB%D0%B8%D1%87%D0%B8%D0%B5-using-%D0%BE%D1%82-typedef
- главное преимущество using перед typedef - **при работе с шаблонами**. шаблоны. В частности, объявления псевдонимов могут быть шаблонизированы (и в этом случае они называются шаблонами псевдонимов), в то время как typedef — нет. 
- тайпдеф от вектора без указания типа того от чего этот вектор будет. А с Юзингом мы сможем (Пример ниже)

- Директива using существует только в С++, typedef - это наследство от чистого C, в нём нет шаблонов.
- При использовании **typedef** мы создаём синоним имени типа
- А **using** позволяет создать синоним типа, либо включить в текущее пространство имён существующее.
```
typedef float (*func_ptr)(int);
using func_ptr = float (*)(int);
```

Рассмотрим, например, определение синонима для связанного списка, который использует пользовательский распределитель памяти MyAlloc. В случае шаблонов псевдонимов это просто:
```
// MyAllocList<T> является синонимом для std::list<T,MyAlloc<T>>:
template<typename T>
    using MyAllocList = std::list<T, MyAlloc<T>>;

MyAllocList<Widget> lw;    // Клиентский код
```
В случае typedef приходится идти длинным окружным путем:
```
// MyAllocList<T>::type — синоним для std::list<T,MyAlloc<T>>:
template<typename T>
struct MyAllocList {
    typedef std::list<T, MyAlloc<T>> type;
};

MyAllocList<Widget>::type lw; // Клиентский код
```

### Когда применяется decltype() ?
- Для определения типа выражения во время компиляции
- Использование decltype наиболее полезно совместно с auto, так как тип переменной, описанной как auto, известен только компилятору. 
- Если мы хотим задать переменной тот же тип, что и у какой-то ранее определённой переменной
```
auto c = 0; // тип c - int   
auto d = c; // тип d - int            
decltype(c) e; // тип e - int, тип сущности, именованной как c 
```

### Когда применяется declval<T>() ?
https://books.google.ru/books?id=Gn11DwAAQBAJ&pg=PA474&lpg=PA474&dq=%D0%BA%D0%BE%D0%B3%D0%B4%D0%B0+%D0%BF%D1%80%D0%B8%D0%BC%D0%B5%D0%BD%D1%8F%D0%B5%D1%82%D1%81%D1%8F+declval+%D1%81%2B%2B&source=bl&ots=W9JNJ-LUYw&sig=ACfU3U2ozjR2tYjzfYHEAu3cafkOUO-lIw&hl=ru&sa=X&ved=2ahUKEwjQ7smS2ojpAhUwxMQBHSIkBr4Q6AEwBHoECAoQAQ#v=onepage&q=%D0%BA%D0%BE%D0%B3%D0%B4%D0%B0%20%D0%BF%D1%80%D0%B8%D0%BC%D0%B5%D0%BD%D1%8F%D0%B5%D1%82%D1%81%D1%8F%20declval%20%D1%81%2B%2B&f=false

declval<T>() - создаёт значенипе типа Т, не требуя наличия конструктора по умолчанию или любой другой операции

```
//g++  5.4.0
#include <iostream>
//аналог std::declval
template<class T>
std::add_rvalue_reference_t<T> my_declval() noexcept;
//Объявляем шаблонную функцию. Определение нам не нужно, т.к. она 
// никогда не будет вызвана.

struct foo
{
  foo() = delete;
  int bar()
  {
    return 200;
  }
};
 
int main()
{
  int d =500;
  decltype(std::declval<foo>().bar()) n = d;
  
  decltype
  (
    //Вывод типа агрумента. Тут всё ясно.
    my_declval<foo>() 
	// Вернет foo&&, самого вызова не будет, т.к. 
	// decltype работает во время помпиляции. 
    // Происходит только вывод типа, без вызова 
	// (как в sizeof происходит вычисление размера аргумента 
	// во время компиляции).
    .bar()
	// Получим что-то вроде temp_obj.bar(), где 
	// temp_obj - rvalue reference на объекта типа foo 
	// (конечно, сам объект никогда не будет создан, мы работаем на типах).
    // собственно, получаем аналог decltype(foo().bar()), 
	// только вот для его работы ему не нужен дефолтный конструктор,
    // т.к. мы работаем с ссылкой, которую вернул my_declval
  )  
}
```

### В чём преимущества make_shared() перед shared_ptr<> ?

https://habr.com/ru/post/509004/

В чём особенность и плюсы make_shared() перед shared_ptr()
- Принцип работы: Он передаст параметры в конструктор своего параметра шаблонного some_struct и там же внутри вызовет placement_new, чтобы выделить память под объект на месте по известному адресу памяти.
1) **ОСОБЕННОСТЬ_0** У нас пропадает вызов **new**, который имеет место быть, когда мы используем обычный  std::shared_ptr<A> x(new A (p,a,r,a,m,s) ); против auto ptr = make_shared <A> (p,a,r,a,m,s);
2) **ОСОБЕННОСТЬ_1** При вызове shared_ptr происходит две алокации 1. Под сам объект и под 2. управляющий блок
- make_shared же при создании выполняет одну аллокацию и при помощи ellidge - смещения, указатель и управляющий блок лежат рядом. ( make_shared + Выделяет память на счётчики ссылок и на объект одним блоком. Меньше проблем с фрагментацией памяти, всё расположено близко, а значит всё более дефрагментированно хранится )
3) **ОСОБЕННОСТЬ_2** Как мы знаем в shared_ptr 2 указателя на сиьные ссылки и на слабые ссылки. 
- ЕСЛИ мы создали shared_ptr и имеем Ноль Сильных ссылок (shared_ptr-s), но есть ещё Слабые ссылки (weak_ptr-s), ТО объект удалится, но управляющий блок останется жить.
- А ЕСЛИ мы создали make_shared наш объект, то даже если счётчик Cильных ссылок стал ноль, а weak_pointer-счетчик ссылок !=0 (weak-ссылки ещё остались), **ТО ОБЪЕКТ НЕ УДАЛИТСЯ**, ИБО у нас была одна общая аллокация.

4) + Нет проблемы с брошенным исключением при вычислении аргументов функции - foo(shared_ptr<A>(new A), bar()). Порядок вызова аргументов и последовательность вызова bar() / new A / shared_ptr не определена строго стандартом, так что Если вызвалось 1. new A 2. вызвалось bar() и bar() вдруг бросит исключение и т.к. конструктор shared_ptr ещё не вызывался, вызвался конструктор только самого объекта А, и память выделенная под А утечёт

- https://youtu.be/wu2FJcfNn3w?list=PLlb7e2G7aSpTFea2FYxp7mFfbZW-xavhL&t=1952
- https://habr.com/ru/post/509004/

process(std::shared_ptr<Bar>(new Bar), foo());

- Плюсы make_shared:
```
 shared_ptr<some_struct> x(new some_struct(a, b, c));
 auto ptr = make_shared<some_struct>(a, b, c);
```	

### Основные примитивы синхронизации в STL ?
- lock_guard, unique_lock. Разница в том, что вы можете заблокировать и разблокировать std::unique_lock. std::lock_guard будет заблокирован только один раз при построении и разблокирован при уничтожении. освобождает мьютекс (вызывая unlock() )
- mutex
- recursive_mutex
- scoped lock
- condition_variable

- mutex & condition_variable // TO DO //

### std::atomic Атомарные операции
- Гарантированно то, что не в рамках одной операции может быть "выполнена половина"
- Одну атомарную операцию всегда будет исполнять строго ОДИН поток от начала и до конца
- Она всегда в одном потоке будет выполнена ЦЕЛИКОМ и в другом потоке это всегда будет видно.

### Может ли внутри Блокировки использоваться мьютекс std::atomic

### ЕСТЬ ЛИ ГАРАНТИЯ того, что C++ Атомик всегда будет на уровне процессора ?
НЕТ гарантии
- Атомарные операции выполняются на уровне прроцессора.

Есть специальная функция которой это можно проверить c++ atomic is_lock_free 

// TO DO

### Отличие TCP от UDP



# Виртуальные функции и конструктор / деструктор

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

### Q. Есть список - нужно понять - Закальцован он или нет
A. 

# // TO DO //

1. использовать set
2. 
3. 

-----------------------------------------------
### Q: Какие типы умных указателей вы знаете ?
A: 
- unique_ptr, shared_ptr, weak_ptr 
- (auto_ptr, scoped_pointer, intrusive_ptr, ... что-то ещё было)

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

-----------------------------------------------
### Q: Можно ли использовать shared_ptr в нескольких потоках ?
A: Да, но есть ограничения.
``` 
1. Разные потоки могут использовать один и тот же экземпляр shared_ptr,
но только если они не вызывают не констатнтые методы.
2. Для синхронизации одного экземпляра можно использовать атомарные функции.
3. То есть для увеличения/уменьшения счётчика ссылок нужно использовать
специальные атомарные фции, типа apomic_inctement / atomic_decrement.
``` 

-----------------------------------------------
### Q: Контейнеры библиотеки STL ?

**Последовательные контейнеры**
- array - статический непрерывный массив 
- vector - динамический непрерывный массив
- deque - Двусторонняя очередь 
- ! forward_list - односвязный список 
- list - двусвязный список  
**Ассоциативные контейнеры**
- set - Коллекция уникальных ключей, отсортированная по ключам 
- map - Коллекция пар ключ-значение, отсортированная по ключам, ключи являются уникальными 
- ! multiset - Коллекция ключей, отсортированная по ключам 
- ! multimap - Коллекция пар ключ-значение, отсортированная по ключам 
**Неупорядоченные ассоциативные контейнеры**
- unordered_set - Коллекция уникальных ключей, хэш-ключами 
- unordered_map - Коллекция пар ключ-значение, хэширован ключи, ключи являются уникальными 
- ! unordered_multiset - Коллекция ключей, хэш-ключами 
- ! unordered_multimap - Коллекция пар ключ-значение, хешируется по ключам 
**Адаптеры контейнеров**
- stack - реализует стек с помощью контейнера (LIFO структуры данных) 
- queue - адаптируется контейнера обеспечивают очереди (FIFO структуры данных) 
- ! priority_queue - адаптация контейнеров для обеспечения приоритета очереди
**Специальные**
- ! tuple
BOOST-товые НЕ STL
- ! circular_bufer 

-----------------------------------------------
### Q: Разница между map и unordnered_map ?
А: Внутренняя организация памяти внутри контейнеров.
- map - красно-чёрное дерево.
- unordnered_map - хеш-таблица

-----------------------------------------------
### Q: Сложность операций всатвки в map ?
A: Амортизированное О(1)
- Коллизии нет  - О(1)
- Коллизии есть - до О(N) формируются Бакеты

-----------------------------------------------
### Q: Сколько нужно мьютексов для получения dead_lock ?
A: Два

-----------------------------------------------
### Q: Перечислите проблемы многопоточного программирования ?
- гонки потоков
- dead_lock / взаимная блокировка
- доступ к общим ресурсам / памяти
- Синхронизация Большого числа Клиентов / Серверов 
 // TO DO //
- "Проблема обедающих философов"
- "Проблема 10.000 клиентов" или "C10k-problem" - https://ru.wikipedia.org/wiki/C10k

-----------------------------------------------
### Q: Сложность операций с контейнером ?
A: См. Таблицу сложностей операций // TO DO

-----------------------------------------------
### Q: Design PATTERNS / Паттерны проектирования КНИГИ:
Книги:
- http://www.sugardas.lt/~p2d/books/Priemioop.pdf

- http://www.williamspublishing.com/PDF/5-8459-0301-7/part1.pdf
- http://www.williamspublishing.com/PDF/5-8459-0301-7/part2.pdf
- http://www.williamspublishing.com/PDF/5-8459-0301-7/part3.pdf
- http://www.williamspublishing.com/PDF/5-8459-0301-7/part4.pdf
- http://www.williamspublishing.com/PDF/5-8459-0301-7/part5.pdf
- http://www.williamspublishing.com/PDF/5-8459-0301-7/part6.pdf

Ссылки:
- https://habr.com/ru/post/210288/
- http://cpp-reference.ru/patterns/
- https://refactoring.guru/ru/design-patterns/cpp
- http://citforum.ru/SE/project/pattern/


-----------------------------------------------
### Q: Design PATTERNS / Паттерны проектирования ?
https://habr.com/ru/post/210288/

- Синглтон
- Стратегия
- Фабричный метод
- Фассад
- Обзёрвер
- Листенер
- Ресивер
- Фабрика
- Декоратор
- Мост
- Строитель
- Команда
- Состояние
- Наблюдатель
- Прокси

### Q: Многокомпонентные паттерны проектирования / шаблоны проектирования ?

http://yarik-raider.blogspot.com/2010/01/mvc.html

https://habr.com/ru/post/215605/

- MVC - Model-View-Controller (Controller -(changes)-> (changing) Model (changed) -(updates)-> View)
- MVP - Model-View-Presenter
- MVVM - Model-View-View Model
- MVPVM - Model-View-Presenter-View Model

-----------------------------------------------
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
- Красно-чёрное дерево
- Куча
3. Графы
- Ориентированный граф
- Ориентированный ациклический граф
- Бинарная диаграмма решений
- Гиперграф
4. Прочие
- Хеш-таблица
- Стек

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

-----------------------------------------------
### Q: Факториал (Итеративный)
A: IN 5   OUT 125 (1x2x3x4x5) 
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

-----------------------------------------------
### Q: Факториал (Рекурсивный)
A: IN 5   OUT 125 (1x2x3x4x5) 
```
int my_fact(int num)  // вычисление факториала числа num
{
  if (num <= 1)
    return 1;  // если число не больше 1, возвращаем 1
  else
    return (num * fact(num - 1));  // рекурсивный вызов для числа на 1 меньше
}
```

-----------------------------------------------
### Q: Возведение в степень (Итеративно) (Доп.)
A: IN 2^4   OUT 16 (2x2x2x2)
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
A: IN 2^4   OUT 16 (2x2x2x2)
```
int my_pow(int n, unsigned int p) // num^pow
{
  int c = 1;
  for(int i=0; i<p; i++)
    c = n * c;
	
  return c;
}
```

-----------------------------------------------
### Q: Возведение в степень (Итеративно) (с учётом отрицательных степеней)
A: IN 2^4    OUT 16 (2x2x2x2)

A: IN 2^-2   OUT 0  (1/4 = 1/(2x2))
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

-----------------------------------------------
### Q: Возведение в степень (Улучшенный по скорости)
A: IN 2^6    OUT 64 (2x2x2x2x2x2)

A: IN 2^5    OUT 32 (2x2x2x2x2)
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

-----------------------------------------------
### Q: Возведение в степень (Рекурсивно)
A: IN 2^4   OUT 16 (2x2x2x2)
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

-----------------------------------------------
### Q: Из строки с пробелами удалить все пробелы в строке
A:
```
```

-----------------------------------------------
### Q: Найти подстроку в строке
A:
```
```

### Q: Алгоритм Бинарного поиска / Бинарный/Двоичный поиск С++
```
int binary_find(int n, int *x, long A)
{
int m, left, right;
left = 0; right = n-1;
while (true)
 {
    if (left > right) return (-1); // значение не найдено
    m = left + (right - left) / 2;
    if (x[m] < A) left = m + 1;
    if (x[m] > A) right = m - 1;
    if (x[m] == A) return m;
 }
}
```

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




### Теоретический минимум для программиста

```
[Кросспост]

http://codeforces.com/blog/entry/1594?locale=ru

https://salykin-vladimir.ru/2015/07/questions-c-c-plus-plus/#.XHkPoPZuJaQ

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

СТРУКТУРЫ ДАННЫХ !!!

http://itedu.ru/courses/cpp/oop-in-cpp

 !!!!!!!!!!!!!!!!!
http://scrutator.me/post/2014/06/02/objects_memory_layout_p2.aspx

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

ЛЯМБДЫ:
Чтобы решить эту проблему, можно вручную указать возвращаемый тип лямбда-функии, используя синтаксис "-> тип":

 В общем случае его можно записать так:
[captures](arg1, arg2) -> result_type { /* code */ }
arg1, arg2
это аргументы. То, что передается алгоритмом в функтор (лямбду).
result_type
это тип возвращаемого значения. Это может показаться несколько непривычно, так как раньше тип всегда писали перед сущностью (переменной, функцией). Но к этом быстро привыкаешь.

[] - Название функции
() - 

[](double d) -> double { }

void func4(std::vector<double>& v) {
  std::transform(v.begin(), v.end(), v.begin(),
                 [](double d) -> double {
                   if (d < 0.0001) {
                     return 0;
                   }
                   else {
                     return d;
                   }
                 });
} 

Запись "[]" означает, что у функции нет имени, она безымянная, или, говоря по-другому, анонимная. Вместо "[]" можно мысленно подставлять имя "безымяннаяФункция",

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//////////////////  РАБОТА  ///////////////////////////////

CONNECT
Законектились - Сервер начинает нас слушать RECIEVE
Мы отправили эти данные SEND-ом.
Сервер проситал эти данные.
Прокол определён, сервер распарсил эту структуру.
Определил -> Что это запрос такого-то метода.
=> Выполнить этот метод на сервере.
И вернули ответ. SEND.

Пул Потоков - создали  Поток и внутри него 1 коннект между клиентом и счервером, и очередь сообщений / коннектов.
По одному потоку на 1 подсистему (1-го клиента) на 1 КОННЕКТ 
- в рамках коннекта делаем что нужно открыли соединение, закрыли соединение.
Все потоки ПАРАЛЛЕЛЬНО.
```

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
###  ОТ СТАСА ПРО WINDOWS  ///////////////////////////////

Процесс:
- контейнер для потоков
- то чему принадлежит память 
	Даже когда поток выделяет какую-то память, то это на самом деле память расположенное в адресном пространстве процесса, и эта память доступна ВСЕМ потокам

Объект Процесс - в Windows это объект ЯДРА.

Суть в том, что На ЛЮБОМ обекте ЯДРА можно ОЖИДАТЬ. ибо они ВСЕ могут находится в 2-ух состояниях - СИГНАЛЬНОЕ = Занят. НЕСИГНАЛЬНОЕ = Свободен.

Пример: Процесс - автообнолвения. Ждём и жержим ресурсы, пока не обновимся.

Поток, Мьютекс, Евент, Семафор:
	СИГНАЛЬНОЕ = Занят. НЕСИГНАЛЬНОЕ = Свободен.

Все потоки в процессе разделяют одно и тоже адресное пространство.
```
Память: У каждого процесса его адресное пространство ИЗОЛИРОВАННО
	Адрес виртуальный один и тот же, а по факту в физической памяти они РАЗНЫЕ
	Указатель в одном процессе  - адрес память в 1-ом процессе -> физич. память Ячейка_1
	Указатель в другом процессе - адрес память в 2-ом процессе -> физич. память Ячейка_2
```

На 1 виртуальный адрес  есть таблица Связи Виртуальныхи Физических адресов. 

На 1 ВИРТУАЛЬНЫЙ адрес в виртуальном адресном пространстве -> ссылка на Страницу физической памяти.

ВСЯ ПАМЯТЬ ПРОЦЕССА - Его виртуальое Адресное простраснство - ИЗОЛИРОВАНО от Адресных пространств всех Других процессов.

В Заголовке Файла: У него есть несколько СЕКЦИЙ - read/write read_only Execute_image ... 
				Смотрится эта секция, и сколько памяти нужно

### Можно ли себе выделить ЕЩЁ Больше Виртуальной памяти ?
Ответ: ДА !

WIRTUAL_ALLOC - выделяем предварително ДИАВПАЗОН Виртуальных адресов в адресном пространстве.
  (НА этом этапе только выделились адреса, НО НЕТ соответсвия с Физ.адресами на Диске)
  А потом MEM_COMMIT - Привязываем и работаем с этими адресами полноценно.

ПРОЦЕСС имеет 2 СТЕКА:
- Ядерный - 16 Килобайт
- Юзермодный - Большой ~1Мб (Или задаём параметром в фции CreateThread() )

Когда мы делаем ситемный вызов какого-то API - CreateEvent() = СИСТЕМНЫЙ ВЫЗОВ - происходит переключение в Режим Ядра
А это означает, что ECX, EAX, ESP .... что эти регистры, STACK_POINTER - Мы эту информацию в (?) частности передаём через Стек Ядерный.

Подгрузка регистров произойдёт по очереди

Плюс перейдя в режим Ядра, мы используем только этот стек для вызовов Функции, работает ядро со своими внутренними вызовами 
+нам нужен этот стек чтобы когда раскрутка стека началсь бы ОБРАТНО, у нас был бы обратный вызов в нашу функцию юзермодную, к тому кто вызывал.

call - достаёт из стека адрес возврата и передаёт управление на этот адрес возврата. В какой-то момент ПЕРЕКЛЮЧАЕТСЯ "КОНТЕКСТ"

Для этого есть структура CONTEXT - она загружает регистры изменённые в ядре, на значения регистров Юзермодные.
```
СТАРЫЙ СТЕК Ядерный с регистрами - зафиксировался.
// вызвали Ядерную функцию = СИСТЕМЕНЫЙ ВЫЗОВ
СЛЕПОК Стека изменённого после системного вызова возвращается обратно
// Продолжаем работать в Юзермодном Режимею.
```

Юзермодный Стек - страницы памяти - 20 Мегабайт задали, зарезевировалось адресное пространство, но страниц зарезервировалось 2 страницы, а не всё, что мы запросили.

Следовательно: Например, когда мы начали использовать Серьёзную Рекрсию. Что может выявить собой то, что Стек по виртуальным Адресам растёт и мы движемся всё дальше и выше.

Нашей виральыной памяти не поставлена в соответсвии Физическая память. => ОС дополниетльно начинает подгркжать страницы физич. памяти на это Виртуальное Адресное пространство.

В каждом приложении много потоков. И всем запросам передаётся не вся память, ибо они итак не расходуют ВСЁ за РАЗ.


Мьютекс - свободен мьютекс - переводим в СИГНАЛЬНОЕ состояние = НЕ ЗАНЯТ / СИГНАЛЬНОЕ - ЗАНЯТ.

Неэффективность Мьютексов - переключение контекста из Пользоват. в режим Ядра это ценно по врмеени слишком.
Эффектвиность Мьютексов - только то,что его можно использовать в рамках ОДНОГО Процесса.

Ядро это всегда посредник, и в рамках одного процесса, нет смылса задействовать ядро (ибо ЗАТРАТНО), если на то нет необходимости.

### События - С Автосбросом / Без Автосброса.
Waitфункция - Евент сделали Ресет - Евента НЕТ. 
10 потоков сделали WaitForSingleObkect() и в ещё одном потоке мы делаем 

### AUTORESET_EVENT
- SetEvent() -> просыпается ОДИН поток , которые выполнил WaitForSinlgeObject()
	Просыпается и этот Евент переводится в Несигнальное состояние Автоматически.

### MANUALRESET_EVENT
- SetEvent() -> просыпаются ВСЕ потоки , и в  Несигнальное состояние 
	событие надо было бы перевести ЯВНО вызвав ResetEvent().


### (?) ПАМЯТЬ WINDOWS: Одному процессу Можно или нельзя писать в память Другого процесса ?

МОЖНО - НО - Чтобы прочитать/записать что-то в другой процесс, нужно вызвать ЯВНО ReadProcessMemory() / WriteProcessMemory()
	передаём в функции ХЭНДЛ процесса, к которому у нас есть право доступа и указатель на участок адресного пространства в которое нам нужно пичать/читать.


(?) Есть HANDLE. Есть объект Ядра - Именованый Event. 
HANDLE = CreateEvent("Event_name"); // HANDLE - Дескриптор этого объекта Ядра. У каждого процесса есть таблица Дескрипторов.

Но получить доступ к этому объекту по ИМЕНИ евента ДРУГОЙ Процесс может открыть его по имени через OpenEvent();

Handle_1 = фактически актуальлен будет только в рамках одного нашего процесса (в которм мы его СОЗДАЛИ)

Handle_2 = фактически актуальлен будет только в рамках одного нашего процесса (в которм мы его ОТКРЫЛИ)


Мы не получили КОПИЮ Объекта. Мы просто получили Handle_2 - ИДЕКС в таблице Дескрипторов в рамках нашего процесса и более ничего, 
и только через дескриптор мы получаем доступ к самосу объекту, работая через системыне вызовы.


### (?) Как Получить доступ к Объекту Ядра Из Двух процессов:

1) Можно получить доступ - ОТКРЫТЬ обхект по ИМЕНИ.
2) можно получить доступ - по РОДИТЕЛЬСКОЙ связи.
3) Явный вызов функции	 - DUPLICATE_HANDLE()

(2) В CreateProcess() - мы имеем такую штуку, как INHERITABLE - параметр - можно ли этот объект НАСЛЕДОВАТЬ, тоесть Процесс может передать Чайлду Те Индексы в твблице дескрипторов, у которых при создании самих объектов мы выставили параметр INHERITABLE.

```
CreateProcess() // Paren
{
  // .. create some Kernel-Objects
  CreateProcess(INHERITABLE) // Child
  {
    // get all the HANDLE-s of created Inheritable Parent Handles
  }
}
```

### Что за параметр  ALTERABLE

Имеется в  WaitForSingleObjectEX(); EX !!!

ALTERABLE  - Для IPC процедур, она просыпается в тот момент, когда ты ожидаешь на каком-то фале и операция для записи - Когда асинхронная функуия выполнлось. и просыпается, если она выполняется.

Просыпается поток, которые ждёт на файле на каком-то

АСИНХРОННЫЙ вызов когда происходит - в тот момент, когда кто-то пишет или читает из Этого файла.


////////////////////////////////////////////////////////////
### СТРАНИЦЫ ПАМЯТИ - ДОСЛУШАТЬ

// TO DO

////////////////////////////////////////////////////////////
### 64-БИТНАЯ СРЕДА

- Всё больше весит за исключением типа chsr = 1 байту всегда и во всех версиях.
- Переадресация Реестра и Файловой системы - SysWOW64
- Для работы в ней, есть специальные версии всех СИСТЕМНЫХ ВЫЗОВОВ / Функций
- Адресное Пространство Выделяемое под процесс - есть указатель, который ограницен определённым числом

```
32 битная машина использует указатель = 4  Гигабайта = 2^32 байт
64 битная машина использует указатель = 16 Экзобайт = 2^64 байт = 64 ТЕРАБАЙТА (гига, тера, пета, экзо)
//	гигабайт	Гбайт	109	
//	терабайт	Тбайт	1012
//	петабайт	Пбайт	1015
//	эксабайт	Эбайт	1018
```

////////////////////////////////////////////////////////////
### WINDBG - DMP - Дампы - Анализ Дампов

UserMode отладчик   - OlieDBG 

KernelMode отладчик - WinDBG - можно ммотреть регистры, отадчивать, переменные, можно отлаживать как Юзер Мод программы, так и Кернел мод программы.

////////////////////////////////////////////////////////////
# СУРРОГАТНЫЙ ПРОЦЕСС в СОМ

https://www.k-press.ru/cs/2002/1/surrogat/surrogat.asp

https://www.osp.ru/winitpro/2001/03/174740/

https://rsdn.org/article/com/introcom.xml

Суррогатные процессы в COM**

Василий Кораблев Вступление

Реализация COM - сервера в виде in - process(DLL) сервера дает значительные преимущества, например, лучшую структуризацию проекта, раздельную компиляцию модулей, возможность повторного использования модулей и т.п.Но в использовании внутрипроцессного(in - process) сервера есть одно небольшое, но порой критичное неудобство.Их невозможно создавать и вызывать по сети.Решить эту проблему помогают суррогатные процессы.

Формально, суррогатный процесс – это процесс, который порождается специальным EXE - модулем.Отличительной особенностью суррогатный процесса является то, что в него можно загрузить внутрипроцессный COM - сервер.Что же дает использование суррогатных процессов ?

Суррогатный процесс позволяет превратить внутрипроцессный сервер в полноценное DCOM - приложение, давая возможность вызывать компоненты этого сервера удаленно.

Суррогатные процессы позволяют изолировать компонент от клиента, тем самым исключая влияние на него таких критических ошибок, как проходы по памяти или утечки памяти во внутрипроцессном сервере.Таким образом, запуская компоненты в суррогатном процессе, клиент может защитить себя от возможно сбойного серверного кода при доступе к сервисам, предоставляемым сервером.

Один суррогатный процесс может обслуживать нескольких клиентов одновременно.

DLL внутрипроцессного сервера, запущенная в суррогатном процессе, получает все настройки контекста безопасности, установленные для суррогатного процесса.

Возможность использования так называемых системных суррогатных процессов, а также создания пользовательских суррогатных процессов появилась в Windows NT 4.0 SP2 и Windows 95 (с установленным DCOM).

Системный суррогатный процесс.

Идея, стоящая за суррогатными процессами COM – независимость способа реализации сервера от способа его активации.Суррогатные объекты реализуются в формате DLL, но могут быть активированы как внепроцессные серверы.Чтобы разрешить DLL активироваться внутри суррогатного процесса, нужно всего лишь добавить в реестр несколько дополнительных ключей, которые COM SCM(Service Control Manager COM, читается как «скам») просматривает при обслуживании запроса на активацию сервера.Эти ключи могут быть добавлены авторами клиентского кода для недопущения загрузки объекта с потенциально опасным кодом внутрь своего процесса или для удаленного запуска внутрипроцессных серверов через сеть...

////////////////////////////////////////////////////////////

```
(???) PRIVATE наследование Классов:

(???)
Каков размер объекта Foo: 
Каков размер объекта Bar: 

Для 32-битной платформы:
Foo: размер указателя int* = 4 байта 
 (базовый класс указателя на наследников нет) 
 + выравнивание кратное 4-ём
Bar: размер указателя char* = 4 байта 
 + (указатель на класс наследник НЕ БУДЕТ УЧТЁН, ибо наследование было НЕВИРТУАЛЬНОЕ!!!)
 + выравнивание кратное 4-ём

class Foo
{
public:
  Foo(int j) { i = new int[j]; }
  ~Foo() { delete i; }
private:
  int* i;
};

class Bar : Foo
{
public:
  Bar(int j) { i = new char[j]; }
  ~Bar() { delete i; }
private:
  char* i;
};

void main()
{
  Foo* f = new Foo(100);
  Foo* b = new Bar(200);

  *f = *b;

  delete f;
  delete b;
}

////////////////////////////////////////
Размер базового класса.
Размер класса также включает размер непосредственного родительского класа.

class D
{
 private:
   int i1;      // 4 байта
   int i2;      // 4 байта
};
 
class E : public D
{
 private:
   int k;       // 4 байта
};
В этом случае класс размер класса E будет равен 12 байтам.



////////////////////////////////////////
(???) СТАС - Чтобы число умножить на 2 - Логическая операция побитовая какую операцию можно применить 

http://informatics.mccme.ru/mod/book/view.php?id=5510&chapterid=496
```

////////////////////////////////////////
### (???) Бросать исключения в Конструкторе это НОРМАЛЬНО ?

Так как конструктор полностью не выполнился. Объект полностью не сформировался 
=> утечки памяти, если не отработал конструктор НЕ БУДЕТ. Следовательно можно не вызывать delete.
(*) Если мы до того не понаделали всяких MALLOC-ов внутри конструктора или до него в переменной / конструкторе

###  (!!!) Относится ко ВСЕМ объектам Ядра:
В случае если семафор используется внутри одного процесса, можно создать его без имени,
передав в качестве lpName значение NIL. Имя семафора не должно совпадать с именем уже существующего
объекта типов event, mutex, waitable timer, job или file-mapping.


### (?) Мьютекс - что это и для чего нужен.
Нужен для блокировки доступа стороннего потока к защищаемому от доступа семафором участка кода /
того участка кода как правило, внутри которго происходит обращение к какому-то общему ресурсу, который нужен Множеству потоков

### (?) Как происходит "БЛОКИРОВКА" стороннего потока мьютексом ? на вечно ли ? Ставятся ли потоки в очередь ?

### (?) Что делать, если поток заблокировал Мьютекс, началось на нём ожидание, и "забыл" выполнить его UNLOCK
или поток вообще УПАЛ (из-за ошибки / необработанного исключ.) / другой поток его прибил TerminateThread-ом ?
Что тогда будет с рассматриваемым (залоченным) Мьютексом ?

Ответ: Так как поток, в котором был создан / залочен Мьютекс перестал существовать, то и мьтекс освободится.
  Поток - объект ядра. И само же ядро следит за тем, чтобы освободить (отпустится) внутри умершего потока все прочие объекты ядра.
```
НЕСИГНАЛЬНОЕ СОСТОЯНИЕ Мьютекса - Мьютекс залочен потоком (чей-то)
  СИГНАЛЬНОЕ СОСТОЯНИЕ Мьютекса - Мьютекс освобождён потоком (ничей)
```
(?) Если мы Перевели Мьютекс в НЕсигнальное состояния внутри нашего потока, ТО разлочить этот мьютекс, 
тоесть перевести его в Сигнальное состояние извне (то есть из какого-либо другого потока) нельзя, это верно ?

Вопрос: Тоесть Стас, можно ли из другого потока сделать вызов OpenMutex() и выполнить в другом потоке ReleaseMutex ?
ОТВЕТ: НЕЛЬЗЯ - ReleaseMutex выбросит Ошибку, если вызывающий её поток не является Владельцем Мьютекса!!!!!
Поток становится его хозяином до тех пор, пока явно не освободит мьютекс функцией ReleaseMutex, или же не завершится.

Замечение: Запрет запуска второго экземпляра программы - вызываем CreateMutex, смотрим ERROR_ALREADY_EXISTS
если GetLastError вернул нам именно такой флаг, значит программа уже запущена.

### 5) Lock Free - ???

### 6) Spin Lock - ???
Спин-локи в ЯДРЕ, спин это бесконечный цикл for() / while() он в очень высокой частостотой запрашивает состояние
разделяемого ресурса, и постоянно запрашивает. Он не засыпает, он использует процессорное время постоянно, не
заплатив никапельки. Их использовать нужно на очень коротких учасках кода - ибо остальные из-за того, что спинлок
будет отбирать на себя всё процессорное время они протсо будут ждать.

### 7) InterLocked функции - ???

### 8) Семафор  представляет собой объект, позволяющий синхронизировать работу двух или нескольких процессов или потоков.
Для программиста семафор - это просто счетчик. Если счетчик равен N, это означает, что к ресурсу имеют доступ N процессов. 
Счетчик текущего числа ресурсов, максимальное , сколько потоков/процессов могут получить доступ к ресурсу одновременно.
- когда счетчик текущего числа ресурсов становится больше 0, семафор - в СВОБОДНОМ СОТОЯНИИ
- если этот счетчик равен 0, семафор ЗАНЯТ

// Уменьшаем счетчик семафора. Если к этому моменту уже запущено
// MAX_THREAD_COUNT потоков — счетчик равен 0 и семафор в
// несигнальном состоянии. Ожидаюшие потоки будут заморожены до завершения
// одного из запущенных ранее.
   WaitForSingleObject(Semaphore, INFINITE);
  
// Здесь располагается код, отвечающий за функциональность потока - загрузка файла
 
// Поток завершил работу, увеличиваем счетчик семафора и позволяем
// начать обработку другим потокам.
   ReleaseSemaphore(Semaphore, 1, NIL);

ПРИМЕР: SEMAPHORE - WaitForSingleObject   
   http://www.firststeps.ru/mfc/winapi/r.php?117
   
###  9) Критическая секция - Крит секция это объект ПОЛЬЗОВАТЕЛЬСКОГО режима. А Мьютекс это объект ЯДРА.
Особенности: Каждая операция с Мьютексом - захват, освобождение - требует переключение конткестного режима из пользовательского режима в контекст ядра.

Тоесть тратися время на сохраненеие регистров потока пользовательского режима, что-то свопается, что-то сохраняется. ЭТО ДОРОГО.
У КРИТ.СЕКЦИИ - всё находится и происходит в рамках пользовательского процесса / режима, тоесть в ядро перехода напрямую не происходит.
ОСОБЕННОСТЬ: - Так как Мьютекс объект ядра, а значит доступен всем процессам, что означает, что шарить мьютекс между процессами можно по имени.

- А Критическая Секция доступна в рамках ТОЛЬКО ОДНОГО Процесса. (Мьютекс тоже можно использовать так, но он более "мобильный")
ОГОВОРКА: ЕСЛИ есть Критическая Секция - множество потоков пытаются в неё войти, а кто-то один её залочил и потоки ждут ДОЛГО.
Так как "ПОД КАПОТОМ" этой Крит.Секции спрятан Мьютекс - Который ЗАХВАТЫВАЕТСЯ тогда, когда уровень критического ожидания входа в Крит.Секцию является Достаточно большим.

(??? СТАС) Кто определяет, на сколько Долгий интервал ожидания и когда будет задействован "подкапотный" Мьютекс

###  10) Wait-функции WaitForMultipleObjects:
Как вы помните, для синхронизации потоков, обычно требуется временно приостановить выполнение одного из потоков.
- При переводе средствами операционной системы в состояние ожидания, "ожидающий" поток НЕ ЗАНИМАЕТ процессорное время.
- Если флаг bWaitAll равен FALSE, функция вернет управление, как только хотя бы один из объектов подаст сигнал.
А если флаг равен TRUE, это произойдет только тогда, когда сразу все объекты начнут сигнализировать
(как мы увидим, это важнейшее свойство этой функции).


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
 
https://www.youtube.com/watch?v=7KIIKrN9Pf8 
```
ЗАДАЧА: Реализовать очередь с приоритетом (последовательно)

Взять массив - долгие операции О(n)
Дерево, Куча или stack_list - О(log(n)) логарифмическое время для выполнения этих операция 
linked_list - связный список (вставка = О(n/2), а удаление О(1))

Вопрос будет таков: Принципиально ли нам иметь время вставки О(log(n)), или мы
можем пожертвовать в пользу более быстрого удаления элемента, чтобы удалять за О(1)
ЕСЛИ НАМ важно часто удалять из очереди приоритетов ?
```

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

Аутентификация - клиент-серверная часть.
gina-dll / credential_provider / security provider
На JAVA разрабатывался и дорабатывался сервер.
через библиотеку Tshrift <-> С Джава-Сервером
Контролер домена свой.

Т.е. человек И именно в Windows встраивались механизмы
- аутентификации
- заведения новой пользовательской учётной записи
- вывод различных кастомизироыванных сообщений, разновго вида / ошибка / предупреждение / справка
- защита от перехвата, организовывалась через подппись и шифрование base64

(??? СТАС) Защита от перехвата вводимых логина и пароля

(??? СТАС) Защита от подделки отклика (перехват отправленного до сервера) - подпись 

STL / Boost:

STL: контейнеры, алгоритмы.
Boost: 
- split, is_any_of, toupper, tolower, shared_ptr, noncopyable, property_tree::ptree, RNGs, circular_buffer

3. из boost - а использовал, от того, что чаще всего, к тому, что реже всего :
3.1 некоторые вещи для удобства работы со строками - boost::algorithm::split, boost::is_any_of, boost::toupper
3.2 boost::shared_ptr, boost::noncopyable // shared_ptr -> 11 стандарт.
3.3 boost::property_tree::ptree - xml-ный тамошний парсер
3.4 И использовал тамошние генераторы случайных чисел. Исследовал их свойства в своей дипломной работе в институте.
3.5 И однажды поигрался с циркулярным буфером

MFC - разработка интерфейса. GUI-приложение.

COM / DCOM:
- написание COM-сервера
- взаимодействие с БД через OLEDB


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

Блиц:
1. - 2^8 = 256
2. - диапазон 4ё-х байтового числа = -(2^31)-1 - +(2^31)-1 

### Чтобы число умножить на 2 какую логическую побитовую операцию можно применить 
Ответ: ПОБИТОВЫЙ СДВИГ Влево
```
Пример с нечётными:
0000 0011 = 3
0000 0110 = 6

Пример с стпенями двойки:
0000 0001 = 2^0 = 1
0000 0010 = 2^1 = 2
0000 0100 = 2^2 = 4
0000 1000 = 2^3 = 8
0001 0000 = 2^4 = 16
0010 0000 = 2^5 = 32
0100 0000 = 2^6 = 64
1000 0000 = 2^7 = 128
```

### Чему равен размер указателя ? 
- В зависимости от платформы. 4ём байтам какой максиманый размер оперативки можно адресовать ?

### К какому верхнему адресу можно обратиться, если иметь доступ ко всему адресному пространству ?
- Указатель в зависимости от платформы - 64битный платформе он 8 байт занимает, на 32 битной 4 байта.

### Бросать исключения в Конструкторе это НОРМАЛЬНО ?
Да это НОМРАЛЬНО - и память НЕ утечёр ибо объект ещё не создался, ибо конструктор полностью не отработал

### Бросать Исключение в Деструкторе НЕБЕЗОПАСНО
В процессе обработки исключения, исключение не может исходить из самого деструктора, так как это считается ошибкой в работе механизма обработки исключений, приводящей к вызову std::terminate(); Однако деструктор может обрабатывать исключения, которые генерируются вызываемыми им функциями, сам же деструктор не должен возбуждать исключений.

https://habrahabr.ru/post/59349/

```
class Cnt {
private:
  X *xa;
  X *xb;
public:
  Cnt(int a, int b) {
    cout << "Cnt::Cnt" << endl;
    xa = new X(a);
    xb = new X(b);
  }
  ~Cnt() {
    cout << "Cnt::~Cnt" << endl;
    delete xa;
    delete xb;
  }
};
```
Казалось бы, ничего не забыли. (Хотя, строго говоря, конечно забыли как 
минимум конструктор копирования и операцию присвоения, которые бы корректно
работали с нашими указателями; ну да ладно.)

Воспользуемся этим классом:
```
try {
  Cnt c(1, 0);
} catch (...) {
  cout << "error" << endl;
}

И разберёмся, что и когда будет конструиваться и уничтожаться.

Сперва запустится процесс создания объекта Cnt.
В нём будет создан объект *xa
Начнёт создание объекта *xb...
… и тут произойдёт исключение

Всё. Конструктор прекратит свою работу, а деструктор объекта Cnt 
вызван не будет (и это правильно, объект-то не создался). Итого, что мы имеем? 
Один объект X, указатель на который (xa) навсегда потерян. 
В этом месте мы сразу получаем утечку памяти, а возможно, получаем утечку 
и более ценных ресурсов, соктов, курсоров...

Обратите внимание, что это одна из самых неприятных ситуаций, утечка возникает 
не всегда, а только при определённых аргументах (первый — не ноль, а второй — ноль).
Отыскать такие утечки бывает очень сложно.

Очевидно, такое решение годится только для очень простеньких программок, 
которые в случае любого исключения просто беспомощно валятся и всё.

Самое простое, надёжное и естественное решение — хранить объект по значению

Пример:

class Cnt {
private:
  X xa;
  X xb;
};
```

////////////////////////////
### Что выведется на экран:

```
struct A0
{
  A0()
  { printf("A0"); }
  virtual ~A0()
  { printf("~A0"); }
};

struct A
{
  A()
  { printf("A"); }
  virtual ~A()
  { printf("~A"); }
};

struct B : public A
{
  B() 
  {
    printf("B");
    throw 1;
  }
  ~B()
  {
    printf("~B");
  }
};


int main()
{
/* // ERROR OPTION:
  try
  {
    A0* a0 = new A0;
    A*  aa = new A;
    A*  ab = new B;
  }
  catch (...)
  {
    return 1;
  }

  // A0 A A B ~A(от B) и ВСЁ !!!
//*/

//* // SOLUTION_1
  try
  {
    std::unique_ptr<A0> a0(new A0());
    std::unique_ptr<A> aa(new A());
    std::unique_ptr<B> ab(new B());
  }
  catch (...)
  {
    return 1;
  }

  // A0 A A B ~A(от B) ~A ~A0
//*/
```

ЯВНО ЧТО-ТО Стасовское, из того, что разбирали со Стасом.

////////////////////////////////////////////////////////////
### СИНХРОНИЗАЦИЯ
```
class A;
A* p = new A();
```
можео ли применять адресную арифметику к подобному указателю ?
МОЖЕМ !

p - не объект ядра

### ЧТО ТАКОЕ ОБЪЕКТ ЯДРА ?
http://novostynauki.com/e-ntsiklopediya/lektsii/sistemnoe-programmnoe-obespechenie/yadro-operatsionnoj-sistemy-funktsii-yadra-ob-ekty-yadra-ob-ekty-yadra-na-primere-os-windows-mikroyadro/

Для работы с важными системными ресурсами ОС Windows создает объекты:
- Объекты ядра. Объект – блок памяти, выделенный ядром для своих целей и доступный только ядру.
- Объект ядра содержит имя объекта
- класс защиты объекта 
- счётчик количества пользователей и другую информацию (смещение при открытии файла и т.д.)
- Все объекты имеют описатели (Handle). Большинство объектов обладают свойством наследования.
Таблица Хэндлов.
 

- управление которыми осуществляет менеджер объектов.
(Когда приложение открывает файл, создает поток или семафор, оно получает описатель ( handle ) 
соответствующего объекта (см. рис. 4.1). Например, после выполнения программного оператора)
- Объекты ядра принадлежат ядру, а не процессу. 
- если Ваш процесс вызывает функцию, создающую объект ядра, а затем завершается, объект ядра может быть не разрушен
- если созданный Вами объект ядра используется другим процессом, ядро запретит разрушение объекта до тех пор, пока от него не откажется и тот процесс.
- Ядру известно, сколько процессов использует конкретный объект ядра, посколь ку в каждом объекте есть счетчик числа его пользователей.


### ГДЕ ХРАНЯТСЯ и НАХОДЯТСЯ Физически Уже созданные объекты ядра?
Мьютекс создали. Можем ли мы его переложить куда-нибудь ?
Обращение к ним возможно через HANDLE = Уникальный идентификатор.
Ядро возвращает нам 

Мы создали Мьютекс.
Мы работаем с Хэндлами через специальные Wait-функции и т.д.
НЕ забываем ЗАКРЫТЬ ХЭНДЛ

### Мьютекс - что это и для чего нужен.
Нужен для блокировки доступа стороннего потока к защищаемому от доступа семафором участка кода /
того участка кода как правило, внутри которго происходит обращение к какому-то общему ресурсу, который нужен Множеству потоков

Мьютексы — это простейшие двоичные семафоры, которые могут находиться в одном из двух состояний — отмеченном или неотмеченном (открыт и закрыт соответственно). Когда какой-либо поток, принадлежащий любому процессу, становится владельцем объекта mutex, последний переводится в неотмеченное состояние. Если задача освобождает мьютекс, его состояние становится отмеченным.
Задача мьютекса — защита объекта от доступа к нему других потоков, отличных от того, который завладел мьютексом. В каждый конкретный момент только один поток может владеть объектом, защищённым мьютексом. Если другому потоку будет нужен доступ к переменной, защищённой мьютексом, то этот поток блокируется до тех пор, пока мьютекс не будет освобождён.

Если другому потоку будет нужен доступ к переменной, защищённой мьютексом, 
то этот поток БЛОКИРУЕТСЯ до тех пор, пока мьютекс не будет освобождён.

### Как происходит "БЛОКИРОВКА" стороннего потока мьютексом ? на вечно ли ? Ставятся ли потоки в очередь ?

### Что делать, если поток заблокировал Мьютекс, началось на нём ожидание, и "забыл" выполнить его UNLOCK
или поток вообще УПАЛ (из-за ошибки / необработанного исключ.) / другой поток его прибил TerminateThread-ом ?
Что тогда будет с рассматриваемым (залоченным) Мьютексом ?

Ответ: Так как поток, в котором был создан / залочен Мьютекс перестал существовать, то и мьтекс освободится.

Верно СЛЕДУЮЩЕЕ:
- Свободном состоянии (signaled) - СИГНАЛЬНОЕ СОСТОЯНИЕ Мьютекса - Мьютекс освобождён потоком
- Занятом (not signaled) - НЕСИГНАЛЬНОЕ СОСТОЯНИЕ Мьютекса - Мьютекс залочен потоком  

### Если мы Перевели Мьютекс в НЕсигнального состояния внутри нашего потока, ТО разлочить этот мьютекс, 
тоесть перевести его в Сигнальное состояние извне (то есть из какого-либо другого потока) нельзя, это верно ?

Мьютекс - mutual exclusion - взаимное исключение

Смысл: Не допустить более одного потока к использованию какого-то ресурса (файл/переменная/сокет)


Семафоры – объекты ядра, использующиеся для учета ресурсов. Семафор имеет внутри счетчик.
Этот счетчик снизу ограничен значением 0 (семафор занят) и некоторым верхним значением N.
 В диапазоне 1..N семафор является свободным. Семафоры можно считать обобщением Mutex на несколько ресурсов


### Семейство Interlocked-функций
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

Interlocked-функции не переводят процесс в режим ожидания, работают 
в пользовательском режиме, и поэтому обладают довольно большим быстродействием,
так что если необходима синхронизация для доступа к одной переменной, 
следует использовать именно их.

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

Используются в случае, если разным потокам необходимо изменять одну и 
ту же переменную. Например, увеличивать счетчик числа операций. 
Код вида var++; компилятором генерируется в что-то вроде
```
MOV EAX, [var]; 1
INC EAX; 2
MOV [var], EAX; 3
```

```
Соответственно, если будут два потока с таким кодом, то нельзя гарантировать,
что выполняться строки 1,2,3 первого потока, а потом 1,2,3 второго, 
первая строчка второго потока может выполнится до того, как первый запишет 
новое значение в переменную, и тогда второй поток изменит старое значение 
и перетрет изменения первого. Использование Interlocked-функций гарантирует, 
что операция будет выполнена атомарно, и описанная выше проблема отпадает.
 При использование процессоров х86 они выдают по шине аппаратный сигнал, 
 не давая другим процессорам процессорам обратится по тому же адресу памяти 
 на время работы функции.

В примере с инкрементом желанного результат можно достичь двумя функциями 
- InterlockedExchangeAdd(&var, 1), которая возвратит начальное значение var,
 либо InterlockedIncrement(&var), которая возвращает уже инкрементированое 
 значение переменной. Также существуют их 64-разрядные версии 
- InterlockedIncrement64 и InterlockedExchangeAdd64.
 
Существуют ещё функции, которые реализуют: 
- декремент: LONG InterlockedDecrement(LONG volatile *Addend), 
возвращает значение декрементированной переменной и 64-битная версия — InterlockedDecrement64
- монопольную замены текущего значения: LONG InterlockedExchange(LONG volatile *Target, LONG Value),
- логические операции: логическое «И» LONG InterlockedAnd(LONG volatile *Destination, LONG Value),
логическое «ИЛИ» InterlockedOr и сумму по модулю два InterlockedXor. 
- выполнение сравнения и присвоения как атомарной операции: 
LONG InterlockedCompareExchange(LONG volatile *Destination, LONG Exchange, LONG Comparand)

 
Большая часть синхронизации потоков связана с атомарным доступом (atomic access) 
— монопольным захватом ресурса обращающимся к нему потоком. Возьмем простой пример

// определяем глобальную переменную lorig g_x = 0;

DWORD WINAPI ThreadFunc1(PVOID pvParam) { 
g_x++; 
return(0); }

DWORD WINAPI ThreadFunc2(PVOID pvParam} { 
g_x++; 
return(0); }

Я объявил глобальную переменную g_n и инициализировал ее нулевым значением. 
Теперь представьте, что я создал два потока: один выполняет ThreadFunc1,
другой — ThreadFunc2 Код этих функций идентичен: обе увеличивают значение 
глобальной переменной g_x на 1. Поэтому Вы, наверное, подумали: когда оба 
потока завершат свою работу, значение g_x будет равно 2. 
Так ли это? Может быть. При таком коде заранее сказать, каким будет конечное 
значение g_x, нельзя. И вот почему. Допустим, компилятор сгенерировал 
для строки, увеличивающей g_x на 1, следующий код:

MOV EAX, [g_x] , значение из g_x помещается в регистр
INC EAX ; значение регистра увеличивается на 1
MOV [g_x], EAX ; значение из регистра помещается обратно в g_x

Вряд ли оба потока будут выполнять этот код в одно и то же время. 
Если они бу дут делать это по очереди — сначала один, потом другой,
тогда мы получим такую картину:

MOV EAX, [g_x] ; поток 1 в регистр помещается 0
INC EAX ; поток V значение регистра увеличивается на 1
MOV [g_x], EAX , поток 1. значение 1 помещается в g_x
MOV EAX, [g_x] ; поток 2 в регистр помещается 1
INC EAX ; поток 2. значение регистра увеличивается до 2
MOV [g_x], EAX , поток 2. значение 2 помещается в g_x

После выполнения обоих потооков значение g_x будет равно 2 
Это просто заме чательно и как раз то, что мы ожидали: 
взяв переменную с нулевым значением, дваж ды увеличили ее 
на 1 и получили в результате 2. Прекрасно. Но постойте-ка, 
ведь Windows — это среда, которая поддерживает многопоточность 
и вытесняющую мно гозадачность. Значит, процессорное время в 
любой момент может быть отнято у од ного потока и передано другому. 
Тогда код, приведенный мной выше, может выпол няться и таким образом:

MOV EAX, [g_x] ; лоток V в регистр помещается 0
INC EAX ; поток 1. значение регистра увеличивается на 1

MOV EAX, [g_x] ; поток 2 в регистр помещается 0 
INC EAX ; поток 2. значение регистра увеличивается на 1 
MOV [g_x], EAX , поток 2. значение 1 помещается в g_x

MOV [g_x], EAX , поток V значение 1 помещается в g_x

А если код будет выполняться именно так, конечное значение 
g_x окажется рав ным 1, а не 2, как мы думали! Д

```
 
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

# ПОДХОДЫ СИНХРОНИЗАЦИИ ПОТОКОВ

### 4 подхода, для организации Синхронизания потоков:

https://www.youtube.com/watch?v=MU6AeNsZ6MM


### 1) МОНИТОР - Объект умеющий в Критической Секции "Приотпускать" замок.
Чтобы после наступления какого-то события (await for condition)
возобносить свою работу (конкурируют за замок на общих началах)
ПРИМЕР: поток deque(), ждёт пока в очереди не появится элемент.
Проблема мониторов Lost wake-up - ненаступление условия - лечится установкой timeout-ов.

### 2) Read-Write Locks - замки устанавливающие уровень доступа к данным.
Возможен захват Reader-замка несколькими потоками.
НЕВОЗМОЖЕН при захваченом Writer-замке.
Пример: SELECT/UPDATE в БД, Доступ к файлу.

### 3) Reentrant lock - замок, который можно повторно захватить, 
если он уже принадлежит исполняющемуся потоку.

### 4) СЕМАФОР - примитив пускающий в "критическую секцию", не более N потоков.
Пример: Распределение ограниченного числа (счтёных) ресурсов 
- сокетов, ядер, IO-операций, подключений к БД, лицензий. 


### (1) Пример: наш объект заблокирован потоком, но этот поток должен 
выполнить какую-то операцию, для которой у него не хватает ресурсов.
К примеру_1 - в него есть вложенная блокировка. Он захватил объект_1, 
и после этого же он хочет захватиь объект_2, но объект_2 для него недосупен.
К примеру_2 - Он захватил объект_1, и он дожидается определённого времени, 
для того, чтобы исполнить свои действия, и не даёт другим в дуоступ объект_1.
Если он будет просто ждать - это неэффективно

Было бы эффективнее, если бы ПОКА наш поток ожидает освобождения объекта_2,
он бы отпустил временно объект_1.

### МОНИТОР - отпускает замок над нашим объектом, до наступления какого-то события.
Монитору должно быть чётко поисано само событие.
Пример МОНИТОРА в жизни - Поток ждёт Момент, когда надо взять элемент из очереди. 
В случае, если в очереди Элементов НЕТ, он будет ждать очень долго,
а очеред при этом будет удерживаться. Но мы можем и ДОЛЖНЫ дать другому потоку
возможность дать отпущеную очередь, чтобы он положил в неё элемент.
МИНУСЫ МОНИТОРОВ - Проблема Пропущенного события - Монитор пропустил событие,
по которому он должен проснуться, потомучто оно наступило, и монитор этого не заметил.
Решение тут - выставлять таймеры на процесс ожидания/СНА, чтобы он Подержал объект, 
отпустил его ушёл спать, и если событие которого он ожидает так и не наступило, 
в течение 30 секунд, он просыпается автоматически и пытается сново получить доступ к объекту.

### (2) Если мы открываем файл на ЧТЕНИЕ, то это не мешает открыть и ЧИТАТЬ файл кому-то другому.
У них имеется РАЗНЫЙ УРОВЕНЬ доступа к данным, т.е. замок может 2 Уровня
- Захват объекта на ЧТЕНИЕ - Он НЕ МЕШАЕТ другим потокам так же захватывать это объект на чтение
	НО, тогда он заблокирован на ЗАПИСЬ и изменение такого объекта
- захват объект на ЗАПИСЬ - будет доступен другим потокам и на чтение и на запись.

ПРИМЕР: Запросы в БД SELECT / UPDATE:
  SELECT может быть множественным, и не блокирует табоицу.
  UPDATE - сначала будет исполнено изменение, а потом передатся управление для SELECT-a

### (3) Дважды залочить один и тот же замок. Используем чужой код, и в нашей внешней части кода нам
  нужно заблокировать что-то, что находится и заблокированно ещё и внтури.
	
### (4) Семафор это организующий примитив, который позволяет некоторому колчиеству потоков ожновреиенно 
  заходить в критическую секцию, но не более чем обозначенному количеству.

Система БД, которая способна обеспечить по лицензии не более чем 5 подключений.
Мы делаем ПУЛ подключений, ограничиваем этот Пул Семафором, 
и использовать то число подключений / потоков,
солько нужно, согласно ограничению

МЬЮТЕКС:
- Крупнозернистая синхронизация.	

	
АБСТРАКТНЫЕ ЗАДАЧИ МНОГОПОТОЧНОСТИ / СИНХРОНИЗАЦИИ:
- Обедающие философы	
- Дилемма заключённого
- Людоед и шапки 2-ух цветов

### 1) Критические Секции
### 2) Мьютексы
### 3) События
### 4) Семафоры
### 5) Lock Free
### 6) InterLocked функции



# BO QUAN

```
Многопроцессность:	
процесс_1 <-> Межпроцессное взаимодействие (каналы) <-> процесс_2
файлы, пайпы, сообщения

Многопоточность:
поток_1 <-> разделяемая память <-> поток_2

+Многопоточность - поток проще и быстрее запустить
    Чтобы запустить процесс, ОС необходимо выделить 
    под него гораздо больше ресурсов.
    Процессу нужно больше безоаснсоти, чтобы они 
    не наступали друг на друга в ОС 
    (процессы же работают через посредников)

    Потоки же существуя в рамках процессов живут 
    более раздельно друг от друга (встречаясь и 
    работая только с разделяемой памятью/ресурсами)

Межпотоковое взаимодействие сильно 
более быстрое, чем Межпроцессное

						
Треды:
  Джойн.
  Можно проверить джоинабл он или нет

////////////////////////////////////////////////////////////
THIS_THREAD

std::this_thread::sleep_for(chrono::second(1));
Всегда позволяет нам узнать НАШ поток, или выполнить именно в ТЕКУЩЕМ потоке что-то
	
////////////////////////////////////////////////////////////
JOIN

В этом примере, thr — это объект, представляющий поток, в котором будет выполняться функция threadFunction().
join() -  блокирует вызывающий поток (в нашем случае — поток main) до тех пор, пока thr 
(а точнее threadFunction()) не выполнит свою работу. Если функция потока возвращает значение 
— оно будет проигнорировано. Однако принять функция может любое количество параметров.
						

////////////////////////////////////////////////////////////
  ПРОБЛЕМЫ СИНХРОНИЗАЦИИ:
- Гонки (Data Race)
- Взаимные блокировки (DeadLock)

////////////////////////////////////////////////////////////
						
Параметры в функцию потока всегда передаются по значению (копия объекта), а не по ссылке.
Чтобы передача происходила по ссылке нужно:

fun(string& msg) {}

main
{
  string s = "1";
  thread t1(fun, ref(s)); 
  thread t1(fun, move(s)); // второй вариант передачи параметра в поток 
}

thread tr2 = tr1; // ERROR will NOT work
tr2 = move(tr1); // перенесём содержимое потока в поток tr2
 
////////////////////////////////////////////////////////////

Гонки ... 

Мы можем разграничить доступ к общему используемому 
ресурсу в нашем случае это cout::  стандартнй вывод:

////////////////////////////////////////////////////////////
обычный мьютекс:
mutex mu;

МНОЖЕСТВО ПОТОКОВ Обращаются именно к этой функции и к общему для всех обхекту  cout
fun(int i)
{
	mu.lock();
	cout << i;
	mu.umlock();
}

лок - один поток
один поток сделал своё дело
анлок - один поток

и сразу после
лок - второй поток
второй поток сделал своё дело
анлок - второй поток

и так каждый поток гарантированно, не задев объект синхронизации одноврмеенно.
или в нужной, прогеру последовательности

////////////////////////////////////////////////////////////
mutex mu;
lock_guard<mutex> guard(mu);

lock_guard - Реализует RAII:
Автоматически выполнит лок и анлок в рамках заданной единицы трансляции
fun()
{ // начало блока
  lock_guard<mutex> guard(mu); // от момента инициализации, до конца  БЛОКА  функции
} // конец блока

// это работает, только если множество потоков будет вызывать именно эту 

////////////////////////////////////////////////////////////
ПРОБЛЕМА.
любой другой поток, тоесть новая функция которую мы введём и которая будет юзать  cout
и если в ней не будет того же самого мутекса с лок/анлок или мутекс_гуарда с этим мутексом, то Ж 

Тогда просто создаём класс, с мьютексом, лочим функцию которую будем шарить между потоками
с использованием лок-гарда и используем объект класса в потоках по ссылке, и в основном потоке просто инстанс.

Никогда не должен возвращаться сам объект, разделяемый между потоками.


```

# ДЕДЛОК - взаимная блокировка

```
mutex m_1
mutex m_2

fun_1()
{
  lock_guard<mutex> locker(m_1);
  lock_guard<mutex> locker2(m_2);
  // защищаемый объект_1 1-ым мьютексом
  // защищаемый объект_2 2-ым мьютексом
}

fun_2()
{
  lock_guard<mutex> locker2(m_2);
  lock_guard<mutex> locker(m_1);
  // защищаемый объект_1 1-ым мьютексом
  // защищаемый объект_2 2-ым мьютексом
}

Синхронно выполняем обе функции:
1 шаг - 1поток в fun - lock_guard<mutex> locker (m_1); залочили m_1
2 шаг - 2поток - lock_guard<mutex> locker2(m_2); залочили m_2
3 шаг - 1поток - lock_guard<mutex> locker2 (m_2); ХОЧЕТ ЗАЛОЧИТЬ m_2, Но он уже залочен в первой строчке второй функции во втором потоке
4 шаг - 2поток - lock_guard<mutex> locker1 (m_1); ХОЧЕТ ЗАЛОЧИТЬ m_1, Но он уже залочен в первой строчке первой функции в  первом потоке
ДЕДЛОК !

РЕШЕНИЕ_0:
Использовать по ОДНОМУ локу анлоку в ОДНОЙ области видмости

fun_1()
{
	part_1
	{
		lock_guard<mutex> locker(m_1);
		// защищаемый объект_1 1-ым мьютексом
	}
	part_2
	{
		lock_guard<mutex> locker2(m_2);
		// защищаемый объект_2 2-ым мьютексом
	}
}

fun_2()
{
  lock_guard<mutex> locker(m_1);
  lock_guard<mutex> locker2(m_2);
  // защищаемый объект_1 1-ым мьютексом
  // защищаемый объект_2 2-ым мьютексом
}

РЕШЕНИЕ_1:
Лочить мьютексы ПОСЛЕДОВАТЕЛЬНО, в каждой из функций в ОДИНАКОВОЙ последоваткельности

fun_1()
{
  lock_guard<mutex> locker(m_1);
  lock_guard<mutex> locker2(m_2);
  // защищаемый объект_1 1-ым мьютексом
  // защищаемый объект_2 2-ым мьютексом
}

fun_2()
{
  lock_guard<mutex> locker(m_1);
  lock_guard<mutex> locker2(m_2);
  // защищаемый объект_1 1-ым мьютексом
  // защищаемый объект_2 2-ым мьютексом
}

РЕШЕНИЕ_2 STL:

fun_1()
{
  std::lock(m_1, m2);
  lock_guard<mutex> locker(m_1, std::adopt_lock);  // adopt_lock - говорит locker-у,
                    // что мьютекс m_1 уже залочен, так что достаточно просто 
  lock_guard<mutex> locker2(m_2, std::adopt_lock);  // "получить владение"
                    // этим мьютексом, чтобы удалить его как выйдем за пределы области
		    // видимости (out of scope), надо просто не забыть освободить мутекс
  // защищаемый объект_1 1-ым мьютексом
  // защищаемый объект_2 2-ым мьютексом
}

fun_2()
{
  lock_guard<mutex> locker(m_1);
  lock_guard<mutex> locker2(m_2);
  // защищаемый объект_1 1-ым мьютексом
  // защищаемый объект_2 2-ым мьютексом
}

НЮАНС:
- стоит избегать вызова функцции определённой пользователем
на том промежутке кода, где вы используете lock_guard<mutex>

fun()
{
	lock_guard<mutex> locker(m);
	user_function();
}

- задать приоритетность мьютексам.
Чтобы пока поток залочил низкоуровневый мьютекс, он не может залочить Высокоуровневый мьютекс

- С защитой объектов от захвата нужно поступать очень аккуратно и с ВЫСОКОЙ ГРАНУЛЯРНОСТЬЮ
"Хороший" Лок - лочит маленький участок кода. Однако это может породить дедлоки
"Плохой" - большие участки кода от исполнения. Однако треды могут долго ждать освобождения ресурсов
"Золотая середина" - которой нет ...



////////////////////////////////////////////////////////////

https://habrahabr.ru/post/182610/

- lock_guard: когда объект создан, он пытается получить мьютекс (вызывая lock()), 
	а когда объект уничтожен, он автоматически освобождает мьютекс (вызывая unlock())

- unique_lock: в отличие от lock_guard, также поддерживает отложенную блокировку,
	временную блокировку, рекурсивную блокировку и использование условных переменных	


Конструкторы классов «оберток» могут принимать параметр, определяющий политику блокировки:
- defer_lock типа defer_lock_t: не получать мьютекс
- try_to_lock типа try_to_lock_t: попытаться получить мьютекс без блокировки
- adopt_lock типа adopt_lock_t: предполагается, что у вызывающего потока уже есть мьютекс

Для решения этой проблемы можно использовать 
- std::lock, который гарантирует блокировку безопасным (с точки зрения взаимоблокировки) способом:

     std::lock(c1._lock, c2._lock); 
     c1.remove(value);
     c2.add(value);
     c1._lock.unlock();
     c2._lock.unlock();

- lock: блокирует мьютекс, используя алгоритм избегания deadlock'ов (используя lock(), try_lock() и unlock())
- try_lock: пытается блокировать мьютексы в порядке, в котором они были указаны	 

```

# ЮНИКЛОК - взаимная блокировка

```

1) позволяет сделать отложенную РАЗблокировку:
void shared_print()
{
	unique_lock<mutex> locker(m);
	// то, что требует синхронизации через мьютекс m
	locker.unlock(); // позволяет сделать это раньше, чем конец области видимости
	// от этого момента нам залоченный мьютекс не нужен
}

2.1) Можно создать объект locker, но при этом НЕ ЗАЛОЧИТЬ мьютекс
     Реализует механизм "ОТЛОЖЕННАЯ БЛОКИРОВКА"
2.2) МНОЖЕСТВО РАЗ делать ЛОК / АЛОК в рамках одной области видимости / одного блока { }
	 lock_guard так не умеет
2.3) Может  std::move() - быть ПЕРЕМЕЩЁН - функцией ПЕРЕМЕЩЕНИЯ, и при перемещении
     ВЛАДЕНИЕ Мьютексом переходит от одного unique_lock_1 к unique_lock_2
	 std::unique_lock<mutex> locker_1(mutex);
	 ...
	 std::unique_lock<mutex> locker_2 = std::move(locker_1);
	 
НЮАНС - Он БОЛЕЕ ТЯЖЁЛЫЙ, чем lock_guard (который более ЛЕГКОВЕСНЫЙ) 
	 
void shared_print()
{
	unique_lock<mutex> locker(m, std::defer_lock); // m - будет
	            // не залочен при создании locker. его нужно будет залочить вручную
	//сначала делаем то, что не требует синхронизации
	locker.lock(); // 1-ЫЙ РАЗ
	// то, что требует синхронизации через мьютекс m
	locker.unlock(); // позволяет сделать это раньше, чем конец области видимости

	// от этого момента нам залоченный мьютекс не нужен
	locker.lock(); // 2-ОЙ РАЗ
	// то, что требует ПОВТОРНОЙ синхронизации через мьютекс m
	locker.unlock();	
}

////////////////////////////////////////////////////////////
ЛЕНИВАЯ ИНИЦИАЛИЗАЦИЯ
Отложенная инициализация
lazy initialisation

class LogFile
{
	std::mutex m_mtx;
	std::ofstream m_f;
public:
	LogFile()
	{
		m_f.open("file.txt");
	}
	
	void shared_print(std::string _s)
	{
		unique_lock<mutex> locker(m_mtx);
		m_f << "PRINT: " << _s << std::endl;
	}
};

Тут в конструкторе мы открываем файл.
НО Что если в коде, разраб, пользующийся
нашим классом никогда не вызовет shared_print
И зачем нам тогда открытый файл вприцнципе ?

Для этого Инициализируем и откроем Файл ТОЛЬКО
внутри самой функци, единожды, кода она вызовется

class LogFile
{
  std::mutex m_mtx;
  std::ofstream m_f;
public:
  LogFile() { }
  
  void shared_print(std::string _s)
  {
    if(!m_f.is_open())	      // Lazy initialisation
    { m_f.open("file.txt"); } // ИДЕОМА - инициализация до первого использования

    unique_lock<mutex> locker(m_mtx);
    m_f << "PRINT: " << _s << std::endl;
  }
};

////////////////////////////////////////////////////////////
ПОТОКО  НЕ  БЕЗОПАСНЫЙ КОД

class LogFile
{
  std::mutex m_mtx_1;
  std::mutex m_mtx_2;
  std::ofstream m_f;
public:
  LogFile() { }
  
  void shared_print(std::string _s)
  {
    if(!f.is_open()) {			    // cтр.1
    unique_lock<mutex> locker_2(m_mtx_2); // cтр.2
      m_f.open("file.txt");		    // cтр.3
    }
    					    // cтр.4
    unique_lock<mutex> locker_1(m_mtx_1);
    m_f << "PRINT: " << _s << std::endl;
  }
};

НЕБЕЗОПАСНОСТЬ: Имеем 2 потока (В рамках 1 шага 2 потока могут выполнить параллельно по одной операции в рамках одной строки):
-- шаг 1
поток 1 (стр.1): зашёл, увидел, что файл НЕ открыт, заходит в функ.
-- шаг 2
поток 1 (стр.2): лочит мьютекс, собирается перейти к строчке 3 и открыть файл
поток 2 (стр.1): зашёл, увидел, что файл ЕЩЁ НЕ открыт (поток 1 в это же время только залочил мьютекс, но ещё не перешёл на строчку 3)
-- шаг 3
поток 1 (стр.3): открывает файл		-- ФАЙЛ ОТКРЫТ (в потоке 1) --
поток 2 (стр.2): лочит мьютекс
-- шаг 4
поток 1 (стр.3): выходит из функции
поток 2 (стр.2): открывает файл		-- ФАЙЛ ОТКРЫТ (в потоке 2) --
--- РЕЗУЛЬТАТ: Файл одновременно открыт в Двух потоках.... Плохо просинхронизировали ....


// РЕШЕНИЕ  НЕправильное:

class LogFile
{
  std::mutex m_mtx_1;
  std::mutex m_mtx_2;
  std::ofstream m_f;
public:
  LogFile() { }
  
  void shared_print(std::string _s)
  {
    {
    unique_lock<mutex> locker_2(m_mtx_2);	// cтр.1
      if(!f.is_open()) 				// cтр.2
      { m_f.open("file.txt"); }			// cтр.3
    }
    						// cтр.4
    unique_lock<mutex> locker_1(m_mtx_1);
    m_f << "PRINT: " << _s << std::endl;
  }
};
	
ТАКОЕ решение плохое, ибо оно требует захвата мутекса КАЖДЫЙ РАЗ,
в каком-то смысле теряется как таковая "многопоточность" +нагрузка процессора и потеря процессорного времени
ПРИ том, что файл нам вообще говоря нужно открыть ЕДИНОЖДЫ


////////////////////////////////////////////////////////////
// РЕШЕНИЕ  ПРАВИЛЬНОЕ:

- ЕСЛИ нужно сделать что-то ЕДИНОЖДЫ в одном из потоков

ONCE_FLAG - позволяет чему-то выполниться в одном из потоков ЕДИНОЖДЫ

class LogFile
{
  std::mutex m_mtx_1;
  std::once_flag flag;
  std::ofstream m_f;
public:
  LogFile() { }
  
  void shared_print(std::string _s)
  {
    call_once(_flag, [&]() { m_f.open("log.txt"); }); // Файл 
                    //будет открыть ЕДИНОЖДЫ Одним из потоков
    unique_lock<mutex> locker_1(m_mtx_1);
    m_f << "PRINT: " << _s << std::endl;
  }
};



////////////////////////////////////////////////////////////
// Condition Variable - УСЛОВНЫЕ ПЕРЕМЕННЫЕ

std::deque<int> q; // доступ к очереди - ГОНКИ потоков! (кто быстрее запихнёт/удалит)
std::mutex mu;

void fun_1() // producer
{
  int count =10;
  while(count > 0)
  {
  unique_lock<mutex> locker(mu);
    q.push_front(count);
    locker.unlock();
    sleep_for(chrono::seconds(1));
    count--;
  }
}

void fun_2() // consumer
{
  int data = 0;
  while(data != 1)
  {
  unique_lock<mutex> locker(mu);
    if(! q.empty())
    {
      data = q.back();
      q.pop_back();
      locker.unlock();  // UNLOCK_1
      cout << "DATA:" << data << endl;
    }
    else
    {
      locker.unlock(); // UNLOCK_2
    }
  }
}

int main()
{
  thread t_1(fun_1);
  thread t_2(fun_2);
  t_1.join();
  t_2.join();
  // ожидаем в основном потоке завершение двух созданных потоков
  return 1;
}

FUN_2  исполняет BUSY WAITING 
- Ожидание ВХОЛОСТУЮ или - ПРОСТОЙ 
- многократный непрерывный заход в цикл while() loop
Происходит МНОГОКРЫТНЫЙ выхов РАЗБЛОКИРОВКИ в fun_2, 
так как разблокировка за пределами цикла

// РЕШЕНИЕ Плохое:

void fun_2() // consumer
{
...
  else
  {
    std::this_thread::sleep_for(chrono::second(1));
    locker.unlock();
  }
}

НЮАНС:  Да мы СОКРАТИМ число заходов в Цикл на единицу времени
  НО: Как как понять СКОЛЬКО нужно спать потоку ?
    1. если выставим слишком мало  - поток будет входить в цикл слишком часто и Ожидать вхолостую
    2. если выставим слишком много - Мы можем таки НЕ получить данные во время

////////////////////////////////////////
// РЕШЕНИЕ ПРАВИЛЬНОЕ:

std::deque<int> q; // доступ к очереди - ГОНКИ потоков! (кто быстрее запихнёт/удалит)
std::mutex mu;
std::condition_variable cond;

void fun_1() // producer
{
  int count =10;
  while(count > 0)
  {
    unique_lock<mutex> locker(mu);
	q.push_front(count);
	locker.unlock();
	cond.notify_one(); // Оповестить один ждущий поток, 
	     //если есть таковой, с данной Условной переменой cond
	     //cond.notify_all(); // Разбудит ВСЕ потоки, 
	                          //что ждут на данной условной переменной.
	sleep_for(chrono::seconds(1));
	count--;
  }
}

void fun_2() // consumer
{
  int data = 0;
  while(data != 1)
  {
    unique_lock<mutex> locker(mu);
	cond.wait(locker); // Дойдёт до сюда 
	// и будет ждать, пока у данной переменной 
	// в другом потоке не будет выствлен 
	// ОПОВЕЩАЮЩИЙ ФЛАГ через метод notify_one
	// и ждать она будет того, чтобы залочить соответсвенно мьютекс
	
	  data = q.back();
	  q.pop_back();
	  locker.unlock();
	  cout << "DATA:" << data << endl;
  }
}

int main()
{
  thread t_1(fun_1);
  thread t_2(fun_2);
  t_1.join();
  t_2.join();
  // ожидаем в основном потоке завершение двух созданных потоков
  return 1;
}

ВЫВОД: С помощью Условных переменных мы можем Управлять 
последовательностью остановки и возобновления потоков,
и реализовывать ОТЛОЖЕННЫЙ Запуск и Возобновление потока, определяемый нами.

НЮАНС: Почему cond.wait(locker);  wait-функции нужен locker в качесве параметра
  на предыдущем шаге мьютекс захвачивается потоком_2
  и поток не должен ожидать "засыпать" при удержании мьютекса, ибо
  зачем задерживать мьютекс, когда ты собираешся заснуть (ведь если заснёшь .... 
  то когда и как освободишь его ... не понятно)
  На время Засыпания потока нам не хочется блокать всех остальных, кто может поспользоваться мьютексом
ИТАК перед wait-функцией мы посылаем поток_2 в СПЯЧКУ и это сделает НЕЯВНЫЙ UNLOCK locker-a, 
  который передаётся в wait-функцию и только после этого 
  поток_2 отправится в СПЯЧКУ.
ААА КАК ТОЛЬКО поток_2 будет РАЗБУЖЕН через вызов функции notify_one(), 
  это ЗАЛОЧИТ locker обратно, и только после этого 
  в рамках потока_1 мы сможем МНОПОЛЬНО владеть ресурсом  q  ,
  и после чего дойдя до locker.unlock(); мы и разлочим залоченый locker.
Так как в данной ситуации надо много много раз вызывать lock() / unlock(),
 то тут нам ЯВНО нужен именно unique_lock,
 в комбинации с condition_variable

////////////////////////////////////////////////////////////
// Condition Variable
// ФОРСМАЖОРНОЕ_ПРОСЫПАНИЕ (САМОПРОСЫПАНИЕ) из режима wait-функции - SPURIOUS WAKE.

Суть: Запустили wait(), поток заснул, есть 2 варианта ПРОСНУТЬСЯ:
(1) condition_variable вызовет  notify_one()
(2) поток может проснуться САМОСОТОЯТЕЛЬНО (spurious wake)

(2) В случае  spurious wake  может возникнуть ситуация, 
КОГДА поток проснулся не по нашему желанию / незапланированно / ФОРСМАЖОРНОЕ_ПРОСЫПАНИЕ ,
в этом случае, мы хотим избежать неопределённого поведения или ошибки, 
или проблем с логикой, которую вызвало ПРОСЫПАНИЕ потока

ЕСЛИ мы не хотим, чтобы поток проснулся и продолжил своё выполнение, 
 мы можем отправить его ОБРАТНО В СОН (СПАТЬ - ибо нефиг было просыпаться)

 Чтобы избежать неконтролируемого ПРОСЫПАНИЯ ПОТОКА "Не при тех условиях и не в то время", 
МОЖНО В качестве дополнительной Функции-предиката, функция, проверяющая УВСЛОВИЕ, 
передавать таковую в качестве ВТОРОГО ПАРАМЕТРА в ф-цию wait(mutex, PREDICATE_FUNC())
		
cond.wait(locker, [](){ return !q.empty();} );
		
Лямбда-функция [](){ return !q.empty();} - будет возвращать успех, если  q  НЕпусто
Следовательно: Если поток_2 проснулся, то ТЕПЕРЬ о посмотрит на результат выполнения функции-предиката.
 predicate_fun() == FALSE - и поток уйдёт СПАТЬ дальше
 predicate_fun() == TRUE  - и поток ПРОСНЁТСЯ и продолжит исполнение кода
		
////////////////////////////////////////////////////////////
// Condition Variable
// МНОГО потоков ожидающих на ОДНОЙ УСЛОВНОЙ Переменной

  cond.notify_one(); - Разбудит ТОЛЬКО ОДИН единственный поток.
  
  cond.notify_all(); - Разбудит ВСЕ потоки, что ждут на данной условной переменной.
		

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Furure  +  Promise  +  async()

#include <future>

using namespace std;

void factoreal(int N)
{
  int res = 1;
  for (int i = N; i > 1; i--)
  {
    res *= i;
  }

  cout << "Result is: " << res << endl
}

int main()
{
  std::thread t1(factoreal, 4);

  t1.join();
  return 0;
}

```

# QT Все ответы на все вопросы:

http://clinuxcode.blogspot.com/2016/09/frequently-asked-questions-in-qt.html

Q) What is Qt?
Qt is a Framework that comprises of several classes that form the Qt object model. The root of this model is the QObject class. Basically all Qt objects inherit from the QObject class. Having inherited from this class, it means all Qt Objects inherit a particular behavior and we can exploit this in order to manage memory.

Q) How qt differs from other mobile development platforms like android?

1. Qt is platform independent- same source code can be used for development for multiple environment with no or very less code change. Android can be used in android environment only.

2. Language independent- For development in Qt many languages like C++,C#,Java can be used. But in android Java is used.

Q) What are the striking features of QT?

Qt is cross platform application development framework using which application can be developed for different platform with no very less change in codebase.
Few features are:
Signal slots, moc, Qt D-bus,QtCore,QtWidget,QtGui,QML,QtMultimedia,Qt Network, Qt SQL, Qt xml.

Q) What is Qpointer?

QPointer:

It will be automatically set to nullptr if the pointed to object is destroyed. It is a weak pointer specialized for QObject.QPointer can only point to QObject instances.
You would normally use QPointer in a situation where the pointer is being created outside of a QObject class such as in the main function or other none Qt based classes you may have created in your code.
The QPointer is a template class that wraps obj as a pointer to MyObject. You will notice that there is no star to mark it as a pointer because QPointer abstracts all that. It does not just stop there but it also deals with dynamically destroying the obj pointer should there no longer be any references to it.

There you have it. A simple and safer way to use pointers and to manage your memory when programming in Qt C++.

Consider this fragment:
```
QObject *obj = new QObject;
QPointer<QObject> pObj(obj);
delete obj;
Q_ASSERT(pObj.isNull()); // pObj will be nullptr now
```

QSharedPointer

A reference-counted pointer. The actual object will only be deleted, when all shared pointers are destroyed. Equivalent to std::shared_ptr.
```
int *pI = new int;
QSharedPointer<int> pI1(pI);
QSharedPointer<int> pI2 = pI1;
pI1.clear();
// pI2 is still pointing to pI, so it is not deleted
pI2.clear();
// No shared pointers anymore, pI is deleted
```
Note that as long there is a shared pointer, the object is not deleted!

QWeakPointer:

Can hold a weak reference to a shared pointer. It will not prevent the object from being destroyed, and is simply reset. Equivalent to std::weak_ptr, where lock is equivalent to toStrongRef.
```
int *pI = new int;
QSharedPointer<int> pI1(pI);
QWeakPointer<int> pI2 = pI1;
pI1.clear();
// No shared pointers anymore, pI is deleted
//
// To use the shared pointer, we must "lock" it for use:
QSharedPointer<int> pI2_locked = pI2.toStrongRef();
Q_ASSERT(pI2_locked.isNull());
```
This can be used if you need access to an object that is controlled by another module.

To use a weak pointer, you must convert it to a QSharedPointer. You should never base a decision on the weak pointer being valid. You can only use data() of isNull to determine that the pointer is null.

Q) What is dpointer?
```
The d-pointer
The trick is to keep the size of all public classes of a library constant by 
only storing a single pointer. This pointer points to a private/internal data structure 
that contains all the data. The size of this internal structure can shrink or grow without 
having any side-effect on the application because the pointer is accessed only in the 
library code and from the application's point of view the size of the object never changes 
- it's always the size of the pointer. This pointer is called the d-pointer.
```
Q) What is MVC architecture? How it is organized?
```
MVC consists of three kinds of objects.
Model is the application object,
View is its screen presentation, and the
Controller defines the way the user interface reacts to user input.
Before MVC, user interface designs tended to lump these objects together.
MVC decouples them to increase flexibility and reuse.
Model/view architecture to manage the relationship between data 
and the way it is presented to the user.
```
Q) What is a signal? And how will it differ from event?
Signal is emitted when a particular event occurs. Qt's widgets have many predefined signals, but we can always subclass widgets to add our own signals to them.
```
signals: /* signals must be declared under 'signals:' or “ Q_SIGNALS:” access specifier */
void foo(); /* This is a signal declaration -- a pure declaration as a method.*/
Before Qt5 signals were protected so that only same class objects acan emit it.
From Qt5 onwards signals are public and to make it private need to declare QPrivateSignal dummy (empty) struct private in the Q_OBJECT macro.

In Qt, events are objects, derived from the abstract QEvent class, 
that represent things that have happened either within an application or as a 
result of outside activity that the application needs to know about. Events can be 
received and handled by any instance of a QObject subclass, but they are especially 
relevant to widgets. This document describes how events are delivered and handled 
in a typical application. in general an event will be generated by an outside 
entity (e.g. Keyboard, Mouswheel) and will be delivered through the event loop in QApplication.
```
Signals and Slots are a convenient way for QObjects to communicate with one another and are more similar to callback functions. In most circumstances, when a "signal" is emitted, any slot function connected to it is called directly. The exception is when signals and slots cross thread boundaries. In this case, the signal will essentially be converted into an event.

Q) What is a slot? And how it differs with callback Method?

Slots are normal C++ member functions and can be called normally; their only special feature is that signals can be connected to them. A slot is called when a signal connected to it is emitted. We can also define slots to be virtual.

Callbacks have two fundamental flaws: Firstly, they are not type-safe. We can never be certain that the processing function will call the callback with the correct arguments. Secondly, the callback is strongly coupled to the processing function since the processing function must know which callback to call.

Q) How many signals at maximum you can connect to a single slot? If more than one how you can do that?
Surely we can connect more than one signal to a single slot. The single slot is called whenever one of those connected signal is emmitted.
Even one signal can be coonected to another signal, which are then emitted automatically whenever the source signal is emitted. Definition for signals are genereated in by moc in code generated by moc.

Q) How many slots can you connect to a signal? If more than one how those slots will be executed?
(The order of execution).
Surely more than on slot can be connected to a single signal. Then Slots are called in the order they are connected when the signal is emmitted.

Q) What is QCast and how will it differ compared to c++’s dynamic cast?
qobject_cast function is used to convert polymorphic pointers on qobject classes.
qobject_cast does not reauire the support for rri while dynamic_cast requires to enable rtti.
Similar to dyanimc_cast it returns a non zero pointer on success ad 0 on failure.
```
For example,
let's assume MyWidget inherits from QWidget and is declared with the Q_OBJECT macro:
class MyWidget: public Qwidget
{
QObject *obj = new MyWidget;
...
};
The cast from QObject to QWidget must be successful, because the object obj is actually a MyWidget, which is a subclass of QWidget.

QWidget *widget = qobject_cast<QWidget *>(obj);
```
The cast from QObject to QWidget is successful, because the object is actually a MyWidget, which is a subclass of QWidget. Since we know that obj is a MyWidget, we can also cast it to MyWidget.

Q) What is the use of Q_OBJECT macro?
OBJECT simply tells the pre-compiler that this class has gui elements and needs to be run through the 'moc' . In turn moc generates definitions for member variables and functions that Q_OBJECT has declared. moc produces a C++ source file containing the meta-object code for those classes which declare Q_OBJECT and derived from QOBJECT or wants to use signal slot or wants to use qobject_cast.
The Q_OBJECT macro must appear in every class that derives from QObject or which wants to use signal slot mechanism, that want to use qobject_cast.

Q) What is MOC and UIC? Explain how they will work for compilation in QT?
The Meta-Object Compiler, moc, is the program that handles Qt's C++ extensions.
The moc tool reads a C++ header file. If it finds one or more class declarations that contain the Q_OBJECT macro, it produces a C++ source file containing the meta-object code for those classes. Among other things, meta-object code is required for the signals and slots mechanism, the run-time type information qobject_cast, and the dynamic property system.

Q) What is qmake? (Explain the usage of it)

Q) How a QT Program/Application written for one platform (Symbian) can be ported/executed in another
platform (Maemo)? (Explain If you need to make any changes or you need to recompile)

Q) What are all the platforms/OS currently QT supports?

Q) What is event loop in Qt?

18) How to design your own widget in Qt?

1. Signal/slot mechanism is synchronous or not? If it is,that means we should not let a slot do much work,otherwise the UI will be blocked for much time.?
Usually, slots are synchronous. Starting with Qt4, you can send signals across threads. If you execute a slot in another thread, you will get an asynchronous slot.
2. QEvent is asynchronous or not? For example ,if I call the update() function,the paintEvent() will be called, if it is 
asychronous,the UI will not be blocked.?
QEvents are syncronous but you can also deliver events in different slots. With Qt3, this was the only option to invoke something in a different thread.
3. Is there any relationship between signal/slot and event? Does Qt put the slots into event queue?
With Qt4, you can tell Qt to post signals in the event queue using the 5th argument of the connect-method.

1) Memory leak?
Memory leaks occur when pointers referencing a certain chunk of memory goes out of scope and therefore the pointer is lost but the content that the pointer was referencing is still lying in the heap. A pointer would normally go out of scope when an execution block ends and returns and the pointer created within that scope is not destroyed along with the referenced data. This can also occur when an exception is thrown or when any sort of error stops execution before the pointer is destroyed.
Qpointer is used in Qt framework to avoid such scenarios.
```
QPointer – This tracks the lifetime of an instance of QObject.
QSharedPointer, QWeakPointer – These are thread-safe sharing pointers, similar to C++11’s std::shared_ptr. QWeakPointer use has been deprecated unless used with QSharedPointer.
QScopedPointer, QScopedPointerArray – These are none shared pointers Ideal for Resource Acquisition Is Initialization (RAII) usage whereby they take ownership of a pointer and ensures it is properly deleted at the end of the scope.
```
Q ) What is D-Bus?
D-bus is inter process communication mechanism using which applications can communicate to each other.
Using dbus an application exposes its services with adapters.
Another application can use the services of other application using proxy by making it d-bus enabled.

Q) What is a System Bus?
Generally system bus starts with system's startup level.
Mainly used with hardwares events(like keyboard press or mouse click).
Event/interrupts/signals are signals from hw/sw to os/processor that it requirs quick attention from os/processor.

Q) What is a Session Bus?
Generally session bus starts when a user logins to desktop.
Mainly used for desktop applications to connect to each other.

Q) What is the syntax of a dbus?
Application one that exposes its services (say test()) creates xml using cpp2xml tool then creates adapter and proxy using xmltocpp tool and finally include adapter, registerService and registerObject.
new MainWindowAdaptor(&w);
```
QDBusConnection connection1 = QDBusConnection::sessionBus();
connection1.registerService("com.testServiceName");
connection1.registerObject("/testObjectPath",&w);
Another applicationTwo which wants to call services/methods exposed by the applicationOne include proxy, create proxy object and call the remote function on proxy object and in turn actual function in remote applicationOne gets called.
MyInterfaceClass * m_window = new MyInterfaceClass("com.testServiceName","/testObjectPath",QDBusConnection::sessionBus(),0);
```
Then call the function “test()” on remote application using local proxy objext as
m_window->test("via setParent");

Note:instead of 0 specific methods can be given.

Q) How to debug if a connect fails?
```
Check that classes using signals and slots inherit QObject or a QObject subclass.
Make sure the Q_OBJECT macro is inserted at the beginning of your class declaration.
If you use custom signals, check that these are declared correctly, with a void return type, in the public/protected/private signals section of your class declaration.
Make sure that your slot function is declared as a slot, e.g. private slots not private.
Put all connect statements before functions calls
Check the return value of the connect statement: connect returns true if it successfully connects the signal to the slot.
Use QErrorMessage::qtHandler() or qInstallMsgHandler() to view connect error warnings.
Put qDebug to check signal slots are called.
Check the compiler warnings.
```
Q) Can we give definition to user defined signal?
No, it will be a redefinition linker error because definition of signal is given in moc code.

Q) Can we connect a signal with another signal?
Yes, as both ara functions.
Example suppose a library code writeCode() emits a signal writeCompleted.
If another code say writeSocket uses the writeCode. On completion it emits another signal say writeSocketCompleted. So we can connect the signal writeSocketCompleted with writeCompleted.(without changing the library code)

Q) can slot have lesser arguments than signal? Yes
The signature of a signal must match the signature of the receiving slot. (In fact a slot may have a shorter signature than the signal it receives because it can ignore extra arguments.)
```
This means that a signal of the form
signal(int, int, QString)
can only be connected with slots with the following signatures
slot1(int, int, QString)
slot2(int, int)
slot3(int)
slot4()
```
Q) can we overload function on basis of extern?
No, redefinition.

Q) Why multithreading is used?

1. efficient program to make best use of cpu cycles.
2. handling event driven code in ntworked, gui based, distributed code.
3. threads does not required ipc(like pipe,fifo,socket used in processes to communicate).
4. since threads run on the same address space of process so fast to create.
5. concurrent programming.

Q) How to use thread in Qt. Two ways?

1) creating a sub-class of QThread and implementing the run method then create object of this class and call start on that. Its old style like in java.
```
class SampleThread : public QThread
{
Q_OBJECT
protected:
virtual void run()
{
// do some work
qDebug() << "Hello from SampleThread: Thread Id:" << QThread::currentThreadId();
// Enters the main event loop and waits until exit() is called.
exec();
}
};
SampleThread aThread;
// start thread
aThread.start();
// check if thread running, returns true
qDebug()<< "thread running " << aThread.isRunning();
// check if thread finished, returns false
qDebug()<< "thread finished" << aThread.isFinished();
// terminate thread and wait for termination
aThread.quit();
aThread.wait();
// check if thread running, returns false
qDebug()<< "thread running " << aThread.isRunning();
// check if thread finished, returns true
qDebug()<< "thread finished" << aThread.isFinished();

Qii) can thread be connected to slots? Yes.
#include <QtCore>
class Thread : public QThread
{
private:
void run()
{
qDebug()<<"From worker thread: "<<currentThreadId();
}
};

int main(int argc, char *argv[])
{
QCoreApplication a(argc, argv);
qDebug()<<"From main thread: "<<QThread::currentThreadId();

Thread t;
QObject::connect(&t, SIGNAL(finished()), &a, SLOT(quit()));

t.start(); //start() calls the run() which is the entry point for a thread.
return a.exec();
}
/*output:
From main thread: 0x15a8
From worker thread: 0x128c
*/
```

2)Second way or new way of creating thread. more elegant way.
Subclass QThread and reimplement its run() function is intuitive and there are still many perfectly valid reasons to subclass QThread, but when event loop is used in worker thread, it's not easy to do it in the subclassing way.
Use worker objects by moving them to the thread is easy to use when event loop exists, as it has hidden the details of event loop and queued connection.

Thread affinity was introduced at the base level of QObject, which is the base object for nearly all Qt related classes.
Signal and slot connections can now be used between objects of different thread affinity.

3. QThread gained a default run implementation.
```
Example:
Usage 2-0

If we only want to make use of QThread::exec(), which has been called by QThread::run() by default, there will be no need to subclass the QThread any more.
Steps:
Create a Worker object
Do signal and slot connections
Move the Worker object to a sub-thread
Start thread

#include <QtCore>

class Worker : public QObject
{
Q_OBJECT
private slots:
void onTimeout()
{
qDebug()<<"Worker::onTimeout get called from?: "<<QThread::currentThreadId();
}
};

#include "main.moc"
int main(int argc, char *argv[])
{
QCoreApplication a(argc, argv);
qDebug()<<"From main thread: "<<QThread::currentThreadId();

QThread t;
QTimer timer;
Worker worker;

QObject::connect(&timer, SIGNAL(timeout()), &worker, SLOT(onTimeout()));
timer.start(1000); //start() calls Qthread::run() which is the entry point for thread.

//timer.moveToThread(&t);
worker.moveToThread(&t);

t.start();

return a.exec();
}
/*
The result is:As expected, the slot doesn't run in the main thread.
From main thread: 0x1310
Worker::onTimeout get called from?: 0x121c
Worker::onTimeout get called from?: 0x121c
Worker::onTimeout get called from?: 0x121c
*/
Q) Command to count characters without opening it?
/prac$ wc -m < test.txt
29
test.txt contains: 29 characters including spaces and \0
```

Casting:
1) reinterpret_cast: used for i) casting between pointers of unrelated classes. ii) for ptr to int type cast.
2) const_cast: used to remove constantness.
3) dynamic_cast: is used to perform safe downcasting. Base ptr to derived class pointer.
4) static_cast: for implicit type conversions
i) non-const object to const,
ii) int to double.
iii) void* pointers to typed pointers,
iv)base pointers to derived pointers.
But it cannot cast from const to non-const object. This can only be done by const_cast operator.


```
#include <iostream>
using namespace std;

struct data {
short a;
short b;
};

int main () {
long value = 0xA2345678;
data* pdata = reinterpret_cast<data*> (&value;);
cout << pdata->a << endl;
return 0;
}
Output on my machine: 22136 which is 2 bytes of value.

Another example might be:

class A {};
class B {};

int main()
{
A * pA = new A;
B * pB = reinterpret_cast<B*>(pA);
}
```

# QML все ответы на все вопросы

http://clinuxcode.blogspot.com/2016/10/what-are-qml-faq-in-interviews.html

Q) What is Qml ?

QML is the name of the language (just like C++, which is another language...)

QML stands for Qt Meta Language or Qt Modelling Language is a user interface markup language.

Q) How to call c++ call from QML?
QML call C + + methods with the Qt meta-object system. Like below:
onClicked: parent.color = parent.randomColor ()

Q) What are Four ways of integreting C++ with Qml?
Subclassing QQuickItem: QQuickItem allows you to write your own visual and non-visual QML items using C++.
Registering C++ types with QML: C++ classes can be registered with the QML type system, allowing them to be instantiated as QML types.

Registering Context Properties: QObjects can be registered with the QML context, allowing their properties to be directly accessed.

Accessing QML objects through the QML object tree: All QML objects reside in a tree hierarchy and can be accessed via the root of the tree.
http://www.ics.com/blog/multilayered-architecture-qt-quick

Q) How to call a C++ function from a qml?
Through setcontext property.
Note
Loading a main.qml with a simple Item as the root type through the QmlApplicationEngine will not show anything on your display, as it requires a window to manage a surface for rendering. The engine is capable of loading qml code which does not contain any user interface (e.g plain objects). Because of this it does not create a window for you by default.
The qmlscene or the new qml runtime will internally first check if the main qml file contains a window as a root item and if not create one for you and set the root item as a child to the newly created window.

Q)What is Q_INVOKABLE?

Add callable methods using Q_INVOKABLE or Qt slots, and connect to Qt signals with an onSignal syntax

Q)How to call a QML function from c++?

QML functions can be called from C++ and vice-versa.
All QML functions are exposed to the meta-object system and can be called usingQMetaObject::invokeMethod(). Here is a C++ application that uses this to call a QML function:
```
// MyItem.qml
import QtQuick 1.0
Item {
    function myQmlFunction(msg) {
        console.log("Got message:", msg)
        return "some return value"
    }
}
// main.cpp
QDeclarativeEngine engine;
QDeclarativeComponent component(&engine, "MyItem.qml");
QObject *object = component.create();
QVariant returnedValue;
QVariant msg = "Hello from C++";
QMetaObject::invokeMethod(object, "myQmlFunction",
        Q_RETURN_ARG(QVariant, returnedValue),
        Q_ARG(QVariant, msg));
qDebug() << "QML function returned:" << returnedValue.toString();
delete object;
```
Q)How to call a C++ function from QML ?

A C++ function can be called from a qml using set contextpropery in c++.
http://doc.qt.io/qt-4.8/qtbinding.html
All QML signals are automatically available to C++, and can be connected to using QObject::connect() like any ordinary Qt C++ signal.

The signal is sent to QML, and the slot is invoked from QML.
There are different ways to send signals from C++ to QML and back. In this article, we show how to do this by embedding a C++ class directly into QML. This has the advantage that no Qt::connect connections need to be set-up manually.

In our example, we have a Receiver class that is implemented in C++.

This class defines a signal sendToQml and a slot receiveFromQml.
 Both have an integer parameter.
The signal is sent to QML, and the slot is invoked from QML.
```
Example:
//signal_slot.pro
TEMPLATE = app
QT += qml quick
#export QT_SELECT=5
SOURCES += main.cpp \
    receiver.cpp
RESOURCES += qml.qrc
# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =
# Default rules for deployment.
include(deployment.pri)
HEADERS += \
    receiver.h
//main.cpp
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "receiver.h"
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    Receiver receiver;
    QQmlContext* ctx = engine.rootContext();
    ctx->setContextProperty("receiver", &receiver);
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));
    receiver.sendToQml(43);
    //QDeclarativeView view;
    //view.rootContext()->setContextProperty("_EBSCModel",m_EBSCDtcDataModel);
    //sourcePath = "qrc:///DTC/qml/ITSDTC.qml";
    //view.setSource(QUrl(sourcePath));
    return app.exec();
}
//receiver.h
#ifndef RECEIVER_H
#define RECEIVER_H
#include <QObject>
class Receiver : public QObject
{
    Q_OBJECT
public:
    explicit Receiver(QObject *parent = 0);
signals:
    void sendToQml(int count);
public slots:
    void receiveFromQml(int count);
};
#endif // RECEIVER_H
//receiver.cpp
#include "receiver.h"
#include <QDebug>
Receiver::Receiver(QObject *parent) :
   QObject(parent)
{
}

void Receiver::receiveFromQml(int count) {
    qDebug() << "Received in C++ from QML:" << count;
}
//main.qml
import QtQuick 2.2
import QtQuick.Window 2.1
Window {
    id: test
    visible: true
    width: 200
    height: 50
    Connections {
        target: receiver
        onSendToQml: {
            console.log("Received in QML from C++: " + count)
        }
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            receiver.receiveFromQml(42);
        }
    }

    Text {
        text: qsTr("Press me to send a signal to C++")
        anchors.centerIn: parent
    }
}
```

# QT questions

http://www.qtcentre.org/threads/38448-QT-related-interview-questions

- How qt differs from other mobile development platforms like android?
- If worked with GTK what is the d+ifference between QT and GTK?
- What are the striking futures of QT?
- What is Qpointer?
- What is dpointer?
- What is MVC architecture? How it is organized?
- What is a signal? And how will it differ from event?
- What is a slot? And how it differs with callback Method?
- What is the difference between signal and slot mechanism and Callback mechanism of GTK?
- How many signals at maximum you can connect to a single slot? If more than one how you can do that?
- How many slots can you connect to a signal? If more than one how those slots will be executed? 
(The order of execution)
- What is QCast and how will it differ compared to c++’s dynamic cast?
- What is the use of Q_OBJECT macro?
- What is MOC and UIC? Explain how they will work for compilation in QT?
- What is qmake? (Explain the usage of it)
- How a QT Program/Application written for one platform (Symbian) can be ported/executed in another 
platform (Maemo)? (Explain If you need to make any changes or you need to recompile)
- What are all the platforms/OS currently QT supports?

### Q. What is a signal? And how will it differ from event?
Что такое СИГНАЛЫ: События, которые могут обрабатываться НЕ лишь одним методом, а многими методами, тоесть слотами

Event - происходит один раз и завязан на одну обработку

### Q. What is a slot? And how it differs with callback Method?
Что такое СЛОТЫ: методы, которые мы "соединяем" с сигналами или (событиями), чтобы эти методы были выполнены в момент настцпления (emit) вызова / срабатывания события/сигнала

callback методы - просто указатели на функцию, по которым происходит её вызов.

### Q. What is the difference between signal and slot mechanism and Callback mechanism of GTK?


# UUID F2FA1C23-2E60-4135-91D3-193007215549

### Q: Шпаргалка по QT !
http://cppstudio.com/post/11167/

### 20 Qt Questions
https://www.wisdomjobs.com/e-university/qt-qml-interview-questions.html01

### Что под капотом у Signal, Slot, Q_OBJECT 

# https://habr.com/ru/post/214379/

```
#define signals public
#define slots /* nothing */

#define Q_OBJECT \
```

### QMAKE
утилита (Qt-шный аналог CMAKE) для генерации Make-файлов, которые нужны для описания процесса построения сполняемых программ из исходных кодов

### MOC
утилита, переводящая код, написанный на Qt, в код на чистом C++

### QML
Это Qt Meta Language или Qt Modelling Language язык для описания интерфейсов, с использованием toolkit-а QtQuick is a toolkit for QML, который и позвляет писать графические интерфейсы дя Qt на QML

### Q_GADGET 
https://habr.com/post/307816/

Макрос Q_GADGET позволяет
- (+) не требует наследования от QObject
- (+) боле легковестный
- (-) он БЕЗ поддержки механизма сигналов и слотов
- (-) можно использовать QMetaObject c некоторыми ограничениями, а именно:

### Q_ENUM

### Q_PROPERTY

### Q_INVOKABLE - Как плохая, но всё же альтернатива сигналам и слотам

### What Are Limitations Of Moc?

- The main problem is that class <b>TEMPLATES can NOT have signals or slots</b>.
- Function Pointers Cannot Be Signal or Slot Parameters
- Type Macros Cannot Be Used for Signal and Slot Parameters
- Nested Classes Cannot Have Signals or Slots
- Signal/Slot return types cannot be references

### Qvariant?

- QVariant is a container of variables. It can store variables of different types. Similar in some way to void*. But it provides You information about the stored type.
- it can be used for example to return different types of values from a function.

### Q: Как работают события в c++ ?

https://habr.com/ru/post/424593/

### Q: Что такое QEvent QT ?

// TO DO

### Q: Как отловить / получить Event из какого-то объекта или внутри определённого объекта в QT ?

https://www.opennet.ru/docs/RUS/qt3_prog/x3974.html

В Qt предусмотрены **ПЯТЬ** уровней, на которых событие может быть перехвачено и обработано:

1. **Обработка событий в функциях-обработчиках**

Перекрытие обработчиков событий, таких как: mousePressEvent(), keyPressEvent() и paintEvent(), безусловно самый распространенный способ. Мы уже видели множество примеров тому.

2. **Перекрытие метода QObject::event()**

Внутри этого обработчика мы можем перехватывать события до того, как они попадут в специализированные функции-обработчики. Этот подход чаще всего используется для того, чтобы изменить реакцию виджета на клавишу табуляции, как это было показано ранее. Он так же используется для обработки событий, которые встречаются не так часто, например: LayoutDirectionChange. Если мы перекрываем функцию event(), то необходимо предусмотреть вызов обработчика event() базового класса, чтобы обработать события, которые нас не интересуют.

3. **Установка фильтра событий для QObject**

После того, как фильтр будет зарегистрирован функцией installEventFilter(), все события, предназначающиеся указанному объекту, сначала будут попадать в обработчик eventFilter(). Такой способ мы использовали для перехвата событий от клавиши "пробел" в примере выше.

4. **Установка фильтра событий объекта QApplication**

После регистрации фильтра, любое событие, предназначенное для любого объекта в приложении, будет сначала попадать в обработчик eventFilter(). Такой подход чаще всего используется в целях отладки и реализации в приложении скрытых сюрпризов (так называемых "пасхальных яиц").

5. **Создание дочернего класса от QApplication и перекрытие метода notify()**

Qt вызывает QApplication::notify(), чтобы передать событие приложению. Таким способом можно перехватить любое событие до того, как оно попадет в фильтр событий. Вообще фильтры событий более удобны, поскольку допускается одновременное существование любого количества фильтров, а функция notify() может быть только одна.

- **Event Handlers**

The normal way for an event to be delivered is by calling a virtual function.
For example, QPaintEvent is delivered by calling QWidget::paintEvent().
For example, the following code handles left mouse button clicks on a custom checkbox widget while passing all other button clicks to the base QCheckBox class:
```
keyPressEvent(QKeyEvent *event) 
mouse release
mouseMoveEvent
void MyCheckBox::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // handle left mouse button here
    } else {
        // pass on other buttons to base class
        QCheckBox::mousePressEvent(event);
    }
}

bool MyWidget::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Tab) {
            // special tab handling here
            return true;
        }
    } else if (event->type() == MyCustomEventType) {
        MyCustomEvent *myEvent = static_cast<MyCustomEvent *>(event);
        // custom event handling here
        return true;
    }

    return QWidget::event(event);
}
```

- **Event Filters**

Sometimes an object needs to look at, and possibly intercept, the events that are delivered to another object. For example, dialogs commonly want to filter key presses for some widgets; for example, to modify Return-key handling.
```
bool FilterObject::eventFilter(QObject *object, QEvent *event)
{
    if (object == target && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Tab) {
            // Special tab handling
            return true;
        } else
            return false;
    }
    return false;
}
```

- **Sending Events**

sendEvent() processes the event immediately. When it returns, the event filters and/or the object itself have already processed the event. For many event classes there is a function called isAccepted() that tells you whether the event was accepted or rejected by the last handler that was called.

postEvent() posts the event on a queue for later dispatch.


### Q: Разница между sendEvent() и postEvent() ?

- sendEvent() - обрабатывает событие сразу, в обход очереди сообщений И она возвращает значение и фильтр или объект к этому времени уже обработал его. У некоторых классов есть isAccepted() был ли евент принят или отклонён обработчиком событий.
- postEvent() - ставит событие в очередь сообщений для последующей обработки.

### Q: Как работают события в QT ?

// TO DO

### Q: Что такое сигнал ?

Сигналы являются публично доступными функциями и могут быть вызваны где угодно, но рекомендуется их вызывать только в классе, где они были определены, а также в его подклассах
Сигналы автоматически генерируются в moc и не должны быть определены в .cpp файле, а также они никогда не возвращают результат.

### Q: Что произойдёт если вызовется EMIT для 2-ух объектов в дном потоке и в разных потоках ?
1. случай - 2 объекта находятся в одном потоке и происходит у них сигнал 
2. случай - 2 объекта находятся в разных потоках и происходит у них сигнал 
Будет ли какая-то разница ?
```
Соединение происходит. Но слот никогда не сработает. Объясню почему. 

QObject::connect(server,SIGNAL(runIt(int)),func,SLOT(run(int))); 
этот метод, кроме этих четырёх параметров имеет и пятой параметр по умолчанию.
Он задаёт тип соединения.
```
Qt поддерживает 4 типа signal-slot connection-ов типов соединений:
1) Auto Connection (default) - If the signal is emitted in the thread which the receiving object has affinity then the behavior is the same as the Direct Connection. Otherwise, the behavior is the same as the Queued Connection."
2) Direct Connection - The slot is invoked immediately, when the signal is emitted. The slot is executed in the emitter's thread, which is not necessarily the receiver's thread.
3) Queued Connection - The slot is invoked when control returns to the event loop of the receiver's thread. The slot is executed in the receiver's thread. Blocking Queued Connection The slot is invoked as for the Queued Connection, except the current thread blocks until the slot returns. Note: Using this type to connect objects in the same thread will cause deadlock.
4) Unique Connection - The behavior is the same as the Auto Connection, but the connection is made only if it does not duplicate an existing connection. i.e., if the same signal is already connected to the same slot for the same pair of objects, then the connection is not made and connect() returns false.

```
По умолчанию сотит AutoConnection. Что это означает?
Если у вас два соединённых объекта находятся 
В ОДНОМ и том же потоке, то слот будет вызываться сразу же,
когда будет имитирован сигнал.

Но когда Ваши объекты находятся 
В РАЗНЫХ потоках, имитированные сигналы не будут вызывать слот сразу,
они будут помещены в цикл событий потока, и будут исполнены,
когда придёт их очередь, т.е. сигнал не сможет быть доставлен
(слот не сможет быть вызван), пока не закончится выполнение той функции,
в которой был имитирован сигнал.

У Вас бесконечный цикл, поэтому и все Ваши сигналы на очереди исполнения.
Остановите Вашу функции)) и удивитесь, сколько же раз будет вызван слот ;)

В Вашем случае, решение проблемы, это указать в connect пятым аргументом DirectConnection.
Но это не рекомендуется делать по документации qt, потому что это не безопасно,
если объекты находятся в разных потоках. Всё, что я сказал, есть в док.
```

1) Создаю объект со слотом run() в основном потоке (прямо в main()):
```
Function* func = new Function;
```
2) Запускаю асинхронно serverThreadGlobal:
```
QFuture<int> future = QtConcurrent::run(serverThreadGlobal,func);
```
3) Внутри функции создаю объект класса с сигналом runIt и соединяю сигнал и слот:
```
int serverThreadGlobal(Function* func)
{
    std::cout<<"serverThreadGlobal\n";
    Server* server = new Server;
    QObject::connect(server,SIGNAL(runIt(int)),func,SLOT(run(int)));
//тут испускается сигнлал emit runIt(0);
    server->serverCycle(func);
    return 1;
}
```
4) Не соединяет! (слот void Function::run(int n) не срабатывает)
```
Код функции serverCycle

void Server::serverCycle(Function* func)
{
    emit runIt(0);
    while(1)
    {
        Sleep(1000);
        emit runIt(0);
        std::cout<<"working\n";
    }
}
```

### C++ Call From Qml?

- QML call C + + methods with the Qt meta-object system. Like below:
- onClicked: parent.color = parent.randomColor ().

### 4 Ways : C++ With Qml?

- Subclassing QQuickItem: QQuickItem allows you to write your own visual and non-visual QML items using C++.
- Registering C++ types with QML: C++ classes can be registered with the QML type system, allowing them to be instantiated as QML types.
- Registering Context Properties: QObjects can be registered with the QML context, allowing their properties to be directly accessed.
- Accessing QML objects through the QML object tree: All QML objects reside in a tree hierarchy and can be accessed via the root of the tree.

### Q: Варианты взаимодействия между С++ и QML ?

1. Макрос Q_PROPERTY в бекенде С++ класса (val, get, set, sig valChanged)
2. Макрос Q_INVOKABLE в бекенде С++ класса (Q_INVOKABLE-метод)
3. Общение через Сигналы заведённые на стороне C++
4. Общение через Сигналы заведённые на стороне QML
5. QQmlProperty::read(object, "someNumber").toInt();
6. QMetaObject::invokeMethod(...)
7. QQuickView view(QUrl::fromLocalFile("MyItem.qml")); QObject *item = view.rootObject();

```
```

**1. Макрос Q_PROPERTY в бекенде С++ класса**

- позволяет задать переменную на стороне плюсов и qml. Оба слоя одинаково имеют к ней доступ.
- с++ может сделать set через set метод, qml и cpp получить через get метод 
- этот макрос так же определяет сигнал  somePropertyChanged, чтобы сpp-сторона информировала qml об изменении переменной
- на стороне qml моно добавить соотвествующий обработчик onSomePropertyChanged: { } чтобы отловить событие об изменении
```
class BackEnd : public QObject { Q_OBJECT
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
public: int   getSomeProperty() const;
        void  setSomeProperty(const int &);
private: int  someProperty;
signals: void somePropertyChanged();
...
```

```
#include "backend.h"
int main(int argc, char *argv[])
{
  qmlRegisterType<BackEnd>("io.qt.examples.backend", 1, 0, "BackEnd");
...
```

```
import io.qt.examples.backend 1.0

ApplicationWindow { id: root
  BackEnd {
    id: backend
    someProperty: 10
    omSomePropertyChanged: {
      // наш код
    }
  }
...
```

**2. Макрос Q_INVOKABLE в бекенде С++ класса**
позволит вызывать Q_INVOKABLE-метод как метод qml объекта
```
class TestClass : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE
    void myMethod();
};
void TestClass::myMethod() { qDebug() << "!"; }
```

```
import ModuleName 1.0
TypeName{
 id: myObj
  someProperty: 10
  }

 Rectangle{
  MouseArea {
   onClicked: {
    myObj.myMethod();
 }}}

```


**3. Общение через Сигналы заведённые на стороне C++**
```
class TestClass : public QObject
{
    Q_OBJECT
signals:
    void someSignal();
};
...
void TestClass::mySlot()
{
    emit someSignal();
}
```

```
TypeName
{
  id: myObj
  onSomeSignal: {
    Qt.quit();
  }
} 
```

**4. Общение через Сигналы заведённые на стороне QML**
```
Item { 
    signal qmlSignal(msg: string)
...
```

```
class MyClass : public QObject
{
    Q_OBJECT
public slots:
    void cppSlot(const QString &msg) {
        qDebug() << "Called the C++ slot with message:" << msg;
    }
};
```

```
int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QQuickView view(QUrl::fromLocalFile("MyItem.qml"));
    QObject *item = view.rootObject();

    MyClass myClass;
    QObject::connect(item, SIGNAL(qmlSignal(QString)),
                     &myClass, SLOT(cppSlot(QString)));
...
```

**5. QQmlProperty::read(object, "someNumber").toInt();**
```
// MyItem.qml
import QtQuick 2.0
Item { property int someNumber: 100; }
```
The value of the someNumber property can be set and read using QQmlProperty, or QObject::setProperty() and QObject::property():
```
QQmlEngine engine;
QQmlComponent component(&engine, "MyItem.qml");
QObject *object = component.create();
qDebug() << "Property value:" << QQmlProperty::read(object, "someNumber").toInt();
QQmlProperty::write(object, "someNumber", 5000);
qDebug() << "Property value:" << object->property("someNumber").toInt();
object->setProperty("someNumber", 100);
```

**6. QMetaObject::invokeMethod(object, "myQmlFunction", Q_RETURN_ARG(QString, returnedValue), Q_ARG(QString, msg));**
```
import QtQuick 2.0

Item {
    function myQmlFunction(msg: string) : string {
        console.log("Got message:", msg)
        return "some return value"
    }
}
```

**7. QQuickView view::rootObject() - parent - children - property**

Получив rootObject() можно по id / objectName объектов ступскаться вгулбь по дереву объектов
и организовывать общение при помощи четырёх ранее описанных методов.
```
QQuickView view(QUrl::fromLocalFile("MyItem.qml")); QObject *item = view.rootObject();**
```

```
 signal qmlSignal(anObject: Item)
```

```
 void cppSlot(QQuickItem *item) {
       qDebug() << "Item dimensions:" << item->width() << item->height();
```

### QUIZFUL

Количество параметров слота всегда меньше или равно количеству параметров сигнала

### Можно ли соединять сигналы с сигналами?
Да
 
### Укажите главное различие между сигналами и событиями.
События могут обрабатываться лишь одним методом, а сигналы многими слотами

### Соединение сигнала с виртуальными слотами медленнее чем с невиртуальным?
Да

### Какой виртуальный метод необходимо переопределить при наследовании от QThread?
void run()  - Необходимо переопределить virtual void run() ,т.к. слот start(), вызываемый для запуска потока, вызывает метод run().

### На чем основывается приведение типов qobject_cast?
Метаинформация

### QStringList list; Что быстрее при выполнении: list[i] или list.at(i)
list.at(i) быстрее чем list[i]

"at() can be faster than operator[](), because it never causes a `deep copy` to occur."

### Чтобы приложение можно было интернационализировать, необходимо:
Заключить все строки в статический метод tr() класса QObject

Заключить все обозначения валюты, клавиши ускорителей в статический метод tr() класса QObject. Установить текстовый кодек


# SIGNALS AND SLOTS

http://blog.kislenko.net/show.php?id=1308

https://www.youtube.com/watch?v=sA2PX_79NkQ

### как работае CONNECT:

https://habr.com/post/214379/

### Callback ФУНКЦИИ
- Передаётся Указатель на Calback-функцию внутрь Рабочей функции, и в случае необходимости получения ответа Рабочая функция вызыввет Calback-фцнкциЮ и таким образом даёт обратный ответ из своего процесса.

- функций обратного вызова () - в результате действий пользователя вызываются обычные методы класса типа void. Чтобы сопоставить код с кнопкой, необходимо передать в функцию указатель на кнопку. Элементы графического интерфейса пользователя оказываются тесно связаны с функциональными частями программы.
 
## КАРТА СООБЩЕНИЙ
 Для обеспечения связей сообщения и методов обработки используются макросы — карты сообщений. Примеры интерфейсов, где так сделано - Windows API, MFC.

### Особенности работы механизма сигналов и слотов следующие:
- сигналы и слоты не являются частью языка C++, поэтому требуется запуск дополнительного препроцессора перед компиляцией программы;
- отправка сигналов происходит медленнее, чем обычный вызов функции, который производится при использовании механизма функций обратного вызова;
- существует необходимость в наследовании класса QObject;
- в процессе компиляции не производится никаких проверок: имеется ли сигнал или слот в соответствующих классах или нет; совместимы ли сигнал и слот друг с другом и могут ли они быть соединены вместе. Об ошибке можно будет узнать лишь тогда, когда приложение будет запущено. Вся эта информация выводится на консоль, поэтому, для того чтобы увидеть ее в Windows, в проектном файле необходимо в секции CONFIG добавить опцию console.

###  Что такое Сигнал:
http://blog.kislenko.net/show.php?id=1308&s=0

Сигналы (signals)
- это методы, которые в состоянии осуществлять пересылку сообщений.
- Сигналы определяются в классе, как обычные методы, но без реализации.
- Выслать сигнал можно при помощи ключевого слова emit. 
- emit – пустой макрос. Он даже не парсится MOC. Другими словами, emit опционален и ничего не значит (подсказка для разработчика).

#  Что такое Слот:
http://blog.kislenko.net/show.php?id=1308&s=0

Слоты (slots) 
- это методы, которые присоединяются к сигналам. По сути, они являются обычными методами. 
- Основное их отличие состоит в возможности принимать сигналы. Они определяются в классе как <b>private slots:</b>, <b>protected slots:</b> или <b>public slots</b>: ПО УМОЛЧАНИЮ СЛОТЫ  <b>PRIVATE</b>
- В слотах нельзя использовать параметры по умолчанию, например slotMethod (int n = 8)
- и нельзя определять слоты как static.

# Важно
в этом случае код после ключевого слова emit продолжает выполнение немедленно, а слоты будут выполнены позже.
Если несколько слотов подключены к одному сигналу, слоты будут выполнены один за другим в произвольном порядке после выработки сигнала.

Connect (Метод соединения объектов)
Соединение объектов осуществляется при помощи статического метода connect(), который определен в классе QObject.

В общем виде, вызов метода connect() выглядит следующим образом:
```
QObject::connect(const QObject* sender, 
const char* signal,
const QObject* receiver,
const char* slot,
Qt::ConnectionType type = Qt::AutoConnection
);
```

Ему передаются пять следующих параметров:
- sender — указатель на объект, отправляющий сигнал;
- signal — это сигнал, с которым осуществляется соединение. Прототип (имя и аргументы) метода сигнала должен быть заключен в специальный макрос SIGNAL(method());
- receiver — указатель на объект, который имеет слот для обработки сигнала;
- slot — слот, который вызывается при получении сигнала. Прототип слота должен быть заключен в специальном макросе SLOT(method());
- type — управляет режимом обработки. Имеется три возможных значения:

### ConnectionType type, 4 возможных значения:
- **В ОБЩЕМ СЛУЧАЕ:**

| Connections                 | Type | Description                                                             |
| --------------------------- |:----:|:-----------------------------------------------------------------------:|
| Qt::DirectConnection        | Sync | сигнал обрабатывается сразу вызовом соответствующего метода слота       |
| Qt::QueuedConnection        | Async| сигнал преобразуется в событие и ставится в общую очередь для обработки |
| Qt::AutoConnection          |Sy/Asy| это автоматический режим, который действует следующим образом (СМ. ТУТ) |
| Qt::BlockingQueuedConnection| Async| как и QueuedConection но текущий поток блокается пока слот не доставится|
| Qt::UniqueConnection        | -    | Передаётся как флаг " | Qt::UniqueConnection" блочит повторный коннект  |
| Qt::UniqueConnection        | -    | одних и тех же сигналов и слотов                                        |

**(СМ. ТУТ)**:
- если отсылающий сигнал объект находится в одном потоке с принимающим его объектом, то устанавливается режим Qt::DirectConnection - CИНХРОННЫЙ Вариант,
- в противном случае — режим Qt::QueuedConnection - АСИНХРОННЫЙ. Этот режим (Qt::AutoConnection) определен в методе connection() по умолчанию.

- **В МНОГОПОТОЧНОМ СЛУЧАЕ:**

Qt поддерживает следующие типы соединений сигнал-слот:

- Автоматическое соединение **(Auto Connection)** (именно оно выставлено 5-тым параметром у connect по умолчанию)
**Работает так**: идёт проверка находятся ли emit-ящий объект и объект слота в одном или в разных потоках, и принимается решение:
1. Если источник и получатель находятся в ОДНОМ и том же потоке - определить поведение такое же, как и при DirectConnection (см ниже),  2. Если источник и получатель находятся в РАЗНЫХ потоках - определить поведение такое же, как и при QueuedConnection (см. ниже).
- Прямое соединение **(Direct Connection)** Слот вызывается немедленно при отправке сигнала. Слот выполняется в потоке отправителя (в том потоке где был выполнен emit), который не обязательно является потоком-получателем. Это соединение ВСЕГДА исполняет ваш код внутри ОДНОГО потока, даже в случае, если вызываемый слот находится в другом. (Фактически это прямой вызов колбэк функции в момент эмита сигнла - Что может привести к проблемам. Помните, что использование прямых соединений, когда отправитель и получатель "живут" в **разных** потоках, опасно в случае, если цикл обработки событий выполняется в потоке, где "живет" приемник, по той же самой причине, по которой небезопасен вызов функций объекта, принадлежащего другому потоку.)
- Соединение через очередь сообщений **(Queued Connection)** Слот вызывается, когда управление возвращается в цикл обработки событий в потоке получателя. Слот выполняется в потоке получателя. Нюансы: Сигнал преобразуется в событие, и ставится в общую очередь или ЦИКЛ сообытий (event loop) в потоке получаетля для обработки. (В РАЗНЫХ потоках, имитированные сигналы не будут вызывать слот сразу, они будут помещены в цикл событий потока, и будут исполнены, когда придёт их очередь. НЮАНС имеется следующий, сигнал не сможет быть доставлен (соотвественно слот не сможет быть вызван), пока не закончится выполнение той функции, в которой был имитирован сигнал, то есть нельзя в случе Queued Connection, писать что-то типа while(true) { emit mySignal(); } )
- Блокирующее соединение через очередь **(Blocking Queued Connection)** Слот вызывается так же, как и при соединении через очередь, за исключением того, что текущий поток блокируется до тех пор, пока слот не возвратит управление. Замечание: Использование этого типа подключения объектов в одном потоке приведет к взаимной блокировке.
- Уникальное соединение **(Unique Connection)** Поведение такое же, что и при автоматическом соединении, но соединение устанавливается только если оно не дублирует уже существующее соединение. т.е., если тот же сигнал уже соединён с тем же самым слотом для той же пары объектов, то соединение не будет установлено и connect() вернет false.

Эти типы соединений, передаются 5-тым аргументом в функцию connect(). 

- Ещё есть способ, чтобы заставить выполнение кода "прыгнуть" в выполнение слота в другом потоке, делается это через вызов метода:
QMetaObject::invokeMethod( pointerToObject*, "functionName", Qt::QueuedConnection);

QObject::connect() сама по себе потокобезопасна.

**СУТЬ:** Всё построена на QThread-ах, они начитнают своё выполнение после вызова фции run(). По умолчанию, run() запускает цикл сообщений (event loop), вызывая соотвественно exec() что и запускает Qt-шный цикл обработки сообщений (event loop) внутри потока.
Запуск собственного цикла обработки сообщений и дает возможность получать их в том числе и из других потоков. Фишка сигналов/слотов кьюта в том, что это еще и простая message queue система. Вызов в рамках одного потока - это прямой вызов подписанных слотов на этом же стеке, а вот QueuedConnection или межпоточные вызовы кладут в очередь соответствующего потока сообщение (эти сообщения лежат в одной очереди с сообщениями от ОС), и другой поток будет их извлекать в своём event-loop-е. Таким образом, вы просто просите другой поток вызвать слот, а вызовет он его когда в очередной раз будет отрабатывать цикл сообщений и он дойдет до сообщения о вызове слота.

https://ru.stackoverflow.com/questions/758609/qt-%D0%BD%D0%B5-%D0%B2%D1%8B%D0%B7%D1%8B%D0%B2%D0%B0%D0%B5%D1%82%D1%81%D1%8F-%D1%81%D0%BB%D0%BE%D1%82-%D0%B2-%D0%B4%D1%80%D1%83%D0%B3%D0%BE%D0%BC-%D0%BF%D0%BE%D1%82%D0%BE%D0%BA%D0%B5#comment1174156_758616

Запускаем QThread::start() поток. => начинается выполнение run(), Для того чтоб обрабатывать сообщения, внтури, нужно запустить QEventLoop::exec(). И далее как работают сигналы. В краце: Есть DirectConnection и QueuedConnection способы обработки сигнала, первый работает прямым вызовом колбэк функции в момент эмита сигнла, QueuedConnection отправляет служебное сообщение очередь потока к которому приписан объект и пока эта сообщение из очереди не будет обработано сигнал не дойдет до получателя. Для того чтоб обработать сообщение нужно создать QEventLoop объект (это и есть очередь) и вызвать exec() метод, это обработка сообщений. Когда ты запускаешь main, в нем есть QApplication::exec() это и есть запуск QEventLoop объекта, который хранится в QApplication. Таким образом у тебя начинают работать сигналы отправленные через QueuedConnection, но только в главном потоке. Для других потоков надо делать все ручками


https://stackoverflow.com/questions/38376840/qt-signals-and-slots-direct-connection-behaviour-in-application-with-a-single-th

Let´s get this out of the way before diving into the basics.

If it is a direct connection, your code is ALWAYS executed on a single thread, regardless as to whether or not the slot is running on a different thread. (In most cases a VERY BAD idea)

If the connection is queued and the slot is running inside the same thread, it acts exactly like a direct connection. If the slot is running on a different thread, the slot will execute within its proper thread context that it is running in, so making the code execution multi-threaded.

If the connection is auto which is the default, for a good reason, then it will chose the appropriate connection type.

Now there is a way to force your code execution to jump into a slot in another thread, that is by invoking a method:

QMetaObject::invokeMethod( pointerToObject*, "functionName", Qt::QueuedConnection);

https://stackoverflow.com/questions/41299480/qt-4-8-connection-behavior-between-two-signals-and-one-slot-from-different-thr

If the type is Qt::DirectConnection, the second signal is always emitted from the thread that emitted the first signal.
if the type is Qt::QueuedConnection, the second signal is always queued to be invoked when control returns to the event loop of the receiver object's thread.
If the type is Qt::AutoConnection, the connection type is resolved when the signal is emitted and the thread of the sending object is ignored.

### Qt / QML:
###  QML + CPP:
https://habrahabr.ru/post/140899/

- Писать класс наследник от QQuickItem: QQuickItem класс на стороне С++ позволяет писать свои визуальные и не-визуальные QML объекты на C++ стороне.
- Регистрируем C++ типы при помощи qmlRegisterType<MyCppForQmlTypeClass>(): C++ классы можно регистрировать при помощи функции qmlRegisterType<MyQMLType>("com.yourcompany.xyz", 1, 0, "MyQMLType"); при помощи QMLной системы типов, позволяющая потом в qml писать имя класса и создавать в qml-е объекты этого класса как стандартные QML объекты.
- Можно использовать класс на стороне qml, просто сделав его наследником QObject : QObject-ы могут быть загеристрированны в нутри QML-ного контекста, что позволяет внутри qml обращаться к их Q_PROPERTY и даёт прямой доступ к ним внутри qml. (Регистрация Context Q_PROPERTY)
- Доступ до QML объектов через Дерево QML-объектов: все QML объекты хранятся в древовидной иерархии и доступ к ним можно получить через root()-элемент в дереве.


# Как передавали данные в QML из C++ ?

- Первый вариант:

cpp: QDeclarativeView -> SetSource(some.QML) -> RootObject -> RootContext()->SetContextProperty("QtFun", this);

qml: QtFun.function_name();

h: Q_INVOCABLE function_name(); 

- Второй Вариант

```
class TestClass : public QObject
{
 Q_OBJECT
  Q_PROPERTY(int someProperty READ getSomeProperty WRITE setSomeProperty NOTIFY somePropertyChanged)
public:
 explicit TestClass(QObject *parent = 0);
 int getSomeProperty()const;
 void setSomeProperty(const int &);
private:
 int someProperty;
signals:
 void somePropertyChanged();
 public slots:
};
int TestClass::getSomeProperty()const
{
 qDebug() << "I'm getter";
 return someProperty;
}
void TestClass::setSomeProperty(const int &i)
{
 qDebug() << "I'm setter";
 someProperty = i;
}
```

- emit - ничего не делает, синтаксический сахар перед функцией-сигналом, его можно и не писать и сигнал всё равно вызовется


### - Signal Slot Permissions:
https://stackoverflow.com/questions/19129133/qt-signals-and-slots-permissions

### - EMIT можно и НЕ писать при высылке сигнала
это синтаксический сахар, который нужен не компиляторам с++ или кутэ, а больше программисту

```emit someSignal();``` 
Идентично простому вызову:
```someSignal();``` 

### - SLOT по умолчанию НИКАКОЙ (требует самим дописать модификатор доступа)
могут быть объявлены как virtual, public, protected, private

```
#define slots
is defined as an empty macro and therefore can be used with:
```
Программист ДОЛЖЕН написать перед slots что-то сам, иначе ошибка.

### - SIGNAL по умолчанию PROTECTED (в Qt4) но PUBLIC (в Qt5)
Signals are protected in Qt4 but are public in Qt5
```
qobjectdefs.h (QT5.0+). In there are defined the moc macros
#define signals public
```

Такой вариант тоже будет работать:
```
private:
public: //signals:
    void theSignal();
```

A signal can be defined with QPrivateSignal as its final argument.

### ВИРТУАЛЬНЫЙ Слот
https://stackoverflow.com/questions/19129133/qt-signals-and-slots-permissions

### Соединение СИГНАЛА с ВИРТУАЛЬНЫМ Слотом МЕДЛЕННЕ, чем с Невритуальным.
- События могут обрабатываться лишь одним методом, а сигналы многими слотами. 

### Чем отличается механизм Q_INVOCABLE и SLOT / Qinvocable?
### Разница между Q_INVOCABLE и SLOT-ом ?

Q_INVOCABLE - привязывается как обычная колбэчная функция, с одним методом, вызываемом при "emit"-е, для данной функции. Функции С++ вызывающиеся в QML.

SLOT - может быть любоче число сигналов, как и на любой сигнал можно повесить любое число слотов.

### QObject это базовый класс для всех Qt классов. 

###  Наследование класса от QObject-а.
Нужно в иерархии наследования выставлять его первым.

### Для чего нужен макрос Q_OBJECT (ТОЛЬКО В PRIVATE СЕКЦИИ !!!)
### Зачем нужен макрос Q_OBJECT ?

Q_OBJECT макрос используется для включения мета объектных функций в классах и на этапе компиляции moc (Meta Object Compilator) работает как препроцессор который преобразует применения макроса Q_OBJECT в исходный код C++.

Внутри Q_OBJECT:
- QMetaObject
- qt_metacall
- META_MACROS
- Q_MOC_RUN
- signals, slots, Q_PROPERTY, и прочее

### Что такое MOC

Мета-объектный компилятор, moc, - программа, которая обрабатывает расширения C++ от Qt.
moc работает как препроцессор который преобразует применения макроса Q_OBJECT в исходный код C++

Инструмент moc читает заголовочный файл C++. Если он находит одно или более объявлений классов, которые содержат макрос Q_OBJECT, то он порождает файл исходного кода C++, содержащий мета-объектный код для этих классов. Кроме всего прочего, мета-объектный код требуется механизму сигналов и слотов, информации о типе времени выполнения и системы динамических свойств.

Файл исходного кода C++, сгенерированный moc, должен компилироваться и компоноваться с помощью реализации класса.

Если вы используете qmake для создания своих make-файлов, в правила сборки будет включен вызов moc когда это необходимо, поэтому вам не нужно использовать moc непосредственно. Дополнительную информацию по moc смотрите в статье Почему Qt не использует шаблон для сигналов и слотов?



Клиент тсерверное приложение
- контроль целостности файлов / папок / ключей реестра
- Политики пользователей
- 

Qt сначала писалось на Виджетах

Осовременить графически:
Если нужен более интересный дизайн интерфейса, то идём в QML
Много динамики: Опасити менялось, что-то пропадало, на его месте что-то появлялось.
изменение цвета с одного на другой, вылетающие, двигающиеся элементы, почти анимация присуствовала.
Мы смотрели на решения от конкурентов думали как повотрить то или инове поведение, что 
хочется перенять из интерфейса того

Вёрстка на QML

Прямое соответсвие Виджетов и MFC-шных классов


# MFC
https://www.go4expert.com/forums/mfc-interview-questions-t724/


# Назовите 10 MFC-классов ?

# Назовите 10 QML-классов ?
CheckBox
DialogBox
 Slider
 Rectangle
 Column 
 Button 
 Text
 
 MouseArea
 Component

ListView
GridView
 


```
#define Q_OBJECT \
public: \
	Q_OBJECT_CHECK \ QT_WARNING_PUSH \ Q_OBJECT_NO_OVERRIDE_WARNING \
	static const QMetaObject staticMetaObject; \
	virtual void *qt_metacast(const char *); \
	virtual int qt_metacall(QMetaObject::Call, int, void **); \
	QT_TR_FUNCTIONS \
private: \
	Q_DECL_HIDDEN_STATIC_METACALL static void qt_static_metacall(QObject *, QMetaObject::Call, int, void **); \
	struct QPrivateSignal {}; \
	QT_ANNOTATE_CLASS(qt_qobject, "")
	
#ifndef QT_NO_META_MACROS

#else // Q_MOC_RUN
#define slots slots
#define signals signals
#define Q_SLOTS Q_SLOTS
#define Q_SIGNALS Q_SIGNALS
#define Q_CLASSINFO(name, value) Q_CLASSINFO(name, value)
#define Q_INTERFACES(x) Q_INTERFACES(x)
#define Q_PROPERTY(text) Q_PROPERTY(text)
#define Q_PRIVATE_PROPERTY(d, text) Q_PRIVATE_PROPERTY(d, text)
#define Q_REVISION(v) Q_REVISION(v)
#define Q_OVERRIDE(text) Q_OVERRIDE(text)
#define Q_ENUMS(x) Q_ENUMS(x)
```

# Как работает класс QObject / Макрос Q_OBJECT ?

На первом шаге запускается  MOC-компилятор. Meta Object Compiler, он у всех классов берёт и смотрит ПЕРВЫЙ класс среди базовых это  QObject или НЕ_QObject и далее действует, и внутри класса уже он же смотрит чтобы в "шапке" класса стоял Q_OBJECT, иначе он не отработает и пойдут ошибки компиляции.

# Возможно ли МНОЖЕСТВЕННОЕ НАСЛЕДОВАНИЕ от класса QObject ?

Нельзя и никаким образом. MOC просто не понимает классов, наследованных дважды от QObject при любой глубине дерева наследования 

# Важен ли порядок при наследовании от QObject ?
https://www.linux.org.ru/forum/development/814529

Должен стоять ПЕРВЫМ наследником, среди списка наследников:

moc просто не понимает классов, в которых QObject стоит не на первом месте (в качестве базового класса)

	class classA : public QObject, classV, classC
	{
	  Q_OBJECT

	  classA();
	  ...
	};



# Какой 5-тый параметр у функции Connect(Object, SIGNAL, Object, SLOT, SynhroniousAsync) ? (по умолчанию стоит Auto Connection)
# После соединения SIGNAL-a и SLOT-а, Как будет вызвана SLOT(func) функция, которая вызовется через emit СИНХРОННО или АСИНХронно ?

- Зависит всё от следующего: Работает ли всё в одном потоке или нескольких.

А именно: Auto Connection - выставлен по умолчанию => в ОДНОМ потоке будет СИНХронный вызов.


# Два раза вызвали Connect с одинаковыми параметрами - Что будет после emit SIGNAL ?

Двойной вызов функции Слота. ПО УМОЛЧАНИЮ !!! 	А так этот момент и задаётся в функции Connect 5-тым параметром.


Direct / Queued

https://stackoverflow.com/questions/38376840/qt-signals-and-slots-direct-connection-behaviour-in-application-with-a-single-th

Let´s get this out of the way before diving into the basics.

- If it is a direct connection, your code is ALWAYS executed on a single thread, regardless as to whether or not the slot is running on a different thread. (In most cases a VERY BAD idea)
- If the connection is queued and the slot is running inside the same thread, it acts exactly like a direct connection. If the slot is running on a different thread, the slot will execute within its proper thread context that it is running in, so making the code execution multi-threaded.
- If the connection is auto which is the default, for a good reason, then it will chose the appropriate connection type.

- Now there is a way to force your code execution to jump into a slot in another thread, that is by invoking a method:

QMetaObject::invokeMethod( pointerToObject*, "functionName", Qt::QueuedConnection);

https://stackoverflow.com/questions/41299480/qt-4-8-connection-behavior-between-two-signals-and-one-slot-from-different-thr

- If the type is Qt::DirectConnection, the second signal is always emitted from the thread that emitted the first signal.
- if the type is Qt::QueuedConnection, the second signal is always queued to be invoked when control returns to the event loop of the receiver object's thread.
- If the type is Qt::AutoConnection, the connection type is resolved when the signal is emitted and the thread of the sending object is ignored.

###### If the receiver object lives in the same thread where the first signal is emitted, this will be the same as using Qt::DirectConnection.
###### Otherwise, this will be the same as using Qt::QueuedConnection.

http://www.doc.crossplatform.ru/qt/4.6.x/threads-qobject.html



### Q: Casablanca c++Library for Rest ?

// TO DO


### Q: С++ 14 Стандарт ?
https://ru.wikipedia.org/wiki/C%2B%2B14
```
1	Изменения в языке
1.1	Вывод типа возвращаемого значения для функций
1.2	Альтернативный вывод типа при объявлении
1.3	Уменьшение ограничений на константные выражения
1.4	Шаблоны переменных
1.5	Агрегатная инициализация классов с инициализаторами полей
1.6	Литералы двоичных чисел
1.7	Разделители разрядов
1.8	Обобщённые лямбда-функции
1.9	Захват выражений для лямбда-функций
1.10	Атрибут [[deprecated]]
2	Новые функции стандартной библиотеки
2.1	Разделяемые мьютексы и блокировки
2.2	Гетерогенный поиск в ассоциативных контейнерах
2.3	Стандартные пользовательские литералы
2.4	Адресация к кортежам по типу
2.5	Прочие изменения стандартной библиотеки
3	Примечания
```
### Q: Плюсы и минусы СИНГЛТОН ?

- (+) Класс сам контролирует процесс создания единственного экземпляра.
- (+) Паттерн легко адаптировать для создания нужного числа экземпляров.
- (+) Возможность создания объектов классов, производных от Singleton.

- (-) В случае использования в нескольких потоках и нескольких взаимозависимых одиночек их реализация может резко усложниться.

### Q: Проблемы которые решает СИНГЛТОН ?

Проблема: В многопоточном приложении одновременно 2 или более потоков захотят вызвать функцию получения объекта синглтона с одним и тем же типом T?

https://habr.com/ru/post/150276/

1. Старый стандарт $6.7.4, C++03 - опускал какую либо информацию о многотопоточности при процессе инициализации объектов.
2. Новый стандарт  $6.7.4, C++11 - говорит о том, что если во время инициализации переменной (т.е. создания экземпляра) второй поток пытается получить доступ к этой же переменной, то он (поток) должен ожидать завершения инициализации, при этом реализация не должна допускать ситуации deadlock.

Вопрос остётся открытым лишь в том смысле - какие компиляторы действительно поддерживают новый стандарт, а какие лишь пытаются делать вид, что поддерживают. Что стоит проверять лишь экспериментально.

- (+) Ожидание (Как должно работать): Синглтон инициализируется только один раз и функция threadFunction завершает свою работу только после завершения инициализации синглтона => корректная инициализация объекта в многопоточном окружении.
- (-) Ошибка  (Негативный результат): Компилятор для первого потока начинает инициализировать синглтон, а для остальных — возвращает сразу объект, который даже не успел проинициализироваться => правильная работа в многопоточной среде может быть не обесепчена.

1. Улучшение - через mutex:
- перед созданием объекта мы будем использовать мьютекс для синхронизации доступа к объекту:
- (+) избавляет от проблемы возвращения неинициализированного объекта: перед началом инициализации вызывается mutex.lock(), а после завершения инициализации вызывается mutex.unlock(). Остальные потоки ожидают завершения инициализации перед тем, как начать его использовать. 
- (-) минус: блокировка используется всегда, вне зависимости от того, проинициализирован ли уже объект или нет. 

2. Улучшение - через "Double-Checked Locking Pattern":
- проверка условия: проинициализирован или нет? Если да — то сразу возвращаем ссылку на объект
- берем блокировку
- проверяем условие второй раз, если проинициализирован — то снимаем блокировку и возвращаем ссылку
- проводим инициализацию синглтона
- меняем условие на «проинициализирован»
- снимаем блокировку и возвращаем ссылку

3.1 Через класс SingletonDestroyer, для автоматического разрушения объекта Singleton.
3.2 Использование нескольких взаимозависимых одиночек

http://cpp-reference.ru/patterns/creational-patterns/singleton/

### Q: Потокобезопасность Синглтона Мейерса / Майерса ?
A: НЕ ПОТОКОБЕЗОПАСЕН - в его Традиционной реализации с отложенной инициализацией (lazy initialization). В современном C++11, этот паттерн считается ПОТОКОБЕЗОПАСНЫЙ, о чём написано в стандарте §6.7 [stmt.dcl] p4.

https://habr.com/ru/post/150276/

Из стандарта: If control enters the declaration concurrently while the variable is being initialized, the concurrent execution shall wait for completion of the initialization.

- Visual Studio: supported since Visual Studio 2015
- GCC: supported since GCC 4.3

Ранее проблему решали с использованием метода "Double-Checked Locking" / "Double-Checked Locking Pattern", хоть он и был более затратен.
По причине возникновения такого решения, синглтон с использованием "Double-Checked Locking" назвали Синглтон Дейкстры.

```
In C++03, this code wasn't thread safe. There is an article by Meyers called 
"C++ and the Perils of Double-Checked Locking" which discusses thread safe 
implementations of the pattern, and the conclusion is, more or less, that 
(in C++03) full locking around the instantiating method is basically the 
simplest way to ensure proper concurrency on all platforms, while most forms 
of double-checked locking pattern variants may suffer from race conditions 
on certain architectures, unless instructions are interleaved with 
strategically places memory barriers.
```

### Q: Синглтон (небезопасный в многопоточном окружении) КОД
```
template<typename T>
T& single()
{
    static T t;
    return t;
}
```

### Q: Синглтон Класс (небезопасный в многопоточном окружении)
```
// Singleton.h
class Singleton
{
  private:
    static Singleton * p_instance;
    // Конструкторы и оператор присваивания недоступны клиентам
    Singleton() {}
    Singleton( const Singleton& );  
    Singleton& operator=( Singleton& );
  public:
    static Singleton * getInstance() {
        if(!p_instance)           
            p_instance = new Singleton();
        return p_instance;
    }
};
  
// Singleton.cpp
#include "Singleton.h"
  
Singleton* Singleton::p_instance = 0;
```

### Q: Синглтон Мейерса / Майерса (небезопасный в многопоточном окружении) КОД 
```
// Singleton.h
class Singleton
{
private: 
    Singleton() {}
    Singleton( const Singleton&);  
    Singleton& operator=( Singleton& );
public:
    static Singleton& getInstance() {
        static Singleton  instance;
        return instance;
    }    
}; 
```

### Q: Синглтон  DCLP (Double-Checked Locking Pattern) КОД
```
// небезопасная функция в многопоточном окружении
template<typename T>
T& singleUnsafe()
{
    static T t;
    return t;
}

// функция для использования в многопоточном окружении
template<typename T>
T& single()
{
	static T* volatile pt;
	if (pt == 0)
	{
		T* tmp;
		{
			StaticLock lock;
			tmp = &singleUnsafe<T>();
		}
		pt = tmp;
	}
	return *pt;
}
```
### Q: Синглтон Мейерса / Майерса КОД

// TO DO

### Q: Синглтон Дейкстры КОД

// TO DO

### Q: Синглтон Александреску КОД

// TO DO



# EED0CDB4-3867-4BED-BD73-0752E8D0184F

### Есть 2 строки из символов латинского алфавита, буквы в разном регстре строчные и прописные присуствуют, которые имеют одинаковую длину. Является ли одна строка перестановкой символов другой строки. Повторяющиеся символы присутствуют ?
Пример: 
1 строка автомобиль: automobile
2 строка  перемешка: abeilmootu

Решение:
2 МультиСэта заводим.
последовательно в каждый помещаем все элементы одной строки и второй строки. Сравниваем 2 заполненных мультисета.

https://tproger.ru/problems/method-to-decide-if-two-strings-are-anagrams-or-not/

### Метод, определяющий, является ли одна строка перестановкой другой

Способ 1. Сортировка строк.
Если строки являются анаграммами, то они состоят из одинаковых символов, расположенных в разном порядке. Сортировка двух строк должна упорядочить символы. Теперь остается только сравнить две отсортированные версии строк.
```
public String sort(String s) {
    char[] content = s.toCharArray();
    java.util.Arrays.sort(content);
    return new String(content);
}

public boolean permutation (String s,String t) {
    if (s.length() != t.length()) {
        return false;
    }
    return sort(s).equals(sort(t));
}
```

Способ 2. Проверка счетчиков идентичных символов.
Для реализации этого алгоритма можно использовать свойство анаграммы – одинаковые “счетчики” символов. Мы просто подсчитываем, сколько раз встречался каждый символ в строке. Затем сравниваем массивы, полученные для каждой строки.

Время работы второго способа O(n)

```
public boolean permutation(String s, String t) {
    if (s.length() != t.length()) {
        return false;
    }

    int[] letters = new int[256];

    char[] s_array = s.toCharArray();
    for (char c : s_array) {
        letters[c]++;
    }

    for (int i = 0; i < t.length(); i++) {
        int c = (int) t.charAt(i);
        if (--letters[c] < 0) {
            return false;
        }
    }

    return true;
}
```

# Как в QML нарисовать Круг

1. Ректангл с половинным радиусом. Rectangle { width: 10; height: width; radius: width/2; }
2. На Канвасе нарисовать по точкам: Canvas c использованием Context2D:
```
Rectangle { Canvas { anchors.fill: parent onPaint: {
var ctx = getContext("2d"); ctx.beginPath();ctx.moveTo(centreX, centreY); ctx.arc();
```
3. Шейпы (Shapes) и в них используем Arc-и. Можно один ShapePath с двумя или одним PathArc - https://doc.qt.io/qt-5/qml-qtquick-shapes-shape.html
Пример: https://www.qt.io/blog/2017/07/07/let-there-be-shapes
4. Qml + QPainter - наследуемся от QQuickPaintedItem, переопределяем paint()
```
#include <QtQuick/QQuickPaintedItem> 
class PieChart : public QQuickPaintedItem    
{ public: void paint(QPainter *painter); };
Source:
void PieChart::paint(QPainter *painter)
{ QPen pen(m_color, 2); painter->setPen(pen); ... }
```
5. QML Scene Graph framework class MyCircle : public QQuickPaintedItem
https://doc.qt.io/qt-5/qtquick-customitems-painteditem-example.html
6. Какими-нибудь средствами OpenGL

# Model-View в QML
- ListModel
- VisualItemModel
- XmlListModel
- FolderListModel
- JavaScript-модели

# C++-модель 
- QAbstractItemModel
Это стандартная модель из фреймворка Qt Model-View. Этот класс обладает богатыми возможностями и позволяет строить модели различной сложности.

Существуют три базовых класса для таких моделей. 
- QAbstractTableModel представляет данные в виде таблицы, для доступа к данным используется номер строки и столбца. 
- QAbstractListModel представляет данные в виде списка и, можно сказать, является частным случаем предыдущей модели с одним столбцом.
- QAbstractItemModel - наоборот, более обобщенная версия. Каждый элемент таблицы может иметь еще и дочерние элементы, тоже организованные в виде таблицы. Таким образом, при помощи этой таблицы можно организовать древовидную структуру. 

- QFileSystemModel

QSortFilterProxyModel, - Между моделью и представлением можно вставить специальную прокси-модель. Такие модели перехватывают вызовы к основной модели и могут скрывать определенные элементы, менять их порядок, влиять на получение и запись данных и т.п. В Qt есть готовый класс QSortFilterProxyModel.

Представления в **QML могут отображать ТОЛЬКО СПИСКИ**. При помощи VisualDataModel можно перемещаться по древовидной структуре, но отображать мы можем только элементы текущего уровня. Если данные нужно хранить **в виде дерева** и при этом **отображать в QML**, то стоит либо воспользоваться VisualDataModel либо писать свою прокси-модель, которая превратит это дерево в список.

# Своя модель / Кастомная C++-модель 

http://security-corp.org/programming/33588-model-view-v-qml-chast-chetvertaya-c-modeli.html

Для модели-списка нужно создать производный класс от QAbstractListModel и определить такие методы:
- rowCount() — возвращает количество строк, в нашем случае это количество элементов;
- data() — возвращает данные элемента;
- roleNames() — возвращает список ролей, которые доступны в делегате. По умолчанию определены следующие роли: display, decoration, edit, toolTip, statusTip и whatsThis. В четвертой версии Qt вместо переопределения этой функции нужно было вызвать функцию setRoleNames(), которая устанавливала нужные имена ролей.
- data()
- setData()

Если нам нужна **модель с ДРЕВОВИДНОЙ структурой** (нужно **ДЕРЕВО**), мы используем QAbstractItemModel. У этой модели надо будет дополнительно определить такие функции:
- parent() — возвращает индекс родителя элемента;
- index() — возвращает индекс элемента.
Корневой элемент модели имеет недействительный индекс QModelIndex(). Так что если у нас простой список или таблица — у всех элементов родительский элемент будет именно таким.  Функция index() получает индекс родителя и номер строки и столбца элемента, должна возвращать индекс элемента.


### SDI- и MDI-приложения в Qt
Главное отличие MDI- от SDI-приложения состоит в том, что SDI-приложение содержит только одно окно документа, a MDI-приложение способно содержать несколько таких окон, что дает пользователю возможность параллельной работы с несколькими документами.

Класс **QMainWindow** предоставляет уже готовый лейаут, размещающий в себе виджеты, необходимые большинству приложений. В центре размещена рабочая область, которая может содержать только один виджет. 

При помощи класса **QWorkspace** в этой области можно размещать сразу несколько виджетов, что позволяет реализовывать MDI-приложения. Виджеты находятся в рабочей области, в виде отдельных окон, которые можно перемещать, изменять размеры, сворачивать, разворачивать, упорядочивать их и т. д.

Класс действия **QAction** 


### Как работает и используется moveToThread() ?
// TO DO

### Как работает и используется QObject::deleteLater ?
// TO DO

### Событийно-ориентированное программирование ?
Суть: сигнал - коннектится на много слотов, в функции каждогог из которых крутится своя логика.

### Зачем нужен QSignalMapper ?
Класс QSignalMapper необходим в ситуациях, когда много сигналов подключены к одному и тому же слоту, и этот слот должен реагировать на каждый сигнал по-разному.

### Новый синтаксис УСТАНОВКИ связи сигналов и слотов в QT5

- Старый синтаксис
В Qt 5 по прежнему осталась возможность использования старого синтаксиса для связи сигнала и слотов old string-based syntax определенные в QObject или любом наследнике класса QObject (включая QWidget).
```
connect(sender, SIGNAL (valueChanged(QString,QString)),
 receiver, SLOT (updateValue(QString)) );
```
На самом деле макросы SIGNAL and SLOT преобразуют свои аргументы в строки, connect потом срвнит то что выкинет moc.

Минусы!
- Невозможность проверки во время компиляции: Все проверки осуществляются во время исполнения, после парсинга строк. А это значит, что если в название сигнала или слота вкрадется опечатка, то программа успешно скомпилируется, но соединение не будет создано. Все что мы увидим — это предупреждение во время исполнения.
- Так как все операции проходят со строками, имена типов в слотах обязаны буквально совпадать с именами типов в сигналах. Кроме того они должны совпадать в заголовочных файлах и в коде, описывающем соединение. А это означает проблемы при попытке использовать typedef-ы или пространства имен.

- Новый синтаксис (использование указателей на функции)
Способ связи сигнала и слота между двумя объектами класса QObjects и не строковыми параметрами:
```
connect(sender, &Sender::valueChanged,
 receiver, &Receiver::updateValue );
```
Плюсы:
- Проверка во время компиляции - существования сигнала и слота.
- Аргумент может иметь псевдоним typedefs или находится в другом пространстве имен.
- Автоматическое преобразование типов при неявном объявлении (например, из QString в QVariant)
- Возможность связать любую член-функцию QObject, необязательно слот.
Минусы:
- Кому-то лень указывать явно тип объекта
- Сложный синтаксис в случае перегрузки? (см. ниже)
- Не поддерживаются значения параметров по умолчанию в слотах.

- Связь с простой свободной функцией
connect(sender, &Sender::valueChanged, someFunction);

Плюсы:
- можно использовать tr1::bind
- можно использовать лямбда-выражения c+11
- (Был минус, но с 5.2 устранён) Нет способа автоматического разрыва связи когда удалится объект 'receiver', потому что функтор не является QObject. Однако, начиная с версии Qt 5.2 существует перегрузка, которая добавляет "контекст объекта". Когда объект уничтожен, связь разрывается.
```
connect(sender, &Sender::valueChanged,
 tr1::bind(receiver, &Receiver::updateValue, "senderValue", tr1::placeholder::_1) );
```
```
connect(sender, &Sender::valueChanged, [=](const QString &newValue) {
 receiver->updateValue("senderValue", newValue);
 } );
``` 

### Новый синтаксис РАЗРЫВА связи сигналов и слотов в QT5

- Старый синтаксис
```
disconnect(sender, &Sender::valueChanged,
 receiver, &Receiver::updateValue );
```
Этот способ работает только если связь была установлена таким же способом. Не работает со статическими функциями, функторами и лямбда-выражениями.

- Новый способ с использованием QMetaObject::Connection
```
QMetaObject::Connection m_connection;
m_connection = QObject::connect(…);
QObject::disconnect(m_connection);
```



# QT - РАЗНИЦА между сигналами и событиями / СИГНАЛЫ и СОБЫТИЯ.

События могут обрабатываться лишь одним методом, а сигналы многими слотами

# QT - СИГНАЛЫ И СЛОТЫ:

В программировании графического интерфейса, когда мы меняем один виджет, мы часто хотим что бы другой виджет получил об этом уведомление. В общем случае, мы хотим что бы объекты любого типа могла общаться с другими. Например, если пользователь нажимает кнопку Закрыть, мы вероятно хотим что бы была вызвана функция окна close().
Другие библиотеки добиваются такого рода общения используя обратный вызов. Обратный вызов это указатель на функцию, таким образом, если мы хотим что бы функция уведомила нас о каких-нибудь событиях, мы передаем указатель на другую функцию (обратновызываемую) этой функции. Функция в таком случае делает обратный вызов когда необходимо. Обратный вызов имеет два основных недостатка. Во-первых, он не является типобезопасным. Мы никогда не можем быть уверены что функция делает обратный вызов с корректными аргументами. Во-вторых, обратный вызов жестко связан с вызывающей его функцией, так как эта функция должна точно знать какой обратный вызов надо делать.

Сигналы и слоты

В Qt используется другая техника — сигналы и слоты. Сигнал вырабатывается когда происходит определенное событие. Слот это функция, которая вызывается в ответ на определенный сигнал. Виджеты Qt имеют много предопределенных сигналов и слотов, но мы всегда можем сделать дочерний класс и добавить наши сигналы и слоты в нем.

Сигналы и слоты

Механизм сигналов и слотов типобезопасен. Сигнатура сигнала должна совпадать с сигнатурой слота-получателя. (Фактически слот может иметь более короткую сигнатуру чем сигнал который он получает, так как он может игнорировать дополнительные аргументы). Так как сигнатуры сравнимы, компилятор может помочь нам обнаружить несовпадение типов. Сигналы и слоты слабо связаны. Класс, который вырабатывает сигнал не знает и не заботится о том, какие слоты его получат. Механизм сигналов и слотов Qt гарантирует, что если мы подключим сигнал к слоту, слот будет вызван с параметрами сигнала в нужное время. Сигналы и слоты могут принимать любое число аргументов любого типа. Они полностью типобезопасны.
Все классы, наследуемые от QObject или его дочерних классов (например, QWidget) могут содержать сигналы и слоты. Сигналы вырабатываются объектами когда они изменяют свое состояние так, что это может заинтересовать другие объекты. При этом он на знает и не заботится о том что у его сигнала может не быть получателя. 
Слоты могут быть использованы для получения сигналов, но они так же нормальные функции-члены. Так же как объект не знает ничего о получателях своих сигналов, слот ничего не знает о сигналах, которые к нему подключены. Это гарантирует что полностью независимые компоненты могут быть созданы с помощью Qt.
Мы можем подключать к одному слоту столько сигналов, сколько захотим, также один сигнал может быть подключен к стольким слотам, сколько необходимо. Так же возможно подключать сигнал к другому сигналу (это вызовет выработку второго сигнала немедленно после появления первого).
Сигналы и слоты вместе составляют мощный механизм создания компонентов.


### Слоты не могут быть static !

### Слот не может быть static !

### Сигналы и слоты НЕ являются частью С++ !!
Дополлнительный Препроцессор MOC (Meta Object Compiller) перед компиляцией генерит дополнительный код на месте соотвествующих макросов !

### Какая программа создает заголовочный C++ файл для *.ui файла созданного в Qt-Designer?
uic

### Что если несколько слотов подключены к одному сигналу 

### Что если выполнить emit сигнала, который сконнекчен на несколько слотов ?
Если несколько слотов подключены к одному сигналу, слоты будут выполнены один за другим в произвольном порядке после выработки сигнала.
Сигналы автоматически генерируются программой moc и не должны быть реализованы в исходном коде. Они могут не возвращать значение (т. е., используем тип void).

### В каком порядке выполнгятся сигналы, если их заэмитил один и тот же сигнал ?
Сигналы выполнятся в произвольном порядке после выработки сигнала. (В ранних версиях QT работало правило, что слоты отработают в том порядке в котором для них была вызвана функция connect, примерно с версии 4.6 это уже давно не так)

### Можно ли перемещать между потоками SQL соединения и обращаться к ним из других потоков?
Нет

### Как называется класс для захвата звука в Qt?
QAudioInput

### Какие ограничения существуют в Qt при реализации множественного наследования?
- Если класс уже наследник QObject, то мы не можем его добавить в список множественных наследников нашего класса, если мы сами наследники от QObject.
- QObject или его потомок должен стоять **ПЕРВЫМ** в списке наследования

### Если сигнально-слотовое соединение осуществляется между объектами **разных потоков**, то ...
Оно преобразуется в **СОБЫТИЕ** !!!

### Когда используют отложенные соединения / Queued connection ?
В этом случае код после ключевого слова emit продолжает выполнение немедленно, а слоты ставятся в очередь исполнения событий и будут выполнены не сразу, а позже, как до них дойдёт их очередь.

### Что обозначает макрос Q_INTERFACES ?
Сообщает о списке реализованных в объекте интерфейсов
``` Пояснение: Q_INTERFACES ( ... ) Макрос Qt, который сообщает о том, какие интерфейсы реализованы.
Используется в плагинах. Например: class BasicToolsPlugin :
 public QObject, public BrushInterface, public ShapeInterface 
 { Q_OBJECT Q_INTERFACES(BrushInterface ShapeInterface ) public: ... };
```

### QGraphicsView класс виджет для
 для визуализации содержимого сцены **QGraphicsScene**


### QObject::moveToThread(...) для чего используется ?
- перемещать из потока в поток ???
- перемещать из потока в поток все классы модуля QtNetwork

### QObject::moveToThread(...) чего нельзя делать с помощью этого метода ?
- Соединения с базой данных SQL можно использовать только в том же потоке, который его создал.856 / 1479
- Ни один класс графического интерфейса нельзя переместить из главного потока в дочерние.

### В Qt есть следующие макросы ?
foreach, forever

### Просто slots без указания public/private/protected объявлять нельзя?

slots: void slot(int n); // **ERROR** НЕ хватает модификатора доступа !



# Что такое Small Object Optimization ?
Ответ: внутри класса string есть специальный буфер, под неболшие строки. В случае, если строка не больше, чем этот буфер (для небольших объектов), то память под строку не выделяется, а просто используется этот буфер, для оптимизации процесса выделения памяти подстроку. Ибо выделение памяти всегда дорогостоящий процесс.

# Всякие синтетические примеры
(); - ошибка копиляции - выражение в скобках должно быть выражение или любой другой вычислимый объект.
{}; - на месте скобок будет НОЛЬ - Пустой список инициализации по умолчанию инициализируется нулём.
A a{}; - вызовит конструктор по умолчанию
A a(); // это ПРОТОТИП функции, а не вызов конструктора класса А

int i = 1;
B b(A i); // это ПРОТОТИП функции, а не вызов конструктора класса А

B b(A{i}); // это ХЗ ???
B b( (A(i)) ); // Тоже что и ранее, но без ФИГУРНОЙ ИНИЦИАЛИЗАЦИИ

# Что такое RVALUE и LVALUE
RVALUE - временный объект
int x = 5; //  x - lvalue // 5 - rvalue //
int(10); // тоже rvalue
числа это - pure rvalue

Есть 3 категории:
- lvalue 
- xvalue (expired value)
- prvalue (pure rvalue)
Есть 2  доп категории:
- rvalue  = (pvalue + xvalue)
- glvalue = (lvalue + xvalue) generalized lvalue

https://www.youtube.com/watch?v=8fGP_c3pzNg
https://www.youtube.com/watch?v=N4treTtmy94
https://www.youtube.com/watch?v=xk9y5Bbzv2w

https://www.youtube.com/watch?v=kwyFsdSsTmc

- Основное отличие rvalue от lvalue в том, что объекты rvalue могут быть перемещены, тогда как объекты lvalue всегда копируются.

lvalue - с визуальной точки зрения - то, у чего есть имя.
rvalue - то у чего нет имени
Lvalue reference это любая конструкция типа "any_type&"
Rvalue reference это любая конструкция типа "any_type&&"

# Что такое RVALUE и LVALUE Reference / RVALUE и LVALUE Ссылка
Именно "&&" и есть новы тип называемый RValue **ЭРвэлью Ссылка** или **ЭрВэлью Рефереренс** или **Универсальная ссылка** или **Супер ссылка**

### Для чего на практике нужен Rvalue Reference ?
Для того, чтобы делать перегрузку функций. Пример:
```
string low_case(strin& s);
string low_case(strin&& s);
``` 

```
class Dog {
public:
  Dog() {}
};

int fun() {
  return 101;
};

int main() {
  int a; // lvalue
  Dog rex; // lvalue
  // ver 1: lvalue ref --binds--> lvalue 
  int& simpleRef = a;
  // ver 2: const lvalue ref --binds--> rvalue 
  const int& constRef = a;
  
  // ver 3: rvalue ref --binds--> rvalue ONLY ! 
  int&& rValRef = a; // COMPILLER ERROR a is LValue 
  
  fun(); // результат, который возвращает функция это rvalue
  Dog{}; // rvalue
}
```
# std::move - занимется ТОЛЬКО ПРИВЕДЕНИЕМ ЛЮБОГО ТИПА к типу rvalue. Внутри него происходит Удаление Всех ссылок из входного типа, и подставление Двух ссылок соотвественно, чтобы получилась Rvalue-ссылка.

```
template<typename _Tp>
  inline typename std::remove_reference<_Tp>::type&&
  move(_Tp&& __t)
  { return static_cast<typename std::remove_reference<_Tp>::type&&>(__t); }
```

```
// Пример 1
std::vector
int a = 3;
lvalue a
rvalue 5
выполняем push_back ( a );
В это время происходит копирование объекта a, и копия кладётся в вектор.
выполняем push_back ( 5 );
В этом случае произойдёт просто перемещение 5-ки в вектор, без еопирования а на месте пятёрки останется valid-ное значение.

// Пример 2
// str - lvalue   "Hello ..." - rvalue
std::string str = "Hello world!";
std::vector<std::string> v;
v.pushback(std::move(str));
Было  [Hello world!] Вектор: 0
Без копирования содержимого строки, содержимое именно "переместилось"
Стало [            ] Вектор: [Hello world!]

// ПРАВИЛО мы не можем передавать Rvalue в неконстатнтный Lvalue
void inc(int& n) { n++; }
inc(3) // ERROR !!! 3 - rvalue

void inc(const int& n) { ; }
inc(3) // OK !!!

// T& x = Rvalue
// T& x = Lvalue
// T&& x = Lvalue // ERROR !
```
 
# СКЛЕЙКА ССЫЛОК
```
T&   &  -> T&
T&  &&  -> T&
T&&  &  -> T&
T&& &&  -> T&&
```

### Универсальная ссылка:
```
template<typename T>
void foo(T && t) {}
```
- если вызвать foo от Lvalue типа А, то T = A&
- если вызвать foo от Rvalue типа А, то T = A


### fbvector — улучшенный std::vector от Facebook
https://habr.com/ru/company/infopulse/blog/238131/

### Аллокатор
http://artlang.net/post/vector-c++-effektivno/

# Стандартный Аллокатор в STL - STD::ALLOCATOR

std::allocator. Он, кстати, используется по умолчанию для всех контейнеров библиотеки STL.

Вектору(и всем другим контейнерам STL) от аллокатора нужно прежде всего, чтобы он выделял и освобождал некоторую область памяти, в тот момент когда это требуется контейнеру, Стандартный аллокатор делает это так :
```
template<class T>
class allocator
{
public:
  typedef T value_type;

  typedef value_type *pointer;
  typedef const value_type *const_pointer

  typedef size_t size_type;

  pointer allocate(size_type _Count)  // Выделяем память для _Count элементов
  {                                   // типа value_type
    void *_Ptr = 0;

    if (_Count == 0)  // Если ничего не запросили, то ничего и не делаем
    	;
    else if (((size_t)(-1) / sizeof(value_type) < _Count)
    	|| (_Ptr = ::operator new(_Count * sizeof(value_type))) == 0)
    {
    	throw bad_alloc();  // Выделение памяти не удалось
    }
    return ((pointer)_Ptr);
  }

  void deallocate(pointer _Ptr, size_type)
  {
	::operator delete(_Ptr);  // Освобождение памяти
  }

  // Остальная реализация не приводится, чтобы сохранить наглядность примера
};
```
Так аллокатор реализован у Microsoft(MSVC), и также он реализован в GCC.Нужно понимать, что operator new отличается от просто new.Вызов new (который все мы используем) на самом деле разбивается на два этапа, если так можно выразиться : сначала вызывается operator new, который возвращает указатель на некоторую выделенную область памяти, а затем вызывается конструктор, который создает объект в этой области.Вызвать конструктор напрямую невозможно, однако с помощью синтаксиса размещения можно заставить компилятор вызвать коструктор.Следующие два примера создания foo1 и foo2 идентичны :

```
#include <new>

class Foo
{
public:
  Foo() {}
};

int main(int argc, char** argv)
{
  // Вот так мы все привыкли создавать объект в "куче":
  Foo *foo1 = new Foo(); // Выделение памяти + Вызов конструктора

  // А вот какие вызовы происходят на самом деле:
  void *ptr = operator new(sizeof(Foo)); // Выделение памяти
  Foo *foo2 = ::new (ptr) Foo(); // Вызов конструктора, синтаксис размещения

  return 0;
}
```
Широко распространно заблуждение, что применение оператора new (первый случай в примере) подразумевает необходимость работать с кучей(heap). Вызов new, лишь означает, что будет вызвана функция operator new и эта функция возвратит указатель на некоторую область памяти, Стандартные operator new и operator delete действительно работают с кучей, но члены operator new и operator delete могут делать всё, что угодно!Нет ограничения на то, где будет выделена область памяти.Но вернемся к вектору.

После того, как память будет выделена, она “переходит” под управление вектора.

### Сколько нужно переменных вектору, чтобы реализовать весь свой интерфейс ?
Ответ: ТРИ . Три указатя достаточно, чтобы вектор смог управлять памятью и обеспечил весь свой функционал:

- первый(first) указатель указывает на начало выделенной области памяти
- второй(last) указатель указывает на позицию следующую за последним элементом, хранящимся в выделенной области памяти
- третий(end) указывает на “конец” выделенной области памяти


# STD::ARRAY
```
template < class T,
	std::size_t N
> struct array;
```

# STD::VECTOR / STD::LIST / STD::FORWARD_LIST / STD::DEQUE
```
template< class T, // 1
	class Allocator = std::allocator<T> // 2
> class vector, list, deque;
```

# STD::SET
```
template< class Key, // 1
	class Compare = std::less<Key>, // 2
	class Allocator = std::allocator<Key> //3
> class set;
```

# STD::MAP
```
template< class Key, // 1
	class T, // 2
	class Compare = std::less<Key>, // 3
	class Allocator = std::allocator<std::pair<const Key, T> > // 4
> class map;
```

# STD::UNORDERED_SET
```
template< class Key, // 1
	class Hash = std::hash<Key>, // 2
	class KeyEqual = std::equal_to<Key>, // 3
	class Allocator = std::allocator<Key> // 4
> class unordered_set;
```

# STD::UNORDERED_MAP
```
template< class Key, // 1
	class T, // 2
	class Hash = std::hash<Key>, // 3
	class KeyEqual = std::equal_to<Key>, // 4
	class Allocator = std::allocator< std::pair<const Key, T> > // 5
> class unordered_map;
```

# STD::QUEUE
```
template<
	class T,
	class Container = std::deque<T>
> class queue;
```

# STD::PRIORITY_QUEUE
```
template<
	class T,
	class Container = std::vector<T>,
	class Compare = std::less<typename Container::value_type>
> class priority_queue;
```

# Какой ЧЕТВЁРТЫЙ параметр в шаблоне есть у std::map ?
Ответ: Аллокатор

# Какой ТРЕТИЙ параметр в шаблоне есть у std::map ?
Ответ: функция компаратор. Для простых или POD типов (int,char и т.д.) и готовых (std::string) типов можно использовать библиотечные компараторы **std::less<>** или **std::greater<>**
```
std::map< std::string, int, **std::greater<>** > myMap;

myMap["A"] = 90;
myMap["C"] = 9;
myMap["B"] = 99;
// порядок для greater будет C(9), B(99), A(90)
// порядок для less    будет A(90), B(99), C(9)
```
### Что нужно определить в структуре / классе, чтобы класс можно было добавить в ассоциативный контейнер std::map / std::set и т.д. ?

### Что нужно сделать, чтобы использовать структуру / класс, как ключ в ассоциативном контейнере std::map / std::set и т.д. ?

### Что нужно сделать, чтобы использовать структуру / класс, в качестве ключа в ассоциативном контейнере std::map / std::set и т.д. ?
- В структуре/классе нужно переопределить оператор "меньше", либо определить функтор "меньше" и передавать в качестве параметра шаблона (например - третьим параметром для map или вторым для set ).

Сигнатура оператора Меньше для Класса/Структуры (operator <)
``` bool operator<(const Class& _that) const ```

Оператор Меньше для Класса/Структуры с несколькими полями (operator <)
```
bool operator<(const Class& _that) const
{
  bool res = false;
  if (this->param_1 < _that.param_1)
  { res = true; }
  else if(this->param_1 == _that.param_1)
  {
    if (this->param_2 < _that.param_2 )
    { res = true; }
    else if(this->param_2 == _that.param_2)
    {  ...
      {
    	res = this->param_N < _that.param_N;
      }
    }
  }
  return res;
}
```

Функция компаратор для Класса/Структуры с несколькими полями
```
bool compareFunc(const Param& lhs, const Param& rhs)
{
  bool res = false;
  if (lhs.param_1 < rhs.param_1)
  { res = true; }
  else if(lhs.param_1 == rhs.param_1)
  {
    if (lhs.param_2 < rhs.param_2 )
    { res = true; }
    else if(lhs.param_2 == rhs.param_2)
    {  ...
      {
    	res = lhs.param_N < rhs.param_N;
      }
    }
  }
  return res;
}
```

# PATTERN Observer

// https://www.youtube.com/watch?v=ZCd_7r-iHfY
```
struct XY 
{
XY(int x, int y)
	: X(x)
	, Y(y)
  {}
private:
  int X, Y;
};

struct Coord
{
Coord(int x, int y)
	: X(x)
	, Y(y)
{}

  int getX() { return X; }
  int getY() { return Y; }

  bool operator<(const Coord& p) const {
	return ((this->X < p.X) && (this->Y < p.Y));
  }

private:
  int X, Y;
};

struct Point {
Point(int x, int y)
	: X(x)
	, Y(y)
{}

bool operator<(const Point& _that) const
{
  bool res = false;
  if (this->X < _that.X)
  {
  	res = true;
  }
  else if (this->X == _that.X)
  {
  	res = this->Y < _that.Y;
  }
  return res;
}

/*
bool operator<(const Point& p) const {
  return ((this->X < p.X) && (this->Y < p.Y));
}
*/

bool const operator==(const Point &_that) {
  return this->X == _that.X && this->Y == _that.Y;
}

// https://www.linux.org.ru/forum/development/13246470
//..операторы «больше» и «равно» выводятся из одного оператора «меньше»
// Чтобы поддерживать SORT, FIND и т.д. НЕ нужен оператор equality (a == b)
// А НУЖЕН  equivalence  это когда первое значение не меньше второго, ни второе значение не меньше первого => (!(a < b) && !(b < a))

  // оператор БОЛЬШЕ > выражается через меньше => возвращает тру, когда не меньше и не равно => ()
  // оператор РАВНО ==  =>  
  //операторы «больше» и «равно» выводятся из одного оператора «меньше»
  //по теме : определи оператор «меньше», либо определи функтор «меньше» и передай в качестве параметра шаблона.

//  friend bool operator<(const Point& l, const Point& r);

private:
  int X, Y;
};

//bool operator<(const Point& l, const Point& r) {
//  return (l.X < r.X || (l.X == r.X && l.Y < r.Y));
//}

void main()
{
  std::map<XY, int> mXY;
  XY xy1(0, 1);
  //mXY[xy1] = 1; // ERROR WILL NOT COMPILE //error C2678: бинарный "<": не найден оператор, прини

  std::map<Coord, int> mCrd;
  Coord c1(0, 1);
  Coord c2(1, 0);
  Coord c3(1, 1);
  Coord c4(2, 2);
  mCrd[c1] = 1; // size=1 | mCrd<(0, 1)> = 1;
  mCrd[c2] = 2; // size=1 | mCrd<(0, 1)> = 2;
  mCrd[c3] = 3; // size=1 | mCrd<(0, 1)> = 3;
  mCrd[c3] = 4; // size=1 | mCrd<(0, 1)> = 4;

  //if (c1 < c2)  std::cout << "c1 < c2";

  std::map<Point, int> mPnt;
  Point p1(0, 1);
  Point p2(1, 0);
  Point p3(1, 1);
  Point p4(2, 2);
  mPnt[p1] = 1; // size=1 | mCrd<(0, 1)> = 1;
  mPnt[p2] = 2; // size=2 | mCrd<(0, 1)> = 1; mCrd<(1, 0)> = 2;
  mPnt[p3] = 3; // size=3 | mCrd<(0, 1)> = 1; mCrd<(1, 0)> = 2; mCrd<(1, 1)> = 3;
  mPnt[p3] = 4; // size=4 | mCrd<(0, 1)> = 1; mCrd<(1, 0)> = 2; mCrd<(1, 1)> = 3; mCrd<(2, 2)> = 4;

  if (p1 < p2)  std::cout << "p1 < p2";
  //if (p1>p2)  std::cout << "p1 > p2";
  //if (p1==p2) std::cout << "p1 = p2";

  std::cout << mPnt[p1] << mPnt[p2];
}
```

# Различие между С и С++ / между C и C++ / Разница между С и С++ / между C и C++

- Си (структурное прогр.) | В С++ (Объектно ориентированное прогр.)
- Си (отсуствует полиморфизм) | В С++ (Полиморфизм работает через виртуальные функции)
- Си (можно вызывать main() из других фций) | В С++ (нельзя вызывать main() из других фций)
- В С код программы хранится в файлах с раширением ".c" | В С++ код программы хранится в файлах с раширением ".cpp"
- В С для работы с консолью используются scanf() и printf() | В С++  std::cout и std::cin
- default-аргумент (В Си у фций НЕТ аргументов по умолчанию | В С++ у фций МОЖНО использовать аргументы по умолчанию)
- Объекты (В Си нет модификаторов доступа private, protected, есть структуры в которых всё public | В C++ структуры могут содержать в себе все три мдификатора доступа public, private, protected, и в классах тоже)
- Структура / Класс (1. в структуре по умолчанию все поля публичные, в Классе по умолчанию все поля приватные. 2. В структуре по умолчанию публичноре наследование, в классе по умолчанию приватное наследование)
- Наследование (То, что в C++ - наследование, в C - это просто структура в структуре. При программировании в стиле C++ применяются такие красивые и звучные слова, как "класс Circle порожден от класса Point" или "класс Point наследуется от класса Circle и является производным от него". На практике все это словоблудие заключается в том, что структура Point - это первое поле структуры Circle.
При этом реальных усовершенствований два. ПЕРВОЕ - поля Point считаются так же и полями Circle, в результате доступ к ним записывается короче, чем в C. ВТОРОЕ - в обоих структурах можно иметь функции-методы, у которых имена совпадают с точностью до имени структуры. Например, Point::paint и Circle::paint . Следствие - не надо изобретать имена вроде Point_paint и Circle_paint, как это было в C,)
- new и delete (В Си пользоватлись malloc() и free | В С++ пояились new и delete) // Point *p =malloc(sizeof(Point)); и free(p);  Point *p = new Point; delete p;)
- Конструкторы и деструкторы (В Си заведя переменные их надо проинициализировать отдельно, автоматом конструктор не отработает и не поможет нам, и деструктора тоже нет, который очистит за нас содержимое перемиенных и указателей в структуре | В С++ конструктор вызывается автоматически, но инифилизацию можно написать сразу в него, Деструктор есть вызовется атоматом, деинициализирует сам содержимое POD элементарных типов, содержимое указатели праввда надо всё равно осводить в ручную)
- virtual и firiend - Виртуальные функции - (В Си отсуствует механизм виртуальных фций, поэтому механизм работы с таблицами виртуальных функций (через указатель на соотвествующую таблицу для структуры), если необходимо, то придётся реализовать "своими руками". | В C++ есть поддержка ключевого слова virtual и механизма виртуальных функций и работа с указателем на vtable, а так же есть и ключевое слово friend)
- Исключения (В Си НЕТ поддержки механизма обработки исключений | В С++ ЕСТЬ поддержки механизма обработки исключений)
- Перегруженные операторы (В Си НЕТ перегрузки операторов | В С++ ЕСТЬ перегрузка операторов)
- Ссылка (В Си НЕТ ссылок, но есть указатели | В С++ ЕСТЬ и ссылки и указатели)
- inline (В Си НЕТ поддержки inline фций | В С++ ЕСТЬ поддержки inline фций)
- namespace (В Си НЕТ поддержки ключевого слова namespace | В С++ ЕСТЬ поддержка namespace для непересекаемости имён объектов)
- template 

### Различие 1. Объекты

В C++ появились классы и объекты. Технически класс C++ - это тип структуры в C, а объект - переменная такого типа. Разница только в том, что в C++ есть еще модификаторы доступа и полями могут быть не только данные, но и функции (функции-методы).

Функция-метод - это обычная функция C, у которой первый параметр - это указатель на структуру, данные которой она обрабатывает: this. Если сравнить, как выглядят функции-методы в C++ и функции с параметром-указателем на структуру в C, то мы обнаружим, что всего лишь изменилась форма записи. В C++ получается короче, так как this и имя типа во многих случаях писать не обязательно (подразумевается по умолчанию).

Модификаторы доступа - это слова public, private и protected. В C вместо них была внимательность программиста: public - значит с этими полями делаю, что хочу; private - значит к этим полям обращаюсь только с помощью методов этой структуры; protected - то же, что public, но еще можно обращаться из методов унаследованных структур (см. следующий пункт).


### Различие 2. Наследование

То, что в C++ - наследование, в C - это просто структура в структуре. При программировании в стиле C++ применяются такие красивые и звучные слова, как "класс Circle порожден от класса Point" или "класс Point наследуется от класса Circle и является производным от него". На практике все это словоблудие заключается в том, что структура Point - это первое поле структуры Circle.

При этом реальных усовершенствований два. Первое - поля Point считаются так же и полями Circle, в результате доступ к ним записывается короче, чем в C. Второе - в обоих структурах можно иметь функции-методы, у которых имена совпадают с точностью до имени структуры. Например, Point::paint и Circle::paint . Следствие - не надо изобретать имена вроде Point_paint и Circle_paint, как это было в C, а префиксы Point:: и Circle:: в большинстве случаев можно опускать.


### Различие 3. new и delete

В C++ появились две новые операции: new и delete. В первую очередь это - сокращения для распространенных вызовов функций malloc и free:

В стиле C:

Point *p = (Point*) malloc(sizeof(Point));
free(p);
В стиле C++:

Point *p = new Point;
delete p;
При вызове new автоматически вызывается конструктор, а при вызове delete - деструктор (см. следующий пункт). Так что нововведение можно описать формулой: new = malloc + конструктор, delete = free + деструктор.


### Различие 4. Конструкторы и деструкторы

Когда программируешь в стиле C, после того, как завел новую переменную типа структуры, часто надо ее инициализировать и об этом легко забыть. А перед тем как такая структура закончит свое существование, надо ее почистить, если там внутри есть ссылки на какие-то ресурсы. Опять-таки легко забыть.

В C++ появились функции, которые вызываются автоматически после создания переменной структуры (конструкторы) и перед ее уничтожением (деструкторы). Во всех остальных отношениях это - обычные функции, на которые наложен ряд ограничений. Некоторые из этих ограничений ничем не оправданы и мешают: например, конструктор нельзя вызвать напрямую (дестркутор, к счастью, можно). Нельзя вернуть из конструктора или деструктора значение. Что особенно неприятно для конструктора. А деструктору нельзя задать параметры.

### Чего нельзя делать с конструктором ?
- Конструктор нельзя вызвать напрямую.
- Конструктору запрещено возвращать значение

### Чего нельзя делать с деструктором
- Деструктору нельзя задать параметры.
- Деструктору запрещено возвращать значение

### Различие 5. Виртуальные функции

Из всех усовершенствований это вызывает наибольшую "щенячью радость". Как обычно, придуманы и звучно-научно-рекламные названия: "полиморфизм", "виртуальный", "абстрактный". Если отбросить разницу в терминологии, то что получим в сухом остатке? А получим мы очередное сокращение записи. И очень большое сокращение.

При программировании на C часто бывает так, что имеется несколько вариантов одной и той же структуры, для которых есть аналогичные функции. Например, есть структура, описывающая точку (Point) и структура, описывающая окружность (Circle). Для них обоих часто приходится выполнять операцию рисования (point). Так что, если у нас есть блок данных, где перемешаны точки, окружности и прочие графические примитивы, то перед нами стоит задача быстро вызвать для каждого из них свою функцию рисования.

Обычное решение - построить таблицу соответствия "вариант структуры - фукция". Затем берется очередной примитив, определяется его тип, и по таблице вызывается нужная функция. В C этот метод применять довольно нудно из-за того, что во-первых, надо строить эту таблицу, а во-вторых, внутри структур заводить поле, сигнализирующее о том, какого она типа, и следить за тем, чтобы это поле содержало правильное значение.

В C++ всем этим занимается компилятор: достаточно обозначить функцию-метод как virtual, и для всех одноименных функций будет создана таблица и поле типа, за которыми следить будет опять-таки компилятор. Вам останется только пользоваться ими: при попыке вызвать функцию с таким именем, будет вызвана одна из серии одноименных функций в зависимости от типа структуры.

### Различие 6. Исключения

Исключение по своей сути - это просто последовательность goto и return. Основан на обычной C-технологии setjmp/longjmp. try и catch - это setjmp с проверкой. throw - это longjmp. Когда вызывается throw, то проверяется: если он окажется внутри блока try, то выполняется goto на парный блок catch. Если нет, то делается return и ищется catch на уровень выше и так далее.

Наличие в throw/catch параметра ничего принципиально не меняет: и в обычном C можно было заполнить какие-то переменные перед вызовом longjmp и потом их проанализировать.

### Различие 7. Перегруженные операторы

Относитесь к ним как к уродливым функциям и все станет ясно. a + b, где a и b - типа Point это функция от двух аргументов a и b, возвращающая Point:

Point operator+(Point a, Point b)
Написать a+b равносильно вызову такой функции: operator+(a,b). Иногда эта технология удобна, а иногда вносит путаницу.

### Различие 8. Ссылка

Многие программисты изучали C на основе языка Pascal. В Pascal есть возможность возвращать из функции больше одного параметра. Для этого применялось магическое слово "var". В C для того, чтобы сделать то же самое, приходилось расставлять в тексте уйму символов "*".

Разработчики C++ вняли стонам несчастных программистов и ввели слово var. А чтобы все это выглядел ооригинально, "var" они переименовали в "&" и назвали "ссылкой". Это вызвало большую путаницу, так как в C уже были понятия "указатель" (та самая звездочка) и "адрес" (обозначался тем же символом &), а понятие "ссылка" звучит тоже как что-то указующе-адресующее. Вот если бы, не мудрствуя лукаво, добавили слово var…

С одной стороны, использование ссылок намного сокращает текст программы. Но есть и неприятности. Во-первых, вызов функции, в которой параметр является ссылкой, выглядит так же, как вызов с обычным параметром. В результате "на глаз" незаметно, что параметр может измениться. А в C это заметно по значку &. Во-вторых, многочисленные звездочки в C напоминают программисту о том, что каждый раз выполняется дополнительная операция * разыменования указателя. Что побуждает сделать разумную оптимизацию. В C++ эти операции остаются незамеченными.

### Различие 9. Inline, template и default-аргумент

Аргумент по-умолчанию - это то, о чем мечтали программисты C: чтобы иногда не надо было при вызове задавать некоторые параметры, которые в этом случае должны иметь некоторое "обычное" значение.

Желание программистов C контролировать типы параметров в define-ах породило в C++ inline-функции. Такая функция - это обычный define с параметрами, только не надо мучиться с символами "\" и проверяются типы.

Желание узаконить в параметрах define имя типа породило template. Главный плюс template - то, что #define с одинаковыми параметрами породит два одинаковых куска кода. А template в компиляторе скорее всего будет соптимизирован: одинаковые куски кода будут соединены в один. Имеется небольшой контроль типов по сравнению с #define, но не очень удобный.

В то же время template имеют ряд серьезных недостатков. Первый - ужасный, неудобный синтаксис. Чтобы это ощутить, достаточно попробовать. Уж лучше бы разрешили не контролировать типы некоторых параметров inline-функций. Второй недостаток - template остался так же неудобен при работе с отладчиком, как и #define.

Ну и последнее нововведение, продиктованное, видимо, все тем же стремлением избавиться от #define. Это - тип "имя поля" (pointer to member). В C удобно было применять имена полей структур в define. В C++ тоже самое можно сделать с помощью операторов ::*, .* и ->*. Например &Circle::radius - это имя поля radius структуры Circle, а Circle::*radius - соответствующий тип. Такую величину можно передать, как параметр. Фактически речь идет о смещении этого поля относительно начала структуры. Бывает полезно. Примерно так же можно передать адрес функции-метода.


### Различие 10. Язык более высокого уровня?

Когда появились все эти нововведения, то многим стало видно то, что раньше было видно не столь многим. Это нормально: код упростился, а значит те его свойства, которые раньше были замаскированы лишними символами, стали заметны при меньшем напряжении мозгов. В этом и только в этом заключается более высокий уровень C++. Чуть больше абстракции, чуть меньше деталей - можно сосредоточится на более крупных блоках.

Существует мнение, что писать в стиле C на C++ - дурной стиль. Это мнение - всего лишь дань моде. Если в стиле C++ получается короче, лучше, надежнее, то глупо писать в стиле C. Это так, но верно и обратное!


# 0CB29236-0347-4C21-9A39-ADEF236C61E1

----------------------------------------
# С ЭКЗАМЕНА
----------------------------------------

http://neerc.ifmo.ru/wiki/index.php?title=%D0%A1%D1%82%D0%B0%D1%80%D1%8B%D0%B5_%D0%B2%D0%BE%D0%BF%D1%80%D0%BE%D1%81%D1%8B_%D0%BD%D0%B0_%D1%8D%D0%BA%D0%B7%D0%B0%D0%BC%D0%B5%D0%BD_%D0%BF%D0%BE_C%2B%2B

### Q: Что обозначается ключевым словом this? 
указатель на экземпляр класса, для которого вызван текущий метод. В статическом методе this недоступен.

### Q: Объясните принцип полиморфизма. 
Полиморфизм - возможность работать с разными типами одинаковым образом. Статический - перегрузка функций, шаблоны. Динамический - виртуальные функции.

### Q: Статический полиморфизм 
А: Перегрузка функций, шаблоны.

### Q: Динамический полиморфизм
А: Виртуальные функции.

### Q: Каков размер «пустого» объекта? 
А: Ненулевой в зависимости от компилятора (у меня 1 байт, например). Компилятор добавляет **фиктивный член** чтобы вообще можно было как-то хранить его в памяти.



-------------------------
# DC0A21C3-D27E-477E-984C-301076DAC9DB

### Каков размер типа char / размер объекта типа char ? ?
sizeof(char) всегда вернёт **1** !

НО! В стандарте C++ не определено, чем же именно является тип char — он может быть как знаковым, так и беззнаковым.

### Чтобы число умножить на 2 какую побитовую операцию можно применить 
Ответ: ПОБИТОВЫЙ СДВИГ Влево
```
0000 0011 = 3
0000 0110 = 6
```
### Разница между оператором [] и оператором at()
- vector.at(i); Делает попытку обратиться к элементу и  Бросает исключение std::out_of_range  при выходе за пределы вектора 
- operator[] делает попытку считать данные и падает без бросания исключения

### Двоичный поиск / Бинарный поиск / Алгоритм бинарного поиска 
Задача: Есть искомый Элемент и Массив в котором этот элемент нужно найти.
- Решение рекурсивное.
- Особенность: Бинарный поиск можно осуществлять внутри отсортированного массива
- Сложность: O(log(n))

Суть: Сравниваем искомый элемент с элементом стоящим всередине и сраниваем с
соседним слева или справа от него.
Соотвественно отбрасываем ту половину амссива относительно элемента, который 
был больше искомого и продолжаем поиск в другой половине.
Снова выбираем опорный элемент всередине отобранной середины и продолжаем 
поиск до тех пор пока не найдём искомый постоянно ополовинивая зону поиска.

### Дан Связный список - узнать закольцован он или нет ? 

https://en.wikipedia.org/wiki/Cycle_detection

Известно несколько алгоритмов поиска цикла быстро и при малой памяти:
1. Алгоритм «Черепахи и Зайца» Флойда передвигает два указателя с различной скоростью через последовательность значений, пока не получит одинаковые значения
2. Алгоритм Брента, основан на идее экспоненциального поиска[en].

Решение тут: Sample Cycles test for Node in a List

- Если коротко, то надо "пустить" по списку два указателя, один вдвое быстрее другого. Либо список кончится, либо указатели встретятся.
- В каждом элементе указатель next поменять так что бы он указывал на предыдущий. По достижению null, если текущий элемент равен начальному то цикл есть иначе нет.. Что бы востановить первоначальный список, надо снова пройтись и поменять указатели обратно.
- Моё первое решение: Сохранить начало списка, итеративно перебирать элементы списка, начиная с первого и проверять за сколько шагов с начала доступен текущий елемент [C] и тот, который является следующим за текущим [N]. Если N<C — значит елемент с номером N — начало цикла. Итого: память O(1), сложность O(n^2)
- Второе решение: Сохраняем первый и второй елемент списка, и итеративно меняем указатели каждого елемента списка на послеследующий елемент. Делаем так, пока не встретим конец списка, либо попадём в ситуацию, когда послеследующий является текущим — это признак цикла. Свернуть список обратно, имея 1-й и 2-й елементы просто, т.к. они чередуются. Итого: память O(1), сложность O(n)
- есть классический алгоритм, когда по списку пускаем 2 указателя, один за шаг переходит на 1 элемент вперед, 2-й - на 2 элемента вперед. При проходе 2-м проверяем, если он указывает на тот-же объект, что и первый - значит цикл есть.

### Если для этой задачи мы сильно увеличим входные объёмы, то что может произойти ? 
Сложность операций увеличится на ???

### const expression разница в 11 и в 17 стандарте ?
- В 17-той версии внутри const expression можно делать циклы (в 11той этого делать нельзя) - и поэтому в 17-том Задача "SWITCH CASE" со строками она решается рекурсией. (Рекурсия для рантайма это плохо, а для компайл тайма ещё более менее пойдёт.)

### Двоичное дерево поиска

Есть бинарное / Двочиное дерево, если оно сбалансированно - O(log по основанию 2 от n)

### Сбалансированные Двоичное деревья и разница между двоичными сбалансрованными деревьями?

- B-дерево
- AVL-дерево
- RB-дерево / Красно-чёрное дерево

РАЗНИЦА: Разница в том как деревья **балансируются**

### Хэш-таблица Зачем она нужна, в чём её преимущества и недостатки

При формировании этой структуры даннх нам не нужно постояно заботиться в отличие от дерева, о том, чтобы постоянно "держать баланс" при добавлении каждого онового элемента.

Всё же операция добавления элемента в случае когда НЕТ коллизиц равная О(1) против О(log n) для дерева, существенна!

Одновременно с этим если элементов огромное число, то и скорость работы хэш функции встаёт как вопрос, и вопрос хранения всех Хэш-индексов под хэш таблицу .... Тоесть проблемы с памятью мы можем начать испытывать.

В случае коллизий отдельная проблема, из-за формирования Бакетов, со всеми вытекающими проблемами списков, которые формируются на этом.

### Хэш-таблица 2 основных типа организации

https://ru.wikipedia.org/wiki/%D0%A5%D0%B5%D1%88-%D1%82%D0%B0%D0%B1%D0%BB%D0%B8%D1%86%D0%B0

- Хэш-таблица ЗАКРЫТОГО ТИПА (Формирование Бакетов - типа СИПСОК - методом цепочек)
- Хэш-таблица ОТКРЫТОГО ТИПА (Формирование Бакетов - типа пара ключ-значение - методом пробирования)

Различают хэш таблицы по тому как в них решается проблема Коллизий

1. Метод цепочек 
- Формирование Бакетов - Каждая ячейка массива H является указателем на связный СПИСОК (цепочку) пар ключ-значение, соответствующих одному и тому же хеш-значению ключа. Коллизии просто приводят к тому, что появляются цепочки длиной более одного элемента.
2. Открытая адресация 
- В массиве H хранятся сами пары ключ-значение. Алгоритм вставки элемента проверяет ячейки массива H в некотором порядке до тех пор, пока не будет найдена первая свободная ячейка, в которую и будет записан новый элемент. Этот порядок вычисляется на лету, что позволяет сэкономить на памяти для указателей, требующихся в хеш-таблицах с цепочками.
Последовательность, в которой просматриваются ячейки хеш-таблицы, называется последовательностью проб

### Какие типы Индексов вам известны

1. Хэш таблицы 
2. Б деревья
3. Reversed index
4. Clustered index

### FTS - Full Text Search - как организован ? 
Задача: по входному слову нужно найти либо предложение с этим словом либо указать строку с этим словом в тексте.

Решение: Надо найти объект из каких-то критериев. Критерий может быть - конкретное поле и мы это значение будем индексировать в зависимости от уникальности данных и нужно ли упорядоченное чтение или нужно найти кокнретный объект. Для решения мы соотвественно используем хэш-таблицу или двоичное дерево. 

### Reversed index 
Будет фактически множественным индексом на каждую запись. Разобрали текст по словам, указатель на слово в дерево, и далее мы продолжаем строить наше дерево.

### Clustered index (группировка данных)
Например есть двусвязный список, и мы формируем "кластер". Ибо быстрее будет найти вхождение в кластер по индексу по хэш таблице, далее мы проходя по кластеру, в случае если в нём все данные упорядочены. В том случае если перед нами стоит задача сгруппировать данные по какому-то одному признаку, то организация хранения данных совметно имеет место быть. Всё это делается для того, чтобы Уменьшить количество циклов чтения данных.

https://ru.wikipedia.org/wiki/%D0%9F%D0%BE%D0%BB%D0%BD%D0%BE%D1%82%D0%B5%D0%BA%D1%81%D1%82%D0%BE%D0%B2%D1%8B%D0%B9_%D0%BF%D0%BE%D0%B8%D1%81%D0%BA

https://habr.com/ru/post/40218/

Reversed Index - использовал бы двоичное дерево поиска внутри себя

https://stackoverflow.com/questions/3073390/create-and-use-html-full-text-search-index-c/3130822

Indexing
The map word ==> page data structure is known as an inverted index. Its inverted because its often generated from a forward index of page ==> words. Inverted indexes generally come in two flavors: inverted file index, which map words to each document they occur in, and full inverted index, which map words to each position in each document they occur in.

The important decision is what backend to use for the index, some possibilities are, in order of ease of implementation:

SQLite or Berkly DB - both of these are database engines with C++ APIs that integrated into a project without requiring a separate server process. Persistent databases are essentially files, so multiple index sets can be search by just changing the associated file. Using a DBMS as a backend simplifies index creation, updating and searching.
In memory data structure - if your using a inverted file index that is not prohibitively large (memory consumption and time to load), this could be implemented as a std::map<std::string,word_data_class>, using boost::serialization for persistence.
On disk data structure - I've heard of blazingly fast results using memory mapped files for this sort of thing, YMMV. Having an inverted file index would involve having two index files, one representing words with something like struct {char word[n]; unsigned int offset; unsigned int count; };, and the second representing (word, document) tuples with just unsigned ints (words implicit in the file offset). The offset is the file offset for the first document id for the word in the second file, count is the number of document ids associate with that word (number of ids to read from the second file). Searching would then reduce to a binary search through the first file with a pointer into a memory mapped file. The down side is the need to pad/truncate words to get a constant record size.
The procedure for indexing depends on which backend you use. The classic algorithm for generating a inverted file index (detailed here) begins with reading through each document and extending a list of (page id, word) tuples, ignoring duplicate words in each document. After all documents are processed, sort the list by word, then collapsed into (word, (page id1, page id2, ...)).

The mifluz gnu library implements inverted indexes w/ storage, but without document or query parsing. GPL, so may not be a viable option, but will give you an idea of the complexities involved for an inverted index that supports a large number of documents.

### Что такое Фильтр Блума и зачем он нужен ?

https://neerc.ifmo.ru/wiki/index.php?title=%D0%A4%D0%B8%D0%BB%D1%8C%D1%82%D1%80_%D0%91%D0%BB%D1%83%D0%BC%D0%B0

https://habr.com/ru/post/112069/

Это вероятностная структура данных. 

Задача - есть элемент. Вопрос в том лежит этот элемент в множестве или нет ?

Гарантия при ответе такая - Гарантия того, что возможно элемент в множестве находится.

Пример использования: Например, Гуголь использует фильтры Блума в своей BigTable для уменьшения обращений к диску. Оно и не удивительно, ведь по сути, BigTable — это большая очень разреженная многомерная таблица, поэтому большая часть ключей указывает в пустоту. К тому же, данные распиливаются на относительно небольшие блоки-файлы, каждый из которых опрашивается при запросах, хотя может не содержать требуемых данных.

### Виды ввода вывода / Основные методы ввода вывода (I/O)
https://tproger.ru/translations/diversity-of-input-output-io/

- блокирующий
- неблокирующий
- мультиплексированный
- асинхронный

Виды ввода-вывода и примеры функций ввода-вывода Unix:
| -           | Блокирующий |     Неблокирующий           |
| ------------|:-----------:|:---------------------------:|
| Синхронный  | write, read | write, read + poll / select |
| Асинхронный | -           | aio_write, aio_read         |

(Он тоже блокирует поток на операциях ввода-вывода, но вместо того, чтобы производить блокировку по очереди, вы можете запланировать все операции ввода-вывода, которые вам нужно сделать, и блокировать их все. Операционная система разбудит поток, когда какая-нибудь из операций завершится.)

(форма неблокирующей обработки ввода/вывода)

### Каждый thread под отдельный read/recieve ? Чепм это плохо, в линуксе в windows ?
Ответ: Переключение контекста из user mode в kernel mode удут сильно затратными

### Проблема 1: Какие механизмы асинхронного ввода вывода ?

https://ru.wikipedia.org/wiki/%D0%90%D1%81%D0%B8%D0%BD%D1%85%D1%80%D0%BE%D0%BD%D0%BD%D1%8B%D0%B9_%D0%B2%D0%B2%D0%BE%D0%B4-%D0%B2%D1%8B%D0%B2%D0%BE%D0%B4

- Функции обратного вызова
- Корутины
- Порты (очереди) завершения
- Каналы ввода/вывода

ПРИМЕРЫ:
- Функции обратного вызова - Доступны в FreeBSD, OS X, VMS и Windows. - Потенциальная проблема в том, что глубина стека может расти неконтролируемо, поэтому чрезвычайно ключевая вещь, необходимо, только тогда запланировать другой ввод/вывод когда завершён предыдущий. Если это должно быть удовлетворено немедленно, первоначальный callback не выполняет «раскрутку» стека до того, как следующий вызывается.
- Корутины (сопрограммы) позволяют писать асинхронные программы в синхронном стиле. Примеры: async / await. Доступен в C#, Python, EcmaScript. Генераторы. Доступны в PHP, Python, Ruby с помощью ключевого слова yield. Также есть множество библиотек для создания сопрограмм (libcoro[3], Boost Coroutine)
- Порты (очереди) завершения - Доступен в Microsoft Windows, Solaris и DNIX. Запросы ввода/вывода выдаются асинхронно, но уведомления о выполнении предоставляются через механизм синхронизационной очереди в порядке их завершения.
- Каналы ввода/вывода - Доступные в мейнфреймах IBM, Groupe Bull и Unisys каналы ввода-вывода предназначены для максимального

### Проблема 1: Чем плохо использовать многопоточность в приложении, когда к этому нет явных предпосылок ? 
- (+) Раскладывание 1 процесс на несколько ядер.
- (-) Операций в многопоточном приложении у нас чаще всего следующие: 

1. **ПЕРЕКЛЮЧЕНИЕ КОНТЕКСТА** 
- в Windows: ПЕРЕКЛЮЧЕНИЕ КОНТЕКСТА из USER mode в KERNEL mode и обратно создание/уничтожение в ядре Windows.
- в Линукс: **SETCONTEXT()** аналогичные по сути операции в Линуксе соотвественно.

2. **L1 L2 L3** - Кэш память.
- Т.е. ранее мы УПИРАЛИСЬ в (Интерфейс Диск-Память) проблему **скорость работы памяти Жёского диска** куда чаще. 

А сейчас мы стали упираться (Интерфейс КЭШ-Память) в проблему **как бы ещё ускорить операции с уже и так быстрой КЭШ ПАМЯТЬЮ**. 
Причина в том, что память на диске медленная, и поэтому нам выгоднее держать код и данные в Кэше. Допуская минимум ветвлений. Именно поэтому правило хорошего тона в С/С++ как можно меньше использовать оператор условного перехода if() {} if () и т.д. А использовать именно результаты булевых операций.

### Фреймы стека. Кадр стека. Организация фреймов на стеке ?

https://www.gunsmoker.ru/2015/02/stack-frames.html

Семейство процессоров x86 (как 32-битных, так и 64-битных) 
использует аппаратный стек процессора для хранения последовательности 
вызова подпрограмм (т.е. процедур, функций и методов) по мере их выполнения. 
Другие процессоры могут использовать для этого и другие способы. 
Стековые фреймы (также иногда называемые "фреймами вызовов", 
"записями активации" или "фреймами активации") представляют собой структуры 
в стеке процессора, которые содержат информацию о состоянии подпрограммы. 
Каждый стековый фрейм соответствует вызову какой-то подпрограммы, которая 
была вызвана, но ещё не завершилась. Например, если подпрограмма DrawLine 
была вызвана подпрограммой DrawSquare и пока ещё выполняется, то верхняя 
часть стека может выглядеть примерно так:

### ФИЧА в С++, которой НЕТУ в Си? Что есть в С++ и чего нету в обычном Си?
Ответ: Обработка исключений try catch. Нам нужно пройтись по "Кадрам" стека, найти call (места возврата) для того, чтобы раск5рутка стека началась. В качестве альтернативы иногда используют libandwint
Если заниматься РеалТайм, то лучше их не использовать.

### Синхронный ввод-вывод
Указываем, что дескриптор неблокирующий и есть:
- 2 позиксовых - механизма (??? не лучшие) 
- есть Линуксовый механизм (сильно хороший) 
- есть BSD-шный механизм.
- windows - WSA-Socket

В классическом WSA-Socket-е механизм таков:
- **БЛОКИРУЮЩИЙ ВВОД/ВЫВОД** - Мы ждём accept на **listen()** (read / write). Соотвественно блокируем, в отдельном потоке запускаем и ждём. Сокет открыт, он listen (слушает) и именно это и есть БЛОКИРУЮЩИЙ ВВОД/ВЫВОД. (В Windows мы для этого используем HANDLE окна и wndProck())
- **НЕБЛОКИРУЮЩИЙ ВВОД/ВЫВОД** - В Линуксе же есть **select()**, **poll()**, **epoll()**
- в BSD есть **kqueue** "кейкью". А так же в линуксе используют **Дескрипторы** при работе ввода-вывода.
- **timerfd** http://ru.manpages.org/timerfd_create/2 / http://man7.org/linux/man-pages/man2/timerfd_create.2.html  
- **eventfd** http://ru.manpages.org/eventfd/2 /  http://man7.org/linux/man-pages/man2/eventfd.2.html

### Epoll (extended poll) 
API мультиплексированного ввода-вывода, предоставляемого Linux для приложений. API позволяет приложениям осуществлять мониторинг нескольких открытых файловых дескрипторов (которые могут быть файлами, устройствами или сокетами, в том числе сетевыми), для того, чтобы узнать, готово ли устройство для продолжения ввода (вывода). Epoll планировался как более эффективная замена вызовам select() и poll(), определёнными в POSIX. Epoll может предоставить более эффективный механизм для приложений, обрабатывающих большое количество одновременно открытых соединений — со сложностью O(1) в отличие от стандартного механизма, обладающего сложностью O(n). Epoll аналогичен системе Kqueue из FreeBSD и также представляет собой объект ядра, предоставляемый для работы в пространстве пользователя в виде файлового дескриптора.

- Проблема select/poll
Для использования select или poll приложение должно передать в ядро полный список всех файловых дескрипторов, в которых оно ожидает появление данных; а ядро, в свою очередь, должно для каждого из переданных элементов проверить состояние дескрипторов и сформировать структуру, описывающую состояние каждого переданного дескриптора. Такой подход не создаст много проблем в условиях десятков или сотен дескрипторов. Тем не менее, производительность в больших сетях заметно падает.

- Описание epoll API
Linux предоставляет следующие вызовы в рамках API:

- epoll_create() — создаёт структуру данных (epoll instance), с которой в дальнейшем идёт работа. Структура одна для всех файловых дескрипторов, за которыми идёт наблюдение. Функция возвращает файловый дескриптор, который в дальнейшем передаётся во все остальные вызовы epoll API.
- epoll_ctl() — используется для управления epoll instance, в частности, позволяет выполнять операции EPOLL_CTL_ADD (добавление файлового дескриптора к наблюдению), EPOLL_CTL_DEL (удаление файлового дескриптора из наблюдения), EPOLL_CTL_MOD (изменение параметров наблюдения), EPOLL_CTL_DISABLE (добавлена в Linux 3.7) — для безопасного отключения наблюдения за файловым дескриптором в многопоточных приложениях
- epoll_wait() — возвращает количество (один или более) файловых дескрипторов из списка наблюдения, у которых поменялось состояние (которые готовы к вводу-выводу).

События, за которыми можно наблюдать с помощью epoll:
- EPOLLIN — новые данные (для чтения) в файловом дескрипторе
- EPOLLOUT — файловый дескриптор готов продолжить принимать данные (для записи)
- EPOLLERR — в файловом дескрипторе произошла ошибка
- EPOLLHUP — закрытие файлового дескриптора

### **kqueue** 
масштабируемый интерфейс уведомления о событиях. Впервые появился во FreeBSD 4.1[1], также поддерживается в NetBSD, OpenBSD, DragonflyBSD, и Mac OS X. Kqueue позволяет пользователям оперативно получать уведомления о событиях в системе.
kqueue предоставляет механизм уведомления процесса о некоторых событиях, произошедших в системе. Впервые этот интерфейс появился в FreeBSD 4.1 и на данный момент присутствует практически во всех BSD системах. (в Linux есть его аналог - epoll()).

Как это работает: процесс с помощью вызова kqueue() получает дескриптор очереди сообщений ядра (в случае ошибки процесс завершается):
```
if ( ( kq = kqueue() ) == -1 )
  exit(1);
```
затем с помощью kevent() устанавливает, сообщения о каких именно событиях он желает получать. Например, мы хотим знать когда файл
(которому соотвествкует дескриптор fd) изменится.

O K-QUEUE - https://www.opennet.ru/base/dev/kqueue_overview.txt.html

O select / poll / epoll - https://habr.com/ru/company/infopulse/blog/415259/

### SSE и c++ / SSE в c++ / SIMD

https://habr.com/ru/post/440566/

Есть класс задач, которые нельзя ускорить за счёт оптимизации алгоритмов, а ускорить надо. 
В этой практически тупиковой ситуации к нам на помощь приходят разработчики процессоров, 
которые сделали команды, позволяющие выполнять операции на большим количеством данных за одну операцию. 
В случае x86 процессоров это инструкции сделанные в расширениях MMX, SSE, SSE2, SSE3, SSE4, SSE4.1, SSE4.2, AVX, AVX2, AVX512.

- Что такое SIMD
SIMD (Single Instruction, Multiple Data) — одиночный поток команд, множественный поток данных. 
В x86 совместимых процессорах эти команды были реализованы в нескольких поколениях SSE и AVX расширениях процессора. 
Команд довольно много, полный список от Intel можно посмотреть на странице software.intel.com/sites/landingpage/IntrinsicsGuide. 
В десктопных процессорах AVX расширения недоступны, поэтому сосредоточимся на SSE.

Для работы с SIMD в C/C++ в код надо добавить
```
#include <x86intrin.h>
```
Итог: На сайте **INTEL**, выбрав нужное расширение процессора SSE. Мы можем получить на сайте доступ к списку особых функций по категориям:
- категория «Compare» (семейство функций __m128i _mm_cmpeq_epi* (__m128i a, __m128i b) умеют проводить сравнение сразу с несколькими элементами за один цикл)
- категории «Set» и «Load».
- Быстрый подсчёта количества бит в числе есть функции _popcnt32 и _popcnt64
- _mm_movemask_epi8 - работа с масками для массива и Т.Д.

В итоге использование именно этих функций через Inetl-овский процессор может дать прирост производительности в х2 и до х5 раз.

### Чем отличается fiber от thread-а отличается (в линуксе это называется SET CONTEXT)?

### С++11 Как можно сделать switch по строке / Использовать строку в switch / строки внутри "case" ?
Ответ: Используя CONST EXPRESSION, начиная с C++11

https://habr.com/ru/post/166201/

ОГРАНИЧЕНИЕ В CASE (строки в 9 символов максимум)
```
SWITCH (month)
{
   CASE("february"):
       days = "28 or 29";
       break;
   DEFAULT:
       days = "?";
       break;
}	   
```
Но в C++11, к счастью, появились более понятные constexpr-функции, значения которых также могут вычисляться компилятором.

1. Нам нужно написать constexpr-функцию, которая бы оперировала числовыми кодами char-символов.

2. Она проверяет, содержит ли известная на стадии компиляции строка только символы из диапазона 0-127, и возвращает false в случае нахождения «запретного» символа. 
```
constexpr bool str_is_correct(const char* const str)
{
	return (static_cast<signed char>(*str) > 0) ? str_is_correct(str + 1) : (*str ? false : true);
}
```

3. функцию из пункта 2 вставим в макрос
```
#define CASE(str)  static_assert(str_is_correct(str), "CASE string contains wrong characters");\
   case str_hash(...)
```
4. Берём хэш-функцию, не имеющую коллизий вообще.

Так как уже установлено, что все символы строки расположены в диапазоне 0-127, то функция будет иметь вид:

Сумма от i=1 до len ( от str[i] * (128^(len-i)) )

Её реализация такова:
```
   typedef unsigned char uchar;
   typedef unsigned long long ullong;

   constexpr ullong str_hash(const char* const str, const uchar current_len)
   {
      return *str ? (raise_128_to(current_len - 1) * static_cast<uchar>(*str)
      + str_hash(str + 1, current_len - 1)) : 0;
   }
```   

При какой же максимальной длине строки эта функция не будет иметь коллизий? Очевидно, лишь тогда, когда полученное ею значение всегда уместится в диапазоне типа unsigned long long (также окончательно введённого в C++11), то есть если оно не превышает 264-1.

5. Вот для подсчёта хэша в макросе SWITCH нам придётся сделать отдельную функцию
```
#define SWITCH(str)  switch(str_hash_for_switch(str))
const ullong N_HASH = static_cast<ullong>(-1);  // по аналогии с std::string::npos
	
inline ullong str_hash_for_switch(const char* const str)
{
	return (str_is_correct(str) && (str_len(str) <= MAX_LEN)) ? str_hash(str, str_len(str)) : N_HASH;
}
```


1

