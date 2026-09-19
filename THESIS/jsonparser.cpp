#include "jsonparser.h"

#include <QDebug>
#include <QString>
#include <QRegularExpression>
#include <QMessageBox>
#include <QDirIterator>

#include <QJsonArray>
#include <QJsonValueRef>
#include <QHashIterator>

JSONParser::~JSONParser()
{
    qDebug("JSONParser::~JSONParser()");
}

JSONParser::JSONParser(QObject *parent) : QObject(parent)
{
    this->_counterProgression = 0;
    this->_theValidator = new ValidateJson(this);
}

bool JSONParser::parseDataToFolder(const QString &path, const QDir &workingDir) //add the indexNumberToTheFolder
{
    QFile file(path);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox msgBox;
        msgBox.setText(tr("ERROR ALERT"));
        msgBox.setInformativeText(tr("an error occured while opening the file"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return(false);
    }
    QString currentLine;
    while(!file.atEnd()) {
        currentLine = file.readLine();
        if(currentLine.contains("<json>")) {
            currentLine.remove(QRegularExpression("((<)[^(><)]*>)"));
            QJsonDocument doc = QJsonDocument::fromJson(currentLine.toUtf8());

            if(!this->_mainDB.contains(doc)) {
                this->_counterProgression++;
                QString fileName = QString("%1/%2.json").arg(workingDir.absolutePath(), QString::number(this->_counterProgression));
                QFile saveJSON((fileName));
                saveJSON.open(QIODevice::WriteOnly);
                saveJSON.write(doc.toJson(QJsonDocument::Indented));
                this->addElement(doc);
            }
            else {
                qDebug() << tr("element already present in the database");
            }
        }
    }
    // creates the LIST MENU
    this->_theValidator->itemsList(this->_listMenuItems);
    emit createWidgetMenuItemsElement();
    return(true);
}

void JSONParser::addElement(const QJsonDocument& doc)
{
    if(this->_theValidator->ckeckJsonDoc(doc.object()))
        this->_mainDB.insert(doc, doc.object());
}

bool JSONParser::initDataBase(const QDir &workingDir)
{
    QDirIterator it(workingDir.absolutePath(), QStringList() << "*.json");

    qint32 max = 0;
    qint32 currentNum = 0;
    while(it.hasNext()) {
        QFileInfo fi(it.next());
        currentNum = QLocale::c().toInt(fi.baseName());
        max = std::max(max, currentNum);

        QFile currentFile(it.filePath());
        currentFile.open(QIODevice::ReadOnly);
        this->addElement(QJsonDocument::fromJson(currentFile.readAll()));
    }
    this->_counterProgression = max;
    this->_theValidator->itemsList(this->_listMenuItems);
    emit createWidgetMenuItemsElement();
    return(true);
}

void JSONParser::copyTheData(QVector<QJsonObject> &db)
{
    db.clear();
    QHashIterator<QJsonDocument, QJsonObject> i(this->_mainDB);
    while(i.hasNext()) {
        i.next(); //THIS IS ALWAYS THE FIRST LINE
        //qDebug() << i.key() << ": " << i.value();
        db.push_back(i.value());
    }
}
