/* $Id: shotGun.cpp, v 1.00 2007 / 2008 $
 *
 * shotGun.cpp
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

#include "shotGun.h"

shotGun::shotGun()
{
createMenu();
createBoxes();

QVBoxLayout *mainLayout = new QVBoxLayout;
mainLayout->setMenuBar(menuBar);
mainLayout->addWidget(shootBox);
mainLayout->addWidget(corruptBox);
setLayout(mainLayout);

setWindowTitle(tr("Shoot files!"));
}

void shotGun::createMenu()
{
menuBar = new QMenuBar;

fileMenu = new QMenu(tr("&File"), this);
loadAction = fileMenu->addAction(tr("&Load"));
exitAction = fileMenu->addAction(tr("E&xit"));
menuBar->addMenu(fileMenu);

connect(loadAction, SIGNAL(triggered()), this, SLOT(loadFile()));
connect(exitAction, SIGNAL(triggered()), this, SLOT(accept()));
}

void shotGun::createBoxes()
{
shootBox = new QGroupBox();
corruptBox = new QGroupBox();

QHBoxLayout *sLayout = new QHBoxLayout;
QHBoxLayout *cLayout = new QHBoxLayout;

QLabel* sSizeLabel = new QLabel("Size (1-512):");
sLayout->addWidget(sSizeLabel);
shootSize = new QSlider(Qt::Horizontal);
shootSize->setMinimum(1);
shootSize->setMaximum(512);
shootSize->setValue(1);
shootSize->setTickPosition(QSlider::TicksBothSides);
sLayout->addWidget(shootSize);

QLabel* sCountLabel = new QLabel("Count (1-100):");
sLayout->addWidget(sCountLabel);
shootCount = new QSlider(Qt::Horizontal);
shootCount->setMinimum(1);
shootCount->setMaximum(100);
shootCount->setValue(1);
shootCount->setTickPosition(QSlider::TicksBothSides);
sLayout->addWidget(shootCount);

shooter= new QPushButton(tr("Corrupt Bytes"));
sLayout->addWidget(shooter);
QObject::connect(shooter,SIGNAL(clicked()),this,SLOT(shootFile()));
shootBox->setLayout(sLayout);

QLabel* cSizeLabel = new QLabel("Size (1-512):");
cLayout->addWidget(cSizeLabel);
corruptSize = new QSlider(Qt::Horizontal);
corruptSize->setMinimum(1);
corruptSize->setMaximum(512);
corruptSize->setValue(1);
corruptSize->setTickPosition(QSlider::TicksBothSides);
cLayout->addWidget(corruptSize);

QLabel* cCountLabel = new QLabel("Count (1-100):");
cLayout->addWidget(cCountLabel);
corruptCount = new QSlider(Qt::Horizontal);
corruptCount->setMinimum(1);
corruptCount->setMaximum(100);
corruptCount->setValue(1);
corruptCount->setTickPosition(QSlider::TicksBothSides);
cLayout->addWidget(corruptCount);

corrupter= new QPushButton(tr("Drop Bytes"));
cLayout->addWidget(corrupter);
QObject::connect(corrupter,SIGNAL(clicked()),this,SLOT(corruptFile()));
corruptBox->setLayout(cLayout);
}

void shotGun::loadFile()
{
fileName = QFileDialog::getOpenFileName(this,
                "Select a file to damage",
                "clean",
                "*.*");
if (fileName.isEmpty() == false)
   fileName=fileName.section('/',-1);
}

void shotGun::shootFile()
{
int fileSize;

if (fileName.isEmpty()==true)
   QMessageBox::information(this,"Advice",
                               "Please select the file to be corrupted",
                               "OK");
else
   {
   if ((fileSize=connectFiles())<0) return;
   damageFiles(fileSize,true,shootSize->value(),shootCount->value()); 
   QMessageBox::information(this,"Result",
                            tr("%1 sequences of %2 bytes have been damaged").
                               arg(shootCount->value()).arg(shootSize->value()),
                               "OK");

   cleanFile.close();
   dirtyFile.close();
   }
}
   
void shotGun::damageFiles(
int                       fileSize,
bool                               replace,
int                                        useSize,
int                                                useCount)
{
char buffer[4];
char *map;
long step;
int shot;
int i, mapSize, byte, bit;
unsigned char bits[] = { 0200, 0100, 040, 020, 010, 04, 02, 01 };


mapSize = fileSize / 8 + 1;
map=new char[mapSize];
memset(map,0,mapSize);

for (i=0;i<useCount;i++)
    {
    step=rand() % (fileSize - useSize);
    byte = step / 8;
    bit = step % 8;
    for (shot=useSize;shot;shot--)
        {
        if (map[byte] & bits[bit]) break;
        if (++bit == 8)
           {
           bit=0;
           byte++;
           }
        }
    if (shot)
       {
       i--;
       continue;
       }
    byte = step / 8;
    bit = step % 8;
    for (shot=useSize;shot;shot--)
        {
        map[byte] |= bits[bit];
        if (++bit == 8)
           {
           bit=0;
           byte++;
           }
        }
    }

i=0;
while (!cleanFile.atEnd())
      {
      i++;
      byte = i / 8;
      bit = i % 8;
      cleanFile.getChar(buffer);
      if (map[byte] & bits[bit])
         {
         if (replace==true) dirtyFile.putChar(0);
         }
      else
         {
         dirtyFile.putChar(*buffer);
         }
      }

delete map;
}

int shotGun::connectFiles(void)
{
QString clean("clean/");
QString dirty("dirty/");

clean.append(fileName);
cleanFile.setFileName(clean);
cleanFile.open(QIODevice::ReadOnly);
if (cleanFile.isOpen()==false || cleanFile.isReadable()==false)
   {
   QMessageBox::warning(this,"Warning",
                            "This file cannot be read",
                            "OK");
   return -1;
   }
 
dirty.append(fileName);
dirtyFile.setFileName(dirty);
dirtyFile.open(QIODevice::WriteOnly);
if (dirtyFile.isOpen()==false || dirtyFile.isWritable()==false)
   {
   cleanFile.close();
   QMessageBox::warning(this,"Warning",
                            "This file cannot be written",
                            "OK");
   return -1;
   }

return cleanFile.size();
}

void shotGun::corruptFile()
{
int fileSize;
 
if (fileName.isEmpty()==true)
   QMessageBox::information(this,"Advice",
                               "Please select the file to be corrupted",
                               "OK");
else
   {
   if ((fileSize=connectFiles())<0) return;
   damageFiles(fileSize,false,corruptSize->value(),corruptCount->value()); 
   QMessageBox::information(this,"Result",
                            tr("%1 sequences of %2 bytes have been dropped").
                               arg(corruptCount->value()).arg(corruptSize->value()),
                               "OK");

   cleanFile.close();
   dirtyFile.close();
   }
}
