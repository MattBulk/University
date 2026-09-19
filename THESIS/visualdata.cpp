#include "visualdata.h"

#include <algorithm>
#include <QPieSeries>
#include <QPieSlice>
#include <QMessageBox>
#include <QWidget>
#include <QPieLegendMarker>
#include <QLegendMarker>
#include <QHashIterator>

VisualData::~VisualData()
{
    qDebug("VisualData::~VisualData()");
}

VisualData::VisualData(QObject *parent, ConfigurationParams *myConf) : QObject(parent)
{
    this->_myConf = new QScopedPointer<ConfigurationParams>(myConf);
}

// TO BE REWRITTEN.... DAMNED IT OR TRY A TEMPLATE FUNCTION ... IT MAY WORK
void VisualData::sortData(const QVector<QJsonObject> &jObjs, const QString &str)
{
    this->_jObjs = jObjs;
    struct {
        QString str;
        char type;
        bool operator()(const QJsonObject& a, const QJsonObject& b) const {
            if(type == 'S') {
                QString v_a = a.value(str).toString();
                QString v_b = b.value(str).toString();
                return(v_a < v_b);
            }
            else if(type == 'D') {
                double v_a = a.value(str).toDouble();
                double v_b = b.value(str).toDouble();
                return(v_a < v_b);
            }
        return(false);
        }
    } fn_compare;
    fn_compare.str = str;
    if(_jObjs.at(0).value(str).isDouble())
        fn_compare.type = 'D';
    else if(_jObjs.at(0).value(str).isString())
        fn_compare.type = 'S';

    std::sort(_jObjs.begin(), _jObjs.end(), fn_compare);

    QJsonObject dummyObj;
    _jObjs.push_back(dummyObj);
    // the filter to be visualized
    this->_mainVisualizedFilter = str;
    this->_occurrencesVec.clear();
    qint16 count = 1;
    QPair<QString, qint16> pair;
    for(int i = 0; i < _jObjs.size()-1; ++i) {
        if(count == 1) {
            if(fn_compare.type == 'D') {
                double value = _jObjs.at(i).value(str).toDouble();
                pair.first = QString::number(value);
            }
            else {
                pair.first = _jObjs.at(i).value(str).toString();
            }
        }
        if(fn_compare.type == 'S' && _jObjs.at(i).value(str).toString() == _jObjs.at(i+1).value(str).toString()) {
            count++;
        }
        else if(fn_compare.type == 'D' && _jObjs.at(i).value(str).toDouble() == _jObjs.at(i+1).value(str).toDouble()) {
            count++;
        }
        else {
            pair.second = count;
            this->_occurrencesVec.push_back(pair);
            count = 1;
        }
    }
}

// void linearOccurrences(QVector<T> vec)
// {
//     vec.push_back("NOMATCH");
//     int count = 1;
//     for (int i = 0; i < vec.size()-1; ++i) {
//         if(count == 1) qDebug() << vec.at(i) << ':';
//      // here I can modify the occurrences mathing the last item ... too much fuzz, dummy object makes the code easier.
//      // if(i == vec.size()-1) {
//      //     qDebug() << "end of the vec";
//      //     break;
//      // }
//         if(vec.at(i) == vec.at(i+1)) count++;
//         else {
//             qDebug() << "number of occurrences: " << count;
//             count = 1;
//         }
//     }
// }

//virtual QString prettyName() const {
//        QString name = this->name().replace(QRegularExpression("([A-Z])"), " \\1").replace("_", " ");
//        QStringList list = name.split(QChar(' '), QString::SkipEmptyParts);
//        list.removeAll("S");
//        list.removeAll("Meta");
//        return(list.join(QChar(' ')));
//    }

