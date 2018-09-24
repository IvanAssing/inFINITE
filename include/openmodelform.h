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


#ifndef OPENMODELFORM_H
#define OPENMODELFORM_H

#include <QWidget>

namespace Ui {
  class OpenModelForm;
}

class OpenModelForm : public QWidget
{
  Q_OBJECT

public:
  explicit OpenModelForm(QWidget *parent = 0);
  ~OpenModelForm();

private:
  Ui::OpenModelForm *ui;
};

#endif // OPENMODELFORM_H
