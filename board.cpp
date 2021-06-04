#include "board.h"

using namespace std;

Board::Board()
{
	board = new (Piece*[8])();

	for(int i = 0; i < 8; i++) {
		board[i] = new (Piece[8])();
		for(int j = 0; j < 8; j++) {
			board[i][j] = Piece(0, 0);
		}
	}

	whitePieces = new (map<pair<char, char>, Piece>)();
	blackPieces = new (map<pair<char, char>, Piece>)();

	InitializeBoard();
}

Board::~Board() {
	for(int i = 0; i < 8; ++i)
		delete board[i];
	delete board;
	delete whitePieces;
	delete blackPieces;
}

void Board::InitializeBoard()
{
	board[0][0] = board[0][7] = Piece(PieceType::WhiteRook, 0);
	whitePieces->insert({{0, 0}, Piece(PieceType::WhiteRook, 0)});
	whitePieces->insert({{0, 7}, Piece(PieceType::WhiteRook, 0)});

	board[0][1] = board[0][6] = Piece(PieceType::WhiteKnight, 0);
	whitePieces->insert({{0, 1}, Piece(PieceType::WhiteKnight, 0)});
	whitePieces->insert({{0, 6}, Piece(PieceType::WhiteKnight, 0)});

	board[0][2] = board[0][5] = Piece(PieceType::WhiteBishop, 0);
	whitePieces->insert({{0, 2}, Piece(PieceType::WhiteBishop, 0)});
	whitePieces->insert({{0, 5}, Piece(PieceType::WhiteBishop, 0)});

	board[0][3] = Piece(PieceType::WhiteQueen, 0);
	whitePieces->insert({{0, 3}, Piece(PieceType::WhiteQueen, 0)});

	board[0][4] = Piece(PieceType::WhiteKing, 0);
	whitePieces->insert({{0, 4}, Piece(PieceType::WhiteKing, 0)});

	for(int i = 0; i < 8; i++)
	{
		board[1][i] = Piece(PieceType::WhitePawn, 0);
		whitePieces->insert({{1, i}, Piece(PieceType::WhitePawn, 0)});
	}

	board[7][0] = board[7][7] = Piece(PieceType::BlackRook, 0);
	blackPieces->insert({{7, 7}, Piece(PieceType::BlackRook, 0)});
	blackPieces->insert({{7, 0}, Piece(PieceType::BlackRook, 0)});

	board[7][1] = board[7][6] = Piece(PieceType::BlackKnight, 0);
	blackPieces->insert({{7, 1}, Piece(PieceType::BlackKnight, 0)});
	blackPieces->insert({{7, 6}, Piece(PieceType::BlackKnight, 0)});

	board[7][2] = board[7][5] = Piece(PieceType::BlackBishop, 0);
	blackPieces->insert({{7, 2}, Piece(PieceType::BlackBishop, 0)});
	blackPieces->insert({{7, 5}, Piece(PieceType::BlackBishop, 0)});

	board[7][4] = Piece(PieceType::BlackKing, 0);
	blackPieces->insert({{7, 4}, Piece(PieceType::BlackKing, 0)});

	board[7][3] = Piece(PieceType::BlackQueen, 0);
	blackPieces->insert({{7, 3}, Piece(PieceType::BlackQueen, 0)});

	for(int i = 0; i < 8; i++)
	{
		board[6][i] = Piece(PieceType::BlackPawn, 0);
		blackPieces->insert({{6, i}, Piece(PieceType::BlackPawn, 0)});
	}
}

Piece **Board::getBoard() {
	return board;
}

map<pair<char, char>, Piece>* Board::getWhitePieces() {
	return whitePieces;
}

map<pair<char, char>, Piece>* Board::getBlackPieces() {
	return blackPieces;
}

void Board::PrintBoard() {
	int value;
	FILE *out = fopen("out.txt", "w");

	fprintf(out, "Board:\n");
	for(int i = 7; i >= 0; i--)
	{
		for(int j = 0; j < 8; j++)
		{
			value = board[i][j].pieceType;
			if(value >= 0)
				fprintf(out, " %d,%d ", board[i][j].pieceType, board[i][j].noMoves);
			else
				fprintf(out, "%d,%d ", board[i][j].pieceType, board[i][j].noMoves);
		}
		fprintf(out, "\n");
	}

	fprintf(out, "\nWhite Pieces:\n");
	for(auto it = whitePieces->begin(); it != whitePieces->end(); it++)
	{
		fprintf(out, "(%d, %d) -> %d, %d | ", it->first.first, it->first.second,
		 it->second.pieceType, it->second.noMoves);
	}
	fprintf(out, "\n");

	fprintf(out, "\nBlack Pieces:\n");
	for(auto it = blackPieces->begin(); it != blackPieces->end(); it++)
	{
		fprintf(out, "(%d, %d) -> %d, %d | ", it->first.first, it->first.second,
		 it->second.pieceType, it->second.noMoves);
	}
	fprintf(out, "\n");
	fflush(out);
	fclose(out);
}

map<pair<char, char>, Piece>* Board::verifyForPawn(pair<char, char> king_position,
 map<pair<char, char>, Piece> *enemy_pieces) {

	map<pair<char, char>, Piece>* result = new map<pair<char, char>, Piece>();
	int dx[] = {-1, 1}, dy[] = {-1, 1};

	for(int i = 0; i < 2; ++i)
		for(int j = 0; j < 2; ++j) {
			auto piece = enemy_pieces->find({king_position.first + dx[i],
				king_position.second + dy[j]});

			if(piece == enemy_pieces->end())
				continue;
			if(piece->second.pieceType == PieceType::BlackPawn && piece->first
			 .first < king_position.first)
				continue;
			if(piece->second.pieceType == PieceType::WhitePawn && piece->first
			 .first > king_position.first)
				continue;

			if(piece->second.pieceType == PieceType::WhitePawn ||
				piece->second.pieceType == PieceType::BlackPawn)
				if(piece->second.pieceType * board[(int)king_position.first]
				 [(int)king_position.second].pieceType < 0)
					result->insert(*piece);
		}
	return result;
}

