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
#include "boarditem.h"
#include "boardmodel.h"
#include "pieceitem.h"
#include "leveldata.h"
#include <QCoreApplication>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsSvgItem>
#include <QPixmap>
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include <QMouseEvent>
#include <QDebug>
#include <QFile>

extern const int PieceNum = 0;
static const int Sensitivity = 5;

BoardItem::BoardItem( LevelData *data, BoardModel *model, QGraphicsItem *parent )
		: QObject( 0 ), QGraphicsItem( parent )
{
	track = FALSE;
	//this->leveldata=data;

	this->model = model;
	pieces = data->pieces();
	sx = ( *pieces )[0]->boundingRect().width() - SHADOW_X;
	sy = ( *pieces )[0]->boundingRect().height() - SHADOW_Y;

	pieceanimations.resize( pieces->size() );
	piecetlines.resize( pieces->size() );

	for ( int i = 0;i < pieces->size();i++ ) {
		pieceanimations[i] = new QGraphicsItemAnimation();
		piecetlines[i] = new QTimeLine( 500 );
		pieceanimations[i]->setTimeLine( piecetlines[i] );
	}
}


QRectF BoardItem::boundingRect() const
{
	//qDebug()<< sx << "*" << model->placeColumns() << sy << "*"<< model->placeRows()<< "returned as boundingRect" ;
	return QRectF( 0, 0, sx*model->placeColumns(), sy*model->placeRows() );
}

void BoardItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	Q_UNUSED( painter );
	Q_UNUSED( option );
	Q_UNUSED( widget );
//	painter->drawRect(0, 0, sx*model->placeColumns(), sy*model->placeRows());
}

void BoardItem::movePiece( QGraphicsItem *item, uint x, uint y )
{
	qDebug() << "moving item" <<  item->data( PieceNum ).toInt() << "to" << x*sx << "," << y*sy;
	pieceanimations[item->data( PieceNum ).toInt()]->setItem( item );
	pieceanimations[item->data( PieceNum ).toInt()]->setPosAt( 1, QPointF( x*sx, y*sy ) );
	piecetlines[item->data( PieceNum ).toInt()]->start();
}

void BoardItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
	QList<QGraphicsItem *>  items = scene()->items( event->scenePos() );
	bool ok = false;

	if ( items.isEmpty() ) {
		qDebug() << "You didn't click on an item.";
		return;
	}

	qDebug() << "there are" << items.size() << "items at pos" << event->scenePos();

	for ( int i = 0; i < items.size(); ++i ) {
		int l = ( items.at( i ) )->data( PieceNum ).toInt( &ok );

		if ( ok ) {
			lastpos = event->scenePos();
			lastitem = l;
			track = true;
			qDebug() << "You clicked on item" << lastitem << "number" << i << "in the position children items";
			return;
		}
	}

	qDebug() << "No PieceItems.";

	return;
}


void BoardItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
	Q_UNUSED( event );
	track = FALSE;
}

void BoardItem::mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event )
{
	qDebug() << "Got MouseDoubleClick on BoardItem";

	uint piecenum;

	if ( model->insertPiece( piecenum ) ) {
		qDebug() << "Piece" << piecenum << "inserted";
		( *pieces )[piecenum]->setZValue( 100 );
		( *pieces )[piecenum]->setPos( 0, 0 );
		( *pieces )[piecenum]->setVisible( true );
		update();
		QCoreApplication::processEvents();
		movePiece(( *pieces )[piecenum], model->column( piecenum ), model->row( piecenum ) );
	}

	QGraphicsItem::mouseDoubleClickEvent( event );
}

void BoardItem::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
	if ( !track ) {
		return;
	}

	xdiff = event->scenePos().x() - lastpos.x();

	ydiff = event->scenePos().y() - lastpos.y();

	if ( xdiff > Sensitivity ) {
		qDebug() << "Move " << lastitem << "right.";
		model->movePiece( lastitem, RIGHT );
		movePiece(( *pieces )[lastitem], model->column( lastitem ), model->row( lastitem ) );
		track = FALSE;
	}

	if ( xdiff < -Sensitivity ) {
		qDebug() << "Move " << lastitem << "left.";
		model->movePiece( lastitem, LEFT );

		if ( model->inBoard( lastitem ) ) {
			movePiece(( *pieces )[lastitem], model->column( lastitem ), model->row( lastitem ) );
		} else {
			movePiece(( *pieces )[lastitem], 0, 0 );
			// wait a bit
			track = false; // do it now or the mouse will screw us...
			update();
			QCoreApplication::processEvents();
			update();
			QCoreApplication::processEvents();
			update();
			QCoreApplication::processEvents();
			( *pieces )[lastitem]->setVisible( false );
		}

		track = FALSE;
	}

	if ( ydiff < -Sensitivity ) {
		qDebug() << "Move " << lastitem << "up.";
		model->movePiece( lastitem, UP );
		movePiece(( *pieces )[lastitem], model->column( lastitem ), model->row( lastitem ) );
		track = FALSE;
	}

	if ( ydiff > Sensitivity ) {
		qDebug() << "Move " << lastitem << "down.";
		model->movePiece( lastitem, DOWN );
		movePiece(( *pieces )[lastitem], model->column( lastitem ), model->row( lastitem ) );
		track = FALSE;
	}

	model->isComplete();
}
