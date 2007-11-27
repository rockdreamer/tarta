/*
 *  boardmodel.cpp
 *  tart
 *
 *  Created by Claudio on 08/11/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

//#define NDEBUG

#include "boardmodel.h"
#include <iostream>
#include <cassert>
#include <QDebug>
#include <QDateTime>
#include <QList>
#include <math.h>

using namespace std;

BoardModel::BoardModel(uint piecerows, uint piececolumns, uint placerows, uint placecolumns)
{
	assert(piecerows&&piececolumns&&placerows&&placecolumns);

	// Initialise board numeric values
	m_rows=piecerows;
	m_cols=piececolumns;
	m_placerows=placerows;
	m_placecols=placecolumns;
	m_pieces=piecerows*piececolumns;	// this shows how many pieces the picture is built of
	m_places=placerows*placecolumns;	// this shows how many free spaces the 
										// board has in the beginning
	m_inboard=0; // this shows how many pieces have been inserted in the board currently

	// brd values can range between 0 and m_pieces
	// m_pieces means there is no piece in that position
	for (uint i=0; i<(m_places);i++){
		brd.append(m_pieces);
	}

	// positions values range between 0 and m_places
	// m_places means a piece is not on the board
	for (uint i=0; i<m_pieces;i++){
		positions.append(m_places);
	}

	// randomise the order pieces are presented to the user
	initRandArray();
	
	qDebug() << "Created Board model" << piecerows << piececolumns << placerows << placecolumns;
}

BoardModel::~BoardModel()
{
}

uint BoardModel::pieces()
{
	return m_pieces;
}

uint BoardModel::pieceColumns()
{
	return m_cols;
}

uint BoardModel::pieceRows()
{
	return m_rows;
}

uint BoardModel::placeColumns()
{
	return m_placecols;
}

uint BoardModel::placeRows()
{
	return m_placerows;
}

bool BoardModel::inBoard(uint piece)
{
	return positions[piece]!=m_places;
}

uint BoardModel::pieceAt(uint row,uint column)
{
	return brd[(row*m_placecols)+column];
}

uint BoardModel::row(uint piece)
{
	assert (piece<m_pieces);

	return positions[piece]/m_placecols;
}

uint BoardModel::column(uint piece)
{
	assert (piece<m_pieces);

	return positions[piece]%m_placecols;
}

bool BoardModel::insertPiece(uint& piece)
{
	if (brd[0]!=m_pieces) return false; //there was another piece in (0,0)
	if (m_inboard==m_pieces) return false; //all pieces are in!

	// Side effect!
	piece=randpieces.takeFirst();
	brd[0]=piece;
	positions[piece]=0;
	movePiece(piece,RIGHT);
	m_inboard++;
	return true;
}

bool BoardModel::removePiece(uint piece)
{
	// The piece is not in the board!!!
	if (positions[piece]==m_places) return false;

	// update the board
	brd[positions[piece]]=m_pieces;
	// update piece position
	positions[piece]=m_places;

	// find a random piece in the random pieces array
	uint r=rand();
	float w=float(r)/RAND_MAX;
	r= floor((randpieces.size())*w);

	//put the piece at the end of the array
	randpieces.append(piece);
	//and swap it with the other random piece
	randpieces.swap(r,randpieces.size());
	
	m_inboard--;
	qDebug() << "Removed piece" << piece << "from board";
	return true;
}

void BoardModel::movePiece(uint piece, Direction dir)
{

	assert(piece<m_pieces);
	uint lastpos=0;
	uint newpos=positions[piece]; // set to current position

	switch (dir) {
	case RIGHT:
		lastpos=((row(piece)+1)*m_placecols)-1; // end of row
		while (newpos<lastpos){
			if (brd[newpos+1]==m_pieces){ //next cell is empty, update position
				newpos++;
			} else {
				break;
			}
		}
		break;
	case LEFT:
		if (positions[piece]==0) { // special case... while below doesn't check it...
			removePiece(piece); 
			return;
		}
		lastpos=row(piece)*m_placecols; // start of row
		while (newpos>lastpos){
			if (brd[newpos-1]==m_pieces){ //next cell is empty, update position
				newpos--;
				if (newpos==0){
					// This is a special case, we let the piece out
					removePiece(piece);
					return;
				}
			} else {
				break;
			}
		}
		break;
	case UP:
		lastpos=column(piece); // start of column
		while (newpos>lastpos){
			if (brd[newpos-m_placecols]==m_pieces){ //cell above is empty, update position
				newpos=newpos-m_placecols;
			} else {
				break;
			}
		}
		break;
	case DOWN:
		lastpos=((m_placerows-1)*m_placecols)+column(piece); // start of column
		while (newpos<lastpos){
			if (brd[newpos+m_placecols]==m_pieces){ //cell below is empty, update position
				newpos=newpos+m_placecols;
			} else {
				break;
			}
		}
		break;
	}
	
	brd[positions[piece]]=m_pieces; //clear the old value in the board
	brd[newpos]=piece; // set the new value in the board
	positions[piece]=newpos; // update position array
}

void BoardModel::initRandArray()
{
	uint i,r,first,second;
	float w;
	
	m_inboard=0;
	// Init randpieces vector
	for (i=0;i<m_pieces;i++){
		randpieces.append(i);
	}
		
	// Exchange randpieces items randomly
	for (i=0; i < m_pieces*2; i++){
		r=rand();
		w=float(r)/RAND_MAX;
		first = floor(m_pieces*w);
		r=rand();
		float w=float(r)/RAND_MAX;
		second = floor(m_pieces*w);
		qDebug() << "Exchanging" << first << "and" << second;
		randpieces.swap(first,second);
	}

	for (i=0;i<m_pieces;i++)
		qDebug() << randpieces[i];
}