map<pair<char, char>, Piece>* Board::verifyForKnight(pair<char, char> king_position,
 map<pair<char, char>, Piece> *enemy_pieces) {

	map<pair<char, char>, Piece>* result = new map<pair<char, char>, Piece>();
	int dx[] = {-2, -1, 1, 2, 2, 1, -1, -2}, dy[] = {-1, -2, -2, -1, 1, 2, 2, 1};

	for(int i = 0; i < 8; ++i) {
		auto piece = enemy_pieces->find({king_position.first + dx[i],
		king_position.second + dy[i]});

		if(piece == enemy_pieces->end())
			continue;

		if((piece->second.pieceType * board[(int) king_position.first][(int) king_position
		 .second].pieceType < 0) &&
			(piece->second.pieceType == PieceType::BlackKnight || piece->second.pieceType
			 == PieceType::WhiteKnight))
		result->insert(*piece);
	}
	return result;
}

map<pair<char, char>, Piece>* Board::verifyForRBQ(pair<char, char> king_position,
 int color) {

	map<pair<char, char>, Piece>* result = new map<pair<char, char>, Piece>();
	int dx[] = {1, 0, -1, 0, 1, -1, -1, 1}, dy[] = {0, 1, 0, -1, 1, 1, -1, -1};

	for(int i = 0; i < 8; ++i) {
		int x = king_position.first;
		int y = king_position.second;

		while(1) {
			x += dx[i], y += dy[i];
			if(x < 0 || x > 7 || y < 0 || y > 7)
				break;

			if(board[x][y].pieceType != 0) {
				if(board[x][y].pieceType * color < 0) {
					if(board[x][y].pieceType == PieceType::BlackQueen ||
						board[x][y].pieceType == PieceType::WhiteQueen)
						result->insert({{x, y}, board[x][y]});
					else if((board[x][y].pieceType == PieceType::BlackRook ||
						board[x][y].pieceType == PieceType::WhiteRook) && i < 4)
						result->insert({{x, y}, board[x][y]});
					else if((board[x][y].pieceType == PieceType::BlackBishop ||
						board[x][y].pieceType == PieceType::WhiteBishop) && i >= 4)
						result->insert({{x, y}, board[x][y]});
				}
				break;
			}
		}
	}
	return result;
}

map<pair<char, char>, Piece>* Board::verifyForKing(pair<char, char> king_position,
 map<pair<char, char>, Piece> *enemy_pieces) {

	map<pair<char, char>, Piece>* result = new map<pair<char, char>, Piece>();
	int dx[] = {1, 1, 1, 0, -1, -1, -1, 0}, dy[] = {-1, 0, 1, 1, 1, 0, -1, -1};

	for(int i = 0; i < 8; ++i) {
		auto piece = enemy_pieces->find({king_position.first + dx[i],
		king_position.second + dy[i]});

		if(piece == enemy_pieces->end())
			continue;
		if((piece->second.pieceType * board[(int) king_position.first][(int)
		 king_position.second].pieceType < 0) && (piece->second.pieceType ==
		 PieceType::BlackKing || piece->second.pieceType == PieceType::WhiteKing))
			result->insert(*piece);
	}
	return result;
}

bool Board::verifyBlockRook(pair<char, char> attackPos, pair<char, char> kingPos,
 pair<char, char> defendPos) {

	if(attackPos.first == kingPos.first) {
		if(defendPos.first != kingPos.first)
			return false;
		if(attackPos.second < kingPos.second) {
			if(defendPos.second < attackPos.second)
				return false;
			else if(defendPos.second > kingPos.second)
				return false;
			return true;
		}
		else if(attackPos.second > kingPos.second) {
			if(defendPos.second > attackPos.second)
				return false;
			else if(defendPos.second < kingPos.second)
				return false;
			return true;
		}
	}
	else {
		if(defendPos.second != kingPos.second)
			return false;
		if(attackPos.first < kingPos.first) {
			if(defendPos.first < attackPos.first)
				return false;
			else if(defendPos.first > kingPos.first)
				return false;
			return true;
		}
		else if(attackPos.first > kingPos.first) {
			if(defendPos.first > attackPos.first)
				return false;
			else if(defendPos.first < kingPos.first)
				return false;
			return true;
		}
	}
	return false;
}

bool Board::verifyBlockBishop(pair<char, char> attackPos, pair<char, char> kingPos,
 pair<char, char> defendPos) {
	 
	if(abs(kingPos.first - defendPos.first) != abs(kingPos.second - defendPos.second))
		return false;
	if(kingPos.first > attackPos.first && kingPos.second > attackPos.second) {
		if(kingPos.first > defendPos.first && kingPos.second > defendPos.second &&
			attackPos.first <= defendPos.first && attackPos.second <= defendPos.second)
			return true;
	}
	else if(kingPos.first > attackPos.first && kingPos.second < attackPos.second) {
		if(kingPos.first > defendPos.first && kingPos.second < defendPos.second &&
			attackPos.first <= defendPos.first && attackPos.second >= defendPos.second)
			return true;
	}
	else if(kingPos.first < attackPos.first && kingPos.second < attackPos.second) {
		if(kingPos.first < defendPos.first && kingPos.second < defendPos.second &&
			attackPos.first >= defendPos.first && attackPos.second >= defendPos.second)
			return true;
	}
	else if(kingPos.first < attackPos.first && kingPos.second > attackPos.second) {
		if(kingPos.first < defendPos.first && kingPos.second > defendPos.second &&
			attackPos.first >= defendPos.first && attackPos.second <= defendPos.second)
			return true;
	}
	return false;
}

