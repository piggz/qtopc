#ifndef OPCDASERVERMODEL_H
#define OPCDASERVERMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include "qtopc_global.h"

class QTOPCSHARED_EXPORT OPCDAServerModel : public QAbstractListModel
{
    Q_OBJECT
public:
    OPCDAServerModel(const QString &host = QString());
    ~OPCDAServerModel();

    enum roles {
            ServerName = Qt::UserRole + 1
    };
    Q_ENUM(roles)

    Q_INVOKABLE void setHost(const QString &host);

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

private:
    QString mHost;
    QStringList mServerList;

    void connectHost();
};

#endif // OPCDASERVERMODEL_H
