#include "annotationlist.hpp"
#include "annotation.hpp"

#include <QVariant>
#include <QJsonObject>

#include <memory>

AnnotationList::AnnotationList(QObject *parent)
    : QObject{ parent }
{
    m_annotations = {std::shared_ptr<Annotation>(new Annotation(parent))};
}

AnnotationList::AnnotationList(const QJsonArray &annotations, QObject *parent)
    : QObject{ parent }
{
    for (QVariant const& ann: annotations.toVariantList()) {
        QJsonObject jsonObject = ann.toJsonObject();
        std::shared_ptr<Annotation> new_ann(new Annotation());
        new_ann->setStatement(jsonObject["statement"].toString());
        new_ann->setVerdict(jsonObject["verdict"].toInt());
        new_ann->setEvidence(jsonObject["evidence"].toString());
        new_ann->setStartIndex(jsonObject["start"].toInt());
        new_ann->setEndIndex(jsonObject["end"].toInt());
        m_annotations.append(new_ann);
    }
}

QJsonArray AnnotationList::annotations() const
{
    QJsonArray jsonArray;
    for (auto const& ann: m_annotations) {
        jsonArray.append(ann->annotation());
    }

    return jsonArray;
}

qint16 AnnotationList::size() const
{
    return m_annotations.size();
}

qint16 AnnotationList::length() const
{
    return m_annotations.size();
}

std::shared_ptr<Annotation> AnnotationList::operator[](qint16 &index) const
{
    return m_annotations[index];
}

void AnnotationList::setAnnotations(const QJsonArray &annotations)
{
    while (size() > 0)
        m_annotations.remove(0, 1);

    for (QVariant const& ann: annotations.toVariantList()) {
        QJsonObject jsonObject = ann.toJsonObject();
        std::shared_ptr<Annotation> new_ann(new Annotation());
        new_ann->setStatement(jsonObject["statement"].toString());
        new_ann->setVerdict(jsonObject["verdict"].toInt());
        new_ann->setEvidence(jsonObject["evidence"].toString());
        new_ann->setStartIndex(jsonObject["start"].toInt());
        new_ann->setEndIndex(jsonObject["end"].toInt());
        m_annotations.append(new_ann);
    }
}

void AnnotationList::insert(qint16 idx, const std::shared_ptr<Annotation> &annotation)
{
    m_annotations.insert(idx, annotation);
}

void AnnotationList::remove(qint16 idx)
{
    m_annotations.remove(idx);
}
