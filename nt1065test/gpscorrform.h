#ifndef GPSCORRFORM_H
#define GPSCORRFORM_H

#include <QWidget>
#include <QThread>
#include <QMutex>
#include <thread>
#include <map>
#include <vector>

#include "qcustomplot.h"

#include "gcacorr/gpsvis.h"

// ITs FX3
// GPS L1   = 1575.42 MHz
// RCV FREQ = 1590.00 MHz
//#define RCV_SR          (  53.000e6 )
//#define GPS_FREQ_IN_RCV  ( -14.58e6 )
//#define ADC_DATA_TYPE   ( DT_INT16_REAL )
//#define DATA_SIZE       (    53000 )

// ITs FX3 GPS special w injector
#define RCV_SR          ( 16.368e6 )
#define GPS_FREQ_IN_RCV  ( 4.092e6 )
#define ADC_DATA_TYPE   ( DT_INT16_REAL )
#define DATA_SIZE       ( 16368 )

static const int PRN_CNT = 30;
static const int PRN_MAX = 33;
static const int PRN_IN_OPER[ PRN_CNT ] =
{ 1, 2, 3,
  5, 6, 7, 8, 9,
  11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32
};


struct plot_data_t {
    std::vector<double> freqs_vals;
    std::vector< std::vector<float> > cors;
    int center;
    bool inited;
    QMutex* mutex;
    plot_data_t() : inited( false ){ mutex = new QMutex(); }
    ~plot_data_t() { delete mutex; }
};

namespace Ui {
class GPSCorrForm;
}

class GPSCorrForm : public QWidget
{
    Q_OBJECT

public:
    explicit GPSCorrForm(QWidget *parent = 0);
    ~GPSCorrForm();

private:
    Ui::GPSCorrForm *ui;
    QCustomPlot* plotCorrAll;
    QCustomPlot* plotCorrGraph;

    QCPGraph* gr_vis;
    QCPGraph* gr_inv;

    QVector<double> visible_sats;
    QVector<double> invisible_sats;
    QVector<double> visible_corrs;
    QVector<double> invisible_corrs;

    bool working;
    std::vector< RawSignal* > sigs;
    void calcSats();

    bool running;
    std::thread calc_thread;
    void calcLoop( void );

    void redrawVisGraph();

    std::vector< plot_data_t > cdata;

private slots:
    void satChanged(int prn, float corr, int shift, double freq, bool is_visible );
    void cellSelected( int, int );

public slots:
    void processRawData( const std::vector<short>* data );

signals:
    void satInfo( int prn, float corr, int shift, double freq, bool is_visible );
};

#endif // GPSCORRFORM_H
