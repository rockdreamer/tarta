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
 
#include "singleplayerview.h"
#include "boarditem.h"
#include "leveldata.h"
#include <QCoreApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSvgItem>
#include <QDebug>
#include <QFile>

SinglePlayerView::SinglePlayerView(LevelData *data, QWidget *parent): 
	QGraphicsView(parent)
{
	scene = new QGraphicsScene;
	scene->setItemIndexMethod(QGraphicsScene::NoIndex);		
	
	setScene(scene);
	setCacheMode(CacheBackground);
	setRenderHint(QPainter::Antialiasing);
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	setResizeAnchor(QGraphicsView::AnchorViewCenter);
	setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setOptimizationFlag(QGraphicsView::DontClipPainter);
	setMouseTracking ( true );

	setLevelData(data);
	
}

SinglePlayerView::~SinglePlayerView()
{
	if (scene) delete scene;
}

void SinglePlayerView::setLevelData(LevelData *data)
{
	this->data=data;
	
	QObject::connect(data, SIGNAL(loading(int, const QString&)), this, SLOT(onDataLoading(int, const QString&)));
	QObject::connect(data, SIGNAL(error(int, const QString&)), this, SLOT(onDataError(int, const QString&)));
	QObject::connect(data, SIGNAL(success()), this, SLOT(onDataSuccess()));
	
	if (!data->isDataLoaded()){
		data->loadData();
	}
	if (!data->isDataLoaded()){
		qDebug() << "Holy crap!";
		return;
	}
	
	data->bgItem()->setPos(0,0);
	data->bgItem()->setZValue(0);
	scene->addItem(data->bgItem());
	
	board = new BoardItem(data);
	int n=data->pieces()->size();
	for (int i=0;i<n;i++){
		PieceItem *item=data->pieces()->at(i);
		scene->addItem(item);
		item->setParentItem(board);
	}

	board->setPos(data->boardRect().x(),data->boardRect().y());
	qDebug() << "Put board into position" << data->boardRect().x() << data->boardRect().y();
	board->scale(
		(float)data->boardRect().width()/(float)board->boundingRect().width(),
		(float)data->boardRect().height()/(float)board->boundingRect().height()
	);
	qDebug() << "Scaling board by" << 
	(float)data->boardRect().width()/(float)board->boundingRect().width() << 
	(float)data->boardRect().height()/(float)board->boundingRect().height() <<
	"to size" << (float)data->boardRect().width() << (float)data->boardRect().height() <<
	"from size" << (float)board->boundingRect().width() << (float)board->boundingRect().height() ;
	
	board->setZValue(500);
	board->setAcceptsHoverEvents(true);
	
	scene->addItem(board);
	scene->setSceneRect(data->bgItem()->boundingRect());
	fitInView(data->bgItem()->boundingRect(),Qt::KeepAspectRatio);
	
}

void SinglePlayerView::onDataLoading(int percent, const QString& description)
{
	qDebug() << "Loading at" << percent << "%:" << description;
}

void SinglePlayerView::onDataError(int code, const QString& description)
{
	qDebug() << "Loading error" << code << ":" << description;
}

void SinglePlayerView::onDataSuccess()
{
	qDebug() << "Data loading success";
}

void SinglePlayerView::resizeEvent(QResizeEvent *event)
{
	if (data->isDataLoaded())
		fitInView(data->bgItem()->boundingRect(),Qt::KeepAspectRatio);
	QWidget::resizeEvent(event);
}
