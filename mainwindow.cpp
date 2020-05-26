#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QCompleter>
#include <QFile>
#include <QStringListModel>

#define TIMEOUT 5000
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , settings(QDir::cleanPath(Common::appConfigDir() + QDir::separator() + "settings.ini"), QSettings::IniFormat)
    , bookmarkcfgfile(QDir::cleanPath(Common::appConfigDir() + QDir::separator() + "bookmark.ini"), QSettings::IniFormat)
{
    ui->setupUi(this);
    workspacePath = "?";
    pAssemblyFile = asmNotSelected;
    icProvider = new IdeIconProvider();

    ui->dwProjectExplorer->hide();
    ui->dwOutput->hide();
    ui->dwIssues->hide();
    ui->dwOpenDocument->hide();

    tabifyDockWidget(ui->dwBookmarks, ui->dwOpenDocument);
    tabifyDockWidget(ui->dwMemoryViewer, ui->dwHexEditor);

    statusCurPosInfo = new QLabel("", this);
    ui->statusbar->addPermanentWidget(statusCurPosInfo);
    ui->statusbar->showMessage("READY.", TIMEOUT);

    ui->findDialog->setVisible(false);

    // find action
    findAction = new QAction(QIcon(":/res/images/find.png"), tr("Find And Replace"), this);
    findAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F));
    connect(findAction, SIGNAL(triggered()), this, SLOT(showFind()));

    // action for left toolbar
    menuCode = new QAction(QIcon(":/res/images/coding.png"), tr("Code"), this);
    menuCode->setEnabled(false);
    menuHome = new QAction(QIcon(":/res/images/home.png"), tr("Welcome"), this);

    connect(menuCode, SIGNAL(triggered()), this, SLOT(OpenCode()));
    connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(OpenHelp()));
    connect(menuHome, SIGNAL(triggered()), this, SLOT(OpenHome()));
    connect(ui->menuEdit, SIGNAL(aboutToShow()), this, SLOT(showContextMenu())); // After Click Edit menu

    // toolbar
    spacerTop = new QLabel(" ", this);
    spacerBottom = new QLabel(" ", this);
    spacerMiddle = new QLabel(" ",this);
    spacerMiddle->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    spacerTop->setFixedHeight(16);
    spacerBottom->setFixedHeight(16);
    setTopToolbar();
    leftToolBar = new  QToolBar();
    leftToolBar->setObjectName("leftSideBar");
    leftToolBar->setWindowTitle("Left Sidebar");
    addToolBar(Qt::LeftToolBarArea, leftToolBar);
    leftToolBar->setMinimumSize(64,0);
    leftToolBar->setMovable(false);
    leftToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    leftToolBar->setOrientation(Qt::Vertical);

    leftToolBar->addWidget(spacerTop);
    leftToolBar->addAction(menuHome);
    leftToolBar->addSeparator();
    leftToolBar->addAction(ui->actionSelect_Workspace);
    leftToolBar->addAction(menuCode);   
    leftToolBar->addSeparator();
    leftToolBar->addAction(ui->actionSettings);
    leftToolBar->addAction(ui->actionHelp);

    leftToolBar->addWidget(spacerMiddle);

    leftToolBar->addAction(ui->actionBuild_as_binary);
    leftToolBar->addSeparator();
    leftToolBar->addAction(ui->actionDebugger);
    leftToolBar->addAction(ui->actionBuild_this);
    leftToolBar->addAction(ui->actionBuild_and_Run);
    leftToolBar->addWidget(spacerBottom);

    // file contextmenu
    ui->tvWorkspaceFiles->setContextMenuPolicy(Qt::CustomContextMenu);
    fileContextMenu = new QMenu;

    menuEmulator = new QAction(QIcon(":/res/images/emulator.png"), tr("Run in Emulator"), this);
    menuExomizer = new QAction(QIcon(":/res/images/crunch.png"), tr("Compress"), this);
    menuDirmaster = new QAction(QIcon(":/res/images/floppy.png"), tr("Open with Dirmaster"), this);
    menuSidPlayer = new QAction(QIcon(":/res/images/sid.png"), tr("Play with SID Player"), this);
    menuC1541 = new QAction(QIcon(":/res/images/floppy.png"), tr("Create D64 image"), this);
    menuDelete = new QAction(QIcon(":/res/images/delete.png"), tr("Delete"), this);
    menuRename = new QAction(QIcon(":/res/images/rename.png"), tr("Rename"), this);
    menuHexEditor = new QAction(QIcon(":/res/images/binary.png"), tr("Open with HexEditor"), this);
    menuAssemblyFile = new QAction(QIcon(":/res/images/assembly.png"), tr("Set Assembly File"), this);

    connect(menuEmulator, SIGNAL(triggered()), this, SLOT(RunInEmulator()));
    connect(menuExomizer, SIGNAL(triggered()), this, SLOT(Compress()));
    connect(menuDirmaster, SIGNAL(triggered()), this, SLOT(OpenWithDirMaster()));
    connect(menuSidPlayer, SIGNAL(triggered()), this, SLOT(PlayWithSIDPlayer()));
    connect(menuC1541, SIGNAL(triggered()), this, SLOT(CreateD64Image()));
    connect(menuDelete, SIGNAL(triggered()), this, SLOT(RemoveFileOrFolder()));
    connect(menuRename, SIGNAL(triggered()), this, SLOT(Rename()));
    connect(menuHexEditor, SIGNAL(triggered()), this, SLOT(OpenWithHexEditor()));
    connect(menuAssemblyFile, SIGNAL(triggered()), this, SLOT(SetAssemblyFile()));


    fileContextMenu->addAction(menuEmulator);
    fileContextMenu->addAction(menuExomizer);
    fileContextMenu->addAction(menuC1541);

#ifdef Q_OS_WIN
    fileContextMenu->addSeparator();
    fileContextMenu->addAction(menuDirmaster);
#endif
    fileContextMenu->addAction(menuSidPlayer);
    fileContextMenu->addAction(menuHexEditor);
    fileContextMenu->addSeparator();
    fileContextMenu->addAction(menuAssemblyFile);
    fileContextMenu->addSeparator();
    fileContextMenu->addAction(menuDelete);
    fileContextMenu->addAction(menuRename);

    opendocuments.clear();

    // init settings window
    settingsWin = new SettingsWindow(this);
    settingsWin->ReadCmdLineOption();
    readSettings();

    // file system watcher
    watcher = new QFileSystemWatcher(this);
    connect(watcher, SIGNAL(fileChanged(const QString&)), this, SLOT(fileChanged(const QString&)));

    // hex file system watcher
    hexwatcher = new QFileSystemWatcher(this);
    connect(hexwatcher, SIGNAL(fileChanged(const QString&)), this, SLOT(hexfileChanged(const QString&)));


    // completer
    completer = new QCompleter(this);
    completer->setModel(modelFromFile(":/res/completer/kickass.dat"));
    completer->setModelSorting(QCompleter::CaseSensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseSensitive);
    completer->setWrapAround(false);

    // welcome
    QString styleSheet = QString("background-color: %1;").arg(palette().window().color().name());
    ui->brHome->setStyleSheet(styleSheet);
    // help
    ui->brHelp->setStyleSheet(styleSheet);
    ui->brHelp->setSearchPaths(QStringList()<<":/res/help/");
    ui->brHelp->setSource(QUrl("help.htm"));

    // bookmarks
    auto verticalLayout_13 = new QVBoxLayout(ui->dockBookmarkWidget);
    verticalLayout_13->setSpacing(0);
    verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
    verticalLayout_13->setContentsMargins(0, 0, 0, 0);

    bookmarks = new BookmarkWidget(ui->dockBookmarkWidget);
    bookmarks->setObjectName(QString::fromUtf8("boookmarks"));
    bookmarks->setFrameShape(QFrame::Box);
    bookmarks->setFrameShadow(QFrame::Sunken);
    bookmarks->setFont(ui->dwProjectExplorer->font());

    connect(bookmarks->removeAction, SIGNAL(triggered()), this, SLOT(removeBookmark()));
    connect(bookmarks->removeAllAction, SIGNAL(triggered()), this, SLOT(removeAllBookmark()));
    connect(bookmarks->editAction, SIGNAL(triggered()), this, SLOT(editBookmark()));
    connect(bookmarks, SIGNAL(clicked(QModelIndex)), this, SLOT(bookmarkClicked(QModelIndex)));

    verticalLayout_13->addWidget(bookmarks);
    ui->dwBookmarks->setWidget(ui->dockBookmarkWidget);

    // hex editor
    auto toolbar = new QToolBar;
    toolbar->setIconSize(QSize(16,16));

    ui->verticalLayout_hexeditor->insertWidget(0, toolbar);

    QAction* hOpen = new QAction(QIcon(":/res/images/open.png"), "Open", this);
    toolbar->addAction(hOpen);
    connect(hOpen, SIGNAL(triggered()), this, SLOT(hexFileOpen()));

    hSave = new QAction(QIcon(":/res/images/save.png"), "Save", this);
    toolbar->addAction(hSave);
    connect(hSave, SIGNAL(triggered()), this, SLOT(hexFileSave()));

    QAction* hSaveas = new QAction(QIcon(":/res/images/saveas.png"), "Save As", this);
    toolbar->addAction(hSaveas);
    connect(hSaveas, SIGNAL(triggered()), this, SLOT(hexFileSaveas()));

    hSaveselection = new QAction(QIcon(":/res/images/saveselection.png"), "Save Selection", this);
    toolbar->addAction(hSaveselection);
    connect(hSaveselection, SIGNAL(triggered()), this, SLOT(hexFileSaveselection()));

    toolbar->addSeparator();

    hUndo = new QAction(QIcon(":/res/images/undo.png"), "Undo", this);
    toolbar->addAction(hUndo);
    connect(hUndo, SIGNAL(triggered()), this, SLOT(hexFileUndo()));

    hRedo = new QAction(QIcon(":/res/images/redo.png"), "Redo", this);
    toolbar->addAction(hRedo);
    connect(hRedo, SIGNAL(triggered()), this, SLOT(hexFileRedo()));

    toolbar->addSeparator();

    QAction* hFindReplace = new QAction(QIcon(":/res/images/find.png"), "Find And Replace", this);
    toolbar->addAction(hFindReplace);
    connect(hFindReplace, SIGNAL(triggered()), this, SLOT(hexFileFindReplace()));

    toolbar->addSeparator();

    bytesPerLine = new QSpinBox(this);
    bytesPerLine->setValue(8);
    bytesPerLine->setMaximum(64);
    bytesPerLine->setMinimum(8);
    bytesPerLine->setSingleStep(8);
    toolbar->addWidget(bytesPerLine);
    connect(bytesPerLine, SIGNAL(valueChanged(int)), this, SLOT(bytesperlineValueChanged(int)));

    toolbar->addWidget(new QLabel(" bytes per line", this));

    toolbar->addSeparator();

    hexInsert = new QCheckBox("Insert Mode", this);
    hexInsert->setChecked(false);
    toolbar->addWidget(hexInsert);
    connect(hexInsert, SIGNAL(clicked(bool)), this, SLOT(setOverwriteMode(bool)));

    hexFilename = new QLabel("???", this);
    ui->verticalLayout_hexeditor->addWidget(hexFilename);

    hexEdit = new QHexEdit(ui->dockHexEditWidget);
    hexEdit->setObjectName(QString::fromUtf8("hexEdit"));
    hexEdit->setFrameShape(QFrame::Box);
    hexEdit->setFrameShadow(QFrame::Sunken);
    hexEdit->setFont(QFont(pCodeFontName, pCodeFontSize));
    hexEdit->setOverwriteMode(!hexInsert->isChecked());
    hexEdit->setReadOnly(false);

    hexEdit->setAddressArea(true);
    hexEdit->setAddressAreaColor(palette().window().color());
    hexEdit->setAsciiArea(true);
    hexEdit->setHighlighting(true);
    hexEdit->setHighlightingColor(QColor(0xff, 0xff, 0x99));
    hexEdit->setSelectionColor(palette().highlight().color());

    hexEdit->setAddressWidth(4);
    hexEdit->setBytesPerLine(8);

    // connects
    connect(hexEdit, SIGNAL(dataChanged()), this, SLOT(dataChanged()));

    ui->verticalLayout_hexeditor->addWidget(hexEdit);
    ui->dwHexEditor->setWidget(ui->dockHexEditWidget);

    hexActionEnable();

    // set recent workspace for home
    UpdateRecentWorkspaces();
    ui->menuEdit->setEnabled(false);
    setActionsEnable(false);

    // output and issues dockwidgets
    tabifyDockWidget(ui->dwIssues, ui->dwOutput);

    // view menu
    ui->menuView->addAction(ui->topToolBar->toggleViewAction());
    ui->menuView->addAction(leftToolBar->toggleViewAction());
    ui->menuView->addSeparator();
    ui->menuView->addAction(ui->dwProjectExplorer->toggleViewAction());
    ui->menuView->addAction(ui->dwOpenDocument->toggleViewAction());
    ui->menuView->addSeparator();
    ui->menuView->addAction(ui->dwOutput->toggleViewAction());
    ui->menuView->addAction(ui->dwIssues->toggleViewAction());
    ui->menuView->addSeparator();
    ui->menuView->addAction(ui->dwBookmarks->toggleViewAction());
    ui->menuView->addAction(ui->dwMemoryViewer->toggleViewAction());
    ui->menuView->addAction(ui->dwHexEditor->toggleViewAction());

    // home or code
    if(bWelcome)
    {
        OpenHome();
    }
    else
    {
        if(pOpenLastUsedFiles)
        {
            OpenCode();
        }
        else
        {
            OpenHome();
        }
    }

    QPalette p(palette());
    p.setColor(QPalette::Highlight, palette().window().color().darker(96));
    p.setColor(QPalette::HighlightedText, Qt::black);
    ui->listOpenDocuments->setPalette(p);
    ui->tvWorkspaceFiles->setPalette(p);
    ui->tOutput->setPalette(p);

    QPalette pIssue(palette());
    pIssue.setColor(QPalette::Highlight, QColor(0xf3, 0x34, 0x34));
    pIssue.setColor(QPalette::HighlightedText, Qt::white);
    ui->tIssues->setPalette(pIssue);

    // Memory Viewer
    ui->dwMemoryViewer->installEventFilter(this);
    ui->sbMemoryViewer->setValue(0x80);
    ui->mvWarning->setVisible(false);
    connect(ui->memoryViewer, SIGNAL(currentAddressChanged(int)), this, SLOT(memoryViewerCurrentAddressChanged(int)));
    connect(ui->memoryViewer, SIGNAL(currentCoordinateChanged(int, int)), this, SLOT(memoryViewerCurrentCoordinateChanged(int, int)));


    // bookmark.ini
    int bookmarkCount = bookmarkcfgfile.value("Count",0).toInt();
    QString bookmarkWorkspace = bookmarkcfgfile.value("0","").toString();
    if((bookmarkCount>0)&&(workspacePath==bookmarkWorkspace))
    {
        for(int i=0;i<bookmarkCount;i++)
        {
            QStringList values = bookmarkcfgfile.value(QString::number(i+1),"").toString().split('|');
            QPixmap bPixmap = icProvider->icon(QFileInfo(QDir::cleanPath(workspacePath + QDir::separator() + values[1]))).pixmap(24,24);
            bookmarks->addItem(bPixmap, values[2], values[0] + " : " + values[1]);
        }
    }

    setAcceptDrops(true);

    // hex search dialog
    hexSearchDialog = new HexSearchDialog(hexEdit, this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*! open file via double click event
 */
void MainWindow:: on_tvWorkspaceFiles_doubleClicked(const QModelIndex &index)
{
    prepareBeforeOpen(m_ptrModelForTree->filePath(index));
}

/*! select file via click event
 */
void MainWindow::on_tvWorkspaceFiles_clicked(const QModelIndex &index)
{
    filePath = m_ptrModelForTree->filePath(index);
    selectedIndex = index;
    tabicon = m_ptrModelForTree->fileIcon(index);
}

/*! open file from file menu
 */
void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open file", workspacePath, tr("Assembler source files (*.asm);;Library files (*.lib);;All files (*.*)"));
    if(!fileName.isEmpty())
    {   
        prepareBeforeOpen(fileName);
    }
}

