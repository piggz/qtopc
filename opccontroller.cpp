#include "opccontroller.h"
#include "opcreader.h"
#include "opcdatareceiver.h"

#include <QDebug>
#include <QStringList>

OPCController::OPCController(const QString &configFile, QObject *parent) :
    QObject(parent), mOPCConfiguration(configFile), mReadTime(0), mTagModel(new OPCTagModel()), mReaderConnected(false)
{
    OPCReader *opcReader = new OPCReader(mOPCConfiguration);

    setupReader(opcReader);

    emit readerConnectToServer();
}

OPCController::OPCController(QObject *parent):
    QObject(parent), mOPCConfiguration(QString()), mReadTime(0), mTagModel(new OPCTagModel())
{
    OPCReader *opcReader = new OPCReader();

    setupReader(opcReader);
}

void OPCController::setupReader(OPCReader *reader)
{
    mReceivers.clear();

    reader->moveToThread(&readerThread);

    //Connect the thread finish signal to opcReader deleteLater slot
    connect(&readerThread, &QThread::finished, reader, &QObject::deleteLater);

    //Connect the OPCController signals to OPCReader slots for communication
    connect(this, &OPCController::readerSelectServer, reader, &OPCReader::selectServer);
    connect(this, &OPCController::readerConnectToServer, reader, &OPCReader::connectServer);
    connect(this, &OPCController::readerDisconnectFromServer, reader, &OPCReader::disconnectServer);
    connect(this, &OPCController::readerUpdateTags, reader, &OPCReader::updateTags);
    connect(this, &OPCController::readerAddTag, reader, &OPCReader::addTag);
    connect(this, &OPCController::readerRegisterTags, reader, &OPCReader::registerTags);

    //Connect the OPCReader signals to OPCController slots
    connect(reader, &OPCReader::newTags, this, &OPCController::newTags);
    connect(reader, &OPCReader::newData, this, &OPCController::newData);
    connect(reader, &OPCReader::newReadTime, this, &OPCController::newReadTime);
    connect(reader, &OPCReader::serverTagList, this, &OPCController::serverTagList);
    connect(reader, &OPCReader::connectedChanged, this, &OPCController::readerConnectedChanged);
    connect(reader, &OPCReader::connectedStatusChanged, this, &OPCController::readerConnectedStatusChanged);

    readerThread.start();
}

OPCController::~OPCController()
{
    qDebug() << "Deleting controller thread";
    readerThread.quit();
    readerThread.wait(1000);
    qDebug() << "Done";
}

void OPCController::registerDataReceiver(OPCDataReceiver *receiver)
{
    if (!mReceivers.contains(receiver)) {
        mReceivers << receiver;
    }
}

void OPCController::newTags(const QStringList &tags)
{
    //qDebug() << "Got new tags" << tags;
    mTags << tags;
}

void OPCController::newData(QMap<QString, QVariant> data)
{
    foreach(OPCDataReceiver *receiver, mReceivers) {
        receiver->newData(data);
    }
}

void OPCController::newReadTime(int t)
{
    mReadTime = t;
}

int OPCController::dataReadTime()
{
    return mReadTime;
}

void OPCController::serverTagList(const QStringList &tags)
{
    //qDebug() << "Received tags " << tags;
    mTagModel->setTags(tags);
}

OPCTagModel* OPCController::tagModel()
{
    return mTagModel;
}

void OPCController::selectServer(const QString &opcHost, const QString &opcServer)
{
    emit readerSelectServer(opcHost, opcServer);
}

void OPCController::addTag(const QString &qmlName, const QString &opcName, const QString &opcType)
{
    emit readerAddTag(qmlName, opcName, opcType);
}

void OPCController::connectToServer()
{
    emit readerConnectToServer();
}

void OPCController::disconnectFromServer()
{
    emit readerDisconnectFromServer();
}

void OPCController::registerTags()
{
    emit readerRegisterTags();
}

void OPCController::readerConnectedChanged(bool c)
{
    if (c != mReaderConnected) {
        mReaderConnected = c;
        emit connectedChanged(c);
    }
}

bool OPCController::connected()
{
    return mReaderConnected;
}

void OPCController::readerConnectedStatusChanged(const QString &c)
{
    if (c != mReaderConnectedStatus) {
        mReaderConnectedStatus = c;
        emit connectedStatusChanged(mReaderConnectedStatus);
    }
}

QString OPCController::connectedStatus()
{
    return mReaderConnectedStatus;
}
