#ifndef ANNOTATIONLIST_HPP
#define ANNOTATIONLIST_HPP

#include <QObject>
#include <QJsonArray>
#include <QList>

#include "annotation.hpp"

#include <memory>

class AnnotationList: public QObject
{
    Q_OBJECT
public:
    explicit AnnotationList(QObject* parent = nullptr);
    AnnotationList(QJsonArray const&, QObject* parent = nullptr);

    // getter
    QJsonArray annotations() const;
    qint16 size() const;
    qint16 length() const;
    std::shared_ptr<Annotation> operator[](qint16&) const;

    // setter
    void setAnnotations(QJsonArray const&);
    void insert(qint16, std::shared_ptr<Annotation> const&);
    void remove(qint16);

private:
    QList<std::shared_ptr<Annotation>> m_annotations = {};
};

#endif // ANNOTATIONLIST_HPP
