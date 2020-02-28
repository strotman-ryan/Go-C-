/* File name: go.cpp 

* created by: Ryan Strotman 

* created on: 4/11/17  

* synopsis: the game go 

*/ 

  

  

#include<iostream> 

#include<cmath> 

#include<iomanip> 

#include<string> 

#include<vector> 

  

using namespace std; 

  

  

//class Position 

class Position{ 

  private: 

  int value;     //the value of the point 

  int valueUp, valueDown, valueRight, valueLeft;      //these are the values of the pieces next to tis position 

  int valueTemp;                                      //this detemines if the piece is free or not; free=1; notFree=0 

  //for values 0 = blankspace; 1 = player 1 piece; 2 = player 2 piece; 3 = edge of board; 

  public: 

  void set_value(const int uValue); 

  void set_valueUp(const int uValue); 

  void set_valueDown(const int uValue); 

  void set_valueRight(const int uValue); 

  void set_valueLeft(const int uValue); 

  void set_valueTemp(const int uValue); 

  

  int get_value(); 

  int get_valueUp(); 

  int get_valueDown(); 

  int get_valueRight(); 

  int get_valueLeft(); 

  int get_valueTemp(); 

}; 

  

  

  

  

//Functions 

void size_dimension(int & dimension); 

void initilize_board(Position* board[], const int dimension); 

void display_board(Position* board[], const int dimension); 

void get_position(Position* board[], const int numPlayer, const int dimension, int & endGame); 

void ask_player(const int numPlayer, const int dimension, int &  row, int & column); 

bool check_validity(Position* board[], const int numPlayer, const int dimension, const int row, const int column); 

void change_value(Position* board[], const int row, const int column, const int numPlayer); 

bool end_game(const int endGame); 

void change_player(int & numPlayer); 

void analyse_board(Position* board[], const int dimension, const int numPlayer, int piecesCaptured[]); 

void change_adjacent(Position* board[],const int dimension); 

void determine_freedom(Position* board[],const int dimension, const int numPlayer); 

void remove_pieces(Position* board[],const int dimension, const int numPlater, int piecesCaptured[]); 

void display_end(int piecesCaptured[]); 

  

int main() 

{ 

  //initialize variables 

  int dimension;                  //demension of the baord 

  int numPlayer(1);               //player who's turn it is 

  int endGame(0);                 //if this reaches 2 then the game is over (two passes in a row ends the game) 

  

  //these numbers will be subtracted from ecah players total score 

  int piecesCaptured[] = {0,0};   //first element if the number of pl1 pieces captured; the second element is the same for pl2 

                    

  

  //determine size of board 

  size_dimension(dimension);  

  //initilize the board 

  Position **board = new Position *[dimension]; 

  initilize_board(board,dimension); 

  //display_board 

  display_board(board, dimension); 

  //Enter main game loop 

  while(!end_game(endGame)){ 

    //get the position that wants to be played at 

    get_position(board, numPlayer, dimension, endGame); 

      if(endGame==0){                            //check to see if player passed 

    //remove piececs 

        analyse_board(board,dimension,numPlayer,piecesCaptured); 

    //display board 

        display_board(board,dimension); 

      } 

    change_player(numPlayer); 

  } 

  display_end(piecesCaptured); 

  return (0); 

} 

  

  

  

//class Position functions 

  

//set_value sets just the value at that postion 

void Position::set_value(const int uValue){ 

  value = uValue; 

} 

//the following 4 functions set the values for the pieces around this position 

void Position::set_valueUp(const int uValue){ 

  valueUp = uValue; 

} 

void Position::set_valueDown(const int uValue){ 

  valueDown = uValue; 

} 

void Position::set_valueRight(const int uValue){ 

  valueRight = uValue; 

} 

void Position::set_valueLeft(const int uValue){ 

  valueLeft = uValue; 

} 

//set valueTemp 

void Position::set_valueTemp(const int uValue){ 

  valueTemp = uValue; 

} 

  

  

  

//get_value gets the value of a position 

int Position::get_value(){ 

  return value; 

} 

  

//the following 4 functions get the value of the positions next to them 

int Position::get_valueUp(){ 

  return valueUp; 

} 

int Position::get_valueDown(){ 

  return valueDown; 

} 

int Position::get_valueRight(){ 

  return valueRight; 

} 

int Position::get_valueLeft(){ 

  return valueLeft; 

} 

//returns valueTemp 

int Position::get_valueTemp(){ 

  return valueTemp; 

} 

  

  

  

//Other functions 

  

//size_demension: allows the player to determine what size of board he wants 

