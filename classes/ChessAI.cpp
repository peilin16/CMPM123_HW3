#include"ChessAI.h"
ChessAI::ChessAI(int Num){
    _AIopinion._num = Num;
    if(Num == 0)
        _OpponentOpinion._num = 1;
    else
        _OpponentOpinion._num = 0;
    init_board();
}

void ChessAI::init_board(){
    
    _AIopinion._K_Castling = true;
    _AIopinion._Q_Castling = true;
    //_opinion._rowX = 8 ;
    //_opinion._rowY = 8;
    //_AIopinion._winner = -1;
    _AIopinion._score = 0;
    for(int i = 0; i <8; i ++){
        _AIopinion.EnPassant[i] = false;
    }
    //_AIopinion._currentNum = 0;
    _OpponentOpinion._score = 0;
    _OpponentOpinion._K_Castling = true;
    _OpponentOpinion._Q_Castling = true;
    //_opinion._rowX = 8 ;
    //_opinion._rowY = 8;
    //_AIopinion._winner = -1;
    for(int i = 0; i <8; i ++){
        _OpponentOpinion.EnPassant[i] = false;
    }
    _AIopinion._own.clear();
    _OpponentOpinion._own.clear();
    //_OpponentOpinion._currentNum = 0;
    setUpChess("RNBQKBNRPPPPPPPP00000000000000000000000000000000pppppppprnbqkbnr");
}
//set up the chess
void ChessAI::setUpChess(std::string state){
    //RNBQKBNRPPPPPPPP00000000000000000000000000000000ppppppppprnbkqbnr
    unsigned int y = 0;
    unsigned int x = 0;
    
    for(unsigned int i = 0 ; i < state.size(); i ++){
        if(x >=  8){
            x = 0;
            y++;
        }
        if(state[i] != '0'){
            if(isupper(state[i])){
                if(_AIopinion._num == 0)
                    _AIopinion._own.push_back(bit(state[i],bit_position(y,x), _AIopinion._num));
                else
                    _OpponentOpinion._own.push_back(bit(state[i],bit_position(y,x),_OpponentOpinion._num));
            }else{
                if(_AIopinion._num == 1)
                    _AIopinion._own.push_back(bit(state[i],bit_position(y,x), _AIopinion._num));
                else
                    _OpponentOpinion._own.push_back(bit(state[i],bit_position(y,x),_OpponentOpinion._num));
            }

        }
        _board[y][x] = state[i];
        x++;
    }
}
void ChessAI::SetAICastle(bool Q_Castling,bool K_Castling){

}
//evaluate the score from board
int ChessAI::evaluateBoard(char board[8][8], int playerno  ){

    //80+60+60+100+90+900
    const int total= 13800;//12900;
    int white_score = 0;
    int black_score= 0;
    int pieceValue = 0;
    char temp[8][8];
    resetBoard(board, temp);
    // Iterate through the board
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            char piece = board[y][x];
            int piece_value = 0;

            // Determine material value
            switch (tolower(piece)) {
                case 'p': piece_value = PAWN; break;
                case 'n': piece_value = KNIGHT; break;
                case 'b': piece_value = BISHOP; break;
                case 'r': piece_value = ROOK; break;
                case 'q': piece_value = QUEEN; break;
                case 'k': piece_value = KING; break;
                default: continue; // Empty square or invalid piece
            }

            // Get positional score for the piece
            int positional_value = potentialScore(piece, y, x);

            // Add to appropriate score
            if (islower(piece)) {
                black_score += piece_value + positional_value;
            } else if (isupper(piece)) {
                white_score += piece_value + positional_value;
            }
        }
    }


   // int e = white_score;
   // white_score += total - black_score;
    //black_score += total - e;

    // Optional: Add positional score using tables
    //score += positional_score(piece, i, j);
    if(playerno == 0)
        return white_score;
    else
        return black_score;
}
/*
int ChessAI::evaluateCapture(ChessAI::Board_opinion *player_opinion, ChessAI::Board_opinion *opponent_opinion){
    for(auto& piece : _AIopinion._own)
    return 0 ;
}*/
// get potential score
int ChessAI::potentialScore(char piece,int y, int x){
    int score = 0;
    if(piece == 'p' || piece =='P'){
        if( piece =='p'){
            int pawn_table[8][8] = {
                {  0,  5,  5,  0,  0,  5,  5,  0 },
                { 50, 50, 50, 50, 50, 50, 50, 50 },
                { 10, 10, 20, 30, 30, 20, 10, 10 },
                {  5,  5, 10, 25, 25, 10,  5,  5 },
                {  0,  0,  0, 20, 20,  0,  0,  0 },
                {  5, -5, -10,  0,  0, -10, -5,  5 },
                {  5, 10, 10, -20, -20, 10, 10,  5 },
                {  0,  0,  0,  0,  0,  0,  0,  0 }
            };
            score = pawn_table[y][x];
        }else{
            int pawn_table[8][8] = {
                {  0,  0,  0,  0,  0,  0,  0,  0 },
                {  5, 10, 10, -20, -20, 10, 10,  5 },
                {  5, -5, -10,  0,  0, -10, -5,  5 },
                {  0,  0,  0, 20, 20,  0,  0,  0 },
                {  5,  5, 10, 25, 25, 10,  5,  5 },
                { 10, 10, 20, 30, 30, 20, 10, 10 },
                { 50, 50, 50, 50, 50, 50, 50, 50 },
                {  0,  5,  5,  0,  0,  5,  5,  0 }
            };
            score = pawn_table[y][x];
        }

    }else if(piece == 'n' || piece =='N'){
        int knight_table[8][8] = {
            { -50, -40, -30, -30, -30, -30, -40, -50 },
            { -40, -20,   0,   5,   5,   0, -20, -40 },
            { -30,   5,  10,  15,  15,  10,   5, -30 },
            { -30,   0,  15,  20,  20,  15,   0, -30 },
            { -30,   5,  15,  20,  20,  15,   5, -30 },
            { -30,   0,  10,  15,  15,  10,   0, -30 },
            { -40, -20,   0,   0,   0,   0, -20, -40 },
            { -50, -40, -30, -30, -30, -30, -40, -50 }
        };
        score = knight_table[y][x];
    }else if(piece == 'q' || piece =='Q'){
        int queen_table[8][8] = {
            { -20, -10, -10,  -5,  -5, -10, -10, -20 },
            { -10,   0,   5,   5,   5,   5,   0, -10 },
            { -10,   5,   5,   5,   5,   5,   5, -10 },
            {  -5,   0,   5,   5,   5,   5,   0,  -5 },
            {   0,   0,   5,   5,   5,   5,   0,  -5 },
            { -10,   5,   5,   5,   5,   5,   5, -10 },
            { -10,   0,   5,   5,   5,   5,   0, -10 },
            { -20, -10, -10,  -5,  -5, -10, -10, -20 }
        };
        if(islower(piece))
            score = -queen_table[y][x];
        else
            score = queen_table[y][x];
    }else if(piece == 'r' || piece =='R'){
        if( piece =='r'){
            int rook_table[8][8] = {
                {  0,   0,   0,   5,   5,   0,   0,   0 },
                {  0,   0,   0,  10,  10,   0,   0,   0 },
                {  0,   0,   0,  10,  10,   0,   0,   0 },
                {  0,   0,   0,  10,  10,   0,   0,   0 },
                {  0,   0,   0,  10,  10,   0,   0,   0 },
                {  0,   0,   0,  10,  10,   0,   0,   0 },
                {  25,  25,  25,  25,  25,  25,  25,  25 },
                {   0,   0,   0,   5,   5,   0,   0,   0 }
            };
            score = rook_table[y][x];
        }else{
            int rook_table[8][8] = {
                {  0,   0,   0,   5,   5,   0,   0,   0 },
                {  25,  25,  25,  25,  25,  25,  25,  25 },
                {  0,   0,   0,  10,  10,   0,   0,   0 },
                {  0,   0,   0,  10,  10,   0,   0,   0 },
                {  0,   0,   0,  10,  10,   0,   0,   0 },
                {  0,   0,   0,  10,  10,   0,   0,   0 },
                {  0,   0,   0,  10,  10,   0,   0,   0 },
                {   0,   0,   0,   5,   5,   0,   0,   0 }
            };
            score = rook_table[y][x];
        }
        
    }else if(piece == 'b' || piece =='B'){
        if( piece =='b'){
            int bishop_table[8][8] = {
                { -20, -10, -10, -10, -10, -10, -10, -20 },
                { -10,   5,   0,   0,   0,   0,   5, -10 },
                { -10,  10,  10,  10,  10,  10,  10, -10 },
                { -10,   0,  10,  10,  10,  10,   0, -10 },
                { -10,   5,  10,  10,  10,  10,   5, -10 },
                { -10,   0,   5,  10,  10,   5,   0, -10 },
                { -10,   0,   0,   0,   0,   0,   0, -10 },
                { -20, -10, -10, -10, -10, -10, -10, -20 }
            };
            score = bishop_table[y][x];
        }else  {
            int bishop_table[8][8] = {
                { -20, -10, -10, -10, -10, -10, -10, -20 },
                { -10,   0,   0,   0,   0,   0,   0, -10 },
                { -10,   0,   5,  10,  10,   5,   0, -10 },
                { -10,   5,  10,  10,  10,  10,   5, -10 },
                { -10,   0,  10,  10,  10,  10,   0, -10 },
                { -10,  10,  10,  10,  10,  10,  10, -10 },
                { -10,   5,   0,   0,   0,   0,   5, -10 },
                { -20, -10, -10, -10, -10, -10, -10, -20 }
            };
            score = bishop_table[y][x];
        }
        
    }else if(piece == 'k' || piece =='K'){
        int king_table[8][8] = {
            { -50, -40, -30, -30, -30, -30, -40, -50 },
            { -40, -20,   0,   5,   5,   0, -20, -40 },
            { -30,   5,  10,  15,  15,  10,   5, -30 },
            { -30,   0,  15,  20,  20,  15,   0, -30 },
            { -30,   5,  15,  20,  20,  15,   5, -30 },
            { -30,   0,  10,  15,  15,  10,   0, -30 },
            { -40, -20,   0,   0,   0,   0, -20, -40 },
            { -50, -40, -30, -30, -30, -30, -40, -50 }
        };
        if(islower(piece))
            score = -king_table[y][x];
        else
            score = king_table[y][x];
        
    }
    return score;
   
}

