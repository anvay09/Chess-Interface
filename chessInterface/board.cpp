#include <bits/stdc++.h>
#include "piece.hpp"
#include "board.hpp"
using namespace std;

piece* board::boardLookup(pieceLocation& pLoc){
    return chessboard[pLoc.file - 1][pLoc.rank - 1];
}

void board::pieceAt(pieceLocation& pLoc){
    piece* ptr = boardLookup(pLoc);
    if(ptr){
        cout << ptr->identity << ptr->position.convertToAlgebraicNotation() << endl;
    }
    else{
        cout << "empty" << endl;
    }
}

void board::setBoardLocation(pieceLocation& pLoc, piece* ptr){
    chessboard[pLoc.file - 1][pLoc.rank - 1] = ptr;
}

void board::printBoard(){
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if(chessboard[j][7-i]){
                cout << "|" << chessboard[j][7-i]->identity;
            }
            else cout << "|-";
        }
        cout << "|" << endl;
    }
}

board::board(){
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            chessboard[i][j] = NULL;
        }
    }
}

board::board(string FEN){
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            chessboard[i][j] = NULL;
        }
    }
    stringstream s(FEN);

    s >> piecePlacement; // cout << piecePlacement << endl;
    s >> activeColour; // cout << activeColour << endl;
    s >> castlingInfo; // cout << castlingInfo << endl;
    s >> enPassantTargetSquare; // cout << enPassantTargetSquare << endl;
    s >> halfmoveClock; // cout << halfmoveClock << endl;
    s >> fullmoveClock; // cout << fullmoveClock << endl;
    
    stringstream r(piecePlacement);
    string token;
    int rank = 8;
    while(getline(r, token, '/'))
    {
        rows.insert(pair<int, string> (rank, token));
        rank--;
    }
    
    for (int rank = 1; rank <= 8; rank++)
    {
        char element;
        int file = 1;

        for (int j = 0; j < rows.at(rank).size(); j++)
        {
            element = rows.at(rank)[j];
            // cout << element << file << rank << endl;
            pieceLocation pLoc(file, rank);
            piece* piecePointer = NULL;

            if(element == 'P'){
                piecePointer = new pawn('w', pLoc, 'P');
                file++;
            }
            else if(element == 'p'){
                piecePointer = new pawn('b', pLoc, 'p');
                file++;
            }
            else if(element == 'N'){
                piecePointer = new knight('w', pLoc, 'N');
                
                file++;
            }
            else if(element == 'n'){
                piecePointer = new knight('b', pLoc, 'n');
                file++;
            }
            else if(element == 'B'){
                piecePointer = new bishop('w', pLoc, 'B');
                file++;
            }
            else if(element == 'b'){
                piecePointer = new bishop('b', pLoc, 'b');
                file++;
            }
            else if(element == 'R'){
                piecePointer = new rook('w', pLoc, 'R');
                file++;
            }
            else if(element == 'r'){
                piecePointer = new rook('b', pLoc, 'r');
                file++;
            }
            else if(element == 'Q'){
                piecePointer = new queen('w', pLoc, 'Q');
                file++;
            }
            else if(element == 'q'){
                piecePointer = new queen('b', pLoc, 'q');
                file++;
            }
            else if(element == 'K'){
                piecePointer = new king('w', pLoc, 'K');
                file++;
            }
            else if(element == 'k'){
                piecePointer = new king('b', pLoc, 'k');
                file++;
            }
            else{
                int x = element - '0';
                file = file + x;
            }

            setBoardLocation(pLoc, piecePointer);
        }
    }
}

