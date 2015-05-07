#ifndef DATABASECREATOR_H
#define DATABASECREATOR_H

#include <QDialog>
#include <QProgressBar>

#include "database.h"
#include "facepreprocessorfactory.h"

namespace Ui {
class DatabaseCreator;
}

class DatabaseCreator : public QDialog
{
    Q_OBJECT

	FacePreprocessorFactory preprocessorFactory;

	Database* database = nullptr;

public:
    explicit DatabaseCreator(QWidget *parent = 0);
    ~DatabaseCreator();

	void SetDatabase(Database* database);

private slots:
    void on_pushButton_read_browse_clicked();
    void on_pushButton_save_browse_clicked();
    void on_pushButton_create_clicked();

private:
    Ui::DatabaseCreator *ui;
};

#endif // DATABASECREATOR_H
