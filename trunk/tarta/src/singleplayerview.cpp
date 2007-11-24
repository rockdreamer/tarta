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
#include <QCoreApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSvgItem>
#include <QDebug>
#include <QFile>

SinglePlayerView::SinglePlayerView(QWidget *parent, const QString& levelname): 
	QGraphicsView(parent)
{
	this->levelname=levelname;
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
	
	bg=NULL;
	if (QFile::exists(":/levels/"+levelname+"/background.svg")){
		bg = new QGraphicsSvgItem(":/levels/"+levelname+"/background.svg");
	} else {
		if (QFile::exists(":/levels/default/background.svg")){
			bg = new QGraphicsSvgItem(":/levels/default/background.svg");
		}
	}
	if (bg) {
		bg->setPos(0,0);
		bg->setZValue(0);
		scene->addItem(bg);
	}
	board = new BoardItem(levelname);
	board->setPos(0,0);
	board->setZValue(500);
	board->setAcceptsHoverEvents(true);
	
	scene->addItem(board);
	if (bg)
		scene->setSceneRect(bg->boundingRect());
	fitInView(board->boundingRect(),Qt::KeepAspectRatio);
	
}

SinglePlayerView::~SinglePlayerView()
{
	if (scene) delete scene;
}

void SinglePlayerView::resizeEvent(QResizeEvent *event)
{
	if (bg)
		fitInView(bg->boundingRect(),Qt::KeepAspectRatio);
	QWidget::resizeEvent(event);
}
