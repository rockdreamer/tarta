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
#include "loadingitem.h"
#include <QCoreApplication>
#include <QGraphicsTextItem>
#include <QColor>
#include <QBrush>
#include <QPainter>
#include <QPen>
#include <QWaitCondition>
#include <QMutex>

LoadingItem::LoadingItem(QGraphicsItem *parent) : QObject(0), QGraphicsItem(parent) 
{
	boundrect.setX(0);
	boundrect.setY(0);
	boundrect.setWidth(400);
	boundrect.setHeight(300);

	t = new QGraphicsTextItem(this);
	t->setTextWidth(380);
	t->setPos(10,110);
}

QRectF LoadingItem::boundingRect() const
{
	return boundrect;
}

void LoadingItem::paint(QPainter *painter,
                   const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	QColor bg=Qt::blue;
	bg.setAlpha(70);
	QBrush b(bg);
	painter->setBrush(b);
	painter->setPen(Qt::black);
	painter->drawRoundRect(0, 0, 400, 300,5,5);

	QColor pc=Qt::red;
	pc.setAlpha(254);
	QBrush p(pc);
	painter->setBrush(pc);
	painter->drawRoundRect(10, 10, (pct*380)/100, 100,5,5);
	
}

void LoadingItem::setPct(int percent, const QString& msg)
{
	pct=percent;
	t->setPlainText(msg);
	update();
	QCoreApplication::processEvents();
	QWaitCondition sleep;
	QMutex m;
	sleep.wait(&m,1);   // two seconds	update();
}
