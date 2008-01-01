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
 
#ifndef SINGLEPLAYERVIEW_H
#define SINGLEPLAYERVIEW_H

#include <QGraphicsView>

class QGraphicsScene;
class QResizeEvent;
class BoardItem;
class QGraphicsSvgItem;
class QGraphicsRectItem;
class LevelData;
class BoardModel;
class LoadingItem;
class MsgItem;
class QStringList;
class QGraphicsItemAnimation;
class QTimeLine;


class SinglePlayerView: public QGraphicsView
{
    Q_OBJECT

public:
    SinglePlayerView(QPixmap prevGb, QWidget *parent = 0);
    ~SinglePlayerView();
	
public slots:
	void setLevelData(LevelData *newData);
	void showMsg(const QString& msg);
	void onDataLoading(int percent, const QString& description);
	void onDataError(int code, const QString& description);
	void onLevelComplete();
	void onDataSuccess();
	void onBoardComplete();
	
signals:
	void levelLoaded();
	void levelWon();
	void gameOver();
	
protected:
	void resizeEvent(QResizeEvent *event);
	
private slots:
	void updateLives();
	void hideMsg();
	void hideLoading();
	void cleanUpLBG();
	void cleanUpLoading();
	void cleanUpMsg();
	void showready();
	void showset();
	void showgo();

private:
	int currentScore;
	int timeLeft;
	int lives;
	QGraphicsScene *scene;
	LoadingItem *loading;
	QGraphicsItemAnimation *msganimation, *lanimation, *lbanimation;
	QGraphicsRectItem *lbrect;
	QGraphicsPixmapItem *prevbg;
	QGraphicsPixmapItem *life1, *life2, *life3, *life4;
	QTimeLine *msgtline, *ltline, *lbtline;
	MsgItem *msg;
	LevelData *data, *olddata;
	BoardItem *board, *oldboard;
	BoardModel *model, *oldmodel;
	bool isloading;
};


#endif
