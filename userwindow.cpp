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

#include "userwindow.h"
#include "ui_userwindow.h"

UserWindow::UserWindow(DisplayForm *display, QWidget *parent) :
	QMainWindow(parent),
	m_displayWidget(display),
	ui(new Ui::UserWindow),
	m_songSearchTimer(new QTimer(this)),
	m_fileworker(0),
	m_songActive(false),
	m_displayActive(false)
{
	ui->setupUi(this);

	connect(m_songSearchTimer, SIGNAL(timeout()), this, SLOT(songSearchTimer_timeout()));
	/* clear labels */
	ui->songLabel->clear();
	ui->songNumberLabel->clear();
}

UserWindow::~UserWindow()
{
	delete ui;
}

void UserWindow::keyPressEvent(QKeyEvent *ev)
{
	if (ev->key() >= Qt::Key_0 && ev->key() <= Qt::Key_9) {
		/* active file worker => open new song */
		if (m_songActive) {
			m_lastSongNumber = ev->text();
			m_songActive = false;
			ui->songNumberLabel->setText("");

			if (m_fileworker) {
				m_fileworker->deleteLater();
				m_fileworker = 0;
			}

		} else {
			m_lastSongNumber += ev->text();
		}

		/* start delay timeout */
		m_songSearchTimer->stop();
		m_songSearchTimer->start(SEARCH_SONG_TIMEOUT_SEC * 1000);

	} else {
		switch (ev->key()) {
		case Qt::Key_Plus:
		{
			QString songNumber = m_lastSongNumber.rightJustified(3, '0');

			if (!m_fileworker) {
				qDebug() << __FUNCTION__ << __LINE__;
				m_fileworker = new FileWorker();
				m_fileworker->setFileName(QDir().absolutePath() + "/data/JKS/" + songNumber + ".txt");
				m_fileworker->cacheContent();
				m_songActive = true;
			}

			QString text = m_fileworker->nextVerse();
			qDebug() << text;
			if (!text.isEmpty()) {
				ui->songLabel->setText(text);
				if (m_displayActive)
					m_displayWidget->setMainText(text);
			}
			break;
		}
		case Qt::Key_Minus:
		{
			if (!m_fileworker)
				return;
			QString text = m_fileworker->prevVerse();
			qDebug() << text;
			if (!text.isEmpty()) {
				ui->songLabel->setText(text);
				if (m_displayActive)
					m_displayWidget->setMainText(text);
			}
			break;
		}
		case Qt::Key_Enter:
			// display on display dialog
			if (!m_displayActive) {
				m_displayWidget->setMainText(ui->songLabel->text());
				m_displayActive = true;
			} else {
				m_displayWidget->setMainText("");
				m_displayActive = false;
			}
			break;
		}
	}

	if (m_songActive) {
		if (m_fileworker) {
			/* update verse number */
			ui->songNumberLabel->setText(m_lastSongNumber + " - " + QString::number(m_fileworker->actualVerse()));
		}
	} else {
		ui->songNumberLabel->setText(m_lastSongNumber);
	}
}

/**
 * Search new song
 */
void UserWindow::songSearchTimer_timeout()
{
	m_songSearchTimer->stop();

	// activate song if not active yet
	if (!m_songActive) {
		qDebug() << __FUNCTION__ << __LINE__;
		// open last entered song number
		qDebug() << "Opening song:" << m_lastSongNumber;

		QString songNumber = m_lastSongNumber.rightJustified(3, '0');

		if (m_fileworker) {
			m_fileworker->deleteLater();
			m_fileworker = 0;
		}

		if (!m_fileworker) {
			m_fileworker = new FileWorker();
			m_fileworker->setFileName(QDir().absolutePath() + "/data/JKS/" + songNumber + ".txt");
			m_fileworker->cacheContent();
			ui->songLabel->setText(m_fileworker->nextVerse());
			ui->songNumberLabel->setText(m_lastSongNumber + " - " + QString::number(m_fileworker->actualVerse()));
		}
		m_songActive = true;
	}
}

/** FileWorker class implementation */

FileWorker::FileWorker(QObject *parent) :
	QObject(parent),
	m_actualVerse(0),
	m_listIterator(m_songVerses)
{
}

FileWorker::~FileWorker()
{
	m_file->close();
}

void FileWorker::setFileName(const QString &fileName)
{
	m_fileName = fileName;
}

void FileWorker::cacheContent()
{
	if (!QFile::exists(m_fileName)) {
		qWarning() << "File doesn't exist:" << m_fileName;
		return;
	}

	qDebug() << "Opening:" << m_fileName;
	m_file = new QFile(m_fileName);

	/* Don't chek for open up is check if file exist */
	m_file->open(QIODevice::ReadOnly | QIODevice::Text);


	QTextStream stream(m_file);
	QString verse;
	QString line;
	int lineNumber = 0;

	do {
		line = stream.readLine();

		if (line.isEmpty() && lineNumber != 0) {
			m_songVerses << verse;
			verse.clear();
			lineNumber ++;
			continue;
		}
		verse.append(line + "\n");
		lineNumber ++;

	} while (!line.isNull());

	m_listIterator = m_songVerses;
}

QString FileWorker::nextVerse()
{
	QString ret;

	if (m_listIterator.hasNext()) {
		ret = m_listIterator.next();
		// skip first
		if (m_backWard) {
			if (m_listIterator.hasNext()) {
				ret = m_listIterator.next();
				m_backWard = false;
			}
		}

		m_actualVerse ++;
	}

	return ret;

}

QString FileWorker::prevVerse()
{
	QString ret;

	if (m_listIterator.hasPrevious()) {
		ret = m_listIterator.previous();
		if (m_actualVerse == m_songVerses.size()) {
			if (m_listIterator.hasPrevious())
					ret = m_listIterator.previous();
		}

		m_actualVerse --;
		if (m_actualVerse == 1)
			m_backWard = true;
	}

	return ret;
}
