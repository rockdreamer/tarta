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
 
#include <QMainWindow>
#include <QMessageBox>
#include <QMenu>
#include <QTimer>
#include <QMenuBar>
#include <QLabel>
#include <QApplication>
#include <QStatusBar>
#include "tarta.h"
#include "singleplayerview.h"
#include "leveldata.h"
#include "about.h"

Tarta::Tarta() : QMainWindow(0)
{
	QMenu * file = new QMenu( tr("&File"), this );
    menuBar()->addMenu( file );
    file->addAction( tr("&Play"), this, SLOT( startSinglePlayer() ),  Qt::CTRL + Qt::Key_P );
    file->addAction( tr("&Next Level"), this, SLOT(loadNextLevel() ),  Qt::CTRL + Qt::Key_N );
	file->addSeparator();
    file->addAction( tr("&Quit"), qApp, SLOT( closeAllWindows() ),  Qt::CTRL + Qt::Key_Q );

    menuBar()->addSeparator();

    QMenu * help = new QMenu( tr("&Help"), this );
    menuBar()->addMenu(  help );

    help->addAction( tr("&About"), this, SLOT(about()), Qt::Key_F1 );
    help->addAction( tr("About &Qt"), this, SLOT(aboutQt()));
    
	setWindowTitle(tr("Tarta"));
	intro = new QLabel(this);
	intro->setPixmap(QPixmap(":/tarta-intro.png"));
	intro->setScaledContents(true);
	setCentralWidget( intro );
	resize(800,600);

	this->setAttribute(Qt::WA_Hover);

	prevLevel=currentLevel=NULL;
	levels=NULL;
	listloaded=false;
	isplaying=false;
	ispaused=false;
	
}

void Tarta::startSinglePlayer()
{
	if (intro) {
		intro->setVisible(false);
		intro->deleteLater();
		intro=NULL;
	}
	e = new SinglePlayerView(QPixmap(":/tarta-intro.png"), this);
	e->setFocus();
    setCentralWidget( e );

	if (!setLevelsList(":/levels.lst")){
		e->onDataError(0,"Could not load level list");
		return;
	} else {
		listloaded=true;
	}
	    
	QTimer::singleShot(20, this, SLOT(loadNextLevel()));

}

bool Tarta::setLevelsList(const QString& path)
{
	if (levels) delete levels;
	levels = new QStringList;

	if (!QFile::exists(path)){
		return false;
	}
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		return false;
	}
	while (!file.atEnd()) {
		QByteArray line = file.readLine();
		// remove whitespace at the beginning and at the end
		line=line.trimmed();
		// empty Lines
		if (line.isEmpty())
			continue;
		// Comment lines, ignore
		if (line.startsWith("#"))
			continue;
		// look for comments after the line
		int hasComment=line.indexOf("#");
		if (hasComment!= -1){
			line.truncate(hasComment-1);
			// trim more whitespace
			line=line.trimmed();
		}
		qDebug() << "Parsing cleaned line:"<< line;
		levels->append(line);
	}
	qDebug() << "Levels list" << *levels;
	currentlevelname= new QStringListIterator(*levels);
	return true;
}

void Tarta::loadNextLevel()
{
	if (currentlevelname->hasNext()) {
		qDebug() << "About to load level" << ("/Users/rdfm/tarta/data/levels/"+currentlevelname->peekNext()+"/");
		prevLevel = currentLevel;
		currentLevel = new LevelData();	
		currentLevel->setBaseDir("/Users/rdfm/tarta/data/levels/"+currentlevelname->next()+"/");
		connect(e, SIGNAL(levelLoaded()), this, SLOT(delPrevLevel()));
		connect(e, SIGNAL(levelWon()), this, SLOT(loadNextLevel()));
		e->setLevelData(currentLevel);
	} else {
		// Game is finished, do something pretty....
	}
}

Tarta::~Tarta()
{
	delete currentLevel;
}

void Tarta::delPrevLevel()
{
	if (prevLevel) prevLevel->deleteLater();
}

void Tarta::about()
{
	About *about = new About();
	about->show();
}


void Tarta::aboutQt()
{
    QMessageBox::aboutQt( this, tr("Tarta") );
}
