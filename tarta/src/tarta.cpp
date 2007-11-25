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
#include <QMenuBar>
#include <QApplication>
#include <QStatusBar>
#include "tarta.h"
#include "singleplayerview.h"
#include "leveldata.h"

Tarta::Tarta() : QMainWindow(0)
{
	QMenu * file = new QMenu( tr("&File"), this );
    menuBar()->addMenu( file );
    file->addAction( tr("&Quit"), qApp, SLOT( closeAllWindows() ),  Qt::CTRL + Qt::Key_Q );

    menuBar()->addSeparator();

    QMenu * help = new QMenu( tr("&Help"), this );
    menuBar()->addMenu(  help );

    help->addAction( tr("&About"), this, SLOT(about()), Qt::Key_F1 );
    help->addAction( tr("About &Qt"), this, SLOT(aboutQt()));
    help->addSeparator();
    help->addAction( tr("What's &This"), this, SLOT(whatsThis()), Qt::SHIFT + Qt::Key_F1);

	this->setAttribute(Qt::WA_Hover);
	currentLevel = new LevelData();
	
	currentLevel->setBaseDir(":/levels/default/");
    
	e = new SinglePlayerView(currentLevel, this);
    
	e->setFocus();
    setCentralWidget( e );
    
	setWindowTitle(tr("Tarta"));
	
	statusBar()->showMessage( tr("Ready"), 2000 );
	
}


Tarta::~Tarta()
{
}


void Tarta::about()
{
    QMessageBox::about( this, tr("Tarta"),
			tr("Massive destruction of world productivity."));
}


void Tarta::aboutQt()
{
    QMessageBox::aboutQt( this, tr("Tarta") );
}