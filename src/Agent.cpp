#include "Agent.h"
#include <iostream>
using namespace std;

CAIAgent::CAIAgent(CGameSolver* Solver) : Solver(Solver)
{

}

bool CAIAgent::IsAI()
{
	return true;
}

void CAIAgent::GetNextAction(SAction& Action)
{
	vector<SAction> Actions;
	vector<float> Scores;
	vector<int> VisitCnts;
	GetActionsWithScore(Actions, Scores, VisitCnts);
	Action = Actions[0];
}

void CAIAgent::GetActionsWithScore(vector<SAction>& Actions, vector<float>& Scores, vector<int>& VisitCnts)
{
	Solver->Solve(Chess, Actions, Scores, VisitCnts);
}

bool CHumanAgent::IsAI()
{
	return false;
}

void CHumanAgent::GetNextAction(SAction& Action)
{
	cin >> Action.x1 >> Action.y1 >> Action.x2 >> Action.y2;
}
