#include "Inferencer.h"
#include "ChinaChess.h"

float CSimpleInferencer::GetQvalueOf(CChess* Chess)
{
	// {' ','k','g','e','h','r','c','p','K','G','E','H','R','C','P'}
	static const int ValuesOfPieces[] = { 0,100,3,3,5,10,60,3,-100,-3,-3,-5,-10,-60,-3 };
	static const float C = 30.0f;
	int Qvalue = 0;
	for (int i = 0; i < CChess::H; i++)
	{
		for (int j = 0; j < CChess::W; j++)
		{
			Qvalue += ValuesOfPieces[TFPieceTo<int>(Chess->PieceAt(i, j))];
		}
	}
	return static_cast<float>(Qvalue) / C;
}

void CSimpleInferencer::Inference(CChess* Chess, vector<SAction>& Actions, vector<float>& Predicts, float& QValue)
{
	QValue = GetQvalueOf(Chess);
	Predicts.resize(Actions.size());
	for (size_t i = 0;i<Actions.size();i++)
	{
		SAction Action = Actions[i];
		EPieceType TargetPiece = Chess->ExcuteAction(Action);
		Predicts[i] = GetQvalueOf(Chess);
		Chess->RevertAction(Action, TargetPiece);
	}
}