/*------------------------------------------------------- 
Author: Zining Wen
Date: 02/12/2018
Description:
Implement a simple terminal verison of minesweeper
---------------------------------------------------------*/

#include <vector>
#include <iostream>
#include <string>
#include <locale>
#include <sstream>
#include <ctime>
#include <cstdlib>

using namespace std;

class gameBoard{

private:
	vector< vector<char> > displayBoard;
	vector< vector<int> > mineBoard;

	bool hitbomb = false;
	int mineNumber;

	vector<vector<int>> dir = {{1,1},{1,0},{1,-1},{0,1},{0,-1},{-1,1},{-1,0},{-1,-1}};

	void mineGenerate(int bound){
		int rRow;
		int rCol; 
		do{
			rRow = rand()%bound;
			rCol = rand()%bound;
		}while(mineBoard[rRow][rCol]);
		mineBoard[rRow][rCol] = 1;

	}

	void revealNeighbor(int row, int col){
		if(displayBoard[row][col] != '#') return;
		int count = 0;
		int m = mineBoard.size();
		for(int i = 0; i < 8; i++){
			int newx = row + dir[i][0];
			int newy = col + dir[i][1];
			if(newx >= 0 && newx < m && newy >=0 && newy < m && mineBoard[newx][newy]){
				count++;
			}
		}
		displayBoard[row][col] = count+'0';
		if(!count){
			for(int i = 0; i < 8; i++){
				int newx = row + dir[i][0];
				int newy = col + dir[i][1];
				if(newx >= 0 && newx < m && newy >=0 && newy < m){
					revealNeighbor(newx, newy);
				}
			}
		}
	}

public:
	gameBoard() : displayBoard(9,vector<char>(9,'#')),
	mineBoard(9,vector<int>(9,0)) {} 

	void initializeDisplayboard(int level){
		if(level == 2){
			displayBoard = vector<vector<char>>(16, vector<char>(16,'#'));
		}else if(level == 3){
			displayBoard = vector<vector<char>>(24, vector<char>(24,'#'));
		}
	}

	void initializeMineboard(int level){
		srand(time(nullptr));
		if(level == 1){
			for(int i = 0; i < 10; i++){
				mineGenerate(9);
				mineNumber = 10;
			}
		}else if(level == 2){
			mineBoard = vector<vector<int>>(16, vector<int>(16,0));
			for(int i = 0; i < 16; i++){
				mineGenerate(15);
				mineNumber = 16;
			}
		}else{
			mineBoard = vector<vector<int>>(24, vector<int>(24,0));
			for(int i = 0; i < 24; i++){
				mineGenerate(23);
				mineNumber = 24;
			}
		}
	}

	void displayBoardStatus(){
		int m = displayBoard.size();
		int n = displayBoard[0].size();

		cout<<"\n  ";
		for(int i = 0; i < m; i++){
			if(i < 10)
				cout<<" "<<i<<" ";
			else
				cout<<" "<<i;
		}
		cout<<endl;
		cout<<"  ";
		for(int i = 0; i < m; i++){
			cout<<" - ";
		}
		cout<<endl;

		for(int i = 0; i < m; i++){
			string a = "";
			a +=(char)('a' + i); 
			cout<< a + '|';
			for(int j = 0; j < n; j++){
				cout<<" "<<displayBoard[i][j]<<" ";
			}
			cout<<endl;
		}


		cout<<endl;
		/*for(int i = 0; i < m; i++){
			for(int j = 0; j < n; j++){
				cout<<mineBoard[i][j];
				//cout<<mineBoard[i][j];
			}
			cout<<endl;
		}*/

	}

	int mineNumberGet(){
		return mineNumber;
	}

	bool bombStatusGet(){
		return hitbomb;
	}

	void revealBoard(int row, int col){
		int m = mineBoard.size();
		if(mineBoard[row][col]){
			hitbomb = true;
			return;
		}else{
			revealNeighbor(row, col);
		}
	}

	void markBoard(int row, int col){
		if(mineBoard[row][col]){
			mineNumber--;
		}
		displayBoard[row][col] = '!';
	}

		void unmarkBoard(int row, int col){
		if(mineBoard[row][col]){
			mineNumber++;
		}
		displayBoard[row][col] = '#';
	}
};


class userInterface{

private:
	string userOp;
	int userRow, userCol;

public:
	void userInputSet(){
		cout<<"Enter mark, unmark or reveal plus the character and number\n (For example: mark a2): "<<endl;
		
		string userInput;
		getline(cin, userInput);
		string loc, row = "", col;
		istringstream ss(userInput);
		getline(ss, userOp, ' ');
		getline(ss, loc);
		row += loc[0];
		col = loc.substr(1, col.length()-1);
		userRow = row[0]-'a';
		userCol = stoi(col);
	}

	string userOpGet(){
		return userOp;
	}
	vector<int> userLocGet()
	{
		return {userRow, userCol};
	}

};

class gameFrame{

private:

	int gameLevel;

	gameBoard * gb;

	userInterface *ui;

	bool winGame = false;

	bool endGame = false;

	void setLevel(){

		string levelInput;
		int level = -1;
		do{
			getline(cin, levelInput);

			if( levelInput.length()!=1 || !isdigit(levelInput[0]) ||
				( (level = stoi(levelInput)) > 3 || level < 0 ) ){

				cout<<"Please Enter correct number : )\n"<<endl;
				cout<<"Enter the number: ";
			}

		}while(level > 3  || level < 0);

		gameLevel = level;
	}


public:

	void startMenue(){

		string level;
		cout<<endl;
		cout<<"Hello, welcome to my little MineSweeper!"<<endl;
		cout<<"Now Please choose the level you want to start the chanllenge:"<<endl;
		cout<<"1.Beginner"<<endl;
		cout<<"2.Intermediate"<<endl;
		cout<<"3.Advanced"<<endl;
		cout<<"Enter the number: ";

		setLevel();

	}

	void startGame(){
		gb = new gameBoard();
		gb->initializeMineboard(gameLevel);
		gb->initializeDisplayboard(gameLevel);
		ui = new userInterface();
		int minenum;
		bool hitbomb;
		do{
			gb->displayBoardStatus();
			ui->userInputSet();
			string op = ui->userOpGet();
			vector<int> loc = ui->userLocGet();
			if(op == "mark"){
				gb->markBoard(loc[0], loc[1]);
			}else if(op == "unmark"){
				gb->unmarkBoard(loc[0], loc[1]);
			}else{
				gb->revealBoard(loc[0], loc[1]);
			}
			minenum = gb->mineNumberGet();
			hitbomb = gb->bombStatusGet();
		}while(minenum && !hitbomb);

		if(!minenum)
			winGame = true;
	}

	void endGameMessage(){
		if(winGame){
			cout<<"Congratulations! You win : D"<<endl;
		}else{
			cout<<"Sorry, You lose : ("<<endl;
		}

		cout<<"Do you want to start another game now? (Y/N)"<<endl;
		string endGameChoice;
		getline(cin, endGameChoice);
		if(endGameChoice == "N"){
			endGame = true;
		}
	}

	bool endGameChoiceGet(){
		return endGame;
	}

};

int main(int argc, char const *argv[])
{
	bool endGame = false;
	while(!endGame){
		gameFrame newGame;
		newGame.startMenue();
		newGame.startGame();
		newGame.endGameMessage();
		endGame = newGame.endGameChoiceGet();
	}

	return 0;
}