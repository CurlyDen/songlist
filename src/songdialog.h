#pragma once
#include <QDialog>

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
private:
	void choosePreview();
	QString copiedPreview(const QString& filePath) const;
	void showPreview(const QString& relativePath) const;
	QString fullImagePath(const QString& relativePath) const;
	
	Ui::SongDialog* m_ui = nullptr;
	const QString m_imageDir = "../../images";
};

