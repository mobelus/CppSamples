/* 
 File:   TicTacView.h
 
 Copyright (c) 2020-Present Reza Saffarpour

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
 Licensed under the MIT License, you may not use this file except in compliance 
 with the License. You may obtain a copy of the License at

      https://mit-license.org/
*/

#ifndef TICTACVIEW_H
#define TICTACVIEW_H

#include <iostream>
using namespace std;

#define ROWCOUNT 3
#define COLUMNCOUNT 3

/**
 * View class
 */
class TicTacView {
public:
    TicTacView();
    virtual ~TicTacView();
    
    void renderGameScreen(char * boardStatus, bool drawHint = true);
    void prepareStartingBodyQuestion();
    void preparePlayerMoveQuestion(string freeCellNumber);
    void prepareAskToPlayAgainQuestion();
    
    void announceWinner(char winnerID, string winnerName);
    void announceNoWinner();

    int getUserNextMove();
    
private:
    void showBoardMap(char * boardStatus);
    void drawBoardLine(char * boardStatus, int lastRowNum);
    void drawBoardCellRow(char * dataArray, int lastRowNum);
    void showGameBoard(char * boardStatus);
    void drawGameBoardHint(char * boardStatus);
    void drawBoard(char * boardDataArray); 
        
};

#endif /* TICTACVIEW_H */

