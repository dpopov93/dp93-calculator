/*
* dp93-calculator - Calculating software
*
* This product includes software developed by the
* Denis Popov (https://github.com/dpopov93).
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General
* Public License along with this program. If not, see
* <https://github.com/dpopov93/dp93-calculator/blob/main/LICENSE>.
*/

#include "calcwindow.h"

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;

    switch (QLocale::system().language()) {
    case QLocale::English:
        translator.load("lang/dp93-calculator_en.qm", a.applicationDirPath());
        break;
    case QLocale::Russian:
        translator.load("lang/dp93-calculator_ru.qm", a.applicationDirPath());
        break;
    case QLocale::Chinese:
        translator.load("lang/dp93-calculator_cn.qm", a.applicationDirPath());
        break;
    default:
        translator.load("lang/dp93-calculator_en.qm", a.applicationDirPath());
        break;
    }

    a.installTranslator(&translator);

    CalcWindow w;
    w.show();
    return a.exec();
}
