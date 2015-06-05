/*
Point in Polygon Widget

A small widget to test if a point is inside a polygon

(using a GPS and GPS points)

(c) 2015, Péricles Lopes Machado
*/

#include "point_in_polygon_test.h"
#include "point_in_polygon_widget.h"
#include "ui_pointinpolygon.h"

#include <QMessageBox>

PointInPolygonWidget::PointInPolygonWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PointInPolygon)
{
    ui->setupUi(this);
    ui->PolygonTable->setContextMenuPolicy(Qt::ActionsContextMenu);

    QAction* menu_action_remove_row;
    menu_action_remove_row = new QAction("Remover", this);
    ui->PolygonTable->addAction(menu_action_remove_row);
    QObject::connect(menu_action_remove_row, SIGNAL(triggered()),
                     this, SLOT(remove_row()));
}

PointInPolygonWidget::~PointInPolygonWidget()
{
    delete ui;
}

void PointInPolygonWidget::on_addPointButton_clicked()
{
    int nr = ui->PolygonTable->rowCount();
    ui->PolygonTable->setRowCount(nr + 1);

    for (int i = 0; i < 4; ++i) {
        if (!ui->PolygonTable->item(nr, i)) {
            ui->PolygonTable->setCellWidget(nr, i, new QLabel);
        }
    }

    static_cast<QLabel*>(ui->PolygonTable->cellWidget(nr, 0))->setText(ui->mine->text());
    static_cast<QLabel*>(ui->PolygonTable->cellWidget(nr, 1))->setText(ui->point->text());
    static_cast<QLabel*>(ui->PolygonTable->cellWidget(nr, 2))->setText(ui->xNewPoint->text());
    static_cast<QLabel*>(ui->PolygonTable->cellWidget(nr, 3))->setText(ui->yNewPoint->text());
}

void PointInPolygonWidget::remove_row()
{
    auto items = ui->PolygonTable->selectedRanges();

    QString list;
    for (int i = items.size() - 1; i > -1; --i) {
       auto item = items[i];
       for (int j = item.bottomRow(); j <= item.topRow(); ++j) {
           ui->PolygonTable->removeRow(j);
           list += QString::number(j) + "\n";
       }
    }

    QMessageBox::warning(0, "Removed items", list);
}

void PointInPolygonWidget::on_verifyPointButton_clicked()
{

}
