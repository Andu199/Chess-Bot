#include <vector>
#include <map>
#include <vector>
#include <algorithm>
#include <time.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

enum PieceType {
	WhitePawn = 1,
	WhiteKnight = 2,
	WhiteBishop = 3,
	WhiteRook = 4,
	WhiteQueen = 5,
	WhiteKing = 6,

	BlackPawn = -1,
	BlackKnight = -2,
	BlackBishop = -3,
	BlackRook = -4,
	BlackQueen = -5,
	BlackKing = -6
};

struct Piece {
	char pieceType;
	int noMoves;
	int pieceValue = 0;

	Piece(char pT, int nM) {
		pieceType = pT;
		noMoves = nM;
		pieceValue = value();
	}

	int value() {
		switch(pieceType) {
			case WhitePawn:
			case BlackPawn:
				return 1;
			case WhiteKnight:
			case BlackKnight:
				return 3;
			case WhiteBishop:
			case BlackBishop:
				return 3;
			case WhiteRook:
			case BlackRook:
				return 5;
			case WhiteQueen:
			case BlackQueen:
				return 9;

			case WhiteKing:
			case BlackKing:
				return 1000;
		}
		return 0;
	}

	Piece() {}
};

class Board {
private:
	Piece **board;
	map<pair<char, char>, Piece> *whitePieces;
	map<pair<char, char>, Piece> *blackPieces;
	
	map<pair<char, char>, Piece>* verifyForPawn(pair<char, char> king_position,
		map<pair<char, char>, Piece> *enemy_pieces);
	map<pair<char, char>, Piece>* verifyForKnight(pair<char, char> king_position,
		map<pair<char, char>, Piece> *enemy_pieces);
	map<pair<char, char>, Piece>* verifyForRBQ(pair<char, char> king_position, int color);
	map<pair<char, char>, Piece>* verifyForKing(pair<char, char> king_position,
		map<pair<char, char>, Piece> *enemy_pieces);

	bool verifyBlockRook(pair<char, char> attackPos, pair<char, char> kingPos,
		pair<char, char> defendPos);

	bool verifyBlockBishop(pair<char, char> attackPos, pair<char, char> kingPos,
		pair<char, char> defendPos);

	map<pair<char, char>, Piece>* verifyIfCheck( map<pair<char, char>, Piece>* attackPieces,
		pair<pair<char, char>, pair<char, char>>* move, bool isWhite);
	
public:
	Board();
	~Board();
	void InitializeBoard();

	pair<pair<char, char>, pair<char, char>> whiteLastMove = {{-1, -1}, {-1, -1}};
	pair<pair<char, char>, pair<char, char>> blackLastMove = {{-1, -1}, {-1, -1}};

	Piece** getBoard();
	map<pair<char, char>, Piece>* getWhitePieces();
	map<pair<char, char>, Piece>* getBlackPieces();

	void PrintBoard();
	vector<pair<pair<char, char>, pair<char, char>>>* GetAllLegalMoves(bool isWhite);
	
	vector<pair<pair<char, char>, pair<char, char>>>* GetLegalPawnMoves(pair<pair<char, char>, Piece> piece, map<pair<char, char>, Piece> *attack_pieces);
	vector<pair<pair<char, char>, pair<char, char>>>* GetLegalKnightMoves(pair<pair<char, char>, Piece> piece, map<pair<char, char>, Piece> *attack_pieces);
	vector<pair<pair<char, char>, pair<char, char>>>* GetLegalBishopMoves(pair<pair<char, char>, Piece> piece, map<pair<char, char>, Piece> *attack_pieces);
	vector<pair<pair<char, char>, pair<char, char>>>* GetLegalRookMoves(pair<pair<char, char>, Piece> piece, map<pair<char, char>, Piece> *attack_pieces);
	vector<pair<pair<char, char>, pair<char, char>>>* GetLegalQueenMoves(pair<pair<char, char>, Piece> piece, map<pair<char, char>, Piece> *attack_pieces);
	vector<pair<pair<char, char>, pair<char, char>>>* GetLegalKingMoves(pair<pair<char, char>, Piece> piece, map<pair<char, char>, Piece> *attack_pieces);
	void MakeMoveOnBoard(pair<pair<char, char>, pair<char, char>> actMove, bool isWhite, int newPiece);

	bool VerifyPosition(pair<char, char> position);

	void PrintLegalMoves(vector<pair<pair<char, char>, pair<char, char>>>* moves);
	int evaluateMaterial(bool isWhite);
	int evaluateSpaces(bool isWhite);
	int evaluateIfCheck(bool isWhite);
	int evaluatePawnStruct(bool isWhite);
	int evaluate(bool isWhite);
};