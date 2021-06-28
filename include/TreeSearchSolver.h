#pragma once
#include "GameSolver.h"
#include <memory>
class CInferencer;
class CTreeNode
{
public:
	CChess Chess;
	vector<unique_ptr<CTreeNode>> Childs;
	vector<SAction> Actions;
	vector<float> Scores;
	vector<float> Qvalues;
	vector<float> Predicts;
	vector<int> VisitCnts;
	CTreeNode* Parant;
	float Qvalue;
	bool bIsFinished;
	bool bIsRed;
public:
	CTreeNode(CChess& Chess);
	CTreeNode(vector<SAction>& Actions, vector<float>& Predicts, CChess& Chess);
	CTreeNode(vector<SAction>&& Actions, vector<float>&& Predicts, CChess& Chess);
	void InitNode();

	inline CChess& GetChess()
	{
		return Chess;
	}

	inline void SetParant(CTreeNode* _Parant_)
	{
		Parant = _Parant_;
	}
	inline CTreeNode* GetParanet() const
	{
		return Parant;
	}
	inline float GetQvalue() const
	{
		return Qvalue;
	}
	inline void SetQvalue(float Q)
	{
		Qvalue = Q;
	}
	inline bool IsFinished()
	{
		return bIsFinished;
	}
	inline void Finish()
	{
		bIsFinished = true;
	}
	inline bool IsRed()
	{
		return bIsRed;
	}
	inline vector<SAction>& GetActions()
	{
		return Actions;
	}
	inline vector<float>& GetScores()
	{
		return Scores;
	}
	inline vector<float>& GetQvalues()
	{
		return Qvalues;
	}
	inline vector<float>& GetPredicts()
	{
		return Predicts;
	}
	inline vector<int>& GetVisitCnts()
	{
		return VisitCnts;
	}
	inline SAction GetActionAt(int Index)
	{
		return Actions[Index];
	}
	inline CTreeNode* GetChildAt(int Index)
	{
		return Childs[Index].get();
	}
	int SelectNextActionId();
	void VisitActionAt(int Index);
	void UpdateActionQvalueAt(int Index, float Qvalue);
	void BackwardActionStep(int Index, float Qvalue);
	void ExpandNodeByActionAt(int Index, CTreeNode* NewNode);
};

class CSearchHelper
{
private:
	CSearchHelper() = delete;
public:
	CTreeNode* Node;
	vector<int> SelectionIndexs;
public:
	inline CSearchHelper(CTreeNode* Node) : Node(Node)
	{

	};
	inline CChess& GetChess()
	{
		return Node->GetChess();
	}
	inline CTreeNode* GetNode()
	{
		return Node;
	}
	inline void SetNode(CTreeNode* _Node_)
	{
		Node = _Node_;
	}
	inline SAction GetLastSeletionAction()
	{
		return Node->GetActionAt(SelectionIndexs.back());
	}
	inline vector<int>& GetActionIndexs()
	{
		return SelectionIndexs;
	}
	inline int GetLastSeletionIndex()
	{
		return SelectionIndexs.back();
	}
};

class CSearchTree
{
public:
	unique_ptr<CTreeNode> Root;
public:
	inline CSearchTree(CTreeNode* Root) : Root(Root)
	{
	};
	inline CTreeNode* GetRoot() const
	{
		return Root.get();
	}
	void Search(CSearchHelper* Helper);
	void Backward(CSearchHelper* Helper,float NewQvalue);
	void Expand(CSearchHelper* Helper, CTreeNode* NewNode);
	unique_ptr<CSearchHelper> BuildSearchHelper();
};

class CTreeSearchSolver : public CGameSolver
{
private:
	unique_ptr<CInferencer> Inferencer;
	int N;
public:
	CTreeSearchSolver(CInferencer * Inferencer, int N = 50000);
	virtual void Solve(CChess* Chess, vector<SAction>& Actions, vector<float>& Scores, vector<int>& VisitCnts) override;
};
