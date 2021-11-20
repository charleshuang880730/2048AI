/**********************set _2048AI = 0 to play   2048   *************************/
/**********************set _2048AI = 1 to launch 2048 AI*************************/

/*I use conio.h handle console IO. You may need to change the library and the code according to your OS*/
/*If you don's want to play the game, simply remove this line and the corresponding _getch()*/
int _2048AI = 1;

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
//#include <conio.h> 

double PROB4 = 0.1;


int FastLog2(int a) {
	if (a == 0) {
		return 0;
	}

	int count = 0;
	while (a != 1) {
		a = a >> 1;
		count++;
	}
	return count;
}

// if you want to map a borad to a scord, call this function to map the board to an unsigned long long,
// and then map this unsigned long long to a score
unsigned long long BoardToULL(int board[4][4]) {
	unsigned long long h;
	int logBoard[4][4];
	int exp;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 0) {
				logBoard[i][j] = 0;
			}
			else {
				logBoard[i][j] = FastLog2(board[i][j]);
			}
		}
	}


	h = 0;
	unsigned long long pow2 = 1;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int temp = logBoard[i][j];
			for (int k = 8; k > 0; k = k >> 1) {
				if (temp >= k) {
					h = h | pow2;
					temp -= k;
				}

				pow2 = pow2 << 1;
			}
		}
	}

	return h;
}

//Copy the content of src to dst
void CopyBoard(int dst[4][4], int src[4][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			dst[i][j] = src[i][j];
		}
	}
}

//return 1 if the contents of b1 and b2 are the same
//return 0 otherwise
int AreEqual(int b1[4][4], int b2[4][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (b1[i][j] != b2[i][j]) {
				return 0;
			}
		}
	}
	return 1;
}

void PrintBoard(int board[4][4]) {
	for (int y = 0; y < 17; y++) {
		if (y % 4 == 0) {
			printf("*****************************\n");
		}
		else if (y % 4 == 1 || y % 4 == 3) {
			printf("*      *      *      *      *\n");
		}
		else {
			int i = y / 4;
			for (int j = 0; j < 4; j++) {
				printf("*");

				if (board[i][j] == 0) {
					printf("      ");
				}
				else if (board[i][j] < 10) {
					printf("   %d  ", board[i][j]);
				}
				else if (board[i][j] < 100) {
					printf("  %d  ", board[i][j]);
				}
				else if (board[i][j] < 1000) {
					printf("  %d ", board[i][j]);
				}
				else {
					printf(" %d ", board[i][j]);
				}
			}
			printf("*\n");
		}
	}

	return;
}

//print the old board, the action, and the new board
void PrintGame(int oldBoard[4][4], int newBoard[4][4], char a) {
	PrintBoard(oldBoard);
	if (a == 'w') {
		printf("\n\nUp\n\n");
	}
	else if (a == 's') {
		printf("\n\nDown\n\n");
	}
	else if (a == 'd') {
		printf("\n\nRight\n\n");
	}
	else if (a == 'a') {
		printf("\n\nLeft\n\n");
	}
	PrintBoard(newBoard);
}

//return the number of 0s in the board
int GetNEmptyPos(int b[4][4]) {
	int count = 0;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (b[i][j] == 0) {
				count++;
			}
		}
	}

	return count;
}

int GenNewNumber() {
	if ((double)rand() / (double)RAND_MAX < PROB4) {
		return 4;
	}
	return 2;
}

void InitBoard(int board[4][4]) {
	int initCol1, initRow1, initCol2, initRow2;
	initCol1 = rand() % 4;
	initRow1 = rand() % 4;

	initCol2 = rand() % 4;
	initRow2 = rand() % 4;

	while (initCol1 == initCol2 && initRow1 == initRow2) {
		initCol2 = rand() % 4;
		initRow2 = rand() % 4;
	}


	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			board[i][j] = 0;
		}
	}

	board[initCol1][initRow1] = GenNewNumber();
	board[initCol2][initRow2] = GenNewNumber();

	return;
}


