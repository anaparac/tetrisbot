/*****************************************************************************************
/* File: Game.cpp
/* Desc: General class for the game
/*
/* gametuto.com - Javier López López (javilop.com)
/*
/*****************************************************************************************
/*
/* Creative Commons - Attribution 3.0 Unported
/* You are free:
/*	to Share — to copy, distribute and transmit the work
/*	to Remix — to adapt the work
/*
/* Under the following conditions:
/* Attribution. You must attribute the work in the manner specified by the author or licensor 
/* (but not in any way that suggests that they endorse you or your use of the work).
/*
/*****************************************************************************************/

// ------ Includes -----
#ifndef LINUX
#include <cstdlib>
#include <Windows.h>
#endif
#include "Game.h"

/* 
======================================									
Init
====================================== 
*/
Game::Game(Board *pBoard, Pieces *pPieces, IO *pIO, int pScreenHeight) 
{
	mScreenHeight = pScreenHeight;

	// Get the pointer to the Board and Pieces classes
	mBoard = pBoard;
	mPieces = pPieces;
	mIO = pIO;

	// Game initialization
	InitGame ();
}


/* 
======================================									
Get a random int between to integers

Parameters:
>> pA: First number
>> pB: Second number
====================================== 
*/
int Game::GetRand (int pA, int pB)
{
	return rand () % (pB - pA + 1) + pA;
}


/* 
======================================									
Initial parameters of the game
====================================== 
*/
void Game::InitGame()
{
	// Init random numbers
	srand ((unsigned int) time(NULL));

	// First piece
	mPiece			= GetRand (0, 6);
	mRotation		= 0;
	mPosX 			= (BOARD_WIDTH / 2) + mPieces->GetXInitialPosition (mPiece, mRotation);
	mPosY 			= mPieces->GetYInitialPosition (mPiece, mRotation);
	Score			= 0;

	//  Next piece
	mNextPiece 		= GetRand (0, 6);
	mNextRotation 	= 0;
	mNextPosX 		= BOARD_WIDTH + 5;
	mNextPosY 		= 5;	
}


/* 
======================================									
Create a random piece
====================================== 
*/
void Game::CreateNewPiece()
{
	// The new piece
	mPiece			= mNextPiece;
	mRotation		= mNextRotation;
	mPosX 			= (BOARD_WIDTH / 2) + mPieces->GetXInitialPosition (mPiece, mRotation);
	mPosY 			= mPieces->GetYInitialPosition (mPiece, mRotation);

	// Random next piece
	mNextPiece 		= GetRand (0, 6);
	mNextRotation 	= 0;
}


/* 
======================================									
Draw piece

Parameters:

>> pX:		Horizontal position in blocks
>> pY:		Vertical position in blocks
>> pPiece:	Piece to draw
>> pRotation:	1 of the 4 possible rotations
====================================== 
*/
void Game::DrawPiece (int pX, int pY, int pPiece, int pRotation)
{
	color mColor;				// Color of the block 

	// Obtain the position in pixel in the screen of the block we want to draw
	int mPixelsX = mBoard->GetXPosInPixels (pX);
	int mPixelsY = mBoard->GetYPosInPixels (pY);

	// Travel the matrix of blocks of the piece and draw the blocks that are filled
	for (int i = 0; i < PIECE_BLOCKS; i++)
	{
		for (int j = 0; j < PIECE_BLOCKS; j++)
		{
			// Get the type of the block and draw it with the correct color
			switch (mPieces->GetBlockType (pPiece, pRotation, j, i))
			{
				case 1: mColor = WHITE; break;	// For each block of the piece except the pivot
				case 2: mColor = WHITE; break;	// For the pivot
			}
			
			if (mPieces->GetBlockType (pPiece, pRotation, j, i) != 0)
				mIO->DrawRectangle	(mPixelsX + i * BLOCK_SIZE, 
									mPixelsY + j * BLOCK_SIZE, 
									(mPixelsX + i * BLOCK_SIZE) + BLOCK_SIZE - 1, 
									(mPixelsY + j * BLOCK_SIZE) + BLOCK_SIZE - 1, 
									mColor);
		}
	}
}


