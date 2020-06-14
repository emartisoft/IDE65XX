#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <QFileDialog>
#include <QFontDialog>
#include <QColorDialog>
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
    void setTabPolicy(int index);
    void setCodeFontName(QString &fontName);
    void setCodeFontSize(int fontSize);
    void setApplicationFont(QString fontName, int fontSize);
    void setApplicationTheme(int select);
    void setWordWrap(bool &value);  
    void setShowAllCharacters(bool &value);
    void setAutoCompletion(bool &value);
    void setSIDPlayer(QString &value);
    void setSIDPlayerParameters(QString &value);
    void setCartconv(QString &value);

    void setCustomBackgroundTexture(QString value);
    void setCustomButtonTexture(QString value);
    void setBackground(QColor c);
    void setBrightText(QColor c);
    void setBase(QColor c);
    void setHighlights(QColor c);
    void setDisable(QColor c);
    void setOpcode(QColor c);
    void setNumber(QColor c);
    void setFunction(QColor c);
    void setAssemblerDir(QColor c);
    void setPreprocessorDir(QColor c);
    void setComment(QColor c);
    void setQuotation(QColor c);
    void setLabel(QColor c);


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
    QString getCartconv();
    QString getBuildDir();
    bool getOpenLastUsedFiles();
    int getMaxRecentWorkspace();
    int getTabSize();
    int getTabPolicy();
    QString getCodeFontName();
    int getCodeFontSize();
    bool getWordWrap();
    bool getShowAllCharacters();
    bool getAutoCompletion();

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

    void on_bCartconv_clicked();

    void on_cTheme_currentTextChanged(const QString &arg1);

    void on_bBackgroundTexture_clicked();

    void on_bButtonTexture_clicked();

    void on_bBackgroundColor_clicked();

    void on_bBrightTextColor_clicked();

    void on_bBaseColor_clicked();

    void on_bHighlightsColor_clicked();

    void on_bDisableColor_clicked();

    void on_bOpcode_clicked();

    void on_bNumber_clicked();

    void on_bFunction_clicked();

    void on_bAssemblerDir_clicked();

    void on_bPreprocessorDir_clicked();

    void on_bComment_clicked();

    void on_bQuotation_clicked();

    void on_bLabel_clicked();

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
