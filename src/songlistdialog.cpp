#include "songlistdialog.h"

#include "songdialog.h"
#include "ui_songlistdialog.h"

SongListDialog::SongListDialog(QWidget *parent): QDialog(parent)
{
	m_ui = new Ui::SongListDialog();
	m_ui->setupUi(this);
}

SongListDialog::~SongListDialog()
{
	delete m_ui;
}

void SongListDialog::on_btnCreate_clicked()
{
	SongDialog dlg;
	dlg.exec();
}
