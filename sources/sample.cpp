#include "sample.hpp"

#include "QJsonObject"
#include <QString>
#include <QVariant>

Sample::Sample(QObject *parent)
    : QObject{parent}, m_context{ std::shared_ptr<QString>(new QString("Context is not available")) },
      m_annotations{ QList<std::shared_ptr<Annotation>>({}) }
{

}

Sample::Sample(const QString &context, const QJsonArray &annotations, QObject* parent)
    : QObject{ parent }, m_context{ std::shared_ptr<QString>(new QString(context))}

{
    for (QVariant const& annotation: annotations.toVariantList()) {
        std::shared_ptr<Annotation> ann = std::shared_ptr<Annotation>(new Annotation(parent));
        QJsonObject jsonObject = annotation.toJsonObject();
        ann->setStatement(jsonObject["statement"].toString());
        ann->setVerdict(jsonObject["verdict"].toInt());
        ann->setEvidence(jsonObject["evidence"].toString());
        ann->setStartIndex(jsonObject["start"].toInt());
        ann->setEndIndex(jsonObject["end"].toInt());
        m_annotations.append(ann);
    }
}

QString Sample::context()
{
    return *m_context;
}

QJsonArray Sample::annotations()
{
    QJsonArray jsonArray;
    for (std::shared_ptr<Annotation> const& annotation: m_annotations) {
        QJsonObject jsonObject;
        jsonObject["statement"] = QJsonValue(annotation->statement());
        jsonObject["verdict"] = QJsonValue(annotation->verdict());
        jsonObject["evidence"] = QJsonValue(annotation->evidence());
        jsonObject["start"] = QJsonValue(annotation->startIndex());
        jsonObject["end"] = QJsonValue(annotation->endIndex());
        jsonArray.append(jsonObject);
    }

    return jsonArray;
}

QJsonObject Sample::toJsonObject()
{
    QJsonObject jsonObject;
    jsonObject["context"] = *m_context;
    jsonObject["information"] = annotations();

    return jsonObject;
}

void Sample::setContext(const QString &context)
{
    *m_context = context;
}

void Sample::setAnnotation(const QJsonArray &annotations)
{
    QList<std::shared_ptr<Annotation>> tmp_annotations = {};
    for (QVariant const& annotation: annotations.toVariantList()) {
        std::shared_ptr<Annotation> ann = std::shared_ptr<Annotation>(new Annotation(parent()));
        QJsonObject jsonObject = annotation.toJsonObject();
        ann->setStatement(jsonObject["statement"].toString());
        ann->setVerdict(jsonObject["verdict"].toInt());
        ann->setEvidence(jsonObject["evidence"].toString());
        ann->setStartIndex(jsonObject["start"].toInt());
        ann->setEndIndex(jsonObject["end"].toInt());
        tmp_annotations.append(ann);
    }

    m_annotations = tmp_annotations;
}
