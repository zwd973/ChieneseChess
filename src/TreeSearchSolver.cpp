#include "TreeSearchSolver.h"
#include "Inferencer.h"
#include <algorithm>
#include <iostream>
#include <assert.h>
#include <cmath>
CTreeNode::CTreeNode(CChess& Chess) :Chess(Chess),bIsFinished(false)
{

}

CTreeNode::CTreeNode(vector<SAction>& Actions,vector<float>& Predicts, CChess& Chess) : Actions(Actions), Predicts(Predicts), Chess(Chess)
{
	InitNode();
}

CTreeNode::CTreeNode(vector<SAction>&& Actions, vector<float>&& Predicts, CChess& Chess) : Actions(Actions), Predicts(Predicts), Chess(Chess)
{
	InitNode();
}

void CTreeNode::InitNode()
{
	Childs.resize(Actions.size());
	Scores.resize(Actions.size());
	for (size_t i = 0;i<Scores.size();i++)
	{
		Scores[i] = (Predicts[i] + 1.0);
	}
	Qvalues.resize(Actions.size());
	VisitCnts.resize(Actions.size());
	bIsFinished = false;
}

int CTreeNode::SelectNextActionId()
{
	return max_element(Scores.begin(),Scores.end()) - Scores.begin();
}

void CTreeNode::VisitActionAt(int Index)
{
	VisitCnts[Index] ++;
}

void CTreeNode::UpdateActionQvalueAt(int Index, float Qvalue)
{
	Qvalues[Index] += Qvalue;
}

void CTreeNode::ExpandNodeByActionAt(int Index, CTreeNode* NewNode)
{
	Childs[Index].reset(NewNode);
}

void CTreeNode::BackwardActionStep(int Index, float Qvalue)
{
	// VisitCnts[Index] ++;
	float Alpha = 1.0f, Beta = 2.0f, Gamma = 1.0f;
	Qvalues[Index] += Qvalue;
	float U = 0;
	float Q = 0;
	if (VisitCnts[Index])
	{
		Q = Qvalues[Index] / VisitCnts[Index] * Alpha;
	}
	U = Beta * (Predicts[Index] + 1.0) / (1 + pow(VisitCnts[Index], Gamma));
	Scores[Index] = Q + U;
	// to update scores;
}

unique_ptr<CSearchHelper> CSearchTree::BuildSearchHelper()
{
	return make_unique<CSearchHelper>(Root.get());
}

void CSearchTree::Search(CSearchHelper* Helper)
{
	while (true)
	{
		//Helper->GetChess().Print();
		CTreeNode* Node = Helper->GetNode();
		if (Node->IsFinished())
		{
			Helper->SetNode(Node->GetParanet());
			return;
		}
		int Index = Node->SelectNextActionId();
		Helper->SelectionIndexs.push_back(Index);
		Node->VisitActionAt(Index);
		if (Node->GetChildAt(Index))
		{
			Helper->SetNode(Node->GetChildAt(Index));
		}
		else
		{
			break;
		}
	}
}

void CSearchTree::Expand(CSearchHelper* Helper, CTreeNode* NewNode)
{
	Helper->GetNode()->ExpandNodeByActionAt(Helper->GetLastSeletionIndex(), NewNode);
}

void CSearchTree::Backward(CSearchHelper* Helper, float Qvalue)
{
	CTreeNode* Node = Helper->GetNode();
	Helper->SetNode(nullptr);
	while (Node)
	{
		Qvalue = -Qvalue;
		Node->BackwardActionStep(Helper->SelectionIndexs.back(), Qvalue);
		Node = Node->GetParanet();
		Helper->SelectionIndexs.pop_back();
	}
}

CTreeSearchSolver::CTreeSearchSolver(CInferencer* Inferencer, int N) : Inferencer(Inferencer), N(N)
{

}

void CTreeSearchSolver::Solve(CChess* Chess, vector<SAction>& Actions, vector<float>& Scores, vector<int>& VisitCnts)
{
	vector<SAction> CurActions;
	vector<float> Predicts;
	float Qvalue;
	Chess->GetActions(CurActions);
	Inferencer->Inference(Chess, CurActions, Predicts, Qvalue);
	CSearchTree Tree(new CTreeNode(CurActions, Predicts, *Chess));
	Tree.GetRoot()->SetParant(NULL);
	for (int i = 0; i < N; i++)
	{
		unique_ptr<CSearchHelper> Helper = Tree.BuildSearchHelper();
		// cout << "Start" << endl;
		Tree.Search(Helper.get());

		CTreeNode* NewNode = new CTreeNode(Helper->GetChess());
		float NewQvalue;
		CChess& NewChess = NewNode->GetChess();
		NewNode->SetParant(Helper->GetNode());
		NewChess.ExcuteAction(Helper->GetLastSeletionAction());
		NewChess.RevertBoard();
		if (NewChess.IsWin())
		{
			// a final leaf node
			NewQvalue = -1;
			NewNode->Finish();
		}
		else
		{
			NewChess.GetActions(NewNode->GetActions());
			Inferencer->Inference(&NewChess, NewNode->GetActions(), NewNode->GetPredicts(), NewQvalue);
			NewNode->InitNode();
			Tree.Expand(Helper.get(), NewNode);
		}
		Tree.Backward(Helper.get(),NewQvalue);
	}

	vector<int>& RootVisitCnts = Tree.GetRoot()->GetVisitCnts();
	auto Comp = [&RootVisitCnts](int l, int r) {
		return RootVisitCnts[l] > RootVisitCnts[r];
	};
	vector<int> Indexs(CurActions.size());
	for (size_t i = 0; i < Indexs.size(); i++)
	{
		Indexs[i] = i;
	}
	sort(Indexs.begin(), Indexs.end(), Comp);
	Scores.resize(Indexs.size());
	Actions.resize(Indexs.size());
	VisitCnts.resize(Indexs.size());
	for (int i = 0; i < Indexs.size(); i++)
	{
		Scores[i] = static_cast<float>(RootVisitCnts[Indexs[i]]) / N;
		Actions[i] = CurActions[Indexs[i]];
		VisitCnts[i] = RootVisitCnts[Indexs[i]];
	}
}
