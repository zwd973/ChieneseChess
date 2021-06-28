#pragma once
#include "GameSolver.h"
#include <memory>
class CAgent
{
protected:
	CChess* Chess;
public:
	inline void SetChess(CChess* _Chess_)
	{
		Chess = _Chess_;
	};
	virtual bool IsAI() = 0;
	virtual void GetActionsWithScore(vector<SAction>& Actions, vector<float>& Scores, vector<int>& VisitCnts) {};
	virtual void GetNextAction(SAction& Action)  = 0;
	virtual ~CAgent() = default;
};

class CAIAgent : public CAgent
{
private:
	unique_ptr<CGameSolver> Solver;
public:
	CAIAgent(CGameSolver* Solver);
	virtual bool IsAI() override;
	virtual void GetActionsWithScore(vector<SAction>& Actions, vector<float>& Scores, vector<int>& VisitCnts) override;
	virtual void GetNextAction(SAction& Action) override;
	virtual ~CAIAgent() = default;
};

class CHumanAgent : public CAgent
{
public:
	virtual bool IsAI() override;
	// virtual void GetActionsWithScore(vector<SAction>& Actions, vector<float>& Scores, vector<int>& VisitCnts) override;
	virtual void GetNextAction(SAction& Action) override;
	virtual ~CHumanAgent() = default;
};