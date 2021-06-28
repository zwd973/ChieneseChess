#pragma once
#include "GameSolver.h"
#include <memory>
class CInferencer;
class CAlphaBetaSolver : public CGameSolver
{
public:
	const static int MIN_BOUND = -100000;
	const static int MAX_BOUND = 100000;
private:
	unique_ptr<CInferencer> Inferencer;
	int Deep;
public:
	CAlphaBetaSolver(int Deep = 2);
	void AlphaBetaSearchCore(CChess* Chess, int Alpha, int Beta, int& Qvalue, int Deep);
	virtual void Solve(CChess* Chess, vector<SAction>& Actions, vector<float>& Scores, vector<int>& VisitCnts) override;
};