map<pair<char, char>, Piece>* Board::verifyIfCheck( map<pair<char, char>, Piece>*
 attackPieces, pair<pair<char, char>, pair<char, char>>* move, bool isWhite) {
	
	pair<char, char> king_position;
	map<pair<char, char>, Piece> *act_pieces;
	map<pair<char, char>, Piece> *enemy_pieces;
	map<pair<char, char>, Piece> *result = new map<pair<char, char>, Piece>();
	map<pair<char, char>, Piece> *aux;
	int color;

	if(isWhite) {
		act_pieces = whitePieces;
		enemy_pieces = blackPieces;
		color = 1;
	}
	else {
		act_pieces = blackPieces;
		enemy_pieces = whitePieces;
		color = -1;
	}

	for(auto piece =  act_pieces->begin(); piece != act_pieces->end(); piece++)
		if(piece->second.pieceType == PieceType::WhiteKing || piece->second.pieceType
		 == PieceType::BlackKing) {
			king_position = piece->first;
			break;
		}

	if(move == NULL) {
		aux = verifyForPawn(king_position, enemy_pieces);
		result->insert(aux->begin(), aux->end());
		delete aux;
		aux = verifyForKnight(king_position, enemy_pieces);
		result->insert(aux->begin(), aux->end());
		delete aux;
		aux = verifyForRBQ(king_position, color);
		result->insert(aux->begin(), aux->end());
		delete aux;
		aux = verifyForKing(king_position, enemy_pieces);
		result->insert(aux->begin(), aux->end());
		delete aux;
	} else {
		int dx = move->first.first - king_position.first;
		int dy = move->first.second - king_position.second;

		if(abs(dx) == abs(dy) || dx == 0 || dy == 0) {
			if(dx)
				dx = dx / abs(dx);
			if(dy)
				dy = dy / abs(dy);
			int x = king_position.first + dx; int y = king_position.second + dy;

			while(x >= 0 && x <= 7 && y >= 0 && y <= 7) {
				if(x == move->first.first && y == move->first.second) {
					x += dx;
					y += dy;
					continue;
				}
				if(x == move->second.first && y == move->second.second)
					break;
				if(board[x][y].pieceType * color > 0)
					break;
				if(board[x][y].pieceType * color < 0) {

					if(abs(dx) == abs(dy) && (board[x][y].pieceType == PieceType::
					 WhiteBishop || board[x][y].pieceType == PieceType::BlackBishop
					 || board[x][y].pieceType == PieceType::WhiteQueen || board[x][y]
					 .pieceType == PieceType::BlackQueen)) {

						result->insert({{x, y}, board[x][y]});
						return result;
					} else if(board[x][y].pieceType == PieceType::WhiteRook ||
					 board[x][y].pieceType == PieceType::BlackRook
					 || board[x][y].pieceType == PieceType::WhiteQueen || board[x][y]
					 .pieceType == PieceType::BlackQueen) {
						result->insert({{x, y}, board[x][y]});
						return result;
					}
				}
				x += dx;
				y += dy;
			}
		}

		for(auto it = attackPieces->begin(); it != attackPieces->end(); it++) {
			if(it->first.first == move->second.first && it->first.second == move->second.second)
				continue;
			if(it->second.pieceType == PieceType::BlackPawn || it->second.pieceType == PieceType::WhitePawn ||
				it->second.pieceType == PieceType::BlackKnight || it->second.pieceType == PieceType::WhiteKnight) {
				if(it->first.first != move->second.first ||
					it->first.second != move->second.second) {
						result->insert(*it);
						break;
				}
			}
			else if(it->second.pieceType == PieceType::BlackRook || it->second.pieceType == PieceType::WhiteRook) {
				if(!verifyBlockRook(it->first, king_position, move->second)) {
					result->insert(*it);
					break;
				}
			}
			else if(it->second.pieceType == PieceType::BlackBishop || it->second.pieceType == PieceType::WhiteBishop) {
				if(!verifyBlockBishop(it->first, king_position, move->second)) {
					result->insert(*it);
					break;
				}
			}
			else if(it->second.pieceType == PieceType::BlackQueen || it->second.pieceType == PieceType::WhiteQueen) {
				if(it->first.first == king_position.first || it->first.second == king_position.second) {
					if(!verifyBlockRook(it->first, king_position, move->second)) {
						result->insert(*it);
						break;
					}
				} else {
					if(!verifyBlockBishop(it->first, king_position, move->second)) {
						result->insert(*it);
						break;
					}
				}
			}
		}
	}
	return result;
}

