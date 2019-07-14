#include "widget.h"
#include "ui_widget.h"
#include "widget.h"
#include <QDebug>
#define MAXTIME 120
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
    axisY->setRange(10,70);

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
    double zT = 100;
    double dT = tTerm - tUst;
    static int direction = 0; //0-нагрев. 1- охлаждение
    static int oldDirection = direction;
    if (I < -0.1)
        direction = 0;
    if (I > 0.1)
        direction = 1;
    if (direction !=oldDirection){
        oldDirection = direction;
        //I = 0;
    }
    P = dT / z;
    if (ui->pCB->isChecked()) {
        if (P < -zT) P = - zT;
        if (P > zT) P = zT;
    }

    if (time == 0.) I = 0;
    I += i * dT * dTime / 10000;
    //if (ui->pCB->isChecked()) {
        if (I < -zT) I = - zT;
        if (I > zT) I = zT;
    //}

    static double tlast = tTerm;
    D = d * (tTerm - tlast) / 100./dTime;
    if (ui->pCB->isChecked()) {
        if (D<-1*zT) D = - 1 * zT;
        if (D>1*zT) D = 1 * zT;
    }
    tlast = tTerm;
    double pidK = P + I + D;
    if (direction == 0) {
        if (pidK > 0) pidK = 0;
        if (pidK < -1) pidK = -1;
    }
    else {
        if (pidK > 1) pidK = 1;
        if (pidK < 0) pidK = 0;
    }
    //qDebug()<<tTerm<<p<<i<<d<<pidK<<sDT;
    return pidK;
}

double Widget::pidOld(double tTerm, double tUst,double time,double dTime)
{
    double zT = 10;
    double dT = tTerm - tUst;
    static int direction = 0; //0-нагрев. 1- охлаждение
    static int oldDirection = direction;
    if (dT < -0.5)
        direction = 0;
    if (dT > 0.5)
        direction = 1;
    if (direction !=oldDirection){
        oldDirection = direction;
        //I = 0;
    }
    P = dT / z;
    if (ui->pCB->isChecked()) {
        if (P<-zT) P = - zT;
        if (P>zT) P = zT;
    }

    if (time == 0.) I = 0;
    I += i*dT*dTime/10000;
    //if (ui->pCB->isChecked()) {
        if (I < -zT) I = - zT;
        if (I > zT) I = zT;
    //}

    static double tlast = tTerm;
    D = d * (tTerm - tlast) / 100./dTime;

    if (D<-zT) D = - zT;
    if (D>zT) D = zT;

    tlast = tTerm;
    double pidK = P + I + D;
    if (direction == 0) {
        if (pidK > 0) pidK = 0;
        if (pidK < -1) pidK = -1;
    }
    else {
        if (pidK > 1) pidK = 1;
        if (pidK < 0) pidK = 0;
    }
    //qDebug()<<tTerm<<p<<i<<d<<pidK<<sDT;
    return pidK;
}

void Widget::updateGraf()
{
    qDebug()<<ui->pCB->isChecked()<<"!";
    qDebug()<<"!";
    double pGen = ui->pGenDSB->value();
    double pPel = ui->pPelDSB->value();
    double dTime = 0.001;
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
//        if(time > MAXTIME && time < 2 * MAXTIME)
//            pGen=-ui->pGenDSB->value();
//        else
//            pGen=ui->pGenDSB->value();

        pGen +=120* dTime / MAXTIME / PERIOD;
        double tDL = eDL  /qDL - 273;
        double tTerm = eTerm  /qTerm - 273;
        double Pterm = k * (tDL - tTerm);
        eTerm += Pterm * dTime;
        double dE = !ui->pCB->isChecked() ?
                pid(tTerm,tUst,time,dTime) * pPel * dTime :
                 pidOld(tTerm,tUst,time,dTime) * pPel * dTime;
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

