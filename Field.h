#include <stdlib.h>
#include <iostream>
#include <fstream>

using namespace std;

struct Cell{
	char type = 'e';// E = Empty, F = Flag, N = Number, M = Mine
	char value = '0';
	bool checked = false;
	char prevtype = 'e';
};


// Class used to handle the game field
class Field{
	protected:
		//Attributes
		int rows;
		int cols;
		int mines;
		Cell **mat;						// Declare new pointer to a matrix object called mat
		Cell **usr;						// Declare new pointer to a matrix object called usr
		//Private Methods
		void init();
		char nearMines(int row,int col);
		int isAMine(int row,int col);
		void checkEmptyCell(int row,int col);
		void printField();
	public:
		//Default constructor
		Field(){
			this->rows = 10;
			this->cols = 10;
			this->mines = 10;
		}
		//Constructor with args
		Field(int rows,int cols,int mines){
			this->rows = rows;
			this->cols = cols;
			this->mines = mines;
		}
		// Public methods
		int initField();
		void printUserField();
		int userTurn(int row,int col);
		void placeFlag(int row,int col);
		bool userHasWon();
		void saveToFile();
};

// This method check every cell near the given coords in the matrix and recursively calls itself until it finds a number near every cell
void Field::checkEmptyCell(int row,int col){
	if(row<rows && col<cols && col>= 0 && row>=0){
		if(!mat[row][col].checked && !usr[row][col].checked && mat[row][col].type == 'e'){		// If the cell in that position has not already been checked, and it's empty ( No numbers,mines,or flags)
			usr[row][col].checked = true;														// Set cell on checked (both for user's field and bg field) and set user's cell type to empty
			usr[row][col].type = 'e';
			usr[row][col].value = 219;
			mat[row][col].checked = true;
			checkEmptyCell(row - 1, col - 1);													// Check all the cells around
			checkEmptyCell(row, col - 1);
			checkEmptyCell(row + 1, col - 1);
			checkEmptyCell(row - 1, col);
			checkEmptyCell(row + 1, col);
			checkEmptyCell(row -1, col + 1);
			checkEmptyCell(row, col + 1);
			checkEmptyCell(row + 1, col + 1);
		}else if(mat[row][col].type == 'n' && mat[row][col].value != '0' && !mat[row][col].checked && !usr[row][col].checked){	// Else if the cell is s number and hasn't been checked yet set the user field value to the background value(The number indicating the number of near mines)
			usr[row][col] = mat[row][col];
			usr[row][col].checked = true;
			mat[row][col].checked = true;
		}
	}
}

void Field::saveToFile(){
	
}

// Process the user turn
int Field::userTurn(int row,int col){
	Cell val;
	// Validate arguments
	if(row<rows && col<cols && col>= 0 && row>=0){
		val = mat[row][col];
	}else{
		cout<<"Coordinata non valida, Massimo righe: "<<rows<<", Massimo colonne: "<<cols<<endl;
		return 1;
	}
	
	

	if(val.checked){
		cout<<"Cella già controllata!"<<endl;
		return 1;
	}
	
	// Check if it's a mine
	if(val.type == 'm'){
		cout<<"Ouch! Hai colpito una mina :("<<endl;
		return 0;
	}

	// Check if it's not a void number
	if(val.type == 'n'){
		cout<<"Hai preso un numero!"<<endl;
		mat[row][col].checked = true;
		usr[row][col].value =val.value;
		usr[row][col].type = 'n';
		usr[row][col].checked = true;
		return 1;
	}
	if(val.type == 'e'){
		checkEmptyCell(row,col);
		cout<<"Non trovo niente qui...Ho cancellato tutto il possibile"<<endl;
		return 1;
	}
	return 1;
	
}

// Prind complete field - hidden to user(Debug)
void Field::printField(){
	for(int i=0;i<rows;i++){
		for(int j=0;j<cols;j++){
				cout<<"|"<<mat[i][j].type<<"|";
		}
		cout<<"\n";
	}
}