int board::numTimesAttacked(pieceLocation &pLoc){ // check if given piece is attacked by enemy pieces
                                                  // does not consider stacked attackers (eg. 2 rooks on a file lined up)
                                                  // does not check if attacking pieces are pinned
                                                  // implemented by traversing the 8 directions around a square and counting attackers
    if(!boardLookup(pLoc)) return 0; // square is empty
    int numTimes = 0;
    
    char pieceColour = boardLookup(pLoc)->colour;
    int moves[8][2] = {{0,1}, {0,-1}, {1,0}, {-1,0}, {1,1}, {-1,1}, {1,-1}, {-1,-1}}; // moves to take to move in N,S,E,W,NE,NW,SE or SW directions
    int knightMoves[8][2] = {{1,2}, {-1,2}, {1,-2}, {-1,-2}, {2,1}, {-2,1}, {2,-1}, {-2,-1}};
    int pawnAttacks[2][2] = {{1,1}, {-1,1}}; // mulitply rank by -1 for black

    pieceLocation traverse = pLoc; // moves along files and diagonals

    for (int i = 1; i <= 8; i++){
        traverse.file = traverse.file + moves[i-1][0];
        traverse.rank = traverse.rank + moves[i-1][1];

        while(traverse.isValidLocation()){
            if(boardLookup(traverse)){
                char c = boardLookup(traverse)->colour;
                char id = boardLookup(traverse)->identity;

                if(c != pieceColour){
                    if(i >= 1 && i <= 4){ // i = 1 to 4 indicates attackers which move along files or ranks, Queen and Rook
                        if(id == 'Q' || id == 'q' || id == 'R' || id == 'r'){
                            numTimes ++;
                            break;
                        }
                        else break;
                    }
                    else if(i >= 5 && i <= 8){ // i = 5 to 8 indicates attackers which move along diagonals, Queen and Bishop
                        if(id == 'Q' || id == 'q' || id == 'B' || id == 'b'){
                            numTimes++;
                            break;
                        }
                        else break;
                    }
                }
                else break;
            }

            traverse.file = traverse.file + moves[i-1][0];
            traverse.rank = traverse.rank + moves[i-1][1];
        }

        traverse = pLoc;
    }

    for (int i = 1; i <= 8; i++) // check enemy knights
    {
        traverse = pLoc;
        traverse.file = traverse.file + knightMoves[i-1][0];
        traverse.rank = traverse.rank + knightMoves[i-1][1];

        if(traverse.isValidLocation()){
            if(boardLookup(traverse)){
                char c = boardLookup(traverse)->colour;
                char id = boardLookup(traverse)->identity;

                if(c != pieceColour){
                    if(id == 'N' || id == 'n'){
                        numTimes++;
                    }
                }
            }
        }
    }

    for (int i = 1; i <= 2; i++) // check enemy pawns
    {
        traverse = pLoc;
        int factor = 1;
        if(pieceColour == 'b') factor = -1;
        traverse.file = traverse.file + pawnAttacks[i-1][0];
        traverse.rank = traverse.rank + factor*pawnAttacks[i-1][1];

        if(traverse.isValidLocation()){
            if(boardLookup(traverse)){
                char c = boardLookup(traverse)->colour;
                char id = boardLookup(traverse)->identity;

                if(c != pieceColour){
                    if(id == 'p' || id == 'P'){
                        numTimes++;
                    }
                }
            }
        }
    }
    
    return numTimes;
}

