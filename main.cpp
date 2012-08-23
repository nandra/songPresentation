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
#include "userwindow.h"
#include "displayform.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	DisplayForm d;
	UserWindow w(&d);

	QDesktopWidget *desktop = QApplication::desktop();
	QRect rect = desktop->screenGeometry(1);
	d.move(rect.x(), rect.y());

	w.show();
	d.showFullScreen();

	return a.exec();
}
