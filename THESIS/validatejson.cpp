#include "validatejson.h"

#include <QDebug>

ValidateJson::~ValidateJson()
{
    qDebug("ValidateJson::~ValidateJson()");
}

ValidateJson::ValidateJson(QObject *parent) : QObject(parent)
{

}

bool ValidateJson::ckeckJsonDoc(const QJsonObject &obj)
{
    QString listItem;
    for(int i = 0; i < obj.keys().size(); ++i) {

        listItem = obj.keys().at(i);
        if(obj.value(obj.keys().at(i)).isObject()) {
            //qDebug("is object");
            this->checkObj(obj.value(obj.keys().at(i)).toObject(), listItem);
        }
        else if(obj.value(obj.keys().at(i)).isArray()) {
            qDebug("is Array");
        }
        else if(obj.value(obj.keys().at(i)).isNull()) {
            qDebug("is Null");
        }
        else if(obj.value(obj.keys().at(i)).isUndefined()) {
            qDebug("is Undefined");
        }
    }
    return(true);
}

void ValidateJson::itemsList(QStringList &list)
{
    list.clear();
    list = this->_jSonListItems.toList();
    std::sort(list.begin(), list.end());
}

bool ValidateJson::checkObj(const QJsonObject &obj, const QString &listItem)
{
    QJsonObject reiObj = obj;
    QJsonObject::iterator it;
    for(it = reiObj.begin(); it != reiObj.end(); ++it) {
        QString itemName = QString("%1.%2").arg(listItem, it.key());
        this->_jSonListItems.insert(itemName);
    }
    return(true);
}
