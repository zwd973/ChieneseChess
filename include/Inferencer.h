#pragma once
#include <vector>
using namespace  std;
class CChess;
typedef struct _SAction_ SAction;
class CInferencer
{
public:
	// Some static helper function
	static void FromActionsToIds(vector<SAction>& Actions, vector<SAction>& ActionIds);
	static void FromIdsToActions(vector<SAction>& Actions, vector<SAction>& ActionIds);
public:
	virtual float GetQvalueOf(CChess* Chess) { return 0; };
	virtual void Inference(CChess* Chess, vector<SAction>& Actions, vector<float>& Predicts, float& QValue) = 0;
};

class CNetInferencer : public CInferencer
{
public:
	virtual void Inference(CChess* Chess, vector<SAction>& Actions, vector<float>& Predicts, float& QValue) override;
};

class CSimpleInferencer : public CInferencer
{
public:
	virtual float GetQvalueOf(CChess* Chess);
	virtual void Inference(CChess* Chess, vector<SAction>& Actions, vector<float>& Predicts, float& QValue) override;
};