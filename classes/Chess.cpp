#include "Chess.h"

const int AI_PLAYER = 1;
const int HUMAN_PLAYER = -1;

Chess::Chess()
{
}

Chess::~Chess()
{
}

//
// make a chess piece for the player
//
Bit* Chess::PieceForPlayer(const int playerNumber, ChessPiece piece)
{
    const char* pieces[] = { "pawn.png", "knight.png", "bishop.png", "rook.png", "queen.png", "king.png" ,"movePoint.png","cut.png"};

    // depending on playerNumber load the "x.png" or the "o.png" graphic
    Bit* bit = new Bit();
    // should possibly be cached from player class?
    const char* pieceName = pieces[piece - 1];
    std::string spritePath = std::string("chess/") + (playerNumber == 0 ? "w_" : "b_") + pieceName;
    bit->LoadTextureFromFile(spritePath.c_str());
    bit->setOwner(getPlayerAt(playerNumber));
    bit->setSize(pieceSize, pieceSize);

    return bit;
}
// clear the board and remvoe all bit
void        Chess::clearBoard(){
    for (int y = 0; y < _gameOptions.rowY; y++) {
        for (int x = 0; x < _gameOptions.rowX; x++) {
            _grid[y][x].destroyBit();
            
        }
    }
    //_turns.clear();

}   
//set up board
void Chess::setUpBoard()
{
    setNumberOfPlayers(2);
    _gameOptions.rowX = 8;
    _gameOptions.rowY = 8;
    //
    // we want white to be at the bottom of the screen so we need to reverse the board
    //
    
    char piece[2];
    piece[1] = 0;
    for (int y = 0; y < _gameOptions.rowY; y++) {
        for (int x = 0; x < _gameOptions.rowX; x++) {
            ImVec2 position(((float)(pieceSize * x + pieceSize)) - 60, ((float)(pieceSize * (_gameOptions.rowY - y) + pieceSize))-110);
            _grid[y][x].initHolder(position, "boardsquare.png", x, y);
            _grid[y][x].setGameTag(0);
            piece[0] = bitToPieceNotation(y,x);
            _grid[y][x].setNotation(piece);
        }
    }


    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            ImVec2 position(((float)(pieceSize * x + pieceSize)) +460, ((float)(pieceSize * (_gameOptions.rowY - y) + pieceSize)) - 365);
            player1_capture_grid[y][x].initHolder(position, "captureBacgtound.png", x, y);
            //player1_capture_grid[y][x].setGameTag(0);
            //piece[0] = bitToPieceNotation(y,x);
            //player1_capture_grid[y][x].setNotation(piece);
        }
    }
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            ImVec2 position(((float)(pieceSize * x + pieceSize)) +460, ((float)(pieceSize * (_gameOptions.rowY - y) + pieceSize)) - 110);
            player2_capture_grid[y][x].initHolder(position, "captureBacgtound.png", x, y);
            //player1_capture_grid[y][x].setGameTag(0);
            //piece[0] = bitToPieceNotation(y,x);
            //player1_capture_grid[y][x].setNotation(piece);
        }
    }
    //set up chess
    //player 0
    setUpChessProtect("RNBQKBNRPPPPPPPP00000000000000000000000000000000pppppppprnbqkbnr", true);
}
//set up the chess as string
void        Chess::setUpChessProtect(std::string state, bool init){
//RNBQKBNRPPPPPPPP00000000000000000000000000000000ppppppppprnbkqbnr
    int y = 0;
    int x = 0;
    Bit* bit = nullptr;
    for(unsigned int i = 0 ; i < state.size(); i ++){
        if(x == 8){
            x = 0;
            y++;
        }
        if(state[i] != '0'){
            if(state[i] == 'P' || state[i] == 'p' ){
                if(state[i] == 'P' )
                    bit = PieceForPlayer(0, Pawn);
                else if(state[i] == 'p')
                    bit = PieceForPlayer(1, Pawn);
                bit->setGameTag(Pawn);
            }else if(state[i] == 'R' || state[i] == 'r' ){
                if(state[i] == 'R' )
                    bit = PieceForPlayer(0, Rook);
                else if(state[i] == 'r')
                    bit = PieceForPlayer(1, Rook);
                bit->setGameTag(Rook);
            }else if(state[i] == 'N' || state[i] == 'n' ){
                if(state[i] == 'N' )
                    bit = PieceForPlayer(0, Knight);
                else if(state[i] == 'n')
                    bit = PieceForPlayer(1, Knight);
                bit->setGameTag(Knight);
            }else if(state[i] == 'B' || state[i] == 'b' ){
                if(state[i] == 'B' )
                    bit = PieceForPlayer(0, Bishop);
                else if(state[i] == 'b')
                    bit = PieceForPlayer(1, Bishop);
                bit->setGameTag(Bishop);
            }else if(state[i] == 'K' || state[i] == 'k' ){
                if(state[i] == 'K' )
                    bit = PieceForPlayer(0, King);
                else if(state[i] == 'k')
                    bit = PieceForPlayer(1, King);
                bit->setGameTag(King);
            }else if(state[i] == 'Q' || state[i] == 'q' ){
                if(state[i] == 'Q' )
                    bit = PieceForPlayer(0, Queen);
                else if(state[i] == 'q')
                    bit = PieceForPlayer(1, Queen);
                bit->setGameTag(Queen);
            }
            bit->setPosition(_grid[y][x].getPosition());
            bit->setParent(&_grid[y][x]);
            _grid[y][x].setBit(bit);
            if(init)
            {
                if(bit->gameTag() != Pawn)
                    bit->_init_special = true;
                else
                    bit->_init_special = false;
            }   
            else
                bit->_init_special = false;

        }
        x++;
    }
    /*
    bit->setPosition(player1_capture_grid[0][4].getPosition());
    bit->setParent(&player1_capture_grid[0][4]);
    bit->setGameTag(King);
    player1_capture_grid[0][4].setBit(bit);*/
/*
    Bit* bit = PieceForPlayer(0, King);
    bit->setPosition(_grid[0][4].getPosition());
    bit->setParent(&_grid[0][4]);
    bit->setGameTag(King);
    _grid[0][4].setBit(bit);

    //Pawn
    for (int x = 0; x < _gameOptions.rowX; x++)
    {
        bit = PieceForPlayer(0, Pawn);
        bit->setPosition(_grid[1][x].getPosition());
        bit->setParent(&_grid[1][x]);
        bit->setGameTag(Pawn);
        _grid[1][x].setBit(bit);
        bit->_init_special = false;
    }*/
}
void        Chess::drawCpatureGrid(){
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            player1_capture_grid[y][x].paintSprite();
            if(player1_capture_grid[y][x].bit())
                player1_capture_grid[y][x].bit()->paintSprite();
        }
    }
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            player2_capture_grid[y][x].paintSprite();
            if(player2_capture_grid[y][x].bit())
                player2_capture_grid[y][x].bit()->paintSprite();
        }
    }
}
//
// about the only thing we need to actually fill out for tic-tac-toe
//
bool Chess::actionForEmptyHolder(BitHolder &holder)
{
    return false;
}
//if bit can move to from
bool Chess::canBitMoveFrom(Bit &bit, BitHolder &src)
{
    int x_pos = -1;
    int y_pos = -1;
    //ChessSquare *c = src.getParent().;
    for (int y = 0; y < _gameOptions.rowY; y++)
	{
        if(_grid[y][0].getPosition().y == src.getPosition().y){
            for (int x = 0; x < _gameOptions.rowX; x++)
            {
                if(src.getPosition().x == _grid[y][x].getPosition().x){
                    x_pos = x;
                    y_pos = y;
                    x = 10000;
                    y = 10000;
                }
                //src.bit().get
            }
        }
    }
    if(bit.gameTag() == Pawn && y_pos == 7)
        bit.setGameTag(Queen);
    if(bit.gameTag() == Pawn && y_pos == 0)
        bit.setGameTag(Queen);
    bit_move(bit, x_pos,y_pos);
    
   
    return true;
}
//if bit can move to check
bool Chess::canBitMoveFromTo(Bit& bit, BitHolder& src, BitHolder& dst)
{
    if(dst.highlighted())
    {
        if(dst.bit()){
            if(dst.bit()->gameTag() == -1)
                return true;
            else if(dst.bit()->getOwner()->playerNumber() == bit.getOwner()->playerNumber()){
                _dropTargetSave = nullptr;
                return false;
            }
            else{
                _dropmove = true;
            }
        }

        return true;
    }
    return false;
}
// set the bit as correct movement
void  Chess::bit_move(Bit &bit, int x, int y){
    if(bit.gameTag() == Pawn){
        bit._init_special = false;
        if(bit.getOwner()->playerNumber() == 0){
            if(y + 1< 8){
                if(!_grid[y + 1][x].bit()){
                    check_square(x,y + 1,bit.getOwner()->playerNumber());  
                }
                if(x + 1 < 8 && _grid[y + 1][x + 1].bit())
                    check_square(x + 1,y + 1,bit.getOwner()->playerNumber());
                if(x - 1 >= 0 && _grid[y + 1][x - 1].bit())
                    check_square(x - 1,y + 1,bit.getOwner()->playerNumber());
                if(x + 1 < 8 && _grid[y][x + 1].bit() && _grid[y][x + 1].bit()->gameTag() == Pawn && _grid[y][x + 1].bit()->getOwner()->playerNumber() != 0 &&  _grid[y][x + 1].bit()->_init_special){
                    check_square(x + 1,y + 1,bit.getOwner()->playerNumber());
                    //_grid[y][x + 1].destroyBit();
                }
                if(x - 1 >= 0 && _grid[y][x - 1].bit() && _grid[y][x - 1].bit()->gameTag() == Pawn && _grid[y][x - 1].bit()->getOwner()->playerNumber() != 0 &&  _grid[y][x - 1].bit()->_init_special){
                    check_square(x - 1,y + 1,bit.getOwner()->playerNumber());
                    //_grid[y][x + 1].destroyBit();
                }    
            }
            if(y == 1){
                if(_grid[y + 1][x].bit()->gameTag() == -1 && !_grid[y + 2][x].bit() ){
                    check_square(x,y + 2,bit.getOwner()->playerNumber());
                    bit._init_special = true;
                }

            }
                
        }else if(bit.getOwner()->playerNumber() != 0){
            if(y - 1 >= 0){
                if(!_grid[y - 1][x].bit()){
                    check_square(x,y - 1,bit.getOwner()->playerNumber());  
                    //_grid[0][0].destroyBit();
                }
                if(x + 1 < 8 && _grid[y - 1][x + 1].bit())
                    check_square(x + 1,y - 1,bit.getOwner()->playerNumber());
                if(x - 1 >= 0 && _grid[y - 1][x - 1].bit())
                    check_square(x - 1,y - 1,bit.getOwner()->playerNumber());
                if(x + 1 < 8 && _grid[y][x + 1].bit() && _grid[y][x + 1].bit()->gameTag() == Pawn && _grid[y][x + 1].bit()->getOwner()->playerNumber() == 0 &&  _grid[y][x + 1].bit()->_init_special){
                    check_square(x + 1,y - 1,bit.getOwner()->playerNumber());
                    //_grid[y][x + 1].destroyBit();
                }
                if(x - 1 >= 0 && _grid[y][x - 1].bit() && _grid[y][x - 1].bit()->gameTag() == Pawn && _grid[y][x - 1].bit()->getOwner()->playerNumber() == 0 &&  _grid[y][x - 1].bit()->_init_special){
                    check_square(x - 1,y - 1,bit.getOwner()->playerNumber());
                    //_grid[y][x + 1].destroyBit();
                }    
            }
            if(y == 6){
                if(_grid[y - 1][x].bit()->gameTag() == -1 && !_grid[y - 2][x].bit() ){
                    bit._init_special = true;
                    check_square(x,y - 2,bit.getOwner()->playerNumber());
                }
            }
                
        }

            //_grid[y + 2][x].setHighlighted(true);
        
    }else if(bit.gameTag() == Rook){
        for(int i = x + 1; i <= 7; i ++){
            if(!check_square(i,y,bit.getOwner()->playerNumber()))
                break;
        }
        for(int i = x - 1; i >= 0; i --){
            if(!check_square(i,y,bit.getOwner()->playerNumber()))
                break;
        }
        for(int i = y + 1; i <= 7; i ++){
            if(!check_square(x,i,bit.getOwner()->playerNumber()))
                break;
        }
        for(int i = y - 1; i >= 0; i --){
            if(!check_square(x,i,bit.getOwner()->playerNumber()))
                break;
        }
    }else if(bit.gameTag() == Knight){
        
        if(x + 1 < 8){
            if(y + 2< 8)
                check_square(x + 1,y + 2,bit.getOwner()->playerNumber());
            if(y - 2> 0)
                check_square(x + 1,y - 2,bit.getOwner()->playerNumber());
        }
            
        if(x + 2 < 8)
        {
            if(y + 1 < 8)
                check_square(x + 2,y + 1,bit.getOwner()->playerNumber());  
            if(y - 1 > 0)
                check_square(x + 2,y - 1,bit.getOwner()->playerNumber());
        }
            

        if(x - 1 > 0){
            if(y + 2< 8)
                check_square(x - 1,y + 2,bit.getOwner()->playerNumber());
            if(y - 2> 0)
                check_square(x - 1,y - 2,bit.getOwner()->playerNumber());
        }
            //check_square(x - 1,y ,bit.getOwner()->playerNumber());
        if(x - 2 > 0){
            if(y + 1 < 8)
                check_square(x - 2,y + 1,bit.getOwner()->playerNumber());  
            if(y - 1 > 0)
                check_square(x - 2,y - 1,bit.getOwner()->playerNumber());
        }
            
    }else if(bit.gameTag() == Queen){
        for(int i = x + 1; i <= 7; i ++){
            if(!check_square(i,y,bit.getOwner()->playerNumber()))
                break;
        }
        for(int i = x - 1; i >= 0; i --){
            if(!check_square(i,y,bit.getOwner()->playerNumber()))
                break;
        }
        for(int i = y + 1; i <= 7; i ++){
            if(!check_square(x,i,bit.getOwner()->playerNumber()))
                break;
        }
        for(int i = y - 1; i >= 0; i --){
            if(!check_square(x,i,bit.getOwner()->playerNumber()))
                break;
        }
        int j = y + 1;
        for(int i = x + 1; i < 8 ;i ++){
            if(j < 8){
                if(!check_square(i,j,bit.getOwner()->playerNumber()))
                    break;
            }else
                break;
            j = j + 1;
        }
        j = y - 1;
        for(int i = x + 1; i < 8 ;i ++){
            if(j >= 0){
                if(!check_square(i,j,bit.getOwner()->playerNumber()))
                    break;
            }else
                break;
            j = j - 1;
        }
        j = y + 1;
        for(int i = x - 1; i >= 0 ;i --){
            if(j <= 7){
                if(!check_square(i,j,bit.getOwner()->playerNumber()))
                    break;
            }else
                break;
            j = j + 1;
        }
        j = y - 1;
        for(int i = x - 1; i >= 0 ;i --){
            if(j >= 0){
                if(!check_square(i,j,bit.getOwner()->playerNumber()))
                    break;
            }else
                break;
            j = j - 1;
        }
    }
    else if(bit.gameTag() == King){
        if(x > 0)
             check_square(x - 1,y,bit.getOwner()->playerNumber());
            //_grid[y][x - 1].setHighlighted(true);
        if(x < 7)
            check_square(x + 1,y ,bit.getOwner()->playerNumber());
        if(y > 0)
            check_square(x,y - 1,bit.getOwner()->playerNumber());
        if(y < 7)
            check_square(x,y + 1,bit.getOwner()->playerNumber());
            //_grid[y + 1][x ].setHighlighted(true);
        if(x > 0 && y > 0)
            check_square(x - 1,y - 1,bit.getOwner()->playerNumber());
            
        if(x < 7 && y < 7)
            check_square(x + 1,y + 1,bit.getOwner()->playerNumber());
        if(x != 0 && y != 7)    
            check_square(x - 1,y + 1,bit.getOwner()->playerNumber());
        if(x != 7 && y != 0)  
            check_square(x + 1,y - 1,bit.getOwner()->playerNumber());  
        /*
        if(bit._init_special){//王车易位
            bool can_move_flag = true;
            if( !_grid[x + 2][y].bit() && _grid[x + 1][y].bit() && _grid[x + 1][y].bit()->gameTag() == -1){
                if( _grid[x + 3][y].bit()->getOwner()->playerNumber() == bit.getOwner()->playerNumber() &&  _grid[x + 3][y].bit()->gameTag()== Rook &&  _grid[x + 3][y].bit()->_init_special){
                    
                    int current = x;
                    //int j = y;
                    if(_grid[y + 1][x - 1].bit()->gameTag() != -1 && _grid[y + 1][x - 1].bit()->getOwner()->playerNumber() != bit.getOwner()->playerNumber() ){
                        if(_grid[y + 1][x - 1].bit()->gameTag() == Pawn || _grid[y + 1][x - 1].bit()->gameTag() == King)
                            return;
                    }
                    if(bit.getOwner()->playerNumber() == 0){
                        

                        for(int i = current; i < 3; i++){//if exit chess
                            for(int j = y; j < _gameOptions.rowY; j ++){
                                if(_grid[j][i].bit()){
                                    if(_grid[j][i].bit()->getOwner()->playerNumber() != bit.getOwner()->playerNumber()){
                                        if(_grid[j][i].bit()->gameTag() == Rook || _grid[j][i].bit()->gameTag() == Queen)
                                            return;
                                    }
                                    break;
                                }
                            }
                            int k = y + 1;
                            int j = i + 1;
                            while (k < _gameOptions.rowX && j < _gameOptions.rowY)
                            {
                                if(_grid[k][j].bit()){
                                    if(_grid[k][j].bit()->getOwner()->playerNumber() != bit.getOwner()->playerNumber())
                                    {
                                        if(_grid[k][j].bit()->gameTag() == Bishop || _grid[k][j].bit()->gameTag() == Knight || _grid[i][j].bit()->gameTag() == Queen)
                                            return;
                                    }
                                    break;
                                } 
                                k ++;
                                j ++;
                            }
                            j = i - 1;
                            k = y + 1;
                            while (k >= 0 && j < _gameOptions.rowY)
                            {
                                if(_grid[k][j].bit()){
                                    if(_grid[k][j].bit()->getOwner()->playerNumber() != bit.getOwner()->playerNumber()){
                                        if(_grid[k][j].bit()->gameTag() == Bishop || _grid[k][j].bit()->gameTag() == Knight || _grid[i][j].bit()->gameTag() == Queen){
                                            return;
                                        }
                                    }
                                    break;
                                } 
                                j --;
                                k ++;
                            }
                            //if PAWN and KING

                            if(_grid[y + 1][i + 1].bit() && _grid[y + 1][i + 1].bit()->getOwner()->playerNumber() != bit.getOwner()->playerNumber() ){
                                if(_grid[y + 1][i + 1].bit()->gameTag() == Pawn || _grid[y + 1][i + 1].bit()->gameTag() == King)
                                    return;
                            }



                        }
                        check_square(x + 1,y,bit.getOwner()->playerNumber());

                    }
                    

                }
            }
        }*/

    }else if(bit.gameTag() == Bishop){
        int j = y + 1;
        for(int i = x + 1; i < 8 ;i ++){
            if(j < 8){
                if(!check_square(i,j,bit.getOwner()->playerNumber()))
                    break;
            }else
                break;
            j = j + 1;
        }
        j = y - 1;
        for(int i = x + 1; i < 8 ;i ++){
            if(j >= 0){
                if(!check_square(i,j,bit.getOwner()->playerNumber()))
                    break;
            }else
                break;
            j = j - 1;
        }
        j = y + 1;
        for(int i = x - 1; i >= 0 ;i --){
            if(j <= 7){
                if(!check_square(i,j,bit.getOwner()->playerNumber()))
                    break;
            }else
                break;
            j = j + 1;
        }
        j = y - 1;
        for(int i = x - 1; i >= 0 ;i --){
            if(j >= 0){
                if(!check_square(i,j,bit.getOwner()->playerNumber()))
                    break;
            }else
                break;
            j = j - 1;
        }
    } 
}
// check the square and setting the square as highlight and move point.
bool  Chess::check_square(int x, int y, int player){
    if(_grid[y][x].bit() && _grid[y][x].bit()->gameTag() != -1){
        if(_grid[y][x].bit()->getOwner()->playerNumber() != player){
            _grid[y][x].setHighlighted(true);
        }
            
        return false;
    }else{
        Bit* bit = PieceForPlayer(player, movePoint);
        bit->setPosition(_grid[y][x].getPosition());
        bit->setParent(&_grid[y][x]);
        bit->setGameTag(-1);
        bit->setOwner(getPlayerAt(player));
        _grid[y][x].setBit(bit);
        _grid[y][x].setHighlighted(true);
    }
    
    return true;
}

