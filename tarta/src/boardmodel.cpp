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
#include <math.h>

using namespace std;

BoardModel::BoardModel(uint piecerows, uint piececolumns, uint placerows, uint placecolumns)
{
	assert(piecerows&&piececolumns&&placerows&&placecolumns);
	
	brd=NULL;
	positions=NULL;
	m_rows=piecerows;
	m_cols=piececolumns;
	m_placerows=placerows;
	m_placecols=placecolumns;
	m_pieces=piecerows*piececolumns;
	m_places=placerows*placecolumns;
	m_inboard=0;
	
	brd = new uint[m_places];
	
	for (uint i=0; i<(m_places);i++){
		brd[i]=m_pieces;
	}
	
	positions = new uint[m_pieces];
	for (uint i=0; i<m_pieces;i++){
		positions[i]=m_places;
	}

	initRandArray();
}

BoardModel::~BoardModel()
{
	if (brd) delete [] brd;
	if (positions) delete [] positions;
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
	piece=randpieces[m_inboard];
	brd[0]=piece;
	positions[piece]=0;
	movePiece(piece,RIGHT);
	m_inboard++;
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
		lastpos=row(piece)*m_placecols; // start of row
		while (newpos>lastpos){
			if (brd[newpos-1]==m_pieces){ //next cell is empty, update position
				newpos--;
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
	uint i,t,r,first,second;
	float w;
	
	m_inboard=0;
	// Init randpieces vector
	randpieces=new uint[m_pieces];
	for (i=0;i<m_pieces;i++){
		randpieces[i]=i;
	}
	
	// Prime rand function
	srand(QDateTime::currentDateTime().toTime_t());
	
	// Exchange randpieces items randomly
	for (i=0; i < m_pieces*2; i++){
		r=rand();
		w=float(r)/RAND_MAX;
		first = floor(m_pieces*w);
		r=rand();
		float w=float(r)/RAND_MAX;
		second = floor(m_pieces*w);
		qDebug() << "Exchanging" << first << "and" << second;
		t=randpieces[first];
		randpieces[first]=randpieces[second];
		randpieces[second]=t;
	}

	for (i=0;i<m_pieces;i++)
		qDebug() << randpieces[i];
}
