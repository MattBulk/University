#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QDir>
#include <QSettings>
#include "states.h"
#include "configurationparams.h"

class Settings : public QObject
{
     Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);

    ~Settings();

    void initBaseSettings();
    /**
     * @brief saveProject it works both to save or save_as
     * @param parent the QWidget where the Dialog will be shown
     * @param db the database to be saved
     * @param save_as if true save_as new doc
     * @return bool
     */
    bool saveProject(QWidget *parent, const QVector<QJsonObject> &db, bool save_as = false);
    /**
     * @brief openProject open a .sr project
     * @param parent the QWidget where the Dialog will be shown
     * @param db the database to be saved
     */
    void openProject(QWidget *parent, QVector<QJsonObject> &db);
    /**
     * @brief importDataDialog
     * @param parent the QWidget where the Dialog will be shown
     * @return
     */
    QString importDataDialog(QWidget *parent);
    /**
     * @brief currentPath to the folder where all the docs should go
     * @return QDir
     */
    QDir currentPath() const;
    /**
     * @brief currentState
     * @return the state of the project
     */
    States currentState() const;

    ConfigurationParams *confParams();

signals:
    /**
     * @brief parseMainDB this signal is emitted when the root folder is already created
     * @param path root folder
     */
    void parseMainDB(const QDir &path);

public slots:

private:
    QDir _currentPath;

    States _currentState;

    QString _currentSavingPath;
    /**
     * @brief _confParams QSettings class pointer is part of the general settings
     */
    ConfigurationParams *_confParams;
};

#endif // SETTINGS_H
