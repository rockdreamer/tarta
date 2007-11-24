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
 
#include "boarditem.h"
#include "boardmodel.h"
#include "pieceitem.h"
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

static const int PieceNum = 0;
static const int Sensitivity = 5;
 
BoardItem::BoardItem(const QString& levelname, QGraphicsItem *parent) 
	: QGraphicsItem(parent)
{
	track=FALSE;
	this->levelname=levelname;	
	animation = new QGraphicsItemAnimation();
	timer = new QTimeLine(300);
	animation->setTimeLine(timer);
	
	model = new BoardModel();
	loadData();
}


QRectF BoardItem::boundingRect() const
{
	//qDebug()<< sx << "*" << model->placeColumns() << sy << "*"<< model->placeRows()<< "returned as boundingRect" ;
	return QRectF(0,0,sx*model->placeColumns(), sy*model->placeRows());
}

void BoardItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(painter);
	Q_UNUSED(option);
	Q_UNUSED(widget);
//	painter->drawRect(0, 0, sx*model->placeColumns(), sy*model->placeRows());
}

void BoardItem::loadData() 
{
	QPixmap *wholepix=NULL;
	target = NULL;
	//Load the board image
	if ((!wholepix) && QFile::exists(":/levels/"+levelname+"/boardimg.png")){
		wholepix = new QPixmap(":/levels/"+levelname+"/boardimg.png");
		qDebug() << "Loaded board img from :/levels/"+levelname+"/boardimg.png";
		
	}
	if ((!wholepix) && QFile::exists(":/levels/"+levelname+"/boardimg.jpg")){
		wholepix = new QPixmap(":/levels/"+levelname+"/boardimg.jpg");
	}
	if ((!wholepix) && QFile::exists(":/levels/default/boardimg.png")){
		wholepix = new QPixmap(":/levels/default/boardimg.png");
	}
	
	if (!wholepix){
		qDebug() << "Couldn't load board img";
		return;
	}

	uint p= model->pieces();
	uint pc=model->pieceColumns();
	uint pr=model->pieceRows();
	sx = wholepix->width()/pc;
	sy = wholepix->height()/pr;
	
	qDebug() << "p" << p << "pc" << pc << "pr" << pr;
	qDebug() << "w" << wholepix->width() << "h" << wholepix->height()
			<< "sx" << sx << "sy" << sy;
			
	if (QFile::exists(":/levels/"+levelname+"/target.svg")){
		target = new QGraphicsSvgItem(":/levels/"+levelname+"/target.svg");
	} else {
		if (QFile::exists(":/levels/default/target.svg")){
			target = new QGraphicsSvgItem(":/levels/default/target.svg");
		}
	}
	
	if (!target) {
		qDebug() << "Could not load Target svg";
		return;
	}

	qreal scalevalue=qMin( 
		(qreal)sx/(qreal)target->boundingRect().width() ,
		(qreal)sy/(qreal)target->boundingRect().height());
	target->scale(scalevalue,scalevalue);
	target->setZValue(1000);
	target->setVisible(false);

	pieces.resize(p);	
	for (uint i=0;i<p;i++){
	
		PieceItem *item= 
			new PieceItem(wholepix->copy(
				(i%pc)*sx, (i/pc)*sy, sx, sy) , target, this);
		
		qDebug() << "Created pixmap from (" 
			<< (i%pc)*sx << "," << (i/pc)*sy << ","
			<< sx << "," << sy << ")";
		item->setPos((i%pc)*sx,(i/pc)*sy );
		pieces[i]=item;
		item->setZValue(100);
		item->setVisible(false);
		item->setData(PieceNum,i);
	}
	
	delete wholepix;
	wholepix=NULL;
}

void BoardItem::movePiece(QGraphicsItem *item, uint x, uint y){
	qDebug() << "moving item"<<  item->data(PieceNum).toInt() << "to" << x*sx << "," << y*sy;
	animation->setItem(item);
	animation->setPosAt(1, QPointF(x*sx,y*sy));
	timer->start();
}

void BoardItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
 {
     if (QGraphicsItem *item = scene()->itemAt(event->pos())) {
		if (QGraphicsItem *p=item->parentItem())
				item=p;
         qDebug() << "You clicked on item" << item->data(PieceNum);
		 lastpos=event->scenePos();
		 lastitem=item->data(PieceNum).toInt();
		 track=true;
//		 event->setAccepted(true);
     } else {
         qDebug() << "You didn't click on an item.";
//		 event->setAccepted(true);
     }
 }
 
 
void BoardItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	Q_UNUSED(event);
	track=FALSE;
//	event->setAccepted(true);
}

void BoardItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	qDebug() << "Got MouseDoubleClick on BoardItem";
	//Q_UNUSED(event);
	
	uint piecenum;
	if (model->insertPiece(piecenum)) {
		qDebug() << "Piece" << piecenum << "inserted";
		pieces[piecenum]->setPos(0,0);
		pieces[piecenum]->setVisible(true);
		update();
		QCoreApplication::processEvents();
		movePiece(pieces[piecenum],model->column(piecenum), model->row(piecenum));
	}
	QGraphicsItem::mouseDoubleClickEvent(event);
}

void BoardItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (!track){
//		event->setAccepted(false);
		return;
	}

	xdiff = event->scenePos().x()-lastpos.x();
	ydiff = event->scenePos().y()-lastpos.y();
	if (xdiff>Sensitivity){
		qDebug() << "Move " << lastitem << "right.";
		model->movePiece(lastitem, RIGHT);
		movePiece(pieces[lastitem],model->column(lastitem), model->row(lastitem));
		track=FALSE;
	}
	if (xdiff<-Sensitivity){
		qDebug() << "Move " << lastitem << "left.";
		model->movePiece(lastitem, LEFT);
		movePiece(pieces[lastitem],model->column(lastitem), model->row(lastitem));
		track=FALSE;
	}
	if (ydiff<-Sensitivity){
		qDebug() << "Move " << lastitem << "up.";
		model->movePiece(lastitem, UP);
		movePiece(pieces[lastitem],model->column(lastitem), model->row(lastitem));
		track=FALSE;
	}
	if (ydiff>Sensitivity){
		qDebug() << "Move " << lastitem << "down.";
		model->movePiece(lastitem, DOWN);
		movePiece(pieces[lastitem],model->column(lastitem), model->row(lastitem));
		track=FALSE;
	}
//	event->setAccepted(true);
	
}
