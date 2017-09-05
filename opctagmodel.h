#ifndef OPCTAGMODEL_H
#define OPCTAGMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QObject>

#include "qtopc_global.h"

class QTOPCSHARED_EXPORT OPCTagModel : public QAbstractListModel
{
    Q_OBJECT
public:
    OPCTagModel();
    ~OPCTagModel();

    enum roles {
            TagName = Qt::UserRole + 1
    };
    Q_ENUM(roles)

    void setTags(const QStringList &tags);

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE int tagId(const QString &tag);
    Q_INVOKABLE QString tag(int id);

signals:
    void receivedTags();

private:
    QStringList m_tags;
};

#endif // OPCTAGMODEL_H
