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


#include "model.h"

#include <QString>
#include <QFile>
#include <QFileInfo>
#include "mainwindow.h"

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

#include "solid3dfilemanager.h"

#include <log/viewer.h>


Model::Model(QString filename_, MainWindow *parent_)
  :filename(filename_), parent(parent_)
{
  this->update();
}

void Model::update(void)
{

  QFileInfo fileinfo(filename);
  QString type = fileinfo.completeSuffix();
  name = fileinfo.baseName();

  if (type == "fsxl" || type == "cdb")
    {

      this->gmodel = new Gph::GSolid31(parent, name);
      QObject::connect(gmodel, SIGNAL(newRenderer(Gph::Renderer*)), parent, SLOT(addRendererWindow(Gph::Renderer*)));

      Gph::GSolid31ToolBox *gsolidtoolbox = new Gph::GSolid31ToolBox(static_cast<Gph::GSolid31*> (gmodel), static_cast<QWidget*> (parent));

      Solid3DFileManager s3d_fileManager(gsolidtoolbox->treeWidget);

      s3d_fileManager.currentfilename = filename;
      if(!s3d_fileManager.openFile())
        return;


      this->mesh = new Fea::Solid3D;
      Fea::Solid3D *mesh_local = static_cast<Fea::Solid3D*>(this->mesh);

      Fea::Solid3DReader reader(static_cast<Fea::Solid3D*>(mesh));
      mesh = reader.read(s3d_fileManager.currentfilename);

      Log::Msg::information("Solid31 model loaded");
      Log::Msg::result(QString("%1 nodes and %2 elements").arg(mesh_local->nNodes).arg(mesh_local->nElements));

      mesh_local->evaluateModel();
      double volume, weight;
      mesh_local->infoGeometry(volume, weight);
      Log::Msg::result(QString("Model's volume: %1").arg(volume));
      Log::Msg::result(QString("Model's weight: %1").arg(weight));

      parent->mdiArea->addSubWindow(gsolidtoolbox);

      gsolidtoolbox->show();
      gmodel->setFeaMesh(this->mesh);
//      mesh_local->evalStiffnessMatrix();
//      mesh_local->evalLoadVector();

      gmodel->setFeaMesh(this->mesh);
      textEditor = new QPlainTextEdit;

      QFile *file = new QFile(filename);
      file->open(QFile::ReadWrite | QFile::Text);

      QTextStream *textStream = new QTextStream(file);

      textEditor->setPlainText(textStream->readAll());
      textEditor->setFont(QFont("Courier New"));

      parent->mdiArea->addSubWindow(textEditor);
      textEditor->show();
      //gmodel->model();

    }


  if (type == "ft3d")
    {

      this->gmodel = new Gph::GTruss31(parent, name);
      QObject::connect(gmodel, SIGNAL(newRenderer(Gph::Renderer*)), parent, SLOT(addRendererWindow(Gph::Renderer*)));



      this->mesh = new Fea::Truss3D(filename);
      Fea::Truss3D *mesh_local = static_cast<Fea::Truss3D*>(this->mesh);

      gmodel->setFeaMesh(this->mesh);
      textEditor = new QPlainTextEdit;

      QFile *file = new QFile(filename);
      file->open(QFile::ReadWrite | QFile::Text);

      QTextStream *textStream = new QTextStream(file);

      textEditor->setPlainText(textStream->readAll());
      textEditor->setFont(QFont("Courier New"));

      parent->mdiArea->addSubWindow(textEditor);
      textEditor->show();

      Log::Msg::information("Truss31 model loaded");
      Log::Msg::result(QString(" %1 nodes and %2 elements").arg(mesh_local->nNodes).arg(mesh_local->nElements));
      //gmodel->model();
    }


  if (type == "ff2d")
    {

      this->gmodel = new Gph::GFrame22(parent, name);
      QObject::connect(gmodel, SIGNAL(newRenderer(Gph::Renderer*)), parent, SLOT(addRendererWindow(Gph::Renderer*)));



      this->mesh = new Fea::Frame22(filename);
      Fea::Frame22 *mesh_local = static_cast<Fea::Frame22*>(this->mesh);
      gmodel->setFeaMesh(this->mesh);
      textEditor = new QPlainTextEdit;



      QFile *file = new QFile(filename);
      file->open(QFile::ReadWrite | QFile::Text);

      QTextStream *textStream = new QTextStream(file);

      textEditor->setPlainText(textStream->readAll());
      textEditor->setFont(QFont("Courier New"));

      parent->mdiArea->addSubWindow(textEditor);
      textEditor->show();

      Log::Msg::information("Frame22 model loaded");
      Log::Msg::result(QString(" %1 nodes and %2 elements").arg(mesh_local->nNodes).arg(mesh_local->nElements));

      gmodel->model();
    }


  if (type == "fp2d")
    {

      this->gmodel = new Gph::GPlane22(parent, name);
      QObject::connect(gmodel, SIGNAL(newRenderer(Gph::Renderer*)), parent, SLOT(addRendererWindow(Gph::Renderer*)));

      this->mesh = new Fea::Plane22(filename);
      Fea::Plane22 *mesh_local = static_cast<Fea::Plane22*>(this->mesh);
      gmodel->setFeaMesh(this->mesh);
      textEditor = new QPlainTextEdit;



      QFile *file = new QFile(filename);
      file->open(QFile::ReadWrite | QFile::Text);

      QTextStream *textStream = new QTextStream(file);

      textEditor->setPlainText(textStream->readAll());
      textEditor->setFont(QFont("Courier New"));

      parent->mdiArea->addSubWindow(textEditor);
      textEditor->show();

      Log::Msg::information("Plane22 model loaded");
      Log::Msg::result(QString(" %1 nodes and %2 elements").arg(mesh_local->nNodes).arg(mesh_local->nElements));

      gmodel->model();
    }

}


void Model::save(void)
{
  QFile *file = new QFile(filename);
  file->open(QFile::ReadWrite | QFile::Truncate | QFile::Text);

  QTextStream *textStream = new QTextStream(file);
  (*textStream)<<textEditor->toPlainText();
  textStream->flush();
}

void Model::saveas(QString filename_)
{
  this->filename = filename_;

  QFile *file = new QFile(filename);
  file->open(QFile::ReadWrite | QFile::Truncate | QFile::Text);

  QTextStream *textStream = new QTextStream(file);
  (*textStream)<<textEditor->toPlainText();
  textStream->flush();
}


void Model::solve(bool isIterativeSolver_)
{
  this->save();



  QElapsedTimer timer;
  timer.start();



  mesh->evalStiffnessMatrix();
  mesh->solve(isIterativeSolver_);

  Log::Msg::information(QString("Total solver time was %1 s").arg(timer.elapsed()/1000.));


}


Model::~Model()
{
  textEditor->hide();
  if(mesh) delete mesh;
  if(gmodel) delete gmodel;
}

