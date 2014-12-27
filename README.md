2048 Win 32
=======================

课程: Windows程序设计

项目: Win 32项目

## 目录

- 项目概述
- 功能概述
	- 操作方式
	- 游戏功能
- 使用场景
- 程序架构
	- 核心类
	- 辅助函数
- 设计理念
- 其他

## 项目概述

本项目是一个2048游戏，基于Win 32开发，灵感来自此前网络上流行的2048游戏

## 功能概述

### 一、操作方式

该游戏支持三种键盘操作键位：

1. 方向键操控
2. WASD键位操控：

	__`W`__ - ___上___, __`A`__ - ___左___, __`S`__ - ___下___, __`D`__ - ___右___
	
3. Vim操作方式：

	__`H`__ - ___左___, __`J`__ - ___下___, __`K`__ - ___上___, __`L`__ - ___右___
	
### 二、游戏功能

1. 倒计时

	游戏有一个 ___15分钟___ 的倒计时，玩家需要在规定时间内完成游戏(即合并出数字为`2048`的方块)

2. 方块出现规则

	在游戏中，方块的出现完全随机，仅出现在空方格中。当方块出现时，方块上的数字有 ___90%___ 的几率为`2`， ___10%___ 的几率为4
	
	游戏开始时，会在所有空方格中随机选两个方格放入带有数字的方块，方块上的数字依照上述规则
	
	在玩家每一次移动后，棋盘上会新增一个方块，规则如上所述
	
3. 方块合并规则

	两个数字相同的方块才能合并，合并后生成一个数字为原来2倍的方块

4. 计分规则

	玩家获得分数仅发生在方块合并后，合并后玩家会获得的分数为新生成的方块的数字
	
5. 游戏结束规则

	下列几种情况会使得游戏结束：
	
	- 倒计时结束
	- 合并得到数字为`2048`的方块
	- 棋盘全满，无法再增加新方块，且玩家进行任何移动操作都无法使棋盘新增一个空位

## 使用场景

1. 主界面

<div style="margin: 0 auto; width: 400px;"><img style="width: 100%;" src="https://raw.githubusercontent.com/h1994st/2048-Win32/master/img/index.PNG" alt="主界面"></div>

主界面右上方会显示倒计时和分数

2. 游戏结束

<div style="margin: 0 auto; width: 400px;"><img style="width: 100%;" src="https://raw.githubusercontent.com/h1994st/2048-Win32/master/img/3.PNG" alt="游戏失败"></div>

游戏结束后弹出的窗口中会告诉用户游戏情况(成功/失败/超时)，同时也会显示分数

弹出的窗口中有两个按钮："再来一局"和"退出"。如果按"再来一局"，弹窗会关闭，游戏所有数据将会重置，玩家重新开始游戏；如果按"退出"，将直接退出整个程序

## 程序架构

<div style="margin: 0 auto; width: 600px;"><img style="width: 100%;" src="https://raw.githubusercontent.com/h1994st/2048-Win32/master/img/8.png" alt="项目结构"></div>

这张图由Visual Studio生成，从中可以看出这是一个Win 32项目，其中有一个核心类`Game`，用来处理所有的游戏逻辑

### 核心类`Game`

`Game`这个核心类暴露在外的成员变量和方法如下图：

<div style="margin: 0 auto; width: 200px;"><img style="width: 100%;" src="https://raw.githubusercontent.com/h1994st/2048-Win32/master/img/10.png" alt="Class Game"></div>

