/**
* @file   mainwindow.h
* @author Liangfu Chen <liangfu.chen@nlpr.ia.ac.cn>
* @date   Thu Jan 23 20:35:25 2014
* 
* @brief  
* 
* 
*/

#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include "glcanvas.h"

#include <stdio.h>
#include <assert.h>

#include "ui_brainbrowser.h"
#include <QtGui>

#include "utility.h"

class MainWindow : public QMainWindow,public Ui::MainWindow
{
  Q_OBJECT

private:
  GLCanvas * glcanvas;

public:
  // Worker * m_pDataWorker;
  static const int DisplayMode_IMAGE=1;
  static const int DisplayMode_MESH=2;

  void openFile(QString fname)
  {
    if (!fname.isEmpty())
    {
      qDebug(fname.toAscii());
      char tmp[1024];
      sprintf(tmp,"%s",qPrintable(fname));
      char suffix[1024];
      cvGetFileSuffix(tmp,suffix);
      if (!strncmp(suffix+1,"obj",3)){
        //displayMesh(fname);
        glcanvas->display(fname);
      }else if (!strncmp(suffix+1,"rawiv",5)){
      }else{
        QMessageBox::warning(this,"Load data error","File extension not recognized!");
      }
    }
  }

protected:
  void dragEnterEvent(QDragEnterEvent *e)
  {
    if (e->mimeData()->hasUrls()) {
      e->acceptProposedAction();
    }
  }

  void dropEvent(QDropEvent *e)
  {
    int i;
    for (i=0;i<e->mimeData()->urls().count();i++) {
      const QUrl url=e->mimeData()->urls()[i];
      const QString fname = url.toLocalFile();
      openFile(fname);
    }
  }

private slots:
  void on_actionOpen_triggered()
  {
    QString fname =
      QFileDialog::getOpenFileName(this,"select file",".",
      "Mesh data [*.obj] (*.obj);;"
      "Volume data [*.rawiv] (*.rawiv)");
    openFile(fname);
  }

  void on_actionToolbar_toggled()
  {
    if (actionToolbar->isChecked()){
      toolBar->show();
    }else{
      toolBar->hide();
    }
  }

  void on_actionStatusbar_toggled()
  {
    if (actionStatusbar->isChecked()){
      statusbar->show();
    }else{
      statusbar->hide();
    }
  }

  void on_actionAbout_triggered()
  {
    QMessageBox::about(this, tr("About CompVis"),
      tr("<b>CompVis</b> is an application designed for "
      "vision-base media content manipulations."));
  }

//private slots:
//  void postprocess()
//  {
//    if (glcanvas){
//      if (!glcanvas->initialized()){glcanvas->postprocess();}
//    }
//  }

public:
  MainWindow(QWidget * parent=0):QMainWindow(parent),glcanvas(new GLCanvas(this))//,m_initialized(0)//
  {
    setupUi(this);
    setCentralWidget(glcanvas);
    glcanvas->updateGL();

    //QTimer * timer=new QTimer(this);
    //timer->setSingleShot(1);
    //connect(timer,SIGNAL(timeout()),this,SLOT(postprocess()));
    //timer->start(200);
  }

  ~MainWindow()
  {
    if (glcanvas) { delete glcanvas; glcanvas=0; }
  }
};

#endif // __MAIN_WINDOW_H__
