#include <unordered_map>
#include <iostream>
#include <cstdio>
#include "ChinaChess.h"
using namespace std;

bool CChess::FirstPieceTo( int dx, int dy, int x, int y, int& x1, int& y1) const
{
	x = x + dx;
	y = y + dy;
	for (; x >= 0 && x < H&&y >= 0 && y < W;)
	{
		if (Board[x][y] != EPieceType::NIL)
		{
			x1 = x;
			y1 = y;
			return true;
		}
		x += dx;
		y += dy;
	}
	x1 = x;
	y1 = y;
	return false;
}

int32 CChess::GetActions(vector<SAction>& Actions) const
{

	const int DirectionsC[][2] = { 0,1,0,-1,1,0,-1,0 };
	const int DirectionsE[][2] = { 2,2,2,-2,-2,2,-2,-2 };
	const int DirectionsG[][2] = { 1,1,1,-1,-1,1,-1,-1 };
	const int DirectionsH[][2] = { 2,1,2,-1,-2,1,-2,-1, 1,2,-1,-2,1,-2,-1,2 };
	const int DirectionsK[][2] = { 1,0,-1,0,0,1,0,-1 };
	const int DirectionsP[][2] = { 1,0,0,1,0,-1 };
	const int DirectionsR[][2] = { 0,1,0,-1,1,0,-1,0 };
	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			if (Board[i][j] != EPieceType::NIL)
			{
				switch (Board[i][j])
				{
				case R_C:
					// ÅÚ
					for (int k = 0; k < 4; k++)
					{
						int nx, ny;
						bool bIsEnd;
						if (bIsEnd = FirstPieceTo(DirectionsC[k][0], DirectionsC[k][1], i, j, nx, ny))
						{
							int ex, ey;
							if (FirstPieceTo(DirectionsC[k][0], DirectionsC[k][1], nx, ny, ex, ey) && IsBlack(Board[ex][ey]))
							{
								Actions.push_back({ i,j,ex,ey });
							}
						}
						for (int x = i + DirectionsC[k][0], y = j + DirectionsC[k][1]; x != nx || y != ny;)
						{
							Actions.push_back({ i,j,x,y });
							x += DirectionsC[k][0];
							y += DirectionsC[k][1];
						}
					}
					break;
				case R_E:
					// Ïà
					for (int k = 0; k < 4; k++)
					{
						int x = i + DirectionsE[k][0];
						int y = j + DirectionsE[k][1];
						if (IsInBoard(x, y) && IsInRedField(x,y)&& !IsRed(Board[x][y])&& Board[(x+i)/2][(y+j)/2]==EPieceType::NIL)
						{
							Actions.push_back({ i,j,x,y });
						}
					}
					break;
				case R_G:
					// ÊË
					for (int k = 0; k < 4; k++)
					{
						int x = i + DirectionsG[k][0];
						int y = j + DirectionsG[k][1];
						if (IsInKingField(x, y) && !IsRed(Board[x][y]))
						{
							Actions.push_back({ i,j,x,y });
						}
					}
					break;
				case R_H:
					// Âí
					for (int k = 0; k < 8; k++)
					{
						int x = i + DirectionsH[k][0];
						int y = j + DirectionsH[k][1];
						if (IsInBoard(x, y) && !IsRed(Board[x][y]) && Board[ i + DirectionsH[k][0] / 2][ j + DirectionsH[k][1] / 2] == EPieceType::NIL)
						{
							Actions.push_back({ i,j,x,y });
						}
					}
					break;
				case R_K:
					for (int k = 0; k < 4; k++)
					{
						int x = i + DirectionsK[k][0];
						int y = j + DirectionsK[k][1];
						if (IsInKingField(x, y) && !IsRed(Board[x][y]))
						{
							Actions.push_back({ i,j,x,y });
						}
					}
					break;
				case R_P:
					if (IsInRedField(i, j))
					{
						if (!IsRed(Board[i + 1][j]))
						{
							Actions.push_back({ i, j, i + 1, j });
						}
						break;
					}
					for (int k = 0; k < 3; k++)
					{
						int x = i + DirectionsP[k][0];
						int y = j + DirectionsP[k][1];
						if (IsInBoard(x, y) && !IsRed(Board[x][y]))
						{
							Actions.push_back({ i,j,x,y });
						}
					}
					break;
				case R_R:
					for (int k = 0; k < 4; k++)
					{
						int nx, ny;
						bool bIsInBoard;
						bIsInBoard = FirstPieceTo(DirectionsR[k][0], DirectionsR[k][1], i, j, nx, ny);
						for (int x = i + DirectionsR[k][0], y = j + DirectionsR[k][1]; x != nx || y != ny;)
						{
							Actions.push_back({ i,j,x,y });
							x += DirectionsR[k][0];
							y += DirectionsR[k][1];
						}
						if (bIsInBoard && IsBlack(Board[nx][ny]))
						{
							Actions.push_back({ i,j,nx,ny });
						}
					}
					break;
				}
			}
		}
	}
	return static_cast<int>(Actions.size());
}

EPieceType CChess::ExcuteAction(SAction Action)
{
	EPieceType Piece = Board[Action.x1][Action.y1];
	Board[Action.x1][Action.y1] = EPieceType::NIL;
	EPieceType Piece2 = Board[Action.x2][Action.y2];
	Board[Action.x2][Action.y2] = Piece;
	bIsWin = (Piece2 == B_K);
	return Piece2;
}

