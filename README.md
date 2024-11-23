# Chess AI Implementation Project

![Chess Board](https://raw.githubusercontent.com/zaphodgjd/class-chess-123/main/chess/w_king.png)

## 🎯 Project Overview
This repository contains a skeleton implementation of a Chess AI engine written in C++. The project is designed to teach fundamental concepts of game AI, including board representation, move generation, and basic game tree search algorithms.

### 🎓 Educational Purpose
This project serves as a teaching tool for computer science students to understand:
- Game state representation
- Object-oriented design in C++
- Basic AI concepts in game playing
- Bitboard operations and chess piece movement
- FEN (Forsyth–Edwards Notation) for chess position representation

## 🔧 Technical Architecture

### Key Components
1. **Chess Class**: Core game logic implementation
   - Board state management
   - Move validation
   - Game state evaluation
   - AI player implementation

2. **Piece Representation**
   - Unique identifiers for each piece type
   - Sprite loading and rendering
   - Movement pattern definitions

3. **Board Management**
   - 8x8 grid representation
   - Piece positioning
   - Move history tracking
   - FEN notation support

## 🚀 Getting Started

### Prerequisites
- C++ compiler with C++11 support or higher
- Image loading library for piece sprites
- CMake 3.10 or higher

### Building the Project
```bash
mkdir build
cd build
cmake ..
make
```

### Running Tests
```bash
./chess_tests
```

## 📝 Implementation Details

### Current Features
- Basic board setup and initialization
- Piece movement validation framework
- FEN notation parsing and generation
- Sprite loading for chess pieces
- Player turn management

### Planned Features
- [ ] AI move generation
- [ ] Position evaluation
- [ ] Opening book integration
- [ ] Advanced search algorithms
- [ ] Game state persistence

## 🔍 Code Examples

### Piece Movement Validation
```cpp
bool Chess::canBitMoveFromTo(Bit& bit, BitHolder& src, BitHolder& dst) {
    // TODO: Implement piece-specific movement rules
    return false;
}
```

### FEN Notation Generation
```cpp
const char Chess::bitToPieceNotation(int row, int column) const {
    if (row < 0 || row >= 8 || column < 0 || column >= 8) {
        return '0';
    }
    // Implementation details for FEN notation
}
```

## 📚 Class Assignment Structure

### Phase 1: Board Setup
- Implement piece placement
- Setup initial board state
- Validate board representation

### Phase 2: Move Generation
- Implement basic piece movements
- Add move validation
- Implement special moves (castling, en passant)

### Phase 3: AI Implementation
- Develop position evaluation
- Implement minimax algorithm
- Add alpha-beta pruning
- Basic opening book

## 🤝 Contributing
Students are encouraged to:
1. Fork the repository
2. Create a feature branch
3. Implement assigned components
4. Submit their fork for review

## 🔒 Code Style and Standards
- Use consistent indentation (4 spaces)
- Follow C++ naming conventions
- Document all public methods
- Include unit tests for new features

## 📄 License
This project is licensed under the MIT License.

## 👥 Contributors
- [Your Name] - Initial work
- [Student Names] - Implementation and testing

## 🙏 Acknowledgments
- Chess piece sprites from [Wikipedia](https://en.wikipedia.org/wiki/Chess_piece)
- Original game engine framework by [ocornut](https://github.com/ocornut/imgui)

---
*This README is part of an educational project and is intended to serve as an example of good documentation practices.*

## Chess Function

- Crucial function
1. setUpBoard: print the board and initialize all values.
2. canBitMoveFrom: distinguish the bit move is legal, if legal return true
3. bitMovedFromTo: move the bit can if bit captures another bit remove the capture's bit
4. bit_move: return the bit move and set the legal square as high line
5. setFENfromBoard: set the FEN string by board state
6. canBitMoveFrom get the bit position and set the available move as board
7. setFENfromBoard: set up the board by FEN string
8. undo: The undo function returns to the last move.


- Support function
1. drawCpatureGrid: draw the capture board as right side
2. getFENstring: get FEN string
3. getHolerrow: get the holer row by holer
4. getHolerColumn: et holer column by holer
5. check_square: check the square exits the bit or not set the square; it only be called by bit_move
6. captureBit: capture the bit and move to right side board.
7. clearBoard: clear board and destroy all bit
8. clearBoardHighlights: clear hightline.
9. setStateString: set state string
10. check_the_square_will_be_capture: check the square will be captured by an opponent.
11. endTurn: end the current turn and record the current board state.
12. SetUpState: initialize the current state struct value.
13. PieceForPlayer: return the player chess image by int
14. setFEN: set FEN string by parameter
15. bitToPieceNotation gets bit char from square
## new struct
- struct CurrentState:
- FEN  String
- std::string  FENstring; 
- Castling:
- bool w_K_Castling; 
- bool w_Q_Castling;
- bool b_Q_Castling;
- bool b_K_Castling;
- En Passant record. first row white second row black
- bool  EnPassant[2][8];
- move record(backup)
- std::string	move;
- clock move path
- int half_clock_move;
- int clock_move;