//Return to Last movement
void         Chess::lastMove(){
    if(_winner != nullptr)
        return;
    if(_turns.size() == 0)
        return;
    else if(_turns.size() == 1){
        setUpBoard();
        //_turns.pop_back();
        return;
    }
      
    Turn* last_turn = _turns[_turns.size() - 2];
    clearBoard();
    setUpChessProtect(last_turn->_boardState);
    if(_turns.size() > 2){
        Turn* current_turn = _turns[_turns.size() - 1];
        if( current_turn->_score != _turns[_turns.size() - 3]->_score){
            if(_gameOptions.currentTurnNo == 2){

                for(int y = 3; y >= 0; y --){
                    for(int x= 3; x >= 0; x --){
                        if(player2_capture_grid[y][x].bit()){
                            player2_capture_grid[y][x].destroyBit();
                            x = -1;
                            y = -1;
                        }
                    }
                }
            }else{
                 for(int y = 3; y >= 0; y --){
                    for(int x= 3; x >= 0; x --){
                        if(player1_capture_grid[y][x].bit()){
                            player1_capture_grid[y][x].destroyBit();
                            x = -1;
                            y = -1;
                        }
                    }
                }
            }
        }

        
    }
    _turns.pop_back();
    if(_gameOptions.currentTurnNo == 1)
        _gameOptions.currentTurnNo = 2;
    else if(_gameOptions.currentTurnNo == 2)
        _gameOptions.currentTurnNo = 1;
}
// capture the bit and move to the rightside board
void        Chess::cpatureBit(Bit& bit,int playerNum){
    if(playerNum == 0){
        for(int y = 0; y < 4; y ++){
            for(int x = 0; x < 4; x ++){
                if(!player1_capture_grid[y][x].bit()){
                    
                    bit.setPosition(player1_capture_grid[y][x].getPosition());
                    bit.setParent(&player1_capture_grid[y][x]);
                    bit.setGameTag(bit.gameTag());
                    player1_capture_grid[y][x].setBit(&bit);
                    return;
                }
            }
        }
    }else if(playerNum == 1){
        for(int y = 0; y < 4; y ++){
            for(int x = 0; x < 4; x ++){
                if(!player2_capture_grid[y][x].bit()){
                    
                    bit.setPosition(player2_capture_grid[y][x].getPosition());
                    bit.setParent(&player2_capture_grid[y][x]);
                    bit.setGameTag(bit.gameTag());
                    player2_capture_grid[y][x].setBit(&bit);
                    return;
                }
            }
        }
    }
}
//move bit code and capture code
void Chess::bitMovedFromTo(Bit &bit, BitHolder &src, BitHolder &dst) {
    BitHolder *dro = &dst;
    if(bit.gameTag() == Pawn && !dst.bit()){
        
        int src_x_pos = getHolerrow(src);
        int src_y_pos = getHolerColumn(src);
        int dst_x_pos =  getHolerrow(dst);
        int dst_y_pos =  getHolerColumn(dst);      
        if(src_x_pos != dst_x_pos){
            if(bit.getOwner()->playerNumber() == 0)
                dro =&_grid[dst_y_pos - 1][dst_x_pos];
            else if(bit.getOwner()->playerNumber() != 0)
                dro =&_grid[dst_y_pos + 1][dst_x_pos];
        //src.getIndex(src_x_pos,src_y_pos);
        }
    }
    if(dro->bit()){
        if(_turns.size() != 0){
            _gameOptions.score = _turns[_turns.size() - 2]->_score;
            if(dro->bit()->gameTag() == Pawn)
                _gameOptions.score += 1;
            else if(dro->bit()->gameTag() == Rook)
                _gameOptions.score += 5;
            else if(dro->bit()->gameTag() == Knight)
                _gameOptions.score += 4;
            else if(dro->bit()->gameTag() == Bishop)
                _gameOptions.score += 3;
            else if(dro->bit()->gameTag() == Queen)
                _gameOptions.score += 9;
            else if(dro->bit()->gameTag() == King){
                _gameOptions.score += 10;
                //endTurn();
                if( _gameOptions.gameNumber != 0)
                    _winner = bit.getOwner();
            }
                
        }
        cpatureBit(*dro->bit(), dro->bit()->getOwner()->playerNumber());
        
        
    }
        
    
    dst.setBit(&bit);
    bit.setParent(&dst);
    bit.moveTo(dst.getPosition());
    src.setBit(nullptr);
    if(bit.getOwner()->playerNumber() == 0)
        _gameOptions.currentTurnNo = 1;
    else
        _gameOptions.currentTurnNo = 2;
    endTurn();
}
// clear the highline and movepoint
void  Chess::clearBoardHighlights(){
    for (int y = 0; y < _gameOptions.rowY; y++) {
        for (int x = 0; x < _gameOptions.rowX; x++) {
            if(_grid[y][x].bit()){
                if(_grid[y][x].bit()->gameTag() == -1){
                    _grid[y][x].destroyBit();
                }
            }
            
            _grid[y][x].setHighlighted(false);
        }
    }

}

