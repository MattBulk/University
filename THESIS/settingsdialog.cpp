#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QDebug>

SettingsDialog::SettingsDialog(ConfigurationParams *myConf, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    this->_myConf = new QScopedPointer<ConfigurationParams>(myConf);

    this->ui->spinBoxPrec->setValue(this->_myConf->data()->precisionLimit());
    this->ui->doubleSpinBoxOthersPerc->setValue(this->_myConf->data()->othersSliceLimit());
    this->ui->comboBoxLanguage->setCurrentText(this->_myConf->data()->currentLanguageField());
    this->ui->comboBoxLanguage->setCurrentText(this->_myConf->data()->currentLanguageField());

    this->_prec = this->_myConf->data()->precisionLimit();
    this->_spinDouble = this->_myConf->data()->othersSliceLimit();
    this->_currentLanguage = this->_myConf->data()->currentLanguage();
}

SettingsDialog::~SettingsDialog()
{
    qDebug("SettingsDialog::~SettingsDialog()");
    // Scoped Pointer removes these repetitive
    // this->_myConf = nullptr;
    // delete this->_myConf;
    delete ui;
}

void SettingsDialog::on_doubleSpinBoxOthersPerc_valueChanged(double arg)
{
    this->_spinDouble = arg;
}

void SettingsDialog::on_spinBoxPrec_valueChanged(int arg)
{
    this->_prec = arg;
}

void SettingsDialog::validate()
{
    this->_myConf->data()->setPrecisionLimit(this->_prec);
    this->_myConf->data()->setOthersSliceLimit(this->_spinDouble);
    this->_myConf->data()->setCurrentLanguage(this->_currentLanguage);
    this->_myConf->data()->setCurrentLanguageField(this->_currentLanguageField);
}

void SettingsDialog::on_comboBoxLanguage_currentIndexChanged(const QString &arg)
{
    QString languageCode = this->_myConf->data()->languageKeys(arg);
    this->_currentLanguage = languageCode;
    this->_currentLanguageField = arg;
}
