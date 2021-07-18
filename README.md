# Chess-Interface

This is an attempt at implementing some functions for a Chess Interface. 

piece.cpp contains defintions for the classes "piece" and "pieceLocation". It also contains definitions for the classes "pawn", "knight", "bishop", "rook", "queen" and "king" which inherit from "piece". board.cpp contains definitions for the class "board". The corresponding hpp files contain member declarations and an explanation of what the functions do. main.cpp contains some sample code to demonstrate how to run the functions.

stockfishCommunicate.py queries the engine stockfish to calculate centipawn loss and get the N best moves in a position. 

Run the command
# make
to compile and build an executable for the cpp files named
# myChess 
Run it with the command
# ./myChess

Run stockfishCommunicate.py using the command
#python3 stockfishCommunicate.py