void CChess::RevertAction(SAction Action, EPieceType TargetPiece)
{
	EPieceType Piece1 = Board[Action.x2][Action.y2];
	Board[Action.x1][Action.y1] = Piece1;
	Board[Action.x2][Action.y2] = TargetPiece;
}

void CChess::RevertBoard()
{
	for (int i = 0; i <= 4; i++) {
		for (int j = 0; j < W; j++) {
			EPieceType Piece = Board[i][j];
			Board[i][j] = RollPiece(Board[H - i - 1][W - j - 1]);
			Board[H - i - 1][W - j - 1] = RollPiece(Piece);
		}
	}
}

string CChess::PackageBoard() const
{
	static const vector<char> PieceToChar = {' ','k','g','e','h','r','c','p','K','G','E','H','R','C','P'};
	string s;
	for (int i = 0; i < H; i++)
	{
		if (i > 0)
		{
			s += ' ';
		}
		int k = 0;
		for (int j = 0; j < W; j++)
		{
			if (Board[i][j] != EPieceType::NIL)
			{
				if (k > 0)
				{
					s += static_cast<char>(k + '0');
					k = 0;
				}
				s += PieceToChar[static_cast<int>(Board[i][j])];
			}
			else
			{
				k++;
			}
		}
		if (k > 0)
		{
			s += static_cast<char>(k + '0');
			k = 0;
		}
	}
	return s;
}

EPieceType CharToPiece(char ch)
{
	static const unordered_map<char, EPieceType>  CtP{ 
		{'k',R_K}, {'c',R_C},{'h',R_H},{'e',R_E},{'g',R_G},{'p',R_P},{'r',R_R} ,
		{'K',B_K}, {'C',B_C},{'H',B_H},{'E',B_E},{'G',B_G},{'P',B_P},{'R',B_R} };
	return CtP.at(ch);
}

void CChess::UnPackage(const string& s)
{
	int i = 0, j = 0;
	for (auto ch : s)
	{
		if (ch == ' ')
		{
			i++;
			j = 0;
		}
		else if (!isdigit(ch))
		{
			Board[i][j] = CharToPiece(ch);
			j++;
		}
		else
		{
			for (int k = j; j < static_cast<int>(ch - '0') + k; j++)
			{
				Board[i][j] = EPieceType::NIL;
			}
		}
	}
}

void CChess::Print()
{
	static const vector<string> PieceToStr {" ", "k", "g", "e", "h", "r", "c", "p", "K", "G", "E", "H", "R", "C", "P"};
	cout << "-----------------" << endl;
	for (int i = H-1; i >=0 ; i--)
	{
		for (int j = 0; j < W; j++)
		{
			cout << PieceToStr[static_cast<int>(Board[i][j])];
			//cout << static_cast<int>(Board[x][y]);
			if (j < W - 1)
			{
				cout << ' ';
			}
		}
		cout << endl;
	}
	cout << "------------------" << endl;
}

CChess::CChess()
{
	UnPackage("rhegkgehr 9 1c5c1 p1p1p1p1p 9 9 P1P1P1P1P 1C5C1 9 RHEGKGEHR");
	Init();
	// std::cout << PackageBoard() << endl;
}

CChess::CChess(const string& s)
{
	UnPackage(s);
	Init();
}

bool CChess::operator==(const CChess& R) const
{
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if (Board[i][j] != R.PieceAt(i, j))
			{
				//cout<<i<<j<<' '<<Board[i][j]<<R.PieceAt(i,j) << endl;
				return false;
			}
		}
	}
	return true;
}

const CActionIdHelper CActionIdHelper::HelperInstance;

vector<uint16> CActionIdHelper::HashVToId;

vector<uint16> CActionIdHelper::IdToHashV;

CActionIdHelper::CActionIdHelper()
{
	int cnt = 0;
	CActionIdHelper::IdToHashV.resize(CChess::H * CChess::H * CChess::W * CChess::W);
	CActionIdHelper::HashVToId.resize(0xffff);
	for (int i = 0; i < CChess::H; i++)
	{
		for (int j = 0; j < CChess::W; j++)
		{
			for (int x = 0; x < CChess::H; x++)
			{
				for (int y = 0; y < CChess::W; y++)
				{
					if ((x == i || y == j || (abs(x-i)==1&&abs(y-j)==2||abs(x-i)==2&&abs(y-j)==1))&&!(x==i&&y==j)) 
					{
						SAction Action{ i,j,x,y };
						CActionIdHelper::HashVToId[Action.ToInt16()] = cnt;
						CActionIdHelper::IdToHashV[cnt] = Action.ToInt16();
						cnt++;
					}
				}
			}
		}
	}
	unsigned Actions[] = {0x0220,0x2042,0x4224,0x2406,0x0628,0x2846,0x4624,0x2402,0x1425,0x1403,0x1405,0x1423};
	for (auto HashV : Actions)
	{
		CActionIdHelper::HashVToId[HashV] = cnt;
		CActionIdHelper::IdToHashV[cnt] = HashV;
		cnt++;
		HashV = ((HashV & 0x00ff) << 8) | ((HashV & 0xff00) >> 8);
		CActionIdHelper::HashVToId[HashV] = cnt;
		CActionIdHelper::IdToHashV[cnt] = HashV;
		cnt++;
	}
	// cout << cnt << endl;
}
