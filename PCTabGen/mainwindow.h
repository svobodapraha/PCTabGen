#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  void GenTable(int iType);
private slots:

  void on_btnGenTableKty_clicked();

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
