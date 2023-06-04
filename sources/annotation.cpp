#include "annotation.hpp"

#include <QMap>
#include <QJsonObject>

#include <memory>

Annotation::Annotation(QObject *parent)
    : QObject{parent}
{
    this->m_statement = std::unique_ptr<QString>(new QString("Statement is not available"));
    this->m_verdict = std::unique_ptr<qint16>(new qint16(0));
    this->m_evidence = std::unique_ptr<QString>(new QString("Evidence is not available"));
    this->m_startIndex = std::unique_ptr<qint16>(new qint16(0));
    this->m_endIndex = std::unique_ptr<qint16>(new qint16(0));
}

Annotation::Annotation(Annotation const& annotation, QObject *parent):
    QObject{parent}, m_statement{annotation.m_statement}, m_verdict{annotation.m_verdict},
    m_evidence{annotation.m_evidence}, m_startIndex{annotation.m_startIndex}, m_endIndex{annotation.m_endIndex}
{

}

QJsonObject Annotation::annotation()
{
    return QJsonObject({
                           {"statement", this->statement()},
                           {"verdict", this->verdict()},
                           {"evidence", this->evidence()},
                           {"start", this->startIndex()},
                           {"end", this->endIndex()}
                       });
}

QString Annotation::statement()
{
    return *m_statement;
}

qint16 Annotation::verdict()
{
    return *m_verdict;
}

QString Annotation::evidence()
{
    return *m_evidence;
}

qint16 Annotation::startIndex()
{
    return *m_startIndex;
}

qint16 Annotation::endIndex()
{
    return *m_endIndex;
}

void Annotation::setStatement(QString const& statement)
{
    *m_statement = statement;
}

void Annotation::setVerdict(qint16 const& verdict)
{
    *m_verdict = verdict;
}

void Annotation::setEvidence(QString const& evidence)
{
    *m_evidence = evidence;
}

void Annotation::setStartIndex(qint16 const& startIndex)
{
    *m_startIndex = startIndex;
}

void Annotation::setEndIndex(qint16 const& endIndex)
{
    *m_endIndex = endIndex;
}
