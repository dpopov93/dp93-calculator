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

#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QString>
#include <QRegularExpression>
#include <QPushButton>
#include <QMenu>
#include <QLineEdit>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include "parameter.h"

class ThemeManager
{

public:
    ThemeManager(QString defaultTheme);
    void setTheme(QString themeFile);
    void updateThemeSettings(QString themeFile);
    int countThemes();
    QString currentThemeName();
    QString currentThemeFile();
    void setCurrentThemeFile(QString fileName);
    QString getThemeName(int index);
    QString getParameterValue(QString name);
    QString getFileName(int index);
    void setUI(QWidget *body, void *btnNumPointer[], void *btnActPointer[],
               void *btnTopPointer[], void *contextMenus[], QPushButton *btnFloat, QLineEdit *calcDisplay);

private:
    QDir directory;
    QStringList themeFiles;
    QString fileName;
    QStringList themeNames;
    QList<Parameter> themeSettings;
    QStringList fileThemeContent(QString fileName);

    QWidget *body;
    QPushButton *btnFloat;
    QLineEdit *calcDisplay;
    void *btnNumPointer[10];
    void *btnActPointer[5];
    void *btnTopPointer[3];
    void *contextMenus[2];
};

#endif // THEMEMANAGER_H
