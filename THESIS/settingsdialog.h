#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <configurationparams.h>
#include <QScopedPointer>

namespace Ui {
class SettingsDialog;
}
/**
 * @brief The SettingsDialog class sets the general parameters for the charts
 */
class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief SettingsDialog
     * @param myConf pointer to QSettings
     * @param parent QWidget to destroy it
     */
    explicit SettingsDialog(ConfigurationParams *myConf = nullptr, QWidget *parent = nullptr);

    ~SettingsDialog();
    /**
     * @brief validate saves the data
     */
    void validate();

private slots:
    /**
     * @brief on_doubleSpinBoxOthersPerc_valueChanged sends the percentage to which other slide will start group
     * @param arg
     */
    void on_doubleSpinBoxOthersPerc_valueChanged(double arg);
    /**
     * @brief on_spinBoxPrec_valueChanged sends the precision of the percentage
     * @param arg
     */
    void on_spinBoxPrec_valueChanged(int arg);
    /**
     * @brief on_comboBoxLangague_currentIndexChanged change the language
     * @param arg
     */
    void on_comboBoxLanguage_currentIndexChanged(const QString &arg);

private:
    Ui::SettingsDialog *ui;

    QScopedPointer<ConfigurationParams> *_myConf;

    double _spinDouble;

    int _prec;

    QString _currentLanguage;

    QString _currentLanguageField;
};

#endif // SETTINGSDIALOG_H
