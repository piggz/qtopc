#include "opcvalue.h"

#include <QDebug>

#include "opc\opcda.h"
#include "opc\OPCClient.h"
#include "opc\OPCHost.h"
#include "opc\OPCServer.h"
#include "opc\OPCGroup.h"
#include "opc\OPCItem.h"


OPCValue::OPCValue(const QString &qmlName, const QString &opcName, const QString &opcType, QObject *parent) :
    QObject(parent)
{
    mQMLName = qmlName;
    mOPCName = opcName;

    setType(opcType);

    mOPCItem = 0;
    mData.vDataValue.iVal = 0;

}

OPCValue::~OPCValue()
{

}

QVariant OPCValue::value()
{
    USES_CONVERSION;

    CString err;
    mError = false;

    try {
        if (mOPCItem) {
            mOPCItem->readSync(mData,OPC_DS_CACHE);
        } else {
            qDebug() << "OPCItem not set";
        }
    } catch (const OPCException e) {
        mError = true;
        err = e.reasonString();

    }

    if (mError) {
        mErrorMessage = W2A(err);
        return QVariant(false);
    }

    switch (mType) {
    case Type::Boolean:
        return QVariant((bool)mData.vDataValue.boolVal);
    case Type::Int1:
        return QVariant(mData.vDataValue.bVal);
    case Type::Int2:
        return QVariant(mData.vDataValue.iVal);
    case Type::Int4:
        return QVariant(mData.vDataValue.intVal);
    case Type::UInt1:
        return QVariant(mData.vDataValue.bVal);
    case Type::UInt2:
        return QVariant(mData.vDataValue.uiVal);
    case Type::UInt4:
        return QVariant(mData.vDataValue.uintVal);
    case Type::Real4:
        return QVariant(mData.vDataValue.fltVal);
    case Type::Real8:
        return QVariant(mData.vDataValue.dblVal);
    case Type::Date:
        return QVariant(doubleToDateTime(mData.vDataValue.date));
    case Type::String:
        return QVariant(QString((QChar*)mData.vDataValue.bstrVal, ::SysStringLen(mData.vDataValue.bstrVal)));
    }
    return QVariant(0);
}

void OPCValue::setValue(QVariant v)
{
    mValue = v;
}

void OPCValue::setItem(COPCItem *c)
{
    mOPCItem = c;
}

QString OPCValue::qmlName()
{
    return mQMLName;
}

QString OPCValue::opcName()
{
    return mOPCName;
}

void OPCValue::setType(const QString &typ)
{
    if (typ.toLower() == "boolean") {
        mType = Type::Boolean;
    } else if (typ.toLower() == "uint1") {
        mType = Type::UInt1;
    } else if (typ.toLower() == "uint2") {
        mType = Type::UInt2;
    } else if (typ.toLower() == "uint4") {
        mType = Type::UInt4;
    } else if (typ.toLower() == "int1") {
        mType = Type::Int1;
    } else if (typ.toLower() == "int2") {
        mType = Type::Int2;
    } else if (typ.toLower() == "int4") {
        mType = Type::Int4;
    } else if (typ.toLower() == "real4") {
        mType = Type::Real4;
    } else if (typ.toLower() == "real8") {
        mType = Type::Real8;
    } else if (typ.toLower() == "date") {
        mType = Type::Date;
    } else if (typ.toLower() == "money") {
        mType = Type::Money;
    } else if (typ.toLower() == "string") {
        mType = Type::String;
    }
}

QDateTime OPCValue::doubleToDateTime(double dblDate)
{
    int days = qFloor(dblDate);
    QDate d= QDate(1900,1,1).addDays(days);

    double msecs = (dblDate - static_cast<double>(days)) * 86400.0 * 1000;
    QTime t = QTime(0,0,0,0).addMSecs(qRound(msecs));
    return QDateTime(d,t, Qt::UTC);
}

bool OPCValue::error()
{
    return mError;
}

QString OPCValue:: errorMessage()
{
    return mErrorMessage;
}
