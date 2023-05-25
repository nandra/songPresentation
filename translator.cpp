#include "translator.h"
#include <QDebug>

translator::translator(const QString dataPath, const QString language)
{

    m_translator = new QTranslator();
    //QTranslator translator;
    //qDebug() << "Called << "songPresentation_" + language.toLower() + ".qm" << "\n";
    qDebug() << dataPath + "/SK/\n";
    if (!m_translator->load("songPresentation_sk.qm", dataPath + "/SK/"))
        qDebug() << "cannot load translator";

    qDebug() << "TRanslator empty:" << m_translator->isEmpty();

}

void translator::on_language_change(QString lang)
{
    qDebug() << "Translator:" << lang;
}
