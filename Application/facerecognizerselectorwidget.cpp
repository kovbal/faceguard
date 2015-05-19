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

#include "facerecognizerselectorwidget.h"
#include "ui_facerecognizerselectorwidget.h"

#include "facerecognizercontainer.h"

FaceRecognizerSelectorWidget::FaceRecognizerSelectorWidget(QWidget* parent) :
    QDialog(parent),
    recognizer(FaceRecognizerContainer::Instance()),
    ui(new Ui::FaceRecognizerSelectorWidget)
{
    ui->setupUi(this);

    ui->comboBox->addItem("Eigen Face Recognizer");
    ui->comboBox->addItem("Fisher Face Recognizer");
    ui->comboBox->addItem("LBPH Face Recognizer");

    ui->comboBox->setCurrentIndex(-1);
    ui->stackedWidget->setCurrentIndex(0);
}

FaceRecognizerSelectorWidget::~FaceRecognizerSelectorWidget()
{
    delete ui;
}

void FaceRecognizerSelectorWidget::on_comboBox_activated(int index)
{
    ui->stackedWidget->setCurrentIndex(index + 1);
}

void FaceRecognizerSelectorWidget::on_pushButton_clicked()
{
    switch (ui->comboBox->currentIndex())
    {
    case 0:
    {
        recognizer->SetEigenFaceRecognizer(ui->spinBox_eigenNumComponents->value());

        break;
    }
    case 1:
    {
        recognizer->SetFisherFaceRecognizer(ui->spinBox_fisherNumComponents->value());

        break;
    }
    case 2:
    {
        recognizer->SetLBPHFaceRecognizer(
            ui->spinBox_lbphRadius->value(),
            ui->spinBox_lbphNeighbors->value(),
            ui->spinBox_lbphGridX->value(),
            ui->spinBox_lbphGridY->value());

        break;
    }
    }
}
