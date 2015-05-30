/*M///////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2015, Balázs Kovács, Gergő Róth
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the University of Pannonia nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//M*/

#include "databasecreator.h"
#include "ui_databasecreator.h"

#include <QDebug>

#include <QFile>
#include <QTextStream>
#include <QList>
#include <QDir>
#include <QDirIterator>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QRegExp>
#include <QMap>

#include <opencv2/opencv.hpp>

#include <vector>

#include "facerecognizercontainer.h"

DatabaseCreator::DatabaseCreator(Database& db, QWidget* parent) :
    QDialog(parent),
    database(db),
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
	QString str = QFileDialog::getSaveFileName(this, tr("Save file"), tr(""), tr("DatabaseName (*.facedb)"));

    ui->lineEdit_save->setText(str);
}

void DatabaseCreator::on_pushButton_create_clicked()
{
    QString directory = ui->lineEdit_read->text();
    QString saveFile = ui->lineEdit_save->text();

    if (!directory.isEmpty() && !saveFile.isEmpty())
    {
        static QRegExp fileNameRe("(\\w+_\\w+)_\\d+\\.*");

        QList<QFileInfo> files;

        QDirIterator it(directory, QStringList() << "*.jpg", QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext())
        {
            it.next();

            files.append(it.fileInfo());
        }

        int progressCounter = 0;
        QListIterator<QFileInfo> fileIt(files);
        while (fileIt.hasNext())
        {
            qDebug() << progressCounter << files.size();

            const QFileInfo& fileInfo = fileIt.next();
            ++progressCounter;

            if (fileNameRe.indexIn(fileInfo.fileName()) != -1)
            {
                QString name = fileNameRe.cap(1);

                try
                {
                    cv::Mat rawImage = cv::imread(fileInfo.filePath().toStdString());

                    FacePreprocessor facePreprocessor = preprocessorFactory.GetPreprocessor(rawImage, false);
                    cv::Mat preprocessedImage = facePreprocessor.Preprocess();

                    QDir dir("");
                    dir.rmdir("preprocessed");
                    dir.mkdir("preprocessed");
                    cv::imwrite(("preprocessed/" + fileInfo.fileName()).toStdString(), preprocessedImage);

                    database.AddImage(name, preprocessedImage);
                }
                catch (NoFaceFoundException)
                {
//                  qDebug() << "Could not find face" << fileInfo.filePath();
                }
            }
        }

        qDebug() << "Save name labels";
        database.ExportNameLabels(saveFile + ".nlabels");

        qDebug() << "Train face recognizer";
        database.Train();
        qDebug() << "Save face recognizer";
        database.Save(saveFile);
    }
    else
    {
        QMessageBox::critical(this, tr("Unspecified input and output"),
                              tr("Please specify the folder with the images, and the output file of the created database!"));
    }
}