void Shift(int board[4][4], int oldX, int oldY, int newX, int newY) {
	if (oldX == newX && oldY == newY) {
		return;
	}
	else {
		board[newX][newY] = board[oldX][oldY];
		board[oldX][oldY] = 0;
		return;
	}
}

void UpdateIthColumn(int board[4][4], int i, char a) {
	int dir = 1;
	int first = 0;
	if (a == 's') {
		dir = -1;
		first = 3;
	}

	int hasNumber = 0;
	int nextPos = 0;

	for (int j = first; j >= 0 && j < 4; j += dir) {
		if (board[j][i] > 0) {
			hasNumber = 1;
			Shift(board, j, i, first, i);
			nextPos = j + dir;
			break;
		}
	}

	if (!hasNumber) {
		return;
	}

	int mergePos = first;
	int emptyPos = first + dir;
	int hasMerged = 0;

	for (int j = nextPos; j >= 0 && j < 4; j += dir) {
		if (board[j][i] > 0) {
			if (!hasMerged && board[mergePos][i] == board[j][i]) {
				board[mergePos][i] = board[mergePos][i] << 1;
				board[j][i] = 0;
				hasMerged = 1;
			}
			else {
				Shift(board, j, i, emptyPos, i);
				mergePos = emptyPos;
				emptyPos += dir;
				hasMerged = 0;
			}
		}
	}
}

void UpdateIthRow(int board[4][4], int i, char a) {
	int dir = 1;
	int first = 0;
	if (a == 'd') {
		dir = -1;
		first = 3;
	}

	int hasNumber = 0;
	int nextPos = 0;

	for (int j = first; j >= 0 && j < 4; j += dir) {
		if (board[i][j] > 0) {
			hasNumber = 1;
			Shift(board, i, j, i, first);
			nextPos = j + dir;
			break;
		}
	}

	if (!hasNumber) {
		return;
	}

	int mergePos = first;
	int emptyPos = first + dir;
	int hasMerged = 0;

	for (int j = nextPos; j >= 0 && j < 4; j += dir) {
		if (board[i][j] > 0) {
			if (!hasMerged && board[i][mergePos] == board[i][j]) {
				board[i][mergePos] = board[i][mergePos] << 1;
				board[i][j] = 0;
				hasMerged = 1;
			}
			else {
				Shift(board, i, j, i, emptyPos);
				mergePos = emptyPos;
				emptyPos += dir;
				hasMerged = 0;
			}
		}
	}
}

int IsGameOver(int board[4][4]) {
	if (GetNEmptyPos(board) > 0) {
		return 0;
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == board[i][j + 1]) {
				return 0;
			}
		}
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == board[i + 1][j]) {
				return 0;
			}
		}
	}

	return 1;
}

void UpdateBoard(int board[4][4], char a) {
	if (a == 'w' || a == 's') {
		for (int i = 0; i < 4; i++) {
			UpdateIthColumn(board, i, a);
		}
	}
	else {
		for (int i = 0; i < 4; i++) {
			UpdateIthRow(board, i, a);
		}
	}
	return;
}

void AddNewNumber(int board[4][4]) {
	int nEmptyPos = GetNEmptyPos(board);

	int r = 1 + rand() % nEmptyPos;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (board[i][j] == 0) {
				r--;
				if (r == 0) {
					board[i][j] = GenNewNumber();
					break;
				}
			}
		}
	}
}
int big(int a,int b){

	if(a>b)
		return a;
	else
		return b;
}

