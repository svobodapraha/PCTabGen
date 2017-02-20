#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cmath"

FILE *fw;
double TabValue, LastTableValue;

#define knTableSize        1024
#define knStepSize           16

double const  knUVcc          =  (5000.0);
double const  knRTopDIV       =   3.0;
double const  knRBotDIV       =   1.0;
double const  knRFB           =   4.7;

double const  knRTopKTY       =   4.7;
double const  knRSerKTY       =   0;

#define knRThev           (1/((1/knRTopDIV)+(1/knRBotDIV)))
#define knUThev           (knUVcc*(knRBotDIV/(knRBotDIV+knRTopDIV)))
#define knK1              ((knRThev+knRFB)/knRThev)
#define knK2              (-knUThev*(knRFB/knRThev))

double const fAlfa = 7.635E-3;
double const fBeta = 1.731E-5;
double const fR25  = 1000;



MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}





void MainWindow::GenTable(int iType)
{
  unsigned int TabIndex;
  double Uin;
  double Rpt;
  double Ta;
  double NapetiAD;

  fputs("\n{", fw);
  for(unsigned int i=0;i < knTableSize;i++)
  {
   TabIndex = i;
   if(!(TabIndex%knStepSize))
   {
     fputs("\n  ",fw);
     fprintf(fw, "/*% 5u*/ ", TabIndex);
   }
   NapetiAD = (knUVcc/(knTableSize-1))*TabIndex;
   Uin = (NapetiAD-knK2)/knK1;
   Rpt = ((Uin*knRTopKTY)/(knUVcc - Uin))*1000.0;

    //výpočet teploty ze známého odporu
    double fKt = Rpt/fR25;
    Ta = 25+(sqrt(fAlfa*fAlfa - 4*fBeta + 4*fBeta*fKt)-fAlfa)/(2*fBeta);



    switch(iType)
    {
      case 0: TabValue = Ta * 10;             break;
      case 2: TabValue = Uin;                 break;
      case 3: TabValue = Rpt;                 break;
      case 4: TabValue = NapetiAD;            break;
    }
    fprintf(fw, "% 5d", (int) floor(TabValue+0.5));
    if(i < (knTableSize - 1)) fputs(",",fw);

  }
  fputs("\n};\n", fw);
}

void MainWindow::on_btnGenTableKty_clicked()
{
  fw=fopen("TabTempKTY83.h", "w");

  fprintf (fw,"const I16 TabTempKTY83Lin[%d]=",knTableSize);
  GenTable(0);
  fputs("\n", fw);


  fputs("/*\n", fw);
  fprintf (fw,"const I16 TabUin[%d]=",knTableSize);
  GenTable(2);
  fputs("\n", fw);

  fprintf (fw,"const I16 TabR_KTY83[%d]=",knTableSize);
  GenTable(3);
  fputs("\n", fw);

  fprintf (fw,"const I16 TabUv[%d]=",knTableSize);
  GenTable(4);
  fputs("\n", fw);

  fputs("*/\n", fw);

  fclose(fw);
}
