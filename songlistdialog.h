#pragma once
#include <QDialog>
#include <QStandardItemModel>
#include <string>
namespace Ui 
{
class SongListDialog;
}

class SongListDialog : public QDialog 
{
Q_OBJECT

public:
	SongListDialog(QWidget *parent = nullptr);
	~SongListDialog();
public slots:
	void on_btnCreate_clicked();

    void on_btnEdit_clicked();

    void on_btnSave_clicked();

private slots:
    void on_btnSort_clicked();

private:
	Ui::SongListDialog* m_ui = nullptr;
    QStandardItemModel* model;
    QString filename = "data.json";
};
