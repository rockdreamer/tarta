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
 
#ifndef BOARDITEM_H
#define BOARDITEM_H

#include <QGraphicsItem>

class QGraphicsPixmapItem;
class QMouseEvent;
class QResizeEvent;
class QPoint;
class BoardModel;
class QGraphicsItemAnimation;
class QTimeLine;
class PieceItem;
class QGraphicsSvgItem;

class BoardItem: public QGraphicsItem
{

public:
    BoardItem(const QString& levelname, QGraphicsItem *parent=0);
	void loadData();
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

protected:
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	
private:
	void movePiece(QGraphicsItem *item, uint x, uint y);

	QString levelname;
    QVector<PieceItem*> pieces;
    QPointF lastpos;
	QGraphicsItemAnimation *animation;
	QTimeLine *timer;
	BoardModel *model;
	int xdiff,ydiff, lastitem;
	uint sx,sy;
	bool track;	
	QGraphicsSvgItem *bg,*target;

};


#endif