QChartView* VisualData::draw(bool sliceVisible)
{
    QPieSeries *series = new QPieSeries();
    QPieSlice *slice;
    QString keyValue;
    qint32 count;
    for(int k = 0; k < this->groupResultToChart()->size(); ++k) {
        keyValue = this->groupResultToChart()->at(k).first;
        count = this->groupResultToChart()->at(k).second;

        slice = new QPieSlice();
        slice->setLabel(keyValue);
        slice->setLabelVisible(sliceVisible);
        slice->setValue(count);

        series->append(slice);
        connect(slice, &QPieSlice::pressed, this, &VisualData::sliceActionPressed);
        connect(slice, &QPieSlice::released, this, &VisualData::sliceActionReleased);
        connect(slice, &QPieSlice::doubleClicked, this, &VisualData::sliceActionDbClick);
    }
    series->setLabelsPosition(QPieSlice::LabelOutside);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::NoAnimation);
    chart->setBackgroundBrush(QBrush(QColor(230, 230, 230, 230)));

    QString title = this->_mainVisualizedFilter;
    title = title.replace('.', ' ');
    title = title.replace(QRegularExpression("([A-Z])"), " \\1").remove(0,1);
    chart->setTitle(title);
    chart->setTitleFont(QFont("Verdena", 12));
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->legend()->setBackgroundVisible(true);
    chart->legend()->setBrush(QBrush(QColor(220, 220, 220, 220), Qt::BrushStyle::Dense3Pattern));
    chart->legend()->setPen(QPen(QColor(255, 255, 255, 255)));

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QFont verdena = QFont("Verdena", 8);
    QList<QLegendMarker*> markers = series->chart()->legend()->markers(series);
    for(QLegendMarker *marker : markers) {
        QPieLegendMarker *pieMarker = qobject_cast<QPieLegendMarker *>(marker);
        // modify markers from breakdown series
        pieMarker->setLabel(pieMarker->slice()->label());
        pieMarker->setFont(verdena);
    }
    QList<QPieSlice *> slices = series->slices();
    const int LIMIT = 3;
    int counter = LIMIT;
    this->_sliceChartPercentage.clear();
    for(int i = 0; i < slices.size(); ++i) {
       QPieSlice *slice = slices.at(i);
       qreal perc = slice->percentage() * 100;
       int precisionLimit = this->_myConf->data()->precisionLimit();
       slice->setProperty("RealName", slice->label());
       QString updatedLabel = QString("%1 (%2").arg(slice->label(), QString::number(perc,'f', precisionLimit)).append("%)");
       slice->setLabelFont(verdena);
       slice->setLabel(updatedLabel);
       this->_sliceChartPercentage.append(updatedLabel);
       // rough algo I must improve
       if(perc < 3.00) {
           double fact = double(verdena.pixelSize() * counter) / 10.0;
           slice->setLabelArmLengthFactor(std::abs(fact));
           counter--;
           if(counter == 0) counter = LIMIT;
       }
       else counter = LIMIT;
    }
    return(chartView);
}

void VisualData::sliceActionPressed()
{
    QPieSlice *slice = qobject_cast<QPieSlice *>(sender());
    slice->setExploded(true);
    slice->setExplodeDistanceFactor(.05);
    if(!_myConf->data()->sliceLabelVisibility())
        slice->setLabelVisible(true);
}

void VisualData::sliceActionReleased()
{
    QPieSlice *slice = qobject_cast<QPieSlice *>(sender());
    slice->setExploded(false);
    if(!_myConf->data()->sliceLabelVisibility())
        slice->setLabelVisible(false);
}

void VisualData::sliceActionDbClick()
{
    QPieSlice *slice = qobject_cast<QPieSlice *>(sender());
    QString currentLabel = QString(slice->property("RealName").toString());
    int currentValue = int(slice->value());
    if(currentLabel == "Others") {
        this->_sliceGroupingOcc.clear();
        for (int i = 0; i < this->_othersList.size(); ++i) {
            this->groupSliceToHashTable(this->_othersList.at(i), 0, false);
        }
    }
    else
        this->groupSliceToHashTable(currentLabel, currentValue);

    this->sendDataToTable(currentLabel);

    // qDebug() <<"limited"<<this->_limitedOccuVec.size();
    // qDebug()<<"total"<<this->_occurrencesVec.size();
    // qDebug()<<"list"<<this->_othersList.size() << this->_othersList;
}

const QStringList &VisualData::sliceLegendPercentage()
{
    return(this->_sliceChartPercentage);
}

