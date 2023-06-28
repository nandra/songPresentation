#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QObject>
#include <QTranslator>
#include <QApplication>

class translator : public QObject {
        Q_OBJECT

public:
    translator(const QString dataPath, const QString language, QApplication *a);
    QTranslator *langTranslator() {return m_translator;}
private:
    QTranslator *m_translator;
    QApplication *m_app;
    QString m_dataPath;
private slots:
    void on_language_change(QString);
};

#endif // TRANSLATOR_H
