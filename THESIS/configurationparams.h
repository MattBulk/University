#ifndef CONFIGURATIONPARAMS_H
#define CONFIGURATIONPARAMS_H

#include <QObject>
#include <QSettings>
#include <QHash>
/**
 * @brief The ConfigurationParams class
 * \note extends the QSettings class -> QObject. It holds the getter/setter method for the general params
 */
class ConfigurationParams : public QSettings
{
    Q_OBJECT
public:
    ~ConfigurationParams();
    /**
     * @brief ConfigurationParams main method
     * @param parent
     */
    explicit ConfigurationParams(QObject *parent = nullptr);
    /**
     * @brief setPrecisionLimit setter precision limit
     * @param value
     */
    void setPrecisionLimit(int value);
    /**
     * @brief precisionLimit getter
     * @return
     */
    int precisionLimit() const;
    /**
     * @brief setOthersSliceLimit setter for the others slice
     * @param value
     */
    void setOthersSliceLimit(double value);
    /**
     * @brief othersSliceLimit getter
     * @return
     */
    double othersSliceLimit() const;
    /**
     * @brief setHideOthersSlice setter hides the others slide and show chart without groupby
     * @param value
     */
    void setHideOthersSlice(bool value);
    /**
     * @brief hideOthersSlice getter
     * @return
     */
    bool hideOthersSlice() const;
    /**
     * @brief setSliceLabelVisibility setter for slice label visibility
     * @param value
     */
    void setSliceLabelVisibility(bool value);
    /**
     * @brief sliceLabelVisibility getter
     * @return bool
     */
    bool sliceLabelVisibility() const;
    /**
     * @brief setMasterKey setter the master key
     * @param name
     */
    void setMasterKey(const QString &name);
    /**
     * @brief masterKey getter
     * @return
     */
    QString masterKey() const;
    /**
     * @brief setGroupFilter setter the group by filter
     * @param name
     */
    void setGroupFilter(const QString &name);
    /**
     * @brief groupFilter getter
     * @return
     */
    QString groupFilter() const;
    /**
     * @brief setCurrentLanguage setter for the right name
     * @param name
     */
    void setCurrentLanguage(const QString &name);
    /**
     * @brief groupFilter
     * @return QString
     */
    QString currentLanguage() const;
    /**
     * @brief setCurrentLanguageField the name of the current combo box item
     * @param name
     */
    void setCurrentLanguageField(const QString &name);
    /**
     * @brief currentLanguageField
     * @return QString
     */
    QString currentLanguageField() const;
    /**
     * @brief languageKeys
     * @param str
     * @return QString
     * \note the qhash return the code for the system translation class
     */
    QString languageKeys(const QString &str);


private:
    QHash<QString, QString> _languageKeys;
};

#endif // CONFIGURATIONPARAMS_H
