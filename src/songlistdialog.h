#pragma once
#include <QDialog>
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
private:
	Ui::SongListDialog* m_ui = nullptr;
};