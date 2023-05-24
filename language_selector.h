#ifndef LANGUAGE_SELECTOR_H
#define LANGUAGE_SELECTOR_H

#include <QDialog>
#include <QKeyEvent>

namespace Ui {
class Dialog;
}

class LanguageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LanguageDialog(QDialog *parent = 0);
    ~LanguageDialog();

    enum Language {
        SLOVAK = 0,
        HUNGARY,
    };

    QString language() {
        return m_languageText;
    }
private:
  void keyPressEvent(QKeyEvent *ev);

signals:
  void on_language_changed(const QString);

private:
    Ui::Dialog *ui;
    QString m_languageText = "SK";
    Language m_language = SLOVAK; // default language
};

#endif // LANGUAGE_SELECTOR_H