int Smoothness(int b[4][4]){

	int count;
	count=-(abs(FastLog2(b[0][1])-FastLog2(b[0][0]))+abs(FastLog2(b[1][0])-FastLog2(b[0][0]))+abs(FastLog2(b[0][2])-FastLog2(b[0][1]))+abs(FastLog2(b[1][1])-FastLog2(b[0][1]))+abs(FastLog2(b[0][3])-FastLog2(b[0][2]))+abs(FastLog2(b[1][2])-FastLog2(b[0][2]))+abs(FastLog2(b[1][3])-FastLog2(b[0][3]))+abs(FastLog2(b[1][1])-FastLog2(b[1][0]))+abs(FastLog2(b[2][0])-FastLog2(b[1][0]))+abs(FastLog2(b[1][2])-FastLog2(b[1][1]))+abs(FastLog2(b[2][1])-FastLog2(b[1][1]))+abs(FastLog2(b[1][3])-FastLog2(b[1][2]))+abs(FastLog2(b[2][2])-FastLog2(b[1][2]))+abs(FastLog2(b[2][3])-FastLog2(b[1][3]))+abs(FastLog2(b[2][1])-FastLog2(b[2][0]))+abs(FastLog2(b[3][0])-FastLog2(b[2][0]))+abs(FastLog2(b[2][2])-FastLog2(b[2][1]))+abs(FastLog2(b[3][1])-FastLog2(b[2][1]))+abs(FastLog2(b[2][3])-FastLog2(b[2][2]))+abs(FastLog2(b[3][2])-FastLog2(b[2][2]))+abs(FastLog2(b[3][3])-FastLog2(b[2][3])));	
//	printf("count is %d\n",count);
	return count;

}
int monotonicity(int b[4][4]){
	int upbig=0,downbig=0,rightbig=0,leftbig=0;

	for(int i=0;i<4;i++){
		for(int j=0;j<3;j++){
			if(b[j+1][i]>b[j][i]){
				if(b[j][i]==0){
					continue;
				}
				downbig+=(FastLog2(b[j+1][i])-FastLog2(b[j][i]));

			}

		}
	}
	for(int i=0;i<4;i++){
		for(int j=0;j<3;j++){
			if(b[j+1][i]<b[j][i]){
				if(b[j+1][i]==0){
					continue;
				}
				upbig+=(FastLog2(b[j][i])-FastLog2(b[j+1][i]));

			}

		}
	}

	for(int i=0;i<4;i++){
		for(int j=0;j<3;j++){
			if(b[i][j+1]<b[i][j]){
				if(b[i][j+1]==0){
					continue;
				}
				rightbig+=(FastLog2(b[i][j])-FastLog2(b[i][j+1]));

			}

		}
	}
	for(int i=0;i<4;i++){
		for(int j=0;j<3;j++){
			if(b[i][j+1]>b[i][j]){
				if(b[i][j]==0){
					continue;
				}
				leftbig+=(FastLog2(b[i][j+1])-FastLog2(b[i][j]));

			}

		}
	}



	upbig*=(-1);
	rightbig*=(-1);
	leftbig*=(-1);
	downbig*=(-1);
	int mon=(big(upbig,downbig)+big(leftbig,rightbig));
//	printf("mono is %d\n",mon);
	return mon; 
	/*
	   int leftcount[100],rightcount[100],upcount[100],downcount[100];
	   int leftcounter=0,rightcounter=0,upcounter=0,downcounter=0;
	   int lvalue,rvalue,uvalue,dvalue;

	//left>right
	int i=0,j=0;
	while(i<4){
	if(map[i][j]>map[i][j+1]&&map[i][j]!=0&&map[i][j+1]!=0){
	leftcount[leftcounter]=FastLog2(map[i][j])-FastLog2(map[i][j+1]);
	leftcounter++;
	}
	j++;
	if(j==3){
	i++;
	j=0;
	}
	}
	//right>left
	int a=0,b=0;
	while(a<4){
	if(map[a][b]<map[a][b+1]&&map[a][b]!=0&&map[a][b+1]!=0){
	rightcount[rightcounter]=FastLog2(map[a][b+1])-FastLog2(map[a][b]);
	rightcounter++;
	}
	b++;
	if(b==3){
	a++;
	b=0;
	}
	}
	//up>down
	int c=0,d=0;
	while(d<4){
	if(map[c][d]>map[c+1][d]&&map[c][d]!=0&&map[c+1][d]!=0){
	upcount[upcounter]=FastLog2(map[c][d])-FastLog2(map[c+1][d]);
	upcounter++;
	}
	c++;
	if(c==3){
	d++;
	c=0;
	}
	}
	//down>up
	int e=0,f=0;
	while(f<4){
	if(map[e][f]<map[e+1][f]&&map[e][f]!=0&&map[e+1][f]!=0){
	downcount[downcounter]=FastLog2(map[e+1][f])-FastLog2(map[e][f]);
	downcounter++;
	}
	e++;
	if(e==3){
	f++;
	e=0;
	}
	}
	for(int x=0;x<=leftcounter;x++){
	lvalue+=leftcount[x];
	}
	for(int x=0;x<=rightcounter;x++){
	rvalue+=rightcount[x];
	}
	for(int x=0;x<=upcounter;x++){
	uvalue+=upcount[x];
	}
	for(int x=0;x<=downcounter;x++){
	dvalue+=downcount[x];
	}
	lvalue=lvalue*-1;
	rvalue=rvalue*-1;
	uvalue=uvalue*-1;
	dvalue=dvalue*-1;
	int tempt1,tempt2;

	if(lvalue>rvalue)
		tempt1=lvalue;
	if(lvalue<rvalue)
		tempt1=rvalue;
	if(uvalue>dvalue)
		tempt2=uvalue;
	if(uvalue<dvalue)
		tempt2=dvalue;
	printf("mono is %d\n",tempt1+tempt2);

	return tempt1+tempt2;
	*/
}
float Emptycells(int map[4][4]){

	int emptynum=0;
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			if(map[i][j]==0){
				emptynum++;
			}
		}
	}
	if(emptynum==0)
		return -0.87;
	if(emptynum==1)
		return 0;
	if(emptynum==2)
		return 0.69;
	if(emptynum==3)
		return 1.09;
	if(emptynum==4)
		return 1.38;
	if(emptynum==5)
		return 1.6;
	if(emptynum==6)
		return 1.79;
	if(emptynum==7)
		return 1.94;
	if(emptynum==8)
		return 2.07;
	if(emptynum==9)
		return 2.19;
	if(emptynum==10)
		return 2.3;
	if(emptynum==11)
		return 2.39;
	if(emptynum==12)
		return 2.48;
	if(emptynum==13)
		return 2.56;
	if(emptynum==14)
		return 2.63;
	if(emptynum==15)
		return 2.69;

}
int maxvalue(int map[4][4]){

	int max=0;
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			if(map[i][j]>max){
				max=map[i][j];
			}
		}
	}