vector<pair<pair<char, char>, pair<char, char>>>* Board::GetAllLegalMoves(bool isWhite)
{
	vector<pair<pair<char, char>, pair<char, char>>> *moves = new vector<pair
	 <pair<char, char>, pair<char, char>>>;
	vector<pair<pair<char, char>, pair<char, char>>> *aux;

	map<pair<char, char>, Piece> *act_pieces;
	map<pair<char, char>, Piece> *attack_pieces = verifyIfCheck(NULL, NULL, isWhite);

	if(isWhite) {
		act_pieces = this->whitePieces;
	} else {
		act_pieces = this->blackPieces;
	}

	for(auto it = act_pieces->begin(); it != act_pieces->end(); it++) {
		switch (it->second.pieceType)
		{
		case PieceType::WhitePawn:
		case PieceType::BlackPawn:
			aux = GetLegalPawnMoves(*it, attack_pieces);
			moves->insert(moves->end(), aux->begin(), aux->end());
			delete aux;
			break;

		case PieceType::WhiteRook:
		case PieceType::BlackRook:
			aux = GetLegalRookMoves(*it, attack_pieces);
			moves->insert(moves->end(), aux->begin(), aux->end());
			delete aux;
			break;
		
		case PieceType::WhiteBishop:
		case PieceType::BlackBishop:
			aux = GetLegalBishopMoves(*it, attack_pieces);
			moves->insert(moves->end(), aux->begin(), aux->end());
			delete aux;
			break;

		case PieceType::WhiteKnight:
		case PieceType::BlackKnight:
			aux = GetLegalKnightMoves(*it, attack_pieces);
			moves->insert(moves->end(), aux->begin(), aux->end());
			delete aux;
			break;

		case PieceType::WhiteQueen:
		case PieceType::BlackQueen:
			aux = GetLegalQueenMoves(*it, attack_pieces);
			moves->insert(moves->end(), aux->begin(), aux->end());
			delete aux;
			break;

		case PieceType::WhiteKing:
		case PieceType::BlackKing:
			aux = GetLegalKingMoves(*it, attack_pieces);
			moves->insert(moves->end(), aux->begin(), aux->end());
			delete aux;
			break;

		default:
			break;
		}
	}

	delete attack_pieces;
	reverse(moves->begin(), moves->end());
	return moves;
}

vector<pair<pair<char, char>, pair<char, char>>>* Board::GetLegalPawnMoves(pair
 <pair<char, char>, Piece> piece, map<pair<char, char>, Piece> *attack_pieces)
{
	vector<pair<pair<char, char>, pair<char, char>>> *moves = new vector<pair
	 <pair<char, char>, pair<char, char>>>;

	int sign = (piece.second.pieceType == 1) ? 1 : -1;
	vector<char> dx, dy;
	dx = {1, 2, 1, 1};
	dy = {0, 0, -1, 1};
	bool isWhite = (piece.second.pieceType == PieceType::WhitePawn);
	map<pair<char, char>, Piece> *act_pieces = (isWhite) ? whitePieces : blackPieces;
	map<pair<char, char>, Piece> *enemy_pieces = (isWhite) ? blackPieces : whitePieces;
	pair<pair<char, char>, pair<char, char>> lastMove = (piece.second.pieceType == 1)
	 ? blackLastMove : whiteLastMove;


	for(auto x = dx.begin(), y = dy.begin(); x != dx.end() && y != dy.end(); x++, y++)
	{
		pair<char, char> newMove = {piece.first.first + sign * (*x), piece.first.second + (*y)};
		if(!VerifyPosition(newMove))
			continue;

		// Pionul se poate misca 2 patratele daca este la inceput
		if((*x == 2) && ((piece.second.noMoves > 0) || (board[(int)newMove.first - sign]
		 [(int)newMove.second].pieceType != 0)))
			continue;

		if(*y == 0) {
			if(board[(int)newMove.first][(int)newMove.second].pieceType != 0)
				continue;
		} else {
			if(piece.second.pieceType * board[(int)newMove.first][(int)newMove.second]
			 .pieceType >= 0)
				continue;
		}
		pair<pair<char, char>, pair<char, char>> move = {piece.first, newMove};
		map<pair<char, char>, Piece>* attackPieces = verifyIfCheck(attack_pieces,
			&move, isWhite);
		if(attackPieces->size() > 0) {
			delete attackPieces;
			continue;
		}
		delete attackPieces;

		moves->push_back({piece.first, newMove});
	}

	// Left En passant
	pair<char, char> EPLeft = {piece.first.first, piece.first.second - 1};
	pair<pair<char, char>, pair<char, char>> EPLeftLastMove = {{EPLeft.first + 2 * sign,
	 EPLeft.second}, EPLeft};

	if(VerifyPosition(EPLeft)) {
		if(board[(int) EPLeft.first][(int) EPLeft.second].pieceType * sign == -1) {

			if(lastMove == EPLeftLastMove) {

				pair<pair<char, char>, pair<char, char>> move = {piece.first,
				 {EPLeft.first + sign, EPLeft.second}};
				auto entry = enemy_pieces->find(EPLeft);
				pair<pair<char, char>, Piece> aux_piece_map;
				if(entry != enemy_pieces->end())
					aux_piece_map = *entry;
				else
					aux_piece_map = {{-1, -1}, Piece(0,0)};
				// valoare speciala (nu exista piesa pe acea pozitie)
				Piece aux_piece_board = board[(int)EPLeft.first][(int)EPLeft.second];
				MakeMoveOnBoard(move, isWhite, -1);
				map<pair<char, char>, Piece>* attackPieces = verifyIfCheck(NULL, NULL,
				 isWhite);
				if(attackPieces->size() == 0)
					moves->push_back(move);
				delete attackPieces;
				MakeMoveOnBoard({move.second, move.first}, isWhite, -1);
				board[(int)piece.first.first][(int)piece.first.second].noMoves -= 2;
				entry = act_pieces->find(piece.first);
				entry->second.noMoves -= 2;
				board[(int)EPLeft.first][(int)EPLeft.second] = aux_piece_board;
				if(aux_piece_map.first.first != -1 && aux_piece_map.first.second != -1)
					enemy_pieces->insert(aux_piece_map);
			}
		}
	}

	// Right En passant
	pair<char, char> EPRight = {piece.first.first, piece.first.second + 1};
	pair<pair<char, char>, pair<char, char>> EPRightLastMove = {{EPRight.first + 2 * sign,
	 EPRight.second}, EPRight};
	if(VerifyPosition(EPRight)) {
		if(board[(int) EPRight.first][(int) EPRight.second].pieceType * sign == -1) {

			if(lastMove == EPRightLastMove) {
				pair<pair<char, char>, pair<char, char>> move = {piece.first, {EPRight.first
				 + sign, EPRight.second}};

				auto entry = enemy_pieces->find(EPRight);
				pair<pair<char, char>, Piece> aux_piece_map;
				if(entry != enemy_pieces->end())
					aux_piece_map = *entry;
				else
					aux_piece_map = {{-1, -1}, Piece(0,0)};
				// valoare speciala (nu exista piesa pe acea pozitie)
				Piece aux_piece_board = board[(int)EPRight.first][(int)EPRight.second];
				MakeMoveOnBoard(move, isWhite, -1);
				map<pair<char, char>, Piece>* attackPieces = verifyIfCheck(NULL, NULL,
				 isWhite);
				if(attackPieces->size() == 0)
					moves->push_back(move);
				delete attackPieces;
				MakeMoveOnBoard({move.second, move.first}, isWhite, -1);
				board[(int)piece.first.first][(int)piece.first.second].noMoves -= 2;
				entry = act_pieces->find(piece.first);
				entry->second.noMoves -= 2;
				board[(int)EPRight.first][(int)EPRight.second] = aux_piece_board;
				if(aux_piece_map.first.first != -1 && aux_piece_map.first.second != -1)
					enemy_pieces->insert(aux_piece_map);
			}
		}
	}

	reverse(moves->begin(), moves->end());

	return moves;
}

