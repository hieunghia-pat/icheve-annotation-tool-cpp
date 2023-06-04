#ifndef ANNOTATION_HPP
#define ANNOTATION_HPP

#include <QObject>
#include <QString>

#include <memory>

class Annotation : public QObject
{
    Q_OBJECT

public:
    explicit Annotation(QObject *parent = nullptr);
    Annotation(Annotation const& annotation, QObject *parent = nullptr);

    // getter
    QJsonObject annotation();
    QString statement();
    qint16 verdict();
    QString evidence();
    qint16 startIndex();
    qint16 endIndex();

    // setter
    void setStatement(QString const& statement);
    void setVerdict(qint16 const& verdict);
    void setEvidence(QString const& evidence);
    void setStartIndex(qint16 const& startIndex);
    void setEndIndex(qint16 const& endIndex);

signals:

private:
    std::shared_ptr<QString> m_statement = nullptr;
    std::shared_ptr<qint16> m_verdict = nullptr;
    std::shared_ptr<QString> m_evidence = nullptr;
    std::shared_ptr<qint16> m_startIndex = nullptr;
    std::shared_ptr<qint16> m_endIndex = nullptr;

};

#endif // ANNOTATION_HPP
