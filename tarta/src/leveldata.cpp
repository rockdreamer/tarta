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
 
#include "leveldata.h"
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QString>
#include <QPixmap>
#include <QSvgRenderer>
#include <QImage>
#include <QPainter>
#include <QGraphicsSvgItem>
#include <QGraphicsPixmapItem>

extern const int PieceNum;

LevelData::LevelData(QObject *parent): 
	QObject(parent)
{
	m_isdataloaded=false;
}

void LevelData::setBaseDir(const QString& baseDir)
{
	m_basedir=baseDir;
}

bool LevelData::isDataLoaded()
{
	return m_isdataloaded;
}

QString LevelData::levelName()
{
	return m_level;
}

QRect LevelData::boardRect()
{
	return m_boardrect;
}

QRect LevelData::timeRect()
{
	return m_timerect;
}

QBrush LevelData::bgBrush()
{
	return m_bgbrush;
}

QGraphicsItem * LevelData::bgItem()
{
	return m_levelbackground;
}

QGraphicsItem * LevelData::targetItem()
{
	return m_targetitem;
}

QVector<PieceItem*> * LevelData::pieces()
{
	return m_pieces;
}

int LevelData::time()
{
	return m_time;
}

int LevelData::livesAwarded()
{
	return m_livesawarded;
}

int LevelData::pieceRows()
{
	return m_pieceRows;
}

int LevelData::pieceColumns()
{
	return m_pieceColumns;
}

int LevelData::placeColumns()
{
	return m_placeColumns;
}

int LevelData::placeRows()
{
	return m_placeRows;
}



void LevelData::loadData()
{
	if (m_basedir.isEmpty()) {
		emit error(0, tr("Empty basedir"));
		QCoreApplication::processEvents();
		return;
	}
	emit loading(1,tr("Got a Directory"));
	QCoreApplication::processEvents();

	if (m_basedir.startsWith("http://")) {
		emit error(1, tr("Gimmeabreak")); // ;)
		QCoreApplication::processEvents();
		return;
	}
	if (!parseDefaults()) {
		// handles errors by itself
		return;
	}
	if (!parseLevel()) {
		// handles errors by itself
		return;
	}
	emit loading(70,tr("Loading target picture"));
	QCoreApplication::processEvents();
	if (!loadTargetPix()){
		// handles errors by itself
		return;
	}
	emit loading(75,tr("Loading Background picture"));
	QCoreApplication::processEvents();
	if (!loadBackgroundPix()){
		// handles errors by itself
		return;
	}
	if (!loadBoardPix()){
		// handles errors by itself
		return;
	}
	//done
	emit loading(100,tr("Done"));
	QCoreApplication::processEvents();
	emit success();
	m_isdataloaded=true;
}

bool LevelData::parseDefaults()
{
	if (!QFile::exists(":levels/default/level.txt")){
		emit error(2, tr("Could not find default level description"));
		return false;
	}
	emit loading(5,tr("Got a Default Level Description"));
	QCoreApplication::processEvents();

	QFile file(":levels/default/level.txt");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		emit error(3, tr("Could not load level description"));
		return false;
	}
	QByteArray firstline = file.readLine();
	if (!handle_version(firstline)){
		return false;
	}
	emit loading(10,tr("Level Description opened, good version"));
	QCoreApplication::processEvents();

	linenum=2;

	qint64 sz=file.size();
	qint64 read=firstline.size();

	while (!file.atEnd()) {
		QByteArray line = file.readLine();
		if (!process_line(line)){
			return false;
		}
		read+=line.size();
		linenum++;
		emit loading(10+read*30/sz,tr("Loading level description %1/%2").arg(read).arg(sz));
		QCoreApplication::processEvents();
	}
	return true;
}

