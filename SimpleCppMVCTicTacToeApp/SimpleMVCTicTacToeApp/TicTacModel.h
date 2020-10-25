/* 
 File:   TicTacModel.h
 
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

#ifndef TICTACMODEL_H
#define TICTACMODEL_H

#include <ctime>
#include <iostream>
using namespace std;

#define MAXCELLCOUNT 9
#define SIDECELLCOUNT 3
#define NOTVALIDINDEX -1
#define CORNERCOUNT 4
#define STARTCELLINDEX 0
#define ENDCELLINDEX MAXCELLCOUNT - 1
#define CENTER 4
#define NOTSTARTED -1

#define NODATACHAR ' '
#define NOBODY 'N'
#define TIE 'T'
#define CONFIRM 'Y'

#define COMPUTER true
#define HUMAN false

#define COMPUTERPLAYERNAME "Computer"
#define HUMANPLAYERNAME "Human player"

struct cornerEdgeNeighbor{
    int usedCornerindex;
    int nearEdgeIndex;
    int cornerIndexForNextUse;
};

/**
 * Model Class
 */
class TicTacModel {
public:
    TicTacModel();

    virtual ~TicTacModel();
    char * getBoardStatusCopy(char * boardLayoutCopy);
    bool isPlayerMoveValid(int cellNumber);

    bool isFull();
    char getBoardStatus(int index);
    void recordMove(int cellIndex, char playerID);
    char checkWinner();
    void startGame();
    
    void setStarter(char userStarter);
    bool getNextMove();
    void changeNextMove();
    
    void doComputerMove();
    void setHumanID(char playerID);
    void setComputerID(char playerID);
    
    char getHumanID();
    char getComputerID();
    
    string getComputerPlayerName();
    string getHumanPlayerName();
    
    string getFreeBoardCellsNumbers();
    
private:
    char boardLayout[MAXCELLCOUNT] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
    int cornerList[CORNERCOUNT] = {0, 2, 6, 8};
    cornerEdgeNeighbor neighbors[CORNERCOUNT * 2] = {
    {0, 1, 6}, 
    {0, 3, 2}, 
    {2, 1, 8}, 
    {2, 5, 0}, 
    {6, 3, 8}, 
    {6, 7, 0}, 
    {8, 5, 6}, 
    {8, 7, 2}};
    
    int moveSequence[9]; /// moveSequence is list of players movements
    int lastMoveIndex = -1;
    int nextCornerIndex = NOTVALIDINDEX;
    
    bool nextMove;
    bool startingPlayer;
    bool humanUsedNearEdge;
    
    char humanID;
    char computerID;
    
    bool isCellIndexValid(int index);
    bool isCellEmpty(int cellNumber);
        
    bool isCorner(int cellNumber);
    bool isNearEdge(int cornerCellIndex, int unknownIndex);
    
    int findRandomCorner();
    int findOppositeCorner(int cellIndex);
    
    int findWinnerCellIndex(char playerID);
    int findWinnerCell(int start, int increment, char playerID);
    
    void setNextCornerIndex(int nextCornerIndex);
    int getNextCornerIndex(void);
    
    int findAFreeCornerIndex(void);
};

#endif /* TICTACMODEL_H */