//	printf("max is %d\n",FastLog2(max));
	return FastLog2(max);

}
float maximum(float a,float b,float c,float d){

	float hold=-100;
	if(a>hold){
		hold=a;
	}
	if(b>hold){
		hold=b;
	}
	if(c>hold){
		hold=c;
	}
	if(d>hold){
		hold=d;
	}
//	printf("hold is %f\n",hold);
	return hold;

}

float wscore(int b[4][4]){

	int c[4][4];
	float smoothweight=0.1,emptyweight=2.7;
	CopyBoard(c,b);
	UpdateBoard(c,'w');
	if(AreEqual(c,b))
		return -100;
	AddNewNumber(c);
	int w[4][4],a[4][4],s[4][4],d[4][4];
	CopyBoard(w,c);
	CopyBoard(a,c);
	CopyBoard(s,c);
	CopyBoard(d,c);
	UpdateBoard(w,'w');
	UpdateBoard(a,'a');
	UpdateBoard(s,'s');
	UpdateBoard(d,'d');
	float Max1,Max2,Max3,Max4;
	Max1= smoothweight*Smoothness(w)+monotonicity(w)+emptyweight*Emptycells(w)+maxvalue(w);
	Max2= smoothweight*Smoothness(a)+monotonicity(a)+emptyweight*Emptycells(a)+maxvalue(a);
	Max3= smoothweight*Smoothness(s)+monotonicity(s)+emptyweight*Emptycells(s)+maxvalue(s);
	Max4= smoothweight*Smoothness(d)+monotonicity(d)+emptyweight*Emptycells(d)+maxvalue(d);
//	printf("Max1 is %f\n",Max1);
//	printf("Max2 is %f\n",Max2);
//	printf("Max3 is %f\n",Max3);
//	printf("Max4 is %f\n",Max4);

	return maximum(Max1,Max2,Max3,Max4);
}
float sscore(int b[4][4]){

	int c[4][4];
	float smoothweight=0.1,emptyweight=2.7;
	CopyBoard(c,b);
	UpdateBoard(c,'s');
	if(AreEqual(c,b))
		return -100;
	AddNewNumber(c);
	int w[4][4],a[4][4],s[4][4],d[4][4];
	CopyBoard(w,c);
	CopyBoard(a,c);
	CopyBoard(s,c);
	CopyBoard(d,c);
	UpdateBoard(w,'w');
	UpdateBoard(a,'a');
	UpdateBoard(s,'s');
	UpdateBoard(d,'d');
	float Max1,Max2,Max3,Max4;
	Max1= smoothweight*Smoothness(w)+monotonicity(w)+emptyweight*Emptycells(w)+maxvalue(w);
	Max2= smoothweight*Smoothness(a)+monotonicity(a)+emptyweight*Emptycells(a)+maxvalue(a);
	Max3= smoothweight*Smoothness(s)+monotonicity(s)+emptyweight*Emptycells(s)+maxvalue(s);
	Max4= smoothweight*Smoothness(d)+monotonicity(d)+emptyweight*Emptycells(d)+maxvalue(d);
//	printf("Max1 is %f\n",Max1);
//	printf("Max2 is %f\n",Max2);
//	printf("Max3 is %f\n",Max3);
//	printf("Max4 is %f\n",Max4);

	return maximum(Max1,Max2,Max3,Max4);

}

