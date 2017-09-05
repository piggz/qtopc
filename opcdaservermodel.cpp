#include "opcdaservermodel.h"

#include <QDebug>
#include "opc\opcda.h"
#include "opc\OPCClient.h"
#include "opc\OPCHost.h"

OPCDAServerModel::OPCDAServerModel(const QString &host)
{
    setHost(host);
}

OPCDAServerModel::~OPCDAServerModel()
{

}

void OPCDAServerModel::setHost(const QString &host)
{
    mHost = host;
    connectHost();
}

int OPCDAServerModel::rowCount(const QModelIndex &parent) const
{
    return mServerList.size();
}

QVariant OPCDAServerModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < rowCount(index) && index.row() >= 0 && role == ServerName) {
        return mServerList[index.row()];
    }

    return QVariant();
}

QHash<int, QByteArray> OPCDAServerModel::roleNames() const
{
   QHash<int, QByteArray> roles;

   roles[ServerName] = "servername";

   return roles;
}

void OPCDAServerModel::connectHost()
{
    USES_CONVERSION;
    COPCHost *OPCHost;
    CAtlArray<CString> serverList;

    beginResetModel();
    //Connect to the host
    COPCClient::init();
    OPCHost = COPCClient::makeHost(mHost.toLocal8Bit().data());

    mServerList.clear();

    //Get the list of OPC servers
    OPCHost->getListOfDAServers(IID_CATID_OPCDAServer20, serverList);

    qDebug() << "No of servers:" << serverList.GetCount();
    for (unsigned int i = 0; i < serverList.GetCount(); i++){
        qDebug() << W2A(serverList[i]);
        mServerList.append(W2A(serverList[i]));
    }
    endResetModel();

    COPCClient::stop();
}