//
void ChessAI::setAnotherPlayerMove(bit_position from_pos, bit_position to_pos ){
    char tag = _board[from_pos.y_position][from_pos.x_position];
    
   bitMovedFromTo(tag,from_pos, to_pos,_OpponentOpinion, _AIopinion,_board );
   updateScore(&_OpponentOpinion, &_AIopinion,_board);

}
//AI move function
void ChessAI::AI_Move(bit_position& from_pos,bit_position& to_pos){
    
    Board_opinion currentAIOpinion = _AIopinion;
    Board_opinion currentOpponentOpinion = _OpponentOpinion;
    //std::list<bit> bit_own;
    int step = -99999999;

    int currentBestScore = -99999999;//evalevaluateBoard(_opinion._board, _opinion._AINum);// the best score
    bit moveBit;
    //int moveScore = 0;
    char p; // best move bit
    char temp[8][8];
    std::list<Board_opinion>::iterator s;

    for (auto& piece : _AIopinion._own){
        if(piece._is_capture)
            continue;
        if(piece._tag == 'n')
            int i = 0;
        //moveBit = piece;
        std::queue<bit_position> moves = bit_move(piece, piece._pos,_board, _AIopinion,_AIopinion._num);// 得到移动集合
        while (!moves.empty())
        {
            int current = _AIopinion._num;
            resetBoard(_board ,temp);
            bit_position move = moves.front();
            moves.pop();
            // move bit
            
            
            bitMovedFromTo(piece._tag,piece._pos,move, currentAIOpinion,currentOpponentOpinion ,temp );
            //int score = MiniMax();
            updateScore(&currentAIOpinion,&currentOpponentOpinion,temp);
            if(currentAIOpinion._score > step){
                if(current == 1)
                    current = 0;
                else
                    current = 1;
                //currentBestScore = currentAIOpinion._score;
                step = _AIopinion._score;
                miniMax(&currentOpponentOpinion,&currentAIOpinion,temp,3 , current);
                
                if(currentAIOpinion._score > currentBestScore){//如果移动此步比当前分数更大 则保存这一步
                    currentBestScore = currentAIOpinion._score;
                    moveBit = piece;
                    to_pos = move;
                    
                }
            }
            //回退
            currentAIOpinion = _AIopinion;
            currentOpponentOpinion = _OpponentOpinion;
            //temp = _board;
        }

    }
    from_pos = moveBit._pos;
    
    _AIopinion = currentAIOpinion;
    bitMovedFromTo(moveBit._tag, moveBit._pos, to_pos, _AIopinion, _OpponentOpinion ,_board );
    //return aiMove;
    
}