float ascore(int b[4][4]){

	int c[4][4];
	float smoothweight=0.1,emptyweight=2.7;
	CopyBoard(c,b);
	UpdateBoard(c,'a');
	if(AreEqual(c,b))
		return -100;
	AddNewNumber(c);
	int w[4][4],a[4][4],s[4][4],d[4][4];
	CopyBoard(w,c);
	CopyBoard(a,c);
	CopyBoard(s,c);
	CopyBoard(d,c);
	UpdateBoard(w,'w');
	UpdateBoard(a,'a');
	UpdateBoard(s,'s');
	UpdateBoard(d,'d');
	float Max1,Max2,Max3,Max4;
	Max1= smoothweight*Smoothness(w)+monotonicity(w)+emptyweight*Emptycells(w)+maxvalue(w);
	Max2= smoothweight*Smoothness(a)+monotonicity(a)+emptyweight*Emptycells(a)+maxvalue(a);
	Max3= smoothweight*Smoothness(s)+monotonicity(s)+emptyweight*Emptycells(s)+maxvalue(s);
	Max4= smoothweight*Smoothness(d)+monotonicity(d)+emptyweight*Emptycells(d)+maxvalue(d);
//	printf("Max1 is %f\n",Max1);
//	printf("Max2 is %f\n",Max2);
//	printf("Max3 is %f\n",Max3);
//	printf("Max4 is %f\n",Max4);

	return maximum(Max1,Max2,Max3,Max4);

}


float dscore(int b[4][4]){

	int c[4][4];
	float smoothweight=0.1,emptyweight=2.7;
	CopyBoard(c,b);
	UpdateBoard(c,'d');
	if(AreEqual(c,b))
		return -100;
	AddNewNumber(c);
	int w[4][4],a[4][4],s[4][4],d[4][4];
	CopyBoard(w,c);
	CopyBoard(a,c);
	CopyBoard(s,c);
	CopyBoard(d,c);
	UpdateBoard(w,'w');
	UpdateBoard(a,'a');
	UpdateBoard(s,'s');
	UpdateBoard(d,'d');
	float Max1,Max2,Max3,Max4;
	Max1= smoothweight*Smoothness(w)+monotonicity(w)+emptyweight*Emptycells(w)+maxvalue(w);
	Max2= smoothweight*Smoothness(a)+monotonicity(a)+emptyweight*Emptycells(a)+maxvalue(a);
	Max3= smoothweight*Smoothness(s)+monotonicity(s)+emptyweight*Emptycells(s)+maxvalue(s);
	Max4= smoothweight*Smoothness(d)+monotonicity(d)+emptyweight*Emptycells(d)+maxvalue(d);
//	printf("Max1 is %f\n",Max1);
//	printf("Max2 is %f\n",Max2);
//	printf("Max3 is %f\n",Max3);
//	printf("Max4 is %f\n",Max4);

	return maximum(Max1,Max2,Max3,Max4);

}


