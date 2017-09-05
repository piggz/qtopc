#ifndef OPCDATARECEIVER_H
#define OPCDATARECEIVER_H

#include <QStringList>
#include <QMap>
#include <QVariant>
#include "qtopc_global.h"

class QTOPCSHARED_EXPORT OPCDataReceiver
{
public:
    OPCDataReceiver();

    virtual void newData(QMap<QString, QVariant> data) = 0;
};

#endif // OPCDATARECEIVER_H