/*! new file
 */
void MainWindow::on_actionNew_triggered()
{
    if (!selectedIndex.isValid())
    {
        ui->statusbar->showMessage("No workspace", TIMEOUT);
        return;
    }

    Tab *tab = new Tab;
    ui->tabWidget->addTab(tab, tr("New"));
    ui->tabWidget->setCurrentWidget(tab);
    int old = ui->tabWidget->currentIndex();
    QString newFile = saveAsFile(old, "New file");
    if(!newFile.isEmpty())
    {
        filePath = newFile;
        openFileFromPath(newFile);
        ui->statusbar->showMessage("New file", TIMEOUT);
    }
    closeFile(old);

}

/*! text changed for code editor
 */
void MainWindow::changeCurrentSavedState(bool changed)
{
    QString tabText = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    if (changed)
    {
        if (tabText[0] != '*')
            ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), '*' + tabText);
    }
    else
    {
        if (tabText[0] == '*') tabText.remove(0, 1);
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), tabText);
    }
}

/*! text changed for code editor
 */
void MainWindow::changeCode()
{
    if(firstOpening){
        firstOpening = false;
        return;
    }
    QString tabText = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    if (tabText[0] != '*')
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), '*' + tabText);

    SetCursorPos();
}

/*! tab codeeditor selection changed
 */
void MainWindow::selectionChangeCode()
{
    emit on_tabWidget_currentChanged(ui->tabWidget->currentIndex());
}

/*! Edit menu
 */
void MainWindow::showContextMenu()
{
    Tab *tab = static_cast<Tab*>( ui->tabWidget->widget(ui->tabWidget->currentIndex()));
    ui->menuEdit->clear();
    ui->menuEdit->addAction(findAction);
    ui->menuEdit->addSeparator();
    ui->menuEdit->addActions(tab->code->getContextMenu()->actions());  
}

/*! Cursor pos, selection length, file length info
 */
void MainWindow::SetCursorPos()
{
    if(ui->tabWidget->count()<1) return;
    Tab *tab = static_cast<Tab*>( ui->tabWidget->widget(ui->tabWidget->currentIndex()));
    QString curString = tab->code->textCursor().block().text();
    statusCurPosInfo->setText(QString("  Line %1  |  Column %2  |  Sel %4  |  Length %3  |  %5  ").arg(tab->code->textCursor().blockNumber()+1).arg(tab->code->textCursor().positionInBlock()+1).arg(tab->code->toPlainText().length()).arg(tab->code->textCursor().selectedText().length()).arg(tab->code->overwriteMode()?"OVR":"INS"));
}

/*! show find dialog
 */
void MainWindow::showFind()
{
    Tab *tab = static_cast<Tab*>( ui->tabWidget->currentWidget());
    bool fv = ui->findDialog->isVisible();
    ui->findDialog->setVisible(!fv);
    ui->tFind->setText(tab->code->textCursor().selectedText());

    (!fv) ? ui->tFind->setFocus() : tab->code->setFocus();
}

/*! close file
 */
void MainWindow::closeFile(int index)
{
    Tab *tabForDeleting = static_cast<Tab*>( ui->tabWidget->widget(index));
    bool closing = true;

    QString tabText = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    if (tabText[0] == '*')
    {
        int ret = QMessageBox::question(this, "Save",
                                         "Do you want to save your changes?\n"
                                         "Save file [" + tabForDeleting->getCurrentFilePath() + "]?",
                                         QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,  QMessageBox::Save);


        switch (ret) {
            case QMessageBox::Save:
                // Save was clicked
                    saveFile(index);
                break;
            case QMessageBox::Discard:
                // Don't Save was clicked
                break;
            case QMessageBox::Cancel:
                // Cancel was clicked
                closing = false;
                break;
            default:
                // should never be reached
                break;
          }
    }

    if (closing)
    {
        watcher->removePath(tabForDeleting->getCurrentFilePath());
        ui->tabWidget->removeTab(index);
        delete tabForDeleting;
        ui->statusbar->showMessage("File closed", TIMEOUT);
        RefreshOpenDocuments();
    }

    bool b = ui->tabWidget->count()>0;
    ui->menuEdit->setEnabled(b);
    setActionsEnableForDoc(b);
    ui->findDialog->setVisible(false);
    statusCurPosInfo->setVisible(ui->tabWidget->count()>0);

}

void MainWindow::on_actionClose_triggered()
{
    if(selectedIndex.isValid())
        closeFile(ui->tabWidget->currentIndex());
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    closeFile(index);
}

/*! save file
 */
void MainWindow::saveFile(int index)
{
    Tab *tab;
    if (index == -1)
        tab = static_cast<Tab*>( ui->tabWidget->currentWidget());
    else
        tab = static_cast<Tab*>( ui->tabWidget->widget(index));
    QString filePath = tab->getCurrentFilePath();
    if (filePath.isEmpty())
    {
        saveAsFile(index);
    }
    else
    {
        watcher->removePath(filePath); // REDO/UNDO action works after saving file
        tab->saveCodeToFile(filePath, false);
        watcher->addPath(filePath);
        ui->statusbar->showMessage("File saved", TIMEOUT);
    }
}

/*! save file as ...
 */
QString MainWindow::saveAsFile(int index, QString caption)
{
    QString fileName = QFileDialog::getSaveFileName(this, caption, workspacePath,
                                                    tr("Assembler source files (*.asm);;Library files (*.lib);;All files (*.*)"));
    if (!fileName.isEmpty()) {
       // workspacePath = QFileInfo(fileName).absoluteDir().absolutePath();
        Tab *tab;
        if (index == -1)
            tab = static_cast<Tab*>( ui->tabWidget->currentWidget());
        else
            tab = static_cast<Tab*>( ui->tabWidget->widget(index));
        tab->saveCodeToFile(fileName, false);
        setCurrentTabName(fileName, index);
        ui->statusbar->showMessage("File saved", TIMEOUT);
        return fileName;

    }
    else
    {
        ui->statusbar->showMessage("File not saved", TIMEOUT);
        return "";

    }
}

void MainWindow::setCurrentTabName(const QString &filePath, int index)
{
    QString caption;
    int i;
    if ((i = filePath.lastIndexOf('/')) == -1)
        i = filePath.lastIndexOf('\\');
    caption = filePath.mid(i + 1);
    if (index == -1)
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), caption);
    else
        ui->tabWidget->setTabText(index, caption);
}

