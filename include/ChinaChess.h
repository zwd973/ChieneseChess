#pragma once
#include "DataType.h"
#include <vector>
#include <string>
using namespace std;

enum EPieceType
{
	NIL = 0,
	R_K = 1,
	R_G = 2,
	R_E = 3,
	R_H = 4,
	R_R = 5,
	R_C = 6,
	R_P = 7,

	B_K = 8,
	B_G = 9,
	B_E = 10,
	B_H = 11,
	B_R = 12,
	B_C = 13,
	B_P = 14,
};

template <class T>
inline T TFPieceTo(EPieceType Piece)
{
	return static_cast<T>(Piece);
}

inline EPieceType RollPiece(EPieceType Piece)
{
	int P = static_cast<int>(Piece);
	return static_cast<EPieceType>(P ? (P > 7 ? P - 7 : P + 7) : P);
}

inline bool IsRed(EPieceType Piece)
{
	int P = static_cast<int> (Piece);
	return P >= 1 && P <= 7;
}

inline bool IsBlack(EPieceType Piece)
{
	int P = static_cast<int> (Piece);
	return P > 7;
}

class CActionIdHelper
{
public:
	static const CActionIdHelper HelperInstance;
	static vector<uint16> HashVToId;
	static vector<uint16> IdToHashV;
	CActionIdHelper();
	static inline uint16 GetId(uint16 HashV) 
	{
		return HashVToId[HashV];
	};
	static inline uint16 GetHashVById(uint16 Id) 
	{
		return IdToHashV[Id];
	};
};

typedef struct _SAction_
{
	int x1, y1, x2, y2;
	inline uint32 ToInt32()
	{
		return x1 << 24 | y1 << 16 | x2 << 8 | y2;
	};
	inline uint16 ToInt16()
	{
		return x1 << 12 | y1 << 8 | x2 << 4 | y2;
	}
	inline uint16 GetId()
	{
		uint16 HashV = ToInt16();
		return CActionIdHelper::GetId(HashV);
	}
	inline struct _SAction_ GetRevertedAction()
	{
		return { x2,y2,x1,y1 };
	}
	inline static struct _SAction_ FromId(uint16)
	{
		uint16 HashV = CActionIdHelper::GetHashVById(HashV);
		return { (HashV & 0xf000) >> 12,(HashV & 0x0f00) >> 8 ,(HashV & 0x00f0) >> 4 ,(HashV & 0x000f) };
	}
	inline bool IsLegal()
	{
		return x1 >= 0 && x1 < 10 && y1 >= 0 && y1 < 9 && x2 >=0 && x2 <10 && y2 >=0 && y2 < 9;
	}
	/*inline static SAction FromInt32(uint32 Action)
	{
		return { Action >> 24,(Action & 0x00ff0000 >> 16),(Action & 0x0000ff00) >> 8,Action & 0x000000ff };
	};*/
} SAction;

class CChess
{
public:
	static const int32 W = 9;
	static const int32 H = 10;
private:
	EPieceType Board[H][W];
	bool bIsWin;
public:
	CChess() ;
	CChess(const string& s); 
	inline void Init()
	{
		bIsWin = false;
	};
	inline bool IsWin() const
	{
		return bIsWin;
	}
	bool FirstPieceTo(int dx, int dy, int x, int y, int& x1, int& y1) const;
	inline bool IsInBoard(int x, int y) const
	{
		return x >= 0 && x < H&&y >= 0 && y < W;
	};
	inline bool IsInKingField(int x, int y) const
	{
		return x >= 0 && x <=2&&y >= 3 && y <=5;
	};
	inline bool IsInRedField(int x, int y) const
	{
		return x <= 4;
	};
	int32 GetActions(vector<SAction>& Actions) const;
	EPieceType ExcuteAction(SAction Action);
	void RevertAction(SAction Action, EPieceType TargetPiece);
	void RevertBoard();
	//CChess NextBoard(SAction Action);
	//CChess GetRevertedBoard();
	string PackageBoard() const;
	void UnPackage(const string& s);
	inline EPieceType PieceAt(int x, int y) const
	{
		return Board[x][y];
	};
	inline void SetPieceAt(int x, int y, EPieceType Piece)
	{
		Board[x][y] = Piece;
	};
	void Print();
	bool operator==(const CChess& R) const;
};
