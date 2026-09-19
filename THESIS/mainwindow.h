#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <settings.h>
#include <jsonparser.h>
#include <filterdata.h>
#include <visualdata.h>

#include <QButtonGroup>
#include <QAbstractButton>
#include <QChartView>
#include <QHash>

#include <QPointer>

namespace Ui {
class MainWindow;
}
/**
 * @brief The MainWindow class all the software runs on this class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief MainWindow main method
     * @param parent
     */
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

public slots:
    /**
     * @brief addWidgetToMainFilterList adds all the checkbox based on the listItem passed by \a _theParser
     */
    void addWidgetToMainFilterList();

    void addItemsListToListWidget(const QHash<QString, QVector<qint16>> &hashTable, const QString &name);

private slots:
    /**
     * @brief on_actionImport_triggered open popup menu to add a database file
     */
    void on_actionImport_triggered();
    /**
     * @brief on_Submit_clicked submit the filtered vector and call the draw method from \a visualdata
     * @return boolean
     */
    bool on_Submit_clicked();
    /**
     * @brief checkStates select and deselect the filters fireing the method from the \a FilterData class
     * @param btn a signal from the btn
     * @param stateFlag bool containing the state
     */
    void checkStates(QAbstractButton *btn, bool stateFlag);
    /**
     * @brief on_actionSave_triggered fires one \a Settings class method to save project
     */
    void on_actionSave_triggered();
    /**
     * @brief on_actionSave_as_triggered fires one \a Settings class method to save a project
     */
    void on_actionSave_as_triggered();
    /**
     * @brief on_actionExport_triggered fires a method to export QtChart as .png
     */
    void on_actionExport_triggered();
    /**
     * @brief on_actionClose_triggered fires a method to close the project
     */
    void on_actionClose_triggered();
    /**
     * @brief on_actionOpen_triggered fires a method to open a project
     */
    void on_actionOpen_triggered();
    /**
     * @brief on_labelsVisible_toggled fires a method to enable QPieSeries label visibility
     * @param checked
     */
    void on_checkBoxLabelsVisible_toggled(bool checked);
    /**
     * @brief on_actionTools_triggered fires the Dialog query
     */
    void on_actionTools_triggered();
    /**
     * @brief on_hide_checkBox_toggled fires the hide Others option
     * @param checked
     */
    void on_checkBoxHide_toggled(bool checked);
    /**
     * @brief doubleClickedItemkey it fires the method to retrieve the wanted json
     */
    void doubleClickedItemkey();

private:
    /**
     * @brief closeEvent allows if the \a mainWindow can be closed
     * @param event
     */
    void closeEvent(QCloseEvent *event);
    /**
     * @brief initVisualDataProcess
     * \note start the filtering process, copies the data from jsonParser
     */
    void initVisualDataProcess();
    /**
     * @brief setLeftPaneFilters set all the buttons in the left panel
     */
    void setLeftPaneFilters();
    /**
     * @brief drawing creates the QChartView
     */
    void drawing();
    /**
     * @brief saveReport
     * @return bool
     * \note creates and export the report of the current Chart
     */
    bool saveReport();
    /**
     * @brief initUpdate System dialog
     */
    //void initUpdate();

    Ui::MainWindow *ui;

    QButtonGroup *_btnGroupFilters;

    Settings *_theSettings;

    JSONParser *_theParser;

    FilterData *_theFilterData;

    VisualData *_theVisualData;

    QChartView *_chartView;
    /**
     * @brief _stageAreaCleaned checks if a chart has been added to the stage
     */
    bool _stageAreaCleaned;

    QString _infoStr;

};

#endif // MAINWINDOW_H
