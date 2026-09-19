#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QString>
#include <QMessageBox>
#include <QCheckBox>
#include <QtAlgorithms>
#include <algorithm>

#include <QList>
#include <QPieSlice>
#include <QPieSeries>
#include <settingsdialog.h>
#include <QTableWidgetItem>
#include <QHashIterator>

#include <QOpenGLWidget>
#include <QPoint>
#include <QPainter>
#include <QTextDocumentWriter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->scrollAreaWidgetContents->setLayout(ui->verticalLayoutScroll);
    ui->mainToolBar->setVisible(false);
    // change here
    QApplication::setWindowIcon(QIcon(":/sr.ico"));
    this->_btnGroupFilters = new QButtonGroup(this->ui->verticalLayoutScroll);
    this->_btnGroupFilters->setExclusive(true);
    // adding the vertical splitter
    QSplitter *splitVert = new QSplitter;
    splitVert->setOrientation(Qt::Vertical);
    splitVert->addWidget(ui->tabWidget);
    splitVert->addWidget(ui->frameBottom);
    this->centralWidget()->layout()->addWidget(splitVert);
    // they are gonna die on exit
    this->_theSettings = new Settings(this);
    this->_theParser = new JSONParser(this);
    this->_theFilterData = new FilterData(this);
    this->_theVisualData = new VisualData(this, this->_theSettings->confParams());
    // signals
    connect(this->_theSettings, SIGNAL(parseMainDB(QDir)), this->_theParser, SLOT(initDataBase(QDir)));
    connect(this->_theParser, SIGNAL(createWidgetMenuItemsElement()), this, SLOT(addWidgetToMainFilterList()));
    connect(this->_theVisualData, SIGNAL(emitItemsList(QHash<QString, QVector<qint16>>, QString)),
            this, SLOT(addItemsListToListWidget(QHash<QString, QVector<qint16>>, QString)));
    connect(ui->tableWidget, &QListWidget::doubleClicked, this, &MainWindow::doubleClickedItemkey);
    connect(ui->comboBoxMasterKey, &QComboBox::currentTextChanged, this, &MainWindow::on_Submit_clicked);

    this->_theSettings->initBaseSettings();
    this->_chartView = new QChartView();
    this->_stageAreaCleaned = true;
    QHeaderView *header = ui->tableWidget->horizontalHeader();
    header->setSectionResizeMode(0, QHeaderView::Stretch);
    header->setSectionResizeMode(1, QHeaderView::ResizeToContents);

    this->setLeftPaneFilters();
    this->on_checkBoxLabelsVisible_toggled(true);

    // this->initUpdate();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    switch (this->_theSettings->currentState()) {
        case States::Idle:
        case States::Saved:
        case States::Closed:
        case States::Imported:
            event->accept();
            break;
        default:
            event->ignore();
            break;
    }
}

void MainWindow::addWidgetToMainFilterList()
{
    ui->comboBoxMasterKey->addItems(_theParser->listMenuItems());
    ui->comboBoxEditable->addItems(_theParser->listMenuItems());
    ui->comboBoxEditable->setEditText("");
    int i = 0;
    while(!this->ui->verticalLayoutScroll->isEmpty()) {
        delete this->ui->verticalLayoutScroll->takeAt(0);
        i++;
    }
    for (int i = 0; i < _theParser->listMenuItems().size(); ++i) {
        QCheckBox *check = new QCheckBox();
        check->setFont(QFont("Verdena"));
        check->setText(_theParser->listMenuItems().at(i));
        check->setObjectName(_theParser->listMenuItems().at(i));
        this->_btnGroupFilters->addButton(check);
        this->ui->verticalLayoutScroll->addWidget(check);
    }
    connect(this->_btnGroupFilters, SIGNAL(buttonToggled(QAbstractButton*, bool)), this, SLOT(checkStates(QAbstractButton*, bool)));
    ui->comboBoxMasterKey->setCurrentText(this->_theSettings->confParams()->masterKey());
}

void MainWindow::setLeftPaneFilters()
{
    QList<QAbstractButton *> btnList = this->_btnGroupFilters->buttons();
    for (int b = 0; b < btnList.size(); ++b) {
        QAbstractButton *btn = btnList.at(b);
        if(btn->text() == this->_theSettings->confParams()->groupFilter()) {
            btn->setChecked(true);
            break;
        }
    }
}

