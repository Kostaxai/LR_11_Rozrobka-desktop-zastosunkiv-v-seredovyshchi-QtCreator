#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // кнопка
    connect(ui->pushButtonCalc, &QPushButton::clicked,
            this, &MainWindow::calculateSeconds);

    // меню
    connect(ui->actionOpen, &QAction::triggered,
            this, &MainWindow::openFile);

    connect(ui->actionSave, &QAction::triggered,
            this, &MainWindow::saveFile);

    connect(ui->actionExit, &QAction::triggered,
            this, &MainWindow::close);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::calculateSeconds()
{
    QString text = ui->lineEditN->text().trimmed();

    if (text.isEmpty())
    {
        QMessageBox::warning(this, "Помилка", "Введіть значення N!");
        return;
    }

    bool ok;
    int N = text.toInt(&ok);

    if (!ok)
    {
        QMessageBox::critical(this, "Помилка", "Введіть ціле число!");
        return;
    }

    if (N < 0)
    {
        QMessageBox::warning(this, "Помилка", "Число не може бути від'ємним!");
        return;
    }

    int result = N % 60;

    ui->labelResult->setText("Секунди останньої хвилини: " + QString::number(result));
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Відкрити файл");

    if (fileName.isEmpty()) return;

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Помилка", "Не вдалося відкрити файл");
        return;
    }

    QTextStream in(&file);
    QString data = in.readAll().trimmed();

    ui->lineEditN->setText(data);

    file.close();
}

void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Зберегти файл");

    if (fileName.isEmpty()) return;

    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Помилка", "Не вдалося зберегти файл");
        return;
    }

    QTextStream out(&file);

    out << "N: " << ui->lineEditN->text() << "\n";
    out << ui->labelResult->text() << "\n";

    file.close();
}
