#ifndef FACERECOGNIZERSELECTORWIDGET_H
#define FACERECOGNIZERSELECTORWIDGET_H

#include <QDialog>

namespace Ui {
class FaceRecognizerSelectorWidget;
}

class FaceRecognizerSelectorWidget : public QDialog
{
	Q_OBJECT

public:
	explicit FaceRecognizerSelectorWidget(QWidget *parent = 0);
	~FaceRecognizerSelectorWidget();

private slots:
	void on_comboBox_activated(int index);
	void on_pushButton_clicked();

private:
	Ui::FaceRecognizerSelectorWidget *ui;
};

#endif // FACERECOGNIZERSELECTORWIDGET_H
