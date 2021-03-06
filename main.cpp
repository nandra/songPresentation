/*
 * Copyright (C) 2012 Marek Belisko <marek.belisko@open-nandra.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtGui/QApplication>
#include <QDesktopWidget>
#include <QTranslator>
#include "userwindow.h"
#include "displayform.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QString dataPath;
	for (int i = 0; i < argc; i++ ) {
		if (QString(argv[i]).contains("-dataPath")) {
			dataPath = QString(argv[i]).mid(strlen("-dataPath="));
			qDebug() << "Data path:" << dataPath;
			break;
		}
	}

	QTranslator translator;
	if (!translator.load("songPresentation_sk.qm", dataPath))
		qDebug() << "cannot load translator";

	a.installTranslator(&translator);

	DisplayForm d;
	UserWindow w(&d, dataPath);

	QDesktopWidget *desktop = QApplication::desktop();
	QRect rect = desktop->screenGeometry(0);
	d.move(rect.width(), 0);

	d.showMaximized();
	w.showFullScreen();

	return a.exec();
}
