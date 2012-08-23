#include "displayform.h"
#include "ui_displayform.h"

DisplayForm::DisplayForm(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::DisplayForm)
{
	ui->setupUi(this);
	ui->displayLabel->clear();
	/* set white font */
	QFont f( "Arial", 56, QFont::Bold);
	ui->displayLabel->setFont(f);

	ui->displayLabel->setStyleSheet("QLabel { color: white; }");

	/* set back color for this widget */
	this->setStyleSheet("QWidget { background-color: black }");
}

DisplayForm::~DisplayForm()
{
	delete ui;
}

void DisplayForm::setTitleText(const QString &text)
{

}

void DisplayForm::setMainText(const QString &text)
{
	ui->displayLabel->setText(text);
}
