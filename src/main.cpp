#include "ChessGame.h"
#include "Agent.h"
#include <iostream>
int main(int argc, char* argv[])
{
	CChessGameAVA ChessGame;
	ChessGame.PlayGame();
	std::cout << "end" << endl;
	return 0;
}