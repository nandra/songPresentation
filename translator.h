#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QObject>
#include <QTranslator>

class translator : public QObject {
        Q_OBJECT

public:
    translator(const QString dataPath, const QString language);
    QTranslator *langTranslator() {return m_translator;}
private:
    QTranslator *m_translator;
private slots:
    void on_language_change(QString);
};

#endif // TRANSLATOR_H