void size_dimension(int & dimension){ 

  const string prompt = "Input the size of baord you want to play on (9-19): "; 

  const string error = "Error Out of bounds"; 

  do 

    { 

    cout << prompt; 

    cin >> dimension; 

       if(!(dimension >=9 && dimension <=19)){ 

       cout << error << endl; 

    } 

    } 

  while(dimension <9 || dimension >19); 

} 

  

  

//initilize_board: sets all values to 0 and makes vector 

void initilize_board(Position* board[], const int dimension){ 

  for(int i=0; i<dimension; i++){ 

    board[i] = new Position[dimension]; 

  } 

  for(int row=0; row<dimension; row++){ 

    for(int column=0; column<dimension; column++){ 

      board[row][column].set_value(0); 

      board[row][column].set_valueTemp(0);         //not free 

      //set the values of the positions around each position 

      if(row == 0){board[row][column].set_valueUp(3);} 

      else{board[row][column].set_valueUp(0);} 

      if(row== dimension-1){board[row][column].set_valueDown(3);} 

      else{board[row][column].set_valueDown(0);} 

      if(column == 0){board[row][column].set_valueLeft(3);} 

      else{board[row][column].set_valueLeft(0);} 

      if(column == dimension-1){board[row][column].set_valueRight(3);} 

      else{board[row][column].set_valueRight(0);} 

    } 

  } 

} 

  

//display_board: displays the board using characters 

void display_board(Position* board[],const int dimension){ 

  const char blank = '+';                 //the character for a blank space 

  const char pl1Piece = 'o';              //the character for player one's piece 

  const char pl2Piece= 'x';               //the character for player two's piece 

  char columnHeader = 65;                    //the letters at the top of the column, set to 'A'-1 

  

  for(int i=0; i<dimension+1; i++){ 

    if(i==0){ 

      if(dimension==9){ 

      cout << "  "; 

      } 

      else{                              //add extra space because double digits 

    cout << "   "; 

      }  

    } 

    else{cout << columnHeader << " "; 

    columnHeader++; 

    } 

  } 

  cout << endl; 

  for(int row=0; row<dimension; row++){ 

    if(dimension == 9){ 

    cout << row +1 << " "; 

    } 

    else{                                 //aligns the column numbers appropriately 

      cout << setw(3) << right << row +1 << " "; 

    } 

    for(int column=0; column<dimension; column++){ 

      if(board[row][column].get_value() == 0){ 

        cout << blank << " "; 

      } 

      else if(board[row][column].get_value() == 1){ 

        cout << pl1Piece << " "; 

      } 

      else{ 

        cout << pl2Piece << " "; 

      } 

    } 

    cout <<endl; 

  } 

} 

  

//get_position gets the postion that the player wants to play at 

void get_position(Position* board[], const int numPlayer, const int dimension, int & endGame ){ 

  int row;          //the row number chosen 

  int column;      //the column letter chosen 

  ask_player(numPlayer, dimension, row, column); 

  while(!check_validity(board, numPlayer, dimension, row, column)){ 

    cout << "Error. The inputs must be between 1 and " << dimension << endl; 

    ask_player(numPlayer, dimension, row, column); 

  } 

  if(column != -63){ 

    change_value(board,row, column, numPlayer); 

    change_adjacent(board, dimension); 

    if(endGame ==1){endGame--;}               //resets the counter for the end of the game if someone doesnt pass 

  } 

  else{endGame++;}                //adds a number to counter when someone passes                     

} 

  

  

  

  

//ask_player gets the players input 

void ask_player(const int numPlayer, const int dimension, int &  row, int & column){ 

  char max = 'A' + dimension - 1;                           // the max character ale to be inputed 

  char charColumn;                                          //the cahracter inputted for the column 

  cout << "Player " << numPlayer << " choose a column(A-" << max << ") then a row(1-" << dimension << "). To pass type '!1': "; 

  cin >> charColumn >> row; 

  column = tolower(charColumn) - 96; 

} 

  

//chech_validity checks if the placement of the piece is able to be done 

//returns flase if bad placement 

bool check_validity(Position* board[], const int numPlayer, const int dimension, const int row, const int column){ 

  if(column == -63){ 

    return true; 

  } 

  if(row>dimension || column>dimension || row<1 || column<1 ){ 

    return false; 

  } 

  if(board[row-1][column-1].get_value()!=0){ 

    return false; 

  } 

  return true; 

} 

  

//change_value places a peice on the board 

void change_value(Position* board[], const int row, const int column, const int numPlayer){ 

  if(numPlayer == 1){ board[row-1][column-1].set_value(1);} 

  else{board[row-1][column-1].set_value(2);} 

} 

  

//end_game checks to see if the game should be over or not 

//returns true when the game should be over 

bool end_game(const int endGame){ 

   return (endGame == 2); 

} 

  

