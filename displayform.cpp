#include "displayform.h"
#include "ui_displayform.h"

DisplayForm::DisplayForm(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::DisplayForm)
{
	ui->setupUi(this);
	// set flags to be fullscreen and no controls
	Qt::WindowFlags flags( Qt::Window | Qt::FramelessWindowHint);
	setWindowFlags(flags);

	ui->displayLabel->clear();
	/* set white font */
	QFont f("Cambria", 48);

	ui->displayLabel->setFont(f);
	ui->displayLabel->setStyleSheet("QLabel { color: white; }");

	ui->titleLabel->setFont(f);
	ui->titleLabel->setStyleSheet("QLabel { color: white; }");

	/* set back color for this widget */
	this->setStyleSheet("QWidget { background-color: black }");
}

DisplayForm::~DisplayForm()
{
	delete ui;
}

void DisplayForm::setTitleText(const QString &text)
{
	ui->titleLabel->setText(text);
}

void DisplayForm::setMainText(const QString &text)
{
	ui->displayLabel->setText(text);
}

void DisplayForm::setAlignment(Qt::Alignment alignment)
{
	ui->displayLabel->setAlignment(alignment);
}

void DisplayForm::setFontSize(int size)
{
	QFont f = ui->displayLabel->font();
	f.setPointSize(size);
	ui->displayLabel->setFont(f);
	ui->titleLabel->setFont(f);
}

void DisplayForm::removeTitle()
{
	ui->verticalLayout->removeWidget(ui->titleLabel);
}

void DisplayForm::addTitle()
{
	ui->verticalLayout->addWidget(ui->titleLabel);

	ui->verticalLayout->removeWidget(ui->displayLabel);
	ui->verticalLayout->addWidget(ui->displayLabel);
}
