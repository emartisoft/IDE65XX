#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QMessageBox>
#include <QTime>
#include <QProcess>
#include <QTextStream>
#include <QElapsedTimer>
#include <QSettings>
#include <QTimer>
#include <QFileSystemWatcher>
#include <QInputDialog>
#include <QDesktopServices>
#include <QSpinBox>

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

#include "tab.h"
#include "highlighter.h"
#include "common.h"
#include "settingswindow.h"
#include "about.h"
#include "tfiledirective.h"
#include "tdiskdirective.h"
#include "ideiconprovider.h"
#include "bookmarkwidget.h"
#include "choosetopic.h"
#include "qhexedit2/qhexedit.h"
#include "hexsearchdialog.h"
#include "cartconv.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
class QAbstractItemModel;
class QCompleter;
class CodeEditor;
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void changeCurrentSavedState(bool changed);
    void changeCode();
    void selectionChangeCode();
    void showContextMenu();
    void SetCursorPos();
    void showFind();
    void find(const QString &pattern, Qt::CaseSensitivity cs, bool all, bool replace,
                      const QString &replaceText = nullptr);
    void RunInEmulator();
    void Compress();
    void OpenWithDirMaster();
    void PlayWithSIDPlayer();
    void CreateD64Image();
    void RemoveFileOrFolder();
    void Rename();
    void OpenWithHexEditor();
    void SetAssemblyFile();
    void ConvertToCrt();

    void OpenCode();
    void OpenHome();
    void OpenHelp();

    void removeBookmark();
    void removeAllBookmark();
    void editBookmark();
    void bookmarkClicked(const QModelIndex &index);
    void bookmarksChangedSlot(quint64 lineNumber, QString text, bool isAdded);
    void updateLineNumberSlot(quint64 currentLine, int delta);

    void memoryViewerCurrentAddressChanged(int);
    void memoryViewerCurrentCoordinateChanged(int x, int y);

    void ReceiveLineForCompleter(QString line);

private slots:
    void on_tvWorkspaceFiles_doubleClicked(const QModelIndex &index);
    void on_tvWorkspaceFiles_clicked(const QModelIndex &index);
    void on_actionOpen_triggered();
    void on_actionNew_triggered();
    void on_actionClose_triggered();
    void on_tabWidget_tabCloseRequested(int index);
    void on_actionSave_triggered();
    void on_actionSelect_Workspace_triggered();
    void on_actionSave_as_triggered();
    void on_actionClose_All_triggered();
    void on_actionSaveAll_triggered();
    void on_actionExit_triggered();
    void on_actionBuild_this_triggered();
    void on_bFindClose_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_tFind_textChanged(const QString &arg1);
    void on_bFindNext_clicked();
    void on_bFindAll_clicked();
    void on_bReplace_clicked();
    void on_bReplaceAll_clicked();
    void on_actionSettings_triggered();
    void on_actionAbout_triggered();
    void on_actionBuild_and_Run_triggered();
    void on_actionBuild_as_binary_triggered();
    void on_tvWorkspaceFiles_customContextMenuRequested(const QPoint &pos);
    void on_actionDebugger_triggered();
    void on_actionCloseAllButThis_triggered();
    void fileChanged(const QString &path);
    void hexfileChanged(const QString &path);
    void on_brHome_anchorClicked(const QUrl &arg1);
    void on_bDescription_clicked();
    void on_bIllegal_clicked();
    void on_bStandard_clicked();
    void on_actionGenerate_File_Directive_triggered();
    void on_actionGenerate_Disk_Directive_triggered();
    void on_listOpenDocuments_itemSelectionChanged();
    void on_actionKick_Assembler_Web_Manual_triggered();
    void on_actionInsert_BASIC_SYS_Line_triggered();
    void on_actionPuts_A_Breakpoint_triggered();
    void on_b65C02opcodes_clicked();
    void on_bAssemblerDirectives_clicked();
    void on_bPreprocessorDirectives_clicked();
    void on_bValueTypes_clicked();
    void on_actionWelcome_triggered();
    void on_actionCode_triggered();
    void on_actionClear_Output_triggered();
    void on_tFind_returnPressed();
    void on_tIssues_cellDoubleClicked(int row, int column);
    void on_bStandardMacros_clicked();
    void on_sbMemoryViewer_valueChanged(int addressPerLine);
    void on_horizontalSlider_valueChanged(int value);
    void on_actionReport_An_Issue_triggered();

    // hex editor slots
    void dataChanged();
    void setOverwriteMode(bool mode);
    void bytesperlineValueChanged(int value);
    void hexFileFindReplace();
    void hexFileRedo();
    void hexFileUndo();
    void hexFileSaveas();
    void hexFileSave();
    void hexFileOpen();
    void hexFileSaveselection();

    void on_actionSet_Assembly_File_For_Current_Tab_triggered();

    void on_actionCartridge_Conversion_Utility_triggered();