vector<pair<pair<char, char>, pair<char, char>>>* Board::GetLegalKnightMoves(pair
 <pair<char, char>, Piece> piece, map<pair<char, char>, Piece> *attack_pieces) {

	vector<pair<pair<char, char>, pair<char, char>>> *moves = new vector<pair
	 <pair<char, char>, pair<char, char>>>;
	vector<char> dx = {1, 2, 2, 1, -1, -2, -2, -1};
	vector<char> dy = {-2, -1, 1, 2, 2, 1, -1, -2};
	bool isWhite = (piece.second.pieceType == PieceType::WhiteKnight);

	for(auto x = dx.begin(), y = dy.begin(); x != dx.end() && y != dy.end(); x++, y++) {
		pair<char, char> newMove = {piece.first.first + *x, piece.first.second + *y};
		if(!VerifyPosition(newMove))
			continue;

		if(piece.second.pieceType * board[(int)newMove.first][(int)newMove.second]
		.pieceType > 0)
			continue;

		pair<pair<char, char>, pair<char, char>> move = {piece.first, newMove};
		map<pair<char, char>, Piece>* attackPieces = verifyIfCheck(attack_pieces,
			&move, isWhite);
		if(attackPieces->size() > 0) {
			delete attackPieces;
			continue;
		}
		delete attackPieces;
		
		moves->push_back({piece.first, newMove});
	}

	return moves;
}

vector<pair<pair<char, char>, pair<char, char>>>* Board::GetLegalBishopMoves(pair
 <pair<char, char>, Piece> piece, map<pair<char, char>, Piece> *attack_pieces) {
	vector<pair<pair<char, char>, pair<char, char>>> *moves = new vector<pair
	 <pair<char, char>, pair<char, char>>>;

	vector<char> dx = {1, 1, -1, -1};
	vector<char> dy = {-1, 1, 1, -1};
	bool isWhite = (piece.second.pieceType == PieceType::WhiteBishop);

	
	for(auto x = dx.begin(), y = dy.begin(); x != dx.end() && y != dy.end(); x++, y++) {
		for(int i = 1; i < 8; i++) {
			pair<char, char> newMove = {piece.first.first + (*x) * i, piece.first
			 .second + (*y) * i};
			if(!VerifyPosition(newMove))
				break;

			if(piece.second.pieceType * board[(int)newMove.first][(int)newMove.second]
			 .pieceType > 0)
				break;

			pair<pair<char, char>, pair<char, char>> move = {piece.first, newMove};
			map<pair<char, char>, Piece>* attackPieces = verifyIfCheck(attack_pieces,
				&move, isWhite);
			if(attackPieces->size() == 0)
				moves->push_back({piece.first, newMove});
			delete attackPieces;

			if(board[(int)newMove.first][(int)newMove.second].pieceType != 0)
				break;
		}
	}

	return moves;
}

vector<pair<pair<char, char>, pair<char, char>>>* Board::GetLegalRookMoves(pair
 <pair<char, char>, Piece> piece, map<pair<char, char>, Piece> *attack_pieces) {

	vector<pair<pair<char, char>, pair<char, char>>> *moves = new vector<pair
	 <pair<char, char>, pair<char, char>>>;
	vector<char> dx = {1, 0, -1, 0};
	vector<char> dy = {0, 1, 0, -1};

	bool isWhite = (piece.second.pieceType == PieceType::WhiteRook);

	for(auto x = dx.begin(), y = dy.begin(); x != dx.end() && y != dy.end(); x++, y++) {
		for(int i = 1; i < 8; i++) {
			pair<char, char> newMove = {piece.first.first + (*x) * i, piece.first
			 .second + (*y) * i};
			if(!VerifyPosition(newMove))
				break;

			if(piece.second.pieceType * board[(int)newMove.first][(int)newMove.second]
			 .pieceType > 0)
				break;

			pair<pair<char, char>, pair<char, char>> move = {piece.first, newMove};
			map<pair<char, char>, Piece>* attackPieces = verifyIfCheck(attack_pieces,
				&move, isWhite);
			if(attackPieces->size() == 0)
				moves->push_back({piece.first, newMove});
			delete attackPieces;

			if(board[(int)newMove.first][(int)newMove.second].pieceType != 0)
				break;
		}
	}

	return moves;
}

