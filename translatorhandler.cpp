#include "translatorhandler.h"

#include <QTranslator>
#include <QDebug>
#include <QApplication>

TranslatorHandler::TranslatorHandler(const QString& dataPath, const QString& language)
{
    m_translator = new QTranslator();
    //QTranslator translator;
    //qDebug() << "Called << "songPresentation_" + language.toLower() + ".qm" << "\n";
    qDebug() << dataPath + "/SK/\n";
    if (!m_translator->load("songPresentation_sk.qm", dataPath + "/SK/"))
        qDebug() << "cannot load translator";

    qDebug() << "TRanslator empty:" << m_translator->isEmpty();

    //a->installTranslator(m_translator);

}

QTranslator* TranslatorHandler::translator()
{
    return m_translator;
}
