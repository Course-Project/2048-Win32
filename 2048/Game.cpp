//
//  Game.cpp
//  2048
//
//  Created by Tom Hu on 12/21/14.
//  Copyright (c) 2014 Tom Hu. All rights reserved.
//

#include "stdafx.h"

#include <iostream>
#include <cstdio>
#include <ctime>

int Game::getScore() {
	return this->score;
}

void Game::initChessboard() {
    this->score = 0;
    this->over = false;
    this->won = false;
    this->numberOfEmptyCells = 16;
    
    // Init chessboard
    memset(this->chessboard, 0, 4 * 4 * sizeof(int));
    
    // Add the initial cells
    this->initStartCells();
    
    // Print
    this->print();
}

void Game::initStartCells() {
    for (int i = 0; i < this->numberOfStartCells; i++) {
        this->addRandomCell();
    }
}

void Game::addRandomCell() {
    int random = rand();
    int value = ((double)random / RAND_MAX) < 0.9 ? 2: 4;
    int index = this->getRandomAvailableCell();
    this->chessboard[index / this->size][index % this->size] = value;
    this->numberOfEmptyCells--;
}

int Game::getRandomAvailableCell() const {
    while (true) {
        int random = rand() % (this->size * this->size);
        int row = random / this->size;
        int col = random % this->size;
        if (this->chessboard[row][col] == 0)
            return random;
    }
    return -1;
}

void Game::clearCell(const int &row, const int &col) {
    this->chessboard[row][col] = 0;
    this->numberOfEmptyCells++;
}

void Game::getCellColor(const int &value, LPCOLORREF pFontColor, LPCOLORREF pCellBgColor) const {
	switch (value) {
	case 0: {
		int bgR = 187, bgG = 173, bgB = 160;
		int R = 238, G = 228, B = 218;
		int alpha = 90;

		R = R * alpha / 255 + bgR * (255 - alpha) / 255;
		G = G * alpha / 255 + bgG * (255 - alpha) / 255;
		B = B * alpha / 255 + bgB * (255 - alpha) / 255;
		*pCellBgColor = RGB(R, G, B);
	}
		break;
	case 2:
		*pFontColor = RGB(119, 110, 101);
		*pCellBgColor = RGB(238, 228, 218);
		break;
	case 4:
		*pFontColor = RGB(119, 110, 101);
		*pCellBgColor = RGB(237, 224, 200);
		break;
	case 8:
		*pFontColor = RGB(249, 246, 242);
		*pCellBgColor = RGB(242, 177, 121);
		break;
	case 16:
		*pFontColor = RGB(249, 246, 242);
		*pCellBgColor = RGB(245, 149, 99);
		break;
	case 32:
		*pFontColor = RGB(249, 246, 242);
		*pCellBgColor = RGB(246, 124, 95);
		break;
	case 64:
		*pFontColor = RGB(249, 246, 242);
		*pCellBgColor = RGB(246, 94, 59);
		break;
	case 128:
		*pFontColor = RGB(249, 246, 242);
		*pCellBgColor = RGB(237, 207, 114);
		break;
	case 256:
		*pFontColor = RGB(249, 246, 242);
		*pCellBgColor = RGB(237, 204, 97);
		break;
	case 512:
		*pFontColor = RGB(249, 246, 242);
		*pCellBgColor = RGB(237, 200, 80);
		break;
	case 1024:
		*pFontColor = RGB(249, 246, 242);
		*pCellBgColor = RGB(237, 197, 63);
		break;
	case 2048:
		*pFontColor = RGB(249, 246, 242);
		*pCellBgColor = RGB(237, 194, 46);
		break;
	default:
		*pFontColor = RGB(249, 246, 242);
		*pCellBgColor = RGB(60, 58, 50);
		break;
	}
}

#pragma mark -
#pragma mark Can Move?
bool Game::canMove() const {
	return this->canMoveUp() || this->canMoveDown() || this->canMoveLeft() || this->canMoveRight();
}