void MainWindow::openFileFromPath(QString filenamePath)
{
    Tab *tab = new Tab;
    ui->tabWidget->addTab(tab, tr("New"));
    ui->tabWidget->setCurrentWidget(tab);
    firstOpening = true;
    if(!tab->loadCodeFromFile(filenamePath))
    {
        ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
    }
    else
    {
        QString caption;
        int i;
        if ((i = filenamePath.lastIndexOf('/')) == -1) i = filenamePath.lastIndexOf('\\');
        caption = filenamePath.mid(i + 1);
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), caption);

        QFileInfo fi(filenamePath);
        QString ext = fi.completeSuffix();
        // highlight extension list
        if(     (ext=="asm")||
                (ext=="lib")||
                (ext=="inc")||
                (ext=="sym")||
                (ext=="s")||
                (ext=="a")
            ) tab->highlighter = new Highlighter(tab->code->document());
        connect(tab->code, SIGNAL(modificationChanged(bool)), this, SLOT(changeCurrentSavedState(bool)));
        connect(tab->code, SIGNAL(textChanged()), this, SLOT(changeCode()));
        connect(tab->code, SIGNAL(selectionChanged()), this, SLOT(selectionChangeCode()));
        connect(tab->code, SIGNAL(cursorPositionChanged()), this, SLOT(SetCursorPos()));
        connect(tab->code, SIGNAL(bookmarksChanged(quint64, QString, bool)), this, SLOT(bookmarksChangedSlot(quint64, QString, bool)));
        connect(tab->code, SIGNAL(updateLineNumber(quint64, int)), this, SLOT(updateLineNumberSlot(quint64, int)));
        connect(tab->code, SIGNAL(showFindDialog()), this, SLOT(showFind()));
        connect(tab->code, SIGNAL(overWriteModeChanged()), this, SLOT(SetCursorPos()));

        tab->code->document()->setModified(false);
        tab->code->setCompleter(completer);
        tab->code->setFocus();
        tab->code->setFont(QFont(pCodeFontName, pCodeFontSize));
        tab->code->setWordWrapMode((pWordWrap)? QTextOption::WordWrap : QTextOption::NoWrap);
        QFontMetrics fm(tab->code->font());
        tab->code->setTabStopDistance(fm.horizontalAdvance(' ')*pTabSize); // tab distance
        ui->tabWidget->setTabIcon(ui->tabWidget->currentIndex(), icProvider->icon(fi));
        ui->menuEdit->setEnabled(true);
        ui->statusbar->showMessage("File Opened", TIMEOUT);
        watcher->addPath(filenamePath);
        setActionsEnable(true);
        RefreshOpenDocuments();

        // set bookmark
        for (int i=0;i<bookmarks->model()->rowCount();i++) {
                if(tab->getCurrentFilePath().remove(workspacePath) == bookmarks->getFilepath(i))
                {
                    tab->code->bookmarks.append(bookmarks->getLineNumber(i));
                }
        }
        tab->update();
    }
    statusCurPosInfo->setVisible(ui->tabWidget->count()>0);
}

/*! open workspace
 */
void MainWindow::openWorkspace(QString wDir)
{
    QDir wspace(wDir);
    if(!wspace.exists())
    {
        ui->statusbar->showMessage("Workspace does not exist.", TIMEOUT);
        RemoveFromRecentWorkspace(wDir);
        return;
    }

    m_ptrModelForTree  = new QFileSystemModel(this);
    m_ptrModelForTree->setRootPath("");
    m_ptrModelForTree->setFilter(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);
    m_ptrModelForTree->setNameFilters(QStringList()
                                      << "*.asm"
                                      << "*.prg"
                                      << "*.sid"
                                      << "*.bin"
                                      << "*.txt"
                                      << "*.s"
                                      << "*.lib"
                                      << "*.dbg"
                                      << "*.sym"
                                      << "*.d64"
                                      << "*.d71"
                                      << "*.d81"
                                      << "*.a"
                                      << "*.inc"
                                      << "*.vs"
                                      );
    m_ptrModelForTree->setNameFilterDisables(true);
    m_ptrModelForTree->setIconProvider(icProvider);

    ui->tvWorkspaceFiles->setModel(m_ptrModelForTree);
    const QModelIndex rootIndex = m_ptrModelForTree->index(QDir::cleanPath(wDir));
    if(rootIndex.isValid())
        ui->tvWorkspaceFiles->setRootIndex(rootIndex);

    ui->tvWorkspaceFiles->setHeaderHidden(true);
    ui->tvWorkspaceFiles->setSortingEnabled(true);
    ui->tvWorkspaceFiles->sortByColumn(0, Qt::AscendingOrder);
    ui->tvWorkspaceFiles->setIndentation(20);
    ui->tvWorkspaceFiles->hideColumn(1);
    ui->tvWorkspaceFiles->hideColumn(2);
    ui->tvWorkspaceFiles->hideColumn(3);

    workspacePath = wDir;
    selectedIndex = rootIndex;

    ui->statusbar->showMessage("Selected a directory as workspace", TIMEOUT);

    settings.setValue("Workspace", workspacePath);
    settings.sync();

    ui->dwProjectExplorer->show();
//    ui->dwBookmarks->show();
    ui->dwOpenDocument->show();

    UpdateRecentWorkspaces();
    OpenCode();

    setWindowTitle(wDir + " - IDE 65XX");
    ui->actionNew->setEnabled(true);
    ui->actionOpen->setEnabled(true);
    menuCode->setEnabled(true);
    ui->memoryViewer->reset();
    if((lastMemoryViewX!=-1)&&(lastMemoryViewY!=-1))
    {
        sbMemoryViewerValueNotChanged = false;
        ui->lCurrentAddress->setText("????");
        memoryViewerCurrentCoordinateChanged(lastMemoryViewX, lastMemoryViewY);
    }
}

void MainWindow::closeAll()
{
    while(ui->tabWidget->count()>0)
    {
        closeFile(ui->tabWidget->currentIndex());
    }
}

void MainWindow::on_actionCloseAllButThis_triggered()
{
    while(ui->tabWidget->count()>1)
    {
        if(ui->tabWidget->tabText(0) == ui->tabWidget->tabText(ui->tabWidget->currentIndex()))
        closeFile(1); else closeFile(0);
    }
}

void MainWindow::fileChanged(const QString &path)
{
    for(int i=0;i<ui->tabWidget->count();i++)
    {
        Tab *tab = static_cast<Tab*>( ui->tabWidget->widget(i));
        if(tab->getCurrentFilePath()==path) {
            QTextCursor cr = tab->code->textCursor();
            int p = cr.position();
            tab->loadCodeFromFile(path);
            cr.setPosition(p);
            tab->code->setTextCursor(cr);

        }
    }
}

void MainWindow::hexfileChanged(const QString &path)
{
    if(curHexFilename==path)
    {
       loadHexFile();
    }
}

void MainWindow::setTopToolbar(int index)
{
    ui->topToolBar->clear();
    ui->topToolBar->addAction(menuHome);
    ui->topToolBar->addSeparator();
    ui->topToolBar->addAction(ui->actionSelect_Workspace);
    ui->topToolBar->addAction(menuCode);
    ui->topToolBar->addSeparator();
    ui->topToolBar->addAction(ui->actionNew);
    ui->topToolBar->addAction(ui->actionOpen);
    ui->topToolBar->addAction(ui->actionSave);
    ui->topToolBar->addAction(ui->actionSaveAll);
    ui->topToolBar->addSeparator();
    ui->topToolBar->addAction(ui->actionBuild_as_binary);
    ui->topToolBar->addAction(ui->actionBuild_this);
    ui->topToolBar->addAction(ui->actionBuild_and_Run);


    if (index>-1)
    {
        Tab *tab = static_cast<Tab*>( ui->tabWidget->widget(index));
        ui->topToolBar->addSeparator();
        ui->topToolBar->addActions(tab->code->getContextMenu()->actions());
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    int ret = QMessageBox::question(this, "IDE65xx",
                                     "Do you really want to exit?",
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (ret == QMessageBox::Yes)
    {
        OpenCode();
        writeSettings();
        closeAll();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    Tab *tab = static_cast<Tab*>( ui->tabWidget->widget(ui->tabWidget->currentIndex()));
    switch (event->key())
    {
        case (Qt::Key_F1):
        {
                QString word = tab->code->getHelpWord();
                if(     (word=="if")||
                        (word=="import")||
                        (word=="define")||
                        (word=="importonce")
                  )
                {
                    ChooseTopic ct(this);
                    ct.setWord(word);
                    if (ct.exec() == QDialog::Accepted)
                            word= ct.getWord();
                    else
                            word = "";
                    ct.deleteLater();
                }
                ui->brHelp->setSource(QUrl("help.htm#"+ word.toUpper()));
                OpenHelp();
                break;
        }

        case (Qt::Key_F2):
        {
            OpenCode();
            break;
        }

        case (Qt::Key_F3):
        {
            OpenHome();
            break;
        }
    }

}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::Resize && obj == ui->dwMemoryViewer)
    {
        sbMemoryViewerValueNotChanged = false;
        ui->lCurrentAddress->setText("????");
        memoryViewerCurrentCoordinateChanged(lastMemoryViewX, lastMemoryViewY);
    }
    return QWidget::eventFilter(obj, event);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
        event->accept();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        QList<QUrl> urls = event->mimeData()->urls();
        //QString filePath = ;
        prepareBeforeOpen(urls.at(0).toLocalFile());
        event->accept();
    }
}

// save edited file
void MainWindow::on_actionSave_triggered()
{   
    if(!selectedIndex.isValid()) return;
    QString tabText = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    if (tabText[0] == '*')
    {
            saveFile(ui->tabWidget->currentIndex());
            tabText.remove(0,1);
            ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), tabText);
    }
}

// select workspace
void MainWindow::on_actionSelect_Workspace_triggered()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("Select a directory as workspace"),
                                                    QDir::homePath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    // workspace
    if(!directory.isEmpty())
    {  
        closeAll();
        bookmarks->removeAll();
        pAssemblyFile = asmNotSelected;
        ui->lAssemblyFile->setText(asmNotSelected);
        openWorkspace(directory);
        AddRecentWorkspace(directory);
    }
}

void MainWindow::UpdateRecentWorkspaces()
{
    QStringList ws = settings.value("recentWorkspaces").toStringList();
    int countWs = qMin(ws.size(), pMaxRecentWorkspace);

    QFile f(":/res/welcome/welcome.htm");
    if(f.open(QFile::ReadOnly))
    {
        QString s = f.readAll();
        ui->brHome->clear();
        if(countWs>0)
        {
            for(int i=0; i<countWs; i++)
                s.append (QString("<a href=\"#?%1\" data-toggle=\"tooltip\" title=\"Remove this from recent workspaces\">&nbsp;&nbsp;<img src=\":/res/images/deletews.png\">&nbsp;&nbsp;</a><a href=\"#*%1\" data-toggle=\"tooltip\" title=\"Open workspace\"><span style=\"font-size:10pt; text-decoration: none; color:#000000;\">%2. </span><span style=\"font-size:12pt; text-decoration: none; color:#6161d0;\">%1</span></a><br><br>").arg(ws[i]).arg(i+1));
        }
        else
        {
            s.append("No recent workspace");
        }
        s.append("</body></html>");
        ui->brHome->insertHtml(s);

        f.close();
    }

}

