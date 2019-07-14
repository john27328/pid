#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtCharts/QtCharts>
QT_CHARTS_USE_NAMESPACE

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    double pid(double tTerm, double tUst, double time, double dTime);
    QChart *chart;
    QLineSeries *series;
    QLineSeries *seriesI;
    QLineSeries *seriesP;
    QLineSeries *seriesD;

    QLineSeries *series2;
    QValueAxis *axisX;
    QValueAxis *axisY;
    QChartView *chartView;
    double qDL;
    double k;
    double qTerm;
    double z;
    int i ;
    int d;
    double tUst;
    double P;
    double I;
    double D;
    double pidOld(double tTerm, double tUst, double time, double dTime);
private slots:
    void updateGraf();

};

#endif // WIDGET_H