//change_player changes who's turn it is 

void change_player(int & numPlayer){ 

  if(numPlayer == 1){numPlayer++;} 

  else{numPlayer--;} 

} 

  

//remove_pieces analyses the board and determines if a piece should be removed or not 

//this is the main logic of the game 

void analyse_board(Position* board[], const int dimension, int numPlayer, int piecesCaptured[]){ 

  change_player(numPlayer);                       //do analysis on player who didnt make a move first 

  determine_freedom(board,dimension,numPlayer); 

  remove_pieces(board,dimension,numPlayer, piecesCaptured); 

  change_adjacent(board, dimension); 

  //repeat above but with different player 

  change_player(numPlayer); 

  determine_freedom(board,dimension,numPlayer); 

  remove_pieces(board,dimension,numPlayer, piecesCaptured); 

  change_adjacent(board, dimension); 

  //set all valueTemp back to 0 

  for(int row=0; row<dimension; row++){ 

    for(int column=0; column<dimension; column++){ 

      board[row][column].set_valueTemp(0); 

    } 

  } 

} 

  

//determine_freedom changes the value of valueTemp of an object of type position if it is next to another free space or another piece that is already free 

void determine_freedom(Position* board[],const int dimension, const int numPlayer){ 

  for(int i=0; i<20; i++){ 

    for(int row=0; row<dimension; row++){ 

      for(int column=0; column<dimension; column++){ 

        //skip if not proper players turn 

        if(board[row][column].get_value()==numPlayer){ 

        //check if there is a free space next to space 

          if((board[row][column].get_valueUp()==0 || board[row][column].get_valueDown()==0  

            || board[row][column].get_valueLeft()==0 || board[row][column].get_valueRight()==0) &&  

            ( board[row][column].get_valueTemp()!=1)){ 

        board[row][column].set_valueTemp(1); 

        } 

        //check if position next to it are same pieces that are free 

        if(board[row][column].get_valueUp()==numPlayer && board[row][column].get_valueTemp()!=1){ 

          if(board[row-1][column].get_valueTemp()==1){ 

          board[row][column].set_valueTemp(1); 

          } 

        } 

        if(board[row][column].get_valueDown()==numPlayer && board[row][column].get_valueTemp()!=1){ 

          if(board[row+1][column].get_valueTemp()==1){ 

          board[row][column].set_valueTemp(1); 

          } 

        } 

        if(board[row][column].get_valueLeft()==numPlayer && board[row][column].get_valueTemp()!=1){ 

          if(board[row][column-1].get_valueTemp()==1){ 

          board[row][column].set_valueTemp(1); 

          } 

        } 

        if(board[row][column].get_valueRight()==numPlayer && board[row][column].get_valueTemp()!=1){ 

          if(board[row][column+1].get_valueTemp()==1){ 

          board[row][column].set_valueTemp(1); 

          } 

        } 

    } 

      } 

    } 

  } 

} 

  

//remove_pieces removes pieces from the board if they are not free 

void remove_pieces(Position* board[],const int dimension, const int numPlayer, int piecesCaptured[]){ 

  for(int row=0; row<dimension; row++){ 

    for(int column=0; column<dimension; column++){ 

      if(board[row][column].get_valueTemp()==0 && board[row][column].get_value()==numPlayer){ 

    board[row][column].set_value(0); 

        if(numPlayer == 1){piecesCaptured[0]++;}                   //add count to pieces that have been removed 

    else{piecesCaptured[1]++;}; 

      } 

    } 

  } 

} 

  

  

//change_adjacent changes the values of the pieces next to a position 

void change_adjacent(Position* board[],const int dimension){ 

  for(int row=0; row < dimension; row++){ 

    for(int column=0; column<dimension; column++){ 

      if(row == 0){board[row][column].set_valueUp(3);} 

      else{board[row][column].set_valueUp(board[row-1][column].get_value());} 

      if(row== dimension-1){board[row][column].set_valueDown(3);} 

      else{board[row][column].set_valueDown(board[row+1][column].get_value());} 

      if(column == 0){board[row][column].set_valueLeft(3);} 

      else{board[row][column].set_valueLeft(board[row][column-1].get_value());} 

      if(column == dimension-1){board[row][column].set_valueRight(3);} 

      else{board[row][column].set_valueRight(board[row][column+1].get_value());} 

    } 

  } 

} 

  

//ending statement at the end of the game 

void display_end(int piecesCaptured[]){ 

  cout << "End of game." << endl; 

  cout << "Subtract " << piecesCaptured[0] << " from the number of spaces Player one has." << endl; 

  cout << "Subtract " << piecesCaptured[1] << " from the number of spaces Player two has." << endl; 

} 

 
