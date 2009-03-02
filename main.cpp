/* $Id: main.cpp, v 1.00 2007 / 2008 $
 *
 * main.cpp
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

#include <QApplication>
#include "shotGun.h"

int main(int argc, char *argv[])
{
QApplication app(argc, argv);
QApplication::font().setBold(true);
QApplication::font().setPointSize(20);
shotGun shotgun;
return shotgun.exec();
}
