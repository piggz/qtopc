#include "opcreader.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>

OPCReader::OPCReader(const QString &configurationFile, QObject *parent) :
    QObject(parent), mGroup(0), mOPCServer(0), mOPCHost(0), mOPCConfiguration(configurationFile), mReadTime(0)
{
    readConfiguration();
}

OPCReader::OPCReader(QObject *parent) :
    QObject(parent), mGroup(0), mOPCServer(0), mOPCHost(0), mReadTime(0)
{
}

void OPCReader::selectServer(const QString &opcHost, const QString &opcServer)
{
    USES_CONVERSION;

    qDebug() << "Selecting server: " << opcHost << ":" << opcServer;

    COPCClient::init();
    mHostName = opcHost;
    mOPCServerName = opcServer;

    //Clear server list
    mLocalServerList.RemoveAll();

    qDebug() << "Connecting to selected server";

    if (mOPCServerName.isEmpty()) {
        qDebug() << "OPC Server Name is not set";
        return;
    }

    //Connect to the host

    mOPCHost = COPCClient::makeHost(mHostName.toLocal8Bit().data());

    //Get the list of OPC servers
    mOPCHost->getListOfDAServers(IID_CATID_OPCDAServer20, mLocalServerList);

    qDebug() << "No of servers:" << mLocalServerList.GetCount();
    for (unsigned int i = 0; i < mLocalServerList.GetCount(); i++){
        qDebug() << W2A(mLocalServerList[i]);
    }
}

void OPCReader::connectServer()
{
    USES_CONVERSION;
    CString err;
    QStringList serverTags;
    ServerStatus status;

    mError = false;

    qDebug() << "connetServer";

    //Check if already connected and abort
    if (mOPCServer) {
        mOPCServer->getStatus(status);
        qDebug() << "Server state is " << status.dwServerState;

        if (status.dwServerState == OPC_STATUS_RUNNING) {
            qDebug() << "Not connecting to already connected server!";
            return;
        }
    }

    if (!mOPCHost) {
        qDebug() << "Not connected to host.  Is host/server set?";
        return;
    }

    qDebug() << "connetServer:trying connect";
    //Connect to the server    
    try {
        mOPCServer = mOPCHost->connectDAServer(mOPCServerName.toLocal8Bit().data());
        qDebug() << "connected";

        mOPCServer->getStatus(status);
        qDebug() << "Server state is " << status.dwServerState;

        // browse server
        CAtlArray<CString> opcItemNames;
        mOPCServer->getItemNames(opcItemNames);
        for (unsigned int i = 0; i < opcItemNames.GetCount(); i++){
            serverTags << W2A(opcItemNames[i]);
        }
        //qDebug() << "Server contains the following tags: " << serverTags;
        emit serverTagList(serverTags);
    } catch (const OPCException e) {
        mError = true;
        err = e.reasonString();
    }

    if (mError) {
        mErrorString = W2A(err);
        qDebug() << "Error Connecting: " << mErrorString;
        emit connectedChanged(false);
        emit connectedStatusChanged(mErrorString);
    } else {
        emit connectedChanged(true);
        emit connectedStatusChanged("Connected");
    }
    qDebug() << "connetServer:finished";
}

void OPCReader::disconnectServer()
{
    ServerStatus status;

    if (mOPCServer) {
        mOPCServer->getStatus(status);
        qDebug() << "Server state is " << status.dwServerState;

        delete mGroup;
        mGroup = 0;

        delete mOPCServer;
        mOPCServer = 0;

        //mOPCItems.clear();
        emit connectedChanged(false);
        emit connectedStatusChanged("Disconnected");
    }
}

void OPCReader::registerTags()
{
    USES_CONVERSION;
    qDebug() << "Registering Tags";

    CString err;

    if (!mOPCServer) {
        qDebug() << "Error - No OPC Server Connection";
        return;
    }

    qDebug() << "Creating group";
    //create an item group
    unsigned long refreshRate;
    try {
        mGroup = mOPCServer->makeGroup("Group" , true, 1000, refreshRate, 0.0);
    } catch (const OPCException e) {
        err = e.reasonString();
    }
    QString errorString = W2A(err);

    qDebug() << errorString;

    OPCValue *opcItem;
    COPCItem *cItem;

    QStringList qmlItems;

    qDebug() << "Processing items";
    foreach(opcItem, mOPCItems) {
        //qDebug() << opcItem->opcName();
        ATL::CString iname = opcItem->opcName().toLocal8Bit().data();
        CString err;
        try {
            cItem = mGroup->addItem(iname, true);
            opcItem->setItem(cItem);
            qmlItems << opcItem->qmlName();
        } catch (const OPCException e) {
            err = e.reasonString();
        }
        QString qErr = W2A(err);
        if (!qErr.isEmpty()) {
            qDebug() << "Error:" << opcItem->opcName() << qErr;
        }
    }
    qDebug() << "emitting";
    emit(newTags(qmlItems));
}

void OPCReader::updateTags()
{
    //qDebug() << "Update tags";
    QTime t;
    t.start();
    OPCValue *opcItem;
    QMap<QString, QVariant> itemValues;
    bool errorOccurred = false;

    if (!mOPCServer) {
        return;
    }

    foreach(opcItem, mOPCItems) {
        QVariant value = opcItem->value();
        if (opcItem->error()) {
            errorOccurred = true;
            qDebug() << opcItem->qmlName() << opcItem->errorMessage();
        }
        itemValues[opcItem->qmlName()] = value;
    }

    if (errorOccurred) {
        connectServer();
    }

    if (mReadTime != t.elapsed()) {
        mReadTime = t.elapsed();
        emit newReadTime(mReadTime);
    }
    emit newData(itemValues);
}

void OPCReader::readConfiguration() {
    QFile config(mOPCConfiguration);

    if (!config.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&config);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList lineList = line.split(";");

        if (lineList[0].toLower().trimmed() == "opchost") {
            mHostName = lineList[1];
        }
        if (lineList[0].toLower().trimmed() == "opcserver") {
            mOPCServerName = lineList[1];
        }
        if (lineList[0].toLower().trimmed() == "opcitem") {
            QStringList opcList = lineList[1].split("|");
            addTag(opcList[0], opcList[1], opcList[2]);
        }
    }

    qDebug() << mHostName << mOPCServerName;
    OPCValue *opc;
    foreach (opc, mOPCItems) {
        qDebug() << opc->qmlName() << opc->opcName();
    }
}

void OPCReader::addTag(const QString &qmlName, const QString &opcName, const QString &opcType)
{
    qDebug() << "Adding Tag:" << qmlName << opcName << opcType;

    OPCValue *opcItem = new OPCValue(qmlName, opcName, opcType);

    mOPCItems[qmlName] = opcItem;
}
