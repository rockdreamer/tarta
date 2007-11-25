/**
 *   Copyright (C) 2007 by Claudio Bantaloukas
 *   rockdreamer@gmail.com
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the
 *   Free Software Foundation, Inc., 
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 **/


#ifndef BOARDMODEL_H
#define BOARDMODEL_H

typedef unsigned int uint;

enum Direction {UP, DOWN, LEFT, RIGHT};

class BoardModel{

public:
	BoardModel(uint piecerows=5, uint piececolumns=8, uint placerows=7, uint placecols=8);
	~BoardModel();
	uint pieces();
	uint pieceColumns();
	uint pieceRows();
	uint placeColumns();
	uint placeRows();
	uint row(uint piece);
	uint column(uint piece);
	uint pieceAt(uint row,uint column);
	bool insertPiece(uint& piece);
	bool isComplete();
	void movePiece(uint piece, Direction dir);
private:
	void initRandArray();
	uint *randpieces;
	uint *brd;
	uint *positions;
	uint m_rows,m_cols,m_pieces,m_places,m_placerows,m_placecols,m_inboard;
};

#endif