#include "stdio.h"
#include "Windows.h"
#include "Player.h"
#include "Othello.h"


CPlayer playerBlack;
CPlayer playerWhite;
COthello gameOthello;

char buff[1024];

int main()
{
	gameOthello.initGame();
	playerBlack.initPlayer();
	playerWhite.initPlayer();

	gameOthello.asciiBoard();

	playerBlack.runSubProcess("othello_W");
	playerWhite.runSubProcess("othello_B");
	
	playerBlack.Send("first");
	playerWhite.Send("second");

	int x, y;
	while(true)
	{
		playerBlack.Recv(buff);
		sscanf(buff, "%d %d", &x, &y);
		puts(buff);
		if (gameOthello.isValid(COthello::C_BLACK))
		{
			gameOthello.putChess(x, y, COthello::C_BLACK);
		}
		gameOthello.asciiBoard();

		if (gameOthello.isOver())
		{
			break;
		}

		playerWhite.Send(buff);
		playerWhite.Recv(buff);
		sscanf(buff, "%d %d", &x, &y);
		puts(buff);
		if (gameOthello.isValid(COthello::C_WHITE))
		{
			gameOthello.putChess(x, y, COthello::C_WHITE);
		}
		gameOthello.asciiBoard();
		
		if (gameOthello.isOver())
		{
			break;
		}

		playerBlack.Send(buff);
	}

	DWORD timeBlack = playerBlack.getPlayerTime();
	DWORD memBlack = playerBlack.getPlayerMemory();
	DWORD timeWhite = playerWhite.getPlayerTime();
	DWORD memWhite = playerWhite.getPlayerMemory();

	return 0;
}