(整体类图过大，不予展示，详见[Game类图](https://raw.githubusercontent.com/h1994st/2048-Win32/master/img/9.png))

下面介绍其中比较重要的几个方法：

1. `draw()`

	该方法涉及整个游戏界面的绘制，函数原型为：
	
	```
	void draw(HDC hdc) const;
	```
	
	在程序的主消息循环中，一旦接收到`WM_PAINT`消息，将会调用该方法对界面进行重绘，当然是有选择性的重绘，因为界面上除了棋盘、分数、倒计时，其他部分都没有必要重绘
	
	该方法在`Game`类内部还会调用以下私有方法：
	
	```
	void drawChessboard(HDC hdc) const; // 绘制棋盘背景，绘制上部分数、logo等，会调用下面三个方法
	void drawTopBar(HDC hdc) const; 
	void drawLogoText(HDC hdc) const;
	void drawScoreLabel(HDC hdc) const;
	void drawCell(HDC hdc, const int &row, const int &col, const int &value) const; // 绘制方块
	```

2. `setRect()`

	该方法用于设置游戏画面的区域，函数原型如下：
	
	```
	void setRect(const RECT clientRect);
	```
	
	该方法会设置好`scoreLabelRect`和`chessboardRect`，便于之后在主消息循环中重绘时，进行有选择的重绘

3. `restart()`

	该方法会重置所有游戏信息，让游戏重新开始，如下：
	
	```
	void Game::restart() {
	    this->initChessboard(); // Init chessboard
	}
	```

4. `initChessboard()`

	该方法用于初始化整个游戏，如下：
	
	```
	void Game::initChessboard() {
	    this->score = 0;
	    this->over = false;
	    this->won = false;
	    this->numberOfEmptyCells = 16;
	    memset(this->chessboard, 0, 4 * 4 * sizeof(int)); // Init chessboard
	    this->initStartCells(); // Add the initial cells
	}
	```

5. 枚举变量`MoveCommand`

	```
	typedef enum {
	    MoveCommandUp,
	    MoveCommandDown,
	    MoveCommandLeft,
	    MoveCommandRight
	} MoveCommand;
	```
	
	定义这样的枚举变量用于表示移动方向，更多考虑的是程序源代码的可读性以及可维护性

6. `canMove()`

	该方法有两个重载函数，如下：
	
	```
	bool canMove() const;
	bool canMove(const MoveCommand cmd) const;
	```
	
	含有参数的方法用于判断特定的移动指令下棋盘上的方块能否移动，一般用于玩家按下某个方向键后，如果无法移动，那么将不会改变棋盘状态，也不会进行重绘
	
	在玩家某一次操作结束后，还会判断接下来用户能否继续移动，这时候需要判断四个方向，故会调用没有参数的`canMove()`方法
	
	如果接下来棋盘为满且不能移动，那么玩家输掉这盘游戏，如下：
	
	```
	if (this->numberOfEmptyCells == 0 && !this->canMove()) {
		this->over = true; // Game over
	}
	```

7. `doMove()`

	函数原型如下：
	
	```
	void Game::doMove(const MoveCommand cmd) {
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
	    this->addRandomCell(); // Add cell
		if (this->numberOfEmptyCells == 0 && !this->canMove()) {
			this->over = true; // Game over
		}
	}
	```
	
	可以看到，该函数实际上涉及到四个私有函数：
	
	```
	void doUp();
    void doDown();
    void doLeft();
    void doRight();
	```

	这四个函数才是真正会修改游戏棋盘状态的函数
	
### 辅助函数

该项目中将一些重用较多的函数提取到`Utils.h`和`Utils.cpp`中，如下：

```
long getRectWidth(RECT rect);
long getRectHeight(RECT rect);

void drawLine(HDC hdc, int x1, int y1, int x2, int y2);
void drawRoundRect(HDC hdc, RECT cellRect, COLORREF bgColor);
void drawNumberOnCell(HDC hdc, RECT cellRect, const int &number, COLORREF fontColor);
void drawNumberOnRect(HDC hdc, RECT rect, const int &number, COLORREF fontColor);
void drawNumberOnTimeLabel(HDC hdc, RECT rect, LPCWSTR pWStr, COLORREF fontColor);

HFONT createFont(int height);
void getTimeStringByValue(UINT time, LPCWSTR result);
```

## 设计理念

因为该项目并不复杂，所以将逻辑和界面都写在一个类里，该项目的实际开发过程如下：

1. 完成静态界面的绘制，不会响应任何按键信息
2. 完成[命令行版2048](https://github.com/h1994st/2048-Console)，不涉及任何界面，纯逻辑，核心类`Game`在该阶段完成
3. 将界面绘制整合到核心类`Game`中，为之新添诸如`chessboardRect`等成员变量以及`draw()`等涉及界面绘制的方法

## 其他

项目开发中有一点值得一提。由于绘图时涉及字体还有颜色填充，所以用到了`CreateFont()`、`CreatePen()`和`CreateBrush()`等函数，它们会返回`HBRUSH`、`HPEN`、`HFONT`等类型的变量，这些变量都属于`HGDIOBJ`，本质上都是指针

实际上每次使用时都新建了这样的`HGDIOBJ`，如下：

```
// Draw round rect
void drawRoundRect(HDC hdc, RECT cellRect, COLORREF bgColor) {
	HBRUSH brush = CreateSolidBrush(bgColor);
	HPEN pen = CreatePen(PS_NULL, 0, NULL);
	SelectObject(hdc, brush);
	SelectObject(hdc, pen);
	RoundRect(hdc, cellRect.left, cellRect.top, cellRect.right, cellRect.bottom, 10, 10);
}
```

这样每次都会在内存中分配空间，重点在于并未回收内存，最开始没有意识到这一点，导致游戏进行到最后整个界面崩溃。在不用`HGDIOBJ`后，应该调用`DeleteObject()`来及时释放内存，经过修改后的函数应该如下所示：

```
// Draw round rect
void drawRoundRect(HDC hdc, RECT cellRect, COLORREF bgColor) {
	HBRUSH brush = CreateSolidBrush(bgColor);
	HPEN pen = CreatePen(PS_NULL, 0, NULL);
	SelectObject(hdc, brush);
	SelectObject(hdc, pen);
	RoundRect(hdc, cellRect.left, cellRect.top, cellRect.right, cellRect.bottom, 10, 10);
	DeleteObject(brush);
	DeleteObject(pen);
}
```

整个项目中有多处进行了这样的修改，最后修复了画面崩溃的问题
