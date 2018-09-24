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


#ifndef MODEL_H
#define MODEL_H

#include <fea/mesh.h>
#include <gph/gmodel.h>
#include <QString>
#include <QPlainTextEdit>

class MainWindow;

class Model : public QObject
{
  Q_OBJECT

public:
  Model(QString filename, MainWindow *parent);
  MainWindow *parent;

  QString filename;
  QString name;
  QPlainTextEdit *textEditor;
  Fea::Mesh *mesh;
  Gph::GModel *gmodel;

  public slots:
  void update(void);
  void save(void);
  void saveas(QString filename);
  void solve(bool isIterativeSolver = false);

protected:
  ~Model();

};


#endif // MODEL_H