bool LevelData::parseLevel()
{
	if (!QFile::exists(m_basedir+"/level.txt")){
		emit error(2, tr("Could not find level description"));
		return false;
	}
	emit loading(45,tr("Got a Level Description"));
	QCoreApplication::processEvents();

	QFile file(m_basedir+"/level.txt");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
		emit error(3, tr("Could not load level description"));
		return false;
	}
	QByteArray firstline = file.readLine();
	if (!handle_version(firstline)){
		return false;
	}
	emit loading(50,tr("Level Description opened, good version"));
	QCoreApplication::processEvents();

	linenum=2;

	qint64 sz=file.size();
	qint64 read=firstline.size();

	while (!file.atEnd()) {
		QByteArray line = file.readLine();
		if (!process_line(line)){
			return false;
		}
		read+=line.size();
		linenum++;
		emit loading(50+read*20/sz,tr("Loading level description %1/%2").arg(read).arg(sz));
		QCoreApplication::processEvents();
	}
	return true;
}

bool LevelData::handle_version(QString line)
{
	if (line.isEmpty()){
		emit error(4, tr("No level description version"));
		return false;
	}
	if (!line.startsWith("--TARTACONFVER:1--")){
		emit error(5, tr("Tarta cannot parse this conf file version"));
		return false;
	}
	return true;
	
}

