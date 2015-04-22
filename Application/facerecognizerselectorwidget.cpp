#include "facerecognizerselectorwidget.h"
#include "ui_facerecognizerselectorwidget.h"

#include "facerecognizercontainer.h"

FaceRecognizerSelectorWidget::FaceRecognizerSelectorWidget(QWidget *parent) :
	QDialog(parent),
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
			FaceRecognizerContainer::Instance()->SetEigenFaceRecognizer(ui->spinBox_eigenNumComponents->value());

			break;
		}
		case 1:
		{
			// TODO

			break;
		}
		case 2:
		{
			// TODO

			break;
		}
	}
}