void MainWindow::RemoveFromRecentWorkspace(QString wspc)
{
    QStringList ws = settings.value("recentWorkspaces").toStringList();
    ws.removeAll(wspc);
    settings.setValue("recentWorkspaces", ws);
    UpdateRecentWorkspaces();
}

void MainWindow::AddRecentWorkspace(QString wspc)
{
    // recent workspace
    QStringList ws = settings.value("recentWorkspaces").toStringList();
    ws.removeAll(wspc);
    ws.prepend(wspc);
    while(ws.size()>pMaxRecentWorkspace) ws.removeLast();
    settings.setValue("recentWorkspaces", ws);
}

void MainWindow::setActionsEnable(bool value)
{
    ui->actionNew->setEnabled(value);
    ui->actionOpen->setEnabled(value);
    setActionsEnableForDoc(value);
}

void MainWindow::setActionsEnableForDoc(bool value)
{
    ui->actionSave->setEnabled(value);
    ui->actionSaveAll->setEnabled(value);
    ui->actionSave_as->setEnabled(value);
    ui->actionClose->setEnabled(value);
    ui->actionClose_All->setEnabled(value);
    ui->actionCloseAllButThis->setEnabled(value);
//    ui->actionBuild_this->setEnabled(value);
//    ui->actionBuild_and_Run->setEnabled(value);
//    ui->actionBuild_as_binary->setEnabled(value);
//    ui->actionDebugger->setEnabled(value);
    ui->actionGenerate_Disk_Directive->setEnabled(value);
    ui->actionGenerate_File_Directive->setEnabled(value);
    ui->actionInsert_BASIC_SYS_Line->setEnabled(value);
    ui->actionPuts_A_Breakpoint->setEnabled(value);

}

void MainWindow::on_brHome_anchorClicked(const QUrl &arg1)
{
    QString ahref = arg1.fragment();
    if(ahref=="selectworkspace")
    {
        emit on_actionSelect_Workspace_triggered();
        return;
    }

    // delete workspace ?
    // open workspace *
    QString firstChar = arg1.fragment().left(1);
    QString word = arg1.fragment().remove(firstChar);

    if(firstChar=="?")
    {
        if(QMessageBox::question(this, "Remove?", "Are you sure you want to remove <b>\""+word+"\"</b> from recent workspaces?") == QMessageBox::Yes)
        {
            RemoveFromRecentWorkspace(word);
        }
    }
    else if (firstChar == "*")
    {
        closeAll();
        bookmarks->removeAll();
        pAssemblyFile = asmNotSelected;
        ui->lAssemblyFile->setText(asmNotSelected);
        openWorkspace(word);
        AddRecentWorkspace(word);
    }
}

// save as...
void MainWindow::on_actionSave_as_triggered()
{
    if(!selectedIndex.isValid()) return;
    QString newFile = saveAsFile(ui->tabWidget->currentIndex());
    if(!newFile.isEmpty())
    {
        closeFile(ui->tabWidget->currentIndex());
        openFileFromPath(newFile);
    }
}

// close all
void MainWindow::on_actionClose_All_triggered()
{
    closeAll();
}

// save all
void MainWindow::on_actionSaveAll_triggered()
{
    int opentabindex = ui->tabWidget->currentIndex();
    for (int i=0;i<ui->tabWidget->count();i++)
    {
        ui->tabWidget->setCurrentIndex(i);
        emit on_actionSave_triggered();
    }
    ui->tabWidget->setCurrentIndex(opentabindex);
}

// exit
void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::printOutput(const QString &message, const QColor &color)
{
    QTextCursor cursor = QTextCursor(ui->tOutput->document());
    cursor.movePosition(QTextCursor::End);
    ui->tOutput->setTextCursor(cursor);
    ui->tOutput->setTextColor(color);
    ui->tOutput->insertPlainText(message);
    cursor.movePosition(QTextCursor::End);
    ui->tOutput->setTextCursor(cursor);
}

void MainWindow::printOutputWithTime(const QString &message, const QColor &color)
{
    QTime currentTime = QTime::currentTime();
    QString timeString = currentTime.toString("[HH:mm:ss.zzz] ");
    printOutput(timeString + message, color);
}

bool MainWindow::build(bool afterbuildthenrun, bool binary)
{
   // if(ui->tabWidget->count()==0) return false;
    writeSettings();
    OpenCode();
    ui->dwOutput->show();
    ui->dwIssues->show();

    // If not kickass.jar file exists, app not responding
    if(!QFile::exists(pKickAssJar))
    {
        printOutputWithTime("Kickass.jar file not exist. Check your settings.\n", QColor(0xf3, 0x34, 0x34));
        return false;
    }

    //save all
    emit on_actionSaveAll_triggered();

    if(pAssemblyFile.right(4).toLower() != ".asm")
    {
        printOutputWithTime("Assembly file not selected\n", QColor(0xf3, 0x34, 0x34));
        return false;
    }

//    int crIndex = ui->tabWidget->currentIndex();
//    Tab *tab = /*(Tab *)*/ static_cast<Tab*>( ui->tabWidget->widget(crIndex));
//    QString filePath = tab->getCurrentFilePath();

    filePath = QDir::cleanPath(workspacePath + QDir::separator() + pAssemblyFile);
    QFileInfo fi(filePath);
    QString ext = fi.completeSuffix();
   // filePath = fi.filePath();

    if(!((ext=="asm")||(ext=="s")))
    {
        printOutputWithTime("Unable to build this file [" + pAssemblyFile + "]\n", QColor(0xf3, 0x34, 0x34));
        return false;
    }

    QElapsedTimer timer;
    timer.start();

    if ((filePath.isEmpty()) || (!fi.exists())) {
        ui->statusbar->showMessage("Build error", TIMEOUT);
        printOutputWithTime("File not found: " + filePath + "\n", QColor(0xf3, 0x34, 0x34));
    } else {
        printOutputWithTime("Building for " + pAssemblyFile + "\n", Qt::darkGray);

        QCoreApplication::processEvents();
        QProcess asmProcess;
        QString asmOutput = QDir::cleanPath(Common::appConfigDir() + QDir::separator() + "compileoutput.txt");
        asmProcess.setStandardOutputFile(asmOutput);
        asmProcess.setStandardErrorFile(asmOutput, QIODevice::Append);

        asmProcess.setWorkingDirectory(fi.absolutePath());

        QStringList asmArguments;
        asmArguments << "-jar" << pKickAssJar << filePath << "-cfgfile" << Common::appConfigDir()+"/ide65xx.cfg";
        if(afterbuildthenrun)
        {
            QString tempEmulatorParameters = pEmulatorParameters;
            asmArguments << "-execute" << ((pEmulator.contains(".jar"))? pJRE + " -jar ": "") + pEmulator +" "+ tempEmulatorParameters.remove("<file>");
        }
        if(binary)
        {
            asmArguments << "-binfile";
        }

        QString cmd = pJRE;
        for(int c=0; c<asmArguments.length();c++) cmd+= " " + asmArguments[c];
        printOutputWithTime(cmd+"\n", Qt::darkGray);

        asmProcess.start(pJRE, asmArguments);
        asmProcess.waitForFinished();

        if (asmProcess.error() != QProcess::UnknownError)
        {
            printOutputWithTime("Unable to start assembler. Check your settings.\n", QColor(0xf3, 0x34, 0x34));
            return false;
        }

        QFile logFile;
        logFile.setFileName(asmOutput);
        logFile.open(QIODevice::ReadOnly);
        QTextStream log(&logFile);

        tabifyDockWidget(ui->dwIssues, ui->dwOutput);
        clearIssues();
        bool showmem = settingsWin->getCheckShowMemCmdLine();
        ui->mvWarning->setVisible(!showmem);
        ui->memoryViewer->reset();

        while(!log.atEnd()) {
            QString currentLine = log.readLine();
            printOutput(currentLine+"\n", QColor(0x61, 0x61, 0xd0));
            grabIssues(currentLine, log);
            if(showmem) grabMemoryView(currentLine, log);
        }
        logFile.close();

        if(asmProcess.exitCode() == 0)
        {
            printOutputWithTime("Built successfully.\n", Qt::darkGreen);
            if(ui->dwMemoryViewer->isVisible()) memoryViewerCurrentCoordinateChanged(0, 0);
            if(ui->dwHexEditor->isVisible())
            {
                curHexFilename = QDir::cleanPath(fi.absolutePath() + QDir::separator() + settingsWin->getBuildDir() + QDir::separator() + fi.fileName().remove(fi.completeSuffix()) + ((binary) ? "bin":"prg"));
                loadHexFile();
            }
        }
        else
            printOutputWithTime("Error while building '" + filePath + "'\n", QColor(0xf3, 0x34, 0x34));

        printOutputWithTime("Elapsed time: " + QString::number(timer.elapsed()) + " ms.\n", Qt::darkCyan);

    }

    return true;
}

void MainWindow::clearIssues()
{
    while(ui->tIssues->rowCount()>0) ui->tIssues->removeRow(0);
}

void MainWindow::grabIssues(QString currentLine, QTextStream &tStr)
{
    QString sIssue;
    QString sLine;
    QString sFilename;
    // Error:
    if(currentLine.left(6)=="Error:"){
        // issue
        sIssue = currentLine;
        sIssue.remove("Error:");
        currentLine = tStr.readLine();
        printOutput(currentLine+"\n", QColor(0x61, 0x61, 0xd0));
        if(currentLine.left(7)=="at line")
        {
            // line
            sLine = currentLine;
            sLine.remove("at line");
            sLine = sLine.split(',')[0].trimmed();
            // filename
            sFilename = currentLine.split(" in ")[1].trimmed();
        }
        addIssue(sIssue, sFilename, sLine);
    }
    // Got ?? errors while executing:
    else if(currentLine.contains("errors while executing:"))
    {
        while(true)
        {
            currentLine = tStr.readLine();
            printOutput(currentLine+"\n", QColor(0x61, 0x61, 0xd0));
            if ((currentLine.trimmed()=="")||(currentLine.trimmed()=="...")) break;
            QStringList s1 = currentLine.split(')');
            sIssue = s1[1].trimmed();
            s1[0].remove("(");
            QStringList s2 = (s1[0].trimmed()).split(' ');
            sLine = (s2[s2.count()-1]).split(':')[0];
            s1[0].remove(s2[s2.count()-1]);
            sFilename = s1[0].trimmed();
            addIssue(sIssue, sFilename, sLine);
        }
        return;
    }
    // goto first issue in editor
    if(ui->tIssues->rowCount()>0)
    {
        ui->tIssues->selectRow(0);
        emit on_tIssues_cellDoubleClicked(0,0);
    }
}

void MainWindow::addIssue(QString issue, QString filename, QString line)
{
    if((filename.contains("\\"))||(filename.contains("/")))
    {
        filename.replace('\\','/');
    }
    else
    {
        Tab *tab = static_cast<Tab*>( ui->tabWidget->widget(ui->tabWidget->currentIndex()));
        QFileInfo fi(tab->getCurrentFilePath());
        filename =  fi.absolutePath() + "/" + filename;
        filename.replace('\\','/');
    }
    tabifyDockWidget(ui->dwOutput, ui->dwIssues);
    int lastRow = ui->tIssues->rowCount();
    ui->tIssues->insertRow(lastRow);
    ui->tIssues->setItem(lastRow, 2, new QTableWidgetItem(line));
    ui->tIssues->setItem(lastRow, 1, new QTableWidgetItem(filename + QString(10, ' ')));
    ui->tIssues->setItem(lastRow, 0, new QTableWidgetItem(issue.trimmed() + QString(10, ' ')));
    ui->tIssues->resizeColumnsToContents();
}

