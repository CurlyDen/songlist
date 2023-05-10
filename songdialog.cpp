#include "songdialog.h"
#include "ui_songdialog.h"
#include <QFileDialog>
#include <string>
using namespace std;

SongDialog::SongDialog(QWidget* parent): QDialog(parent), m_ui(new Ui::SongDialog())
{
	m_ui->setupUi(this);
	connect(m_ui->btnPreview, &QPushButton::clicked, this, &SongDialog::choosePreview);
    connect(m_ui->buttonBox, &QDialogButtonBox::accepted, this, &SongDialog::getData);
	QDir dir;
	if (!dir.exists(m_imageDir))
		dir.mkpath(m_imageDir);
}

SongDialog::~SongDialog()
{
	delete m_ui;
}

void SongDialog::choosePreview()
{
	const auto filename = QFileDialog::getOpenFileName(this, tr("Выберите изображение"),
		QString(), "(*.jpeg *.jpg *png *gif)");
	if (filename.isEmpty())
		return;
    this->filename = filename;
    const QString copyName = copiedPreview(filename);
	showPreview(copyName);
}

QString SongDialog::copiedPreview(const QString& filePath) const
{
	const QImage image(filePath);
	const auto smaller = image.scaled(m_ui->labelPreview->width(), m_ui->labelPreview->height(),
		Qt::AspectRatioMode::KeepAspectRatioByExpanding);

	QString name = "1.png";
	if (!smaller.save(fullImagePath(name), "PNG"))
		return QString();

	return  name;
	
}

void SongDialog::showPreview(const QString& relativePath) const
{
	if (relativePath.isEmpty())
		return;
	const auto fn = fullImagePath(relativePath);
	if (!QFile::exists(fn))
		return;

	const QPixmap pixmap(fn);	
	m_ui->labelPreview->setPixmap(pixmap);
}


QString SongDialog::fullImagePath(const QString& relativePath) const
{
	QDir dir;
	dir.cd(m_imageDir);
	return dir.absoluteFilePath(relativePath);
}

QStringList SongDialog::getData()
{
    QStringList data;
    data << m_ui->lineEdit->text();
    data << m_ui->lineEdit_2->text();
    data << QString::number(m_ui->spinBox->value());
    data << QString::number(m_ui->horizontalSlider->value());
    data << this->filename;
    return data;
}

void SongDialog::setData(QList<QStandardItem*> data)
{
    const QString copyName = copiedPreview(data.at(0)->accessibleDescription());
    showPreview(copyName);
    this->filename = data.at(0)->accessibleDescription();
    m_ui->lineEdit->setText(data.at(1)->text());
    m_ui->lineEdit_2->setText(data.at(2)->text());
    m_ui->spinBox->setValue(data.at(3)->text().toInt());
    m_ui->horizontalSlider->setValue(data.at(4)->text().toInt());
}
