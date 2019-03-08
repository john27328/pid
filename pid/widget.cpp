#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#define MAXTIME 60
#define PERIOD 3
#define PGEN 30

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    chart = new QChart();

    series = new QLineSeries();
    series2 = new QLineSeries();
    series->setName("температура ДЛ");
    series2->setName("температура на термистре");
    seriesP = new QLineSeries();
    seriesI = new QLineSeries();
    seriesD = new QLineSeries();
    seriesP->setName("P");
    seriesI->setName("I");
    seriesD->setName("D");

    chart->addSeries(series);
    chart->addSeries(series2);
    chart->addSeries(seriesP);
    chart->addSeries(seriesI);
    chart->addSeries(seriesD);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setTitle("QT Charts example");

    axisX = new QValueAxis;
    axisY = new QValueAxis;
    axisX->setLabelFormat("%f");
    axisY->setLabelFormat("%f");
    axisX->setTitleText("Время");
    axisY->setTitleText("Температура/ ПИД -5..5");

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    series->attachAxis(axisX);
    series->attachAxis(axisY);
    series2->attachAxis(axisX);
    series2->attachAxis(axisY);
    seriesP->attachAxis(axisX);
    seriesP->attachAxis(axisY);
    seriesI->attachAxis(axisX);
    seriesI->attachAxis(axisY);
    seriesD->attachAxis(axisX);
    seriesD->attachAxis(axisY);

    axisX->setRange(0,MAXTIME*PERIOD);
    axisY->setRange(10,30);

    chartView = new QChartView(chart);
    ui->mainLA->addWidget(chartView);
    updateGraf();
}

Widget::~Widget()
{
    delete ui;
}

double Widget::pid(double tTerm, double tUst,double time,double dTime)
{
    double zT = 1;
    double dT = tTerm - tUst;
    P = dT / z;
    if (ui->pCB->isChecked()) {
        if (P<-zT) P = - zT;
        if (P>zT) P = zT;
    }

//    static double sDT = 0;
//    if (time == 0.) sDT = 0;
//    sDT += dT*dTime;
//    if (sDT<-1) sDT = - 1;
//    if (sDT>1) sDT = 1;
//    I = i * sDT/1000;

    if (time == 0.) I = 0;
    I += i*dT*dTime/10000;
    //if (ui->pCB->isChecked()) {
        if (I<-1) I = - 1;
        if (I>1) I = 1;
    //}

    static double tlast = tTerm;
    D = d * (tTerm - tlast) / 100./dTime;
    if (ui->pCB->isChecked()) {
        if (D<-1*zT) D = - 1*zT;
        if (D>1*zT) D = 1*zT;
    }
    tlast = tTerm;
    double pidK = P + I + D;
    if (pidK > 1) pidK = 1;
    if (pidK < 0) pidK = 0;
    //qDebug()<<tTerm<<p<<i<<d<<pidK<<sDT;
    return pidK;
}

void Widget::updateGraf()
{
    qDebug()<<ui->pCB->isChecked()<<"!";
     qDebug()<<"!";
    double pGen = ui->pGenDSB->value();
    double pPel = ui->pPelDSB->value();
    double dTime = 0.0001;
    qDL = ui->qDSB->value();
    k = ui->kDSB->value()/100.;
    qTerm = ui->qkDSB->value()/100.;
    z = ui->zSB->value();
    i = ui->iSB->value();
    d = ui->dSB->value();
    tUst = ui->tDSB->value();
    series->clear();
    seriesP->clear();
    seriesI->clear();
    seriesD->clear();
    series2->clear();
    double eDL = 300 * qDL;
    double eTerm = 300 * qTerm;
    int tpl=0;
    for(double time = 0; time <= MAXTIME * PERIOD; time += dTime)
    {
        if(time > MAXTIME && time < 2 * MAXTIME) pGen=10;
        else pGen=ui->pGenDSB->value();;
        double tDL = eDL  /qDL - 273;
        double tTerm = eTerm  /qTerm - 273;
        double Pterm = k * (tDL - tTerm);
        eTerm += Pterm * dTime;
        double dE = pid(tTerm,tUst,time,dTime) * pPel * dTime;
        eDL += pGen * dTime - dE;
        //ui->graf->addDots(tDL,tTerm,time);
        tpl++;
        if(tpl == (int)(MAXTIME/dTime/100))
        {
            tpl = 0;
            series->append(time, tDL);
            series2->append(time, tTerm);
            seriesP->append(time,P/5*(30-10)+20);
            seriesI->append(time,I/5*(30-10)+20);
            seriesD->append(time,D/5*(30-10)+20);
            qDebug()<<tTerm<<tDL<<time;

        }
    }
    //qDebug()<<"!!!!";
    chartView->update();
}

