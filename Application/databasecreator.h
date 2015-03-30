#ifndef DATABASECREATOR_H
#define DATABASECREATOR_H

#include <QWidget>

namespace Ui {
class DatabaseCreator;
}

class DatabaseCreator : public QWidget
{
    Q_OBJECT

public:
    explicit DatabaseCreator(QWidget *parent = 0);
    ~DatabaseCreator();

private slots:
    void on_pushButton_read_browse_clicked();

    void on_pushButton_save_browse_clicked();

    void on_pushButton_create_clicked();

private:
    Ui::DatabaseCreator *ui;
};

#endif // DATABASECREATOR_H
