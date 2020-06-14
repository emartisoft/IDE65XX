#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow),
    cmdline(Common::appConfigDir()+"/cmdline.ini", QSettings::IniFormat),
    settings(Common::appConfigDir()+"/settings.ini", QSettings::IniFormat)
{
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

#ifndef Q_OS_WIN
    ui->lDirMaster->hide();
    ui->tDirMaster->hide();
    ui->bDirMaster->hide();
#endif

    ui->toolBox->setCurrentIndex(0);

    PopupMenu *pmEmulatorTemplates = new PopupMenu(ui->bEmulatorTemplates, this);
    QAction *tempVice = new QAction("VICE", this);
    QAction *tempEmu64 = new QAction("emu64", this);
    QAction *tempHoxs64 = new QAction("Hoxs64", this);
    QAction *tempMicro64 = new QAction("micro64", this);
    QAction *tempZ64K = new QAction("Z64K", this);

    connect(tempVice, SIGNAL(triggered()), this, SLOT(setVice()));
    connect(tempEmu64, SIGNAL(triggered()), this, SLOT(setEmu64()));
    connect(tempHoxs64, SIGNAL(triggered()), this, SLOT(setHoxs64()));
    connect(tempMicro64, SIGNAL(triggered()), this, SLOT(setMicro64()));
    connect(tempZ64K, SIGNAL(triggered()), this, SLOT(setZ64K()));

    pmEmulatorTemplates->addAction(tempVice);
    pmEmulatorTemplates->addAction(tempEmu64);
    pmEmulatorTemplates->addAction(tempHoxs64);
    pmEmulatorTemplates->addAction(tempMicro64);
    pmEmulatorTemplates->addAction(tempZ64K);

    ui->bEmulatorTemplates->setMenu(pmEmulatorTemplates);
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::setJRE(QString &value)
{
    ui->tJRE->setText(value);
}

void SettingsWindow::setKickAssJar(QString &value)
{
    ui->tKickAssJar->setText(value);
}

void SettingsWindow::setEmulator(QString &value)
{
    ui->tEmulator->setText(value);
}

void SettingsWindow::setEmulatorParameters(QString &value)
{
    ui->tEmulatorParameters->setText(value);
}

void SettingsWindow::setCompressionUtility(QString &value)
{
    ui->tCompressionUtility->setText(value);
}

void SettingsWindow::setCompressionParameters(QString &value)
{
    ui->tCompressionParameters->setText(value);
}

void SettingsWindow::setDirMaster(QString &value)
{
    ui->tDirMaster->setText(value);
}

void SettingsWindow::setC1541(QString &value)
{
    ui->tC1541->setText(value);
}

void SettingsWindow::setDebugger(QString &value)
{
    ui->tDebugger->setText(value);
}

void SettingsWindow::setOpenLastUsedFiles(bool open)
{
    ui->cOpenLastUsed->setChecked(open);
}

void SettingsWindow::setMaxRecentWorkspace(int count)
{
    ui->spMaxRecentWorkspace->setValue(count);
}

void SettingsWindow::setTabSize(int size)
{
    ui->spTabStopDistance->setValue(size);
}

void SettingsWindow::setTabPolicy(int index)
{
    ui->cTabPolicy->setCurrentIndex(index);
}

void SettingsWindow::setCodeFontName(QString &fontName)
{
    ui->lOutput->setFont(QFont(fontName));
}

void SettingsWindow::setCodeFontSize(int fontSize)
{
    ui->lOutput->setFont(QFont(getCodeFontName(), fontSize));
}

void SettingsWindow::setApplicationFont(QString fontName, int fontSize)
{
    ui->lOutput2->setFont(QFont(fontName, fontSize));
}

void SettingsWindow::setApplicationTheme(int select)
{
    ui->cTheme->setCurrentIndex(select);
}

void SettingsWindow::setWordWrap(bool &value)
{
    ui->cWordWrap->setChecked(value);
}

void SettingsWindow::setShowAllCharacters(bool &value)
{
    ui->cShowAllCharacters->setChecked(value);
}

void SettingsWindow::setAutoCompletion(bool &value)
{
    ui->cAutoCompletion->setChecked(value);
}

void SettingsWindow::setSIDPlayer(QString &value)
{
    ui->tSIDPlayer->setText(value);
}

void SettingsWindow::setSIDPlayerParameters(QString &value)
{
    ui->tSIDPlayerParameters->setText(value);
}

void SettingsWindow::setCartconv(QString &value)
{
    ui->tCartconv->setText(value);
}

void SettingsWindow::setCustomBackgroundTexture(QString value)
{
    ui->fBackgroundTexture->setStyleSheet(QString("background-image: url(%1);").arg(value));
}

void SettingsWindow::setCustomButtonTexture(QString value)
{
    ui->fButtonTexture->setStyleSheet(QString("background-image: url(%1);").arg(value));
}

void SettingsWindow::setBackground(QColor c)
{
    ui->fBackground->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(c.red()).arg(c.green()).arg(c.blue()));
}

