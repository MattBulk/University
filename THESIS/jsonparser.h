#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QObject>
#include <QVector>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QHash>

#include "validatejson.h"

/**
 * @brief qHash
 * @param key
 * @param seed
 * @return
 * \note in this way I can use my type of data as QHash
 */
inline uint qHash(const QJsonDocument &key, uint seed)
{
    return qHash(key.toJson(), seed);
}
/**
 * @brief The JSONParser class creates and maintains the database
 */
class JSONParser : public QObject
{
    Q_OBJECT
public:
    ~JSONParser();
    /**
     * @brief JSONParser main method
     * @param parent
     */
    explicit JSONParser(QObject *parent = nullptr);
    /**
     * @brief parseDataToFolder parses a big text file containing <TAG> separated json
     * @param Path path to the file
     * @param workingPath path to save the single json retrieved
     * @return bool
     */
    bool parseDataToFolder(const QString &Path, const QDir &workingPath);
    /**
     * @brief addElement validate the Json doc and insert it in the db
     * @param doc
     */
    void addElement(const QJsonDocument &doc);
    /**
     * @brief mainDB get method
     * @return a ref to the \_mainDB
     */
    const QHash<QJsonDocument, QJsonObject>& mainDB() const
    {
        return(this->_mainDB);
    }
    /**
     * @brief JSONParser::copyTheData
     * @param db
     * \note PIVOTAL: this will keep the imported data safe, and let me save and open the &db Vector to modify
     */
    void copyTheData(QVector<QJsonObject> &db);
    /**
     * @brief listMenuItems keeps the dynamically created list for the checkbox name
     * @return fer to the \a _listMenuItems
     */
    const QStringList& listMenuItems() const {

        return(this->_listMenuItems);
    }

signals:
    /**
     * @brief createWidgetMenuItemsElement send a signal to the mainWindow
     */
    void createWidgetMenuItemsElement();

public slots:
    /**
     * @brief initDataBase
     * @param path where to work
     * @return bool
     * \note if the database is already present the \a initDataBase \note is called by the \a Settings class
     */
    bool initDataBase(const QDir &path);

private:
    /**
     * @brief _counterProgression keep the filename system from falling into peaces
     */
    qint32 _counterProgression;
    /**
     * @brief _mainDB hold all the database
     */
    QHash<QJsonDocument, QJsonObject> _mainDB;
    /**
     * @brief _jsonDbVec
     */
    QVector<QJsonObject> _jsonDbVec;
    /**
     * @brief _theValidator pointer to the \a ValidateJson class
     */
    ValidateJson *_theValidator;

    QStringList _listMenuItems;

};

#endif // JSONPARSER_H
