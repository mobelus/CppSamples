// opera_autoupdate.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <time.h>
#include <windows.h>
#include <conio.h>

using namespace std;

struct Line {
    int x0;
    int y0;
    int offset;
    int tailLength;
    WCHAR lastChar;
};

char *Str1 = "Knock-knock, Neo";
char *Str2 = "The Opera autoupdate has you 0_o";
char *Str3 = "Your mind has been updated. Don't tell anyone. Or else.";


int _tmain(int argc, _TCHAR* argv[])
{
    HANDLE hConsole_c = GetStdHandle(STD_OUTPUT_HANDLE);

    system("cls");
    srand( time(0) );

#if 0

    SetConsoleTitle(L"...");
    SetConsoleTextAttribute(hConsole_c, 10);

    cout << "> ";
    Sleep(1000);

    for (int i = 0; i < strlen(Str1); i++) {
        cout << Str1[i];
        Sleep( 33 + rand() % 300 );
    }

    cout << endl;
    Sleep(300);

    cout << "> " << endl;
    cout << "> " << endl;
    cout << "> ";

    Sleep(500);

    for (int i = 0; i < strlen(Str2); i++) {
        cout << Str2[i];
        Sleep( 33 + rand() % 300 );
    }

    Sleep(300);

    system("cls");

#endif

#if 1

    SetConsoleActiveScreenBuffer(hConsole_c);
    DWORD len = 1;
    DWORD dwBytesWritten = 0;

    int NUM = 66;
    vector<Line> vec;
    WCHAR str[2];

    for (int i = 0; i < NUM; i++) {

        Line l;

        l.offset = 0;
        l.x0 = rand() % 80;
        l.y0 = rand() % 20 + 1;
        l.tailLength = rand() % 15 + 3;

        vec.push_back( l );
    }

    for (int i = 0; i < 1000; i++) {

        for (int line = 0; line < NUM; line++) {

            Line *L = &vec[line];

            COORD pos = { L->x0, L->y0 + L->offset };

            // все символы, кроме ведущего, отрисовываем заново темным цветом
            if( L->offset >= 0 ) {

                if ((L->y0 + L->offset) < 24) {
                    SetConsoleCursorPosition(hConsole_c, pos);
                    SetConsoleTextAttribute(hConsole_c, 2);
                    str[0] = L->lastChar;
                    WriteConsole(hConsole_c, str, 1, &dwBytesWritten, NULL);
                }
            }

            // затираем хвосты
            if( L->offset >= L->tailLength ) {

                // затираем только в пределах видимой области
                if ((pos.Y - L->tailLength) < 24) {

					COORD crd{ pos.X, pos.Y - L->tailLength };
                    SetConsoleCursorPosition(hConsole_c, crd);
                    SetConsoleTextAttribute(hConsole_c, 0);
                    str[0] = ' ';
                    WriteConsole(hConsole_c, str, 1, &dwBytesWritten, NULL);
                }
            }

            L->offset++;
            pos.Y++;

            // по достижении дна экрана переносим строку на новое место
            if( (L->y0 + L->offset) >= 24 ) {

                if ((L->y0 + L->offset) < 40)
                    continue;

                str[0] = L->lastChar;

                Line l;

                l.offset = 0;
                l.x0 = rand() % 80;
                l.y0 = rand() % 20 + 1;
                l.tailLength = rand() % 15 + 3;

                *L = l;

				COORD newPos = { L->x0, L->y0 };
                pos = newPos;
            }



            // рисуем строку на экран
            str[0] = rand() % 256;

            if( !rand() % 5 )
                str[0] = ' ';

            L->lastChar = str[0];
            str[1] = L'\0';

            SetConsoleTextAttribute(hConsole_c, 10);

            SetConsoleCursorPosition(hConsole_c, pos);

            WriteConsole(hConsole_c, str, 1, &dwBytesWritten, NULL);
        }

        Sleep(50);
    }

    SetConsoleCursorPosition(hConsole_c, {0, 0});
    cout << "> ";

    for (int i = 0; i < strlen(Str3); i++) {
        cout << Str3[i];
        Sleep( 33 + rand() % 200 );
    }

    cout << endl;
    cout << "> ";

    CloseHandle(hConsole_c);

    Sleep(1000);

    //_getch();

    return 0;

#else

    HANDLE hConsole;
    int k;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // you can loop k higher to see more color choices
    for(k = 1; k < 255; k++)
    {
        // pick the colorattribute k you want
        SetConsoleTextAttribute(hConsole, k);
        cout << k << " I want to be nice today!" << endl;
        Sleep(1);
    }

    _getch();

#endif

    return 0;
}