void ChessAI::updateScore(ChessAI::Board_opinion *player_opinion, ChessAI::Board_opinion *opponent_opinion,char board[8][8]){
    int player_score = evaluateBoard(board, player_opinion->_num);
    int opponent_score = evaluateBoard(board, opponent_opinion->_num);
    
    player_opinion->_score = player_score;
    opponent_opinion->_score = opponent_score ;

}
//MiniMax
void ChessAI::miniMax( ChessAI::Board_opinion *player_opinion, ChessAI::Board_opinion *opponent_opinion, char  board[8][8], int depth,  int currentNum){
    if(depth <= 0){
        updateScore(player_opinion,opponent_opinion,board);
        return;
    }
    
    Board_opinion currentOpponentOpinion = *opponent_opinion; //copy opponent opinion
    Board_opinion currentPlayerOpinion = *player_opinion;

    Board_opinion bestPlayerOpinion = *player_opinion;
    Board_opinion finalOpponentOpinion = *player_opinion;
    int nextStep = currentNum;
    if(nextStep == 0)
        nextStep = 1;
    else
        nextStep  = 0;
         

    int currentStepScore = player_opinion->_score;
    //int currentBestScore = currentPlayerOpinion._score;//evaluateBoard(_opinion._board, _opinion._AINum);// the best score    
    char temp[8][8];


    for (auto& piece : player_opinion->_own){
        if(piece._is_capture)
            continue;
        
        std::queue<bit_position> moves = bit_move(piece, piece._pos,board, currentPlayerOpinion, currentNum);// 得到移动集合
        while(!moves.empty()){
            //reset path
            if(piece._tag == 'Q' || piece._tag == 'N' )
                int i = 0;
            resetBoard(board,temp);
            bit_position move = moves.front();
            moves.pop();

            bitMovedFromTo(piece._tag,piece._pos,move , currentPlayerOpinion, currentOpponentOpinion , temp);
            updateScore( &currentPlayerOpinion , &currentOpponentOpinion,temp);

            
            //if move score better
            if(currentPlayerOpinion._score > currentStepScore){
                
                // use minmax to recursion
                miniMax(&currentOpponentOpinion , &currentPlayerOpinion, temp, depth - 1 , nextStep);
                
                if(bestPlayerOpinion._score < currentPlayerOpinion._score){
                    bestPlayerOpinion = currentPlayerOpinion;
                    finalOpponentOpinion = currentOpponentOpinion;
                }

            }

            //回退
            currentPlayerOpinion = *player_opinion;
            currentOpponentOpinion = *opponent_opinion;
            
        }

    }
    
    *player_opinion = bestPlayerOpinion;
    *opponent_opinion = finalOpponentOpinion;
    
}

