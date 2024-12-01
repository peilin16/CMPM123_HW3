#include "Chess.h"

const int AI_PLAYER = 1;
const int HUMAN_PLAYER = -1;

Chess::Chess()
{
    SetUpState();
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
    SetUpState();
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
    _gameOptions.currentTurnNo = 0;
    //set up chess
    //player 0
    //setFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    FENtoBoard(true);
    if(_gameOptions.gameNumber == 3 ){
        AI_player.init_board();
    }
    //setUpChessProtect("RNBQKBNRPPPPPPPP00000000000000000000000000000000pppppppprnbqkbnr", true);
}
void        Chess::SetUpState(){
    _currentState.FENstring = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    _currentState.w_K_Castling = true;
    _currentState.w_Q_Castling = true;
    _currentState.b_Q_Castling = true;
    _currentState.b_K_Castling = true;
    _currentState.half_clock_move = 0;
    _currentState.clock_move = 0;

    for(int y = 0; y < 2; y ++){
        for(int x =0; x < 8; x ++){
            _currentState.EnPassant[y][x] = false;
        }
    }
}
//set up the chess as string
/*
void        Chess::setUpChessProtect(std::string state, bool init){
//RNBQKBNRPPPPPPPP00000000000000000000000000000000ppppppppprnbkqbnr
    
}*/
/**set fen state to chess board */