vector<pair<pair<char, char>, pair<char, char>>>* Board::GetLegalQueenMoves(pair
 <pair<char, char>, Piece> piece, map<pair<char, char>, Piece> *attack_pieces) {

	vector<pair<pair<char, char>, pair<char, char>>> *moves = new vector<pair
	 <pair<char, char>, pair<char, char>>>;

	vector<char> dx = {1, 1, -1, -1, 1, 0, -1, 0};
	vector<char> dy = {-1, 1, 1, -1, 0, 1, 0, -1};
	bool isWhite = (piece.second.pieceType == PieceType::WhiteQueen);

	for(auto x = dx.begin(), y = dy.begin(); x != dx.end() && y != dy.end(); x++, y++) {
		for(int i = 1; i < 8; i++) {
			pair<char, char> newMove = {piece.first.first + (*x) * i, piece.first
			 .second + (*y) * i};
			if(!VerifyPosition(newMove))
				break;

			if(piece.second.pieceType * board[(int)newMove.first][(int)newMove.second]
			 .pieceType > 0)
				break;

			pair<pair<char, char>, pair<char, char>> move = {piece.first, newMove};
			map<pair<char, char>, Piece>* attackPieces = verifyIfCheck(attack_pieces,
				&move, isWhite);
			if(attackPieces->size() == 0)
				moves->push_back({piece.first, newMove});
			delete attackPieces;

			if(board[(int)newMove.first][(int)newMove.second].pieceType != 0)
				break;
		}
	}

	return moves;
}

vector<pair<pair<char, char>, pair<char, char>>>* Board::GetLegalKingMoves(pair
 <pair<char, char>, Piece> piece, map<pair<char, char>, Piece> *attack_pieces) {

	vector<pair<pair<char, char>, pair<char, char>>> *moves = new vector<pair
	 <pair<char, char>, pair<char, char>>>;

	vector<char> dx = {1, 1, -1, -1, 1, 0, -1, 0};
	vector<char> dy = {-1, 1, 1, -1, 0, 1, 0, -1};
	
	bool isWhite = (piece.second.pieceType == PieceType::WhiteKing);
	map<pair<char, char>, Piece> *enemy_pieces = isWhite ? blackPieces : whitePieces;
	map<pair<char, char>, Piece> *act_pieces = isWhite ? whitePieces : blackPieces;

	for(auto x = dx.begin(), y = dy.begin(); x != dx.end() && y != dy.end(); x++, y++) {
		pair<char, char> newMove = {piece.first.first + (*x), piece.first.second + (*y)};
		if(!VerifyPosition(newMove))
			continue;

		if(piece.second.pieceType * board[(int)newMove.first][(int)newMove.second]
		 .pieceType > 0)
			continue;

		pair<char, char> old_king_pos = piece.first;
		pair<pair<char, char>, Piece> aux_piece_map;
		auto entry = enemy_pieces->find(newMove);
		if(entry != enemy_pieces->end())
			aux_piece_map = *entry;
		else
			aux_piece_map = {{-1, -1}, Piece(0,0)};
			
		Piece aux_piece_board = board[(int)newMove.first][(int)newMove.second];
		MakeMoveOnBoard({old_king_pos, newMove}, isWhite, -1);
		map<pair<char, char>, Piece>* attackPieces = verifyIfCheck(NULL, NULL, isWhite);
		if(attackPieces->size() == 0)
			moves->push_back({piece.first, newMove});
		delete attackPieces;
		MakeMoveOnBoard({newMove, old_king_pos}, isWhite, -1);
		
		board[(int)piece.first.first][(int)piece.first.second].noMoves -= 2;
        entry = act_pieces->find(piece.first);
        entry->second.noMoves -= 2;

		board[(int)newMove.first][(int)newMove.second] = aux_piece_board;
		if(aux_piece_map.first.first != -1 && aux_piece_map.first.second != -1)
			enemy_pieces->insert(aux_piece_map);
	}

	PieceType myRook = (isWhite) ? PieceType::WhiteRook : PieceType::BlackRook;

	// Rocada
	if(piece.second.noMoves == 0 && attack_pieces->size() == 0) {
		Piece rightRookMaybe = board[(int) piece.first.first][7];
		if((rightRookMaybe.pieceType == myRook) && (rightRookMaybe.noMoves == 0)) {
			bool pathIsEmpty = true;

			for(int i = piece.first.second + 1; i < 7; i++) {
				if(board[(int) piece.first.first][i].pieceType != 0) {
					pathIsEmpty = false;
					break;
				}
			}

			if(pathIsEmpty) {
				MakeMoveOnBoard({piece.first, {piece.first.first, piece.first
				 .second + 1}} ,isWhite, -1);
				map<pair<char, char>, Piece>* attackPieces = verifyIfCheck(NULL,
				 NULL, isWhite);
				if(attackPieces->size() == 0) {
					delete attackPieces;
					MakeMoveOnBoard({{piece.first.first, piece.first.second + 1},
					 {piece.first.first, piece.first.second + 2}} ,isWhite, -1);
					attackPieces = verifyIfCheck(NULL, NULL, isWhite);

					if(attackPieces->size() == 0) {
						moves->push_back({piece.first, {piece.first.first, piece
						 .first.second + 2}});
					}

					delete attackPieces;
					MakeMoveOnBoard({{piece.first.first, piece.first.second + 2},
					 {piece.first.first, piece.first.second + 1}} ,isWhite, -1);
				} else
					delete attackPieces;

				MakeMoveOnBoard({{piece.first.first, piece.first.second + 1},
				 piece.first} ,isWhite, -1);

				board[(int)piece.first.first][(int)piece.first.second].noMoves = 0;
				auto entry = act_pieces->find(piece.first);
				entry->second.noMoves = 0;

			}
		}

		Piece leftRookMaybe = board[(int) piece.first.first][0];
		if((leftRookMaybe.pieceType == myRook) && (leftRookMaybe.noMoves == 0)) {
			bool pathIsEmpty = true;

			for(int i = piece.first.second - 1; i > 0; i--) {
				if(board[(int) piece.first.first][i].pieceType != 0) {
					pathIsEmpty = false;
					break;
				}
			}

			if(pathIsEmpty) {
				MakeMoveOnBoard({piece.first, {piece.first.first, piece.first
				 .second - 1}} ,isWhite, -1);
				map<pair<char, char>, Piece>* attackPieces = verifyIfCheck(NULL,
				 NULL, isWhite);
				if(attackPieces->size() == 0) {
					delete attackPieces;
					MakeMoveOnBoard({{piece.first.first, piece.first.second - 1},
					 {piece.first.first, piece.first.second - 2}} ,isWhite, -1);
					attackPieces = verifyIfCheck(NULL, NULL, isWhite);

					if(attackPieces->size() == 0) {
						moves->push_back({piece.first, {piece.first.first, piece
						 .first.second - 2}});
					}

					delete attackPieces;
					MakeMoveOnBoard({{piece.first.first, piece.first.second - 2},
					 {piece.first.first, piece.first.second - 1}} ,isWhite, -1);
				} else
					delete attackPieces;

				MakeMoveOnBoard({{piece.first.first, piece.first.second - 1},
				 piece.first} ,isWhite, -1);
				board[(int)piece.first.first][(int)piece.first.second].noMoves = 0;
				auto entry = act_pieces->find(piece.first);
				entry->second.noMoves = 0;
			}
		}
	}

	return moves;
}

