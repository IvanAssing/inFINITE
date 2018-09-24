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


#include "openmodelform.h"
#include "ui_openmodelform.h"

#include <QPushButton>

OpenModelForm::OpenModelForm(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::OpenModelForm)
{
  ui->setupUi(this);


  int size=100;
  int nx = 5;
  int ny = 10;


  QPushButton **button = new QPushButton*[nx*ny];
  for(int ii=0; ii<ny; ii++)
  for(int jj=0; jj<nx; jj++)
    {
      int i = ii*nx+jj;
      button[i] = new QPushButton(this);

      button[i]->setIcon(QIcon(QString(":/icons/inFinite_v%1.png").arg(1+(i%2))));
      button[i]->setIconSize(QSize(size, size));
      button[i]->setFixedSize(QSize(size, size));
      ui->gridLayout_2->addWidget(button[i],ii,jj);
    }


}

OpenModelForm::~OpenModelForm()
{
  delete ui;
}
