/* 
 File:   TicTacController.cpp
 
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

#include "TicTacController.h"

TicTacController::TicTacController() {
}

TicTacController::TicTacController(TicTacModel &ticTacModel, TicTacView &ticTacView) {
    this->ticTacModel = ticTacModel;
    this->ticTacView = ticTacView;
}

TicTacController::~TicTacController() {
}

/**
 * Asks to determine who plays first
 * and with which sign (X or O)
 */
void TicTacController::initializePlayer() {
    askWhoStarts();
    askPlayersSign();
}

/**
 * Asks user to find who is starting 
 * This function will be developed in future. 
 * Now computer starts first
 */
void TicTacController::askWhoStarts() {
    // TODO ask user about the starting body    
    ticTacModel.setStarter(CONFIRM);
}

/**
 * Asks user to find who is starting 
 * This function will be developed in future. 
 * Now computer starts sign is "X"
 */
void TicTacController::askPlayersSign() {
    // TODO  ask user about sign of of players
    ticTacModel.setHumanID('O');
    ticTacModel.setComputerID('X');
}

/**
 * Main process of the game
 */
bool TicTacController::startGame() {
    char winner = NOBODY;
    string winnerName;
    char boardLayoutCopy[MAXCELLCOUNT];

    while (winner == NOBODY) {
        if (ticTacModel.getNextMove() == HUMAN){   
            //Human turn
            askPlayerToMove();
            ticTacModel.changeNextMove();
        } else {
            askComputerToMove();
            ticTacModel.changeNextMove();
        }
        winner = ticTacModel.checkWinner();
    }

    ticTacView.renderGameScreen(ticTacModel.getBoardStatusCopy(boardLayoutCopy), false);

    if(winner == TIE){
        ticTacView.announceNoWinner();
        return askToPlayAgain();
    }
    
    if(winner == ticTacModel.getComputerID())
        ticTacView.announceWinner(ticTacModel.getComputerID(), 
                                    ticTacModel.getComputerPlayerName());
    else
        ticTacView.announceWinner(ticTacModel.getHumanID(), 
                                    ticTacModel.getHumanPlayerName());
    
    return askToPlayAgain();
}

/**
 * Asks player about restarting the game
 * @return 
 */
bool TicTacController::askToPlayAgain(){
    string playerAnswer = "";

    ticTacView.prepareAskToPlayAgainQuestion();
    ticTacModel.setStarter((char)toupper(playerAnswer[0]));
    
    cin >> playerAnswer;
    
    return ((char)toupper(playerAnswer[0]) == CONFIRM);
}

/**
 * Render screen and prepare question and find user answer
 */
void TicTacController::askPlayerToMove() {
    int playerNextCellIndex = 0;
    char boardLayoutCopy[MAXCELLCOUNT];
    
    do {
        ticTacView.renderGameScreen(ticTacModel.getBoardStatusCopy(boardLayoutCopy));
        ticTacView.preparePlayerMoveQuestion(ticTacModel.getFreeBoardCellsNumbers());

        playerNextCellIndex = ticTacView.getUserNextMove();

    } while (!ticTacModel.isPlayerMoveValid(playerNextCellIndex));

    ticTacModel.recordMove(playerNextCellIndex, humanID);
}

/**
 * Calls Model to run main logic of computer moves. 
 */
void TicTacController::askComputerToMove() {
    ticTacModel.doComputerMove();
}

/**
 * TODO : For future use only
 */
void TicTacController::doRandomMove(){
    int playerCellIndex;
    srand(time(0));
    do {
        playerCellIndex = rand() % 10;
    } while (!ticTacModel.isPlayerMoveValid(playerCellIndex));
    ticTacModel.recordMove(playerCellIndex, computerID);
}
