#include "filterdata.h"
#include <QDebug>
#include <QStringList>
#include <QFuture>
#include <QtConcurrent>
#include <functional>
#include <QMessageBox>

FilterData::~FilterData()
{
    qDebug("FilterData::~FilterData()");
}

FilterData::FilterData(QObject *parent) : QObject(parent)
{

}

void FilterData::addFilter(const QString &filter)
{
    this->_filterVec.push_back(filter);
}

void FilterData::removeFilter(const QString &filter)
{
    if(this->_filterVec.contains(filter))
        this->_filterVec.remove(_filterVec.indexOf(filter));
}

void FilterData::filterObjKeys(const QVector<QJsonObject> &jdb, const QStringList &strlist, QVector<QJsonObject> &fillObj)
{
    for(int i = 0; i < jdb.size(); ++i) {
        if(jdb.at(i).contains(strlist.at(0))) {
            QJsonObject reiObj = jdb.at(i)[strlist.at(0)].toObject();
            QString uniqueKey = QString("%1.%2").arg(strlist.at(0), strlist.at(1));
            // I can use array[] because I know the index is always correct
            fillObj[i].insert(uniqueKey, reiObj.value(strlist.at(1)));
            // ------------------------------------------------------------------------------
            // QJsonObject licenseObj = jdb.at(i).value("ReportProgramInfoValue").toObject();
            // //copy the key as unique key
            // QString licenseKey = licenseObj.value("LicenseKey").toString();
            // fillObj[i].insert("key", licenseKey);
            // ------------------------------------------------------------------------------
            QJsonObject tempObj = jdb.at(i).value(this->_masterKeyList.at(0)).toObject();
            fillObj[i].insert("masterKey", tempObj.value(this->_masterKeyList.at(1)));
            fillObj[i].insert("indexRef", i);
        }
    }
}

QVector<QJsonObject> FilterData::workTheData(const QString &masterKey, bool group)
{
    this->_masterKeyList = masterKey.split('.');
    if(group) this->groupDataByMasterFilter();
    /*
     * OPT: split every filter string into a Stringlist so that I can filter throghout the jsonObjectDb
     * QString neededWord = pieces.value( pieces.length() - n );
     *
     * OPT: remove key from the QJsonObject based on the filters !! as usual remove data from a database always a bad idea !!!!
     * OPT: create a new QJsonObject based on the filtes ---->>>>>> easiest way
     * in both cases I have the mainBD if a fresh copy is needed
     */
    QVector<QStringList> strLiVec;
    // a new QVector holding the same size of the _jsonDbVec
    QVector<QJsonObject> filteredObjVec(this->_jsonDbVec.size());
    for(int i = 0; i < this->_filterVec.size(); ++i) {
        QStringList pieces = this->_filterVec.at(i).split('.');
        strLiVec.push_back(pieces);
    }
    for(int j = 0; j < strLiVec.size(); ++j) {
        /*
         * std::ref lets me force QtConcurrent pass the array for reference ! since C++11 !
         * https://stackoverflow.com/questions/25091518/qt-concurrent-run-pass-value-by-reference-but-the-memory-address-is-different
         */
        QFuture<void> filtered = QtConcurrent::run(this, &FilterData::filterObjKeys, this->_jsonDbVec, strLiVec.at(j), std::ref(filteredObjVec));
        // comment the line to work on threads at the same time --> if commented a strange bug came out with the limiter I set up
        filtered.waitForFinished();
    }
    return(filteredObjVec);
}

bool FilterData::workTheQuery(const QString &query, const QStringList &listItems)
{
    Seeker theSeeker = Seeker(listItems);
    bool syntaxResult;
    QVector<QJsonObject> tempVect;
    QWidget *mainWin = qobject_cast<QWidget*>(this->parent());
    for (int i = 0; i < this->_jsonDbVec.size(); ++i) {
        theSeeker.addDoc(this->_jsonDbVec.at(i));
        syntaxResult = theSeeker.search(query);
        if(!syntaxResult) {
            QMessageBox::critical(mainWin, tr("Syntax error"), tr("You encounted a syntax error, make sure you inserted the correct query"));
            return false;
        }
        if(theSeeker.result() == "quit") {
            QMessageBox::critical(mainWin, tr("Parsing error"), tr("You have tried to evaluate unmatchable types"));
            return false;
        }
        else if(theSeeker.result() == "true") {
            tempVect.append(this->_jsonDbVec.at(i));
        }
        if(i == this->_jsonDbVec.size()-1 && tempVect.size() > 0) {
            this->_jsonDbVec = tempVect;
            return true;
        }
    }
    if(tempVect.size() == 0) {
        QMessageBox::critical(mainWin, tr("No results"), tr("no matches have been found for the current query"));
        return false;
    }
    return false;
}

void FilterData::groupDataByMasterFilter()
{
    this->_masterKeyFilteredDb.clear();
    QStringList pieces = this->_masterKeyList;
    for(int i = 0; i < this->_jsonDbVec.size(); ++i) {
        QJsonObject currentObj = this->_jsonDbVec.at(i)[pieces.at(0)].toObject();
        QString currentKey;
        //copy the key as unique key
        if(currentObj.value(pieces.at(1)).isString())
            currentKey = currentObj.value(pieces.at(1)).toString();
        else if(currentObj.value(pieces.at(1)).isDouble())
            currentKey = QString::number(currentObj.value(pieces.at(1)).toDouble());
        if(this->_masterKeyFilteredDb.contains(currentKey)) {
            this->_masterKeyFilteredDb[currentKey].push_back(this->_jsonDbVec.at(i));
        }
        else {
            QList<QJsonObject> list;
            list.push_back(this->_jsonDbVec.at(i));
            this->_masterKeyFilteredDb.insert(currentKey, list);
        }
    }
    this->_jsonDbVec.clear();
    QHashIterator<QString, QList<QJsonObject>> it(this->_masterKeyFilteredDb);
    while(it.hasNext()) {
        it.next();
        this->_jsonDbVec.push_back(it.value().at(0));
    }
}