/* 
======================================									
Draw board

Draw the two lines that delimit the board
====================================== 
*/
void Game::DrawBoard ()
{
	// Calculate the limits of the board in pixels	
	int mX1 = BOARD_POSITION - (BLOCK_SIZE * (BOARD_WIDTH / 2)) - 1;
	int mX2 = BOARD_POSITION + (BLOCK_SIZE * (BOARD_WIDTH / 2));
	int mY = mScreenHeight - (BLOCK_SIZE * BOARD_HEIGHT);
	
	// Check that the vertical margin is not to small
	//assert (mY > MIN_VERTICAL_MARGIN);

	// Rectangles that delimits the board
	mIO->DrawRectangle (mX1 - BOARD_LINE_WIDTH, mY, mX1, mScreenHeight - 1, BLACK);
	mIO->DrawRectangle (mX2, mY, mX2 + BOARD_LINE_WIDTH, mScreenHeight - 1, BLACK);
	
	// Check that the horizontal margin is not to small
	//assert (mX1 > MIN_HORIZONTAL_MARGIN);

	// Drawing the blocks that are already stored in the board
	mX1 += 1;
	for (int i = 0; i < BOARD_WIDTH; i++)
	{
		for (int j = 0; j < BOARD_HEIGHT; j++)
		{	
			// Check if the block is filled, if so, draw it
			if (!mBoard->IsFreeBlock(i, j))	
				mIO->DrawRectangle (	mX1 + i * BLOCK_SIZE, 
										mY + j * BLOCK_SIZE, 
										(mX1 + i * BLOCK_SIZE) + BLOCK_SIZE - 1, 
										(mY + j * BLOCK_SIZE) + BLOCK_SIZE - 1, 
										GRAY);
		}
	}	
}



void Game::DrawScore(int pX, int pY, int score_, enum color pC) {
	std::stringstream strs;
	strs << score_;
	std::string temp_str = strs.str();
	char* score_string = (char*)temp_str.c_str();


	int mPixelsX_ = mBoard->GetXPosInPixels(pX - 1);
	int mPixelsY_ = mBoard->GetYPosInPixels(pY - 2);


	int mPixelsX = mBoard->GetXPosInPixels(pX);
	int mPixelsY = mBoard->GetYPosInPixels(pY);

	char naslov[] = { 'S','C','O','R','E',':','\0' };

	mIO->DrawString(mPixelsX_, mPixelsY_, naslov, pC);

	mIO->DrawString(mPixelsX, mPixelsY, score_string, pC);

	mIO->DrawClearRectangle(mPixelsX_ - 20, mPixelsY_ - 15, mPixelsX_ + 70, mPixelsY_ + 60, BLACK);

}

/* 
======================================									
Draw scene

Draw all the objects of the scene
====================================== 
*/
void Game::DrawScene ()
{
	char ispis[] = "NEXT:";
	DrawBoard ();													// Draw the delimitation lines and blocks stored in the board
	DrawPiece (mPosX, mPosY, mPiece, mRotation);					// Draw the playing piece
	DrawPiece (mNextPosX, mNextPosY, mNextPiece, mNextRotation);	// Draw the next piece
	
	int mPixelsX_ = mBoard->GetXPosInPixels(mNextPosX );
	int mPixelsY_ = mBoard->GetYPosInPixels(mNextPosY );

	mIO->DrawString(mPixelsX_ + 15, mPixelsY_ - 5, ispis, BLACK);
	mIO->DrawClearRectangle(mPixelsX_- 5, mPixelsY_- 20, mPixelsX_ + 85, mPixelsY_ + 82, BLACK);
	
	DrawScore(mNextPosX + 2, mNextPosY + 10, Score, BLACK);
}