bool Game::canMove(const MoveCommand cmd) const {
    switch (cmd) {
        case MoveCommandLeft:
            return this->canMoveLeft();
            break;
        case MoveCommandUp:
            return this->canMoveUp();
            break;
        case MoveCommandRight:
            return this->canMoveRight();
            break;
        case MoveCommandDown:
            return this->canMoveDown();
            break;
        default:
            break;
    }
    return false;
}

bool Game::canMoveUp() const {
    for (int j = 0; j < this->size; j++) {
        bool result = false;
        for (int i = 0; i < this->size; i++) {
            if (this->chessboard[i][j] == 0)
                result = true;
            else if ((i != 0 && this->chessboard[i][j] == this->chessboard[i - 1][j]) || result)
                    return true;
        }
    }
    return false;
}

bool Game::canMoveDown() const {
    for (int j = 0; j < this->size; j++) {
        bool result = false;
        for (int i = this->size - 1; i >= 0; i--) {
            if (this->chessboard[i][j] == 0)
                result = true;
            else if ((i != this->size - 1 && this->chessboard[i][j] == this->chessboard[i + 1][j]) || (result))
                return true;
        }
    }
    return false;
}

bool Game::canMoveLeft() const {
    for (int i = 0; i < this->size; i++) {
        bool result = false;
        for (int j = 0; j < this->size; j++) {
            if (this->chessboard[i][j] == 0)
                result = true;
            else if ((j != 0 && this->chessboard[i][j] == this->chessboard[i][j - 1]) || (result))
                return true;
        }
    }
    return false;
}

bool Game::canMoveRight() const {
    for (int i = 0; i < this->size; i++) {
        bool result = false;
        for (int j = this->size - 1; j >= 0; j--) {
            if (this->chessboard[i][j] == 0)
                result = true;
            else if ((j != this->size - 1 && this->chessboard[i][j] == this->chessboard[i][j + 1]) || (result))
                return true;
        }
    }
    return false;
}

#pragma mark -
#pragma mark Constructor & Destructor
Game::Game() {
    this->printHelpInfo(); // Help info
    srand((unsigned)time(0));
    this->initChessboard(); // Init chessboard
}

Game::Game(RECT clientRect) {
	srand((unsigned)time(0));
	this->initChessboard(); // Init chessboard
	this->setRect(clientRect); // Set chessboard rect
}

Game::~Game() {
    
}

#pragma mark -
#pragma mark Actions
void Game::doMove(const MoveCommand cmd) {
    if (!this->canMove(cmd)) return;
    
    switch (cmd) {
        case MoveCommandLeft:
            this->doLeft();
            break;
        case MoveCommandUp:
            this->doUp();
            break;
        case MoveCommandRight:
            this->doRight();
            break;
        case MoveCommandDown:
            this->doDown();
            break;
        default:
            break;
    }
    
    std::cout << (cmd == MoveCommandUp ? "Move Up" : (cmd == MoveCommandDown ? "Move Down" : (cmd == MoveCommandLeft ? "Move Left" : "Move Right"))) << std::endl;
    this->addRandomCell();
    
	if (this->numberOfEmptyCells == 0 && !this->canMove())  this->over = true; // Game over
    
    this->print();
}

void Game::doUp() {
    for (int j = 0; j < this->size; j++) {
        int l = 0, m = -1, n = -1;
        for (int i = 0; i < this->size; i++) {
            if (this->chessboard[i][j] != 0) {
                if (m == -1) {
                    m = i;
                } else {
                    if (n == -1) {
                        n = i;
                        if (this->chessboard[m][j] == this->chessboard[n][j]) {
                            // Merge
                            this->chessboard[l][j] = this->chessboard[m][j] * 2;
							if (this->chessboard[l][j] == 2048) this->won = true; // Win!!!
                            if (m != l) {
                                this->numberOfEmptyCells--;
                                this->clearCell(m, j);
                            }
                            this->clearCell(n, j);
                            
                            // Add score
                            this->score += this->chessboard[l][j];
                            
                            m = -1;
                        } else {
                            // Move
                            if (m != l) {
                                this->chessboard[l][j] = this->chessboard[m][j];
                                this->numberOfEmptyCells--;
                                this->clearCell(m, j);
                            }
                            
                            m = n;
                        }
                        
                        l++;
                        n = -1;
                    }
                }
            }
        }
        if (m != -1 && m != l) {
            this->chessboard[l][j] = this->chessboard[m][j];
            this->numberOfEmptyCells--;
            this->clearCell(m, j);
        }
    }
}

