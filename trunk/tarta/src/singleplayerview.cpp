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
#include "msgitem.h"
#include "loadingitem.h"
#include "boardmodel.h"
#include "leveldata.h"
#include <QCoreApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSvgItem>
#include <QGraphicsRectItem>
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include <QDebug>
#include <QFile>
#include <QTimer>

SinglePlayerView::SinglePlayerView( QPixmap prevBg, QWidget *parent ) :
		QGraphicsView( parent )
{
	scene = new QGraphicsScene;
	scene->setItemIndexMethod( QGraphicsScene::NoIndex );

	data = olddata = NULL;
	model = oldmodel = NULL;
	msganimation = NULL;
	msgtline = NULL;
	loading = NULL;
	board = NULL;
	msg = NULL;

	isloading = false;
	currentScore = 0;
	timeLeft = 0;
	lives = 5;

	setScene( scene );
	setCacheMode( CacheBackground );
	setRenderHint( QPainter::Antialiasing );
	setTransformationAnchor( QGraphicsView::AnchorUnderMouse );
	setResizeAnchor( QGraphicsView::AnchorViewCenter );
	setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	setViewportUpdateMode( QGraphicsView::SmartViewportUpdate );
	setOptimizationFlag( QGraphicsView::DontClipPainter );
	setMouseTracking( true );

	prevbg = new QGraphicsPixmapItem( prevBg );
	prevbg->setZValue( 0 );
	scene->addItem( prevbg );
}


void SinglePlayerView::setLevelData( LevelData *newData )
{
	this->data = newData;

	QObject::connect( data, SIGNAL( loading( int, const QString& ) ), this, SLOT( onDataLoading( int, const QString& ) ) );
	QObject::connect( data, SIGNAL( error( int, const QString& ) ), this, SLOT( onDataError( int, const QString& ) ) );
	QObject::connect( data, SIGNAL( success() ), this, SLOT( onDataSuccess() ) );

	if ( !data->isDataLoaded() ) {
		data->loadData();
	}

	if ( !data->isDataLoaded() ) {
		return;
	}

	model = new BoardModel(

	    data->pieceRows(),
	    data->pieceColumns(),
	    data->placeRows(),
	    data->placeColumns()
	);

	QObject::connect( model, SIGNAL( boardComplete() ), this, SLOT( onBoardComplete() ) );

	// Set the backgroung
	scene->setBackgroundBrush( data->bgBrush() );
	data->bgItem()->setPos( 0, 0 );
	data->bgItem()->setZValue( 0 );
	scene->addItem( data->bgItem() );
	scene->removeItem( prevbg );

	board = new BoardItem( data, model );

	int n = data->pieces()->size();

	for ( int i = 0;i < n;i++ ) {
		PieceItem *item = data->pieces()->at( i );
		scene->addItem( item );
		item->setParentItem( board );
	}

	board->setPos( data->boardRect().x(), data->boardRect().y() );

	qDebug() << "Put board into position" << data->boardRect().x() << data->boardRect().y();
	board->scale(
	    ( float ) data->boardRect().width() / ( float ) board->boundingRect().width(),
	    ( float ) data->boardRect().height() / ( float ) board->boundingRect().height()
	);

	qDebug() << "Scaling board by" <<
	( float ) data->boardRect().width() / ( float ) board->boundingRect().width() <<
	( float ) data->boardRect().height() / ( float ) board->boundingRect().height() <<
	"to size" << ( float ) data->boardRect().width() << ( float ) data->boardRect().height() <<
	"from size" << ( float ) board->boundingRect().width() << ( float ) board->boundingRect().height() ;

	board->setZValue( 500 );
	board->setAcceptsHoverEvents( true );

	scene->addItem( board );

	// Add Lives

	if ( data->lifePixmap() ) {
		int x0=data->lifeRect().x();
		int y0=data->lifeRect().y();
		int dx=data->lifeRect().width()/4;
		int dy=data->lifeRect().height();
		qDebug() << "Life position (" << x0 << y0 << ") and size ("<<dx<<dy<<")";

		QPixmap s=data->lifePixmap()->scaled(QSize(dx,dy), Qt::KeepAspectRatio, Qt::SmoothTransformation);
		qDebug() << "Life position (" << x0 << y0 << ") and size" << s.size();
		life1 = new QGraphicsPixmapItem( s );
		life2 = new QGraphicsPixmapItem( s );
		life3 = new QGraphicsPixmapItem( s );
		life4 = new QGraphicsPixmapItem( s );
		life1->setPos( x0, y0 );
		life2->setPos( x0+dx, y0 );
		life3->setPos( x0+2*dx, y0 );
		life4->setPos( x0+3*dx, y0 );
		scene->addItem(life1);
		scene->addItem(life2);
		scene->addItem(life3);
		scene->addItem(life4);

	}

	updateLives();

	// Add Time
	totalTime=timeLeft= data->time();
	timerrect=new QGraphicsRectItem(data->timeRect());
	timerrect->setBrush(data->timerBrush());
	scene->addItem(timerrect);
	updateTimer();
	// Add Score

	scene->setSceneRect( data->bgItem()->boundingRect() );
	fitInView( data->bgItem()->boundingRect(), Qt::KeepAspectRatio );
	QTimer::singleShot( 10, this, SLOT( showready() ) );
	QTimer::singleShot( 1110, this, SLOT( showset() ) );
	QTimer::singleShot( 2210, this, SLOT( showgo() ) );
}

SinglePlayerView::~SinglePlayerView()
{
	if ( scene ) delete scene;
}

void SinglePlayerView::showready()
{
	showMsg( "ready" );
}

