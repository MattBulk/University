#ifndef FILTERDATA_H
#define FILTERDATA_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QJsonObject>
#include <QHash>
#include <QStringList>
#include <QList>
#include <seeker.h>
/**
 * @brief The FilterData class provides the methods for filtering the database
 */
class FilterData : public QObject
{
    Q_OBJECT
public:
    ~FilterData();
    /**
     * @brief FilterData is the main method
     * @param parent
     */
    explicit FilterData(QObject *parent = nullptr);
    /**
     * @brief addFilter adds the name of the selected button to the \a _filterVec
     * @param filter the QString collected from the checkbox
     */
    void addFilter(const QString &filter);
    /**
     * @brief removeFilter removes the name of the selected button to the \a _filterVec
     * @param filter adds the name of the selected button to the \a _filterVec
     */
    void removeFilter(const QString &filter);
    /**
     * @brief workTheData copies the filtered data to a QVector
     * @param mainKeyFilter this is the key to group the data
     * @return a QVector of QJsonObject
     * \note this method takes the filtered and splices each string to find the keys useful for retrieving the JsonDocs.
     * After the subroutine \a filterObjKeys \note finishes a QVector returning simple jsonDocs containing key.
     * The subroutine is launched with QFuture.
     */
    QVector<QJsonObject> workTheData(const QString &masterKey, bool group=false);
    /**
     * @brief clearFilters clear the QVector
     */
    inline void clearFilters() { this->_filterVec.clear(); }
    /**
     * @brief filterVec
     * @return the list of the filter applied
     */
    const QVector<QString>& filterVec() const
    {
        return(this->_filterVec);
    }
    /**
     * @brief jsonDbVec
     * @return the QVector used as a copy placed for the mainDB
     */
    QVector<QJsonObject>& jsonDbVec()
    {
        return(this->_jsonDbVec);
    }
    /**
     * @brief masterKeyFilteredDb
     * @return QHash
     * \note return a reference to the hash table
     */
    const QHash<QString, QList<QJsonObject>>& masterKeyFilteredDb() const
    {
        return(this->_masterKeyFilteredDb);
    }
    /**
     * @brief workTheQuery
     * @param query
     * @param listItems
     * @return bool
     * \note this method launch the seeker parser I wrote
     */
    bool workTheQuery(const QString &query, const QStringList &listItems);

private:
    /**
     * @brief groupDataByMasterFilter group the json object by the masterkey
     */
    void groupDataByMasterFilter();
    /**
     * @brief filterObjKeys create a smaller QVector of json ready to be sorted
     * @param jdb
     * @param strlist has the key for sorting value
     * @param fillObj a Qvector to be filled with small QJsonObject
     * \note the method takes the string I previously separated; it looks up the db for the filter field and return the object with the value.
     * After that, a json doc is created and inserted in the \a fillObj \note QVector
     */
    void filterObjKeys(const QVector<QJsonObject> &jdb, const QStringList &strlist, QVector<QJsonObject> &fillObj);
    /**
     * @brief _filterVec holds the filter list selected by the user
     */
    QVector<QString> _filterVec;
    /**
     * @brief _jsonDbVec keep a copy of the database
     */
    QVector<QJsonObject> _jsonDbVec;
    /**
     * @brief _keyFilteredDb holds the database as Qhash table based on the combobox filter
     */
    QHash<QString, QList<QJsonObject>> _masterKeyFilteredDb;

    QStringList _masterKeyList;

};

#endif // FILTERDATA_H