// Print user's field
void Field::printUserField(){
	Cell el;
	for(int i=0;i<rows;i++){
		for(int j=0;j<cols;j++){
			el = usr[i][j];														// Saving element in position [i][x] into el
			if(el.type =='e' && el.checked){
				cout<<"|"<<"█"<<"|";
			}else if(el.type != 'e' && el.type !='m'){					// If not 0 and not a Mine
				cout<<"|"<<el.value<<"|";
			}else{
				cout<<"|_|";
			}
		}
		cout<<endl;										// New line every new row
	}

}

// Method to check if given cell is a mine
int Field::isAMine(int row,int col){
	if(row<rows && col<cols && col>= 0 && row>=0){
		Cell x = mat[row][col];
		if(x.type=='m'){
			return 1;
		}
		return 0;
	}
	return 0;
}

// Method to count how many mines a given cell has near itself
char Field::nearMines(int row, int col){
	char tot = 48;
	if(row<rows && col<cols && col>= 0 && row>=0){
		tot += isAMine(row-1,col);	// Check the left cell from the required one
		tot += isAMine(row-1,col-1);	// Check the top left cell from the required one
		tot += isAMine(row+1,col);	// Check the right cell from the required one
		tot += isAMine(row+1,col-1);
		tot += isAMine(row,col-1);	// Check the cell above the required one
		tot += isAMine(row,col+1);	// Check the cell below the required one
		tot += isAMine(row-1,col+1);	// Check the top right cell from the required one
		tot += isAMine(row+1,col+1);	// Check the bottom right cell from the required one
	}
	return tot;
}

// Method used to em
void Field::init(){
	// Init hided and user matrix
	mat = new Cell*[rows];
	usr = new Cell*[rows];
	for(int i = 0;i<rows;i++){
		mat[i] = new Cell[this->cols];
		usr[i] = new Cell[this->cols];
	}

	for(int i=0;i<rows;i++){
		for(int j=0;j<cols;j++){
			mat[i][j].type = 'e';
			mat[i][j].checked = false;
			usr[i][j].type = 'e';
			usr[i][j].checked = false;
		}
	}

}

// Method to init the field of game (doesn't init user's visible one)
int Field::initField(){
	init();
	int row,col,maxmines = rows * cols;
	if(mines > maxmines){
		cout<<"Errore durante la creazione del campo: Troppe mine("<<mines<<") per un massimo di: "<<maxmines<<endl;
		return 0;
	}

	for(int i=0;i<mines;i++){							// Places mine randomly until they're all been placed
		do{
			row = rand()%rows;
			col = rand()%cols;
			mat[row][col].type = 'm';					// Generate new coords and retry until the element has been successfully changed
		}while(mat[row][col].type != 'm');
	}


	for(int i=0;i<rows;i++){							// Calculate the number of mines near every cell
		for(int j=0;j<cols;j++){
			if(mat[i][j].type != 'm'){
				if(nearMines(i,j) != 48){				// If there are some mines nearby the cell
					mat[i][j].type = 'n';
					mat[i][j].value = nearMines(i,j);		// Recalling nearMines method and setting value for the current cell
				}
			}
		}
	}
	return 1;
}

// Method used to place/remove a flag into given coords
void Field::placeFlag(int row,int col){
	if(row<rows && col<cols && col>= 0 && row>=0 ){
		if(usr[row][col].type != 'f' && !usr[row][col].checked){
			usr[row][col].prevtype = usr[row][col].type ;
			usr[row][col].type = 'f';
			usr[row][col].value = 'F';
		}else if(usr[row][col].type == 'f'){
			usr[row][col].type = usr[row][col].prevtype;
		}
	}
}

// Method used to determine if the user has or hasn't won the match yet
bool Field::userHasWon(){
	int checked = 0;
	for(int i=0;i<this->rows;i++){
		for(int j=0;j<this->cols;j++){
			if(mat[i][j].checked){
				checked++;
			}
		}
	}
	if(checked+mines==rows*cols){
		return true;
	}
	return false;
}
