#pragma once

#include "bitPosition.h"
#include <queue>
#include <string>
#include<map>
#include<list>
class ChessAI
{

public:

    ChessAI(int no = 1);    //init function
    void AI_Move(bit_position& from_pos,bit_position& to_pos); // get AI move
    void setAnotherPlayerMove(bit_position from_pos, bit_position to_pos );// set the opponent move for Ai board
    void init_board();
    void SetAICastle(bool Q_Castling,bool K_Castling);
    int getAINum(){return _AIopinion._num;}
private:
    const int PAWN = 1000;
    const int KNIGHT = 4000;
    const int BISHOP = 4000;
    const int ROOK = 5000;
    const int QUEEN = 9000;
    const int KING = 90000; // Arbitrary high value since the king's value is decisive.  
    struct bit{
        bit(){}
        bit(char t, bit_position pos,int playerNum = 1){ _tag = t; _is_capture = false; _pos = pos; _playerNum = playerNum;}
        

        int _playerNum;
        char _tag;
        bool _is_capture;
        bit_position _pos;
        bit& operator=(bit const& obj){
            _tag = obj._tag;
            _is_capture = obj._is_capture;
            _playerNum = obj._playerNum;
            _pos = obj._pos;
            return *this;
        }
    };
    struct Board_opinion{
        int _num;
        int _currentNum;
        //int _winner;
        //unsigned int _rowX;
        //unsigned int _rowY;
        int _score;
        //CASing

        bool _K_Castling;
        bool _Q_Castling;
        bool  EnPassant[8]; // EN Passent
        std::list<ChessAI::bit> _own;
        //std::list<ChessAI::bit> _player1_own;
        Board_opinion& Board_opinion::operator=(Board_opinion const& obj){
            _num = obj._num;
            _currentNum= obj._currentNum;
            //_winner = obj._winner;
            //_rowX = obj._rowX;
            //_rowY = obj._rowY;
            _score = obj._score;
            _K_Castling = obj._K_Castling;
            _Q_Castling = obj._Q_Castling;
            for(unsigned int i = 0; i < 8; i ++){
                EnPassant[i] = obj.EnPassant[i];
            }
            _own = obj._own;
            
            //_player1_own = obj._player1_own;
            return *this;
        }
    };
     
    int evaluateBoard(char board[8][8], int playerno);
    void updateScore(ChessAI::Board_opinion *player_opinion, ChessAI::Board_opinion *opponent_opinion,char board[8][8]);
    int potentialScore(char piece,int y, int x);
    Board_opinion _AIopinion;
    Board_opinion _OpponentOpinion;
    //int currentNum;
    char _board[8][8];
    void resetBoard(char source[8][8], char destination[8][8]);
    void setUpChess(std::string state);
    void miniMax( ChessAI::Board_opinion *player_opinion, ChessAI::Board_opinion *opponent_opinion, char  board[8][8],  int depth,int currentNum);
    
    int checkWinner();
    void bitMovedFromTo(char bit,bit_position from,  bit_position to, ChessAI::Board_opinion& mover_opinion, ChessAI::Board_opinion& opponent_opinion, char  board[8][8]);
    std::queue<bit_position> bit_move(bit piece, bit_position pos, char  board[8][8],Board_opinion _opinion,int playerNum);
    
    //std::map<char, char[8][8]> _sim;
};