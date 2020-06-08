#include "agent.h"
#include "IO.h"
#include <cstdlib>
#include "windows.h"
#define CHECKED 3


// TODO: kasnije
int number_of_holes(Board board_){

	int holes = 0;
	int temp;
	// krecemo se po ploci od donjeg ljevog ugla i trazimo rupe
	for ( int i = 0; i < 10; i++ ) {
		
		for ( int j = 19; j > -1; j-- ) {
			int k = j;
			temp = 0;
			while (board_.IsFreeBlock(i, k) && (k != -1)) {
				board_.mBoard[i][k] = CHECKED;
				temp++;
				--k;
			}
			if (k != -1)
				holes += temp;
		}
	}
	return holes;
}


// TODO:
int clear_lines(Board board_) {
	int lines = 0;
	int free;

	for (int i = 19; i > -1; i--) {
		for (int j = 0; j < 10; j++) {
			//provjeravamo im li cijeli blok punih linija
			free = 0;
			if (board_.IsFreeBlock(j, i)) {// (( j,i))
				free = 1;
				break;
			}
		}
		if (!free)
			lines++;
	}
	return lines;
}

int sum_height(Board board_) {
	
	int height = 0;
	int j;

	for (int i = 0; i < 10; i++) {
		j = 0;
		while ( j < 20 ) {
			if (!board_.IsFreeBlock(i, j)) {
				break;
			}
			else 
				j++;
		}
		height += (20 - j);
	}
	return height;
}

int max_height(Board board_) {
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 10; j++)
			if (!board_.IsFreeBlock(j, i)) {
				return 20 - i;
			}
	}
	return 0;
}

int blockades(Board board_) {

	int blocks = 0;		//brojac za pune blokove iznad rupa

	for (int i = 19; i<-1; i--) {
		for (int j = 0; j>10; j++) {

			if (board_.IsFreeBlock(i, j)) {	//kada naide na rupu
				int k = i - 1;
				while ((!board_.IsFreeBlock(k, j)) && k>-1) {		//broji pune blokove dok ne dode do iduce rupe ili kraja ploce
					blocks++;
					k--;
				}
			}
		}
	}
	return blocks;
}

int bumpiness(Board board_) {
	int height1, height2;
	int j;
	int sum_bump = 0;

	j = 0;
	while (j < 20) {
		if (!board_.IsFreeBlock(0, j)) {
			break;
		}
		else
			j++;
	}
	height1 = (20 - j);

	for (int i = 1; i < 10; i++) {
		j = 0;
		while (j < 20) {
			if (!board_.IsFreeBlock(i, j)) {
				break;
			}
			else
				j++;
		}
		height2 = (20 - j);

		sum_bump += abs(height1 - height2);
		height1 = height2;
	}

	return sum_bump;
}



//#define HOLE -3
//#define LINE 7
//#define HEIGHT -1
//#define BLOCK -1
//#define COLUMN -2
//#define BUMP	-3

pair<int, int> agent(Board board_, int mPieces_, int mPieces_next) {
	double score;
	int scoreRotation = 0; 
	int scoreX = -2;
	int j;
	int rotation, rotation_next;


	double HOLE		= -9;
	double LINE		= 7;
	double HEIGHT	= -1;
	double BLOCK	= -1;
	double COLUMN	= -2;
	double BUMP		= -3;

	//double HOLE		= -8.74473;
	//double LINE		= 7;
	//double HEIGHT	= -1;
	//double BLOCK	= -1;
	//double COLUMN	= -2;
	//double BUMP		= -3;

	//testiranje s 20, 20 do 600 svaki 6 puta ( izvrsilo se samo do 15. iteracije)
	/*double BUMP = -9.06437;
	double LINE = 7;
	double BLOCK = -8.10619;
	double COLUMN = -0.751767;
	double HEIGHT = 0.0;
	double HOLE = -2.28029;*/
	
	//testiranje s 20, 20 do 600 svaki 6 puta ( izvrsilo se samo do 15. iteracije)
	//double BUMP = -8.63768;
	//double LINE = 7;
	//double BLOCK = -8.19046;
	//double COLUMN = -3.40762;
	//double HEIGHT = 0.0;
	//double HOLE = -1.19864;
	
	
	//testiranje s: 20, 15, do 600, svaki tetris 4 puta
	//double HOLE = -2.21893;
	//double LINE = 2.0;
	//double HEIGHT = 0.0;
	//double BLOCK = -4.05894;
	//double COLUMN = -1.58669;
	//double BUMP = -9.97876;

	/*testiranje s: 15, 15, do 350 svaki tetris jednom
	double HOLE = -0.629372;
	double LINE = 2.0;
	double HEIGHT = 0.0;
	double BLOCK = -8.90224;
	double COLUMN = -7.74115;
	double BUMP = -9.5425;*/

	//varijabla have sluzi kako bi prvi score zapamtili jer ga nemamo sa cime usporedivati
	int have = 0;
	double scoreNew;
	int j_next;
	for (rotation = 0; rotation < 4; rotation++) {
		for (int i = -2; i < 8; i++) {
			j = -3;
			if (!(board_.IsPossibleMovement(i, j, mPieces_, rotation))) continue;
			while (j <= 17) {
				if (!(board_.IsPossibleMovement(i, j, mPieces_, rotation))) {
					//stavljamo element na plocu da ga mozemo izracunati
					j--;
					board_.StorePiece(i, j, mPieces_, rotation);
					
					for (rotation_next = 0; rotation_next < 4; rotation_next++) {
						for (int i_next = -2; i_next < 8; i_next++) {
							j_next = -3;
							if (!(board_.IsPossibleMovement(i_next, j_next, mPieces_next, rotation_next))) continue;
							while (j_next <= 17) {
								if (!(board_.IsPossibleMovement(i_next, j_next, mPieces_next, rotation_next))) {
									//stavljamo element na plocu da ga mozemo izracunati
									j_next--;
									board_.StorePiece(i_next, j_next, mPieces_next, rotation_next);

									scoreNew = BUMP * bumpiness(board_) + LINE * clear_lines(board_) + BLOCK * blockades(board_) + COLUMN * max_height(board_) + HEIGHT * sum_height(board_) + HOLE * number_of_holes(board_);
									
									if (!have || scoreNew > score) {
										have = 1;
										//nasla sam bolji rezultat 
										//zamjeni rezultate pazi na promjenu i pozicije i rotacije
										score = scoreNew;
										scoreRotation = rotation;
										scoreX = i;
									}
									board_.DeletePiece(i_next, j_next, mPieces_next, rotation_next);
									break;
								}
								j_next++;
							}
						}
					}
					board_.DeletePiece(i, j, mPieces_, rotation);
					break;	
				}
				//moramo ga pomaknuti za jedan dolje kako bi hodali po ploci
				j++;
			}
		}
	}
	

	pair<int, int> returnValue(scoreX, scoreRotation);
	return returnValue;
}