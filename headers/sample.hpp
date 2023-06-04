#ifndef SAMPLE_H
#define SAMPLE_H

#include <QObject>
#include <QString>
#include <QList>
#include <QJsonArray>
#include <QJsonObject>

#include "annotation.hpp"

#include <memory>

class Sample : public QObject
{
    Q_OBJECT
public:
    explicit Sample(QObject *parent = nullptr);
    Sample(QString const& context, QJsonArray const& annotations, QObject* parent = nullptr);

    // getter
    QString context();
    QJsonArray annotations();
    QJsonObject toJsonObject();

    // setter
    void setContext(QString const&);
    void setAnnotation(QJsonArray const&);

signals:

public slots:

private:
    std::shared_ptr<QString> m_context = nullptr;
    QList<std::shared_ptr<Annotation>> m_annotations = {};
};

#endif // SAMPLE_H
