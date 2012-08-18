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

#ifndef USERWINDOW_H
#define USERWINDOW_H

#include "displayform.h"

#include <QMainWindow>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QKeyEvent>
#include <QDir>
#include <QHash>
#include <QDebug>

namespace Ui {
class UserWindow;
}

#define SEARCH_SONG_TIMEOUT_SEC (5)

class FileWorker;
class Category;

class UserWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit UserWindow(QWidget *parent = 0);
	~UserWindow();

private:
	Ui::UserWindow *ui;
	QString m_lastSongNumber;
	QTimer *m_songSearchTimer;
	FileWorker *m_fileworker;
	bool m_songActive;
	DisplayForm *m_displayWidget;
	bool m_displayActive;
	Category m_category;

private:
	void keyPressEvent(QKeyEvent *ev);

private slots:
	void songSearchTimer_timeout();
};

class FileWorker : public QObject {

	Q_OBJECT

public:
	FileWorker(QObject *parent = 0);
	~FileWorker();
	void setFileName(const QString& fileName);
	QString nextVerse();
	QString prevVerse();
	QString firstVerse() { return m_songVerses.at(0); }
	void cacheContent();
	int actualVerse() { return m_actualVerse; }

private:
	QString m_fileName;
	QFile *m_file;
	QTextStream m_stream;
	QStringList m_songVerses;
	int m_actualFilePos;
	int m_actualVerse;
	bool m_backWard;
};

class Category : public QObject {

	Q_OBJECT

public:
	Category(QObject *parent = 0);
	enum SongCategory {
		JKS = 0,
		Psalm,
		Breviary,
		Other,
	};

	SongCategory nextCategory();
	SongCategory prevCategory();
	const QString categoryName();

private:
	QHash<int, SongCategory> m_categories;
	int m_actualCategory;
};

#endif // USERWINDOW_H
