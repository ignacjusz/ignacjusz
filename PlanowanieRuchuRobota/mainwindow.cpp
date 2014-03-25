#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Resizer"));
    pixmap = new QPixmap();
    this->kindOfJob = false;//przeszukiwanie wszerz
    this->ui->actionPrzeszukiwanie_wszerz->setChecked(true);
    this->filePath = "";

    StepTimer = new QTimer(this);
    QObject::connect(StepTimer, SIGNAL(timeout()), this, SLOT(StepTimerTimeout()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOtw_rz_triggered()
{
    // Show a file open dialog at QStandardPaths::PicturesLocation.
	filePath = QFileDialog::getOpenFileName(this, tr("Select Images"), ".","*.jpg *.png");
	if(!filePath.isEmpty())
    {
		pixmap->load(this->filePath);
        int x = ui->pic_label->width();
        int y = ui->pic_label->height();
        *pixmap = pixmap->scaled(QSize(x, y), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        ui->pic_label->setPixmap(*pixmap);
    }
    else
    {
		QMessageBox::critical( this, tr("Błąd"), tr("Nie wczytano pliku") );
    }
}

void MainWindow::on_actionKoniec_triggered()
{
    this->~MainWindow();
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);

    if(filePath != QString(""))
    {
        int windowheight = this->height();
        int windowwidth = this->width();
        ui->pic_label->setGeometry(QRect(10,10,windowwidth-20,windowheight-100));
        ui->pic_label->updateGeometry();

        pixmap->load(filePath);
        int x = ui->pic_label->width();
        int y = ui->pic_label->height();
        *pixmap = pixmap->scaled(QSize(x, y), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        ui->pic_label->setPixmap(*pixmap);
    }
}

void MainWindow::on_actionPraca_krokowa_triggered()
{
    //Slot obsługujący pracę krokową
    //można potem dorobić długość kroku i czas
    StepTimer->start(1000);
}

void MainWindow::StepTimerTimeout()
{
    //obsluga pracu krokowej wyzwalane z timera "StepTimer
    qDebug() << "Slot timera!";
}

void MainWindow::on_actionRozwi_ca_e_zadanie_triggered()
{
    //po wybrani tej opcji program powinien rozwiązać całe zadanie i wyświetlić rozwiązanie
    if(kindOfJob)
    {
        qDebug() << "Przeszukiwanie wglab";//przeszukiwanie wglab
    }
    else
    {
        qDebug() << "Przeszukiwanie wszerz";//przeszukiwanie wszerz
    }
}

void MainWindow::on_actionPrzeszukiwanie_wszerz_triggered()
{
    StepTimer->stop();
    kindOfJob = false;//przeszukiwanie wszerz
    ui->actionPrzeszukiwanie_wszerz->setChecked(true);
    ui->actionPrzeszukiwanie_wg_b->setChecked(false);
}

void MainWindow::on_actionPrzeszukiwanie_wg_b_triggered()
{
    StepTimer->stop();
    this->kindOfJob = true;//przeszukiwanie wglab
    this->ui->actionPrzeszukiwanie_wszerz->setChecked(false);
    this->ui->actionPrzeszukiwanie_wg_b->setChecked(true);
}
