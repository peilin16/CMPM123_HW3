#pragma once
#include "Game.h"
#include "ChessSquare.h"
const int pieceSize = 64;

enum ChessPiece {
    NoPiece = 0,
    Pawn = 1,
    Knight,
    Bishop,
    Rook,
    Queen,
    King,
    movePoint,
    cut
};
struct CurrentState{
    std::string  FENstring;
    bool w_K_Castling;
    bool w_Q_Castling;
    bool b_Q_Castling;
    bool b_K_Castling;
    bool  EnPassant[2][8]; // first row white second row black
    std::string	move;
    int half_clock_move;
    int clock_move;
};
//
// the main game class
//
class Chess : public Game
{
public:
    Chess();
    ~Chess();

    // set up the board
    void        setUpBoard() override;

    Player*     checkForWinner() override;
    bool        checkForDraw() override;
    std::string initialStateString() override;
    std::string stateString() override;
    void        setStateString(const std::string &s) override;
    bool        actionForEmptyHolder(BitHolder& holder) override;
    bool        canBitMoveFrom(Bit& bit, BitHolder& src) override;
    bool        canBitMoveFromTo(Bit& bit, BitHolder& src, BitHolder& dst) override;
    void        bitMovedFromTo(Bit &bit, BitHolder &src, BitHolder &dst) override;
    void        clearBoard();        
    void        stopGame() override;
    BitHolder& getHolderAt(const int x, const int y) override { return _grid[y][x]; }
    void        clearBoardHighlights() override;
	void        updateAI() override;
    bool        gameHasAI() override { return true; }
    void        drawCpatureGrid();
    void        undo();
    std::string getFENstring(){return _currentState.FENstring;}
    void        setFENfromBoard();
    
private:
    void        SetUpState();
    void        setFEN(std::string str){
        _currentState.FENstring = str;
    }
    bool      check_the_square_will_be_capture(int y, int x,int playerNumber);
    void       FENtoBoard(bool is_first = false);
    
    //void        setUpChessProtect(std::string state , bool init = false);
    int         getHolerColumn(BitHolder& src);
    void        cpatureBit(Bit& bit,int playerNum);
    int         getHolerrow(BitHolder& src);
    Bit *       PieceForPlayer(const int playerNumber, ChessPiece piece);
    const char  bitToPieceNotation(int row, int column) const;
    void        bit_move(Bit &bit, int x, int y);
    bool        check_square(int x, int y, int player);//if square exit bit
    CurrentState    _currentState;
    ChessSquare      _grid[8][8];
    ChessSquare      player1_capture_grid[4][4];
    ChessSquare      player2_capture_grid[4][4];
};

