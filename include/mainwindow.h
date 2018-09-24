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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>
#include <gph/renderer.h>

#include <QMap>
#include <QList>

#include "model.h"


namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  QMdiArea *mdiArea;
  QToolBar *modelsToolBar;
  QComboBox *modelsComboBox;

  int iCurrentModel;

  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private:
  Ui::MainWindow *ui;
  Gph::Renderer *currentRenderer;
  QMap<QMdiSubWindow*, Gph::Renderer*> *rendererMap;
  QList<Model*> models;

public slots:
  void setCurrentRenderer(QMdiSubWindow *window);
  void addRendererWindow(Gph::Renderer* renderer);
  void closeRendererWindow(Gph::Renderer* renderer);

  void setCurrentModel(int index);
  void closeModel(void);
  void updateOpenedModelsList(void);

  void openFile(void);
  void saveFile(void);
  void saveFileAs(void);
  void solverIteractive(void);
  void solverDirect(void);
  void openDXFFile(void);
};

#endif // MAINWINDOW_H
