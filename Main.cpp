
#include "Game.h"
#include "agent.h"
#ifndef LINUX
#include <windows.h>
#include <cstdlib>


#endif


int NasRand(int pA, int pB)
{
	return rand() % (pB - pA + 1) + pA;
}


#ifndef LINUX
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)	// Linux users should quit this line
#else
int main()
#endif

{

	IO mIO;
	int mScreenHeight = mIO.GetScreenHeight();
	int score = 0;
	
	Pieces mPieces;
	Board mBoard(&mPieces, mScreenHeight);
	Game mGame(&mBoard, &mPieces, &mIO, mScreenHeight);

	// ----- Pocetak igre -----

	while (1){
		mIO.ClearScreen(); 		
		mGame.DrawScene();		
		mIO.UpdateScreen();		

		pair<int, int> agentResult;
		agentResult = agent(mBoard, mGame.mPiece, mGame.mNextPiece);

		int possiblePos = agentResult.first;   
		int possbileRot = agentResult.second;

		mGame.mPosX = possiblePos;
		mGame.mRotation = possbileRot;
		
		while (mBoard.IsPossibleMovement(mGame.mPosX, mGame.mPosY, mGame.mPiece, mGame.mRotation)) {			
			mIO.ClearScreen(); 		
			mGame.DrawScene();			
			mIO.UpdateScreen();
			mGame.mPosY++;
		}
		
		mBoard.StorePiece(mGame.mPosX, mGame.mPosY-1, mGame.mPiece, mGame.mRotation);
		int temp;
		temp = mBoard.DeletePossibleLines();
		score += temp;
		mGame.Score += temp;

		if (mBoard.IsGameOver()){			
			mIO.Getkey();
			exit(0);
		}
		mGame.CreateNewPiece();	
	}
	return 0;
}
