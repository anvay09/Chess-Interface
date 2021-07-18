#include <bits/stdc++.h>
#include "piece.hpp"
#include "board.hpp"
using namespace std;

pieceLocation::pieceLocation(){ //default constructor
    file = 0; 
    rank = 0;
}

pieceLocation::pieceLocation(int f, int r){ // constructor given rank and file
    file = f;
    rank = r;
}

pieceLocation::pieceLocation(string algebraicNotation){ // constructor given position in algebraic notation
    file = algebraicNotation[0] - 'a' + 1;
    rank = algebraicNotation[1] - '0';
}

pieceLocation::pieceLocation(const pieceLocation &p){ // copy constructor
    file = p.file;
    rank = p.rank;
}

string pieceLocation::convertToAlgebraicNotation(){ // convert from 1,8 to "a8"
    string s;
    s.push_back(file + 'a' - 1);
    s.push_back(rank + '0');
    return s;
}

bool pieceLocation::isValidLocation(){
    return file >= 1 && file <= 8 && rank >= 1 && rank <= 8;
}

piece::piece(char c, pieceLocation p, char id){
    colour = c;
    position = p;
    identity = id;
}

pawn::pawn(char c, pieceLocation p, char id) : piece(c,p,id){
}

knight::knight(char c, pieceLocation p, char id) : piece(c,p,id){
}

bishop::bishop(char c, pieceLocation p, char id) : piece(c,p,id){
}

rook::rook(char c, pieceLocation p, char id) : piece(c,p,id){
}

queen::queen(char c, pieceLocation p, char id) : piece(c,p,id){
}

king::king(char c, pieceLocation p, char id) : piece(c,p,id){
}

