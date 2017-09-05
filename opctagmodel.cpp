#include "opctagmodel.h"
#include <QDebug>

OPCTagModel::OPCTagModel()
{

}

OPCTagModel::~OPCTagModel()
{

}

void OPCTagModel::setTags(const QStringList &tags)
{
    beginResetModel();
    m_tags = tags;
    endResetModel();

    emit receivedTags();
}

int OPCTagModel::rowCount(const QModelIndex &parent) const
{
    return m_tags.size();
}

QVariant OPCTagModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < rowCount(index) && index.row() >= 0 && role == TagName) {
        return m_tags[index.row()];
    }

    return QVariant();
}

QHash<int, QByteArray> OPCTagModel::roleNames() const
{
   QHash<int, QByteArray> roles;

   roles[TagName] = "tagname";

   return roles;
}

int OPCTagModel::tagId(const QString &tag)
{
    return m_tags.indexOf(tag);
}

QString OPCTagModel::tag(int id)
{
    return m_tags.value(id);
}
