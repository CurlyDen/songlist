#pragma once
#include <QDialog>
#include <QStandardItem>

namespace Ui 
{	
class SongDialog;
}

class SongDialog : public QDialog
{
	Q_OBJECT
public:
	SongDialog(QWidget* parent = nullptr);
	~SongDialog();
    QStringList getData();
    void setData(QList<QStandardItem*> data);

private:
	void choosePreview();
	QString copiedPreview(const QString& filePath) const;
	void showPreview(const QString& relativePath) const;
	QString fullImagePath(const QString& relativePath) const;
	
	Ui::SongDialog* m_ui = nullptr;
	const QString m_imageDir = "../../images";
    QString filename;
};