private:
    Ui::MainWindow *ui;
    QFileSystemModel *m_ptrModelForTree;

    QAction *findAction;
    QToolBar *leftToolBar;

    QString filePath; // selected file on workspace
    QModelIndex selectedIndex;

    QString workspacePath;

    // settings
    QSettings settings;
    void writeSettings();
    void readSettings();
    void readSettingsOptionsOnly();
    SettingsWindow *settingsWin;

    CartConv *cc;

    QFileSystemWatcher *watcher;
    QFileSystemWatcher *hexwatcher;

    IdeIconProvider *icProvider;

    bool firstOpening;
    QIcon tabicon;

    void closeFile(int index);
    void saveFile(int index = -1);
    QString saveAsFile(int index = -1, QString caption = "Save file");
    void setCurrentTabName(const QString &filePath, int index);
    void openFileFromPath(QString filenamePath);
    void openWorkspace(QString wDir);
    void closeAll();
    void setTopToolbar(int index=-1);

    void printOutput(const QString & message, const QColor &color);
    void printOutputWithTime(const QString & message, const QColor &color);
    bool build(bool afterbuildthenrun = false , bool binary = false);

    // paths
    QString pJRE;
    QString pKickAssJar;
    QString pEmulator;
    QString pEmulatorParameters;
    QString pCompressionUtility;
    QString pCompressionParameters;
    QString pSIDPlayer;
    QString pSIDPlayerParameters;
    QString pCartconv;
    QString pDirMaster;
    QString pC1541;
    QString pDebugger;
    QString pAssemblyFile;
    QString asmNotSelected = "[ Assembly file not selected ]";

    bool pOpenLastUsedFiles;
    int pMaxRecentWorkspace;
    int pTabSize;
    QString pCodeFontName;
    int pCodeFontSize;
    bool pWordWrap;
    int pZoom;

    bool bWelcome;
    QStringList opendocuments;
    void RefreshOpenDocuments();

    BookmarkWidget *bookmarks;
    QSettings bookmarkcfgfile;

    QHexEdit *hexEdit;
    QLabel *hexFilename;
    QSpinBox *bytesPerLine;
    QCheckBox *hexInsert;
    QAction *hRedo;
    QAction *hUndo;
    QAction *hSave;
    QAction *hSaveas;
    QAction *hSaveselection;
    void hexActionEnable();
    void saveHexFile(QString &hfilename, bool saveSelection=false);
    void loadHexFile();
    QString curHexFilename;
    HexSearchDialog *hexSearchDialog;

    QProcess p;
    void ExecuteAppWithFile(QString pApplication, QString additionalArgs="");
    QMenu *fileContextMenu;
    QAction *menuDirmaster;
    QAction *menuExomizer;
    QAction *menuEmulator;
    QAction *menuC1541;
    QAction *menuDelete;
    QAction *menuRename;
    QAction *menuSidPlayer;
    QAction *menuHexEditor;
    QAction *menuAssemblyFile;
    QAction *menuCartConv;

    QAbstractItemModel *modelFromFile(const QString& fileName);
    QCompleter *completer = nullptr;

    QAction *menuHome;
    QAction *menuCode;
    QLabel *spacerTop, *spacerBottom, *spacerMiddle; //vertical spacer for left toolbar

    QLabel *statusCurPosInfo;

    //QByteArray dockState;
    void HideDocks();
    void RestoreDocks();

    void UpdateRecentWorkspaces();
    void RemoveFromRecentWorkspace(QString wspc);
    void AddRecentWorkspace(QString wspc);

    void setActionsEnable(bool value);
    void setActionsEnableForDoc(bool value);

    void insertCode(QString code, QTextCursor::MoveOperation operation = QTextCursor::Start);

    // issues
    void clearIssues();
    void grabIssues(QString currentLine, QTextStream &tStr);
    void addIssue(QString issue, QString filename, QString line);

    void grabMemoryView(QString currentLine, QTextStream &tStr);
    int lastMemoryViewX = -1;
    int lastMemoryViewY = -1;
    bool sbMemoryViewerValueNotChanged = true;

    void prepareBeforeOpen(QString filename);

    void saveUserCompleter();

protected:
    void closeEvent(QCloseEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
};


#endif // MAINWINDOW_H
