#pragma once
#include <vector>
#include "ChinaChess.h"
using namespace std;
class CGameSolver
{
public:
	virtual void Solve(CChess* Board, vector<SAction>& Actions, vector<float>& Scores, vector<int>& VisitCnts) = 0;
};