void MainWindow::grabMemoryView(QString currentLine, QTextStream &tStr)
{
    if(currentLine.contains("segment:"))
    {
        const QColor colors[4] = {
            QColor(0x00, 0x80, 0x80),
            QColor(0x00, 0x80, 0x00),
            QColor(0x00, 0x00, 0x80),
            QColor(0x80, 0x00, 0x00)
        };
        int c = 0;
        while(true)
        {
            currentLine = tStr.readLine();
            printOutput(currentLine+"\n", QColor(0x61, 0x61, 0xd0));
            //  $0801-$080c Basic
            if(currentLine.trimmed().isEmpty()) break;
            QStringList sl= currentLine.trimmed().split("-$");
            bool Ok = false;
            int startAdr = sl[0].right(4).toInt(&Ok, 16);
            if(!Ok) break;
            int endAdr = sl[1].left(4).toInt(&Ok, 16);
            if(!Ok) break;
            ui->memoryViewer->poke(startAdr, endAdr, colors[c%4]);
            c++;
        }

    }
}

void MainWindow::prepareBeforeOpen(QString filename)
{
    // is file opened?
    for (int i=0;i<ui->tabWidget->count();i++)
    {
        Tab *tabForOpened = static_cast<Tab*>(ui->tabWidget->widget(i));
        if(tabForOpened->getCurrentFilePath() == filename)
        {
            ui->tabWidget->setCurrentIndex(i);
            return;
        }
    }

    // open file by extension with external app
    QFileInfo fi(filename);
    QString ext = fi.completeSuffix();

    if(     (ext=="asm")||
            (ext=="lib")||
            (ext=="inc")||
            (ext=="txt")||
            (ext=="sym")||
            (ext=="dbg")||
            (ext=="s")||
            (ext=="a")||
            (ext=="vs")
        )
    {
        openFileFromPath(filename);
        SetCursorPos();
    }
}

void MainWindow::on_tIssues_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column)

    quint64 line = ui->tIssues->item(row, 2)->text().toULongLong();
    if(line < 1) return;
    QString filename = ui->tIssues->item(row, 1)->text().trimmed();

    Tab *tab = nullptr;
    // is file opened?
    bool isopen = false;
    for (int i=0;i<ui->tabWidget->count();i++)
    {
        tab = static_cast<Tab*>(ui->tabWidget->widget(i));
        if(tab->getCurrentFilePath() == filename)
        {
            ui->tabWidget->setCurrentIndex(i);
            isopen = true;
            break;
        }
    }

    if(!isopen)
    {
        openFileFromPath(filename);
        SetCursorPos();
        tab = static_cast<Tab*>(ui->tabWidget->widget(ui->tabWidget->currentIndex()));
    }

    tab->code->moveCursor(QTextCursor::Start);
    for (quint64 i=0;i<line-1;i++) {
        tab->code->moveCursor(QTextCursor::Down);
    }
    tab->code->setFocus();

}

void MainWindow::RefreshOpenDocuments()
{
    int tc = ui->tabWidget->count();
    ui->listOpenDocuments->clear();
    opendocuments.clear();

    if(tc<1) return;
    int selected = ui->tabWidget->currentIndex();
    for(int i=0;i<tc;i++) opendocuments.append(ui->tabWidget->tabText(i));
    ui->listOpenDocuments->addItems(opendocuments);
    ui->listOpenDocuments->setCurrentRow(selected);
}

// changed tab
void MainWindow::on_tabWidget_currentChanged(int index)
{
    setTopToolbar(index);
    ui->listOpenDocuments->setCurrentRow(index);
    SetCursorPos();
}

// close find dialog
void MainWindow::on_bFindClose_clicked()
{
    ui->findDialog->setVisible(false);
    emit find(QString(), Qt::CaseSensitive, true, false);
    (static_cast<Tab*>( ui->tabWidget->currentWidget()))->code->setFocus();
}

// disabled/enabled find/replace buttons
void MainWindow::on_tFind_textChanged(const QString &arg1)
{
    ui->bFindNext->setEnabled(!arg1.isEmpty());
    ui->bFindAll->setEnabled(!arg1.isEmpty());
    ui->bReplace->setEnabled(!arg1.isEmpty());
    ui->bReplaceAll->setEnabled(!arg1.isEmpty());
}

void MainWindow::find(const QString &pattern, Qt::CaseSensitivity cs, bool all, bool replace, const QString &replaceText)
{
    // Clear all highlights and disable highlighting of current line
    for (int i = 0; i < ui->tabWidget->count(); i++)
    {
        CodeEditor *code = (static_cast<Tab*>( ui->tabWidget->widget(i))->code);
        disconnect(code, SIGNAL(cursorPositionChanged()), code, SLOT(highlightCurrentLine()));
        code->setExtraSelections(QList<QTextEdit::ExtraSelection>());
    }
    // Restore highlight
    if (pattern.isEmpty())
    {
        for (int i = 0; i < ui->tabWidget->count(); i++)
        {
            CodeEditor *code = (static_cast<Tab*>( ui->tabWidget->widget(i))->code);
            connect(code, SIGNAL(cursorPositionChanged()), code, SLOT(highlightCurrentLine()));
            code->highlightCurrentLine();
        }
    }
    else
    {
        //If true, find all
        if (all)
        {
            for (int i = 0; i < ui->tabWidget->count(); i++)
            {
                QTextEdit::ExtraSelection selection;
                QList<QTextEdit::ExtraSelection> extraSelections;
                selection.format.setBackground(QBrush(Qt::green));
                QTextDocument *document = (static_cast<Tab*>( ui->tabWidget->widget(i)))->getCodeDocument();
                QTextCursor newCursor = QTextCursor(document);
                CodeEditor *code = (static_cast<Tab*>( ui->tabWidget->widget(i))->code);

                while (!newCursor.isNull() && !newCursor.atEnd())
                {
                    if (cs == Qt::CaseSensitive)
                        newCursor = document->find(pattern, newCursor, QTextDocument::FindCaseSensitively);
                    else
                        newCursor = document->find(pattern, newCursor, nullptr);
                    // Replace mode
                    if (replace && i == ui->tabWidget->currentIndex())
                    {
                        //newCursor.removeSelectedText();
                        newCursor.insertText(replaceText);
                    }
                    if (!newCursor.isNull()) {
                        selection.cursor = newCursor;
                        extraSelections.append(selection);
                    }
                }
                // Highlight all
                code->setExtraSelections(extraSelections);
            }
        }
        //Find next only
        else
        {
            QTextEdit::ExtraSelection selection;
            QList<QTextEdit::ExtraSelection> extraSelections;
            selection.format.setBackground(QBrush(Qt::green));
            QTextDocument *document = (static_cast<Tab*>( ui->tabWidget->currentWidget()))->getCodeDocument();
            CodeEditor *code = (static_cast<Tab*>( ui->tabWidget->currentWidget()))->code;
            static QTextCursor newCursor(document);
            // if documents differ, cursor is ignored in QTextDocument::find()
            if (replace)
            {
                //newCursor.removeSelectedText();
                newCursor.insertText(replaceText);
            }
            if (cs == Qt::CaseSensitive)
                newCursor = document->find(pattern, newCursor, QTextDocument::FindCaseSensitively);
            else
                newCursor = document->find(pattern, newCursor, nullptr);
            // Continue from start
            if (newCursor.isNull())
            {
                if (cs == Qt::CaseSensitive)
                    newCursor = document->find(pattern, newCursor, QTextDocument::FindCaseSensitively);
                else
                    newCursor = document->find(pattern, newCursor, nullptr);
            }
            if (!newCursor.isNull())
            {
                selection.cursor = newCursor;
                extraSelections.append(selection);
                QTextCursor cursor = newCursor;
                cursor.clearSelection();
                code->setTextCursor(cursor);
            }
            code->setExtraSelections(extraSelections);
        }
    }
}

void MainWindow::ExecuteAppWithFile(QString pApplication, QString additionalArgs)
{
    if(!QFile::exists(pApplication))
    {
        QMessageBox::information(this, "Error?", "Application not found. Check your settings.", QMessageBox::Ok);
        return;
    }
    QCoreApplication::processEvents();

    printOutputWithTime(pApplication +" ", Qt::darkGray);
    if(additionalArgs.isEmpty())
    {
        printOutput(" "+filePath+"\n", Qt::darkGray);
        p.startDetached(pApplication, QStringList() << filePath);

    }
    else
    {
        QStringList parameters;
        QFileInfo fi(filePath);
        parameters = additionalArgs.split(' ');

        for(int i=0;i<parameters.length();i++)
        {
            // compress
            if(parameters[i].contains("<infile>"))
                parameters[i] = parameters[i].replace("<infile>", filePath);

            if(parameters[i].contains("<outfile>"))
            {
                parameters[i] = parameters[i].replace("<outfile>", fi.filePath().remove(fi.fileName())+fi.baseName()+"-compressed.prg");
            }

            // create d64 via c1541
            if(parameters[i].contains("<name>"))
                parameters[i] = parameters[i].replace("<name>", fi.baseName());

            if(parameters[i].contains("<pathname>"))
            {
                parameters[i] = parameters[i].replace("<pathname>", fi.filePath().remove(fi.fileName())+fi.baseName());
            }

            if(parameters[i].contains("<dbgname>"))
            {
//                int crIndex = ui->tabWidget->currentIndex();
//                Tab *tab = /*(Tab *)*/ static_cast<Tab*>( ui->tabWidget->widget(crIndex));
//                QString filePath = tab->getCurrentFilePath();
                QSettings cmdline(Common::appConfigDir()+"/cmdline.ini", QSettings::IniFormat);
                QFileInfo fid(QDir::cleanPath(workspacePath + QDir::separator() + pAssemblyFile));
                QString fOutDir = "";
                if(cmdline.value("odir", false).toBool())
                    fOutDir = cmdline.value("odirtext", "build").toString()+"/";
                parameters[i] = parameters[i].replace("<dbgname>", fid.filePath().remove(fid.fileName())+fOutDir+fid.baseName());
            }

            // sid player
            if(parameters[i].contains("<sidfile>"))
            {
                parameters[i] = parameters[i].replace("<sidfile>", filePath);
            }

            // emulator parameters
            if(parameters[i].contains("<file>"))
            {
                parameters[i] = parameters[i].replace("<file>", filePath);
            }

            printOutput(parameters[i]+" ", Qt::darkGray);
        }
        printOutput("\n", Qt::darkGray);

        // If pApplication is JAR file
        if(pApplication.contains(".jar"))
        {
            parameters.prepend(pApplication);
            parameters.prepend("-jar");
            pApplication = pJRE;
        }
        p.startDetached(pApplication, parameters);
    }
}

QAbstractItemModel *MainWindow::modelFromFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return new QStringListModel(completer);

#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
    QStringList words;

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        if (!line.isEmpty())
            words << QString::fromUtf8(line.trimmed());
    }

#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif
    return new QStringListModel(words, completer);
}

