#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <QFileDialog>
#include <QFontDialog>
#include <QTextStream>
#include <QSettings>
#include <QMessageBox>
#include "common.h"
#include "popupmenu.h"

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();
    void setJRE(QString &value);
    void setKickAssJar(QString &value);
    void setEmulator(QString &value);
    void setEmulatorParameters(QString &value);
    void setCompressionUtility(QString &value);
    void setCompressionParameters(QString &value);
    void setDirMaster(QString &value);
    void setC1541(QString &value);
    void setDebugger(QString &value);
    void setOpenLastUsedFiles(bool open);
    void setMaxRecentWorkspace(int count);
    void setTabSize(int size);
    void setCodeFontName(QString &fontName);
    void setCodeFontSize(int fontSize);
    void setApplicationFont(QString fontName, int fontSize);
    void setApplicationTheme(int select);
    void setWordWrap(bool &value);  
    void setSIDPlayer(QString &value);
    void setSIDPlayerParameters(QString &value);

    void setCmdLinesEnabledForDebug(bool debugdump = true, bool vicesymbols = true);
    bool getCmdLineDebugDump();
    bool getCmdLineViceSymbols();

    QString getJRE();
    QString getKickAssJar();
    QString getEmulator();
    QString getEmulatorParameters();
    QString getCompressionUtility();
    QString getCompressionParameters();
    QString getDirMaster();
    QString getC1541();
    QString getDebugger();
    QString getSIDPlayer();
    QString getSIDPlayerParameters();
    QString getBuildDir();
    bool getOpenLastUsedFiles();
    int getMaxRecentWorkspace();
    int getTabSize();
    QString getCodeFontName();
    int getCodeFontSize();
    bool getWordWrap();

    bool restartRequired;

    void ReadCmdLineOption();

    bool getCheckShowMemCmdLine();

private slots:
    void on_bKickAssJar_clicked();

    void on_bJRE_clicked();

    void on_bEmulator_clicked();

    void on_bCompressionUtility_clicked();

    void on_bChangeFontOutput_clicked();

    void on_coAsminfo_toggled(bool checked);

    void on_coBytedump_toggled(bool checked);

    void on_coDefine_toggled(bool checked);

    void on_coFillbyte_toggled(bool checked);

    void on_coLibdir_toggled(bool checked);

    void on_coMaxaddr_toggled(bool checked);

    void on_coO_toggled(bool checked);

    void on_coOutputDir_toggled(bool checked);

    void on_coReplacefile_toggled(bool checked);

    void on_coLog_toggled(bool checked);

    void on_coExtra_toggled(bool checked);

    void on_buttonBox_accepted();

    void on_coExecutelog_toggled(bool checked);

    void on_coSymbolfile_toggled(bool checked);

    void on_bDirMaster_clicked();

    void on_bC1541_clicked();

    void on_bDebugger_clicked();

    void on_bSIDPlayer_clicked();

    void on_bChangeFontOutput2_clicked();

    void on_cTheme_currentIndexChanged(int index);

    void setVice();
    void setEmu64();
    void setHoxs64();
    void setMicro64();
    void setZ64K();

private:
    Ui::SettingsWindow *ui;
    QSettings cmdline;
    QSettings settings;
    void NoEmptyForInputText();
#ifdef Q_OS_MACOS
    void changePathForMacOSAppFile(QString &AppFilePath);
#endif
};

#endif // SETTINGSWINDOW_H