int board::numTimesDefended(pieceLocation &pLoc){ // check if given piece is defended by own pieces
                                                  // does not consider stacked defenders (eg. 2 rooks on a file lined up)
                                                  // does not check if defending pieces are pinned
                                                  // same implementation as numTimesDefended but considers same colour pieces instead
    if(!boardLookup(pLoc)) return 0; // square is empty
    int numTimes = 0;
    
    char pieceColour = boardLookup(pLoc)->colour;
    int moves[8][2] = {{0,1}, {0,-1}, {1,0}, {-1,0}, {1,1}, {-1,1}, {1,-1}, {-1,-1}}; // moves to take to move in N,S,E,W,NE,NW,SE or SW directions
    int knightMoves[8][2] = {{1,2}, {-1,2}, {1,-2}, {-1,-2}, {2,1}, {-2,1}, {2,-1}, {-2,-1}};
    int pawnAttacks[2][2] = {{1,1}, {-1,1}}; // mulitply rank by -1 for black

    pieceLocation traverse = pLoc; // moves along files and diagonals

    for (int i = 1; i <= 8; i++){
        traverse.file = traverse.file + moves[i-1][0];
        traverse.rank = traverse.rank + moves[i-1][1];

        while(traverse.isValidLocation()){
            if(boardLookup(traverse)){
                char c = boardLookup(traverse)->colour;
                char id = boardLookup(traverse)->identity;

                if(c == pieceColour){
                    if(i >= 1 && i <= 4){ // attackers move along files or ranks, Queen and Rook
                        if(id == 'Q' || id == 'q' || id == 'R' || id == 'r'){
                            numTimes ++;
                            break;
                        }
                        else break;
                    }
                    else if(i >= 5 && i <= 8){ // attackers move along diagonals, Queen and Bishop
                        if(id == 'Q' || id == 'q' || id == 'B' || id == 'b'){
                            numTimes++;
                            break;
                        }
                        else break;
                    }
                }
                else break;
            }

            traverse.file = traverse.file + moves[i-1][0];
            traverse.rank = traverse.rank + moves[i-1][1];
        }

        traverse = pLoc;
    }

    for (int i = 1; i <= 8; i++) // check player knights
    {
        traverse = pLoc;
        traverse.file = traverse.file + knightMoves[i-1][0];
        traverse.rank = traverse.rank + knightMoves[i-1][1];

        if(traverse.isValidLocation()){
            if(boardLookup(traverse)){
                char c = boardLookup(traverse)->colour;
                char id = boardLookup(traverse)->identity;

                if(c == pieceColour){
                    if(id == 'N' || id == 'n'){
                        numTimes++;
                    }
                }
            }
        }
    }

    for (int i = 1; i <= 2; i++) // check player pawns
    {
        traverse = pLoc;
        int factor = 1;
        if(pieceColour == 'w') factor = -1;
        traverse.file = traverse.file + pawnAttacks[i-1][0];
        traverse.rank = traverse.rank + factor*pawnAttacks[i-1][1];

        if(traverse.isValidLocation()){
            if(boardLookup(traverse)){
                char c = boardLookup(traverse)->colour;
                char id = boardLookup(traverse)->identity;

                if(c == pieceColour){
                    if(id == 'p' || id == 'P'){
                        numTimes++;
                    }
                }
            }
        }
    }
    
    return numTimes;
}

bool board::isPinned(pieceLocation &pLoc){ // check if given piece is pinned
                                           // does not check if pinned piece can capture the piece imposing the pin 
                                           // (eg. [ K --- R --- q ] here the rook is pinned but it can capture the enemy queen)
    if(!boardLookup(pLoc)) return false; // square is empty
    bool directions[8]; // booleans to check if N,S,E,W,NE,NW,SE or SW directions contain an attacker
    for (int i = 0; i < 8; i++) directions[i] = false;
    
    int kingPos = 0; // 0 if king is not on files or diagonals
                 // 1-8 based on whether king is in N,S,E,W,NE,NW,SE or SW directions
    char pieceColour = boardLookup(pLoc)->colour;
    int moves[8][2] = {{0,1}, {0,-1}, {1,0}, {-1,0}, {1,1}, {-1,1}, {1,-1}, {-1,-1}}; // moves to take to move in N,S,E,W,NE,NW,SE or SW directions

    pieceLocation traverse = pLoc; // moves along files and diagonals

    for (int i = 1; i <= 8; i++){
        traverse.file = traverse.file + moves[i-1][0];
        traverse.rank = traverse.rank + moves[i-1][1];

        while(traverse.isValidLocation()){
            if(boardLookup(traverse)){
                char c = boardLookup(traverse)->colour;
                char id = boardLookup(traverse)->identity;

                if(c != pieceColour){
                    if(i >= 1 && i <= 4){ // attackers move along files or ranks, Queen and Rook
                        if(id == 'Q' || id == 'q' || id == 'R' || id == 'r'){
                            directions[i-1] = true;
                            break;
                        }
                        else break;
                    }
                    else if(i >= 5 && i <= 8){ // attackers move along diagonals, Queen and Bishop
                        if(id == 'Q' || id == 'q' || id == 'B' || id == 'b'){
                            directions[i-1] = true;
                            break;
                        }
                        else break;
                    }
                }
                else{
                    if(id == 'K' || id == 'k'){
                        kingPos = i;
                        break;
                    }
                }
            }

            traverse.file = traverse.file + moves[i-1][0];
            traverse.rank = traverse.rank + moves[i-1][1];
        }

        traverse = pLoc;
    }

    switch (kingPos)
    {
    case 1: // N
        return directions[1]; // S
        break;
    case 2: // S
        return directions[0]; // N
        break;
    case 3: // E
        return directions[3]; // W
        break;
    case 4: // W
        return directions[2]; // E
        break;
    case 5: // NE
        return directions[7]; // SW
        break;
    case 6: // NW
        return directions[6]; // SE
        break;
    case 7: // SE
        return directions[5]; // NW
        break;
    case 8: // SW
        return directions[4]; // NE
        break;
    
    default:
        return false;
        break;
    }
}

