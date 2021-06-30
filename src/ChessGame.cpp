#include "ChessGame.h"
#include "Agent.h"
#include "AlphaBetaSolver.h"
#include "TreeSearchSolver.h"
#include "Inferencer.h"
#include <iostream>
#include <random>
#include <ctime>
using namespace std;
CChessGame::CChessGame(CAgent* Agent1, CAgent* Agent2):  Agent1(Agent1), Agent2(Agent2)
{

}

CChessGamePVP::CChessGamePVP(): CChessGame(new CHumanAgent(), new CHumanAgent())
{

}

void CChessGame::RefreshView()
{
	Chess->Print();
}

void CChessGame::PlayGame()
{
	bool bIsFinished = false;
	Chess = make_unique<CChess>();
	Agent1->SetChess(Chess.get());
	Agent2->SetChess(Chess.get());
	while (!bIsFinished)
	{
		SAction Action;
		RefreshView();
		Agent1->GetNextAction(Action);
		Chess->ExcuteAction(Action);
		RefreshView();
		bIsFinished = Chess->IsWin();
		if (bIsFinished)
		{
			break;
		}
		Chess->RevertBoard();
		Agent2->GetNextAction(Action);
		Chess->ExcuteAction(Action);
		Chess->RevertBoard();
		bIsFinished = Chess->IsWin();
	}
};

CChessGamePVA::CChessGamePVA() : CChessGame(new CHumanAgent(),new CAIAgent(new CTreeSearchSolver(new CSimpleInferencer(),100000)))
{

}

CChessGameAVA::CChessGameAVA() :CChessGame(
		new CAIAgent(new CAlphaBetaSolver(3)),
		new CAIAgent(new CTreeSearchSolver(new CSimpleInferencer(), 500000)))
{

}

bool CChessGameAVA::CanSelectAction(CChess* Chess,SAction Action)
{
	if (Histories.size() < 7)
	{
		return true;
	}
	size_t Len = Histories.size();
	if (Histories[Len - 2] != Histories[Len - 6])
	{
		return true;
	}
	EPieceType Piece =  Chess->ExcuteAction(Action);
	Chess->RevertBoard();
	string State = Chess->PackageBoard();
	Chess->RevertBoard();
	Chess->RevertAction(Action, Piece);
	return State != Histories[Len - 4];
}

SAction CChessGameAVA::SelectActionForAgent(CChess* Chess,CAgent* Agent)
{
	vector<SAction> Actions;
	vector<float> Scores;
	vector<int> VisitCnts;
	Agent->GetActionsWithScore(Actions, Scores, VisitCnts);
	HistorVisitCnts.push_back(VisitCnts);
	HistoryActions.push_back(Actions);

	vector<bool> Mask(Actions.size(),true);
	static default_random_engine generator(static_cast<uint32>(time(NULL)));
	size_t Cnt = Actions.size();
	float S = 1.0f;
	while (true)
	{
		uniform_real_distribution<float> distribution(0.0f, S);
		float P = distribution(generator);
		float Sum = 0;
		// cout << "p" << endl;
		for (size_t i = 0; i < Actions.size(); i++)
		{
			if (Mask[i])
			{
				if (Cnt == 1)
				{
					return Actions[i];
				}
				Sum += Scores[i];
				if (P <= Sum)
				{
					if (CanSelectAction(Chess,Actions[i]))
					{
						return Actions[i];
					}
					Mask[i] = false;
				}
			}
		}
	}
}

void CChessGameAVA::PlayGame()
{
	int T = 0;
	while (T < 1)
	{
		T++;
		Chess = make_unique<CChess>();
		Agent1->SetChess(Chess.get());
		Agent2->SetChess(Chess.get());
		bool bIsFinished = false;
		int Cnt = 0;
		while (!bIsFinished && Cnt < 1000)
		{
			SAction Action;
			RefreshView();
			Histories.push_back(Chess->PackageBoard());
			// Action = SelectActionForAgent(Chess.get(), Agent1.get());
			Agent1->GetNextAction(Action);
			Chess->ExcuteAction(Action);
			RefreshView();
			bIsFinished = Chess->IsWin();
			Chess->RevertBoard();
			if (bIsFinished)
			{
				Histories.push_back(Chess->PackageBoard());
				Winer = 0;
				break;
			}
			Histories.push_back(Chess->PackageBoard());
			Action = SelectActionForAgent(Chess.get(), Agent2.get());
			// Agent2->GetNextAction(Action);
			Chess->ExcuteAction(Action);
			Chess->RevertBoard();
			bIsFinished = Chess->IsWin();
			if (bIsFinished)
			{
				Histories.push_back(Chess->PackageBoard());
				Winer = 1;
			}
			Cnt++;
		}
		Winer = 3;
	}

}