void Game::doDown() {
    for (int j = 0; j < this->size; j++) {
        int l = this->size - 1, m = -1, n = -1;
        for (int i = this->size - 1; i >= 0; i--) {
            if (this->chessboard[i][j] != 0) {
                if (m == -1) {
                    m = i;
                } else {
                    if (n == -1) {
                        n = i;
                        if (this->chessboard[m][j] == this->chessboard[n][j]) {
                            // Merge
                            this->chessboard[l][j] = this->chessboard[m][j] * 2;
							if (this->chessboard[l][j] == 2048) this->won = true; // Win!!!
                            if (m != l) {
                                this->numberOfEmptyCells--;
                                this->clearCell(m, j);
                            }
                            this->clearCell(n, j);
                            
                            // Add score
                            this->score += this->chessboard[l][j];
                            
                            m = -1;
                        } else {
                            // Move
                            if (m != l) {
                                this->chessboard[l][j] = this->chessboard[m][j];
                                this->numberOfEmptyCells--;
                                this->clearCell(m, j);
                            }
                            
                            m = n;
                        }
                        
                        l--;
                        n = -1;
                    }
                }
            }
        }
        if (m != -1 && m != l) {
            this->chessboard[l][j] = this->chessboard[m][j];
            this->numberOfEmptyCells--;
            this->clearCell(m, j);
        }
    }
}

void Game::doLeft() {
    for (int i = 0; i < this->size; i++) {
        int l = 0, m = -1, n = -1;
        for (int j = 0; j < this->size; j++) {
            if (this->chessboard[i][j] != 0) {
                if (m == -1) {
                    m = j;
                } else {
                    if (n == -1) {
                        n = j;
                        if (this->chessboard[i][m] == this->chessboard[i][n]) {
                            // Merge
                            this->chessboard[i][l] = this->chessboard[i][m] * 2;
							if (this->chessboard[i][l] == 2048) this->won = true; // Win!!!
                            if (m != l) {
                                this->numberOfEmptyCells--;
                                this->clearCell(i, m);
                            }
                            this->clearCell(i, n);
                            
                            // Add score
                            this->score += this->chessboard[i][l];
                            
                            m = -1;
                        } else {
                            // Move
                            if (m != l) {
                                this->chessboard[i][l] = this->chessboard[i][m];
                                this->numberOfEmptyCells--;
                                this->clearCell(i, m);
                            }
                            
                            m = n;
                        }
                        
                        l++;
                        n = -1;
                    }
                }
            }
        }
        if (m != -1 && m != l) {
            this->chessboard[i][l] = this->chessboard[i][m];
            this->numberOfEmptyCells--;
            this->clearCell(i, m);
        }
    }
}

void Game::doRight() {
    for (int i = 0; i < this->size; i++) {
        int l = this->size - 1, m = -1, n = -1;
        for (int j = this->size - 1; j >= 0; j--) {
            if (this->chessboard[i][j] != 0) {
                if (m == -1) {
                    m = j;
                } else {
                    if (n == -1) {
                        n = j;
                        if (this->chessboard[i][m] == this->chessboard[i][n]) {
                            // Merge
                            this->chessboard[i][l] = this->chessboard[i][m] * 2;
							if (this->chessboard[i][l] == 2048) this->won = true; // Win!!!
                            if (m != l) {
                                this->numberOfEmptyCells--;
                                this->clearCell(i, m);
                            }
                            this->clearCell(i, n);
                            
                            // Add score
                            this->score += this->chessboard[i][l];
                            
                            m = -1;
                        } else {
                            // Move
                            if (m != l) {
                                this->chessboard[i][l] = this->chessboard[i][m];
                                this->numberOfEmptyCells--;
                                this->clearCell(i, m);
                            }
                            
                            m = n;
                        }
                        
                        l--;
                        n = -1;
                    }
                }
            }
        }
        if (m != -1 && m != l) {
            this->chessboard[i][l] = this->chessboard[i][m];
            this->numberOfEmptyCells--;
            this->clearCell(i, m);
        }
    }
}