bool board::isHanging(pieceLocation &pLoc){ // check if given piece is hanging
    if(!boardLookup(pLoc)) return false; // square is empty
    
    if(numTimesDefended(pLoc) == 0 && numTimesAttacked(pLoc) >= 1) return true;
    else return false;
}

vector<string> board::piecesAttacked(pieceLocation &pLoc){ // returns squares of pieces that can be captured by a given piece
    vector<string> capturablePieces;

    if(!boardLookup(pLoc)) return capturablePieces; // square is empty
    
    char pieceColour = boardLookup(pLoc)->colour;
    char pieceId = boardLookup(pLoc)->identity;
    int moves[8][2] = {{0,1}, {0,-1}, {1,0}, {-1,0}, {1,1}, {-1,1}, {1,-1}, {-1,-1}}; // moves to take to move in N,S,E,W,NE,NW,SE or SW directions
    int knightMoves[8][2] = {{1,2}, {-1,2}, {1,-2}, {-1,-2}, {2,1}, {-2,1}, {2,-1}, {-2,-1}};
    int pawnAttacks[2][2] = {{1,1}, {-1,1}}; // mulitply rank by -1 for black

    pieceLocation traverse = pLoc; // moves along files and diagonals

    if(pieceId == 'Q' || pieceId == 'q'){ // if piece is queen
        for (int i = 1; i <= 8; i++){ 
            traverse.file = traverse.file + moves[i-1][0];
            traverse.rank = traverse.rank + moves[i-1][1];

            while(traverse.isValidLocation()){
                if(boardLookup(traverse)){
                    char c = boardLookup(traverse)->colour;

                    if(c != pieceColour){
                        capturablePieces.push_back(boardLookup(traverse)->position.convertToAlgebraicNotation());
                        break;
                    }
                    else break;
                }

                traverse.file = traverse.file + moves[i-1][0];
                traverse.rank = traverse.rank + moves[i-1][1];
            }

            traverse = pLoc;
        }
    }
    else if(pieceId == 'R' || pieceId == 'r'){ // if piece is rook
        for (int i = 1; i <= 4; i++){ 
            traverse.file = traverse.file + moves[i-1][0];
            traverse.rank = traverse.rank + moves[i-1][1];

            while(traverse.isValidLocation()){
                if(boardLookup(traverse)){
                    char c = boardLookup(traverse)->colour;

                    if(c != pieceColour){
                        capturablePieces.push_back(boardLookup(traverse)->position.convertToAlgebraicNotation());
                        break;
                    }
                    else break;
                }

                traverse.file = traverse.file + moves[i-1][0];
                traverse.rank = traverse.rank + moves[i-1][1];
            }

            traverse = pLoc;
        }
    }
    else if(pieceId == 'B' || pieceId == 'b'){ // if piece is bishop
        for (int i = 5; i <= 8; i++){ 
            traverse.file = traverse.file + moves[i-1][0];
            traverse.rank = traverse.rank + moves[i-1][1];

            while(traverse.isValidLocation()){
                if(boardLookup(traverse)){
                    char c = boardLookup(traverse)->colour;

                    if(c != pieceColour){
                        capturablePieces.push_back(boardLookup(traverse)->position.convertToAlgebraicNotation());
                        break;
                    }
                    else break;
                }

                traverse.file = traverse.file + moves[i-1][0];
                traverse.rank = traverse.rank + moves[i-1][1];
            }

            traverse = pLoc;
        }
    }
    else if(pieceId == 'N' || pieceId == 'n'){ // if piece is Knight
        for (int i = 1; i <= 8; i++) 
        {
            traverse = pLoc;
            traverse.file = traverse.file + knightMoves[i-1][0];
            traverse.rank = traverse.rank + knightMoves[i-1][1];

            if(traverse.isValidLocation()){
                if(boardLookup(traverse)){
                    char c = boardLookup(traverse)->colour;

                    if(c != pieceColour){
                        capturablePieces.push_back(boardLookup(traverse)->position.convertToAlgebraicNotation());
                    }
                }
            }
        }
    }
    else if(pieceId == 'K' || pieceId == 'k'){ // if piece is King
        for (int i = 1; i <= 8; i++) 
        {
            traverse = pLoc;
            traverse.file = traverse.file + moves[i-1][0];
            traverse.rank = traverse.rank + moves[i-1][1];

            if(traverse.isValidLocation()){
                if(boardLookup(traverse)){
                    char c = boardLookup(traverse)->colour;

                    if(c != pieceColour){
                        capturablePieces.push_back(boardLookup(traverse)->position.convertToAlgebraicNotation());
                    }
                }
            }
        }
    }
    else { // if piece is Pawn
        for (int i = 1; i <= 2; i++) 
        {
            traverse = pLoc;
            int factor = 1;
            if(pieceColour == 'b') factor = -1;
            traverse.file = traverse.file + pawnAttacks[i-1][0];
            traverse.rank = traverse.rank + factor*pawnAttacks[i-1][1];

            if(traverse.isValidLocation()){
                if(boardLookup(traverse)){
                    char c = boardLookup(traverse)->colour;

                    if(c != pieceColour){
                        capturablePieces.push_back(boardLookup(traverse)->position.convertToAlgebraicNotation());
                    }
                }
            }
        }
    }

    return capturablePieces;
}

