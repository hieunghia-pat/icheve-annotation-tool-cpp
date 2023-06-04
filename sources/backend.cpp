#include "backend.hpp"
#include "sample.hpp"

#include <QUrl>
#include <QList>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QDebug>
#include <QVariant>

Backend::Backend(QObject* parent):
    QObject{parent}
{
    m_samples = QList<std::shared_ptr<Sample>>({
                                                   std::shared_ptr<Sample>(new Sample(parent))
                                               });
}

QJsonArray Backend::annotations(qint16 index)
{
    return m_samples[index]->annotations();
}

QString Backend::context()
{
    QString context = m_samples[*m_currentIdx]->context();
    if (*startIndex != *endIndex) {
        context = context.insert(*endIndex+1, QString("</strong>"));
        context = context.insert(*startIndex, QString("<strong style=\"color: #098f2c\">"));
    }

    QList<QString> passages = context.split("\n\n");
    for (int i = 0; i < passages.size(); i++) {
        passages[i] = QString("<p>") + passages[i] + QString("</p>");
    }

    context = passages.join("");
    return context;
}

bool Backend::loadData(QString path)
{
    path = QUrl(path).toLocalFile();
    QFile loadFile(path);
    if (!loadFile.open(QIODeviceBase::ReadOnly)) {
        emit openingFileError(QString("Cannot open file %1").arg(path));
        return false;
    }

    QJsonParseError jsonParseError;
    QJsonDocument document = QJsonDocument::fromJson(loadFile.readAll(), &jsonParseError);
    if (jsonParseError.error != QJsonParseError::NoError) {
        emit openingFileError(QString("Error while parsing json from file %1").arg(path));
        return false;
    }

    while (m_samples.size() > 0)
        m_samples.remove(0);
    QJsonArray jsonArray = document.array();
    for (QVariant const& object: jsonArray.toVariantList()) {
        QJsonObject jsonObject = object.toJsonObject();
        m_samples.append(std::shared_ptr<Sample>(new Sample(
                                                    jsonObject["context"].toString(),
                                                    jsonObject["information"].toArray(),
                                                    parent()))
                                                );
    }
    *m_selectedPath = path;
    emit contextChanged();
    emit loadedAnnotations(m_samples[*m_currentIdx]->annotations());

    return true;
}

void Backend::updateAnnotation(QJsonArray const& annotations)
{
    m_samples[*m_currentIdx]->setAnnotation(annotations);
}

void Backend::saveData()
{
    QJsonArray jsonArray;
    for (std::shared_ptr<Sample> const& sample: m_samples) {
        QJsonObject sampleJson = sample->toJsonObject();
        jsonArray.append(sampleJson);
    }
    QJsonDocument jsonDocument(jsonArray);

    QFile saveFile(*m_selectedPath, parent());
    if (!saveFile.open(QIODeviceBase::WriteOnly)) {
        emit savingFileError(QString("Cannot save data to %1").arg(*m_selectedPath));
        return;
    }
    saveFile.write(jsonDocument.toJson());
}

void Backend::nextAnnotation()
{
    if (*m_currentIdx == (m_samples.size() - 1))
        return;

    saveData();

    *m_currentIdx += 1;
    emit contextChanged();
    emit loadedAnnotations(m_samples[*m_currentIdx]->annotations());
}

void Backend::previousAnnotation()
{
    if (*m_currentIdx == 0)
        return;

    saveData();
    *m_currentIdx -= 1;
    emit contextChanged();
    emit loadedAnnotations(m_samples[*m_currentIdx]->annotations());
}

void Backend::onStartIndexChanged(const qint16 &index)
{
    *startIndex = index;
    emit contextChanged();
}

void Backend::onEndIndexChanged(const qint16 &index)
{
    *endIndex = index;
    emit contextChanged();
}
