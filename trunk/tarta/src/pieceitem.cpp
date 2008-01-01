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

#include "defines.h"
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

PieceItem::PieceItem( const QPixmap& pixmap, QGraphicsItem *target, QGraphicsItem *parent ) : QGraphicsItem( parent )
{
	Q_UNUSED( parent );

	setAcceptsHoverEvents( true );
	setHandlesChildEvents( true );

	pix = new QGraphicsPixmapItem( pixmap, this );
	pix->setZValue( 500 );
	this->target = target;
	qreal scalevalue = qMin(
	                       ( qreal ) pix->boundingRect().width() / ( qreal ) target->boundingRect().width() ,
	                       ( qreal ) pix->boundingRect().height() / ( qreal ) target->boundingRect().height() );

	tpos = QPointF(
	           ( pix->boundingRect().width()  - ( target->boundingRect().width() * scalevalue ) ) / 2,
	           ( pix->boundingRect().height() - ( target->boundingRect().height() * scalevalue ) ) / 2 );

	boundrect.setX( 0 );
	boundrect.setY( 0 );
	boundrect.setWidth( pix->boundingRect().width() + SHADOW_X );
	boundrect.setHeight( pix->boundingRect().height() + SHADOW_Y );

	path.addRect( pix->boundingRect() );
}

QPainterPath PieceItem::shape() const
{
	return path;
}

QRectF PieceItem::boundingRect() const
{
	return boundrect;
}

void PieceItem::paint( QPainter *painter,
                       const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	Q_UNUSED( painter );
	Q_UNUSED( option );
	Q_UNUSED( widget );
	QColor sh = Qt::gray;
	sh.setAlpha( 70 );
	QBrush b( sh );
	painter->setBrush( b );
	painter->setPen( Qt::NoPen );
	painter->drawRect( 0, 0, pix->boundingRect().width() + SHADOW_X, pix->boundingRect().height() + SHADOW_Y );
}

void PieceItem::hoverEnterEvent( QGraphicsSceneHoverEvent * event )
{
	target->setParentItem( this );
	target->setZValue( 2000 );
	target->setPos( tpos );
	target->setVisible( true );
	//qDebug() << "Mouse entered piece" << data(0);
	QGraphicsItem::hoverEnterEvent( event );
}

void PieceItem::hoverLeaveEvent( QGraphicsSceneHoverEvent * event )
{
	target->setVisible( false );
	//qDebug() << "Mouse left piece" << data(0);
	QGraphicsItem::hoverLeaveEvent( event );
}
