#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

bool game = 1;
const int maxRow = 3, maxCol = 3, instructionCount = 23;
char board[maxRow][maxCol];
int possibleMove[maxRow][maxCol];
int historyMove[5][4];
int botHistory[3][2];
char inPiece[2], inMove[2];
int pieceRow, pieceCol, moveRow, moveCol;
int win = 0;

int turn[23] = {2, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6}; //turnCount
int pmc[23] = {1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5}; //previosMoveCount
int amc[23] = {3, 2, 2, 3, 3, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2};
int pm[23][5][4] = {//previous moves library: array countaining different previos moves set
	{{2, -1, 1, -1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  //instruction 1
	{{2, 0, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  //instruction 2
	{{2, -1, 1, -1}, {0, 1, 1, 1}, {2, 0, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  //instruction 3
	{{2, -1, 1, -1}, {0, 1, 1, 1}, {2, 0, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},  //instruction 4
	{{2, -1, 1, -1}, {0, 0, 1, 0}, {2, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  //instruction 5
	{{2, -1, 1, -1}, {0, 0, 1, -1}, {2, 0, 1, -1}, {0, 0, 0, 0}, {0, 0, 0, 0}},  //instruction 6
	{{2, -1, 1, -1}, {0, 0, 1, -1}, {2, 0, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  //instruction 7
	{{2, -1, 1, -1}, {0, 0, 1, -1}, {2, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},  //instruction 8
	{{2, 0, 1, 0}, {0, -1, 1, -1}, {2, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},  //instruction 9
	{{2, 0, 1, 0}, {0, -1, 1, 0}, {2, -1, 1, -1}, {0, 0, 0, 0}, {0, 0, 0, 0}},  //instruction 10
	{{2, 0, 1, 0}, {0, -1, 1, 0}, {2, -1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  //instruction 11
	{{2, 0, 1, 0}, {0, -1, 1, 0}, {2, -1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},  //instruction 12
	{{2, 0, 1, 0}, {0, -1, 1, 0}, {2, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  //instruction 13
	{{2, -1, 1, -1}, {0, 1, 1, 1}, {2, 0, 1, 0}, {0, -1, 1, 0}, {2, 1, 1, 0}},  //instruction 14
	{{2, -1, 1, -1}, {0, 1, 1, 1}, {2, 0, 1, 1}, {0, 0, 1, 0}, {2, 1, 1, 0}},  //instruction 15
	{{2, -1, 1, -1}, {0, 0, 1, 0}, {2, 1, 1, 0}, {0, 1, 1, 1}, {2, 0, 1, 1}},  //instruction 16
	{{2, 0, 1, 0}, {0, -1, 1, -1}, {2, 1, 1, 1}, {0, 1, 1, 0}, {2, -1, 1, 0}},  //instruction 17
	{{2, 0, 1, 0}, {0, -1, 1, 0}, {2, -1, 1, -1}, {0, 1, 1, 1}, {2, 1, 1, 0}},  //instruction 18
	{{2, 0, 1, 0}, {0, -1, 1, 0}, {2, -1, 1, 0}, {0, 1, 1, 0}, {2, 1, 1, 1}},  //instruction 19
	{{2, 0, 1, 0}, {0, -1, 1, 0}, {2, 1, 1, 1}, {0, 0, 1, 1}, {2, -1, 1, -1}},  //instruction 20
	{{2, 0, 1, 0}, {0, -1, 1, 0}, {2, 1, 1, 1}, {0, 0, 1, 1}, {2, -1, 1, 0}},  //instruction 21
	{{2, 0, 1, 0}, {0, -1, 1, 0}, {2, 1, 1, 0}, {0, 1, 1, 1}, {2, -1, 1, -1}},  //instruction 22
	{{2, 0, 1, 0}, {0, -1, 1, 0}, {2, 1, 1, 0}, {0, 1, 1, 0}, {2, -1, 1, -1}} //instruction 23
};
int am[23][4][4] = {//available moves library: array countaining different available moves set
	{{0, 1, 0, 2}, {0, 0, 0, 2}, {0, 0, -1, 2}},  //instruction 1
	{{0, -1, 0, 2}, {0, -1, 1, 2}, {0, 0, 0, 0}},  //instruction 2
	{{0, -1, 1, 2}, {0, 1, 1, 2}, {0, 0, 0, 0}},  //instruction 3
	{{0, 0, 1, 2}, {0, 0, -1, 2}, {0, 0, 0, 2}},  //instruction 4
	{{0, -1, 1, 2}, {0, 1, -1, 2}, {0, 1, 0, 2}},  //instruction 5
	{{0, 1, 0, 2}, {0, 0, 0, 0}, {0, 0, 0, 0}},  //instruction 6
	{{1, -1, 0, 2}, {0, 1, 0, 2}, {0, 0, 0, 0}},  //instruction 7
	{{1, -1, 1, 2}, {1, -1, 0, 2}, {0, 0, 0, 0}},  //instruction 8
	{{0, 1, -1, 2}, {0, 0, 1, 2}, {0, 0, 0, 0}},  //instruction 9
	{{1, 0, 0, 2}, {0, 1, 0, 2}, {0, 0, 0, 0}},  //instruction 10
	{{0, 1, 0, 2}, {1, 1, -1, 2}, {0, 0, 0, 0}},  //instruction 11
	{{1, 0, 0, 2}, {0, 0, 1, 2}, {0, 0, 0, 0}},  //instruction 12
	{{0, 1, 0, 2}, {0, 1, -1, 2}, {0, 0, 0, 0}},  //instruction 13
	{{0, 0, -1, 2}, {1, 1, 0, 2}, {0, 0, 0, 0}},  //instruction 14
	{{0, -1, 1, 2}, {0, 0, 0, 0}, {0, 0, 0, 0}},  //instruction 15
	{{0, -1, 1, 2}, {0, 0, 0, 0}, {0, 0, 0, 0}},  //instruction 16
	{{1, -1, 0, 2}, {0, 0, 1, 2}, {0, 0, 0, 0}},  //instruction 17
	{{0, 0, -1, 2}, {1, 1, 0, 2}, {0, 0, 0, 0}},  //instruction 18
	{{0, 0, 1, 2}, {1, 0, 0, 2}, {0, 0, 0, 0}},  //instruction 19
	{{1, 0, 0, 2}, {1, 1, 0, 2}, {0, 0, 0, 0}},  //instruction 20
	{{1, 1, 0, 2}, {0, 1, -1, 2}, {0, 0, 0, 0}},  //instruction 21
	{{1, 0, -1, 2}, {1, 1, 0, 2}, {0, 0, 0, 0}},  //instruction 22
	{{1, 0, 0, 2}, {0, 0, -1, 2}, {0, 0, 0, 0}} //instruction 23
};

struct stats{
  int win;
  int territoryWin;
  int stuckWin;
  int lost;
};
stats player = {0, 0, 0, 0};
stats computer = {0, 0, 0, 0};

void writeMoveLib(string fileName);
void readMoveLib(string fileName);
void resetGame();
void resetBoard();
void resetPossibleMove();
void resetHistoryMove();
void resetBotHistory();
void selectPiece();
void selectMove();
bool compareArray(int arr1[4], int arr2[4]);
bool compareArrayFlip(int arr1[4], int arr2[4]);
void moveTo(int oldRow, int oldCol, int newRow, int newCol);
void botMove(int prevMove[5][4], int turnCount);
void botLearn(int t);
void appendMoveHistory(int index, int pr, int pc, int mr, int mc);
void checkMove(int inrow, int col);
bool checkBot(int inrow, int col);
bool checkPlayer(int inrow, int col);
void checkTerritory();
void checkContinue(int t);
bool checkWin(int t);
void printBoard();
void printSelected();
void printPlayersTurn();
void printBotsTurn();
void controls();
void startMenu();
void endMenu();
void statistics();

int main(){
	writeMoveLib("hexapawnOriginal.txt");
	startMenu();
	
	int inGameTurn;
	while (game) {
		for(inGameTurn = 0; inGameTurn < 7&&win<1; inGameTurn += 2) {
			printPlayersTurn();
			if(checkWin(inGameTurn-1)) break;
			selectPiece();
			checkMove(pieceRow, pieceCol);
			printSelected();
			selectMove();
			moveTo(pieceRow, pieceCol, moveRow, moveCol);
			printBotsTurn();
			if(checkWin(inGameTurn)) break;
			appendMoveHistory(inGameTurn, pieceRow, pieceCol, moveRow, moveCol);
			resetPossibleMove();
			botMove(historyMove, inGameTurn + 1);
			printBoard();
		}
		botLearn((inGameTurn/2));
		statistics();
		endMenu();
	}
	
	return 0;
}

void writeMoveLib(string fileName){
	const string f = fileName;
	ofstream outfile;
	outfile.open(fileName.c_str());
	for(int i = 0; i<23; i++){
		outfile<<turn[i]<<endl<<pmc[i]<<endl<<amc[i]<<endl;
		for(int j = 0; j<pmc[i]; j++){
			outfile<<pm[i][j][0]<<'\t'<<pm[i][j][1]<<'\t'<<pm[i][j][2]<<'\t'<<pm[i][j][3]<<endl;
		}
		for(int j = 0; j<amc[i]; j++){
			outfile<<am[i][j][0]<<'\t'<<am[i][j][1]<<'\t'<<am[i][j][2]<<'\t'<<am[i][j][3]<<endl;
		}
	}
	outfile.close();
}

void readMoveLib(string fileName){
	ifstream infile;
	infile.open(fileName.c_str());
	for(int i = 0; i<instructionCount; i++){
//		cout<<"reading instruction "<<i<<endl;
		infile>>turn[i];
//		cout<<turn[i]<<endl;
		infile>>pmc[i];
//		cout<<pmc[i]<<endl;
		infile>>amc[i];
//		cout<<amc[i]<<endl;
		for(int j = 0; j<pmc[i]; j++){
			infile>>pm[i][j][0]>>pm[i][j][1]>>pm[i][j][2]>>pm[i][j][3];
		}
		for(int j = 0; j<amc[i]; j++){
			infile>>am[i][j][0]>>am[i][j][1]>>am[i][j][2]>>am[i][j][3];
		}
	}
	infile.close();
}

void resetGame(){
	resetBoard();
	resetPossibleMove();
	resetHistoryMove();
	resetBotHistory();
	win = 0;
}

void resetBoard(){
	for(int i = 0; i<maxCol; i++) board[0][i] = 'r';
	for(int i = 0; i<maxCol; i++) for(int j = 1; j<maxRow-1; j++)  board[j][i] = 'o';
	for(int i = 0; i<maxCol; i++) board[maxRow-1][i] = 'b'; 
	for(int i = 0; i<5; i++) for(int j = 0; j<4; j++) historyMove[i][j] = 0;
}

void resetPossibleMove(){
	for(int i = 0; i<maxRow; i++) for(int j = 0; j<maxCol; j++) possibleMove[i][j] = 0;
}

void resetHistoryMove(){
	for(int i = 0; i<maxRow; i++) for(int j = 0; j<maxCol; j++) historyMove[i][j] = 0;
}

void resetBotHistory(){
	for(int i = 0; i<3; i++) for(int j = 0; j<2; j++) possibleMove[i][j] = 0;
}

void selectPiece(){
	cout<<"Choose a piece to move: ";
	cin>>inPiece[0]>>inPiece[1];
	if(inPiece[0] > 95) pieceRow = inPiece[0] - 97;
		else pieceRow = inPiece[0] - 65;
	pieceCol = inPiece[1] - 49;
	while(board[pieceRow][pieceCol] != 'b'){
		cout<<"Invalid Piece! please choose a 'b' piece\nChoose a piece to move: ";
		cin>>inPiece[0]>>inPiece[1];
		if(inPiece[0] > 95) pieceRow = inPiece[0] - 97;
		else pieceRow = inPiece[0] - 65;
		pieceCol = inPiece[1] - 49;
	}
}

void checkMove(int inrow, int col){
	int row = inrow - 1;
	if(col > 0)	if(board[row][col-1]=='r')
		possibleMove[row][col-1] = 1;
	if(board[row][col]=='o')
		possibleMove[row][col] = 1;
	if(col < maxCol-1)	if(board[row][col+1]=='r')
		possibleMove[row][col+1] = 1;
}

void selectMove(){
	cout<<"Choose a cell to move to: ";
	cin>>inMove[0]>>inMove[1];
	if(inMove[0] > 95) moveRow = inMove[0] - 97;
	else moveRow = inMove[0] - 65;
	moveCol = inMove[1] - 49;
//	cout<<possibleMove[moveRow][moveCol]<<endl;
	while(possibleMove[moveRow][moveCol] < 1){
		cout<<"Invalid Move! please choose a cell with [_]\nChoose a cell to move to: ";
		cin>>inMove[0]>>inMove[1];
		if(inMove[0] > 95) moveRow = inMove[0] - 97;
		else moveRow = inMove[0] - 65;
		moveCol = inMove[1] - 49;
	}
}

void printSelected(){
	char row = 'A';
	cout<<"\nState of the Board:\n====================\n\n\t 1 \t 2 \t 3 \n";
	for(int i = 0;i <3; i++){	
		cout<<row++<<"\t";
		for(int j = 0; j<maxCol; j++){
			if(i == pieceRow&&j == pieceCol) cout<<">"<<board[i][j]<<"<\t";
			else if(possibleMove[i][j] > 0) cout<<"["<<board[i][j]<<"]\t";
			else cout<<" "<<board[i][j]<<" \t";
		}
		cout<<endl;
	}
	cout<<"\n====================\n\n";
	
//	for(int i = 0; i<maxRow; i++){
//		for(int j = 0; j<maxCol; j++){
//			if(i == pieceRow&&j == pieceCol) cout<<"["<<board[i][j]<<"] ";
//			else if(possibleMove[i][j] > 0) cout<<">"<<board[i][j]<<"< ";
//			else cout<<" "<<board[i][j]<<"  ";
//		}
//		cout<<endl;
//	}
}

void printBoard(){
	char row = 'A';
	cout<<"State of the Board:\n=============================\n\n\t 1 \t 2 \t 3 \n";
	for(int i = 0;i <3; i++){	
		cout<<row++<<"\t";
		for(int j = 0; j<maxCol; j++){
			cout<<" "<<board[i][j]<<" \t";
		}
		cout<<endl;
	}
	cout<<"\n=============================\n\n";
	
//	for(int i = 0; i<maxRow; i++){
//		for(int j = 0; j<maxCol; j++){
//			cout<<" "<<board[i][j]<<"  ";
//		}
//		cout<<endl;
//	}
}

void printPlayersTurn(){
	cout<<"******************************************\n\n";
	cout<<"YOUR TURN\n\n";
	cout<<"******************************************\n";
	printBoard();
}

void printBotsTurn(){
	cout<<"******************************************\n\n";
	cout<<"COMPUTER'S TURN\n\n";
	cout<<"******************************************\n";
	printBoard();
}

void botLearn(int t){
	int modifier = ((win<2)*-1) + (win>1);
	for(int i = 0; i<t; i++){
		am[botHistory[i][0]][botHistory[i][1]][3] += modifier*(am[botHistory[i][0]][botHistory[i][1]][3]>0);
	}
	writeMoveLib("hexapawnCurrent.txt");
}

void appendMoveHistory(int index, int pr, int pc, int mr, int mc){
	historyMove[index][0] = pr;
	historyMove[index][1] = pc;
	historyMove[index][2] = mr;
	historyMove[index][3] = mc;
}

void moveTo(int oldRow, int oldCol, int newRow, int newCol){
	cout<<"Moving from ("<<oldRow<<", "<<oldCol<<") to ("<<newRow<<", "<<newCol<<")\n\n\n";
	board[newRow][newCol] = board[oldRow][oldCol];
	board[oldRow][oldCol] = 'o';
}

bool compareArray(int arr1[4], int arr2[4]){
	bool temp = true;
	for(int i = 0; i<4; i++){
//		cout<<arr1[i]<<'\t'<<arr2[i]<<temp<<endl;
		if(i%2>0) temp = temp&&(arr1[i]+1 == arr2[i]);
		else temp = temp&&(arr1[i] == arr2[i]);
	}
	return temp;
}

bool compareArrayFlip(int arr1[4], int arr2[4]){
	bool temp = true;
	for(int i = 0; i<4; i++){
		if(i%2>0) temp = temp&&((arr1[i]*-1)+1 == arr2[i]);
		else temp = temp&&(arr1[i] == arr2[i]);
	}
	return temp;
}

void botMove(int prevMove[5][4], int turnCount){
//	cout<<turnCount+1<<endl;
	bool cont = true;
//	cout<<"found turn count = "<<turnCount<<endl;
	for(int i = 0; i<23; i++){
		if (turnCount+1 == turn[i]&&cont){
//			cout<<"matched turn count"<<endl; //successful
			bool temp = true;
			int flipCoeff = 1;
			int maxRand = 0;
			
			for(int j = 0; j<pmc[i]&&temp; j++){
				temp = temp&&(compareArray(pm[i][j], historyMove[j]));
//				cout<<"checking"<<temp<<endl;
//				maxRand += am[i][j][3];	
			}
			
			if(!temp) {
				temp = true;
				
				for(int j = 0; j<pmc[i]&&temp; j++){
					temp = temp&&(compareArrayFlip(pm[i][j], historyMove[j]));
				}
				
				flipCoeff = -1;
			}
			
			if(temp){
				//define local variable
				int randomNum;
				int botMovePointer = 0;
				
				//calculate maxmum for rng (n(SampleSpace))
				for(int j = 0; j<amc[i]; j++) maxRand += am[i][j][3]; 
					 
				//reset rng, generate a random number from 0 - (maxRand-1)
				srand(time(NULL));
				randomNum = 0+rand()%(maxRand);

				//convert random number to move decision
				for(int j = 0; j<amc[i]&&cont; j++){
					if(randomNum > am[i][j][3]) {
						botMovePointer++;
						randomNum -= am[i][j][3];
					}
					else break;
				}
							
				//proceed to move piece by randomized decision based on probability
				moveTo(am[i][botMovePointer][0], (am[i][botMovePointer][1]*flipCoeff)+1, am[i][botMovePointer][0]+1, ((am[i][botMovePointer][2]+am[i][botMovePointer][1])*flipCoeff)+1);
				
				//append move history
				appendMoveHistory(turnCount, am[i][botMovePointer][0], (am[i][botMovePointer][1]*flipCoeff)+1, am[i][botMovePointer][0]+1, ((am[i][botMovePointer][2]+am[i][botMovePointer][1])*flipCoeff)+1);
				
				//append bot history
				botHistory[(turnCount/2)][0] = i;
				botHistory[(turnCount/2)][1] = botMovePointer;
				
				//stop matching move
				cont = false;
			}
		}
	}
}

bool checkPlayer(int inrow, int col){
	bool temp = 0;
	int row = inrow - 1;
	if(col > 0&&temp < 1)	if(board[row][col-1]=='r') temp = 1;
	if(board[row][col]=='o'&&temp < 1) temp = 1;
	if(col < maxCol-1&&temp < 1)	if(board[row][col+1]=='r') temp = 1;
	return temp;
}

bool checkBot(int inrow, int col){
	bool temp = 0;
	int row = inrow + 1;
//	cout<<"checking\t"<<inrow<<col<<"\t";
//	for(int i = 0; i<maxCol; i++) cout<<board[row][i];
//	cout<<endl;
	if(col > 0&&temp < 1)	if(board[row][col-1]=='b') temp = 1;
	if(board[row][col]=='o'&&temp < 1) temp = 1;
	if(col < maxCol-1&&temp < 1)	if(board[row][col+1]=='b') temp = 1;	
	return temp;
}

void checkTerritory(){
	for(int i = 0; i<maxCol&&win<1; i++){
		if(board[0][i] == 'b') {
			win = 1;
			player.territoryWin += 1;
			player.win += 1;
			computer.lost += 1;
		}
		else if (board[2][i] == 'r'){
			win = 2;
			computer.territoryWin += 1;
			computer.win += 1;
			player.lost += 1;
		} 
	}
}

void checkContinue(int t){
	bool botPass = 0;
	bool playerPass = 0;
	for(int i = 0; i<maxRow&&win<1&&t%2 == 1; i++){
		for(int j = 0; j<maxCol&&win<1; j++){
			if(board[i][j] == 'b'){
				playerPass = playerPass||checkPlayer(i, j);
			}
		}
	}
	for(int i = 0; i<maxRow&&win<1&&t%2 == 0; i++){
		for(int j = 0; j<maxCol&&win<1; j++){
			if(board[i][j] == 'r'){
				botPass = botPass||checkBot(i, j);
			}
		}
	}
	if(!botPass&&t%2 == 0) {
		win = 1;
		player.stuckWin += 1;
		player.win += 1;
		computer.lost += 1;
	}
	else if(!playerPass&&t%2 == 1) {
		win = 2;
		computer.stuckWin += 1;
		computer.win += 1;
		player.lost += 1;
	} 
}

bool checkWin(int t){
	checkTerritory();
	if(win<1) checkContinue(t);
	if(win == 1) cout<<"\nPLAYER WINS!\n\n\n";
	else if(win == 2) cout<<"\nCOMPUTER WINS!\n\n\n";
	return win > 0;
}

void controls(){
	cout<<"When the program started, the player will see the message below \n";
	cout<<"with a question to ask what you want to do, \n";
	cout<<"which you will need to answer your choice in number and press ENTER key.\n";
	cout<<"Welcome to Hexapawn!\n";
	cout<<"\n";
	cout<<"You are playing as the blue team which are the 'b' pieces. \n";
	cout<<"You are playing against the red team, which is the computer. \n";
	cout<<"Your objective is to win by: \n";
	cout<<"	reaching the other side of the board. \n";
	cout<<"	or \n";
	cout<<"	forcing the opponent to an unmovable position. \n";
	cout<<"	\n";
	cout<<"What would you like to do? \n";
	cout<<"1.Start [new AI, overwrite existing AI] \n";
	cout<<"2.Start [existing AI] \n";
	cout<<"3.Game Controls \n";
	cout<<"4.Exit \n";
	cout<<"\n";
	cout<<"Choose an Action: 1 \n";
	cout<<"\n";
	cout<<"The 1st choice allow the player to start the game with \n";
	cout<<"an untrained bot and will overwrite the present bot. \n";
	cout<<"The 2nd option present you a trained bot. \n";
	cout<<"The 3rd choice will show up an explanation of the game \n";
	cout<<"to the player and \n";
	cout<<"the 4th choice will let's the player exit the game. \n";
	cout<<"In this case, we will choose the 1st choice thus enter number 1.\n";
	cout<<"\n";
	cout<<"Then the following message will appear.\n";
	cout<<"\n";
	cout<<"******************************************\n";
	cout<<"\n";
	cout<<"YOUR TURN\n";
	cout<<"\n";
	cout<<"******************************************\n";
	cout<<"State of the Board:\n";
	cout<<"=============================\n";
	cout<<"\n";
	cout<<"         1       2       3\n";
	cout<<"A        r       r       r\n";
	cout<<"B        o       o       o\n";
	cout<<"C        b       b       b\n";
	cout<<"\n";
	cout<<"=============================\n";
	cout<<"\n";
	cout<<"Choose a piece to move: C1\n";
	cout<<"\n";
	cout<<"The player's pieces is dubbed as \"b\" \n";
	cout<<"while the \"r\" piece represent the bot's pieces. \n";
	cout<<"Here, the program allow the player to choose the piece \n";
	cout<<"they want to move by enter the location, such as C1, C2, or C3. \n";
	cout<<"As we want to move the bottom left piece, \n";
	cout<<"we will enter C1 in the terminal.\n";
	cout<<"\n";
	cout<<"Then, the player will be shown the board with a >_< symbol \n";
	cout<<"dubbing the piece that player chose \n";
	cout<<"and the tiles dub with [_] \n";
	cout<<"are representing the tile that the piece can move to.\n";
	cout<<"\n";
	cout<<"State of the Board:\n";
	cout<<"====================\n";
	cout<<"\n";
	cout<<"         1       2       3\n";
	cout<<"A        r       r       r\n";
	cout<<"B       [o]      o       o\n";
	cout<<"C       >b<      b       b\n";
	cout<<"\n";
	cout<<"====================\n";
	cout<<"\n";
	cout<<"Choose a cell to move to: B1\n";
	cout<<"\n";
	cout<<"After the player choose where to move the piece to, \n";
	cout<<"in this case it is B1 tile, \n";
	cout<<"the piece will move according to player choice. \n";
	cout<<"Then the bot will move it pieces \n";
	cout<<"and the program will show the state of board \n";
	cout<<"after the bot turn thus resume the player turn.\n";
	cout<<"\n";
	cout<<"Finally, if any sides were to win, \n";
	cout<<"the program will show the who the winner is, \n";
	cout<<"game statistics, \n";
	cout<<"and the following question will appear.\n";
	cout<<"\n";
	cout<<"******************************************\n";
	cout<<"\n";
	cout<<"YOUR TURN\n";
	cout<<"\n";
	cout<<"******************************************\n";
	cout<<"State of the Board:\n";
	cout<<"=============================\n";
	cout<<"\n";
	cout<<"         1       2       3\n";
	cout<<"A        r       o       r\n";
	cout<<"B        o       b       o\n";
	cout<<"C        r       o       b\n";
	cout<<"\n";
	cout<<"=============================\n";
	cout<<"\n";
	cout<<"\n";
	cout<<"COMPUTER WINS!\n";
	cout<<"\n";
	cout<<"\n";
	cout<<"******************************************\n";
	cout<<"total game played this session:	1\n";
	cout<<"PLAYER:                         COMPUTER:\n";
	cout<<" win:           0                win:           1\n";
	cout<<" win by   :     0                win by   :     1\n";
	cout<<" territory                       territory\n";
	cout<<" win by stuck:  0                win by stuck:  0\n";
	cout<<" lost:          1                lost:          0\n";
	cout<<"******************************************\n";
	cout<<"\n";
	cout<<"1.New Game\n";
	cout<<"2.Return to Menu\n";
	cout<<"3.Exit\n";
	cout<<"Choose an Action:\n";
	cout<<"\n";
	cout<<"\n";
	cout<<"The 1st choice will enable the player to start a new game \n";
	cout<<"with the same bot that the player just battle with. \n";
	cout<<"The 2nd option will bring the player back to the start screen. \n";
	cout<<"Lastly, the 3rd option will exit the game upon chosen.\n";
	startMenu();
}

void startMenu(){
	bool current;
	ifstream newGame;
	newGame.open("hexapawnCurrent.txt");
	current = newGame.good();
	newGame.close(); 
	
	int temp = 1;
	cout<<"Welcome to Hexapawn!\n\n";
	cout<<"You are playing as the blue team which are the 'b' pieces.\n";
	cout<<"You are playing against the red team, which is the computer.\n";
	cout<<"Your objective is to win by:\n";
	cout<<"\treaching the other side of the board.\n";
	cout<<"\tor\n";
	cout<<"\tforcing the opponent to an unmovable position.\n\n";
	cout<<"What would you like to do?\n";
	cout<<"1.Start [new AI, overwrite existing AI]\n";
	cout<<"2.Start [existing AI]\n";
	cout<<"3.Game Controls\n";
	cout<<"4.Exit\n\n";
	do {
		if (temp<1 || temp>5) cout<<"Invalid Input!\n";
		cout<<"Choose an Action: ";
		cin>>temp;
	} while (temp<1 || temp>5);
	cout<<"\n\n\n\n\n\n\n\n";
	switch(temp){
		case 1: {
			readMoveLib("hexapawnOriginal.txt");
			writeMoveLib("hexapawnCurrent.txt");
			resetGame();
			break;
		}
		case 2: {
			if(current) readMoveLib("hexapawnCurrent.txt");
			else readMoveLib("hexapawnOriginal.txt");
			resetGame();
			break;
		}
		case 3:	{
			controls();
			break;
		}
		case 4:{
			game = 0;
			break;
		}
	}
}

void endMenu(){
	int temp = 1;
	cout<<"1.New Game\n";
	cout<<"2.Return to Menu\n";
	cout<<"3.Exit\n";
	do {
		if (temp<1 || temp>4) cout<<"Invalid Input!\n";
		cout<<"Choose an Action: ";
		cin>>temp;
	} while (temp<1 || temp>4);
	switch(temp){
		case 1: {
			resetGame();
			break;
		}
		case 2: {
			startMenu();
			break;
		}
		case 3:{
			game = 0;
			break;
		}
	}
}

void statistics(){
	cout<<"******************************************\n";
	cout<<"total game played this session:\t"<<player.win+player.lost<<endl;
	cout<<"PLAYER:\t\t\t\tCOMPUTER:\n";
	cout<<" win:\t\t"<<player.win<<"\t\t win:\t\t"<<computer.win<<"\n";
	cout<<" win by   :\t"<<player.territoryWin<<"\t\t win by   :\t"<<computer.territoryWin<<"\n";
	cout<<" territory\t\t\t territory\n";
	cout<<" win by stuck:\t"<<player.stuckWin<<"\t\t win by stuck:\t"<<computer.stuckWin<<"\n";
	cout<<" lost:\t\t"<<player.lost<<"\t\t lost:\t\t"<<computer.lost<<"\n";
	cout<<"******************************************\n\n";
}
