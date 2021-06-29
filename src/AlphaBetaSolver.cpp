#include "AlphaBetaSolver.h"
#include "Inferencer.h"
#include <vector>
#include <algorithm>
#include <assert.h>
#include <iostream>
CAlphaBetaSolver::CAlphaBetaSolver(int Deep) : Inferencer(new CSimpleInferencer()), Deep(Deep)
{

}

static bool flag = false;

void CAlphaBetaSolver::AlphaBetaSearchCore(CChess* Chess, int Alpha, int Beta, int& Qvalue, int Deep)
{
	if (Chess->IsWin())
	{
		Qvalue =  MIN_BOUND;
		return;
	}
	vector<SAction> Actions;
	vector<float> Predicts;
	float PreQvalue;
	Chess->GetActions(Actions);
	vector<int> Indexs(Actions.size());
	for (size_t i = 0; i < Indexs.size(); i++)
	{
		Indexs[i] = static_cast<int>(i);
	}
	if (Deep <= 0)
	{
		Qvalue = Inferencer->GetIntQvalueOf(Chess);
		return;
	}
	Inferencer->Inference(Chess, Actions, Predicts, PreQvalue);
	auto Comp = [&Predicts](int l, int r) {
		return Predicts[l] > Predicts[r];
	};
	sort(Indexs.begin(), Indexs.end(), Comp);
	Qvalue = MIN_BOUND;
	for (size_t i = 0; i < Indexs.size(); i++)
	{
		int Index = Indexs[i];
		SAction Action = Actions[i];
		EPieceType TargetPiece = Chess->ExcuteAction(Action);
		Chess->RevertBoard();
		int NextQvalue;
		AlphaBetaSearchCore(Chess, -Beta, -Alpha, NextQvalue, Deep - 1);
		Qvalue = max(-NextQvalue, Qvalue);
		Alpha = max(Alpha, Qvalue);
		Chess->RevertBoard();
		Chess->RevertAction(Action, TargetPiece);
		if (Qvalue > Beta)
		{
			return;
		}
	}
}

void CAlphaBetaSolver::Solve(CChess* Chess, vector<SAction>& Actions, vector<float>& Scores, vector<int>& VisitCnts)
{
	vector<SAction> CurActions;
	vector<float> CurScores;
	vector<float> Predicts;
	float PreQvalue;
	Chess->GetActions(CurActions);
	vector<int> Indexs(CurActions.size());
	for (size_t i = 0; i < Indexs.size(); i++)
	{
		Indexs[i] = static_cast<int>(i);
	}
	Inferencer->Inference(Chess, CurActions, Predicts, PreQvalue);
	auto Comp = [&Predicts](int l, int r) {
		return Predicts[l] > Predicts[r];
	};
	sort(Indexs.begin(), Indexs.end(), Comp);
	CurScores.resize(CurActions.size());
	for (size_t i = 0; i < Indexs.size(); i++)
	{
		int Index = Indexs[i];
		SAction Action = CurActions[Index];
		EPieceType TargetPiece = Chess->ExcuteAction(Action);
		Chess->RevertBoard();
		int Qvalue;
		AlphaBetaSearchCore(Chess, MIN_BOUND, MAX_BOUND, Qvalue, Deep);
		Qvalue = -Qvalue;
		CurScores[Index] = static_cast<float>(Qvalue);
		Chess->RevertBoard();
		Chess->RevertAction(Action, TargetPiece);
	}
	Actions.resize(CurActions.size());
	auto Comp2 = [&CurScores](int l, int r) {
		return CurScores[l] > CurScores[r];
	};
	sort(Indexs.begin(), Indexs.end(), Comp2);
	Scores.resize(Actions.size());
	for (size_t i = 0; i < Indexs.size(); i++)
	{
		Actions[i] = CurActions[Indexs[i]];
		Scores[i] = CurScores[Indexs[i]];
	}
}