void MainWindow::addItemsListToListWidget(const QHash<QString, QVector<qint16>> &hashTable, const QString &name)
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(hashTable.size());
    ui->labelTable->setText(tr("View master keys grouped by occurrences"));
    QString compostedLabel = QString("%1: ( %2 )").arg(ui->labelTable->text(), name);
    ui->labelTable->clear();
    ui->labelTable->setText(compostedLabel);
    QHashIterator<QString, QVector<qint16>> it(hashTable);
    int r = 0;
    while(it.hasNext()) {
        it.next();
        QTableWidgetItem *newItemName = new QTableWidgetItem(it.key());
        QTableWidgetItem *newItemOccu = new QTableWidgetItem(it.value().size());
        ui->tableWidget->setItem(r, 0, newItemName);
        ui->tableWidget->setItem(r, 1, newItemOccu);
        ++r;
    }
}

void MainWindow::checkStates(QAbstractButton *btn, bool stateFlag)
{
    /* here I can send to the Filter Class my filters selection, so that I can add and remove filters based on the state boolean
     *
     * I could have written the statement as, but I think it is more readable this way
     * if(state) addFilter()
     * else removeFilter()
     */
    this->_theSettings->confParams()->setGroupFilter(btn->text());
    if(stateFlag == true)
        this->_theFilterData->addFilter(btn->text());
    else if(stateFlag == false)
        this->_theFilterData->removeFilter(btn->text());
    emit on_Submit_clicked();
}

bool MainWindow::on_Submit_clicked()
{
    if(this->_theFilterData->filterVec().isEmpty())
        return(false);
    this->initVisualDataProcess();
    return(true);
}

void MainWindow::drawing() {
    // must clean the stage and redraw, it is safe even if takeAt(0) returns null
    delete ui->verticalLayoutDrawFrame->takeAt(0);
    if(this->_theVisualData->checkOccSize(this)) {
        this->_stageAreaCleaned = false;
        delete this->_chartView;
        bool vis = this->ui->checkBoxLabelsVisible->isChecked();
        _chartView = this->_theVisualData->draw(vis);
        this->_chartView->setVisible(true);
        this->ui->verticalLayoutDrawFrame->addWidget(_chartView);
        this->_infoStr = QString(tr("Total documents researched: %1 of which %2 are the unique used to draw the current chart")).arg(
                    QString::number(this->_theFilterData->jsonDbVec().size()),
                    QString::number(this->_theFilterData->masterKeyFilteredDb().size()));
        this->statusBar()->showMessage(this->_infoStr);
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);
        ui->labelTable->setText(tr("View master keys grouped by occurrences"));
    }
}

bool MainWindow::saveReport()
{
    QTextEdit doc;
    QString preMainTitle = this->_theFilterData->filterVec().at(0);
    preMainTitle = preMainTitle.replace('.',' ').replace(QRegularExpression("([A-Z])"), " \\1").remove(0,1);
    QString mainTitle = QString("<div style='text-align:center'><h1>%1<br>").arg(preMainTitle);
    QString statusbar = QString("<h4>%1<br><br>").arg(this->_infoStr);
    QString date = QDate::currentDate().toString();
    QString dateNote = tr("report created on:");
    QString dateLine = QString("<h4>%1 %2<br><br>").arg(dateNote, date);
    QString queryParagraph, query;
    if(!ui->comboBoxEditable->currentText().isEmpty()) {
        query = tr("The query used for the chart results is:");
        queryParagraph = QString("<h4>%1 %2<br><br>").arg(query, ui->comboBoxEditable->currentText());
    }
    else {
        query = tr("No query search has been used to filter the results in the chart.");
        queryParagraph = QString("<h4>%1<br><br>").arg(query);
    }
    doc.setDocumentTitle(tr("Statistics Ripper Report"));
    doc.insertHtml(mainTitle);
    doc.insertHtml(dateLine);
    //trick : saved to file bigger, then revert back
    QRect vp = this->_chartView->geometry();
    this->_chartView->resize(600, 500);
    QPixmap p = this->_chartView->grab();
    QOpenGLWidget *glWidget  = this->_chartView->findChild<QOpenGLWidget*>();
    if(glWidget){
        QPainter painter(&p);
        QPoint d = glWidget->mapToGlobal(QPoint())-this->_chartView->mapToGlobal(QPoint());
        painter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
        painter.drawImage(d, glWidget->grabFramebuffer());
        painter.end();
    }
    p.save("report.png", "PNG");
    this->_chartView->setGeometry(vp);
    //QImage img = p.toImage();
    //QTextCursor cursor (doc.document());
    //cursor.insertImage(img);
    doc.insertHtml("<img src='report.png'><br><br>");
    QString legend = tr("Legend with percentage:");
    doc.insertHtml(QString("<h3>%1<br><br>").arg(legend));
    for(int p = 0; p < this->_theVisualData->sliceLegendPercentage().size(); ++p) {
       QString legendItem = this->_theVisualData->sliceLegendPercentage().at(p);
       doc.insertHtml(QString("<p>%1<br>").arg(legendItem));
    }
    doc.insertHtml("<br>");
    doc.insertHtml(queryParagraph);
    doc.insertHtml(statusbar);
    // save file
    // QFile file;
    // file.setFileName("export.html");
    // file.open(QIODevice::WriteOnly | QIODevice::Text);
    // QTextStream out(&file);
    // out << doc.document()->toHtml();
    // file.close();
    QString savedName = preMainTitle;
    savedName = savedName.replace(" ", "");
    QString name = QFileDialog::getSaveFileName(this, tr("Save File"), savedName,  "Open Document (*.odt)");
    QTextDocumentWriter f(name);
    if(name.endsWith(".odt", Qt::CaseInsensitive)) {
        QTextDocumentWriter f(name);
        f.write(doc.document());
        return(true);
    }
    else
        return(false);
}

