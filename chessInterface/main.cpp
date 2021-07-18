#include <bits/stdc++.h>
#include "board.hpp"
#include "piece.hpp"
using namespace std;

int main()
{
    // sample code to demonstrate functions implemented

    string FEN1("r1bqk2r/1p2ppbp/p1n3p1/1Bp5/3PP3/P1P1B3/5PPP/R2QK1NR w KQkq - 0 10");
    board b1(FEN1);
    b1.printBoard();

    pieceLocation p1("c6");
    cout << "Is c6 pinned? " << b1.isPinned(p1) << endl;

    pieceLocation p3("b5");
    cout << "Is b5 hanging? " << b1.isHanging(p3) << endl;
    cout << "Does a6b5 capture a hanging piece? " << b1.capturesHangingPiece("a6b5") << endl;
    vector<string> s = b1.piecesAttacked(p3);
    cout << "Pieces attacked by b5 are: ";
    for (int i = 0; i < s.size(); i++)
        cout << s[i] << " ";
    cout << endl;

    pieceLocation p4("d8");
    cout << "Number of legal moves of d8 are: " << b1.numLegalMoves(p4) << endl;
    cout << "Number of legal moves of pieces that d8 attacks are: ";
    vector<int> n = b1.numLegalMovesForAttackedPieces(p4);
    for (int i = 0; i < n.size(); i++)
        cout << n[i] << " ";
    cout << endl;
    
    pieceLocation p5("d4");
    cout << "Number of times d4 is attacked: " << b1.numTimesAttacked(p5) << endl;
    cout << "Number of times d4 is defended: " << b1.numTimesDefended(p5) << endl;


    // sample code to calculate static evaluation exchange
    string FEN2("r2qk2r/pp2ppbp/2n3p1/2p5/3PP1b1/2P1BN2/P3BPPP/R2Q1RK1 b kq - 5 10");
    board b2(FEN2);
    b2.printBoard();
    pieceLocation p2("d4");
    cout << b2.staticEvaluationExchange(p2, "c5d4") << endl;


    return 0;
}