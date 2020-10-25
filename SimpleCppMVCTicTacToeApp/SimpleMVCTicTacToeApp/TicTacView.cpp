/* 
 File:   TicTacView.cpp
 
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

#include "TicTacView.h"
#include <conio.h>
#include <iostream>
#include <string>

using namespace std;


TicTacView::TicTacView() {
}

TicTacView::~TicTacView() {
}

/**
 * Draws a line of board
 * @param boardStatus
 * @param lastRowNum
 */
void TicTacView::drawBoardLine(char * boardStatus, int lastRowNum) 
{
    if (((lastRowNum + 1) % ROWCOUNT) == 0)
		return;

	std::string noDataLine = "";

    for (int j = 0; j < COLUMNCOUNT; j++)
	{
        noDataLine += "---";
        if (((j + 1) % COLUMNCOUNT) != 0)
            noDataLine += "|";
    }

    std::cout << noDataLine.c_str() << "\n";
}

/**
 *
 * @param dataArray
 * @param lastRowNum
 */
void TicTacView::drawBoardCellRow(char * dataArray, int lastRowNum) {

    std::string boardRow = "";
    std::string s;

    for (int j = 0; j < COLUMNCOUNT; j++) {
        s = dataArray[lastRowNum * 3 + j];
        boardRow += " " + s + " ";
        if (((j + 1) % COLUMNCOUNT) != 0)
            boardRow += "|";
    }

    std::cout << boardRow.c_str() << "\n";
}

void TicTacView::drawBoard(char * boardDataArray) {
    for (int i = 0; i < ROWCOUNT; i++) {
        drawBoardCellRow(boardDataArray, i);
        drawBoardLine(boardDataArray, i);
    }
}

/**
 *
 * @param boardStatus
 */
void TicTacView::showGameBoard(char * boardStatus) {
    cout << "============\n";
    cout << " Computer: X \n";
    cout << "  Player: O \n";
    cout << "============\n";
    cout << " Game Board \n";
    cout << "============\n";
    drawBoard(boardStatus);
    cout << "\n\n";
}

void TicTacView::drawGameBoardHint(char * boardStatus) {
    char boardHintData[9];
    int dataArraySize = ROWCOUNT * COLUMNCOUNT;

    cout << "\n\n";
    cout << "===============\n";
    cout << "Game Board Map \n";
    cout << "===============\n";

    for (int i = 0; i < dataArraySize; i++) {
		if (boardStatus[i] == ' ')
		{
			std::string s = std::to_string(i + 1);
			boardHintData[i] = s[0];
		}
        else
            boardHintData[i] = ' ';
    }

    drawBoard(boardHintData);
    cout << "\n\n";
}

/**
 * Shows game on screen
 * @param char * boardStatus a copy of boardStatus from Model
 * @param bool drawHint true means hint should be drawn
 */
void TicTacView::renderGameScreen(char * boardStatus, bool drawHint) {
    system("cls"); /** System call */

    showGameBoard(boardStatus);
    if (drawHint)
        drawGameBoardHint(boardStatus);

}

void TicTacView::preparePlayerMoveQuestion(string freeCellNumber) {
    cout << "Select a cell number " << freeCellNumber.c_str() <<" for your next move -> ";
}

void TicTacView::prepareAskToPlayAgainQuestion() {
    cout << "Do you want to play again? (Y or N) ";
}


/**
 * Gets user next move by asking cell number
 * @return int selected CellIndex, which is equal to playerNextCellNumber - 1
 */
int TicTacView::getUserNextMove(){
    int playerNextCellNumber;
    cin >> playerNextCellNumber;
    return playerNextCellNumber - 1;
}

void TicTacView::announceWinner(char winnerID, string winnerName) {
    char noUseChar;
    cout << "\n*******************************\n";
    cout << "*** " << winnerName.c_str() << " (" << winnerID << ") is winner! ***\n";
    cout << "*******************************\n\n";
}

void TicTacView::announceNoWinner() {
    char noUseChar;
    cout << "\n ------------------------\n";
    cout << " Tie! No one is winner!\n";
    cout << " ------------------------\n";
}