void MainWindow::HideDocks()
{
    dockState = saveState();
    ui->dwOutput->setVisible(false);
    ui->dwIssues->setVisible(false);
    ui->dwProjectExplorer->setVisible(false);
    ui->dwOpenDocument->setVisible(false);
    ui->dwBookmarks->setVisible(false);
    ui->dwHexEditor->setVisible(false);
    ui->dwMemoryViewer->setVisible(false);
    ui->menuView->setEnabled(false);
    ui->menuEdit->setEnabled(false);
    setActionsEnable(false);
    statusCurPosInfo->setVisible(false);
}

void MainWindow::RestoreDocks()
{
    restoreState(dockState);
    ui->menuView->setEnabled(true);
    if(ui->tabWidget->count()>0)
    {
        ui->menuEdit->setEnabled(true);
        statusCurPosInfo->setVisible(true);
        setActionsEnable(true);
    }
}

// Run in emulator
void MainWindow::RunInEmulator()
{
    ExecuteAppWithFile(pEmulator, pEmulatorParameters);
}

// Compress prg
void MainWindow::Compress()
{
    ExecuteAppWithFile(pCompressionUtility, pCompressionParameters);
}

// open prg, d64, d71, d81 via dirmaster
void MainWindow::OpenWithDirMaster()
{
    ExecuteAppWithFile(pDirMaster);
}

void MainWindow::PlayWithSIDPlayer()
{
    ExecuteAppWithFile(pSIDPlayer, pSIDPlayerParameters);
}

// create d64 from prg via c1541
void MainWindow::CreateD64Image()
{
    ExecuteAppWithFile(pC1541, "-format \"<name>,01\" d64 <pathname>.d64 -attach <pathname>.d64 -write <pathname>.prg <name>");
}

