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

#ifndef LEVELDATA_H
#define LEVELDATA_H

#include <QObject>
#include <QBrush>
#include "pieceitem.h"

class QRect;
class QGraphicsItem;
class QPixmap;

class LevelData: public QObject
{
	Q_OBJECT

public:
	LevelData( QObject* parent = 0 );
	bool isDataLoaded();
	QString levelName();
	QRect boardRect();
	QRect timeRect();
	QBrush bgBrush();
	QBrush timerBrush();
	QBrush lifeBrush();
	QPixmap *lifePixmap();
	QGraphicsItem *bgItem();
	QGraphicsItem *targetItem();
	QVector<PieceItem*> *pieces();
	int time();
	int livesAwarded();
	int pieceRows();
	int pieceColumns();
	int placeColumns();
	int placeRows();

public slots:
	void setBaseDir( const QString& baseDir );
	void loadData();

signals:
	void loading( int percent, const QString& description );
	void error( int code, const QString& description );
	void success();

private:
	bool parseDefaults();
	bool parseLevel();
	bool loadBoardPix();
	bool loadTargetPix();
	bool loadBackgroundPix();
	bool process_line( QString line, bool defaults );
	bool handle_version( QString line );
	bool m_isdataloaded;
	QString m_level;
	QRect m_boardrect, m_timerect;
	QBrush m_bgbrush, m_timerbrush, m_lifebrush;
	QPixmap *m_lifepixmap;
	QGraphicsItem *m_levelbackground;
	QGraphicsItem *m_targetitem;
	QVector<PieceItem*> *m_pieces;
	int m_time, m_livesawarded;
	int m_pieceRows, m_pieceColumns, m_placeColumns, m_placeRows;
	int linenum;
	QString m_targetfile, m_bgfile, m_picfile, m_basedir;
};


#endif
