#include "databasecreator.h"
#include "ui_databasecreator.h"

#include <QDir>
#include <QFileDialog>

#include <QString>

DatabaseCreator::DatabaseCreator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DatabaseCreator)
{
    ui->setupUi(this);
}

DatabaseCreator::~DatabaseCreator()
{
    delete ui;
}

void DatabaseCreator::on_pushButton_read_browse_clicked()
{
    QString str = QFileDialog::getExistingDirectory(this, tr("Directory"));

    ui->lineEdit_read->setText(str);
}

void DatabaseCreator::on_pushButton_save_browse_clicked()
{
    QString str = QFileDialog::getSaveFileName(this, tr("Save file"), tr(""), tr("Valami (*.valami)"));

    ui->lineEdit_save->setText(str);
}

void DatabaseCreator::on_pushButton_create_clicked()
{
}
