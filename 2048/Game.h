//
//  Game.h
//  2048
//
//  Created by Tom Hu on 12/21/14.
//  Copyright (c) 2014 Tom Hu. All rights reserved.
//
#pragma once

#ifndef ___048__Game__
#define ___048__Game__

#include "stdafx.h"

typedef enum {
    MoveCommandUp,
    MoveCommandDown,
    MoveCommandLeft,
    MoveCommandRight
} MoveCommand;

class Game {
private:
    int score;

public:
	int getScore();

private:
    bool over;
    bool won;
    
    int numberOfEmptyCells;
    
    static const int size = 4;
    static const int numberOfStartCells = 2;

	long chessboardWidth;
    
    int chessboard[4][4];

public:
	RECT chessboardRect;
	RECT scoreLabelRect;

private:
	RECT topBarRect;
    
private:
    void initChessboard();
    void initStartCells();
    void addRandomCell();
    int getRandomAvailableCell() const;
    
    void clearCell(const int &row, const int &col);
	void getCellColor(const int &value, LPCOLORREF fontColor, LPCOLORREF cellBgColor) const;

#pragma mark Can Move?
public:
	bool canMove() const;
    bool canMove(const MoveCommand cmd) const;

private:
    bool canMoveUp() const;
    bool canMoveDown() const;
    bool canMoveLeft() const;
    bool canMoveRight() const;
    
#pragma mark -
#pragma mark Constructor & Destructor
public:
    Game();
	Game(RECT clientRect);
    ~Game();
    
#pragma mark -
#pragma mark Actions
public:
    void doMove(const MoveCommand cmd);

private:
    void doUp();
    void doDown();
    void doLeft();
    void doRight();

public:
    void restart();
	void stop();
    
#pragma mark -
#pragma mark View
public:
    void print() const;
    void printHelpInfo() const;
	void draw(HDC hdc) const;

private:
	void drawTopBar(HDC hdc) const;
	void drawLogoText(HDC hdc) const;
	void drawScoreLabel(HDC hdc) const;
	void drawChessboard(HDC hdc) const;
	void drawCell(HDC hdc, const int &row, const int &col, const int &value) const;

public:
	void setRect(const RECT clientRect);
    
#pragma mark -
#pragma mark Is over or won or terminated?
public:
    bool isOver() const;
    bool isWon() const;
    bool isTerminated() const;
};

#endif /* defined(___048__Game__) */
