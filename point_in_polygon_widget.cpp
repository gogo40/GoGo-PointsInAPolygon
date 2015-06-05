/*
Point in Polygon Widget

A small widget to test if a point is inside a polygon

(using a GPS and GPS points)

(c) 2015, Péricles Lopes Machado
*/

#include "point_in_polygon_widget.h"
#include "ui_pointinpolygon.h"

#include <cmath>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <map>
#include <tuple>

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

    //Initialize GPS
    position_info_ = QGeoPositionInfoSource::createDefaultSource(this);
    dist_acc_= 0;
    is_first_distance_ = true;
    if (position_info_) {
        position_info_->startUpdates();
        connect(position_info_, SIGNAL(positionUpdated(QGeoPositionInfo)),
                this, SLOT(positionUpdated(QGeoPositionInfo)));
    }
}

PointInPolygonWidget::~PointInPolygonWidget()
{
    delete ui;
}

void PointInPolygonWidget::on_addPointButton_clicked()
{
    add_new_point(ui->mine->text(), ui->point->text(), ui->xNewPoint->value(), ui->yNewPoint->value());
    update_points_map();
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

    update_points_map();
}

void PointInPolygonWidget::on_verifyPointButton_clicked()
{
    PointInPolygonTest::Point p;

    p.x = ui->xPoint->value();
    p.y = ui->yPoint->value();

    verify_point(p);
}

void PointInPolygonWidget::on_loadFromCSVFileButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(0, "Carregar pontos dos polígonos do arquivo CSV",
                                                     "~",
                                                     "Todos (*.*);;"
                                                     "CSV (*.csv *.CSV *.txt)");

    QFile file(file_name);

    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(0, "Falha ao abrir arquivo!",
                             "Ocorreu um erro ao abrir o arquivo: " + file_name +
                             ". Erro: " + file.errorString());
        return;
    }

    while (!file.atEnd()) {
        QString line = file.readLine();
        QStringList content = line.split(",");

        if (content.size() >= 4) {
            const QString& mine = content.at(0);
            const QString& point = content.at(1);

            double x = content.at(2).toDouble();
            double y = content.at(3).toDouble();

            add_new_point(mine, point, x, y);
        }
    }

    file.close();
    update_points_map();
}

void PointInPolygonWidget::add_new_point(const QString &mine, const QString &point, double x, double y)
{
    int nr = ui->PolygonTable->rowCount();
    ui->PolygonTable->setRowCount(nr + 1);

    for (int i = 0; i < 4; ++i) {
        if (!ui->PolygonTable->item(nr, i)) {
            ui->PolygonTable->setCellWidget(nr, i, new QLabel);
        }
    }

    static_cast<QLabel*>(ui->PolygonTable->cellWidget(nr, 0))->setText(mine);
    static_cast<QLabel*>(ui->PolygonTable->cellWidget(nr, 1))->setText(point);
    static_cast<QLabel*>(ui->PolygonTable->cellWidget(nr, 2))->setText(QString::number(x));
    static_cast<QLabel*>(ui->PolygonTable->cellWidget(nr, 3))->setText(QString::number(y));
}

void PointInPolygonWidget::update_points_map()
{
    int nr = ui->PolygonTable->rowCount();
    pts_map_.clear();
    for (int i = 0; i < nr; ++i) {
        PointInPolygonTest::Point pt;
        QString mine = static_cast<QLabel*>(ui->PolygonTable->cellWidget(i, 0))->text();
        QString point = static_cast<QLabel*>(ui->PolygonTable->cellWidget(i, 1))->text();
        PointInPolygonTest::Points& pts = pts_map_[IdMine(mine, point)];

        pt.x = static_cast<QLabel*>(ui->PolygonTable->cellWidget(i, 2))->text().toDouble();
        pt.y = static_cast<QLabel*>(ui->PolygonTable->cellWidget(i, 3))->text().toDouble();

        pts.push_back(pt);
    }
}

void PointInPolygonWidget::verify_point(const PointInPolygonTest::Point &p)
{
    for (auto it: pts_map_) {
        const IdMine& id = it.first;
        PointInPolygonTest::Points& pts = it.second;


        std::size_t n = pts.size();

        if (fabs(pts[n - 1].x - pts[0].x) > 1e-8 ||
            fabs(pts[n - 1].y - pts[0].y) > 1e-8) {
            pts.push_back(pts[0]);
        }

        int wn = wn_PnPoly(p, pts);

        if (wn != 0) {
            QMessageBox::warning(0, "Ponto dentro de um polígono!", "O ponto (" +
                                 QString::number(p.x) + ", " +
                                 QString::number(p.y) + ") está dentro do polígono:\n"
                                 "Mina: " + std::get<0>(id) + "\n" +
                                 "Ponto: " + std::get<1>(id));
            return;
        }
    }

    QMessageBox::warning(0, "Ponto fora de todos polígonos!", "O ponto (" +
                         QString::number(p.x) + ", " +
                         QString::number(p.y) + ") está fora de todos polígonos.");
}

void PointInPolygonWidget::on_testPointsFromCSVButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(0, "Carregar pontos de teste do arquivo CSV",
                                                     "~",
                                                     "Todos (*.*);;"
                                                     "CSV (*.csv *.CSV *.txt)");

    QFile file(file_name);

    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(0, "Falha ao abrir arquivo!",
                             "Ocorreu um erro ao abrir o arquivo: " + file_name +
                             ". Erro: " + file.errorString());
        return;
    }

    while (!file.atEnd()) {
        QString line = file.readLine();
        QStringList content = line.split(",");

        if (content.size() >= 2) {
            PointInPolygonTest::Point p;
            p.x = content.at(0).toDouble();
            p.y = content.at(1).toDouble();

            verify_point(p);
        }
    }

    file.close();
}

void PointInPolygonWidget::positionUpdated(const QGeoPositionInfo &info)
{
    QGeoPositionInfo pos_info = info;
    QGeoCoordinate pos = pos_info.coordinate();

    if (pos.isValid()) {

        ui->xNewPoint->setValue(pos.latitude());
        ui->yNewPoint->setValue(pos.longitude());

        ui->xPoint->setValue(pos.latitude());
        ui->yPoint->setValue(pos.longitude());

        double dist = 0;
        if (is_first_distance_) {
            dist_acc_ = 0;
            is_first_distance_ = false;
        } else {
            if (std::fabs(pos.altitude()) < 1e-3) {
                pos.setAltitude(last_position_.coordinate().altitude());
                pos_info.setCoordinate(pos);
            }
            dist = pos_info.coordinate().distanceTo(last_position_.coordinate());
            if (dist > 10) {
                dist_acc_ += dist;
            }
        }
        last_position_ = pos_info;
    }
}

void PointInPolygonWidget::resetGPS()
{
    dist_acc_ = 0;
    is_first_distance_ = true;
}