void MainWindow::RemoveFileOrFolder()
{
    if(!selectedIndex.isValid()) return;
    QFileInfo fileinfo (filePath);
    if(fileinfo.isFile())
    {

        int ret = QMessageBox::question(this, "Are you sure?",
                                         "Remove file [" + filePath + "]?",
                                         QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

        if (ret == QMessageBox::Yes)
        {
            QFile file (filePath);
            for (int i=0;i<ui->tabWidget->count();i++)
            {
                Tab *tabForOpened = static_cast<Tab*>(ui->tabWidget->widget(i));
                if(tabForOpened->getCurrentFilePath() == filePath)
                {
                    closeFile(i);
                    break;
                }
            }
            if(file.remove())
            {
                ui->statusbar->showMessage("File removed", TIMEOUT);
            }

        }
    }
    else if(fileinfo.isDir())
    {
        int ret = QMessageBox::question(this, "Are you sure?",
                                         "Remove Directory [" + filePath + "]?",
                                         QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

        if (ret == QMessageBox::Yes)
        {
            QDir(filePath).removeRecursively();
        }

    }
}

void MainWindow::Rename()
{
    if(!selectedIndex.isValid()) return;
    QFileInfo fileinfo (filePath);
    bool ok;
    QString text;

    if(fileinfo.isFile())
    {
        text = QInputDialog::getText(this, "Rename File?",
                                             "File: "+fileinfo.fileName()+"\nEnter new filename:",
                                             QLineEdit::Normal,
                                             fileinfo.fileName(), &ok);

        if (ok && !text.isEmpty())
        {
            QFile file (filePath);
            for (int i=0;i<ui->tabWidget->count();i++)
            {
                Tab *tabForOpened = static_cast<Tab*>(ui->tabWidget->widget(i));
                if(tabForOpened->getCurrentFilePath() == filePath)
                {
                    closeFile(i);
                    break;
                }
            }
            if(file.rename(fileinfo.absolutePath()+"/"+text))
                ui->statusbar->showMessage("Filename Changed", TIMEOUT);
            else
                ui->statusbar->showMessage("Filename Not Changed", TIMEOUT);

        }
    }
    else if(fileinfo.isDir())
    {
        text = QInputDialog::getText(this, "Rename Folder?",
                                             "Folder: "+fileinfo.fileName()+"\nEnter new name of the directory:",
                                             QLineEdit::Normal,
                                             fileinfo.fileName(), &ok);

        if (ok && !text.isEmpty())
        {
            if(QDir(filePath).rename(fileinfo.absoluteFilePath(), fileinfo.dir().absolutePath() +"/"+ text))
                ui->statusbar->showMessage("Name Of The Directory Changed", TIMEOUT);
            else
                ui->statusbar->showMessage("Name Of The Directory Not Changed", TIMEOUT);

        }

    }
}

void MainWindow::OpenWithHexEditor()
{
    curHexFilename = filePath;
    loadHexFile();
    ui->dwHexEditor->show();
    ui->statusbar->showMessage("Hex file loaded", TIMEOUT);
}

void MainWindow::SetAssemblyFile()
{
    pAssemblyFile = filePath.remove(workspacePath);
    ui->lAssemblyFile->setText(pAssemblyFile);
    settings.setValue("AssemblyFile", pAssemblyFile);
    settings.sync();
}

void MainWindow::OpenCode()
{ 
    RestoreDocks();
    ui->stackedWidget->setCurrentWidget(ui->pCode);
}

void MainWindow::OpenHome()
{
    if(ui->stackedWidget->currentWidget() == ui->pCode)
        HideDocks();
    ui->stackedWidget->setCurrentWidget(ui->pHome);
}

void MainWindow::OpenHelp()
{
    if(ui->stackedWidget->currentWidget() == ui->pCode)
        HideDocks();
    ui->stackedWidget->setCurrentWidget(ui->pHelp);
}

void MainWindow::removeBookmark()
{
    // show tab->code
    int crIndex = ui->tabWidget->currentIndex();
    emit bookmarkClicked(bookmarks->selectionModel()->currentIndex());

    int selectedindex = bookmarks->currentIndex().row();
    quint64 lnum = bookmarks->getLineNumber(selectedindex);
    Tab *tab = /*(Tab *)*/ static_cast<Tab*>( ui->tabWidget->widget(ui->tabWidget->currentIndex()));
    if(tab->code->bookmarks.contains(lnum))
    {
        tab->code->bookmarks.removeOne(lnum);
        tab->code->update();
        bookmarks->removeItem(selectedindex);
    }

    ui->tabWidget->setCurrentIndex(crIndex);
}

void MainWindow::removeAllBookmark()
{
    Tab *tab = nullptr;// = /*(Tab *)*/ static_cast<Tab*>( ui->tabWidget->widget(ui->tabWidget->currentIndex()));
    for (int i=0;i<ui->tabWidget->count();i++) {
        tab = static_cast<Tab*>( ui->tabWidget->widget(i));
        tab->code->bookmarks.clear();
        tab->code->update();
    }

    bookmarks->removeAll();
}

void MainWindow::editBookmark()
{
    bool ok;
    int crBookmarkIndex = bookmarks->currentIndex().row();
    QString text = QInputDialog::getText(this, tr("Edit Bookmark"),
                                         tr("Note text:"), QLineEdit::Normal,
                                         bookmarks->getItemText(crBookmarkIndex), &ok);
    if (ok && !text.isEmpty()) bookmarks->setItemText(crBookmarkIndex, text);

}

void MainWindow::bookmarkClicked(const QModelIndex &index)
{
    Q_UNUSED(index)
    //qDebug() << index.data().toString();
    //qDebug() << QString::number(bookmarks->getLineNumber(bookmarks->currentIndex().row()));
    //qDebug() << bookmarks->getFilepath(bookmarks->currentIndex().row());

    int bookmarksCurrentIndex = bookmarks->currentIndex().row();
    quint64 bLineNumber = bookmarks->getLineNumber(bookmarksCurrentIndex);
    QString bFilePath = bookmarks->getFilepath(bookmarksCurrentIndex);

    if(!QFile::exists(workspacePath + bFilePath))
    {
        bookmarks->removeItem(bookmarksCurrentIndex);
        return;
    }

    Tab *tab = nullptr;
    bool opened = false;

    // if have opened tab
    if(ui->tabWidget->count()>0)
    {
        for (int i=0;i<ui->tabWidget->count();i++)
        {
            tab = static_cast<Tab*>(ui->tabWidget->widget(i));
            if(tab->getCurrentFilePath() == workspacePath + bFilePath)
            {
                ui->tabWidget->setCurrentIndex(i);
                opened = true;
                break;
            }
        }
    }

    if(!opened){
        openFileFromPath(workspacePath + bFilePath);
        SetCursorPos();
        tab = static_cast<Tab*>(ui->tabWidget->widget(ui->tabWidget->currentIndex()));
    }

    ui->tabWidget->setTabIcon(ui->tabWidget->currentIndex(), bookmarks->getIcon(bookmarksCurrentIndex));
    tab->code->moveCursor(QTextCursor::Start);
    for (quint64 i=0;i<bLineNumber-1;i++) {
        tab->code->moveCursor(QTextCursor::Down);
    }

    tab->code->setFocus();
}

void MainWindow::updateLineNumberSlot(quint64 currentLine, int delta)
{
    int crIndex = ui->tabWidget->currentIndex();
    Tab *tab = /*(Tab *)*/ static_cast<Tab*>( ui->tabWidget->widget(crIndex));

    QString topTextFilePath = tab->getCurrentFilePath().remove(workspacePath);

     if(delta<0)
     {
        for(int i=0;i<bookmarks->model()->rowCount();i++)
        {
            if(bookmarks->getFilepath(i) == topTextFilePath)
            {
                quint64 bline = bookmarks->getLineNumber(i);

                if((bline < (currentLine - delta))&&(bline > currentLine))
                {
                    bookmarks->removeItem(i);
                    tab->code->bookmarks.removeOne(bline);
                    i--;
                }
            }
        }
    }

    for(int i=0;i<bookmarks->model()->rowCount();i++)
    {
        if(bookmarks->getFilepath(i) == topTextFilePath)
        {
            quint64 bline = bookmarks->getLineNumber(i);

            if(bline >= ( (delta>0) ? currentLine-1 : currentLine ))
            {
                QString topText =  QString::number(bline + delta) + " : " + topTextFilePath;
                bookmarks->setTopText(i, topText);

                tab->code->bookmarks.removeOne(bline);
                tab->code->bookmarks.append(bline + delta);
            }
        }
    }

    tab->code->update();
}

void MainWindow::bookmarksChangedSlot(quint64 lineNumber, QString text, bool isAdded)
{
    int crIndex = ui->tabWidget->currentIndex();
    Tab *tab = /*(Tab *)*/ static_cast<Tab*>( ui->tabWidget->widget(crIndex));

    QString topTextFilePath = tab->getCurrentFilePath().remove(workspacePath);

    // add/remove bookmark
    if(isAdded)
    {
        QPixmap icon = ui->tabWidget->tabIcon(crIndex).pixmap(24,24);
        QString topText =  QString::number(lineNumber) + " : " + topTextFilePath;
        bookmarks->addItem(icon, text, topText);
        ui->dwBookmarks->show(); // show bookmark dockwidget only
    }
    else
    {
        for(int i=0;i<bookmarks->model()->rowCount();i++)
        {
            if(bookmarks->getFilepath(i) == topTextFilePath)
            {
                if(bookmarks->getLineNumber(i)== lineNumber)
                {
                    bookmarks->removeItem(i);
                    break;
                }
            }
        }

    }
}

void MainWindow::memoryViewerCurrentCoordinateChanged(int x, int y)
{
    lastMemoryViewX = x;
    lastMemoryViewY = y;

    if((x>ui->sbMemoryViewer->value())||(y>0x10000/ui->sbMemoryViewer->value())) return;

    int zoom = ui->horizontalSlider->value();

    int zww = ui->scaledMemoryViewer->width();
    int zwh = ui->scaledMemoryViewer->height();

    int zw = zww / zoom;
    int zh = zwh / zoom;

    int zx = x - zw / 2;
    int zy = y - zh / 2;

    if (zx < 0) zx = 0;
    if (zy < 0) zy = 0;

    QPixmap px = ui->memoryViewer->grab(QRect(zx, zy, zw, zh));
    if (sbMemoryViewerValueNotChanged)
    {
        QPainter painter(&px);
        painter.setPen(QColor(0xff, 0x00, 0xff));
        painter.drawPoint(x-zx,y-zy);
        painter.end();
    }
    else
    {
        sbMemoryViewerValueNotChanged = true;
    }

    ui->scaledMemoryViewer->setPixmap(px.scaled(zww, zwh, Qt::KeepAspectRatioByExpanding));
}

void MainWindow::memoryViewerCurrentAddressChanged(int)
{
    ui->lCurrentAddress->setText(ui->memoryViewer->getCurrentAddressString().toUpper());
}

// Debugger (vice or c64debuggger)
void MainWindow::on_actionDebugger_triggered()
{
    QFileInfo debuggerApp(pDebugger);
    QString dName = debuggerApp.baseName();
    QString dArgs = "";
    if(dName.left(3)=="C64") dArgs = "-autojmp -layout 10 -symbols <dbgname>.vs -wait 2500 -prg <dbgname>.prg"; // c64 debugger
    if(dName.left(3)=="x64") dArgs = "-moncommands <dbgname>.vs <dbgname>.prg"; // vice

    if((debuggerApp.isFile())&&(!dArgs.isEmpty()))
    {
        bool preDebugDump = settingsWin->getCmdLineDebugDump();
        bool preViceSymbols = settingsWin->getCmdLineViceSymbols();
        settingsWin->setCmdLinesEnabledForDebug();
        if(build())
        {
            printOutputWithTime("Starting Debugger ... \n", Qt::darkCyan);
            ExecuteAppWithFile(pDebugger, dArgs);
            settingsWin->setCmdLinesEnabledForDebug(preDebugDump, preViceSymbols);
        }
    }
    else
    {
        printOutputWithTime("Unable to start debugger application. Check your settings.\n", QColor(0xf3, 0x34, 0x34));
    }

}

void MainWindow::on_bFindNext_clicked()
{
    emit find(ui->tFind->text(), ui->cMatchCase->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive, false, false);
}

void MainWindow::on_bFindAll_clicked()
{
    emit find(ui->tFind->text(), ui->cMatchCase->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive, true, false);
}

void MainWindow::on_bReplace_clicked()
{
    emit find(ui->tFind->text(), ui->cMatchCase->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive, false, true, ui->tReplace->text());
}

void MainWindow::on_bReplaceAll_clicked()
{
    emit find(ui->tFind->text(), ui->cMatchCase->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive, true, true, ui->tReplace->text());
}

void MainWindow::on_actionSettings_triggered()
{
    readSettingsOptionsOnly();
    settingsWin->ReadCmdLineOption();
    settingsWin->restartRequired = false;
    if(settingsWin->exec() == QDialog::Accepted)
    {
        writeSettings();
        // if tab has opened source
        for (int i = 0; i < ui->tabWidget->count(); i++)
        {
            Tab *tab = static_cast<Tab*>( ui->tabWidget->widget(i));
            tab->code->setWordWrapMode((pWordWrap)? QTextOption::WordWrap : QTextOption::NoWrap);
            tab->code->setFont(QFont(pCodeFontName, pCodeFontSize));
            QFontMetrics fm(tab->code->font());
            tab->code->setTabStopDistance(fm.horizontalAdvance(' ')*pTabSize);
        }

        // restart required?
        if(settingsWin->restartRequired)
        {
            if(QMessageBox::question(this,"Restart?", "Restart the IDE 65XX application for this change to take effect?")==QMessageBox::Yes)
            {
                qApp->quit();
                QProcess::startDetached(qApp->arguments()[0], QStringList() << "restart");//application restart
            }
            settingsWin->restartRequired = false;
        }
    }

}

void MainWindow::writeSettings()
{
    // Size, Pos
    settings.setValue("AppSize", this->size());
    settings.setValue("AppPos", this->pos());
    // Dockstate
    settings.setValue("DockState", this->saveState());
    // Open Files
    QStringList files;
    for (int i = 0; i < ui->tabWidget->count(); i++)
    {
        Tab *tab = static_cast<Tab*>( ui->tabWidget->widget(i));
        files << tab->getCurrentFilePath();
    }
    settings.setValue("OpenFiles", QVariant::fromValue(files));
    // workspace
    settings.setValue("Workspace", workspacePath);
    settings.setValue("AssemblyFile", pAssemblyFile);

    // kits
    pJRE = settingsWin->getJRE();
    settings.setValue("JRE", pJRE);

    pKickAssJar = settingsWin->getKickAssJar();
    settings.setValue("KickAssJar", pKickAssJar);

    // kickass.cfg would be empty
    QString KickAssJarCopy = pKickAssJar;
    QFile file(KickAssJarCopy.remove(".jar", Qt::CaseInsensitive) + ".cfg");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << "# KickAss.cfg is changed by IDE 65XX\n";
        file.close();
    }

    pEmulator = settingsWin->getEmulator();
    settings.setValue("Emulator", pEmulator);

    pEmulatorParameters = settingsWin->getEmulatorParameters();
    settings.setValue("EmulatorParameters", pEmulatorParameters);

    pCompressionUtility = settingsWin->getCompressionUtility();
    settings.setValue("CompressionUtility", pCompressionUtility);

    pDirMaster = settingsWin->getDirMaster();
    settings.setValue("DirMaster", pDirMaster);

    pCompressionParameters = settingsWin->getCompressionParameters();
    settings.setValue("CompressionParameters", pCompressionParameters);

    pSIDPlayerParameters = settingsWin->getSIDPlayerParameters();
    settings.setValue("SIDPlayerParameters", pSIDPlayerParameters);

    pSIDPlayer = settingsWin->getSIDPlayer();
    settings.setValue("SIDPlayer", pSIDPlayer);

    pC1541 = settingsWin->getC1541();
    settings.setValue("C1541", pC1541);

    pDebugger = settingsWin->getDebugger();
    settings.setValue("Debugger", pDebugger);

    //---- general ----

    pOpenLastUsedFiles = settingsWin->getOpenLastUsedFiles();
    settings.setValue("OpenLastUsedFiles", pOpenLastUsedFiles);

    pMaxRecentWorkspace = settingsWin->getMaxRecentWorkspace();
    settings.setValue("MaxRecentWorkspace", pMaxRecentWorkspace);

    pTabSize = settingsWin->getTabSize();
    settings.setValue("TabSize", pTabSize);

    pCodeFontName = settingsWin->getCodeFontName();
    settings.setValue("CodeFontName", pCodeFontName);

    pCodeFontSize = settingsWin->getCodeFontSize();
    settings.setValue("CodeFontSize", pCodeFontSize);

    pWordWrap = settingsWin->getWordWrap();
    settings.setValue("WordWrap", pWordWrap);

    settings.sync();

    //bookmark list saved to ini file
    int bookmarkCount = bookmarks->model()->rowCount();
    bookmarkcfgfile.clear();
    bookmarkcfgfile.setValue("Count", bookmarkCount); // Count: bookmark count
    bookmarkcfgfile.setValue("0", workspacePath); // 0: workspace, 1...oo line|relative-file-path

    if(bookmarkCount>0)
    {
        for (int i=0;i<bookmarkCount;i++)
        {
            bookmarkcfgfile.setValue(QString::number(i+1), QString("%1|%2|%3").arg(bookmarks->getLineNumber(i)).arg(bookmarks->getFilepath(i)).arg(bookmarks->getItemText(i)));
        }
    }

    bookmarkcfgfile.sync();
}

void MainWindow::readSettings()
{
    // pos, Size
    resize(settings.value("AppSize", QSize(WINDOW_WIDTH, WINDOW_HEIGHT)).toSize());
    move(settings.value("AppPos", QPoint(100,100)).toPoint());
    // Dockstate
    const QVariant state = settings.value("DockState");
    if (state.isValid())
    {
        restoreState(state.toByteArray());
    }
    // workspace
    QString strWorkspace = settings.value("Workspace").toString();

    pOpenLastUsedFiles = settings.value("OpenLastUsedFiles", true).toBool();
    settingsWin->setOpenLastUsedFiles(pOpenLastUsedFiles);
    const QStringList files = settings.value("OpenFiles").toStringList();
    bWelcome = files.count() < 1;

    // Open Files
    if(strWorkspace != "?")
    {
        if(pOpenLastUsedFiles)
        {
            openWorkspace(strWorkspace);

            QTimer::singleShot(100, [=] {
                    foreach (const QString &file, files) {
                        if (QFile::exists(file)) {
                            filePath=file;
                            openFileFromPath(file);
                        }
                    }
            });

            setActionsEnable(true);
        }
    }
    else
    {
        setActionsEnable(false);
    }

    readSettingsOptionsOnly();

}

void MainWindow::readSettingsOptionsOnly()
{
    //---- kits ----
    pJRE = settings.value("JRE",
                      #ifdef Q_OS_WIN
                          "java.exe"
                      #else
                          "java"
                      #endif
                          ).toString();
    settingsWin->setJRE(pJRE);
    pKickAssJar = settings.value("KickAssJar", "KickAss.jar").toString();
    settingsWin->setKickAssJar(pKickAssJar);
    pEmulator = settings.value("Emulator",
                           #ifdef Q_OS_WIN
                               "x64.exe"
                           #else
                               "x64"
                           #endif
                               ).toString();
    settingsWin->setEmulator(pEmulator);
    pEmulatorParameters = settings.value("EmulatorParameters", "<file>").toString();
    settingsWin->setEmulatorParameters(pEmulatorParameters);
    pCompressionUtility = settings.value("CompressionUtility",
                                     #ifdef Q_OS_WIN
                                         "exomizer.exe"
                                     #else
                                         "exomizer"
                                     #endif
                                         ).toString();
    settingsWin->setCompressionUtility(pCompressionUtility);
    pDirMaster = settings.value("DirMaster", "DirMaster.exe").toString();
    settingsWin->setDirMaster(pDirMaster);
    pCompressionParameters = settings.value("CompressionParameters", "sfx basic -n <infile> -o <outfile>").toString();
    settingsWin->setCompressionParameters(pCompressionParameters);
    pC1541 = settings.value("C1541",
                        #ifdef Q_OS_WIN
                            "c1541.exe"
                        #else
                            "c1541"
                        #endif
                            ).toString();
    settingsWin->setC1541(pC1541);
    pDebugger = settings.value("Debugger",
                           #ifdef Q_OS_WIN
                               "x64.exe"
                           #else
                               "x64"
                           #endif
                               ).toString();
    pAssemblyFile = settings.value("AssemblyFile").toString();
    ui->lAssemblyFile->setText(pAssemblyFile);
    settingsWin->setDebugger(pDebugger);
    pMaxRecentWorkspace = settings.value("MaxRecentWorkspace", 10).toInt();
    settingsWin->setMaxRecentWorkspace(pMaxRecentWorkspace);
    pTabSize = settings.value("TabSize", 10).toInt();
    settingsWin->setTabSize(pTabSize);
    pCodeFontName = settings.value("CodeFontName", "Ubuntu Mono").toString();
    settingsWin->setCodeFontName(pCodeFontName);
    pCodeFontSize = settings.value("CodeFontSize", 12).toInt();
    settingsWin->setCodeFontSize(pCodeFontSize);
    settingsWin->setApplicationFont(settings.value("ApplicationFontName", "Ubuntu Mono").toString(), settings.value("ApplicationFontSize", 12).toInt());
    settingsWin->setApplicationTheme(settings.value("ApplicationStyle", 3).toInt());
    pWordWrap = settings.value("WordWrap", false).toBool();
    settingsWin->setWordWrap(pWordWrap);
    pSIDPlayer = settings.value("SIDPlayer",
                            #ifdef Q_OS_WIN
                                "vsid.exe"
                            #else
                                "vsid"
                            #endif
                                ).toString();
    settingsWin->setSIDPlayer(pSIDPlayer);
    pSIDPlayerParameters = settings.value("SIDPlayerParameters", "<sidfile>").toString();
    settingsWin->setSIDPlayerParameters(pSIDPlayerParameters);

}

// About
void MainWindow::on_actionAbout_triggered()
{
    About ad(this);
    ad.exec();
}

// Build this
void MainWindow::on_actionBuild_this_triggered()
{
    build();
}

// Build & Run
void MainWindow::on_actionBuild_and_Run_triggered()
{
    build(true);
}

// Build as binary
void MainWindow::on_actionBuild_as_binary_triggered()
{
    build(false, true);
}

// file contextmenu on treeview
void MainWindow::on_tvWorkspaceFiles_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->tvWorkspaceFiles->indexAt(pos);

    if (index.isValid())
    {
        filePath = m_ptrModelForTree->filePath(index);
        selectedIndex = index;
        tabicon = m_ptrModelForTree->fileIcon(index);
        menuC1541->setVisible(false);
        menuEmulator->setVisible(false);
        menuExomizer->setVisible(false);
        menuDirmaster->setVisible(false);
        menuSidPlayer->setVisible(false);
        menuHexEditor->setVisible(false);
        menuAssemblyFile->setVisible(false);

        QFileInfo fi(filePath);
        QString ext = fi.completeSuffix();

        if(fi.isFile())
        {
            menuDelete->setText("Delete File");
            menuRename->setText("Rename file");
        }
        else
        {
            menuDelete->setText("Delete Folder");
            menuRename->setText("Rename Directory");
        }

        if((ext=="prg")||(ext=="bin"))
        {
            menuC1541->setVisible(true);
            menuEmulator->setVisible(true);
            menuExomizer->setVisible(true);
            menuDirmaster->setVisible(true);
            menuHexEditor->setVisible(true);
        }

        if(ext=="sid")
        {
            menuSidPlayer->setVisible(true);
        }

        if((ext=="d64")||(ext=="d71")||(ext=="d81"))
        {
            menuEmulator->setVisible(true);
            menuDirmaster->setVisible(true);
        }

        if(ext=="asm")
        {
            menuAssemblyFile->setVisible(true);
        }


        fileContextMenu->exec(ui->tvWorkspaceFiles->viewport()->mapToGlobal(pos));

    }
}

