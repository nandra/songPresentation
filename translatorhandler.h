#ifndef TRANSLATORHANDLER_H
#define TRANSLATORHANDLER_H

#include <QObject>
#include <QApplication>

class TranslatorHandler
{
public:
    TranslatorHandler(const QString& dataPath, const QString& language);
    QTranslator* translator();
private:
    QTranslator *m_translator;
};

#endif // TRANSLATORHANDLER_H
