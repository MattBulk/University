#ifndef VALIDATEJSON_H
#define VALIDATEJSON_H

#include <QObject>
#include <QJsonObject>
#include <QSet>
#include <QStringList>
/**
 * @brief The ValidateJson class checks each json document
 */
class ValidateJson : public QObject
{
    Q_OBJECT
public: 
    ~ValidateJson();
    /**
     * @brief ValidateJson main method
     * @param parent
     */
    explicit ValidateJson(QObject *parent = nullptr);
    /**
     * @brief ckeckJsonDoc check each json in the database
     * @param obj
     * @return bool
     * \note it checks each doc assuming the content can be different in order or have more fields in it
     */
    bool ckeckJsonDoc(const QJsonObject &obj);
    /**
     * @brief itemsList
     * @param list
     * \note a QStringList is passed as ref and a sorted list is copied in it. This items are the checkbox names.
     */
    void itemsList(QStringList &list);

private:
    /**
     * @brief checkObj this is a subroutine for the \a checkJsonDoc method
     * @param obj the jsonDoc
     * @param listItem the string containing the name of the field object
     * @return
     */
    bool checkObj(const QJsonObject &obj, const QString &listItem);
    /**
     * @brief _jSonListItems this does the trick is a string is already present in the set a new one will be discarded
     */
    QSet<QString> _jSonListItems;
};

#endif // VALIDATEJSON_H