void Game::restart() {
    this->initChessboard(); // Init chessboard
}

void Game::stop() {
	this->over = true;
}

#pragma mark -
#pragma mark Is over or won?
bool Game::isOver() const {
    return this->over;
}

bool Game::isWon() const {
    return this->won;
}

bool Game::isTerminated() const {
    return this->over || this->won;
}

#pragma mark -
#pragma mark View
void Game::print() const {
    std::cout << "================================================================" << std::endl;
    std::cout << "Score: " << this->score << std::endl;
    std::cout << "Empty: " << this->numberOfEmptyCells << std::endl;
    
    if (this->isTerminated()) {
        if (this->won)
            std::cout << "YOU WIN!!!" << std::endl;
        else if (this->over)
            std::cout << "YOU LOSE!!!" << std::endl;
    }
    
    for (int i = 0; i < this->size; i++) {
        for (int j = 0; j < this->size; j++) {
            printf("%4d ", this->chessboard[i][j]);
        }
        std::cout << std::endl;
    }
    std::cout << "================================================================" << std::endl << std::endl;
}

void Game::printHelpInfo() const {
    // 2048
    std::cout << std::endl;
    std::cout << "      ####            ####               ##          ######     " << std::endl;
    std::cout << "   ##########       ########            ##         ###    ###   " << std::endl;
    std::cout << " #####    #####    ###    ###          ##  ###    ###      ###  " << std::endl;
    std::cout << " ###       ####   ###      ###        ##   ###   ###        ### " << std::endl;
    std::cout << "          ####   ###        ###      ##    ###    ###      ###  " << std::endl;
    std::cout << "         ####    ###        ###     ##     ###     ###    ###   " << std::endl;
    std::cout << "        ####     ###        ###    ##      ###       ######     " << std::endl;
    std::cout << "       ####      ###        ###   ##       ###     ####  ####   " << std::endl;
    std::cout << "      ####       ###        ###  ###       ###    ###      ###  " << std::endl;
    std::cout << "     ####        ###        ###  ##############  ###        ### " << std::endl;
    std::cout << "    ####         ###        ###  ##############  ###        ### " << std::endl;
    std::cout << "   ####           ###      ###             ###   ###        ### " << std::endl;
    std::cout << "  ####             ###    ###              ###    ###      ###  " << std::endl;
    std::cout << " ##############     ########               ###     ###    ###   " << std::endl;
    std::cout << " ##############       ####                 ###       ######     " << std::endl;
    std::cout << std::endl;
    
    // Help
    std::cout << "Control:" << std::endl;
    std::cout << "WASD: A - Left, S - Down, W - Up, D - Right" << std::endl;
    std::cout << "Vim:  H - Left, J - Down, K - Up, L - Right" << std::endl;
    std::cout << std::endl;
}

void Game::draw(HDC hdc) const {
	this->drawChessboard(hdc); // Draw chessboard grid

	// Draw cells
	for (int i = 0; i < this->size * this->size; i++) {
		int row = i / this->size;
		int col = i % this->size;

		// if (this->chessboard[row][col] == 0) continue;

		this->drawCell(hdc, row, col, this->chessboard[row][col]);
	}
}

void Game::drawTopBar(HDC hdc) const {
	// Draw 2048
	this->drawLogoText(hdc);

	// Draw score label
	this->drawScoreLabel(hdc);

	// Draw time rect bg
	RECT timeRect = this->scoreLabelRect;
	long width = getRectWidth(timeRect);
	timeRect.left -= width;
	timeRect.right -= width;

	// Draw bg round rect
	drawRoundRect(hdc, timeRect, RGB(187, 173, 160));
}

void Game::drawLogoText(HDC hdc) const {
	RECT logoRect = this->topBarRect;
	logoRect.right = (logoRect.left + logoRect.right) / 2;

	drawNumberOnRect(hdc, logoRect, 2048, RGB(119, 110, 101));
}

