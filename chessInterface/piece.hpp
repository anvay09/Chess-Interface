#ifndef _PIECE_
#define _PIECE_ 

#include <bits/stdc++.h>
#include "board.hpp"
using namespace std;
class board;

class pieceLocation {
public:
    int file;
    int rank;

    pieceLocation();

    pieceLocation(int f, int r);

    pieceLocation(string algebraicNotation);

    pieceLocation(const pieceLocation &p);

    string convertToAlgebraicNotation();

    bool isValidLocation();
};


class piece {
public:
    char colour;
    char identity;
    pieceLocation position; // rank and file of piece
    
    piece(char c, pieceLocation p, char id);
};


class pawn : public piece {
    int pawnMoves[4][2] = {{0,2}, {0,1}, {1,1}, {-1,1}};

public:
    pawn(char c, pieceLocation p, char id);
};


class knight : public piece {
    int knightMoves[8][2] = {{1,2}, {-1,2}, {1,-2}, {-1,-2}, {2,1}, {-2,1}, {2,-1}, {-2,-1}};

public:
    knight(char c, pieceLocation p, char id);
};


class bishop : public piece {
    int bishopMoves[4][2] = {{1,1}, {-1,-1}, {-1,1}, {1,-1}};
public:
    bishop(char c, pieceLocation p, char id);
};


class rook : public piece {
    int rookMoves[4][2] = {{1,0}, {0,1}, {-1,0}, {0,-1}};

public:
    rook(char c, pieceLocation p, char id);
};


class queen : public piece {
    int queenMoves[8][2] = {{1,0}, {0,1}, {-1,0}, {0,-1}, {1,1}, {-1,-1}, {-1,1}, {1,-1}};

public:
    queen(char c, pieceLocation p, char id);
};


class king : public piece {
    int kingMoves[8][2] = {{1,0}, {0,1}, {-1,0}, {0,-1}, {1,1}, {-1,-1}, {-1,1}, {1,-1}};

public:
    king(char c, pieceLocation p, char id);
};

#endif