void MainWindow::on_bDescription_clicked()
{
    ui->brHelp->setSource(QUrl("help.htm#description"));
}

void MainWindow::on_bIllegal_clicked()
{
    ui->brHelp->setSource(QUrl("help.htm#illegal"));
}

void MainWindow::on_bStandard_clicked()
{
    ui->brHelp->setSource(QUrl("help.htm#standart"));
}

void MainWindow::on_b65C02opcodes_clicked()
{
    ui->brHelp->setSource(QUrl("help.htm#65c02opcodes"));
}

void MainWindow::on_bAssemblerDirectives_clicked()
{
    ui->brHelp->setSource(QUrl("help.htm#AssemblerDirectives"));
}

void MainWindow::on_bPreprocessorDirectives_clicked()
{
    ui->brHelp->setSource(QUrl("help.htm#PreprocessorDirectives"));
}

void MainWindow::on_bValueTypes_clicked()
{
    ui->brHelp->setSource(QUrl("help.htm#ValueTypes"));
}

void MainWindow::on_bStandardMacros_clicked()
{
    ui->brHelp->setSource(QUrl("help.htm#StandardMacros"));
}

void MainWindow::on_actionGenerate_File_Directive_triggered()
{
    TFileDirective fd(this);
    fd.exec();
    QString strFd = fd.getFileDirective();
    fd.deleteLater();
    if(strFd.isEmpty()) return;
    insertCode(strFd, QTextCursor::Left);
}

void MainWindow::on_actionGenerate_Disk_Directive_triggered()
{
    TDiskDirective dd(this);
    dd.exec();
    QString strDd = dd.getDiskDirective();
    dd.deleteLater();
    if(strDd.isEmpty()) return;
    insertCode(strDd);
}

void MainWindow::insertCode(QString code, QTextCursor::MoveOperation operation)
{
    Tab *tab = static_cast<Tab*>( ui->tabWidget->widget(ui->tabWidget->currentIndex()));
    QTextCursor tmpCursor = tab->code->textCursor();

    tmpCursor.movePosition(operation, QTextCursor::MoveAnchor, tmpCursor.columnNumber());
    tab->code->setTextCursor(tmpCursor);
    tab->code->insertPlainText(code);
}

void MainWindow::on_listOpenDocuments_itemSelectionChanged()
{
    ui->tabWidget->setCurrentIndex(ui->listOpenDocuments->currentRow());
}

void MainWindow::on_actionKick_Assembler_Web_Manual_triggered()
{
    QDesktopServices::openUrl(QUrl("http://www.theweb.dk/KickAssembler/webhelp/content/cpt_Introduction.html"));
}

void MainWindow::on_actionInsert_BASIC_SYS_Line_triggered()
{
    insertCode("BasicUpstart2(start)\nstart:\n", QTextCursor::Left);
}

void MainWindow::on_actionPuts_A_Breakpoint_triggered()
{
    insertCode(".break\n", QTextCursor::Left);
}

void MainWindow::on_actionWelcome_triggered()
{
    OpenHome();
}

void MainWindow::on_actionCode_triggered()
{
    OpenCode();
}

void MainWindow::on_actionClear_Output_triggered()
{
    ui->tOutput->clear();
}

void MainWindow::on_tFind_returnPressed()
{
    emit on_bFindAll_clicked();
}

void MainWindow::on_sbMemoryViewer_valueChanged(int addressPerLine)
{
    ui->memoryViewer->setLineWidth(addressPerLine);
    if((lastMemoryViewX!=-1)&&(lastMemoryViewY!=-1))
    {
        sbMemoryViewerValueNotChanged = false;
        ui->lCurrentAddress->setText("????");
        memoryViewerCurrentCoordinateChanged(lastMemoryViewX, lastMemoryViewY);
    }
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->lZoomValue->setText(QString("X%1").arg(value));
    if((lastMemoryViewX!=-1)&&(lastMemoryViewY!=-1))
    {
        sbMemoryViewerValueNotChanged = false;
        ui->lCurrentAddress->setText("????");
        memoryViewerCurrentCoordinateChanged(lastMemoryViewX, lastMemoryViewY);
    }
}

void MainWindow::on_actionReport_An_Issue_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/emartisoft/IDE65XX/issues"));
}

void MainWindow::dataChanged()
{
    hexActionEnable();
}

void MainWindow::setOverwriteMode(bool mode)
{
    hexEdit->setOverwriteMode(!mode);
}

void MainWindow::bytesperlineValueChanged(int value)
{
    hexEdit->setBytesPerLine(value);
}

void MainWindow::hexFileFindReplace()
{
    hexSearchDialog->show();
}

void MainWindow::hexFileRedo()
{
    hexEdit->redo();
}

void MainWindow::hexFileUndo()
{
    hexEdit->undo();
}

void MainWindow::hexFileSaveas()
{
    if(hexFilename->text() == "???") return;
    QString hfileName = QFileDialog::getSaveFileName(this, "Save As...", curHexFilename, "Binary files (*.prg; *.bin)");
    if (!hfileName.isEmpty())
    {
        saveHexFile(hfileName);
    }
}

void MainWindow::hexFileSave()
{
    saveHexFile(curHexFilename);
}

void MainWindow::hexFileOpen()
{
    curHexFilename = QFileDialog::getOpenFileName(this, "Open Binary File", workspacePath, "Binary files (*.prg; *.bin)");
    if(!curHexFilename.isEmpty())
    {
        loadHexFile();
        ui->statusbar->showMessage("Hex file loaded", TIMEOUT);
    }
}

void MainWindow::hexFileSaveselection()
{
    if(hexEdit->selectedData().isEmpty()) return;
    QString hfileName = QFileDialog::getSaveFileName(this, "Save Selection", curHexFilename, "Binary file (*.bin)");
    if (!hfileName.isEmpty())
    {
        saveHexFile(hfileName, true);
    }
}

void MainWindow::hexActionEnable()
{
    bool hModified = hexEdit->isModified();
    hSave->setEnabled(hModified);
    hUndo->setEnabled(hModified);
    hRedo->setEnabled(hModified);
}

void MainWindow::saveHexFile(QString &hfilename, bool saveSelection)
{
    QFile file;
    file.setFileName(hfilename);
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, "Hex Editor - IDE 65XX", QString("Cannot write file %1.").arg(hfilename));
        return;
    }

    QFileInfo fi(curHexFilename);
    QString ext = fi.completeSuffix();
    if((ext=="prg")&&(!saveSelection))
    {
        QByteArray sa;
        quint8 ls = hexEdit->addressOffset() & 0xff;
        quint8 hs = (hexEdit->addressOffset() & 0xff00) >> 8;
        sa[0] = ls;
        sa[1] = hs;
        file.write(sa);
    }
    if(saveSelection)
    {
        qint64 ss = hexEdit->getSelectionBegin();
        qint64 se = hexEdit->getSelectionEnd();
        file.write(hexEdit->dataAt(ss, se-ss));
    }
    else
        file.write(hexEdit->data());
    file.close();
    ui->statusbar->showMessage("Hex file saved", TIMEOUT);

}

void MainWindow::loadHexFile()
{
    quint16 startAddress = 0;
    QFileInfo fi(curHexFilename);
    QString ext = fi.completeSuffix();

    QFile file;
    file.setFileName(curHexFilename);
    if(!file.open(QIODevice::ReadOnly)) return;

    if(ext=="prg")
    {
        auto bytesOfStartAddress = file.read(2);
        startAddress = (bytesOfStartAddress.at(1) << 8) + bytesOfStartAddress.at(0);
    }

    hexEdit->setData(file.readAll());
    hexEdit->setAddressOffset(startAddress);
    file.close();

    hexwatcher->removePath(workspacePath + hexFilename->text());
    hexFilename->setText(fi.canonicalFilePath().remove(workspacePath));
    hexwatcher->addPath(curHexFilename);

}

void MainWindow::on_actionSet_Assembly_File_For_Current_Tab_triggered()
{
    if(ui->tabWidget->count()==0) return;
    int crIndex = ui->tabWidget->currentIndex();
    Tab *tab = /*(Tab *)*/ static_cast<Tab*>( ui->tabWidget->widget(crIndex));
    filePath = tab->getCurrentFilePath();
    SetAssemblyFile();
}
