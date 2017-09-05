#ifndef OPCCONTROLLER_H
#define OPCCONTROLLER_H

#include <QObject>
#include <QThread>
#include <QVariant>
#include <QStringList>
#include "qtopc_global.h"
#include "opctagmodel.h"

class OPCDataReceiver;
class OPCReader;

class QTOPCSHARED_EXPORT OPCController : public QObject
{
    Q_OBJECT

public:
    OPCController(QObject *parent = 0);
    OPCController(const QString & configFile, QObject *parent = 0);
    ~OPCController();
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY(QString connectedStatus READ connectedStatus NOTIFY connectedStatusChanged)

    void registerDataReceiver(OPCDataReceiver* receiver);

public slots:
    int dataReadTime();
    OPCTagModel* tagModel();
    void selectServer(const QString &opcHost, const QString &opcServer);
    void addTag(const QString &qmlName, const QString &opcName, const QString &opcType);
    void connectToServer();
    void disconnectFromServer();
    void registerTags();
    bool connected();
    QString connectedStatus();

signals:
    void readerConnectToServer();
    void readerDisconnectFromServer();
    void readerUpdateTags();
    void readerRegisterTags();
    void readerSelectServer(const QString &opcHost, const QString &opcServer);
    void readerAddTag(const QString &qmlName, const QString &opcName, const QString &opcType);
    void connectedChanged(bool c);
    void connectedStatusChanged(const QString &c);

private slots:
    void newTags(const QStringList&);
    void newData(const QMap<QString, QVariant>);
    void newReadTime(int);
    void serverTagList(const QStringList&);
    void readerConnectedChanged(bool c);
    void readerConnectedStatusChanged(const QString &c);

private:
    QThread readerThread;
    QString mOPCConfiguration;
    QList<OPCDataReceiver*> mReceivers;
    QStringList mTags;
    OPCTagModel *mTagModel;
    bool mReaderConnected;
    QString mReaderConnectedStatus;
    int mReadTime;

    void setupReader(OPCReader* reader);

};

#endif // OPCCONTROLLER_H
