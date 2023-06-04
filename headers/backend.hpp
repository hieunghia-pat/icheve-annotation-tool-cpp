#ifndef BACKEND_HPP
#define BACKEND_HPP

#include "sample.hpp"

#include <QObject>
#include <QString>
#include <QStandardPaths>

#include <memory>

class Backend: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString context READ context NOTIFY contextChanged)

public:
    Backend(QObject* parent = nullptr);

    // getter
    QJsonArray annotations(qint16);
    QString context();

signals:
    void fileNotFound(QString const&);
    void openingFileError(QString const&);
    void openedFileSignal(QString const&);
    void loadedAnnotations(QJsonArray const&);
    void savingFileError(QString const&);
    void savedData();
    void contextChanged();

public slots:
    bool loadData(QString path);
    void updateAnnotation(QJsonArray const&);
    void saveData();
    void nextAnnotation();
    void previousAnnotation();
    void onStartIndexChanged(qint16 const&);
    void onEndIndexChanged(qint16 const&);

private:
    QList<std::shared_ptr<Sample>> m_samples = {};
    std::shared_ptr<qint16> m_currentIdx = std::shared_ptr<qint16>(new qint16(0));
    std::shared_ptr<QString> m_selectedPath = std::shared_ptr<QString>(new QString(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0]));
    // indices of evidence
    std::shared_ptr<qint16> startIndex = std::shared_ptr<qint16>(new qint16(0));
    std::shared_ptr<qint16> endIndex = std::shared_ptr<qint16>(new qint16(0));
};

#endif // BACKEND_HPP
