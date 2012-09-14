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
#include "displayform.h"
#include <QTextCodec>

#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

UserWindow::UserWindow(DisplayForm *display, const QString& dataPath, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::UserWindow),
	m_songSearchTimer(new QTimer(this)),
	m_fileworker(0),
	m_songActive(false),
	m_displayWidget(display),
	m_displayActive(false),
	m_category(new Category()),
	m_dataPath(dataPath)
{
	ui->setupUi(this);

	connect(m_songSearchTimer, SIGNAL(timeout()), this, SLOT(songSearchTimer_timeout()));
	/* clear labels */
	ui->songLabel->clear();
	ui->songLabel->setWordWrap(true);
	ui->songNumberLabel->clear();
	ui->categoryLabel->clear();

	// default category text
	categoryChanged();
	// connect on change
	connect(m_category, SIGNAL(categoryChanged()), this, SLOT(categoryChanged()));
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
				m_fileworker->setFileName(absoluteDataPath(songNumber));
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
				ui->displayActiveCheckBox->setChecked(true);
			} else {
				m_displayWidget->setMainText();
				m_displayWidget->setTitleText();
				m_displayActive = false;
				ui->displayActiveCheckBox->setChecked(false);
			}
			break;
		case Qt::Key_Asterisk:
			m_category->nextCategory();
			qDebug() << m_category->categoryName();
			break;
		}
	}

	if (m_songActive) {
		if (m_fileworker) {
			/* update verse number */
			QString title = QString("%1 - %2").arg(m_lastSongNumber).arg(QString::number(m_fileworker->actualVerse()));
			ui->songNumberLabel->setText(title);
			if (m_displayActive)
				m_displayWidget->setTitleText(title);
			else
				m_displayWidget->setTitleText();
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
			m_fileworker->setFileName(absoluteDataPath(songNumber));
			m_fileworker->cacheContent();
			ui->songLabel->setText(m_fileworker->nextVerse());
			ui->songNumberLabel->setText(m_lastSongNumber + " - " + QString::number(m_fileworker->actualVerse()));
		}
		m_songActive = true;
	}
}

void UserWindow::categoryChanged()
{
	ui->categoryLabel->setText("Category : " + m_category->categoryName());
}

QString UserWindow::absoluteDataPath(const QString& songNumber)
{
	return m_dataPath + "/" + m_category->categoryName() + "/" + songNumber + ".txt";
}

/** FileWorker class implementation */

FileWorker::FileWorker(QObject *parent) :
	QObject(parent),
	m_file(NULL),
	m_actualVerse(0)
{
}

FileWorker::~FileWorker()
{
	if (m_file)
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
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	stream.setCodec(codec);

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
}

QString FileWorker::nextVerse()
{
	QString ret;

	int pos = m_actualVerse;
	if (++pos > m_songVerses.size())
		return ret;

	ret = m_songVerses.at(m_actualVerse);

	m_actualVerse ++;

	return ret;

}

QString FileWorker::prevVerse()
{
	QString ret;

	int pos = m_actualVerse;

	if (--pos < 1)
		return ret;

		pos -= 1;

	m_actualVerse --;

	ret = m_songVerses.at(pos);

	return ret;
}

/** Category class implementation */

Category::Category(QObject *parent) :
	QObject(parent),
	m_actualCategory(0)
{
	m_categories.insert(0, Category::JKS);
	m_categories.insert(1, Category::Psalm);
	m_categories.insert(2, Category::Breviary);
	m_categories.insert(3, Category::Youth);
	m_categories.insert(4, Category::Other);
}

Category::SongCategory Category::nextCategory()
{
	SongCategory category;

	if (m_actualCategory >= m_categories.size())
		m_actualCategory = 0;

	category = m_categories.value(m_actualCategory++);
	emit categoryChanged();
	return category;
}

const QString Category::categoryName()
{
	SongCategory category = m_categories.value(m_actualCategory);

	if (category == Category::JKS) return "JKS";
	if (category == Category::Psalm) return "Psalm";
	if (category == Category::Breviary) return "Breviary";
	if (category == Category::Youth) return "Youth";
	if (category == Category::Other) return "Other";

	return "";
}

void ProjectorControl::powerOn()
{
	if (m_enabled) {
		return;
	}

	changeStatus(true);
	m_enabled = true;
}

void ProjectorControl::standby()
{
	changeStatus(false);
	m_enabled = false;
}

void ProjectorControl::changeStatus(bool enable)
{
	hostent *host;
	sockaddr_in serverSock;
	int sock;
	int port;
#define BUFSIZE 256
	char buf[BUFSIZE];
	int size;

	host = gethostbyname("169.254.100.100");
	port = 10000;

	if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		qDebug() << "Cannot open sock";
		return;
	}

	serverSock.sin_family = AF_INET;
	serverSock.sin_port = htons(port);
	memcpy(&(serverSock.sin_addr), host->h_addr, host->h_length);

	if (connect(sock, (sockaddr *)&serverSock, sizeof(serverSock)) == -1) {
		qDebug() << "Cannot connect";
		return;
	}

	if ((size = send(sock, "\r", 2, 0)) == -1) {
		qDebug() << "Cannot send passwd";
		return;
	}

	size = recv(sock, buf, BUFSIZE, 0);

	if ((size = send(sock, "\r", 2, 0)) == -1) {
		qDebug() << "Cannot send - ENTER";
		return;
	}
	QByteArray cmd = enable ? "C00\r" : "C01\r";
	if ((size = send(sock, cmd.data(), cmd.size() + 1, 0)) == -1) {
		qDebug() << "Cannot send - cmd";
		return;
	}

	size = recv(sock, buf, BUFSIZE, 0);
	size = recv(sock, buf, BUFSIZE, 0);

	close(sock);
}