void SettingsWindow::setBrightText(QColor c)
{
    ui->fBrightText->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(c.red()).arg(c.green()).arg(c.blue()));
}

void SettingsWindow::setBase(QColor c)
{
    ui->fBase->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(c.red()).arg(c.green()).arg(c.blue()));
}

void SettingsWindow::setHighlights(QColor c)
{
    ui->fHighlights->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(c.red()).arg(c.green()).arg(c.blue()));
}

void SettingsWindow::setDisable(QColor c)
{
    ui->fDisable->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(c.red()).arg(c.green()).arg(c.blue()));
}

void SettingsWindow::setOpcode(QColor c)
{
    ui->fOpcode->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(c.red()).arg(c.green()).arg(c.blue()));
}

void SettingsWindow::setNumber(QColor c)
{
    ui->fNumber->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(c.red()).arg(c.green()).arg(c.blue()));
}

void SettingsWindow::setFunction(QColor c)
{
    ui->fFunction->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(c.red()).arg(c.green()).arg(c.blue()));
}

void SettingsWindow::setAssemblerDir(QColor c)
{
    ui->fAssemblerDir->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(c.red()).arg(c.green()).arg(c.blue()));
}

void SettingsWindow::setPreprocessorDir(QColor c)
{
    ui->fPreprocessorDir->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(c.red()).arg(c.green()).arg(c.blue()));
}

void SettingsWindow::setComment(QColor c)
{
    ui->fComment->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(c.red()).arg(c.green()).arg(c.blue()));
}

void SettingsWindow::setQuotation(QColor c)
{
    ui->fQuotation->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(c.red()).arg(c.green()).arg(c.blue()));
}

void SettingsWindow::setLabel(QColor c)
{
    ui->fLabel->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(c.red()).arg(c.green()).arg(c.blue()));
}

void SettingsWindow::setCmdLinesEnabledForDebug(bool debugdump, bool vicesymbols)
{
    ui->coDebugdump->setChecked(debugdump);
    ui->coVicesymbols->setChecked(vicesymbols);
    emit on_buttonBox_accepted();
}

bool SettingsWindow::getCmdLineDebugDump()
{
    return ui->coDebugdump->isChecked();
}

bool SettingsWindow::getCmdLineViceSymbols()
{
    return ui->coVicesymbols->isChecked();
}

QString SettingsWindow::getJRE()
{
    return ui->tJRE->text();
}

QString SettingsWindow::getKickAssJar()
{
    return ui->tKickAssJar->text();
}

QString SettingsWindow::getEmulator()
{
    return ui->tEmulator->text();
}

QString SettingsWindow::getEmulatorParameters()
{
    return ui->tEmulatorParameters->text();
}

QString SettingsWindow::getCompressionUtility()
{
    return ui->tCompressionUtility->text();
}

QString SettingsWindow::getCompressionParameters()
{
    return ui->tCompressionParameters->text();
}

QString SettingsWindow::getDirMaster()
{
    return ui->tDirMaster->text();
}

QString SettingsWindow::getC1541()
{
    return ui->tC1541->text();
}

QString SettingsWindow::getDebugger()
{
    return ui->tDebugger->text();
}

QString SettingsWindow::getSIDPlayer()
{
    return ui->tSIDPlayer->text();
}

QString SettingsWindow::getSIDPlayerParameters()
{
    return ui->tSIDPlayerParameters->text();
}

QString SettingsWindow::getCartconv()
{
    return ui->tCartconv->text();
}

QString SettingsWindow::getBuildDir()
{
    if(ui->coOutputDir->isChecked())
        return ui->coOutputDirText->text();
    else
        return "";
}

bool SettingsWindow::getOpenLastUsedFiles()
{
    return ui->cOpenLastUsed->isChecked();
}

int SettingsWindow::getMaxRecentWorkspace()
{
    return ui->spMaxRecentWorkspace->value();
}

int SettingsWindow::getTabSize()
{
    return ui->spTabStopDistance->value();
}

int SettingsWindow::getTabPolicy()
{
    return ui->cTabPolicy->currentIndex();
}

QString SettingsWindow::getCodeFontName()
{
    return ui->lOutput->font().family();
}

int SettingsWindow::getCodeFontSize()
{
    return ui->lOutput->font().pointSize();
}

bool SettingsWindow::getWordWrap()
{
    return ui->cWordWrap->isChecked();
}

bool SettingsWindow::getShowAllCharacters()
{
    return ui->cShowAllCharacters->isChecked();
}

bool SettingsWindow::getAutoCompletion()
{
    return ui->cAutoCompletion->isChecked();
}