void MainWindow::initVisualDataProcess()
{
    // PIVOTAL POINT ! MAIN DATA MANAGING COPY
    this->_theParser->copyTheData(this->_theFilterData->jsonDbVec());
    QString strNameFilter = this->_theFilterData->filterVec().at(0);
    QString masterKey = ui->comboBoxMasterKey->currentText();
    this->_theSettings->confParams()->setMasterKey(masterKey);
    this->_theVisualData->jObjsExtended() = this->_theFilterData->workTheData(masterKey, false);
    this->_theVisualData->sortData(this->_theFilterData->workTheData(masterKey, true), strNameFilter);
    this->_theParser->copyTheData(this->_theFilterData->jsonDbVec());
    QString querySearch = ui->comboBoxEditable->currentText();
    if(!querySearch.isEmpty()) {
        if(this->_theFilterData->workTheQuery(querySearch, this->_theParser->listMenuItems())) {
            this->_theVisualData->jObjsExtended() = this->_theFilterData->workTheData(masterKey, false);
            this->_theVisualData->sortData(this->_theFilterData->workTheData(masterKey, false), strNameFilter);
            drawing();
        }
        else {
           this->_chartView->setVisible(false);
        }
    }
    else drawing();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->_theSettings;
}

void MainWindow::on_actionImport_triggered()
{
    QString filepath = this->_theSettings->importDataDialog(this);
    if(_theParser->parseDataToFolder(filepath, this->_theSettings->currentPath())) {
        QMessageBox::about(this, tr("Parsing the data"), tr("Data loaded correctly"));
        this->on_Submit_clicked();
    }
}

void MainWindow::on_actionSave_triggered()
{
    this->_theSettings->saveProject(this, this->_theFilterData->jsonDbVec());
}

void MainWindow::on_actionSave_as_triggered()
{
    this->_theSettings->saveProject(this, this->_theFilterData->jsonDbVec(), true);
}

void MainWindow::on_actionExport_triggered()
{
    this->saveReport();
}

void MainWindow::on_actionClose_triggered()
{

}

void MainWindow::on_actionOpen_triggered()
{
    this->_theSettings->openProject(this, this->_theFilterData->jsonDbVec());
}

void MainWindow::on_checkBoxLabelsVisible_toggled(bool checked)
{
    this->_theSettings->confParams()->setSliceLabelVisibility(checked);
    if(!this->_stageAreaCleaned)
        this->_theVisualData->showAllSlicesLabels(this->_chartView->chart(), checked);
}

void MainWindow::on_actionTools_triggered()
{
    SettingsDialog *setDial = new SettingsDialog(this->_theSettings->confParams(), this);
    connect(setDial, SIGNAL(finished(int)), setDial, SLOT(deleteLater()));
    if(setDial->exec() == QDialog::Accepted) {
        setDial->validate();
        emit on_Submit_clicked();
    }
}

void MainWindow::on_checkBoxHide_toggled(bool checked)
{
    this->_theSettings->confParams()->setHideOthersSlice(checked);
    if(!this->_stageAreaCleaned)
        emit on_Submit_clicked();
}

void MainWindow::doubleClickedItemkey()
{
    QTableWidget *table = qobject_cast<QTableWidget*>(sender());
    QTableWidgetItem *selectedItem = table->item(table->currentRow(), 0);
    ui->plainTextEdit->clear();
    QVector<qint16> indexVect = this->_theVisualData->jsonDocsIndex(selectedItem->text());
    for (int j = 0; j < indexVect.size(); ++j) {
        qint16 in = indexVect.at(j);
        QJsonDocument doc(this->_theFilterData->jsonDbVec().at(in));
        QString strJson(doc.toJson(QJsonDocument::Indented));
        if(j == 0) ui->plainTextEdit->appendHtml("<h3>Json Documents Reporting</h3><br>");
        ui->plainTextEdit->appendPlainText(strJson);
        if(indexVect.size() > 1 && (indexVect.size()-j != 1))
            ui->plainTextEdit->appendHtml("<h3>next json document:</h3>");
    }
    ui->tabWidget->setCurrentIndex(1);
}
