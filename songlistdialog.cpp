#include "songlistdialog.h"
#include <QStandardItemModel>
#include "songdialog.h"
#include "ui_songlistdialog.h"
#include <iostream>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <map>
using namespace std;

SongListDialog::SongListDialog(QWidget *parent): QDialog(parent)
{
    m_ui = new Ui::SongListDialog();
    m_ui->setupUi(this);

    QHeaderView* Vheader=m_ui->tableView->verticalHeader();
    QHeaderView* Hheader=m_ui->tableView->horizontalHeader();
    Vheader->setDefaultSectionSize(200); // 20 px height
    Vheader->sectionResizeMode(QHeaderView::Fixed);
    Hheader->setDefaultSectionSize(200); // 20 px height
    Hheader->sectionResizeMode(QHeaderView::Fixed);

    model = new QStandardItemModel(0, 4, this);
    m_ui->tableView->setModel(model);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Превью"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Название"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Исполнитель"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Продолжительность"));
//    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Оценка"));
    m_ui->tableView->setColumnHidden(4, true);

    QFile file;
    file.setFileName(this->filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QJsonDocument doc = QJsonDocument().fromJson(file.readAll());
    QJsonObject obj = doc.object();
    QJsonArray arr = obj["items"].toArray();
    file.close();

    int row = 0;

    foreach (const QJsonValue &value, arr) {
        QJsonObject itemObj = value.toObject();
        QString img_name = itemObj.value(model->headerData(0, Qt::Horizontal).toString()).toString();
        QStandardItem* preview = new QStandardItem();
        QImage image(img_name);
        double k = image.width()/(double)image.height();
        preview->setData(QVariant(QPixmap::fromImage(QImage(image.scaled(200, int(200 / k))))), Qt::DecorationRole);
        preview->setAccessibleDescription(img_name);
        preview->setToolTip("Оценка " + itemObj.value("Оценка").toString());
        model->setItem(row, 0, preview);
        for (int i=1; i<4; i++){
            QStandardItem *itemName = new QStandardItem(itemObj.value(model->headerData(i, Qt::Horizontal).toString()).toString());
            model->setItem(row, i, itemName);
        }
        row++;
    }


}

SongListDialog::~SongListDialog()
{
	delete m_ui;
}

void SongListDialog::on_btnCreate_clicked()
{
    SongDialog dlg = new SongDialog;
    dlg.exec();
    if (dlg.result()){
        QStringList data = dlg.getData();
        QList<QStandardItem*> newRow;
        QStandardItem* preview = new QStandardItem();
        QImage image(data[4]);
        double k = image.width()/(double)image.height();
        preview->setData(QVariant(QPixmap::fromImage(QImage(image.scaled(200, int(200 / k))))), Qt::DecorationRole);
        preview->setAccessibleDescription(data[4]);
        preview->setToolTip("Оценка " + data[3]);
        newRow.append(preview);
        newRow.append(new QStandardItem(data[0]));
        newRow.append(new QStandardItem(data[1]));
        newRow.append(new QStandardItem(data[2]));
        newRow.append(new QStandardItem(data[3]));
//        newRow.append(new QStandardItem(data[4]));
        model->appendRow(newRow);
//        m_ui->tableView->setColumnHidden(4, true);
    };
}

void SongListDialog::on_btnEdit_clicked()
{
    SongDialog dlg;
    auto index = m_ui->tableView->currentIndex();
    if (index.row() < 0){
        return;
    }

    QList<QStandardItem*> data;
    for (int i=0; i<model->columnCount(); i++){
        data << model->item(index.row(), i);
    }
    data << new QStandardItem(model->item(index.row(), 0)->toolTip().split(" ")[1]);
    dlg.setData(data);
    dlg.exec();
    if (dlg.result()){
        QStringList data = dlg.getData();
        QList<QStandardItem*> newRow;
        QStandardItem* preview = new QStandardItem();
        preview->setAccessibleDescription(data[4]);
        preview->setToolTip("Оценка " + data[3]);
        QImage image(data[4]);
        double k = image.width()/(double)image.height();
        preview->setData(QVariant(QPixmap::fromImage(QImage(image.scaled(200, int(200 / k))))), Qt::DecorationRole);
        newRow.append(preview);
        newRow.append(new QStandardItem(data[0]));
        newRow.append(new QStandardItem(data[1]));
        newRow.append(new QStandardItem(data[2]));
//        newRow.append(new QStandardItem(data[3]));
        //        newRow.append(new QStandardItem(data[4]));
        model->removeRow(index.row());
        model->insertRow(index.row(), newRow);
//        model->setHeaderData(model->rowCount(), Qt::Horizontal, data[3], Qt::ToolTipRole);
//        m_ui->tableView->setColumnHidden(4, true);
    };
}


void SongListDialog::on_btnSave_clicked()
{
    QJsonObject json;
    QJsonArray items;
    QFile file;
    file.setFileName(this->filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    for (int i=0; i<model->rowCount(); i++){
        QJsonObject row;
        for (int j=0; j<model->columnCount(); j++){
            if (j == 0) {
                row[model->headerData(j, Qt::Horizontal).toString()] = model->item(i, 0)->accessibleDescription();
            } else {
                row[model->headerData(j, Qt::Horizontal).toString()] = model->item(i, j)->text();
            }
        }
        row["Оценка"] = model->item(i)->toolTip().split(" ")[1];
        items.append(row);
    }
    json["items"] = items;
    QJsonDocument saveDoc(json);
    file.write(saveDoc.toJson());
    file.close();
}


void SongListDialog::on_btnSort_clicked()
{
    for (int i=0; i<model->rowCount(); i++){
        model->setItem(i, 4, new QStandardItem(model->item(i)->toolTip()));
    }
    model->sort(4, Qt::DescendingOrder);
    for (int i=0; i<model->rowCount(); i++){
        model->item(i)->setToolTip(model->item(i, 4)->text());
    }
    model->removeColumn(4);
}

