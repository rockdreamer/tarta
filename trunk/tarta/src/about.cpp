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

#include "about.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>
#include <QTextBrowser>
#include <QPixmap>
#include <QVBoxLayout>
#include <QWidget>

About::About(QWidget *parent):QWidget(parent)
{
    
    resize(400, 500);
    vboxLayout = new QVBoxLayout(this);
	vboxLayout->setContentsMargins(2,2,2,2);
    label = new QLabel(this);
	label->setPixmap(QPixmap(":about.png"));
    vboxLayout->addWidget(label);

    textBrowser = new QTextBrowser(this);
	textBrowser->setSource(QUrl("qrc:/credits.html"));
	textBrowser->setOpenExternalLinks(true);
    vboxLayout->addWidget(textBrowser);

    hboxLayout = new QHBoxLayout();
	hboxLayout->setContentsMargins(2,2,2,2);

    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hboxLayout->addItem(spacerItem);

    done = new QPushButton(this);
    done->setText(tr("Tartalicious!"));
	connect(done, SIGNAL(clicked()), this, SLOT(close()));
    hboxLayout->addWidget(done);

    spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem1);

    vboxLayout->addLayout(hboxLayout);
}
