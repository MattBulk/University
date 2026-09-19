#ifndef VISUALDATA_H
#define VISUALDATA_H

#include <QJsonObject>
#include <QObject>
#include <QVector>
#include <QString>
#include <QtCharts>
#include <QPair>
#include <QWidget>
#include <QHash>
#include <configurationparams.h>
#include <jsonparser.h>
/**
 * @brief The VisualData class visualizes the QtChart after ordering the data by occurences
 */
class VisualData : public QObject
{
    Q_OBJECT
public:
    ~VisualData();
    /**
     * @brief VisualData init method
     * @param parent
     */
    explicit VisualData(QObject *parent = nullptr, ConfigurationParams *myConf = nullptr);
    /**
     * @brief sortData
     * @param jObj the vector filtered
     * @param str the name of the field you want to orderBy
     * \note vector is sorted by the rule in the struct. After the counting of the occurences is performed
     */
    void sortData(const QVector<QJsonObject> &jObj, const QString &str);
    /**
     * @brief draw the QChart is [drawn here (to attract), double meaning phrasal verb]
     * @return pointer
     * \note the pointer is return to the QChartView in the main
     */
    QChartView *draw(bool sliceVisible);
    /**
     * @brief checkOccSize if vec size is
     * @param parent the window where the QMessageBox will be displayed
     * @return bool
     */
    bool checkOccSize(QWidget *parent);
    /**
     * @brief showAllSlicesLabels set visible to all the labels slices
     * @param chart
     * @param check
     */
    void showAllSlicesLabels(QChart *chart, bool check);
    /**
     * @brief jsonDocsIndex
     * @param name
     * @return Qvector
     * \note based on the key, it returns a vector containing all the indexes of the main filter vector.
     */
    QVector<qint16> jsonDocsIndex(const QString &name) const;
    /**
     * @brief jObjsExtended keep the filtered duplicates
     * @return ref
     */
    QVector<QJsonObject>& jObjsExtended()
    {
        return(this->_jObjsExtended);
    }
    /**
     * @brief sliceLegendPercentage
     * @return all the labels with the percentage
     */
    const QStringList& sliceLegendPercentage();

signals:
    /**
     * @brief emitItemsList
     * @param hashTable
     * \note send a ref to the table to fill it with data
     */
    void emitItemsList(const QHash<QString, QVector<qint16>> &hashTable, const QString &name);

private slots:
    /**
     * @brief sliceActionPressed show the current slice label
     */
    void sliceActionPressed();
    /**
     * @brief sliceActionReleased hides the current slice label
     */
    void sliceActionReleased();
    /**
     * @brief sliceActionDbClick fires the key filter
     */
    void sliceActionDbClick();

private:
    /**
     * @brief groupSliceToHashTable
     * @param name
     * @param value
     * @param clear
     */
    void groupSliceToHashTable(const QString &name, int value, bool clear = true);
    /**
     * @brief groupResultToChart group
     */
    QVector<QPair<QString, qint16>> *groupResultToChart();
    /**
     * @brief sendDataToTable
     * @param name
     * \emit the hash with a string and occurrences to the table
     */
    void sendDataToTable(const QString &name);

    QVector<QJsonObject> _jObjs;

    QVector<QJsonObject> _jObjsExtended;

    QString _mainVisualizedFilter;

    QVector<QPair<QString, qint16>> _occurrencesVec;

    QVector<QPair<QString, qint16>> _limitedOccuVec;
    /**
     * @brief _LIMIT_OCC_SIZE if higher the chart won't be shown
     */
    const int _LIMIT_OCC_SIZE = 35;

    QScopedPointer<ConfigurationParams> *_myConf;

    QScopedPointer<JSONParser> *_jsonParser;

    QHash<QString, QVector<qint16>> _sliceGroupingOcc;

    QStringList _othersList;

    QStringList _sliceChartPercentage;

};

#endif // VISUALDATA_H