void       Chess::FENtoBoard(bool is_first){
    int y = 7;
    int x = 0;
    
    Bit* bit = nullptr;
    bool isSetup = false;
    std::string FENstring = _currentState.FENstring;
    for(unsigned int i = 0 ; i < FENstring.size(); i ++){
        if(!isSetup){
            if(FENstring[i] == '/'){
                y--;
                x = 0;
                continue;
            }
            if(FENstring[i] == ' '){
                isSetup = true;
                continue;
            }
            /*if(FENstring[i] == '1' &&(FENstring[i + 1] == 'p' || FENstring[i + 1] == 'P' || FENstring[i - 1] == 'p' ||FENstring[i - 1] == 'P')){

            }*/
            if(!(FENstring[i] >= '0' && FENstring[i] <='9')){
            
                if(FENstring[i] == 'P' || FENstring[i] == 'p' ){
                    if(FENstring[i] == 'P' )
                        bit = PieceForPlayer(0, Pawn);
                    else if(FENstring[i] == 'p')
                        bit = PieceForPlayer(1, Pawn);
                    bit->setGameTag(Pawn);
                }else if(FENstring[i] == 'R' || FENstring[i] == 'r' ){
                    if(FENstring[i] == 'R' )
                        bit = PieceForPlayer(0, Rook);
                    else if(FENstring[i] == 'r')
                        bit = PieceForPlayer(1, Rook);
                    bit->setGameTag(Rook);
                }else if(FENstring[i] == 'N' || FENstring[i] == 'n' ){
                    if(FENstring[i] == 'N' )
                        bit = PieceForPlayer(0, Knight);
                    else if(FENstring[i] == 'n')
                        bit = PieceForPlayer(1, Knight);
                    bit->setGameTag(Knight);
                }else if(FENstring[i] == 'B' || FENstring[i] == 'b' ){
                    if(FENstring[i] == 'B' )
                        bit = PieceForPlayer(0, Bishop);
                    else if(FENstring[i] == 'b')
                        bit = PieceForPlayer(1, Bishop);
                    bit->setGameTag(Bishop);
                }else if(FENstring[i] == 'K' || FENstring[i] == 'k' ){
                    if(FENstring[i] == 'K' )
                        bit = PieceForPlayer(0, King);
                    else if(FENstring[i] == 'k')
                        bit = PieceForPlayer(1, King);
                    bit->setGameTag(King);
                }else if(FENstring[i] == 'Q' || FENstring[i] == 'q' ){
                    if(FENstring[i] == 'Q' )
                        bit = PieceForPlayer(0, Queen);
                    else if(FENstring[i] == 'q')
                        bit = PieceForPlayer(1, Queen);
                    bit->setGameTag(Queen);
                }
                bit->setPosition(_grid[y][x].getPosition());
                bit->setParent(&_grid[y][x]);
                _grid[y][x].setBit(bit);
                
                if(is_first)
                {
                    if(bit->gameTag() != Pawn)
                        bit->_init_special = true;
                    else
                        bit->_init_special = false;
                }   
                else
                    bit->_init_special = false;
                x++;
            }else{
                x += FENstring[i] - '0';
            }

        }else{


        }
        //x++;
   }


}
/**get fen from chess board */
void     Chess::setFENfromBoard(){
    std::string fen_str = "";
    char s = '0';
    int space = 0;
    //get boad state
    for (int y =  _gameOptions.rowY - 1; y >= 0; y--) {
        for (int x = 0; x < _gameOptions.rowX ; x++) {

            s = bitToPieceNotation(y, x);
            if(s == '0')
            {
                space ++;
                continue;
            }
            if(space != 0){
                fen_str += space +'0';
                //fen_str += 
                space = 0;
                //continue;
            }
            fen_str += s;
        }
        if(space != 0){
            fen_str += space +'0';
            space = 0;
        }
        if(y != 0)
            fen_str += '/';
    }
    fen_str += ' ';

    _currentState.clock_move ++ ;
    if(_gameOptions.gameNumber != 0){
        if(_gameOptions.currentTurnNo == 0)
			fen_str += 'w';
		else if(_gameOptions.currentTurnNo == 1){
            fen_str += 'b';
            _currentState.half_clock_move ++;
        }
			
    }else{
        fen_str += 'n'; //in free mode there are not exist player turn n is none
    }
    fen_str += ' ';
    _currentState.w_K_Castling = false;
    _currentState.w_Q_Castling = false;
    //check Castling for white side

    if(_grid[0][4].bit() && _grid[0][4].bit()->_init_special ){
        //if(_grid[0][4].bit()->getOwner()->playerNumber() == _grid[0][7].bit()->getOwner()->playerNumber()){
        int p = _grid[0][4].bit()->getOwner()->playerNumber();
        
        if(check_the_square_will_be_capture(0, 4,p)){
            if( _grid[0][7].bit() && _grid[0][7].bit()->_init_special &&  check_the_square_will_be_capture(0, 5,p ) && check_the_square_will_be_capture(0, 6,p ) ){
                fen_str += 'K';
                _currentState.w_K_Castling = true;
            }
            if(_grid[0][0].bit() && _grid[0][0].bit()->_init_special && check_the_square_will_be_capture(0, 3,p ) && check_the_square_will_be_capture(0, 2,p )){
                fen_str += 'Q';
                _currentState.w_Q_Castling = true;
            }
        }
        
      //  }
    }
    _currentState.b_K_Castling = false;
    _currentState.b_Q_Castling = false;
    //check Castling for black side
    if(_grid[7][4].bit() && _grid[7][4].bit()->_init_special ){
        int p = _grid[7][4].bit()->getOwner()->playerNumber();
        
        if(check_the_square_will_be_capture(7, 4,p)){
            if(_grid[7][7].bit() && _grid[7][7].bit()->_init_special && check_the_square_will_be_capture(7, 5,p) && check_the_square_will_be_capture(7, 6,p) ){
                fen_str += 'k';
                _currentState.b_K_Castling = true;
            }
            if(_grid[7][0].bit() && _grid[7][0].bit()->_init_special && check_the_square_will_be_capture(7, 3,p) && check_the_square_will_be_capture(7, 2,p)){
                fen_str += 'q';
                _currentState.b_Q_Castling = true;
            }
        }
        

    }
    if(!_currentState.w_K_Castling && !_currentState.w_Q_Castling && !_currentState.b_K_Castling && !_currentState.b_Q_Castling)
        fen_str += '-';
    fen_str += ' ';
    bool is_set = false;

    // remove en passant
    if(_gameOptions.currentTurnNo == 0){
        for (size_t x = 0; x < 8; x++){
            _currentState.EnPassant[0][x] = false;
            if(_currentState.EnPassant[1][x]){
                is_set = true;
                fen_str += 'a' + x;
                fen_str += '3';
                fen_str += ' ';
            }
        }
            
    }else{
        for (size_t x = 0; x < 8; x++){
            _currentState.EnPassant[1][x] = false;
            if(_currentState.EnPassant[0][x]){
                is_set = true;
                fen_str += 'a' + x;
                fen_str += '5';
                fen_str += ' ';
            }
        }
            
    }
    
    if(!is_set)
        fen_str += '-';
    if(_gameOptions.gameNumber == 0){
        fen_str += ' ';
        fen_str +='-';
        fen_str +=' ';
        fen_str += '-';
    }else{
        fen_str += std::to_string(_currentState.clock_move);
        fen_str += ' ';
        fen_str += std::to_string(_currentState.half_clock_move);
    }

    _currentState.FENstring = fen_str;
    //return fen_str;
}
bool Chess::check_the_square_will_be_capture(int y, int x,int playerNumber){
    //check upper
    for(int i = y + 1; i < _gameOptions.rowY; i ++){
        if(_grid[i][x].bit()){
            if(_grid[i][x].bit()->getOwner()->playerNumber() == playerNumber)
                break;
            else{
                if(_grid[i][x].bit()->gameTag() == Queen || _grid[i][x].bit()->gameTag() == Rook )
                    return false;
                else if(i == y + 1 && _grid[i][x].bit()->gameTag() == King )
                    return false;
                break;
            }
                
        }
    }
    //check left
    for(int i = x - 1; i >= 0; i --){
        if(_grid[y][i].bit()){
            if(_grid[y][i].bit()->getOwner()->playerNumber() == playerNumber)
                break;
            else{
                if(_grid[y][i].bit()->gameTag() == Queen || _grid[y][i].bit()->gameTag() == Rook )
                    return false;
                else if(i == x - 1 && _grid[y][i].bit()->gameTag() == King )
                    return false;
                break;
            }
        }
    }
    //check right
    for(int i = x + 1; i < _gameOptions.rowX; i ++){
        if(_grid[y][i].bit()){
            if(_grid[y][i].bit()->getOwner()->playerNumber() == playerNumber)
                break;
            else{
                if(_grid[y][i].bit()->gameTag() == Queen || _grid[y][i].bit()->gameTag() == Rook )
                    return false;
                else if(i == x + 1 && _grid[y][i].bit()->gameTag() == King )
                    return false;
                break;
            }
        }
    }
    //check down
    for(int i = y - 1; i >=0; i --){
        if(_grid[i][x].bit()){
            if(_grid[i][x].bit()->getOwner()->playerNumber() == playerNumber)
                break;
            else{
                if(_grid[i][x].bit()->gameTag() == Queen || _grid[i][x].bit()->gameTag() == Rook )
                    return false;
                else if(i == y - 1 && _grid[i][x].bit()->gameTag() == King )
                    return false;
                break;
            }
        }
    }
    //check upper left diagonal
    for(int i = y + 1; i < _gameOptions.rowY; i ++){
        for(int j = x - 1; j >= 0; j --){
            if(_grid[i][j].bit()){
                if(_grid[i][j].bit()->getOwner()->playerNumber() == playerNumber){
                    i= 1000;
                    break;
                }
                    
                else{
                    if(_grid[i][j].bit()->gameTag() == Queen || _grid[i][j].bit()->gameTag() == Bishop )
                        return false;
                    else if(i == y + 1 && j == x - 1 && (_grid[i][j].bit()->gameTag() == King || _grid[i][j].bit()->gameTag() == Pawn ))
                        return false;
                    i = 1000;
                    break;
                }
                    
            }
        }
    }
    //check upper righ diagonal
    for(int i = y + 1; i < _gameOptions.rowY; i ++){
        for(int j = x + 1; j < _gameOptions.rowX; j ++){
            if(_grid[i][j].bit()){
                if(_grid[i][j].bit()->getOwner()->playerNumber() == playerNumber){
                    i= 1000;
                    break;
                }
                else{
                    if(_grid[i][j].bit()->gameTag() == Queen || _grid[i][j].bit()->gameTag() == Bishop )
                        return false;
                    else if(i == y + 1 && j == x + 1 && (_grid[i][j].bit()->gameTag() == King || _grid[i][j].bit()->gameTag() == Pawn ))
                        return false;
                    i = 1000;
                    break;
                }
            }
        }
    }
    //check down left diagonal
    for(int i = y - 1; i >=0; i --){
        for(int j = x - 1; j >= 0; j --){
            if(_grid[i][j].bit()){
                if(_grid[i][j].bit()->getOwner()->playerNumber() == playerNumber){
                    i = -1;
                    break;
                }
                else{
                    if(_grid[i][j].bit()->gameTag() == Queen || _grid[i][j].bit()->gameTag() == Bishop )
                        return false;
                    else if(i == y - 1 && j == x - 1 && (_grid[i][j].bit()->gameTag() == King || _grid[i][j].bit()->gameTag() == Pawn ))
                        return false;
                    i = -1;
                    break;
                }
            }
        }
    }
    //check down righ diagonal
    for(int i = y - 1; i >=0; i --){
        for(int j = x + 1; j < _gameOptions.rowX; j ++){
            if(_grid[i][j].bit()){
                if(_grid[i][j].bit()->getOwner()->playerNumber() == playerNumber){
                    i = -1;
                    break;
                }
                else{
                    if(_grid[i][j].bit()->gameTag() == Queen || _grid[i][j].bit()->gameTag() == Bishop )
                        return false;
                    else if(i == y - 1 && j == x + 1 && (_grid[i][j].bit()->gameTag() == King || _grid[i][j].bit()->gameTag() == Pawn ))
                        return false;
                    i = -1;
                    break;
                    
                }
            }
        }
    }
    //check knight
    if(x + 1 < _gameOptions.rowX){
        if(y + 2< _gameOptions.rowY){
            if(_grid[y + 2][x + 1].bit() && _grid[y + 2][x + 1].bit()->getOwner()->playerNumber() != playerNumber){
                if(_grid[y + 2][x + 1].bit()->gameTag() == Knight)
                    return false;
            }
        }
            
        if(y - 2>= 0){
            if(_grid[y - 2][x + 1].bit() && _grid[y - 2][x + 1].bit()->getOwner()->playerNumber() != playerNumber){
                if(_grid[y - 2][x + 1].bit()->gameTag() == Knight)
                    return false;
            }
        }
            
    }
            
    if(x + 2 < _gameOptions.rowX)
    {
        if(y + 1 < _gameOptions.rowY)
        {
            if(_grid[y + 1][x + 2].bit() && _grid[y + 1][x + 2].bit()->getOwner()->playerNumber() != playerNumber){
                if(_grid[y + 1][x + 2].bit()->gameTag() == Knight)
                    return false;
            }
        } 
        if(y - 1 >= 0){
            if(_grid[y - 1][x + 2].bit() && _grid[y - 1][x + 2].bit()->getOwner()->playerNumber() != playerNumber){
                if(_grid[y - 1][x + 2].bit()->gameTag() == Knight)
                    return false;
            }
        }
    }
            

    if(x - 1 >= 0){
        if(y + 2<  _gameOptions.rowY){
            if(_grid[y + 2][x - 1].bit() && _grid[y + 2][x - 1].bit()->getOwner()->playerNumber() != playerNumber){
                if(_grid[y + 2][x - 1].bit()->gameTag() == Knight)
                    return false;
            }
        }
        if(y - 2>= 0){
            if(_grid[y - 2][x - 1].bit() && _grid[y - 2][x - 1].bit()->getOwner()->playerNumber() != playerNumber){
                if(_grid[y - 2][x - 1].bit()->gameTag() == Knight)
                    return false;
            }
        }
                
    }
    if(x - 2 >= 0){
            if(y + 1 <  _gameOptions.rowY){
                if(_grid[y + 1][x - 2].bit() && _grid[y + 1][x - 2].bit()->getOwner()->playerNumber() != playerNumber){
                    if(_grid[y + 1][x - 2].bit()->gameTag() == Knight)
                        return false;
                }
            }
            if(y - 1 >= 0){
                if(_grid[y - 1][x - 2].bit() && _grid[y - 1][x - 2].bit()->getOwner()->playerNumber() != playerNumber){
                    if(_grid[y - 1][x - 2].bit()->gameTag() == Knight)
                        return false;
                }
            }
        }

    return true;
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
                if(y == 4){
                    if(x < _gameOptions.rowX && _currentState.EnPassant[1][x + 1] &&  !_grid[y + 1][x + 1].bit())
                        check_square(x + 1,y + 1,bit.getOwner()->playerNumber());
                    if(x >= 0 && _currentState.EnPassant[1][x - 1])
                        check_square(x - 1,y + 1,bit.getOwner()->playerNumber() && ! _grid[y + 1][x - 1].bit());
                }
            }
            if(y == 1){
                if(_grid[y + 1][x].bit()->gameTag() == -1 && !_grid[y + 2][x].bit() ){
                    check_square(x,y + 2,bit.getOwner()->playerNumber());
                    _currentState.EnPassant[0][x] = true;
                    
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
               /* if(x + 1 < 8 && _grid[y][x + 1].bit() && _grid[y][x + 1].bit()->gameTag() == Pawn && _grid[y][x + 1].bit()->getOwner()->playerNumber() == 0 &&  _grid[y][x + 1].bit()->_init_special){
                    check_square(x + 1,y - 1,bit.getOwner()->playerNumber());
                    //_grid[y][x + 1].destroyBit();
                }
                if(x - 1 >= 0 && _grid[y][x - 1].bit() && _grid[y][x - 1].bit()->gameTag() == Pawn && _grid[y][x - 1].bit()->getOwner()->playerNumber() == 0 &&  _grid[y][x - 1].bit()->_init_special){
                    check_square(x - 1,y - 1,bit.getOwner()->playerNumber());
                    //_grid[y][x + 1].destroyBit();
                } */  
               if(y == 3){
                    if(x < _gameOptions.rowX && _currentState.EnPassant[0][x + 1] &&  !_grid[y - 1][x + 1].bit())
                        check_square(x + 1,y - 1,bit.getOwner()->playerNumber());
                    if(x >= 0 && _currentState.EnPassant[0][x - 1])
                        check_square(x - 1,y - 1,bit.getOwner()->playerNumber() && ! _grid[y - 1][x - 1].bit());
                }
            }
            if(y == 6){
                if(_grid[y - 1][x].bit()->gameTag() == -1 && !_grid[y - 2][x].bit() ){
                    //bit._init_special = true;
                    _currentState.EnPassant[1][x] = true;
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
            if(y - 2>= 0)
                check_square(x + 1,y - 2,bit.getOwner()->playerNumber());
        }
            
        if(x + 2 < 8)
        {
            if(y + 1 < 8)
                check_square(x + 2,y + 1,bit.getOwner()->playerNumber());  
            if(y - 1 >= 0)
                check_square(x + 2,y - 1,bit.getOwner()->playerNumber());
        }
            

        if(x - 1 > 0){
            if(y + 2< 8)
                check_square(x - 1,y + 2,bit.getOwner()->playerNumber());
            if(y - 2>= 0)
                check_square(x - 1,y - 2,bit.getOwner()->playerNumber());
        }
            //check_square(x - 1,y ,bit.getOwner()->playerNumber());
        if(x - 2 > 0){
            if(y + 1 < 8)
                check_square(x - 2,y + 1,bit.getOwner()->playerNumber());  
            if(y - 1 >= 0)
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
        if(bit.getOwner()->playerNumber() == 0){
            if(_currentState.w_K_Castling && _grid[0][5].bit()->gameTag() == -1 && !_grid[0][6].bit()){
                check_square(x + 2,0,bit.getOwner()->playerNumber());  
            }
            if(_currentState.w_Q_Castling && !_grid[0][1].bit() && !_grid[0][2].bit()&& _grid[0][3].bit()->gameTag()== -1){
                check_square(x - 2,0,bit.getOwner()->playerNumber());  
            }
        }else{
            if(_currentState.b_K_Castling && _grid[7][5].bit()->gameTag() == -1 && !_grid[7][6].bit()){
                    check_square(x + 2,7,bit.getOwner()->playerNumber());  
            }
            if(_currentState.b_Q_Castling && !_grid[7][1].bit()  && !_grid[7][2].bit()&& _grid[7][3].bit()->gameTag() == -1){
                check_square(x - 2,7,bit.getOwner()->playerNumber());  
            }
        }
        

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
void         Chess::undo(){
    if(_winner != nullptr)
        return;
    if(_turns.size() == 0)
        return;
    else if(_turns.size() == 1){
        setUpBoard();
        //_turns.pop_back();
        return;
    }
      
    Turn* last_turn = _turns[_turns.size() - 1];
    clearBoard();
    setFEN(last_turn->_boardState);
    FENtoBoard();
    //setUpChessProtect(last_turn->_boardState);
    if(_turns.size() > 2){
        Turn* current_turn = _turns[_turns.size() - 1];
        if( current_turn->_score != _turns[_turns.size() - 3]->_score){
            if(_gameOptions.currentTurnNo == 1){

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
    if(_gameOptions.currentTurnNo == 0)
        _gameOptions.currentTurnNo = 1;
    else if(_gameOptions.currentTurnNo != 0)
        _gameOptions.currentTurnNo = 0;
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
    //EN Pawn
    bit_position src_pos = getHolerPos(src);
    bit_position dst_pos = getHolerPos(dst);
    if(bit.gameTag() == Pawn && !dst.bit()){

        if(src_pos.x_position != dst_pos.x_position){
            if(bit.getOwner()->playerNumber() == 0)
            {
                dro =&_grid[dst_pos.y_position - 1][dst_pos.x_position];
                _currentState.EnPassant[1][dst_pos.x_position] = false;
            }
            else if(bit.getOwner()->playerNumber() != 0){
                dro =&_grid[dst_pos.y_position + 1][dst_pos.x_position];
                _currentState.EnPassant[0][dst_pos.x_position] = false;
            }
                
        //src.getIndex(src_x_pos,src_y_pos);
        }
    }
    //Castling move
    if(bit.gameTag() == King && bit._init_special){

        if(dst_pos.x_position == src_pos.x_position - 2 || dst_pos.x_position == src_pos.x_position  + 2){
            Bit *rook = nullptr;
            if(bit.getOwner()->playerNumber() == 0){
                if(dst_pos.x_position == src_pos.x_position + 2){
                    rook = _grid[0][_gameOptions.rowX - 1].bit();
                    _grid[0][src_pos.x_position + 1].setBit(rook);
                    rook->_init_special = false;
                    rook->setParent(&_grid[0][src_pos.x_position + 1]);
                    rook->moveTo(_grid[0][src_pos.x_position + 1].getPosition());
                    _grid[0][_gameOptions.rowX - 1].setBit(nullptr);
                }else if(dst_pos.x_position == src_pos.x_position - 2){
                    rook = _grid[0][0].bit();
                    _grid[0][src_pos.x_position - 1].setBit(rook);
                    rook->_init_special = false;
                    rook->setParent(&_grid[0][src_pos.x_position - 1]);
                    rook->moveTo(_grid[0][src_pos.x_position - 1].getPosition());
                    _grid[0][0].setBit(nullptr);
                }
            }else{
                if(dst_pos.x_position == src_pos.x_position + 2){
                    rook = _grid[7][_gameOptions.rowX - 1].bit();
                    _grid[7][src_pos.x_position + 1].setBit(rook);
                    rook->_init_special = false;
                    rook->setParent(&_grid[7][src_pos.x_position + 1]);
                    rook->moveTo(_grid[7][src_pos.x_position + 1].getPosition());
                    _grid[7][_gameOptions.rowX - 1].setBit(nullptr);
                }else if(dst_pos.x_position == src_pos.x_position - 2){
                    rook = _grid[7][0].bit();
                    _grid[7][src_pos.x_position - 1].setBit(rook);
                    rook->_init_special = false;
                    rook->setParent(&_grid[7][src_pos.x_position - 1]);
                    rook->moveTo(_grid[7][src_pos.x_position - 1].getPosition());
                    _grid[7][0].setBit(nullptr);
                }
            }
        }
    }


    // capture bit
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
    bit._init_special = false;    
    dst.setBit(&bit);
    bit.setParent(&dst);
    bit.moveTo(dst.getPosition());
    src.setBit(nullptr);
    
    endTurn();
    if(_gameOptions.currentTurnNo == 0)
        _gameOptions.currentTurnNo = 1;
    else
        _gameOptions.currentTurnNo = 0;
    setFENfromBoard();
    
    if(_gameOptions.AIPlaying && _gameOptions.currentTurnNo == AI_player.getAINum()){
        AI_player.setAnotherPlayerMove(src_pos,dst_pos);
        updateAI();
        if(_gameOptions.currentTurnNo == 0)
            _gameOptions.currentTurnNo = 1;
        else
            _gameOptions.currentTurnNo = 0;
        setFENfromBoard();

    }

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


bit_position Chess::getHolerPos(BitHolder& src){
    int _y = -1;
    for (int y = 0; y < _gameOptions.rowY; y++)
	{
        if(_grid[y][0].getPosition().y == src.getPosition().y){
            _y = y;
            break;
        }
    }
    int e = 0;
    for (int x = 0; x < _gameOptions.rowX; x++)
    {
        if(src.getPosition().x == _grid[0][x].getPosition().x){
            e = x;
        }
    }
    return bit_position(_y,e);

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
    //std::string fen_str = getFENfromBoard();
    /*
    std::string s;
    for (int y = 0; y < _gameOptions.rowY; y++) {
        for (int x = 0; x < _gameOptions.rowX; x++) {
            s += bitToPieceNotation(y, x);
        }
    }*/
    return _currentState.FENstring;
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


    bit_position from_pos;
    bit_position to_pos;
    AI_player.AI_Move(from_pos,to_pos);
    bitMove(from_pos,to_pos);
    /*
    */

                
}

void        Chess::bitMove(bit_position from,bit_position to){
    Bit* move = _grid[from.y_position][from.x_position].bit();


    if(_grid[to.y_position][to.x_position].bit()){
            // capture bit
        Bit *dro = _grid[to.y_position][to.x_position].bit();
        if(_turns.size() != 0){
            _gameOptions.score = _turns[_turns.size() - 2]->_score;
            if(dro->gameTag() == Pawn)
                _gameOptions.score += 1;
            else if(dro ->gameTag() == Rook)
                _gameOptions.score += 5;
            else if(dro ->gameTag() == Knight)
                _gameOptions.score += 4;
            else if(dro ->gameTag() == Bishop)
                _gameOptions.score += 3;
            else if(dro ->gameTag() == Queen)
                _gameOptions.score += 9;
            else if(dro ->gameTag() == King){
                _gameOptions.score += 10;
                if( _gameOptions.gameNumber != 0)
                    _winner = move->getOwner();
            }
                
        }
        cpatureBit(*dro , dro->getOwner()->playerNumber());
        
    }
    _grid[to.y_position][to.x_position].setBit(move);
    move->_init_special = false;
    move->setParent(&_grid[to.y_position][to.x_position]);
    move->moveTo(_grid[to.y_position][to.x_position].getPosition());
    _grid[to.y_position][to.x_position].setBit(move);
}