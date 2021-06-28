#pragma once
#include <memory>
#include "ChinaChess.h"
class CAgent;
using namespace std;
class CChessGame
{
protected:
	unique_ptr<CChess> Chess;
	unique_ptr<CAgent> Agent1;
	unique_ptr<CAgent> Agent2;
public:
	CChessGame(CAgent* Agent1,CAgent* Agent2);
	virtual void RefreshView();
	virtual void PlayGame();
};


class CChessGamePVP :public CChessGame
{
public:
	CChessGamePVP();
	//virtual void RefreshView() override;
	//virtual void PlayGame() override;
};

class CChessGamePVA :public CChessGame
{
public:
	CChessGamePVA();
	//virtual void RefreshView() override;
	//virtual void PlayGame() override;
};

class CChessGameAVA :public CChessGame
{
public:
	CChessGameAVA();
	vector<string> Histories;
	vector<vector<SAction>> HistoryActions;
	vector<vector<int>> HistorVisitCnts;
	int Winer;
	bool CanSelectAction(CChess* Chess,SAction Action);
	SAction SelectActionForAgent(CChess* Chess,CAgent* Agent);
	//virtual void RefreshView() override;
	virtual void PlayGame() override;
};