bool LevelData::process_line(QString line)
{
	// remove whitespace at the beginning and at the end
	line=line.trimmed();
	// empty Lines
	if (line.isEmpty())
		return true;
	// Comment lines, ignore
	if (line.startsWith("//"))
		return true;
		
	// look for comments after a useful line and trim
	int hasComment=line.indexOf("//");
	if (hasComment!= -1){
		line.truncate(hasComment-1);
		// trim more whitespace
		line=line.trimmed();
	}
	
	qDebug() << "Parsing cleaned line"<< linenum <<  ":" << line;

	if (line.startsWith("name")){
		QString t=line.section('|',1,1);
		if (t.isEmpty()){
			emit error(6, tr("Invalid name line at line %1").arg(linenum));
			return false;
		}
		m_level = t;
		qDebug() << "Parsed name:"<<m_level;
		return true;
	}
	
	if (line.startsWith("boardrect")){
		bool ok=true;
		int x=line.section('|',1,1).toInt(&ok);
		if (!ok){
			emit error(7, tr("Invalid Boardrect x at line %1").arg(linenum));
			return false;
		}
		int y=line.section('|',2,2).toInt(&ok);
		if (!ok){
			emit error(8, tr("Invalid Boardrect y at line %1").arg(linenum));
			return false;
		}
		int w=line.section('|',3,3).toInt(&ok);
		if (!ok){
			emit error(9, tr("Invalid Boardrect w at line %1").arg(linenum));
			return false;
		}
		int h=line.section('|',4,4).toInt(&ok);
		if (!ok){
			emit error(10, tr("Invalid Boardrect h at line %1").arg(linenum));
			return false;
		}
		m_boardrect= QRect(x,y,w,h);
		qDebug() << "Parsed Boardrect:"<<m_boardrect;
		return true;
	}

	if (line.startsWith("timerect")){
		bool ok=true;
		int x=line.section('|',1,1).toInt(&ok);
		if (!ok){
			emit error(11, tr("Invalid Timerrect x at line %1").arg(linenum));
			return false;
		}
		int y=line.section('|',2,2).toInt(&ok);
		if (!ok){
			emit error(12, tr("Invalid Timerrect y at line %1").arg(linenum));
			return false;
		}
		int w=line.section('|',3,3).toInt(&ok);
		if (!ok){
			emit error(13, tr("Invalid Timerrect w at line %1").arg(linenum));
			return false;
		}
		int h=line.section('|',4,4).toInt(&ok);
		if (!ok){
			emit error(14, tr("Invalid Timerrect h at line %1").arg(linenum));
			return false;
		}
		m_timerect= QRect(x,y,w,h);
		qDebug() << "Parsed Timerrect:"<<m_timerect;
		return true;
	}

	if (line.startsWith("boardgeometry")){
		bool ok=true;
		m_pieceRows=line.section('|',1,1).toInt(&ok);
		if (!ok){
			emit error(15, tr("Invalid Boardgeometry piecerows at line %1").arg(linenum));
			return false;
		}
		m_pieceColumns=line.section('|',2,2).toInt(&ok);
		if (!ok){
			emit error(16, tr("Invalid Boardgeometry piececols at line %1").arg(linenum));
			return false;
		}
		m_placeRows=line.section('|',3,3).toInt(&ok);
		if (!ok){
			emit error(17, tr("Invalid Boardgeometry placerows at line %1").arg(linenum));
			return false;
		}
		m_placeColumns=line.section('|',4,4).toInt(&ok);
		if (!ok){
			emit error(18, tr("Invalid Boardgeometry placecols at line %1").arg(linenum));
			return false;
		}
		qDebug() << "Parsed Board Geometry:" << 
			m_pieceRows << m_pieceColumns << m_placeRows << m_placeColumns;
		return true;
	}

	if (line.startsWith("backgroundpic")){
		QString t=line.section('|',1,1);
		if (t.isEmpty()){
			emit error(19, tr("Invalid backgroundpic at line %1").arg(linenum));
			return false;
		}
		m_bgfile = t;
		qDebug() << "Parsed backgroundpic filename:"<<m_bgfile;
		return true;
	}

	if (line.startsWith("targetpic")){
		QString t=line.section('|',1,1);
		if (t.isEmpty()){
			emit error(20, tr("Invalid targetpic at line %1").arg(linenum));
			return false;
		}
		m_targetfile = t;
		qDebug() << "Parsed targetpic filename:"<<m_targetfile;
		return true;
	}

	if (line.startsWith("boardpic")){
		QString t=line.section('|',1,1);
		if (t.isEmpty()){
			emit error(21, tr("Invalid boardpic at line %1").arg(linenum));
			return false;
		}
		m_picfile = t;
		qDebug() << "Parsed boardpic filename:"<<m_picfile;
		return true;
	}

	if (line.startsWith("backgroundbrush")){
		if (line.section('|',1,1)=="color"){
			QColor m_backgroundcolor;
			m_backgroundcolor.setNamedColor(line.section('|',2,2));
			if (!m_backgroundcolor.isValid()){
				emit error(22, tr("Invalid Backgroundcolor at line %1").arg(linenum));
				return false;
			}
			m_bgbrush.setColor(m_backgroundcolor);
			qDebug() << "Parsed background color"<< m_backgroundcolor.name();
			return true;
		}
		if (line.section('|',1,1)=="file"){
			if (!QFile::exists(m_basedir+line.section('|',2,2))){
				emit error(253, "Non existent board background file");
				return false;
			}
			QPixmap *m_bg = new QPixmap(m_basedir+line.section('|',2,2));
			if (!m_bg){
				emit error(2254, tr("Invalid board pixmap at line %1").arg(linenum));
				return false;
			}
			m_bgbrush.setTexture(*m_bg);
			qDebug() << "Parsed background pixmap"<< line;
			return true;
		}
		return false;
	}

	if (line.startsWith("playtime")){
		bool ok=true;
		m_time=line.section('|',1,1).toInt(&ok);
		if (!ok){
			emit error(23, tr("Invalid Time at line %1").arg(linenum));
			return false;
		}
		qDebug() << "Parsed time"<< m_time;

		return true;
	}

	if (line.startsWith("livesawarded")){
		bool ok=true;
		m_livesawarded=line.section('|',1,1).toInt(&ok);
		if (!ok){
			emit error(24, tr("Invalid livesawarded at line %1").arg(linenum));
			return false;
		}
		qDebug() << "Parsed livesawarded"<< m_livesawarded;
		return true;
	}
	qDebug() << "I don't know how to handle line" << linenum <<":"<< line;

	return true;
}

