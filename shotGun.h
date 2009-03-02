/* $Id: shotGun.h, v 1.00 2007 / 2008 $
 *
 * shotGun.h
 * This module is part of the "shotGun" written for teaching
 * purposes within the Delos and Planets projects
 *
 * Written by Manfred Thaller <manfred.thaller@uni-koeln.de> 2007 - 2008
 *
 * Copyright (C)  2007 - 2008 Manfred Thaller
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef shotGun_H
#define shotGun_H

#include <QtGui>

class QAction;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QMenuBar;
class QPushButton;
class QTextEdit;

class shotGun : public QDialog
{
   Q_OBJECT

public:
   shotGun();

private:
   void createMenu();
   void createBoxes();

   QMenuBar *menuBar;
   QGroupBox *shootBox;
   QPushButton *shooter;
   QSlider *shootSize;
   QSlider *shootCount;
   QGroupBox *corruptBox;
   QPushButton *corrupter;
   QSlider *corruptSize;
   QSlider *corruptCount;

   QMenu *fileMenu;
   QAction *exitAction;
   QAction *loadAction;

   QFile cleanFile;
   QFile dirtyFile;
   QString fileName;

   int connectFiles(void);
   void damageFiles(int fileSize,bool replace,int useSize,int useCount);

private slots:
   void loadFile(void);
   void shootFile(void);
   void corruptFile(void);
} ;

#endif

