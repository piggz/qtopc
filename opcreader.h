#ifndef OPCREADER_H
#define OPCREADER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QPair>

#include "opc\opcda.h"
#include "opc\OPCClient.h"
#include "opc\OPCHost.h"
#include "opc\OPCServer.h"
#include "opc\OPCGroup.h"
#include "opc\OPCItem.h"
#include <sys\timeb.h>
#include "opcvalue.h"

class QQmlContext;

class OPCReader : public QObject
{
    Q_OBJECT
public:
    explicit OPCReader(const QString &configurationFile, QObject *parent = 0);
    OPCReader(QObject *parent = 0);

public slots:
    void selectServer(const QString &opcHost, const QString &opcServer);
    void addTag(const QString &qmlName, const QString &opcName, const QString &opcType);
    void connectServer();
    void disconnectServer();
    void updateTags();
    void registerTags();

signals:
    void newData(QMap<QString, QVariant>);
    void newTags(const QStringList& tag);
    void newReadTime(int);
    void serverTagList(const QStringList& tags);
    void connectedChanged(bool c);
    void connectedStatusChanged(const QString &c);

private:
    void readConfiguration();
    void connectToServer();


    QString mOPCConfiguration;
    QString mHostName;
    QString mOPCServerName;
    QHash<QString, OPCValue*> mOPCItems;

    COPCHost *mOPCHost;
    COPCServer *mOPCServer;
    CAtlArray<CString> mLocalServerList;
    COPCGroup *mGroup;

    bool mError;
    QString mErrorString;

    int mReadTime;
};

#endif // OPCREADER_H
