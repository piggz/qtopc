#ifndef OPCVALUE_H
#define OPCVALUE_H
#undef min
#undef max

#define NOMINNAX
#include <QObject>
#include <QVariant>
#include <qmath.h>
#include <QDateTime>

#include "opc/opcda.h"
#include "opc/OPCClient.h"
#include "opc/OPCItemData.h"

class COPCItem;

class OPCValue : public QObject
{
    Q_OBJECT

public:
    explicit OPCValue(const QString &qmlName, const QString &opcName, const QString &opcType, QObject *parent = 0);
    ~OPCValue();

    enum Type {
        Boolean = 1,
        UInt1,
        UInt2,
        UInt4,
        Int1,
        Int2,
        Int4,
        Real4,
        Real8,
        Date,
        Money,
        String
    };

signals:

public slots:
    QVariant value();
    bool error();
    QString errorMessage();
    void setValue(QVariant v);
    void setItem(COPCItem* c);

    QString qmlName();
    QString opcName();

private:
    QString mQMLName;
    QString mOPCName;
    QVariant mValue;
    COPCItem *mOPCItem;
    OPCItemData mData;
    Type mType;
    bool mError;
    QString mErrorMessage;

    void setType(const QString &typ);
    QDateTime doubleToDateTime(double julian);
};

#endif // OPCVALUE_H