bool LevelData::loadBoardPix() 
{
	QPixmap *wholepix=NULL;
	
	//Load the board image
	if (!QFile::exists(m_basedir+m_picfile)){
		emit error(25, "Non existent board picture file");
		return false;
	}
	if (m_picfile.endsWith("svg")){
		qDebug() << "Loading" << m_basedir+m_picfile;
		// svg is a particular case, we need to render the thing before we can slice and dice
		QSvgRenderer bigmama(m_basedir+m_picfile);
			
		if (!bigmama.isValid()){
			emit error(26, "Invalid board picture svg file");
			return false;
		}
			
		QImage bg(bigmama.viewBox().width(), bigmama.viewBox().height() ,QImage::Format_ARGB32);
		QPainter p(&bg);
		bigmama.render(&p);
		wholepix = new QPixmap(QPixmap::fromImage(bg));
	} else {
		qDebug() << "Loading" << m_basedir+m_picfile;
		wholepix = new QPixmap(m_basedir+m_picfile);
	}
	
	if (!wholepix){
		emit error(27, "Could not load board picture");
		return false;
	}
	emit loading(70,tr("Cutting up picture"));
	QCoreApplication::processEvents();

	uint p= m_pieceRows*m_pieceColumns;
	uint pc=m_pieceColumns;
	uint pr=m_pieceRows;
	uint sx = wholepix->width()/pc;
	uint sy = wholepix->height()/pr;
	
	qDebug() << "p" << p << "pc" << pc << "pr" << pr;
	qDebug() << "w" << wholepix->width() << "h" << wholepix->height()
			<< "sx" << sx << "sy" << sy;
			
	// prepare the target item for the picture
	qreal scalevalue=qMin( 
		(qreal)sx/(qreal)m_targetitem->boundingRect().width() ,
		(qreal)sy/(qreal)m_targetitem->boundingRect().height());
	m_targetitem->scale(scalevalue,scalevalue);
	m_targetitem->setVisible(false);
	
	m_pieces = new QVector<PieceItem*>(p);	
	for (uint i=0;i<p;i++){
	
		PieceItem *item= 
			new PieceItem(wholepix->copy(
				(i%pc)*sx, (i/pc)*sy, sx, sy) , m_targetitem);
		
		qDebug() << "Created pixmap from (" 
			<< (i%pc)*sx << "," << (i/pc)*sy << ","
			<< sx << "," << sy << ")";

		item->setVisible(false);
		item->setData(PieceNum,i);

		(*m_pieces)[i]=item;
		emit loading(70+(30*i/p),tr("Cutting up picture %1/%2").arg(i).arg(p));
		QCoreApplication::processEvents();

	}
	
	delete wholepix;
	wholepix=NULL;
	return true;
}

bool LevelData::loadTargetPix() 
{
	m_targetitem=NULL;
	
	if (QFile::exists(m_basedir+m_targetfile)){
		if (m_targetfile.endsWith("svg")){
			m_targetitem = new QGraphicsSvgItem(m_basedir+m_targetfile);
		} else {
			m_targetitem = new QGraphicsPixmapItem(m_basedir+m_targetfile);
		}
	} 
	
	if (!m_targetitem) {
		emit error(28, "Could not load Target image");
		return false;
	}
	
	return true;
}

bool LevelData::loadBackgroundPix() 
{
	m_levelbackground=NULL;
	
	if (QFile::exists(m_basedir+m_bgfile)){
		if (m_bgfile.endsWith("svg")){
			m_levelbackground = new QGraphicsSvgItem(m_basedir+m_bgfile);
		} else {
			m_levelbackground = new QGraphicsPixmapItem(m_basedir+m_bgfile);
		}
	} 
	
	if (!m_levelbackground) {
		emit error(29, "Could not load Background image");
		return false;
	}
	
	return true;
}
