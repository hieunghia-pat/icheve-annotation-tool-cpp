#ifndef ANNOTATIONMODEL_HPP
#define ANNOTATIONMODEL_HPP

#include <annotation.hpp>
#include <annotationlist.hpp>

#include <QObject>
#include <QAbstractListModel>
#include <QString>
#include <QList>
#include <QHash>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonArray>

#include <memory>

class AnnotationModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(qint16 selectedIndex READ selectedIndex WRITE setSelectedIndex NOTIFY selectedIndexChanged)

    enum ROLES {
        STATEMENT = Qt::UserRole,
        VERDICT = Qt::UserRole + 1,
        EVIDENCE = Qt::UserRole + 2,
        START = Qt::UserRole + 3,
        END = Qt::UserRole + 4
    };

public:
    explicit AnnotationModel(QObject *parent = nullptr);

    // getter
    qint16 selectedIndex();
    QList<std::shared_ptr<Annotation>> annotations();

    // setter
    void setSelectedIndex(qint16 const&);

    // derived methods
    int rowCount(QModelIndex const& parent = QModelIndex()) const;
    QVariant data(QModelIndex const& index, int role = Qt::DisplayRole) const;
    bool setData(QModelIndex const& index, QVariant const& value, int role = Qt::EditRole);
    bool insertRow(QModelIndex const&, QModelIndex const& parent = QModelIndex());
    bool removeRow(QModelIndex const&, QModelIndex const& parent = QModelIndex());
    Qt::ItemFlags flag(QModelIndex const& index) const;
    QHash<int, QByteArray> roleNames() const;

signals:
    void rowInsertedError(QString);
    void setStatementError(QString);
    void setVerdictError(QString);
    void setEvidenceError(QString);
    void annotationUpdated(QJsonArray const&);
    void selectedIndexChanged();
    void startedLoadingAnnotation();
    void finishedLoadingAnnotation();
    void startIndexChanged(qint16);
    void endIndexChanged(qint16);

public slots:
    bool addAnnotation(qint16 const& index);
    bool removeAnnotation(qint16 const& index);
    bool setAnnotations(QJsonArray const& item);

private:
    AnnotationList m_annotations;
    std::shared_ptr<qint16> m_selectedIndex = std::shared_ptr<qint16>(new qint16(0));
};

#endif // ANNOTATIONMODEL_HPP