int board::numLegalMoves(pieceLocation &pLoc){ // returns number of legal moves of a given piece
                                               // does not consider castling, en passant and pieces which cannot moves due to pins or checks
    if(!boardLookup(pLoc)) return 0; // square is empty
    int numMoves = 0;
    
    char pieceColour = boardLookup(pLoc)->colour;
    char pieceId = boardLookup(pLoc)->identity;
    int moves[8][2] = {{0,1}, {0,-1}, {1,0}, {-1,0}, {1,1}, {-1,1}, {1,-1}, {-1,-1}}; // moves to take to move in N,S,E,W,NE,NW,SE or SW directions
    int knightMoves[8][2] = {{1,2}, {-1,2}, {1,-2}, {-1,-2}, {2,1}, {-2,1}, {2,-1}, {-2,-1}};
    int pawnAttacks[2][2] = {{1,1}, {-1,1}}; // mulitply rank by -1 for black
    int pawnMoves[2][2] = {{0,2}, {0,1}};

    pieceLocation traverse = pLoc; // moves along files and diagonals

    if(pieceId == 'Q' || pieceId == 'q'){ // if piece is queen
        for (int i = 1; i <= 8; i++){ 
            traverse.file = traverse.file + moves[i-1][0];
            traverse.rank = traverse.rank + moves[i-1][1];

            while(traverse.isValidLocation()){
                if(boardLookup(traverse)){
                    char c = boardLookup(traverse)->colour;

                    if(c != pieceColour){
                        numMoves++;
                        break;
                    }
                    else break;
                }

                numMoves++;
                traverse.file = traverse.file + moves[i-1][0];
                traverse.rank = traverse.rank + moves[i-1][1];
            }

            traverse = pLoc;
        }
    }
    else if(pieceId == 'R' || pieceId == 'r'){ // if piece is rook
        for (int i = 1; i <= 4; i++){ 
            traverse.file = traverse.file + moves[i-1][0];
            traverse.rank = traverse.rank + moves[i-1][1];

            while(traverse.isValidLocation()){
                if(boardLookup(traverse)){
                    char c = boardLookup(traverse)->colour;

                    if(c != pieceColour){
                        numMoves++;
                        break;
                    }
                    else break;
                }

                numMoves++;
                traverse.file = traverse.file + moves[i-1][0];
                traverse.rank = traverse.rank + moves[i-1][1];
            }

            traverse = pLoc;
        }
    }
    else if(pieceId == 'B' || pieceId == 'b'){ // if piece is bishop
        for (int i = 5; i <= 8; i++){ 
            traverse.file = traverse.file + moves[i-1][0];
            traverse.rank = traverse.rank + moves[i-1][1];

            while(traverse.isValidLocation()){
                if(boardLookup(traverse)){
                    char c = boardLookup(traverse)->colour;

                    if(c != pieceColour){
                        numMoves++;
                        break;
                    }
                    else break;
                }

                numMoves++;
                traverse.file = traverse.file + moves[i-1][0];
                traverse.rank = traverse.rank + moves[i-1][1];
            }

            traverse = pLoc;
        }
    }
    else if(pieceId == 'N' || pieceId == 'n'){ // if piece is Knight
        for (int i = 1; i <= 8; i++) 
        {
            traverse = pLoc;
            traverse.file = traverse.file + knightMoves[i-1][0];
            traverse.rank = traverse.rank + knightMoves[i-1][1];

            if(traverse.isValidLocation()){
                if(boardLookup(traverse)){
                    char c = boardLookup(traverse)->colour;

                    if(c != pieceColour){
                        numMoves++;
                    }
                }
                else numMoves++;
            }
        }
    }
    else if(pieceId == 'K' || pieceId == 'k'){ // if piece is King
        for (int i = 1; i <= 8; i++) 
        {
            traverse = pLoc;
            traverse.file = traverse.file + moves[i-1][0];
            traverse.rank = traverse.rank + moves[i-1][1];

            if(traverse.isValidLocation()){
                if(boardLookup(traverse)){
                    char c = boardLookup(traverse)->colour;

                    if(c != pieceColour){
                        numMoves++;
                    }
                }
                else numMoves++;
            }
        }
    }
    else { // if piece is Pawn
        for (int i = 1; i <= 2; i++) // check attacks
        {
            traverse = pLoc;
            int factor = 1;
            if(pieceColour == 'b') factor = -1;
            traverse.file = traverse.file + pawnAttacks[i-1][0];
            traverse.rank = traverse.rank + factor*pawnAttacks[i-1][1];

            if(traverse.isValidLocation()){
                if(boardLookup(traverse)){
                    char c = boardLookup(traverse)->colour;

                    if(c != pieceColour){
                        numMoves++;
                    }
                }
            }
        }

        for (int i = 1; i <= 2; i++) // check moves
        {
            traverse = pLoc;
            if(i == 1 && pieceColour == 'w' && pLoc.rank != 2) continue;
            if(i == 1 && pieceColour == 'b' && pLoc.rank != 7) continue;

            int factor = 1;
            if(pieceColour == 'b') factor = -1;
            traverse.file = traverse.file + pawnMoves[i-1][0];
            traverse.rank = traverse.rank + factor*pawnMoves[i-1][1];

            if(traverse.isValidLocation()){
                if(!boardLookup(traverse)){
                    numMoves++;
                }
            }
        }
    }

    return numMoves;
}