QVector<QPair<QString, qint16>> *VisualData::groupResultToChart()
{
    double LIMITER = this->_myConf->data()->othersSliceLimit();
    bool hide = this->_myConf->data()->hideOthersSlice();
    if(LIMITER == double(0) || hide)
        return(&this->_occurrencesVec);

    this->_othersList.clear();
    std::vector<int> indexes;
    double percentage = 0;
    double tot = 0;
    this->_limitedOccuVec.clear();
    for (int c = 0; c < this->_occurrencesVec.size(); ++c) {
        tot += this->_occurrencesVec.at(c).second;
    }
    for(int i = 0; i < this->_occurrencesVec.size(); ++i) {
        percentage = (this->_occurrencesVec.at(i).second / tot) * 100;
        if(percentage < LIMITER) {
            indexes.push_back(i);
            this->_othersList.append(this->_occurrencesVec.at(i).first);
        }
        else this->_limitedOccuVec.push_back(this->_occurrencesVec.at(i));
    }
    qint16 sum = 0;
    for(uint j = 0; j < indexes.size(); ++j) {
        sum += this->_occurrencesVec.at(indexes.at(j)).second;
    }
    QPair<QString, qint16> pair;
    pair.first = "Others";
    pair.second = sum;
    this->_limitedOccuVec.push_back(pair);
    return(&this->_limitedOccuVec);
}

bool VisualData::checkOccSize(QWidget *parent)
{
    if(this->_occurrencesVec.size() > _LIMIT_OCC_SIZE) {
         QMessageBox::warning(parent, tr("Size Slice Warning"), tr("the number of Piechart slices are exceeding the limit"));
         return(false);
    }
    return(true);
}

void VisualData::showAllSlicesLabels(QChart *chart, bool check)
{
    QPieSeries *series = qobject_cast<QPieSeries*>(chart->series().at(0));
    QList<QPieSlice *> slices = series->slices();
    for(int i = 0; i < slices.size(); ++i) {
       QPieSlice *slice = slices.at(i);
       slice->setLabelVisible(check);
    }
}

QVector<qint16> VisualData::jsonDocsIndex(const QString &name) const
{
    return(this->_sliceGroupingOcc.value(name));
}

void VisualData::groupSliceToHashTable(const QString &name, int value, bool clear)
{
    QString currentName;
    QString masterKeyName;
    Q_UNUSED(value);
    if(clear)
        this->_sliceGroupingOcc.clear();

    for(int n = 0; n < this->_jObjsExtended.size(); ++n) {
        if(_jObjsExtended.at(n).value(this->_mainVisualizedFilter).isString())
            currentName = this->_jObjsExtended.at(n).value(this->_mainVisualizedFilter).toString();
        else if(this->_jObjsExtended.at(n).value(this->_mainVisualizedFilter).isDouble())
            currentName = QString::number(this->_jObjsExtended.at(n).value(this->_mainVisualizedFilter).toDouble());
        if(currentName == name) {
            qint16 documentIndex = qint16(this->_jObjsExtended.at(n).value("indexRef").toInt());
            if(this->_jObjsExtended.at(n).value("masterKey").isString())
                masterKeyName = this->_jObjsExtended.at(n).value("masterKey").toString();
            else if(this->_jObjsExtended.at(n).value("masterKey").isDouble())
                masterKeyName = QString::number(this->_jObjsExtended.at(n).value("masterKey").toDouble());

            if(this->_sliceGroupingOcc.contains(masterKeyName)) {
                this->_sliceGroupingOcc[masterKeyName].append(documentIndex);
            }
            else {
                QVector<qint16> indexVect;
                indexVect.append(documentIndex);
                this->_sliceGroupingOcc.insert(masterKeyName, indexVect);
            }
            // optim to use in case of duplicates used to draw the chart
            //value--;
            //if(value == 0)
            //    break;
        }
    }
}

void VisualData::sendDataToTable(const QString &name)
{
    QStringList listItemsKey;
    int occ = 0;
    QHashIterator<QString, QVector<qint16>> it(this->_sliceGroupingOcc);
    while(it.hasNext()) {
        it.next();
        occ = it.value().size();
        listItemsKey.append(it.key());
    }
    emit emitItemsList(this->_sliceGroupingOcc, name);
}