void SinglePlayerView::showset()
{
	showMsg( "set" );
}

void SinglePlayerView::showgo()
{
	showMsg( "go" );
}

void SinglePlayerView::onLevelComplete()
{
	emit levelWon();
}

void SinglePlayerView::updateLives()
{
	life1->setVisible(( lives >= 1 ) );
	life2->setVisible(( lives >= 2 ) );
	life3->setVisible(( lives >= 3 ) );
	life4->setVisible(( lives >= 4 ) );
}

void SinglePlayerView::updateTimer()
{
	timerrect->scale(1,(qreal)timeLeft/(qreal)totalTime);
}

void SinglePlayerView::onDataLoading( int percent, const QString& description )
{
	if ( !isloading ) {
		lbrect = new QGraphicsRectItem( 0, 0, 1024, 768 );
		QColor bgcol( Qt::gray );
		bgcol.setAlpha( 200 );
		lbrect->setBrush( bgcol );
		lbrect->setPen( Qt::NoPen );
		scene->addItem( lbrect );
		lbrect->scale( 1, 0.01 );

		loading = new LoadingItem();
		scene->addItem( loading );
		loading->setZValue( 100 );
		loading->setPos( QPointF( width() / 2, height() / 2 ) - loading->boundingRect().center() );
		loading->scale( 0.01, 0.01 );

		lanimation = new QGraphicsItemAnimation();
		ltline = new QTimeLine( 500 );
		lanimation->setItem( loading );
		lanimation->setTimeLine( ltline );
		lanimation->setScaleAt( 0, 0.01, 0.01 );
		lanimation->setScaleAt( 1, 1, 1 );

		lbanimation = new QGraphicsItemAnimation();
		lbtline = new QTimeLine( 500 );
		lbanimation->setItem( lbrect );
		lbanimation->setTimeLine( lbtline );
		lbanimation->setScaleAt( 0, 1, 0.01 );
		lbanimation->setScaleAt( 1, 1, 1 );

		lbtline->start();
		ltline->start();
		isloading = true;
	}

	loading->setPct( percent, description );
}

void SinglePlayerView::onDataError( int code, const QString& description )
{
	showMsg( description );
	emit gameOver();
	QTimer::singleShot( 10, this, SLOT( hideLoading() ) );
	qDebug() << "Loading error" << code << ":" << description;
}

void SinglePlayerView::onDataSuccess()
{
	QTimer::singleShot( 10, this, SLOT( hideLoading() ) );
	isloading = false;
	emit levelLoaded();
	qDebug() << "Data loading success";
}

void SinglePlayerView::onBoardComplete()
{
	showMsg( "Level Complete!" );
	qDebug() << "Board Complete!";
}

void SinglePlayerView::showMsg( const QString& ms )
{
	while ( msg ) {
		QCoreApplication::processEvents();
		//scene->removeItem(msg);
		//delete msg;
	}

	msg = new MsgItem( ms );

	msg->setPos( sceneRect().center() - msg->boundingRect().center() );

	msganimation = new QGraphicsItemAnimation();
	msgtline = new QTimeLine( 500 );
	msganimation->setItem( msg );
	msganimation->setTimeLine( msgtline );
	msganimation->setScaleAt( 0, 0.01, 0.01 );
	msganimation->setScaleAt( 1, 1, 1 );
	msg->scale( 0.01, 0.01 );
	scene->addItem( msg );
	msgtline->start();
	QTimer::singleShot( 1000, this, SLOT( hideMsg() ) );
}

void SinglePlayerView::hideMsg()
{
	connect( msgtline, SIGNAL( finished() ), this, SLOT( cleanUpMsg() ) );
	msganimation->setPosAt( 0, sceneRect().center() - msg->boundingRect().center() );
	msganimation->setScaleAt( 0, 1, 1 );
	msganimation->setPosAt( 1, QPointF( sceneRect().center().x(), sceneRect().bottomRight().y() ) );
	msganimation->setScaleAt( 1, 0.01, 0.01 );
	msgtline->start();
}

void SinglePlayerView::hideLoading()
{
	lanimation->setScaleAt( 0, 1, 1 );
	lanimation->setScaleAt( 1, 0.01, 0.01 );
	lbanimation->setScaleAt( 0, 1, 1 );
	lbanimation->setScaleAt( 1, 1, 0.01 );
	connect( lbtline, SIGNAL( finished() ), this, SLOT( cleanUpLBG() ) );
	connect( ltline, SIGNAL( finished() ), this, SLOT( cleanUpLoading() ) );
	lbtline->start();
	ltline->start();

}

void SinglePlayerView::cleanUpLBG()
{
	qDebug() << "Cleaning up loading bg";
	scene->removeItem( lbrect );
	delete lbrect;
	delete lbanimation;
	delete lbtline;
	lbrect = NULL;
	lbanimation = NULL;
	lbtline = NULL;
}

void SinglePlayerView::cleanUpLoading()
{
	qDebug() << "Cleaning up loading";
	scene->removeItem( loading );
	delete loading;
	delete lanimation;
	delete ltline;
	loading = NULL;
	lanimation = NULL;
	ltline = NULL;
}

void SinglePlayerView::cleanUpMsg()
{
	qDebug() << "Cleaning up msg";

	scene->removeItem( msg );
	delete msg;
	delete msganimation;
	delete msgtline;
	msg = NULL;
	msganimation = NULL;
	msgtline = NULL;
}

void SinglePlayerView::resizeEvent( QResizeEvent *event )
{
	if ( data && data->isDataLoaded() )
		fitInView( data->bgItem()->boundingRect(), Qt::KeepAspectRatio );

	QWidget::resizeEvent( event );
}
