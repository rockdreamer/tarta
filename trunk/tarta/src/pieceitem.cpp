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
 
#include "pieceitem.h"
#include <QDebug>
#include <QGraphicsSvgItem>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneHoverEvent>
#include <QColor>
#include <QBrush>
#include <QPainter>
#include <QPen>
#include "math.h"

PieceItem::PieceItem(const QPixmap& pixmap, QGraphicsItem *target, QGraphicsItem *parent) : QGraphicsItem(parent)
{
	Q_UNUSED(parent);
	setAcceptsHoverEvents ( true );
	setHandlesChildEvents(true);
	pix=new QGraphicsPixmapItem(pixmap, this);
	pix->setZValue(500);
	this->target=target;
	qreal scalevalue=qMin( 
		(qreal)pix->boundingRect().width()/(qreal)target->boundingRect().width() ,
		(qreal)pix->boundingRect().height()/(qreal)target->boundingRect().height());
	tpos = QPointF(
	(pix->boundingRect().width()  - (target->boundingRect().width() *scalevalue))/2, 
	(pix->boundingRect().height() - (target->boundingRect().height()*scalevalue))/2 );
	QGraphicsRectItem *shadow = new QGraphicsRectItem(pix->boundingRect(), this);
	QColor sh=Qt::lightGray;
	sh.setAlpha(50);
	QBrush b(sh);
	shadow->setBrush(b);
	shadow->setPen(Qt::NoPen);
	shadow->setPos(10,10);
	shadow->setZValue(0);
	
}

QRectF PieceItem::boundingRect() const
{
	//qDebug() << "returned" << pix->boundingRect() << "as piece boundingrect";
	return pix->boundingRect();
}

void PieceItem::paint(QPainter *painter,
                   const QStyleOptionGraphicsItem *option, QWidget *widget)
{
     Q_UNUSED(painter);
     Q_UNUSED(option);
     Q_UNUSED(widget);
	 //painter->drawRect(0, 0, 213, 80);
}

void PieceItem::hoverEnterEvent ( QGraphicsSceneHoverEvent * event ){
	target->setParentItem(this);	
	target->setPos( tpos );
	target->setVisible(true);
	qDebug() << "Mouse entered piece" << data(0);
	QGraphicsItem::hoverEnterEvent(event);
}

void PieceItem::hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ){
	target->setVisible(false);
	qDebug() << "Mouse left piece" << data(0);
	QGraphicsItem::hoverLeaveEvent(event);
}
