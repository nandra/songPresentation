#ifndef DISPLAYFORM_H
#define DISPLAYFORM_H

#include <QWidget>

namespace Ui {
class DisplayForm;
}

class DisplayForm : public QWidget
{
	Q_OBJECT

public:
	explicit DisplayForm(QWidget *parent = 0);
	~DisplayForm();
	void setTitleText(const QString& text = "");
	void setMainText(const QString& text = "");
	void setAlignment(Qt::Alignment alignment);
	void setFontSize(int size);
	void removeTitle();
	void addTitle();

private:
	Ui::DisplayForm *ui;
};

#endif // DISPLAYFORM_H
