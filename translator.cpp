#include "translator.h"
#include <QDebug>

translator::translator(const QString dataPath, const QString language, QApplication *a)
    :m_app(a),
     m_dataPath(dataPath)

{

    m_translator = new QTranslator();
    //QTranslator translator;
    //qDebug() << "Called << "songPresentation_" + language.toLower() + ".qm" << "\n";
    qDebug() << dataPath + "/SK/\n";
    if (!m_translator->load(":/languages/songPresentation_sk.qm"))
        qDebug() << "cannot load translator";

    qDebug() << "TRanslator empty:" << m_translator->isEmpty();

    a->installTranslator(m_translator);

}

void translator::on_language_change(QString lang)
{
    qDebug() << "Translator:" << lang;

    m_app->removeTranslator(m_translator);

    delete m_translator;

    m_translator = new QTranslator();
    //QTranslator translator;
    //qDebug() << "Called << "songPresentation_" + language.toLower() + ".qm" << "\n";
    qDebug() << m_dataPath + "/"+lang;
    if (!m_translator->load(":/languages/songPresentation_"+lang.toLower()+".qm"))
        qDebug() << "cannot load translator";

    qDebug() << "TRanslator empty:" << m_translator->isEmpty();

    m_app->installTranslator(m_translator);
    qDebug() << "Translator updated";

}