bool Board::VerifyPosition(pair<char, char> position)
{
	if(position.first < 0 || position.first > 7 || position.second < 0 || position
	 .second > 7)
		return false;
	return true;
}

void Board::PrintLegalMoves(vector<pair<pair<char, char>, pair<char, char>>>* moves)
{
	printf("Legal moves:\n");
	for(auto move = moves->begin(); move != moves->end(); move++)
	{
		printf("(%d, %d) -> (%d, %d)\n", move->first.first, move->first.second,
		 move->second.first, move->second.second);
	}
}

void Board::MakeMoveOnBoard(pair<pair<char, char>, pair<char, char>> actMove,
 bool isWhite, int newPiece) {
    map<pair<char, char>, Piece> *actColorPieces, *enemyColorPieces;
	pair<pair<char, char>, pair<char, char>> lastMove;
	int sign;

    if(isWhite) {
        actColorPieces = this->getWhitePieces();
        enemyColorPieces = this->getBlackPieces();
		whiteLastMove = actMove;
		sign = 1;


    } else {
        actColorPieces = this->getBlackPieces();
        enemyColorPieces = this->getWhitePieces();
		blackLastMove = actMove;
		sign = -1;
    }

    Piece **actBoard = this->getBoard();

    Piece actPiece = actBoard[(int)actMove.first.first][(int)actMove.first.second];
    Piece enemyPiece = actBoard[(int)actMove.second.first][(int)actMove.second.second];
    
    actPiece.noMoves++;

    if((actPiece.pieceType == sign) &&(actMove.first.second - actMove.second.second != 0) &&
     (actBoard[(int)actMove.second.first][(int)actMove.second.second].pieceType == 0)) {
        ///////////////////// EP
        actBoard[(int)actMove.first.first][(int)actMove.first.second] = Piece(0, 0);
        actBoard[(int)actMove.second.first][(int)actMove.second.second] = actPiece;
        actBoard[(int)actMove.second.first - sign][(int)actMove.second.second] = Piece(0, 0);

        auto entryHandler = actColorPieces->extract(actMove.first);
        entryHandler.key() = actMove.second;

        entryHandler.mapped().noMoves++;
        actColorPieces->insert(std::move(entryHandler));

        enemyColorPieces->erase({actMove.second.first - sign, actMove.second.second});

    } else if((actPiece.pieceType == PieceType::WhiteKing * sign) && (abs(actMove
	 .first.second - actMove.second.second) == 2)) {
		//// E rocada
		if(actMove.first.second - actMove.second.second == -2) {
			Piece myRook = actBoard[(int)actMove.second.first][7];
			myRook.noMoves++;

			actBoard[(int) actMove.first.first][(int) actMove.first.second] = Piece(0, 0);
			actBoard[(int) actMove.first.first][7] = Piece(0, 0);

			actBoard[(int)actMove.second.first][(int)actMove.second.second] = actPiece;
			actBoard[(int) actMove.first.first][5] = myRook;

			auto entryHandler = actColorPieces->extract(actMove.first);
			entryHandler.key() = actMove.second;
			entryHandler.mapped().noMoves++;
			actColorPieces->insert(std::move(entryHandler));

			entryHandler = actColorPieces->extract({actMove.first.first, 7});
			entryHandler.key() = {actMove.first.first, 5};
			entryHandler.mapped().noMoves++;
			actColorPieces->insert(std::move(entryHandler));
		} else {
			Piece myRook = actBoard[(int)actMove.second.first][0];
			myRook.noMoves++;

			actBoard[(int) actMove.first.first][(int) actMove.first.second] = Piece(0, 0);
			actBoard[(int) actMove.first.first][0] = Piece(0, 0);

			actBoard[(int)actMove.second.first][(int)actMove.second.second] = actPiece;
			actBoard[(int) actMove.first.first][3] = myRook;

			auto entryHandler = actColorPieces->extract(actMove.first);
			entryHandler.key() = actMove.second;
			entryHandler.mapped().noMoves++;
			actColorPieces->insert(std::move(entryHandler));

			entryHandler = actColorPieces->extract({actMove.first.first, 0});
			entryHandler.key() = {actMove.first.first, 3};
			entryHandler.mapped().noMoves++;
			actColorPieces->insert(std::move(entryHandler));
		}
	} else {
		////////////////////// E altceva

		actBoard[(int)actMove.first.first][(int)actMove.first.second] = Piece(0, 0);
		actBoard[(int)actMove.second.first][(int)actMove.second.second] = actPiece;

		if(((int)actMove.second.first == 7 || (int)actMove.second.first == 0)
			&& abs(actPiece.pieceType) == 1)
			actBoard[(int)actMove.second.first][(int)actMove.second.second].pieceType
			 *= newPiece;

		auto entryHandler = actColorPieces->extract(actMove.first);
		entryHandler.key() = actMove.second;

		if(((int)actMove.second.first == 7 || (int)actMove.second.first == 0)
			&& abs(actPiece.pieceType) == 1)
			entryHandler.mapped().pieceType *= newPiece;
		
		entryHandler.mapped().noMoves++;
		actColorPieces->insert(std::move(entryHandler));

		if(enemyPiece.pieceType != 0)
			enemyColorPieces->erase(actMove.second);
	}
}


