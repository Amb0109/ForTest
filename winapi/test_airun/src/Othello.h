#ifndef _GAMELOGIC_OTHELLO_H_
#define _GAMELOGIC_OTHELLO_H_

typedef int CHESS_TYPE;

static const int DIR_X[8] = {1, 1, 1, 0, 0, -1, -1, -1};
static const int DIR_Y[8] = {0, -1, 1, -1, 1, 0, -1, 1};

class COthello
{
public:
	static const int N_BOARD = 8;

	static const CHESS_TYPE C_EMPTY = 0;
	static const CHESS_TYPE C_BLACK = 1;
	static const CHESS_TYPE C_WHITE = 2;

private:
	CHESS_TYPE _BOARD[N_BOARD][N_BOARD];

public:
	COthello ();
	~COthello ();

public:
	void initGame();
	bool putChess(int x, int y, CHESS_TYPE c_flg);

	CHESS_TYPE antiC(CHESS_TYPE c_flg);
	bool inBoard(int x, int y);
	bool isValid(CHESS_TYPE c_flg);
	bool isOver();

	void asciiBoard();
};

#endif //_GAMELOGIC_OTHELLO_H_