char AIGenNextMove(int b[4][4]) {
	char move = 'w'; //move = 'w', 's', 'a', or 'd'
	/*Your code goes here*/
	float scorew,scorea,scores,scored;
	int w[4][4],a[4][4],s[4][4],d[4][4];
	float smoothweight=0.1,emptyweight=2.7;

	CopyBoard(w,b);
	UpdateBoard(w,'w');
	if(AreEqual(w,b)){
		scorew=-100;
	}
	else{
		AddNewNumber(w);
		scorew=maximum(wscore(w),ascore(w),sscore(w),dscore(w));
		//	scorew= smoothweight*Smoothness(w)+monotonicity(w)+emptyweight*Emptycells(w)+maxvalue(w);
	}

	CopyBoard(a,b);
	UpdateBoard(a,'a');
	if(AreEqual(a,b)){
		scorea=-100;
	}
	else{
		AddNewNumber(a);
		scorea=maximum(wscore(a),ascore(a),sscore(a),dscore(a));
		//	scorea= smoothweight*Smoothness(a)+monotonicity(a)+emptyweight*Emptycells(a)+maxvalue(a);
	}

	CopyBoard(s,b);
	UpdateBoard(s,'s');
	if(AreEqual(s,b)){
		scores=-100;
	}
	else{
		AddNewNumber(s);
		scores=maximum(wscore(s),ascore(s),sscore(s),dscore(s));
		//	scores= smoothweight*Smoothness(s)+monotonicity(s)+emptyweight*Emptycells(s)+maxvalue(s);
	}
	CopyBoard(d,b);
	UpdateBoard(d,'d');
	if(AreEqual(d,b)){
		scored=-100;
	}
	else{
		AddNewNumber(d);
		scored=maximum(wscore(d),ascore(d),sscore(d),dscore(d));
		//	scored= smoothweight*Smoothness(d)+monotonicity(d)+emptyweight*Emptycells(d)+maxvalue(d);
	}
//	printf("scorew = %f\n",scorew);
//	printf("scorea = %f\n",scorea);
//	printf("scores = %f\n",scores);
//	printf("scored = %f\n",scored);

	if(scorew>=scorea&&scorew>=scores&&scorew>=scored){
		return 'w';
	}
	if(scorea>=scorew&&scorea>=scores&&scorea>=scored){
		return 'a';
	}
	if(scores>=scorew&&scores>=scorea&&scores>=scored){
		return 's';
	}
	if(scored>=scorew&&scored>=scorea&&scored>=scores){
		return 'd';
	}
	if(scorew==scorea&&scorew==scores&&scorew==scored){
		return 'w';
	}

}

int main() {
	int board[4][4];
	int oldBoard[4][4];
	srand(time(NULL));
	InitBoard(board);
	PrintBoard(board);

	for (;;) {
		char c;
		CopyBoard(oldBoard, board);

		if (!_2048AI) {
			c = getchar();
			if (c != 'w' && c != 's' && c != 'a' && c != 'd') {
				printf("\n\n please press w, s, a, or d\n");
				continue;
			}
		}
		else {
			c = AIGenNextMove(oldBoard);
		}

		UpdateBoard(board, c);

		if (!AreEqual(board, oldBoard)) {
			AddNewNumber(board);

			// I use system("cls") to clear screen. Change the code according to your OS.*/
			system("cls");
			// I use system("cls") to clear screen. Change the code according to your OS.*/

			PrintGame(oldBoard, board, c);
		}

		if (IsGameOver(board)) {
			printf("\n\n Game Over!!\n\n");
			getchar();
		}
	}

	return 0;
}