int Board::evaluateMaterial(bool isWhite) {
    map<pair<char, char>, Piece> *actPieces, *enemyPieces;
    if(isWhite) {
        actPieces = whitePieces;
        enemyPieces = blackPieces;
    } else {
        actPieces = blackPieces;
        enemyPieces = whitePieces;
    }
    int result = 0;
    for(auto piece = actPieces->begin(); piece != actPieces->end(); ++piece)
        result += piece->second.pieceValue;
    for(auto piece = enemyPieces->begin(); piece != enemyPieces->end(); ++piece)
        result -= piece->second.pieceValue;

    return result;
}

int Board::evaluateSpaces(bool isWhite) {

	int boardValue[8][8] = {{0, 0, 1, 1, 1, 1, 0, 0},
                            {0, 1, 2, 2, 2, 2, 1, 0},
                            {1, 2, 2, 3, 3, 2, 2, 1},
                            {1, 2, 3, 3, 3, 3, 2, 1},
                            {1, 2, 3, 3, 3, 3, 2, 1},
                            {1, 2, 2, 3, 3, 2, 2, 1},
                            {0, 1, 2, 2, 2, 2, 1, 0},
                            {0, 0, 1, 1, 1, 1, 0, 0}};
    int result = 0;
    int color = (isWhite) ? 1 : -1;
    for(int i = 0; i < 8; ++i)
        for(int j = 0; j < 8; ++j)
            if(board[i][j].pieceType != 0)
                result += color *  (board[i][j].pieceType / abs(board[i][j].pieceType)) * boardValue[i][j];
    return result;
}

int Board::evaluateIfCheck(bool isWhite) {
	map<pair<char, char>, Piece> *attack_pieces_1 = verifyIfCheck(NULL, NULL, isWhite);
	map<pair<char, char>, Piece> *attack_pieces_2 = verifyIfCheck(NULL, NULL, !isWhite);

	return 20 * (attack_pieces_2->size() - attack_pieces_1->size());
}

int Board::evaluatePawnStruct(bool isWhite) {
	map<pair<char, char>, Piece> *actPieces, *enemyPieces;
    if(isWhite) {
        actPieces = whitePieces;
        enemyPieces = blackPieces;
    } else {
        actPieces = blackPieces;
        enemyPieces = whitePieces;
    }

	vector<int> actVec(8, -1);
	vector<int> enemyVec(8, -1);

	for(auto piece = actPieces->begin(); piece != actPieces->end(); ++piece) {
        if(abs(piece->second.pieceType) == WhitePawn)
			actVec[piece->first.first] = piece->first.second;
	}

    for(auto piece = enemyPieces->begin(); piece != enemyPieces->end(); ++piece) {
        if(abs(piece->second.pieceType) == WhitePawn)
			enemyVec[piece->first.first] = piece->first.second;
	}

	int size = 0;
	int ans = 0;
	for(int i = 0; i < 7; i++) {
		if(abs(actVec[i] - actVec[i + 1]) < 2) {
			size++;
		} else {
			ans += size * (max(actVec[i], actVec[i + 1]));
			size = 0;
		}
	}
	ans += size * size;

	size = 0;
	for(int i = 0; i < 7; i++) {
		if(abs(enemyVec[i] - enemyVec[i + 1]) < 2) {
			size++;
		} else {
			ans -= size * (max(actVec[i], actVec[i + 1]));
			size = 0;
		}
	}
	ans -= size * (max(actVec[6], actVec[7]));

	return ans;
}

int Board::evaluate(bool isWhite) {
    return 2 * evaluateMaterial(isWhite) + 2 * evaluateSpaces(isWhite) + evaluateIfCheck
	 (isWhite) + evaluatePawnStruct(isWhite) / 2;
}