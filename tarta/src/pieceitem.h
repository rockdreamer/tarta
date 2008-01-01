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

#ifndef PIECEITEM_H
#define PIECEITEM_H

#include <QGraphicsItem>

class QGraphicsSvgItem;
class QPixmap;

class PieceItem: public QGraphicsItem
{
public:
	PieceItem( const QPixmap& pixmap, QGraphicsItem *target, QGraphicsItem *parent = 0 );
	QRectF boundingRect() const;
	void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0 );
	QPainterPath shape() const;

protected:
	void hoverEnterEvent( QGraphicsSceneHoverEvent * event );
	void hoverLeaveEvent( QGraphicsSceneHoverEvent * event );

private:
	QGraphicsItem *target;
	QGraphicsPixmapItem *pix;
	QPointF tpos;
	QRectF boundrect;
	QPainterPath path;
};


#endif