void ChessAI::resetBoard(char source[8][8], char destination[8][8]) {
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            destination[y][x] = source[y][x];
        }
    }
}


// move bit from to
void ChessAI::bitMovedFromTo(char bit,bit_position from,  bit_position to, ChessAI::Board_opinion& mover_opinion, ChessAI::Board_opinion& opponent_opinion, char  board[8][8] ){

    //char piece = temp[form_pos.y_position][form_pos.x_position];
    if(board[to.y_position][to.x_position] != '0')
    {
        for (auto& piece : opponent_opinion._own){
            if(piece._pos == to){
                piece._is_capture = true;
                break;
            }
        }
    }


    if(bit == 'r' || bit == 'R'){
        if(from.x_position == 0)
            mover_opinion._Q_Castling = false;
        else if(from.x_position == 7)
            mover_opinion._K_Castling = false;
    }


    board[to.y_position][to.x_position] = bit;
    board[from.y_position][from.x_position] = '0';
    //piece._pos = to_pos;
    for (auto& piece : mover_opinion._own){
        if(bit == piece._tag && from == piece._pos){
            piece._pos = to;
            break;
        }
    }
    
    
    if(!isupper(bit)){
        for(int i = 0; i < 8; i ++){
            mover_opinion.EnPassant[i] = false;
        }
        if(bit == 'p' && to.y_position == from.y_position - 2)
            mover_opinion.EnPassant[to.x_position] = true;
    }

    if(islower(bit)){
        if(bit =='k'){
            if(mover_opinion._K_Castling && from.x_position + 2 == to.x_position){
                
                board[7][7] = '0';
                board[7][5] = 'r';
            }else if(mover_opinion._Q_Castling && from.x_position - 2 == to.x_position){
                board[7][0] = '0';
                board[7][3] = 'r';
            }
            mover_opinion._Q_Castling = false;
            mover_opinion._K_Castling = false;
        }
    }else{
        if(bit =='K'){
            if(mover_opinion._K_Castling && from.x_position + 2 == to.x_position){
                
                board[0][7] = '0';
                board[0][5] = 'r';
            }else if(mover_opinion._Q_Castling && from.x_position - 2 == to.x_position){
                board[0][0] = '0';
                board[0][3] = 'r';
            }
            mover_opinion._Q_Castling = false;
            mover_opinion._K_Castling = false;
        }
    }
    

}
int ChessAI::checkWinner(){
    return 0;
}
//return the movement queue
std::queue<bit_position> ChessAI::bit_move(bit piece, bit_position pos, char  board[8][8],Board_opinion _opinion,int playerNum){
    std::queue<bit_position> moves;

    int y = pos.y_position;
    int x = pos.x_position;
    char tag = piece._tag;
    if(tag == 'p' || tag == 'P'){
        if(piece._playerNum == 0 ){
            if(y + 1 < 8){
                if(board[y + 1][x] == '0'){
                    moves.push(bit_position(y + 1,x));
                        //_grid[0][0].destroyBit();
                }
                if(x + 1 < 8 && board[y + 1][x + 1]  != '0' && islower(board[y + 1][x + 1])  )
                    moves.push(bit_position(y + 1,x + 1));
                if(x - 1 >= 0 &&board[y + 1][x - 1]  != '0'&& islower(board[y + 1][x - 1]))
                    moves.push(bit_position(y + 1,x + 1));

                if(y == 4){
                    if(x < 8 - 1 && _opinion.EnPassant[x + 1])
                        moves.push(bit_position(y,x + 1));
                    if(x >= 0 && _opinion.EnPassant[x - 1])
                        moves.push(bit_position(y,x - 1));
                }
            }
            if(y == 1){
                if(board[y + 1][x] == '0' && board[y + 2][x] == '0' ){
                        //bit._init_special = true;
                    moves.push(bit_position(y + 2,x ));
                }
            }    
        }else{
            if(y - 1 >= 0){
                if(board[y - 1][x] == '0'){
                    moves.push(bit_position(y - 1,x));
                        //_grid[0][0].destroyBit();
                }
                if(x + 1 < 8 && board[y - 1][x + 1]  != '0' && isupper(board[y - 1][x + 1]))
                    moves.push(bit_position(y - 1,x + 1));
                if(x - 1 >= 0 && board[y - 1][x - 1]  != '0' && isupper(board[y - 1][x - 1]))
                    moves.push(bit_position(y - 1,x + 1));

                if(y == 3){
                    if(x < 8 -1 && _opinion.EnPassant[x + 1])
                        moves.push(bit_position(y,x + 1));
                    if(x >= 0 && _opinion.EnPassant[x - 1])
                        moves.push(bit_position(y,x - 1));
                }
            }
            if(y == 6){
                if(board[y - 1][x] == '0' && board[y - 2][x] == '0' ){
                        //bit._init_special = true;
                    moves.push(bit_position(y - 2,x ));
                }
            }
        }
        
        
    }else if(tag == 'r' || tag == 'R'){
        for(int i = x + 1; i <= 7; i ++){
            if( board[y][i] != '0'){
                if((piece._playerNum == 1 &&  isupper( board[y][i] )) || (piece._playerNum == 0 &&  islower( board[y][i] )))
                   moves.push(bit_position(y,i));
                break;
            }
            moves.push(bit_position(y,i));    
        }
        for(int i = x - 1; i >= 0; i --){
            if( board[y][i] != '0'){
                if((piece._playerNum == 1 &&  isupper( board[y][i] ) ) || (piece._playerNum == 0 &&  islower( board[y][i] )))
                   moves.push(bit_position(y,i));
                break;
            }
            moves.push(bit_position(y,i));    
        }
        for(int i = y + 1; i <= 7; i ++){
            if( board[i][x] != '0'){
                if((piece._playerNum == 1 &&  isupper( board[i][x] )) || ( piece._playerNum == 0 &&  islower( board[i][x] )))
                   moves.push(bit_position(i,x));
                break;
            }
            moves.push(bit_position(i,x));    
        }
        for(int i = y - 1; i >= 0; i --){
            if( board[i][x] != '0'){
                if((piece._playerNum == 1 &&  isupper( board[i][x] ) ) || (piece._playerNum == 0 &&  islower( board[i][x] )))
                   moves.push(bit_position(i,x));
                break;
            }
            moves.push(bit_position(i,x));  
        }
    }else if(tag == 'n' || tag == 'N'){
        int i = x + 1;
        int j = y + 2;
        if(i  < 8){
            if(j< 8)
                if( board[j][i] == '0' || (piece._playerNum == 1 &&  isupper( board[j][i] ) ) || ( piece._playerNum == 0 &&  islower(board[j][i] )))
                    moves.push(bit_position(j,i));  
            j = y -2;
            if(j>= 0)
                if( board[j][i] == '0' || (piece._playerNum == 1 &&  isupper( board[j][i] ) ) || ( piece._playerNum == 0 &&  islower(board[j][i] )))
                    moves.push(bit_position(j,i));   
        }
        i = x + 2;
        j = y + 1;
        if(i< 8)
        {
            if(j < 8)
                if( board[j][i] == '0' || (piece._playerNum == 1 &&  isupper( board[j][i] ) ) || ( piece._playerNum == 0 &&  islower(board[j][i] )))
                    moves.push(bit_position(j,i));   
            j = y - 1;
            if(j >= 0)
                if( board[j][i] == '0' || (piece._playerNum == 1 &&  isupper( board[j][i] ) ) || ( piece._playerNum == 0 &&  islower(board[j][i] )))
                    moves.push(bit_position(j,i));  
        }
        i = x - 1;
        j = y + 2;
        if(i > 0){
            if(j< 8)
                if( board[j][i] == '0' || (piece._playerNum == 1 &&  isupper( board[j][i] ) ) || ( piece._playerNum == 0 &&  islower(board[j][i] )))
                    moves.push(bit_position(j,i));
                
            j = y -2;
            if(j>= 0)
                if( board[j][i] == '0' || (piece._playerNum == 1 &&  isupper( board[j][i] ) ) || ( piece._playerNum == 0 &&  islower(board[j][i] )))
                    moves.push(bit_position(j,i));
        }
            //check_square(x - 1,y ,bit.getOwner()->playerNumber());
        i = x - 2;
        j = y + 1;
        if(i > 0){
            if(j < 8)
                if( board[j][i] == '0' || (piece._playerNum == 1 &&  isupper( board[j][i] ) ) || ( piece._playerNum == 0 &&  islower(board[j][i] )))
                    moves.push(bit_position(j,i));
            j = y - 1;
            if(y - 1 >= 0)
                if( board[j][i] == '0' || (piece._playerNum == 1 &&  isupper( board[j][i] ) ) || ( piece._playerNum == 0 &&  islower(board[j][i] )))
                    moves.push(bit_position(j,i));
        }
            
    }else if(tag == 'q' || tag == 'Q'){
        for(int i = x + 1; i <= 7; i ++){
            if(board[y][i] != '0'){
                if((piece._playerNum == 1 &&  isupper( board[y][i] )) || ( piece._playerNum == 0 &&  islower(board[y][i] )))
                   moves.push(bit_position(y,i));
                break;
            }
            moves.push(bit_position(y,i));    
        }
        for(int i = x - 1; i >= 0; i --){
            if(board[y][i] != '0'){
                if((piece._playerNum == 1 &&  isupper( board[y][i] ) ) || (piece._playerNum == 0 &&  islower(board[y][i] )))
                   moves.push(bit_position(y,i));
                break;
            }
            moves.push(bit_position(y,i));    
        }
        for(int i = y + 1; i <= 7; i ++){
            if(board[i][x] != '0'){
                if((piece._playerNum == 1 &&  isupper( board[i][x] ) ) || (piece._playerNum == 0 &&  islower(board[i][x] )))
                   moves.push(bit_position(i,x));
                break;
            }
            moves.push(bit_position(i,x));    
        }
        for(int i = y - 1; i >= 0; i --){
            if(board[i][x] != '0'){
                if((piece._playerNum == 1 &&  isupper( board[i][x] ) ) || ( piece._playerNum == 0 &&  islower(board[i][x] )))
                   moves.push(bit_position(i,x));
                break;
            }
            moves.push(bit_position(i,x));  
        }

        int j = y + 1;
        for(int i = x + 1; i < 8 ;i ++){
            if(j < 8){
                if(board[j][i] != '0'){
                    if((piece._playerNum == 1 &&  isupper( board[j][i] ) ) || ( piece._playerNum == 0 &&  islower(board[j][i] )))
                        moves.push(bit_position(j,i));
                    break;
                }
                moves.push(bit_position(j,i));
            }else
                break;
            j = j + 1;
        }
        j = y - 1;
        for(int i = x + 1; i < 8 ;i ++){
            if(j >= 0){
                if(board[j][i] != '0'){
                    if((piece._playerNum == 1 &&  isupper( board[j][i] ) ) || (piece._playerNum == 0 &&  islower(board[j][i] )))
                        moves.push(bit_position(j,i));
                    break;
                }
                moves.push(bit_position(j,i));
            }else
                break;
            j = j - 1;
        }
        j = y + 1;
        for(int i = x - 1; i >= 0 ;i --){
            if(j <= 7){
                if(board[j][i] != '0'){
                    if((piece._playerNum == 1 &&  isupper( board[j][i] ) ) || (piece._playerNum == 0 &&  islower(board[j][i] )))
                        moves.push(bit_position(j,i));
                    break;
                }
                moves.push(bit_position(j,i));
            }else
                break;
            j = j + 1;
        }
        j = y - 1;
        for(int i = x - 1; i >= 0 ;i --){
            if(j >= 0){
                if(board[j][i] != '0'){
                    if((piece._playerNum == 1 &&  isupper( board[j][i] )) || ( piece._playerNum == 0 &&  islower(board[j][i] )))
                        moves.push(bit_position(j,i));
                    break;
                }
                moves.push(bit_position(j,i));
            }else
                break;
            j = j - 1;
        }
    }
    else if(tag == 'k' || tag == 'K'){
        if(x > 0 )
            if(board[y][x - 1] == '0' || (piece._playerNum == 1 &&  isupper( board[y][x -1] )) || (piece._playerNum == 0 &&  islower( board[y][x -1] )))
                moves.push(bit_position(y,x -1));
        if(x < 7)
            if(board[y][x + 1] == '0' || (piece._playerNum == 1 &&  isupper( board[y][x +1] )) || (piece._playerNum == 0 &&  islower( board[y][x + 1] )))
                moves.push(bit_position(y,x +1));
        if(y > 0)
            if(board[y - 1][x ] == '0' || (piece._playerNum == 1 &&  isupper( board[y - 1][x  ] )) || (piece._playerNum == 0 &&  islower( board[y - 1][x ] )))
                moves.push(bit_position(y - 1,x));
        if(y < 7)
            if(board[y + 1][x ] == '0' || (piece._playerNum == 1 &&  isupper( board[y + 1][x  ] )) || (piece._playerNum == 0 &&  islower( board[y + 1][x ] )))
                moves.push(bit_position(y + 1,x));
            
        if(x > 0 && y > 0)
            if(board[y - 1 ][x - 1] == '0' || (piece._playerNum == 1 &&  isupper( board[y - 1 ][x - 1] )) || (piece._playerNum == 0 &&  islower( board[y - 1 ][x - 1] )))
             moves.push(bit_position(y - 1,x - 1));
            
        if(x < 7 && y < 7)
            if(board[y + 1][x + 1] == '0' || (piece._playerNum == 1 &&  isupper( board[y + 1][x + 1])) || (piece._playerNum == 0 &&  islower(board[y + 1][x + 1] )))
                moves.push(bit_position(y + 1,x + 1));
        if(x > 0 && y < 7)    
            if(board[y + 1][x - 1] == '0' || (piece._playerNum == 1 &&  isupper( board[y + 1][x - 1] )) || (piece._playerNum == 0 &&  islower( board[y + 1][x - 1])))
                moves.push(bit_position(y + 1,x - 1));
        if(x < 7 && y > 0) 
            if(board[y - 1][x + 1] == '0' || (piece._playerNum == 1 &&  isupper( board[y - 1][x + 1] )) || (piece._playerNum == 0 &&  islower( board[y - 1][x + 1] )))
                moves.push(bit_position(y - 1,x + 1));


        if(piece._playerNum == 1){
            if(_opinion._K_Castling && board[7][5] == '0' && board[7][6] == '0'){
                moves.push(bit_position(y,x + 2));
            }
            if(_opinion._Q_Castling && board[7][1]== '0'  && board[7][2]== '0' && board[7][3]== '0' ){
                moves.push(bit_position(y,x - 2)); 
            }
        }else{
            if(_opinion._K_Castling &&  board[0][5] == '0' &&  board[0][6] == '0'){
                moves.push(bit_position(y,x + 2));
            }
            if(_opinion._Q_Castling &&  board[0][1]== '0'  &&  board[0][2]== '0' && board[0][3]== '0' ){
                moves.push(bit_position(y,x - 2)); 
            }
        }

        
        

    }else if(tag  == 'b' || tag  == 'B'){
        int j = y + 1;
        for(int i = x + 1; i < 8 ;i ++){
            if(j < 8){
                if(board[j][i] != '0'){
                    if((piece._playerNum == 1 &&  isupper( board[j][i] )) || ( piece._playerNum == 0 &&  islower(board[j][i] )))
                        moves.push(bit_position(j,i));
                    break;
                }
                moves.push(bit_position(j,i));
            }else
                break;
            j = j + 1;
        }
        j = y - 1;
        for(int i = x + 1; i < 8 ;i ++){
            if(j >= 0){
                if(board[j][i] != '0'){
                    if((piece._playerNum == 1 &&  isupper( board[j][i] ) ) || ( piece._playerNum == 0 &&  islower(board[j][i] )))
                        moves.push(bit_position(j,i));
                    break;
                }
                moves.push(bit_position(j,i));
            }else
                break;
            j = j - 1;
        }
        j = y + 1;
        for(int i = x - 1; i >= 0 ;i --){
            if(j <= 7){
                if(board[j][i] != '0'){
                    if((piece._playerNum == 1 &&  isupper( board[j][i] ) ) || ( piece._playerNum == 0 &&  islower(board[j][i] )))
                        moves.push(bit_position(j,i));
                    break;
                }
                moves.push(bit_position(j,i));
            }else
                break;
            j = j + 1;
        }
        j = y - 1;
        for(int i = x - 1; i >= 0 ;i --){
            if(j >= 0){
                if(board[j][i] != '0'){
                    if((piece._playerNum == 1 &&  isupper( board[j][i] ) ) || ( piece._playerNum == 0 &&  islower(board[j][i] )))
                        moves.push(bit_position(j,i));
                    break;
                }
                moves.push(bit_position(j,i));
            }else
                break;
            j = j - 1;
        }
    } 
    return moves;
}