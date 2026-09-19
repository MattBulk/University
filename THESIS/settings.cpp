#include "settings.h"
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <QVector>
#include <QJsonObject>
#include <QJsonDocument>

Settings::~Settings()
{
    qDebug("Settings::~Settings()");
}

Settings::Settings(QObject *parent) : QObject(parent)
{
    this->_confParams = new ConfigurationParams(this);
    this->_currentState = States::Idle;
    this->_currentSavingPath = "null";
}

void Settings::initBaseSettings() {

    this->_currentPath = QDir(QString("%1/%2").arg(QDir::home().absolutePath(), "/Documents/static_ripper"));
    if(_currentPath.exists()) {
        emit parseMainDB(this->currentPath());
    }
    else {
        _currentPath.mkpath(_currentPath.absolutePath());
    }
    this->_confParams->setPrecisionLimit(2);
    this->_confParams->setOthersSliceLimit(0.00);
    this->_confParams->setSliceLabelVisibility(false);
}

bool Settings::saveProject(QWidget *parent, const QVector<QJsonObject> &db, bool save_as)
{ 
    QFile file;
    if(save_as || this->_currentSavingPath == "null") { // save to current path
        if(this->_currentSavingPath == "null") {
            this->_currentSavingPath = QFileDialog::getSaveFileName(parent, tr("Save File"), "project",  "Statistic Project (*.sr)");
        }
        else {
            QString newSavingPath = QFileDialog::getSaveFileName(parent, tr("Save File"), "project",  "Statistic Project (*.sr)");
            // file.setFileName(newSavingPath);
            // file.open(QIODevice::WriteOnly);
            return true;
        }
    }
    this->_currentState = States::Saved;
    qDebug() << this->_currentSavingPath;
    file.setFileName(this->_currentSavingPath);

    //saved as binary data ok ! but I need a separator beetween json.
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);   // we will serialize the data into the file
    for (int i = 0; i < db.size(); ++i) {
        QJsonDocument obj(db.at(i));
        out << obj.toJson();
    }
    file.close();
    return(true);
}

//void Settings::saveAsProject(QWidget *parent, QVector<QJsonObject> &db) {}

void Settings::openProject(QWidget *parent, QVector<QJsonObject> &db)
{
    Q_UNUSED(db)

    QString filePath = QFileDialog::getOpenFileName(parent, tr("Open File"));
    this->_currentSavingPath = filePath;
    QFile file(this->_currentSavingPath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    file.close();
    this->_currentState = States::Opened;
}

QString Settings::importDataDialog(QWidget *parent)
{
    QString filePath = QFileDialog::getOpenFileName(parent, tr("Import File"));
    this->_currentState = States::Imported;
    return filePath;
}

QDir Settings::currentPath() const
{
    return(this->_currentPath);
}

States Settings::currentState() const
{
    return(this->_currentState);
}

ConfigurationParams* Settings::confParams()
{
    return(this->_confParams);
}
