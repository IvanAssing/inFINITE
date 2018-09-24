/****************************************************************************
** Copyright (C) 2018 Ivan Assing da Silva
** Contact: ivanassing@gmail.com
**
** This file is part of the inFINITE project.
**
** This file is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 3 of the License, or
** (at your option) any later version.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <fea/truss3d.h>
#include <fea/frame21.h>
#include <fea/frame22.h>
#include <fea/solid3d.h>
#include <fea/solid3dreader.h>

#include <log/msg.h>
#include <log/viewer.h>

#include <gph/gph.h>
#include <gph/renderer.h>
#include <gph/gsolid31.h>
#include <gph/gsolid31toolbox.h>

#include <gph/gtruss31.h>

#include <gph/gframe21.h>
#include <gph/gframe22.h>
#include <gph/gplane22.h>

#include <gph/dxfreader.h>


#include <gph/chartxy.h>


#include <QPlainTextEdit>
#include <QTextStream>

#include "openmodelform.h"
#include "solid3dfilemanager.h"

#include "model.h"


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  setWindowIcon((QIcon(":/icons/infinite_v5.png")));
  QDir::setCurrent("../database");

  rendererMap = new QMap<QMdiSubWindow*, Gph::Renderer*>;

  QListWidget *listWidget = new QListWidget;
  Log::Msg::output = listWidget;
  Log::Viewer::windowOutput = ui->mdiArea;
  mdiArea = ui->mdiArea;

  Log::Msg::information(QString("inFINITE..."));
  Log::Msg::information(QString("Finite Element Analysis"));
  this->setWindowTitle(QString("inFINITE FEA"));

  QPlainTextEdit *text3 = new QPlainTextEdit;
  Log::Viewer::output = text3;


  // log msgs
  listWidget->setMinimumWidth(400);
  listWidget->setMinimumHeight(800);
  listWidget->setAutoScroll(true);
  ui->mdiArea->addSubWindow(listWidget);

  // signals
  connect(ui->actionOpenFile, SIGNAL(triggered(bool)), this, SLOT(openFile()));
  connect(ui->actionCloseModel, SIGNAL(triggered(bool)), this, SLOT(closeModel()));
  connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(close()));
  connect(ui->actionSolverGPU, SIGNAL(triggered(bool)), this, SLOT(solverIteractive()));
  connect(ui->actionSolverCPU, SIGNAL(triggered(bool)), this, SLOT(solverDirect()));
  connect(ui->actionSave, SIGNAL(triggered(bool)), this, SLOT(saveFile()));
  connect(ui->actionSaveAs, SIGNAL(triggered(bool)), this, SLOT(saveFileAs()));
  connect(ui->actionOpenDXFFile, SIGNAL(triggered(bool)), this, SLOT(openDXFFile()));


  connect(ui->actionCascadeSubWindows, SIGNAL(triggered(bool)), ui->mdiArea, SLOT(cascadeSubWindows()));
  connect(ui->actionTileSubWindows, SIGNAL(triggered(bool)), ui->mdiArea, SLOT(tileSubWindows()));
  connect(ui->mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(setCurrentRenderer(QMdiSubWindow*)));

  //Model model1(QString("../database/cube.fsxl"), this);


  // models toolbar
  modelsToolBar = new  QToolBar("Models toolbar", parent);
  modelsComboBox = new QComboBox();
  modelsComboBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  modelsComboBox->setMaximumWidth(200);
  connect(modelsComboBox, SIGNAL(activated(int)), this, SLOT(setCurrentModel(int)));
  modelsToolBar->addWidget(modelsComboBox);
  modelsToolBar->addSeparator();
  modelsToolBar->addAction(ui->actionCloseModel);
  addToolBar(modelsToolBar);


  this->showMaximized();
}



void MainWindow::setCurrentModel(int index)
{
  iCurrentModel = index;
  this->setWindowTitle(QString("inFINITE FEA [%1]").arg(models.at(iCurrentModel)->filename));
}

void MainWindow::setCurrentRenderer(QMdiSubWindow* window)
{
  Gph::Renderer* newCurrentRenderer;

  QMap<QMdiSubWindow*, Gph::Renderer*>::const_iterator i =rendererMap->find(window);
  if(i != rendererMap->end() && i.key() == window && i.value()!=currentRenderer) {
      newCurrentRenderer = i.value();
    }
  else
    return;

  if(currentRenderer!=nullptr)
    {
      disconnect(ui->actionTopView, SIGNAL(triggered(bool)), currentRenderer, SLOT(setXYTopView(void)));
      disconnect(ui->actionBottomView, SIGNAL(triggered(bool)), currentRenderer, SLOT(setXYBottomView(void)));
      disconnect(ui->actionRightView, SIGNAL(triggered(bool)), currentRenderer, SLOT(setYZTopView(void)));
      disconnect(ui->actionLeftView, SIGNAL(triggered(bool)), currentRenderer, SLOT(setYZBottomView(void)));
      disconnect(ui->actionFrontView, SIGNAL(triggered(bool)), currentRenderer, SLOT(setXZTopView(void)));
      disconnect(ui->actionBackView, SIGNAL(triggered(bool)), currentRenderer, SLOT(setXZBottomView(void)));
      disconnect(ui->actionIsometricView, SIGNAL(triggered(bool)), currentRenderer, SLOT(setIsometricView(void)));
      disconnect(ui->actionOrthographicProjection, SIGNAL(triggered(bool)), currentRenderer, SLOT(setParallelProjection(void)));
      disconnect(ui->actionPerpectiveProjection, SIGNAL(triggered(bool)), currentRenderer, SLOT(setPerspectiveProjection(void)));
      disconnect(ui->actionScreenshoot, SIGNAL(triggered(bool)), currentRenderer, SLOT(screenshot(void)));
    }
  currentRenderer = newCurrentRenderer;

  connect(ui->actionTopView, SIGNAL(triggered(bool)), currentRenderer, SLOT(setXYTopView(void)));
  connect(ui->actionBottomView, SIGNAL(triggered(bool)), currentRenderer, SLOT(setXYBottomView(void)));
  connect(ui->actionRightView, SIGNAL(triggered(bool)), currentRenderer, SLOT(setYZTopView(void)));
  connect(ui->actionLeftView, SIGNAL(triggered(bool)), currentRenderer, SLOT(setYZBottomView(void)));
  connect(ui->actionFrontView, SIGNAL(triggered(bool)), currentRenderer, SLOT(setXZTopView(void)));
  connect(ui->actionBackView, SIGNAL(triggered(bool)), currentRenderer, SLOT(setXZBottomView(void)));
  connect(ui->actionIsometricView, SIGNAL(triggered(bool)), currentRenderer, SLOT(setIsometricView(void)));
  connect(ui->actionOrthographicProjection, SIGNAL(triggered(bool)), currentRenderer, SLOT(setParallelProjection(void)));
  connect(ui->actionPerpectiveProjection, SIGNAL(triggered(bool)), currentRenderer, SLOT(setPerspectiveProjection(void)));
  connect(ui->actionScreenshoot, SIGNAL(triggered(bool)), currentRenderer, SLOT(screenshot(void)));


}

void MainWindow::addRendererWindow(Gph::Renderer* renderer)
{
  renderer->setMinimumSize(QSize(400,300));
  ui->mdiArea->addSubWindow(renderer);

  QObject::connect(renderer, SIGNAL(closeRenderer(Gph::Renderer*)), this, SLOT(closeRendererWindow(Gph::Renderer*)));

  currentRenderer = renderer;
  renderer->show();
  rendererMap->insert(ui->mdiArea->currentSubWindow(), renderer);

  setCurrentRenderer(ui->mdiArea->currentSubWindow());
}



void MainWindow::closeRendererWindow(Gph::Renderer* renderer)
{
  QMap<QMdiSubWindow*, Gph::Renderer*>::const_iterator i = rendererMap->constBegin();
  while (i != rendererMap->constEnd()) {
      if(i.value() == renderer)
        {
          rendererMap->remove(i.key());
          break;
        }
      ++i;
    }

  currentRenderer = nullptr;
  if(!rendererMap->isEmpty())
    setCurrentRenderer(rendererMap->lastKey());

}




void MainWindow::openFile(void)
{

  QString filename =
      QFileDialog::getOpenFileName(this, QObject::tr("Open inFINITE Mesh File"),
                                   QDir::currentPath(),
                                   QObject::tr("All Files (*.*);; Solid 3D Files (*.fsxl);;Ansys CDB File (*.cdb);;Truss3D Files (*.ftxl *.ft3d);;Frame2 File (*.ff2d);;Plane2 File (*.fp2d);;DXF File (*.dxf)"));
  if (filename.isEmpty())
    return;

  models.append(new Model(filename, this));
  updateOpenedModelsList();
}


void MainWindow::closeModel(void)
{
  if(models.count()>0)
    {
      models.removeAt(iCurrentModel);
      updateOpenedModelsList();
    }
}

void MainWindow::updateOpenedModelsList(void)
{

  QStringList str_list;
  for(int i=0; i<models.count(); i++)
    str_list.append(models.at(i)->name);
  modelsComboBox->clear();
  modelsComboBox->addItems(str_list);
  modelsComboBox->setCurrentIndex(models.count()-1);
  setCurrentModel(models.count()-1);

}


void MainWindow::saveFile(void)
{
  models.at(iCurrentModel)->save();
}

void MainWindow::saveFileAs(void)
{
  QString filename =
      QFileDialog::getOpenFileName(this, QObject::tr("Save As inFINITE Mesh File"),
                                   QDir::currentPath(),
                                   QObject::tr("All Files (*.*);; Solid 3D Files (*.fsxl);;Ansys CDB File (*.cdb);;Truss3D Files (*.ftxl *.ft3d);;Frame2 File (*.ff2d);;Plane2 File (*.fp2d);;DXF File (*.dxf)"));
  if (filename.isEmpty())
    return;
  models.at(iCurrentModel)->saveas(filename);
}

void MainWindow::solverIteractive(void)
{
  models.at(iCurrentModel)->solve(true);
}
void MainWindow::solverDirect(void)
{
  models.at(iCurrentModel)->solve(false);
}


 void MainWindow::openDXFFile(void)
 {
//   Gph::DXFReader *reader = new Gph::DXFReader(this);
//   ui->mdiArea->addSubWindow(reader);
//   reader->show();

   QString filename =
       QFileDialog::getOpenFileName(this, QObject::tr("DXF Translator inFINITE Mesh File"),
                                    QDir::currentPath(),
                                    QObject::tr("All Files (*.*);;DXF File (*.dxf)"));
   if (filename.isEmpty())
     return;

   Gph::DXFReader reader2(filename);
   reader2.update();
   reader2.toPlane2();
 }


MainWindow::~MainWindow()
{
  delete ui;
}
