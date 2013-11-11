#include "Othello.h"
#include "stdio.h"

COthello::COthello()
{

}

COthello::~COthello()
{
}

void COthello::initGame()
{
	for (int ix = 0 ; ix < N_BOARD; ++ix)
	{
		for (int iy = 0; iy < N_BOARD; ++iy)
		{
			_BOARD [ix][iy] = C_EMPTY;
		}
	}
	_BOARD [3][3] = _BOARD [4][4] = C_BLACK;
	_BOARD [3][4] = _BOARD [4][3] = C_WHITE;
}

bool COthello::inBoard(int x, int y)
{
	if ( x<0 || x >= N_BOARD ) return false;
	if ( y<0 || y >= N_BOARD ) return false;
	return true;
}

CHESS_TYPE COthello::antiC(CHESS_TYPE c_flg)
{
	return (3-c_flg);
}

bool COthello::putChess(int x, int y, int c_flg)
{
	if ( c_flg != C_BLACK && c_flg != C_WHITE ) return false;
	if ( x == -1 && y == -1 && !isValid(c_flg) ) return true;
	if ( !inBoard(x, y) ) return false;
	if ( _BOARD[x][y] != C_EMPTY ) return false;
	
	bool bFlip = false;
	for (int dir = 0; dir < 8; ++dir)
	{
		int tx = x+DIR_X[dir];
		int ty = y+DIR_Y[dir];
		if ( !inBoard(tx, ty) ) continue;
		if ( _BOARD[tx][ty] != antiC(c_flg) ) continue;

		while ( inBoard(tx, ty) && _BOARD[tx][ty] == antiC(c_flg) )
		{
			tx += DIR_X[dir];
			ty += DIR_Y[dir];
		}

		if ( inBoard(tx, ty) && _BOARD[tx][ty] == c_flg)
		{
			bFlip = true;
			tx -= DIR_X[dir];
			ty -= DIR_Y[dir];
			while (inBoard(tx, ty) && _BOARD[tx][ty] == antiC(c_flg) )
			{
				_BOARD[tx][ty] = c_flg;
				tx -= DIR_X[dir];
				ty -= DIR_Y[dir];
			}
		}
	}

	if (bFlip == true)
	{
		_BOARD[x][y] = c_flg;
		return true;
	}
	else
	{
		return false;
	}
}

bool COthello::isValid(CHESS_TYPE c_flg)
{
	for (int ix = 0; ix < N_BOARD; ++ix)
	{
		for (int iy = 0; iy < N_BOARD; ++iy)
		{
			if ( _BOARD[ix][iy] != C_EMPTY ) continue;

			for (int dir = 0; dir < 8; ++dir)
			{
				int tx = ix+DIR_X[dir];
				int ty = iy+DIR_Y[dir];
				if ( !inBoard(tx, ty) ) continue;
				if ( _BOARD[tx][ty] != antiC(c_flg) ) continue;
				while ( inBoard(tx, ty) && _BOARD[tx][ty] == antiC(c_flg) )
				{
					tx += DIR_X[dir];
					ty += DIR_Y[dir];
				}
				if ( inBoard(tx, ty) && _BOARD[tx][ty] == c_flg)
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool COthello::isOver()
{
	int CNT_BLACK = 0;
	int CNT_WHITE = 0;
	int CNT_EMPTY = 0;

	for (int x = 0; x < N_BOARD; ++x)
	{
		for (int y = 0; y < N_BOARD; ++y)
		{
			if ( _BOARD[x][y] == C_BLACK ) CNT_BLACK ++;
			if ( _BOARD[x][y] == C_WHITE ) CNT_WHITE ++;
			if ( _BOARD[x][y] == C_EMPTY ) CNT_EMPTY ++;
		}
	}

	if ( CNT_BLACK == 0 ) return true;
	if ( CNT_WHITE == 0 ) return true;
	if ( CNT_EMPTY == 0 ) return true;

	if ( isValid(C_BLACK) ) return false;
	if ( isValid(C_WHITE) ) return false;

	return true;
}

void COthello::asciiBoard()
{
	for (int x = 0; x < N_BOARD; ++x)
	{
		for (int y = 0; y < N_BOARD; ++y)
		{
			if ( _BOARD[x][y] == C_EMPTY ) putchar('.');
			if ( _BOARD[x][y] == C_BLACK ) putchar('X');
			if ( _BOARD[x][y] == C_WHITE ) putchar('O');
		}
		puts("");
	}
	puts("");
};