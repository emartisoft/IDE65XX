#include "cartconv.h"
#include "ui_cartconv.h"

CartConv::CartConv(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CartConv)
{
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

    QFile fType(":/res/data/types.txt");
    if(fType.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&fType);
        while(!stream.atEnd())
            ui->cTypes->addItem(stream.readLine());
        fType.close();
        ui->cTypes->setCurrentIndex(0);
    }
}

CartConv::~CartConv()
{
    delete ui;
}

void CartConv::setCartConvFilename(QString &value)
{
    cartconvFilename = value;
}

void CartConv::setPrgFilePath(QString prgFilePath)
{
    ui->lPrgFilePath->setText(prgFilePath);
}

void CartConv::setCartName(QString name)
{
    ui->lCrtCartName->setText(name);
}

void CartConv::clear()
{
    ui->lPrgFilePath->clear();
    ui->lCrtCartName->clear();
    ui->tOutput->clear();
    ui->bConvert->setFocus();
}

void CartConv::on_bPrgFilePath_clicked()
{
    prgFileNames = QFileDialog::getOpenFileNames(this, tr("Select input file(s)"),
                                                       Common::appLocalDir(),
                                                       tr("Input File(s) (*.prg;*.bin;*.crt)"
                                                       ));
    int t = prgFileNames.count();
    if (t == 0)
    {
        ui->lPrgFilePath->clear();
        return;
    }
    QString strPrgFileNames = "";
    for (int i=0;i<t;i++) {
        strPrgFileNames += prgFileNames[i] + "|";
    }
    strPrgFileNames.remove(strPrgFileNames.length()-1,1);
    ui->lPrgFilePath->setText(strPrgFileNames);
}

void CartConv::on_bConvert_clicked()
{
    ui->tOutput->clear();

    if(ui->lCrtCartName->text().isEmpty())
    {
        ui->tOutput->appendPlainText("Please enter CRT cart name");
        ui->lCrtCartName->setFocus();
        return;
    }

    if(ui->lPrgFilePath->text().isEmpty())
    {
        ui->tOutput->appendPlainText("Please browse input file(s)");
        ui->lPrgFilePath->setFocus();
        return;
    }

    QStringList inputFilenames;
    inputFilenames = ui->lPrgFilePath->text().split("|");
    QString firstFile;
    if(ui->lPrgFilePath->text().contains('|'))
        firstFile = inputFilenames[0];
    else
        firstFile = ui->lPrgFilePath->text();

    QFileInfo fi(firstFile);

    QProcess cartconvProcess;
    QString cartconvOutput = QDir::cleanPath(Common::appConfigDir() + QDir::separator() + "cartconvoutput.txt");
    cartconvProcess.setStandardOutputFile(cartconvOutput);
    cartconvProcess.setStandardErrorFile(cartconvOutput, QIODevice::Append);

    cartconvProcess.setWorkingDirectory(fi.absolutePath());
    QString opFileWithoutExt = QDir::cleanPath(fi.absolutePath() + QDir::separator() + ui->lCrtCartName->text().trimmed() + ".");

    // cartconv.exe -t normal -name "cartname" -i input.prg -o output.crt
    QStringList parameters;
    parameters << "-t"
               << ui->cTypes->currentText().split(" - ")[0]
               << "-name"
               << ui->lCrtCartName->text().trimmed();

    if(ui->lPrgFilePath->text().contains('|'))
    {
        for (int i=0; i<prgFileNames.count();i++) {
           parameters << "-i" << prgFileNames[i];
        }
    }
    else
    {
        parameters << "-i" << firstFile;
    }

    parameters << "-o"
               << opFileWithoutExt + "crt";

    cartconvProcess.start(cartconvFilename, parameters);
    cartconvProcess.waitForFinished();

    QFile output(cartconvOutput);
    if(output.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&output);
        ui->tOutput->appendPlainText(stream.readAll());
        output.close();
    }

    if(cartconvProcess.exitCode()!=0) return;

    if(ui->cConvertToBinary->isChecked())
    {
        // cartconv -i input.crt -o output.bin
        cartconvProcess.start(cartconvFilename, QStringList() << "-i" << opFileWithoutExt + "crt" << "-o" << opFileWithoutExt + "bin");
        cartconvProcess.waitForFinished();

        if(output.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream stream(&output);
            ui->tOutput->appendPlainText(stream.readAll());
            output.close();
        }

        if(cartconvProcess.exitCode()!=0) return;
    }

    // cartconv -f input.crt
    cartconvProcess.start(cartconvFilename, QStringList() << "-f" << opFileWithoutExt + "crt");
    cartconvProcess.waitForFinished();

    if(output.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&output);
        ui->tOutput->appendPlainText(stream.readAll());
        output.close();
    }

}