vector<int> board::numLegalMovesForAttackedPieces(pieceLocation &pLoc){
    vector<string> capturablePieces = piecesAttacked(pLoc);
    vector<int> numMoves;

    for (int i = 0; i < capturablePieces.size(); i++)
    {
        pieceLocation Loc(capturablePieces[i]);
        numMoves.push_back(numLegalMoves(Loc));
    }
    
    return numMoves;
}

vector<string> board::listOfAttackers(pieceLocation &pLoc, char side){ // returns list of direct and stacked attackers at a given square by a given colour
    vector<string> attackers;
    
    int moves[8][2] = {{0,1}, {0,-1}, {1,0}, {-1,0}, {1,1}, {-1,1}, {1,-1}, {-1,-1}}; // moves to take to move in N,S,E,W,NE,NW,SE or SW directions
    int knightMoves[8][2] = {{1,2}, {-1,2}, {1,-2}, {-1,-2}, {2,1}, {-2,1}, {2,-1}, {-2,-1}};
    int pawnAttacks[2][2] = {{1,1}, {-1,1}}; // mulitply rank by -1 for black

    pieceLocation traverse = pLoc; // moves along files and diagonals

    for (int i = 1; i <= 8; i++){
        traverse.file = traverse.file + moves[i-1][0];
        traverse.rank = traverse.rank + moves[i-1][1];

        while(traverse.isValidLocation()){
            if(boardLookup(traverse)){
                char c = boardLookup(traverse)->colour;
                char id = boardLookup(traverse)->identity;

                if(c == side){
                    if(i >= 1 && i <= 4){ // i = 1 to 4 indicates attackers which move along files or ranks, Queen and Rook
                        if(id == 'Q' || id == 'q' || id == 'R' || id == 'r'){
                            attackers.push_back(traverse.convertToAlgebraicNotation());
                        }
                        else break;
                    }
                    else if(i >= 5 && i <= 8){ // i = 5 to 8 indicates attackers which move along diagonals, Queen and Bishop
                        if(id == 'Q' || id == 'q' || id == 'B' || id == 'b'){
                            attackers.push_back(traverse.convertToAlgebraicNotation());
                        }
                        else break;
                    }
                }
                else break;
            }

            traverse.file = traverse.file + moves[i-1][0];
            traverse.rank = traverse.rank + moves[i-1][1];
        }

        traverse = pLoc;
    }

    for (int i = 1; i <= 8; i++) // check enemy knights
    {
        traverse = pLoc;
        traverse.file = traverse.file + knightMoves[i-1][0];
        traverse.rank = traverse.rank + knightMoves[i-1][1];

        if(traverse.isValidLocation()){
            if(boardLookup(traverse)){
                char c = boardLookup(traverse)->colour;
                char id = boardLookup(traverse)->identity;

                if(c == side){
                    if(id == 'N' || id == 'n'){
                        attackers.push_back(traverse.convertToAlgebraicNotation());
                    }
                }
            }
        }
    }

    for (int i = 1; i <= 2; i++) // check enemy pawns
    {
        traverse = pLoc;
        int factor = 1;
        if(side == 'w') factor = -1;
        traverse.file = traverse.file + pawnAttacks[i-1][0];
        traverse.rank = traverse.rank + factor*pawnAttacks[i-1][1];

        if(traverse.isValidLocation()){
            if(boardLookup(traverse)){
                char c = boardLookup(traverse)->colour;
                char id = boardLookup(traverse)->identity;

                if(c == side){
                    if(id == 'p' || id == 'P'){
                        attackers.push_back(traverse.convertToAlgebraicNotation());
                    }
                }
            }
        }
    }
    
    return attackers;
}