void SettingsWindow::on_bKickAssJar_clicked()
{
    QString strFileName = QFileDialog::getOpenFileName(this, tr("Select KickAss.jar"),
                                                       Common::appLocalDir(),
                                                       tr("KickAss.jar (KickAss.jar)"));
    if (strFileName.isEmpty()) return;
    ui->tKickAssJar->setText(strFileName);
}

void SettingsWindow::on_bJRE_clicked()
{
    QString strFileName = QFileDialog::getOpenFileName(this,
                                                        #ifdef Q_OS_WIN
                                                       tr("Select JRE java.exe"),
                                                        #else
                                                       tr("Select JRE java"),
                                                        #endif
                                                       Common::appLocalDir(),
                                                        #ifdef Q_OS_WIN
                                                       tr("java.exe (java.exe)"
                                                        #else
                                                       tr("java (*)"
                                                        #endif
                                                          ));
    if (strFileName.isEmpty()) return;
    ui->tJRE->setText(strFileName);
}

void SettingsWindow::on_bEmulator_clicked()
{
    QString strFileName = QFileDialog::getOpenFileName(this, tr("Select Emulator"),
                                                       Common::appLocalDir(),
                                                   #ifdef Q_OS_WIN
                                                       tr("Emulator Application (*.exe);;Emulator Application (*.jar)"
                                                   #else
                                                       tr("Emulator Application (*)"
                                                   #endif
                                                          ));
    if (strFileName.isEmpty()) return;
#ifdef Q_OS_MACOS
    changePathForMacOSAppFile(strFileName);
#endif
    ui->tEmulator->setText(strFileName);
}

void SettingsWindow::on_bCompressionUtility_clicked()
{
    QString strFileName = QFileDialog::getOpenFileName(this, tr("Select Compression Utility"),
                                                       Common::appLocalDir(),

                                                   #ifdef Q_OS_WIN
                                                       tr("Compression Utility Application (*.exe)"
                                                   #else
                                                       tr("Compression Utility Application (*)"
                                                   #endif
                                                       ));
    if (strFileName.isEmpty()) return;
#ifdef Q_OS_MACOS
    changePathForMacOSAppFile(strFileName);
#endif
    ui->tCompressionUtility->setText(strFileName);
}

void SettingsWindow::on_bChangeFontOutput_clicked()
{
    bool ok;
    QFontDialog fd(this);
    fd.setOption(QFontDialog::MonospacedFonts);
    QFont font = fd.getFont(&ok, ui->lOutput->font(), this);
    if (ok) {
        ui->lOutput->setFont(font);
    }
}

void SettingsWindow::on_coAsminfo_toggled(bool checked)
{
    ui->coAsminfofile->setEnabled(checked);
    ui->coAsminfofilename->setEnabled(checked);
    ui->lcoAsminfofilename->setEnabled(checked);
}

void SettingsWindow::on_coBytedump_toggled(bool checked)
{
    ui->coBytedumpfile->setEnabled(checked);
    ui->lcoBytedumpfilename->setEnabled(checked);
    ui->coBytedumpfilename->setEnabled(checked);
}

void SettingsWindow::on_coDefine_toggled(bool checked)
{
    ui->lcoDefinepresym->setEnabled(checked);
    ui->coDefinepresymtext->setEnabled(checked);
}

//void SettingsWindow::on_coExecute_toggled(bool checked)
//{
//    ui->coExecutelog->setEnabled(checked);
//    ui->lcoExecutelog->setEnabled(checked);
//    ui->coExecutelogfile->setEnabled(checked);
//}

void SettingsWindow::on_coFillbyte_toggled(bool checked)
{
    ui->lcoFillbyte->setEnabled(checked);
    ui->coFillbytevalue->setEnabled(checked);
}

void SettingsWindow::on_coLibdir_toggled(bool checked)
{
    ui->lcolibdirpath->setEnabled(checked);
    ui->colibdirpathtext->setEnabled(checked);
}

void SettingsWindow::on_coMaxaddr_toggled(bool checked)
{
    ui->lcoMaxaddr->setEnabled(checked);
    ui->coMaxaddrValue->setEnabled(checked);
}

void SettingsWindow::on_coO_toggled(bool checked)
{
    ui->lcoPrgfilename->setEnabled(checked);
    ui->coPrgfilename->setEnabled(checked);
}

void SettingsWindow::on_coOutputDir_toggled(bool checked)
{
    ui->lcoOutputDir->setEnabled(checked);
    ui->coOutputDirText->setEnabled(checked);
}

void SettingsWindow::on_coReplacefile_toggled(bool checked)
{
    ui->lcoReplacefile->setEnabled(checked);
    ui->coReplacefilesText->setEnabled(checked);
}

//void SettingsWindow::on_coSymbolfiledir_toggled(bool checked)
//{
//    ui->lcoSymbolfiledir->setEnabled(checked);
//    ui->coSymbolfiledirtext->setEnabled(checked);
//    ui->coSymbolfiledir->setEnabled(checked);
//}

void SettingsWindow::on_coLog_toggled(bool checked)
{
    ui->lcoLog->setEnabled(checked);
    ui->coLogfilename->setEnabled(checked);
}

void SettingsWindow::on_coExtra_toggled(bool checked)
{
    ui->coOtherCLOArg->setEnabled(checked);
}

void SettingsWindow::ReadCmdLineOption()
{
    // settings file for command line options
    ui->coAfo->setChecked(cmdline.value("afo", false).toBool());
    ui->coAom->setChecked(cmdline.value("aom", false).toBool());
    ui->coAsminfo->setChecked(cmdline.value("asminfo", false).toBool());
    ui->coAsminfofile->setChecked(cmdline.value("asminfofile", false).toBool());
    ui->coAsminfofilename->setText(cmdline.value("asminfofiletext", "asminfo.txt").toString());
    ui->coBytedump->setChecked(cmdline.value("bytedump", false).toBool());
    ui->coBytedumpfile->setChecked(cmdline.value("bytedumpfile", false).toBool());
    ui->coBytedumpfilename->setText(cmdline.value("bytedumpfiletext", "bytedump.txt").toString());
    ui->coDebug->setChecked(cmdline.value("debug", false).toBool());
    ui->coDebugdump->setChecked(cmdline.value("debugdump", false).toBool());
    ui->coDefine->setChecked(cmdline.value("define", false).toBool());
    ui->coDefinepresymtext->setText(cmdline.value("definepresymtext", "DEBUG").toString());
    ui->coDtv->setChecked(cmdline.value("dtv", false).toBool());
    ui->coExcludeillegal->setChecked(cmdline.value("excludeillegal", false).toBool());
    ui->coExecutelog->setChecked(cmdline.value("executelog", false).toBool());
    ui->coExecutelogfile->setText(cmdline.value("executelogfiletext", "executelog.txt").toString());
    ui->coFillbyte->setChecked(cmdline.value("fillbyte", false).toBool());
    ui->coFillbytevalue->setValue(cmdline.value("fillbytevalue", 255).toInt());

    ui->coLibdir->setChecked(cmdline.value("libdir", false).toBool());
    ui->colibdirpathtext->setText(cmdline.value("libdirpathtext", "../stdlib").toString());

    ui->coMaxaddr->setChecked(cmdline.value("maxaddr", false).toBool());
    ui->coMaxaddrValue->setValue(cmdline.value("maxaddrvalue", 65535).toInt());

    ui->coMbfiles->setChecked(cmdline.value("mbfiles", false).toBool());
    ui->coNoeval->setChecked(cmdline.value("noeval", false).toBool());

    ui->coO->setChecked(cmdline.value("o", false).toBool());
    ui->coPrgfilename->setText(cmdline.value("Prgfilename", "out.prg").toString());

    ui->coOutputDir->setChecked(cmdline.value("odir", true).toBool());
    ui->coOutputDirText->setText(cmdline.value("odirtext", "build").toString());

    ui->coPseudoc3x->setChecked(cmdline.value("pseudoc3x", false).toBool());
    ui->coReplacefile->setChecked(cmdline.value("replacefile", false).toBool());
    ui->coReplacefilesText->setText(cmdline.value("replacefilesText", "source.asm replace.asm").toString());

    ui->coShowmem->setChecked(cmdline.value("showmem", true).toBool());
    ui->coTime->setChecked(cmdline.value("time", false).toBool());
    ui->coSymbolfile->setChecked(cmdline.value("symbolfile", false).toBool());

    ui->coSymbolfiledir->setChecked(cmdline.value("symbolfiledir", false).toBool());
    ui->coSymbolfiledirtext->setText(cmdline.value("symbolfiledirtext", "symbol").toString());

    ui->coVicesymbols->setChecked(cmdline.value("vicesymbols", true).toBool());
    ui->coWarningoff->setChecked(cmdline.value("warningoff", false).toBool());
    ui->coLog->setChecked(cmdline.value("log", false).toBool());
    ui->coLogfilename->setText(cmdline.value("logfilename", "log.txt").toString());

    ui->coExtra->setChecked(cmdline.value("extra", false).toBool());
    ui->coOtherCLOArg->setText(cmdline.value("extraarg", ":version=beta2 :x=64").toString());

}

bool SettingsWindow::getCheckShowMemCmdLine()
{
    // -showmem is checked?
    return ui->coShowmem->isChecked();
}

void SettingsWindow::on_buttonBox_accepted()
{
    NoEmptyForInputText();
    // config file for kickassembler
    QFile cfgFile(Common::appConfigDir()+"/ide65xx.cfg");
    // settings file for command line options

    if(cfgFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&cfgFile);
        bool b;
        stream << "# This config file is created by IDE65XX\n";

        //-afo
        b = ui->coAfo->isChecked();
        if (b){stream << "-afo\n";}
        cmdline.setValue("afo", b);

        //-aom
        b = ui->coAom->isChecked();
        if (b){stream << "-aom\n";}
        cmdline.setValue("aom", b);

        //-asminfo all
        b = ui->coAsminfo->isChecked();
        if (b){stream << "-asminfo all\n";}
        cmdline.setValue("asminfo", b);

        //-asminfofile
        if(b) // if <asminfo all> is  true
        {
            b = ui->coAsminfofile->isChecked();
            if(b) {stream << "-asminfofile " << ui->coAsminfofilename->text().trimmed() << "\n";}
            cmdline.setValue("asminfofile", b);
            cmdline.setValue("asminfofiletext", ui->coAsminfofilename->text().trimmed());
        }

        //-bytedump
        b = ui->coBytedump->isChecked();
        if (b){stream << "-bytedump\n";}
        cmdline.setValue("bytedump", b);

        //-bytedumpfile
        if(b) // if <bytedump> is  true
        {
            b = ui->coBytedumpfile->isChecked();
            if(b) {stream << "-bytedumpfile " << ui->coBytedumpfilename->text().trimmed() << "\n";}
            cmdline.setValue("bytedumpfile", b);
            cmdline.setValue("bytedumpfiletext", ui->coBytedumpfilename->text().trimmed());
        }

        //-debug
        b = ui->coDebug->isChecked();
        if (b){stream << "-debug\n";}
        cmdline.setValue("debug", b);

        //-debugdump
        b = ui->coDebugdump->isChecked();
        if (b){stream << "-debugdump\n";}
        cmdline.setValue("debugdump", b);

        //-define
        b = ui->coDefine->isChecked();
        if (b){stream << "-define " << ui->coDefinepresymtext->text().trimmed() << "\n";}
        cmdline.setValue("define", b);
        cmdline.setValue("definepresymtext", ui->coDefinepresymtext->text().trimmed());

        //-dtv
        b = ui->coDtv->isChecked();
        if (b){stream << "-dtv\n";}
        cmdline.setValue("dtv", b);

        //-excludeillegal
        b = ui->coExcludeillegal->isChecked();
        if (b){stream << "-excludeillegal\n";}
        cmdline.setValue("excludeillegal", b);

        //-executelog
        b = ui->coExecutelog->isChecked();
        if(b) {stream << "-executelog " << ui->coExecutelogfile->text().trimmed() << "\n";}
        cmdline.setValue("executelog", b);
        cmdline.setValue("executelogfiletext", ui->coExecutelogfile->text().trimmed());


        //-fillbyte
        b = ui->coFillbyte->isChecked();
        if (b){stream << "-fillbyte " << ui->coFillbytevalue->value() << "\n";}
        cmdline.setValue("fillbyte", b);
        cmdline.setValue("fillbytevalue", ui->coFillbytevalue->value());

        //-libdir
        b = ui->coLibdir->isChecked();
        if (b){stream << "-libdir " << ui->colibdirpathtext->text().trimmed() << "\n";}
        cmdline.setValue("libdir", b);
        cmdline.setValue("libdirpathtext", ui->colibdirpathtext->text().trimmed());

        //-maxaddr
        b = ui->coMaxaddr->isChecked();
        if (b){stream << "-maxaddr " << ui->coMaxaddrValue->value() << "\n";}
        cmdline.setValue("maxaddr", b);
        cmdline.setValue("maxaddrvalue", ui->coMaxaddrValue->value());

        //-mbfiles
        b = ui->coMbfiles->isChecked();
        if (b){stream << "-mbfiles\n";}
        cmdline.setValue("mbfiles", b);

        //-noeval
        b = ui->coNoeval->isChecked();
        if (b){stream << "-noeval\n";}
        cmdline.setValue("noeval", b);

        //-o
        b = ui->coO->isChecked();
        if (b){stream << "-o " << ui->coPrgfilename->text().trimmed() << "\n";}
        cmdline.setValue("o", b);
        cmdline.setValue("Prgfilename", ui->coPrgfilename->text().trimmed());

        //-odir
        b = ui->coOutputDir->isChecked();
        if (b){stream << "-odir " << ui->coOutputDirText->text().trimmed() << "\n";}
        cmdline.setValue("odir", b);
        cmdline.setValue("odirtext", ui->coOutputDirText->text().trimmed());

        //-pseudoc3x
        b = ui->coPseudoc3x->isChecked();
        if (b){stream << "-pseudoc3x\n";}
        cmdline.setValue("pseudoc3x", b);

        //-replacefile
        b = ui->coReplacefile->isChecked();
        if (b){stream << "-replacefile " << ui->coReplacefilesText->text().trimmed() << "\n";}
        cmdline.setValue("replacefile", b);
        cmdline.setValue("replacefilesText", ui->coReplacefilesText->text().trimmed());

        //-showmem
        b = ui->coShowmem->isChecked();
        if (b){stream << "-showmem\n";}
        cmdline.setValue("showmem", b);

        //-time
        b = ui->coTime->isChecked();
        if (b){stream << "-time\n";}
        cmdline.setValue("time", b);

        //-symbolfile
        b = ui->coSymbolfile->isChecked();
        if (b){stream << "-symbolfile\n";}
        cmdline.setValue("symbolfile", b);

        //-symbolfiledir
        if(b) // if <symbolfile> is  true
        {
            b = ui->coSymbolfiledir->isChecked();
            if(b) {stream << "-symbolfiledir " << ui->coSymbolfiledirtext->text().trimmed() << "\n";}
            cmdline.setValue("symbolfiledir", b);
            cmdline.setValue("symbolfiledirtext", ui->coSymbolfiledirtext->text().trimmed());
        }

        //-vicesymbols
        b = ui->coVicesymbols->isChecked();
        if (b){stream << "-vicesymbols\n";}
        cmdline.setValue("vicesymbols", b);

        //-warningoff
        b = ui->coWarningoff->isChecked();
        if (b){stream << "-warningoff\n";}
        cmdline.setValue("warningoff", b);

        //-log
        b = ui->coLog->isChecked();
        if (b){stream << "-log " << ui->coLogfilename->text().trimmed() << "\n";}
        cmdline.setValue("log", b);
        cmdline.setValue("logfilename", ui->coLogfilename->text().trimmed());

        //additional options
        b = ui->coExtra->isChecked();
        if(b)
        {
            QStringList listArguments = ui->coOtherCLOArg->text().trimmed().split(' ');
            foreach(const QString &argument, listArguments)
            {
                stream << argument << "\n";
            }
        }
        cmdline.setValue("extra", b);
        cmdline.setValue("extraarg", ui->coOtherCLOArg->text().trimmed());


        cfgFile.close();
        cmdline.sync();

    }

}


void SettingsWindow::on_coExecutelog_toggled(bool checked)
{
    ui->lcoExecutelog->setEnabled(checked);
    ui->coExecutelogfile->setEnabled(checked);
}

void SettingsWindow::on_coSymbolfile_toggled(bool checked)
{
    ui->coSymbolfiledir->setEnabled(checked);
    ui->lcoSymbolfiledir->setEnabled(checked);
    ui->coSymbolfiledirtext->setEnabled(checked);
}

void SettingsWindow::NoEmptyForInputText()
{
    if(ui->coAsminfofilename->text().isEmpty())
        ui->coAsminfofilename->setText(cmdline.value("asminfofiletext", "asminfo.txt").toString());
    if(ui->coBytedumpfilename->text().isEmpty())
        ui->coBytedumpfilename->setText(cmdline.value("bytedumpfiletext", "bytedump.txt").toString());
    if(ui->coDefinepresymtext->text().isEmpty())
        ui->coDefinepresymtext->setText(cmdline.value("definepresymtext", "DEBUG").toString());
    if(ui->coExecutelogfile->text().isEmpty())
        ui->coExecutelogfile->setText(cmdline.value("executelogfiletext", "executelog.txt").toString());
    if(ui->colibdirpathtext->text().isEmpty())
        ui->colibdirpathtext->setText(cmdline.value("libdirpathtext", "../stdlib").toString());
    if(ui->coPrgfilename->text().isEmpty())
        ui->coPrgfilename->setText(cmdline.value("Prgfilename", "out.prg").toString());
    if(ui->coOutputDirText->text().isEmpty())
        ui->coOutputDirText->setText(cmdline.value("odirtext", "build").toString());
    if(ui->coReplacefilesText->text().isEmpty())
        ui->coReplacefilesText->setText(cmdline.value("replacefilesText", "source.asm replace.asm").toString());
    if(ui->coSymbolfiledirtext->text().isEmpty())
        ui->coSymbolfiledirtext->setText(cmdline.value("symbolfiledirtext", "symbol").toString());
    if(ui->coLogfilename->text().isEmpty())
        ui->coLogfilename->setText(cmdline.value("logfilename", "log.txt").toString());
    if(ui->coOtherCLOArg->text().isEmpty())
        ui->coOtherCLOArg->setText(cmdline.value("extraarg", ":version=beta2 :x=64").toString());


}

#ifdef Q_OS_MACOS
void SettingsWindow::changePathForMacOSAppFile(QString &AppFilePath)
{
    QFileInfo fi(AppFilePath);
    QString fiFilename = fi.fileName();
    if(fiFilename.right(4) == ".app") AppFilePath += "/Contents/MacOS/" + fiFilename.left(fiFilename.length()-4);
}
#endif

void SettingsWindow::on_bDirMaster_clicked()
{
    QString strFileName = QFileDialog::getOpenFileName(this, tr("Select DirMaster.exe"),
                                                       Common::appLocalDir(),
                                                       tr("DirMaster.exe (DirMaster.exe)"));
    if (strFileName.isEmpty()) return;
    ui->tDirMaster->setText(strFileName);
}

void SettingsWindow::on_bC1541_clicked()
{
    QString strFileName = QFileDialog::getOpenFileName(this,
                                                   #ifdef Q_OS_WIN
                                                  tr("Select c1541.exe"),
                                                   #else
                                                  tr("Select c1541"),
                                                   #endif
                                                  Common::appLocalDir(),
                                                   #ifdef Q_OS_WIN
                                                  tr("c1541.exe (c1541.exe)"
                                                   #else
                                                  tr("c1541 (*)"
                                                   #endif
                                                          ));
    if (strFileName.isEmpty()) return;
#ifdef Q_OS_MACOS
    changePathForMacOSAppFile(strFileName);
#endif
    ui->tC1541->setText(strFileName);
}

void SettingsWindow::on_bDebugger_clicked()
{
    QString strFileName = QFileDialog::getOpenFileName(this, tr("Select Debugger"),
                                                       Common::appLocalDir(),

                                                   #ifdef Q_OS_WIN
                                                       tr("Debugger Application (*.exe)"
                                                   #else
                                                       tr("Debugger Application (*)"
                                                   #endif
                                                       ));
    if (strFileName.isEmpty()) return;
#ifdef Q_OS_MACOS
    changePathForMacOSAppFile(strFileName);
#endif
    ui->tDebugger->setText(strFileName);
}

void SettingsWindow::on_bSIDPlayer_clicked()
{
    QString strFileName = QFileDialog::getOpenFileName(this, tr("Select SID Player"),
                                                       Common::appLocalDir(),

                                                   #ifdef Q_OS_WIN
                                                       tr("SID Player Application (*.*)"
                                                   #else
                                                       tr("SID Player Application (*)"
                                                   #endif
                                                       ));
    if (strFileName.isEmpty()) return;
#ifdef Q_OS_MACOS
    changePathForMacOSAppFile(strFileName);
#endif
    ui->tSIDPlayer->setText(strFileName);
}

void SettingsWindow::on_bChangeFontOutput2_clicked()
{
    bool ok;
    QFontDialog fd(this);
    restartRequired = false;
    QFont font = fd.getFont(&ok, ui->lOutput2->font(), this);
    if (ok) {
        ui->lOutput2->setFont(font);
        settings.setValue("ApplicationFontName", font.family());
        settings.setValue("ApplicationFontSize", font.pointSize());
        settings.sync();
        restartRequired = true;
    }
}

void SettingsWindow::on_cTheme_currentIndexChanged(int index)
{
    settings.setValue("ApplicationStyle", index);
    settings.sync();
    restartRequired = true;
}

void SettingsWindow::setVice()
{
    ui->tEmulatorParameters->setText("<file>");
}

void SettingsWindow::setEmu64()
{
    ui->tEmulatorParameters->setText("--nosplash -a <file>");
}

void SettingsWindow::setHoxs64()
{
    ui->tEmulatorParameters->setText("-autoload <file>");
}

void SettingsWindow::setMicro64()
{
    ui->tEmulatorParameters->setText("<file>");
}

void SettingsWindow::setZ64K()
{
    ui->tEmulatorParameters->setText("c64 <file>");
}

void SettingsWindow::on_bCartconv_clicked()
{
    QString strFileName = QFileDialog::getOpenFileName(this, tr("Select Cartridge Conversion Utility (cartconv VICE)"),
                                                       Common::appLocalDir(),

                                                   #ifdef Q_OS_WIN
                                                       tr("Cartridge Conversion Utility (cartconv VICE) (cartconv.exe)"
                                                   #else
                                                       tr("Cartridge Conversion Utility (cartconv VICE) (*)"
                                                   #endif
                                                       ));
    if (strFileName.isEmpty()) return;
#ifdef Q_OS_MACOS
    changePathForMacOSAppFile(strFileName);
#endif
    ui->tCartconv->setText(strFileName);
}

void SettingsWindow::on_cTheme_currentTextChanged(const QString &arg1)
{
    ui->gCustomTheme->setEnabled(arg1 == "Custom");
}

void SettingsWindow::on_bBackgroundTexture_clicked()
{
    QString strFileName = QFileDialog::getOpenFileName(this, tr("Select Background Texture"),
                                                       Common::appLocalDir(),
                                                       tr("Images (*.png; *.jpg)"
                                                       ));
    if (strFileName.isEmpty()) return;
    setCustomBackgroundTexture(strFileName);
    settings.setValue("CustomBackgroundTexture", strFileName);
    settings.sync();
    restartRequired = true;

}

void SettingsWindow::on_bButtonTexture_clicked()
{
    QString strFileName = QFileDialog::getOpenFileName(this, tr("Select Button Texture"),
                                                       Common::appLocalDir(),
                                                       tr("Images (*.png; *.jpg)"
                                                       ));
    if (strFileName.isEmpty()) return;
    setCustomBackgroundTexture(strFileName);
    settings.setValue("CustomButtonTexture", strFileName);
    settings.sync();
    restartRequired = true;
}

void SettingsWindow::on_bBackgroundColor_clicked()
{
    QColor c = QColorDialog::getColor(settings.value("CustomBackground").value<QColor>(), this, "Background Color");
    setBackground(c);
    settings.setValue("CustomBackground", c);
    settings.sync();
    restartRequired = true;
}

void SettingsWindow::on_bBrightTextColor_clicked()
{
    QColor c = QColorDialog::getColor(settings.value("CustomBrightText").value<QColor>(), this, "Bright Text Color");
    setBrightText(c);
    settings.setValue("CustomBrightText", c);
    settings.sync();
    restartRequired = true;
}

void SettingsWindow::on_bBaseColor_clicked()
{
    QColor c = QColorDialog::getColor(settings.value("CustomBase").value<QColor>(), this, "Base Color");
    setBase(c);
    settings.setValue("CustomBase", c);
    settings.sync();
    restartRequired = true;
}

void SettingsWindow::on_bHighlightsColor_clicked()
{
    QColor c = QColorDialog::getColor(settings.value("CustomHighlights").value<QColor>(), this, "Highlights Color");
    setHighlights(c);
    settings.setValue("CustomHighlights", c);
    settings.sync();
    restartRequired = true;
}

void SettingsWindow::on_bDisableColor_clicked()
{
    QColor c = QColorDialog::getColor(settings.value("CustomDisable").value<QColor>(), this, "Disable Color");
    setDisable(c);
    settings.setValue("CustomDisable", c);
    settings.sync();
    restartRequired = true;
}

void SettingsWindow::on_bOpcode_clicked()
{
    QColor c = QColorDialog::getColor(settings.value("CustomOpcode").value<QColor>(), this, "Opcode Color");
    setOpcode(c);
    settings.setValue("CustomOpcode", c);
    settings.sync();
    restartRequired = true;
}

void SettingsWindow::on_bNumber_clicked()
{
    QColor c = QColorDialog::getColor(settings.value("CustomNumber").value<QColor>(), this, "Number Color");
    setNumber(c);
    settings.setValue("CustomNumber", c);
    settings.sync();
    restartRequired = true;
}

void SettingsWindow::on_bFunction_clicked()
{
    QColor c = QColorDialog::getColor(settings.value("CustomFunction").value<QColor>(), this, "Function Color");
    setFunction(c);
    settings.setValue("CustomFunction", c);
    settings.sync();
    restartRequired = true;
}

void SettingsWindow::on_bAssemblerDir_clicked()
{
    QColor c = QColorDialog::getColor(settings.value("CustomAssemblerDir").value<QColor>(), this, "Assembler Directive Color");
    setAssemblerDir(c);
    settings.setValue("CustomAssemblerDir", c);
    settings.sync();
    restartRequired = true;
}

void SettingsWindow::on_bPreprocessorDir_clicked()
{
    QColor c = QColorDialog::getColor(settings.value("CustomPreprocessorDir").value<QColor>(), this, "Preprocessor Directive Color");
    setPreprocessorDir(c);
    settings.setValue("CustomPreprocessorDir", c);
    settings.sync();
    restartRequired = true;
}

void SettingsWindow::on_bComment_clicked()
{
    QColor c = QColorDialog::getColor(settings.value("CustomComment").value<QColor>(), this, "Comment Color");
    setComment(c);
    settings.setValue("CustomComment", c);
    settings.sync();
    restartRequired = true;
}

void SettingsWindow::on_bQuotation_clicked()
{
    QColor c = QColorDialog::getColor(settings.value("CustomQuotation").value<QColor>(), this, "Quotation Color");
    setQuotation(c);
    settings.setValue("CustomQuotation", c);
    settings.sync();
    restartRequired = true;
}

void SettingsWindow::on_bLabel_clicked()
{
    QColor c = QColorDialog::getColor(settings.value("CustomLabel").value<QColor>(), this, "Label Color");
    setQuotation(c);
    settings.setValue("CustomLabel", c);
    settings.sync();
    restartRequired = true;
}
