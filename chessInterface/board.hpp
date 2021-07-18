#ifndef _BOARD_
#define _BOARD_

#include <bits/stdc++.h>
#include "piece.hpp"
using namespace std;

class piece;
class pieceLocation;

// The board class initializes a chessboard as a 2D array with pointers to piece objects, given a postion in FEN

class board {
    string piecePlacement;
    string activeColour;
    string castlingInfo;
    string enPassantTargetSquare;
    string halfmoveClock;
    string fullmoveClock;
    map <int, string> rows;
public:
    piece* chessboard[8][8]; // stores pointers to piece objects

    board(); // default constructor, initializes pointers to NULL

    board(string FEN); // takes FEN position as input and sets up chessboard, creates piece objects

    piece* boardLookup(pieceLocation& p); // look up given coordinate on board and return pointer to piece there

    void pieceAt(pieceLocation& p); // print piece at a given coordinate on board

    void setBoardLocation(pieceLocation& p, piece* ptr); // put given piece on a given square

    void printBoard(); // print board

    int numTimesAttacked(pieceLocation &p); // check if given piece is attacked by enemy pieces
                                            // does not consider stacked attackers (eg. 2 rooks on a file lined up)
                                            // does not check if attacking pieces are pinned

    int numTimesDefended(pieceLocation &p); // check if given piece is defended by own pieces
                                            // does not consider stacked defenders (eg. 2 rooks on a file lined up)
                                            // does not check if defending pieces are pinned
    
    
    bool isPinned(pieceLocation &p); // check if given piece is pinned
                                     // does not check if pinned piece can capture the piece imposing the pin 
                                     // (eg. [ K --- R --- q ] here the rook is pinned but it can capture the enemy queen)

    bool isHanging(pieceLocation &p); // check if given piece is hanging
                                      // does not check if capturing pieces are pinned

    vector<string> piecesAttacked(pieceLocation &p); // returns squares of pieces attacked by a given piece
                                                     // does not consider whether attacking piece can move

    int numLegalMoves(pieceLocation &p); // returns number of legal moves of a given piece
                                         // does not consider castling, en passant and pieces which cannot moves due to pins or checks

    vector<int> numLegalMovesForAttackedPieces(pieceLocation &p); // returns number of legal moves of pieces that can be captured by a given piece

    vector<string> listOfAttackers(pieceLocation &pLoc, char side); // returns list of direct and stacked attackers at a given square by a given colour

    bool capturesHangingPiece(string move); // checks whether given move captures a hanging piece

    int staticEvaluationExchange(pieceLocation &square, string capture); // calculates SEE for the side who's turn it is at a square
};

#endif
