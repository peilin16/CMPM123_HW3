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
    void        lastMove();
    int         chance;
private:
    void        setUpChessProtect(std::string state , bool init = false);
    int        getHolerColumn(BitHolder& src);
    void        cpatureBit(Bit& bit,int playerNum);
    int        getHolerrow(BitHolder& src);
    Bit *       PieceForPlayer(const int playerNumber, ChessPiece piece);
    const char  bitToPieceNotation(int row, int column) const;
    void        bit_move(Bit &bit, int x, int y);
    bool        check_square(int x, int y, int player);//if square exit bit

    ChessSquare      _grid[8][8];
    ChessSquare      player1_capture_grid[4][4];
    ChessSquare      player2_capture_grid[4][4];
};