void Game::drawScoreLabel(HDC hdc) const {
	RECT scoreLabelRect = this->scoreLabelRect;
	
	// Draw bg round rect
	drawRoundRect(hdc, this->scoreLabelRect, RGB(187, 173, 160));
	
	// Draw 'SCORE' text
	long scoreLabelHeight = getRectHeight(scoreLabelRect);
	scoreLabelRect.bottom -= (scoreLabelHeight * 0.7);

	HFONT font = createFont(getRectHeight(scoreLabelRect) * 0.66);
	SetTextColor(hdc, 0xeee4da); // Font color
	SetBkMode(hdc, TRANSPARENT); // Background color transparentk
	SelectObject(hdc, font); // Set font

	DrawText(hdc, TEXT("SCORE"), -1, &scoreLabelRect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	
	DeleteObject(font);

	// Draw score
	scoreLabelRect.top += (scoreLabelHeight * 0.3);
	scoreLabelRect.bottom += (scoreLabelHeight * 0.7);

	drawNumberOnRect(hdc, scoreLabelRect, this->score, RGB(249, 246, 242));
}

void Game::drawChessboard(HDC hdc) const {
	//long left = this->chessboardRect.left;
	//long top = this->chessboardRect.top;
	//long right = this->chessboardRect.right;
	//long bottom = this->chessboardRect.bottom;

	//long gridWidth = this->chessboardWidth;

	//long spacing = gridWidth / 4;

	//// 绘制竖线
	//for (int i = left; i <= right; i += spacing) {
	//	drawLine(hdc, i, top, i, bottom);
	//}

	//// 绘制横线
	//for (int i = top; i <= bottom; i += spacing) {
	//	drawLine(hdc, left, i, right, i);
	//}

	// Draw top bar
	this->drawTopBar(hdc);

	// Draw chessboard bg
	drawRoundRect(hdc, this->chessboardRect, RGB(187, 173, 160));
}

void Game::drawCell(HDC hdc, const int &row, const int &col, const int &value) const {
	long spacing = this->chessboardWidth / 4;
	long cellWidth = spacing * 0.8;

	long cellMargin = (spacing - cellWidth) / 2;
	POINT cellTopLeftPoint = { cellMargin + this->chessboardRect.left + col * spacing, cellMargin + this->chessboardRect.top + row * spacing };

	RECT cellRect = { cellTopLeftPoint.x, cellTopLeftPoint.y, cellTopLeftPoint.x + cellWidth, cellTopLeftPoint.y + cellWidth };

	COLORREF bgColor;
	COLORREF fontColor;

	this->getCellColor(value, &fontColor, &bgColor);
	
	// Draw cell bg
	drawRoundRect(hdc, cellRect, bgColor);

	if (0 == value) return;
	
	// Draw cell number
	drawNumberOnCell(hdc, cellRect, value, fontColor);
}

void Game::setRect(const RECT clientRect) {
	long clientWidth = getRectWidth(clientRect);
	long clientHeight = getRectHeight(clientRect);

	long gameWidth, gameHeight;
	if (clientWidth < clientHeight) {
		gameHeight = min(clientWidth / 3 * 4, clientHeight);
		gameWidth = gameHeight / 4 * 3;
	} else {
		gameHeight = clientHeight;
		gameWidth = gameHeight / 4 * 3;
	}

	this->chessboardWidth = gameWidth;

	long topBarHeight = gameHeight - gameWidth;

	long marginTop = (clientHeight - gameHeight) / 2;
	long marginLeft = (clientWidth - gameWidth) / 2;

	// Top bar rect
	this->topBarRect = { marginLeft, marginTop, marginLeft + gameWidth, marginTop + topBarHeight };

	// Score label rect
	this->scoreLabelRect = this->topBarRect;
	this->scoreLabelRect.left += (gameWidth * 3 / 4);
	this->scoreLabelRect.bottom -= (getRectHeight(this->scoreLabelRect) / 2);
	this->scoreLabelRect.top += (topBarHeight * 0.1);
	this->scoreLabelRect.bottom += (topBarHeight * 0.1);

	// Chessboard rect
	this->chessboardRect = { marginLeft, marginTop + topBarHeight, marginLeft + gameWidth, marginTop + gameHeight }; 
}