bool board::capturesHangingPiece(string move){ // checks whether given move captures a hanging piece
    pieceLocation startPos(move.substr(0,2));
    pieceLocation endPos(move.substr(2,2));

    return boardLookup(startPos)->colour != boardLookup(endPos)->colour && isHanging(endPos);
}

int board::staticEvaluationExchange(pieceLocation &square, string capture){ // calculates SEE for the side who's turn it is at a square
    char sideToMove = activeColour[0];
    char otherSide = sideToMove == 'w' ? 'b' : 'w';
    pieceLocation firstCapture(capture.substr(0,2));

    vector<int> gainList;
    int gain = 0;

    vector<string> attackers = listOfAttackers(square, sideToMove); // attackers == pieces from players side
    vector<string> defenders = listOfAttackers(square, otherSide); // defenders == pieces from opponent side

    map<char, int> pieceValues {{'P', 100}, {'N', 325}, {'B', 350}, {'R', 550}, {'Q', 1000},
                                  {'p', 100}, {'n', 325}, {'b', 350}, {'r', 550}, {'q', 1000}}; 
                                  // arbitrary values assigned to pieces
    
    int valueOnSquare = pieceValues[boardLookup(square)->identity]; // stores value of piece on the square
    gain += valueOnSquare;
    gainList.push_back(gain);
    // cout << gain << " " << boardLookup(firstCapture)->identity << endl;
    valueOnSquare = pieceValues[boardLookup(firstCapture)->identity]; // update value of square with value of capturer
    
    for (auto it = attackers.begin(); it != attackers.end(); ++it){
        if(*it == firstCapture.convertToAlgebraicNotation()){
            attackers.erase(it); // erase piece that makes first capture from list 
            break; 
        }
    }

    int numCaptures = attackers.size() < defenders.size() ? attackers.size() : defenders.size();

    for (int i = 0; i < numCaptures; i++)
    {
        string recapturer = "";
        int lowestValueRecapturer = 1000;
        for (int j = 0; j < defenders.size(); j++){ // find defender with least value that can recapture
            pieceLocation recapturerPos (defenders[j]);

            if(pieceValues[boardLookup(recapturerPos)->identity] <= lowestValueRecapturer){
                lowestValueRecapturer = pieceValues[boardLookup(recapturerPos)->identity];
                recapturer = defenders[j];
            }
        }

        pieceLocation recapturerPos(recapturer);
        gain -= valueOnSquare; // opponent has recaptured your piece!
        gainList.push_back(gain);
        // cout << gain << " " << boardLookup(recapturerPos)->identity << endl;
        valueOnSquare = pieceValues[boardLookup(recapturerPos)->identity];

        for (auto it = defenders.begin(); it != defenders.end(); ++it){
            if(*it == recapturer){
                defenders.erase(it); // erase piece that makes recapture from list 
                break; 
            }
        }

        // attacker captures again

        string capturer = "";
        int lowestValueCapturer = 1000;
        for (int j = 0; j < attackers.size(); j++){ // find attacker with least value that can recapture
            pieceLocation capturerPos (attackers[j]);

            if(pieceValues[boardLookup(capturerPos)->identity] <= lowestValueCapturer){
                lowestValueCapturer = pieceValues[boardLookup(capturerPos)->identity];
                capturer = attackers[j];
            }
        }

        pieceLocation capturerPos(capturer);
        gain += valueOnSquare; // you have captured your opponents piece!
        gainList.push_back(gain);
        // cout << gain << " " << boardLookup(capturerPos)->identity << endl;
        valueOnSquare = pieceValues[boardLookup(capturerPos)->identity];
        
        for (auto it = attackers.begin(); it != attackers.end(); ++it){
            if(*it == capturer){
                attackers.erase(it); // erase piece that makes capture from list 
                break; 
            }
        } 
    }

    if(defenders.size() > 0){ // if there are still remaining pieces in defenders list
        string recapturer = "";
        int lowestValueRecapturer = 1000;
        for (int j = 0; j < defenders.size(); j++){ // find defender with least value that can recapture
            pieceLocation recapturerPos (defenders[j]);

            if(pieceValues[boardLookup(recapturerPos)->identity] <= lowestValueRecapturer){
                lowestValueRecapturer = pieceValues[boardLookup(recapturerPos)->identity];
                recapturer = defenders[j];
            }
        }

        pieceLocation recapturerPos(recapturer);
        gain -= valueOnSquare; // opponent has recaptured your piece!
        gainList.push_back(gain);
        // cout << gain << " " << boardLookup(recapturerPos)->identity << endl;
        valueOnSquare = pieceValues[boardLookup(recapturerPos)->identity];

        for (auto it = defenders.begin(); it != defenders.end(); ++it){
            if(*it == recapturer){
                defenders.erase(it); // erase piece that makes recapture from list 
                break; 
            }
        }
    }

    // gain = gainList[0];

    // for (int i = 1; i < gainList.size(); i++)
    // {
    //     cout << gainList[i] << " ";
    // }
    // cout << endl;
    
    return gain;
}