#include "board.h"

class Game {
private:
    Board board;
    bool verifyValidMoveAsString(string input);

public:
    Board getBoard();
    string getNextMove(bool isWhite);
    int updateBoardForEnemy(string input, bool isBlack);
    string FromMoveToStr(pair<pair<char, char>, pair<char, char>> actMove);
    pair<int, pair<pair<char, char>, pair<char, char>>> alphabeta_negamax(int depth, bool isWhite, int alpha, int beta);
};