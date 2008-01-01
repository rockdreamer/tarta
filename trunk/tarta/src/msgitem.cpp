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
#include "msgitem.h"
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
#include <QFont>
#include "math.h"

MsgItem::MsgItem( QString text, QGraphicsItem *parent ) : QGraphicsItem( parent )
{
	txt = new QGraphicsTextItem( this );
	txt->setPlainText( text );
	txt->setFont( QFont( "Helvetica", 28, QFont::Bold ) );
	txt->setPos( 5, 5 );
	boundrect.setX( 0 );
	boundrect.setY( 0 );
	boundrect.setWidth( txt->boundingRect().width() + 10 );
	boundrect.setHeight( txt->boundingRect().height() + 10 );
}

QRectF MsgItem::boundingRect() const
{
	return boundrect;
}

void MsgItem::paint( QPainter *painter,
                     const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	Q_UNUSED( option );
	Q_UNUSED( widget );
	QColor bg = Qt::cyan;
	bg.setAlpha( 70 );
	QBrush b( bg );
	painter->setBrush( b );
	painter->setPen( Qt::black );
	painter->drawRoundRect( 0, 0, boundrect.width(), boundrect.height(), 5, 5 );
}

