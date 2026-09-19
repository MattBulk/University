#include "configurationparams.h"
#include <QDebug>

ConfigurationParams::~ConfigurationParams()
{
    qDebug("ConfigurationParams::~ConfigurationParams()");
}

ConfigurationParams::ConfigurationParams(QObject *parent) : QSettings(parent)
{
    this->_languageKeys.insert("Italian", "it");
    this->_languageKeys.insert("English", "en");
    this->_languageKeys.insert("System language", QString("%1").arg(QLocale::system().name()));
}

void ConfigurationParams::setPrecisionLimit(int value)
{
    this->setValue("settingsDialog/precisionLimit", value);
}

int ConfigurationParams::precisionLimit() const
{
    return(this->value("settingsDialog/precisionLimit").toInt());
}

void ConfigurationParams::setOthersSliceLimit(double value)
{
    this->setValue("settingsDialog/othersSliceLimit", value);
}

double ConfigurationParams::othersSliceLimit() const
{
    return(this->value("settingsDialog/othersSliceLimit").toDouble());
}

void ConfigurationParams::setHideOthersSlice(bool value)
{
    this->setValue("baseSettings/hideSlice", value);
}

bool ConfigurationParams::hideOthersSlice() const
{
    return(this->value("baseSettings/hideSlice").toBool());
}

void ConfigurationParams::setSliceLabelVisibility(bool value)
{
    this->setValue("baseSettings/slicesLabelVisible", value);
}

bool ConfigurationParams::sliceLabelVisibility() const
{
    return(this->value("baseSettings/slicesLabelVisible").toBool());
}

void ConfigurationParams::setMasterKey(const QString &name)
{
    this->setValue("baseSettings/masterKey", name);
}

QString ConfigurationParams::masterKey() const
{
    return(this->value("baseSettings/masterKey").toString());
}

void ConfigurationParams::setGroupFilter(const QString &name)
{
    this->setValue("baseSettings/groupFilter", name);
}

QString ConfigurationParams::groupFilter() const
{
    return(this->value("baseSettings/groupFilter").toString());
}

void ConfigurationParams::setCurrentLanguage(const QString &name)
{
    this->setValue("settingsDialog/language", name);
}

QString ConfigurationParams::currentLanguage() const
{
    return(this->value("settingsDialog/language").toString());
}

void ConfigurationParams::setCurrentLanguageField(const QString &name)
{
    this->setValue("settingsDialog/languageField", name);
}

QString ConfigurationParams::currentLanguageField() const
{
    return(this->value("settingsDialog/languageField").toString());
}

QString ConfigurationParams::languageKeys(const QString &str)
{
    return(this->_languageKeys.value(str));
}
