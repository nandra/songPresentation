#include "language_selector.h"
#include "ui_language_dialog.h"

#include <QDebug>

LanguageDialog::LanguageDialog(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    // set flags to be fullscreen and no controls
    Qt::WindowFlags flags( Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setWindowFlags(flags);

    /* set back color for this widget */
    this->setStyleSheet("QWidget { background-color: white }");

    ui->lang_label->setText("\t1. " + tr("Slovak")+"\n\n\t2. " + tr("Magyar") + "\n");
    ui->lang_label_help->setText(tr("Select language and confirm by Enter."));
}

LanguageDialog::~LanguageDialog()
{
    delete ui;
}

void LanguageDialog::keyPressEvent(QKeyEvent *ev)
{
    if (ev->key() >= Qt::Key_0 && ev->key() <= Qt::Key_9) {
        /* active file worker => open new song */
        qDebug() << "Event:" << ev->text() << "\n";
        if (ev->text() == "1") {
            m_languageText = "SK";
        } else if (ev->text() == "2") {
            m_languageText = "HU";
        } else {
            qDebug() << "Unknown lanaguge number. Ignoring.\n";
        }

    }

    switch (ev->key()) {
    case Qt::Key_Enter:
        // confirm language change
        emit language_changed(m_languageText);
        this->hide();
        break;
    }
}
