#include "annotationmodel.hpp"

#include <QDebug>
#include <QVariant>

#include <memory>

AnnotationModel::AnnotationModel(QObject *parent)
    : QAbstractListModel{parent}, m_annotations{ parent }
{

}

qint16 AnnotationModel::selectedIndex()
{
    return *m_selectedIndex;
}

void AnnotationModel::setSelectedIndex(qint16 const& index)
{
    *m_selectedIndex = index;
    emit selectedIndexChanged();
    emit startIndexChanged(m_annotations[*m_selectedIndex]->startIndex());
    emit endIndexChanged(m_annotations[*m_selectedIndex]->endIndex());
}

int AnnotationModel::rowCount(const QModelIndex&) const
{
    return m_annotations.length();
}

QVariant AnnotationModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    qint16 row = index.row();

    switch (role) {
        case STATEMENT:
            return QVariant(m_annotations[row]->statement());
        case VERDICT:
            return QVariant(m_annotations[row]->verdict());
        case EVIDENCE:
            return QVariant(m_annotations[row]->evidence());
        case START:
            return QVariant(m_annotations[row]->startIndex());
        case END:
            return QVariant(m_annotations[row]->endIndex());
        default:
            return QVariant();
    }
}

bool AnnotationModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

    if (!index.isValid()) {
        return false;
    }

    qint16 row = index.row();

    switch (role) {
        case ROLES::STATEMENT:
            m_annotations[row]->setStatement(value.toString());
            emit dataChanged(index, index, {role, Qt::EditRole, Qt::DisplayRole});
            emit annotationUpdated(m_annotations.annotations());
            return true;
        case ROLES::VERDICT:
            m_annotations[row]->setVerdict(value.toInt());
            emit dataChanged(index, index, {role, Qt::EditRole, Qt::DisplayRole});
            emit annotationUpdated(m_annotations.annotations());
            return true;
        case ROLES::EVIDENCE:
            m_annotations[row]->setEvidence(value.toString());
            emit dataChanged(index, index, {role, Qt::EditRole, Qt::DisplayRole});
            emit annotationUpdated(m_annotations.annotations());
            return true;
        case ROLES::START:
            m_annotations[row]->setStartIndex(value.toInt());
            emit dataChanged(index, index, {role, Qt::EditRole, Qt::DisplayRole});
            emit annotationUpdated(m_annotations.annotations());
            emit return true;
        case ROLES::END:
            m_annotations[row]->setEndIndex(value.toInt());
            emit dataChanged(index, index, {role, Qt::EditRole, Qt::DisplayRole});
            emit annotationUpdated(m_annotations.annotations());
            return true;
        default:
            return false;
    }
}

bool AnnotationModel::insertRow(QModelIndex const& index, const QModelIndex &parent)
{
    if (!index.isValid())
        return false;

    qint16 row = index.row();
    beginInsertRows(parent, row, row);
    m_annotations.insert(row, std::shared_ptr<Annotation>(new Annotation()));
    endInsertRows();

    return true;
}

bool AnnotationModel::removeRow(QModelIndex const& index, const QModelIndex &parent)
{
    if (!index.isValid())
        return false;

    qint16 row = index.row();
    beginRemoveRows(parent, row, row);
    m_annotations.remove(row);
    endRemoveRows();

    return true;
}

Qt::ItemFlags AnnotationModel::flag(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return index.flags() | Qt::ItemIsEditable;
}

QHash<int, QByteArray> AnnotationModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ROLES::STATEMENT] = QString("statement").toUtf8();
    roles[ROLES::VERDICT] = QString("verdict").toUtf8();
    roles[ROLES::EVIDENCE] = QString("evidence").toUtf8();
    roles[ROLES::START] = QString("start").toUtf8();
    roles[ROLES::END] = QString("end").toUtf8();

    return roles;
}

bool AnnotationModel::addAnnotation(const qint16 &index)
{
    return insertRow(this->index(index+1));
}

bool AnnotationModel::removeAnnotation(const qint16 &index)
{
    return removeRow(this->index(index));
}

bool AnnotationModel::setAnnotations(const QJsonArray &annotations)
{
    emit startedLoadingAnnotation();

    // clear all previous annotations
    while (rowCount() > 0)
        removeRow(index(0));

    // adding new annotations
    qint16 ith = 0;
    for (QVariant const& annotation: annotations.toVariantList()) {
        QJsonObject jsonObject = annotation.toJsonObject();
        QModelIndex idx = createIndex(ith, 0);

        bool insertedRow = insertRow(idx);
        if (!insertedRow) {
            emit rowInsertedError(QString("Cannot insert new annotation at %1").arg(ith));
            return false;
        }

        bool setStatement = setData(idx, jsonObject["statement"].toVariant(), ROLES::STATEMENT);
        if (!setStatement) {
            emit setStatementError(QString("Cannot set statement for the %1ith annotation").arg(ith));
            return false;
        }

        bool setVerdict = setData(idx, jsonObject["verdict"].toVariant(), ROLES::VERDICT);
        if (!setVerdict) {
            emit setVerdictError(QString("Cannot set verdict for the {}ith annotation").arg(ith));
            return false;
        }

        bool setEvidence = setData(idx, jsonObject["evidence"].toVariant(), ROLES::EVIDENCE);
        if (!setEvidence) {
            emit setEvidenceError(QString("Cannot set evidence for the {}ith annotation").arg(ith));
            return false;
        }

        bool setStartIndex = setData(idx, jsonObject["start"].toVariant(), ROLES::START);
        if (!setStartIndex) {
//            emit setStatementError(QString("Cannot set starting index for the {}ith annotation").arg(ith));
            return false;
        }

        bool setEndIndex = setData(idx, jsonObject["end"].toVariant(), ROLES::END);
        if (!setEndIndex) {
//            emit setStatementError(QString("Cannot set ending index for the {}ith annotation").arg(ith));
            return false;
        }

        ith += 1;
    }

    emit finishedLoadingAnnotation();

    return true;
}