//get the Holer Column
int         Chess::getHolerColumn(BitHolder& src){
    int _y = -1;
    for (int y = 0; y < _gameOptions.rowY; y++)
	{
        if(_grid[y][0].getPosition().y == src.getPosition().y){
            _y = y;
            break;
        }
    }
    return _y;
}
// get the Holer row
int         Chess::getHolerrow(BitHolder& src){
    int e = 0;
    for (int x = 0; x < _gameOptions.rowX; x++)
    {
        if(src.getPosition().x == _grid[0][x].getPosition().x){
            e = x;
        }
    }
    return e;
}
//
// free all the memory used by the game on the heap
//
void Chess::stopGame()
{
    _gameOptions.gameNumber = -1;
}

Player* Chess::checkForWinner()
{
    
    return _winner;
}

bool Chess::checkForDraw()
{
    // check to see if the board is full

    return false;
}

//
// add a helper to Square so it returns out FEN chess notation in the form p for white pawn, K for black king, etc.
// this version is used from the top level board to record moves
//
const char Chess::bitToPieceNotation(int row, int column) const {
    if (row < 0 || row >= 8 || column < 0 || column >= 8) {
        return '0';
    }

    const char* wpieces = { "?PNBRQK" };
    const char* bpieces = { "?pnbrqk" };
    unsigned char notation = '0';
    Bit* bit = _grid[row][column].bit();
    if (bit) {
        //notation = bit->gameTag() < 128 ? wpieces[bit->gameTag()] : bpieces[bit->gameTag() & 127];
        if (bit->getOwner()->playerNumber() == 0) {
            notation = wpieces[bit->gameTag()];
        } else {
            notation = bpieces[bit->gameTag() & 127];
        }
    } else {
        notation = '0';
    }
    return notation;
}

//
// state strings
//
std::string Chess::initialStateString()
{
    return stateString();
}

//
// this still needs to be tied into imguis init and shutdown
// we will read the state string and store it in each turn object
//
std::string Chess:: stateString()
{
    std::string s;
    for (int y = 0; y < _gameOptions.rowY; y++) {
        for (int x = 0; x < _gameOptions.rowX; x++) {
            s += bitToPieceNotation(y, x);
        }
    }
    return s;
}

//
// this still needs to be tied into imguis init and shutdown
// when the program starts it will load the current game from the imgui ini file and set the game state to the last saved state
//
void Chess::setStateString(const std::string &s)
{
    for (int y = 0; y < _gameOptions.rowY; y++) {
        for (int x = 0; x < _gameOptions.rowX; x++) {
            int index = y * _gameOptions.rowX + x;
            int playerNumber = s[index] - '0';
            if (playerNumber) {
                _grid[y][x].setBit(PieceForPlayer(playerNumber - 1, Pawn));
            } else {
                _grid[y][x].setBit(nullptr);
            }
        }
    }
}


//
// this is the function that will be called by the AI
//
void Chess::updateAI() 
{
}

