#include "game.h"

using namespace std;

Board Game::getBoard() { return board; }

string Game::getNextMove(bool isWhite) {
    pair<int, pair<pair<char, char>, pair<char, char>>> ans = alphabeta_negamax(5, isWhite, INT32_MIN, INT32_MAX);


    if(ans.first == INT32_MIN)
        return "resign";
    else {
        Piece **my_board = board.getBoard();
        pair<pair<char, char>, pair<char, char>> actMove = ans.second;

        if(abs(my_board[(int) actMove.first.first][(int) actMove.first.second].pieceType) == PieceType::WhitePawn
         && (actMove.second.first == 7 || actMove.second.first == 0))
            board.MakeMoveOnBoard(actMove, isWhite, 5);
        else
            board.MakeMoveOnBoard(actMove, isWhite, -1);
            
        return (FromMoveToStr(ans.second));
    }
}

pair<int, pair<pair<char, char>, pair<char, char>>> Game::alphabeta_negamax(int depth, bool isWhite, int alpha, int beta) {
    if(depth == 0)
        return {board.evaluate(isWhite), {{-1,-1}, {-1, -1}}};

    vector<pair<pair<char, char>, pair<char, char>>> *all_moves = board.GetAllLegalMoves(isWhite);

    if((all_moves == NULL) || (all_moves->size() == 0))
        return {INT32_MIN, {{-1,-1}, {-1, -1}}};

    pair<pair<char, char>, pair<char, char>> best_move;
    int best_score = INT32_MIN;

    for(auto move : *all_moves) {
        Piece **my_board = board.getBoard();
        Piece **auxTable = new (Piece*[8])();

        for(int i = 0; i < 8; i++) {
            auxTable[i] = new (Piece[8])();
            for(int j = 0; j < 8; j++) {
                auxTable[i][j] = my_board[i][j];
            }
        }

        map<pair<char, char>, Piece> *auxWhitePieces = new map<pair<char, char>, Piece>();
        map<pair<char, char>, Piece> *auxBlackPieces = new map<pair<char, char>, Piece>();

        map<pair<char, char>, Piece> *whitePieces = this->board.getWhitePieces();
        map<pair<char, char>, Piece> *blackPieces = this->board.getBlackPieces();

        for(auto x = whitePieces->begin(); x != whitePieces->end(); x++) {
            auxWhitePieces->insert(*x);
        }

        for(auto x = blackPieces->begin(); x != blackPieces->end(); x++) {
            auxBlackPieces->insert(*x);
        }

        if(abs(my_board[(int) move.first.first][(int) move.first.second].pieceType) == PieceType::WhitePawn
         && (move.second.first == 7 || move.second.first == 0))
            board.MakeMoveOnBoard(move, isWhite, 5);
        else
            board.MakeMoveOnBoard(move, isWhite, -1);
        
        int score = board.evaluateIfCheck(isWhite);
        if(score == 0)
            score = -alphabeta_negamax(depth - 1, !isWhite, -beta, -alpha).first;


        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                my_board[i][j] = auxTable[i][j];
            }
        }

        whitePieces->clear();
        blackPieces->clear();
        for(auto x = auxWhitePieces->begin(); x != auxWhitePieces->end(); x++) {
            whitePieces->insert(*x);
        }

        for(auto x = auxBlackPieces->begin(); x != auxBlackPieces->end(); x++) {
            blackPieces->insert(*x);
        }

        for(int i = 0; i < 8; i++) {
            delete auxTable[i];
        }
        delete auxTable;

        delete auxWhitePieces;
        delete auxBlackPieces;

        if (score > best_score) {
            best_score = score;
            best_move = move;
        }
        alpha = max(alpha, best_score);

        if (alpha >= beta)
            break;

    }
    delete all_moves;

    return {best_score, best_move};
}


string Game::FromMoveToStr(pair<pair<char, char>, pair<char, char>> actMove) {
    string nextMove = "";
    nextMove += (char)(97 + actMove.first.second);
    nextMove += (char)(49 + actMove.first.first);
    nextMove += (char)(97 + actMove.second.second);
    nextMove += (char)(49 + actMove.second.first);

    return "move " + nextMove;
}

bool Game::verifyValidMoveAsString(string input) {
    return isalpha(input[0]) && isalpha(input[2]) && isdigit(input[1])
        && isdigit(input[3]);
}

int Game::updateBoardForEnemy(string input, bool isBlack) {
    if(!verifyValidMoveAsString(input))
        return 0;

    pair<pair<char, char>, pair<char, char>> actMove = {{input[1] - '1', input[0] - 'a'},
        {input[3] - '1', input[2] - 'a'}};
    
    int newPiece = -1;

    if(actMove.second.first == 7 || actMove.second.first == 0) {
        switch (input[4])
        {
        case 'n':
            newPiece = 2;
            break;

        case 'b':
            newPiece = 3;
            break;
        
        case 'r':
            newPiece = 4;
            break;

        case 'q':
        case '\0':
            newPiece = 5;
            break;
        
        default:
            break;
        }
    }
    board.MakeMoveOnBoard(actMove, isBlack, newPiece);

    return 1;
}