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

#include "thememanager.h"

ThemeManager::ThemeManager(QString defaultTheme)
{
    directory.setPath("themes");
    themeFiles = directory.entryList(QStringList() << "*.dp93ct", QDir::Files);
    setCurrentThemeFile(defaultTheme);

    for (int i = 0; i < themeFiles.size(); i++)
    {
        QStringList lines = fileThemeContent(themeFiles[i]);
        QRegularExpression re("(\\w+)\\s*=\\s*\"\\s*(.*)\\s*\"");
        QRegularExpressionMatch match = re.match(lines[0]);
        themeNames.append(match.captured(2));
        if (QString::compare(QString(defaultTheme), themeFiles[i]) == 0)
            themeSettings.append(Parameter(match.captured(1), match.captured(2)));
    }
}

void ThemeManager::setTheme(QString themeFile)
{
    updateThemeSettings(themeFile);

    for (int i = 0; i < 10; i++)
        static_cast<QPushButton*>(this->btnNumPointer[i])->setStyleSheet(getParameterValue("BUTTON_CENTRAL"));

    for (int i = 0; i < 3; i++)
        static_cast<QPushButton*>(this->btnTopPointer[i])->setStyleSheet(getParameterValue("BUTTON_TOP"));

    for (int i = 0; i < 5; i++)
        static_cast<QPushButton*>(this->btnActPointer[i])->setStyleSheet(getParameterValue("BUTTON_SIDE"));

    for (int i = 0; i < 2; i++)
        static_cast<QMenu*>(this->contextMenus[i])->setStyleSheet(getParameterValue("CONTEXT_MENU"));

    this->btnFloat->setStyleSheet(getParameterValue("BUTTON_CENTRAL"));
    this->calcDisplay->setStyleSheet(getParameterValue("NUMERIC_DISPLAY"));
    this->body->setStyleSheet(getParameterValue("BODY_STYLE"));
}

void ThemeManager::updateThemeSettings(QString themeFile)
{
    QStringList lines = fileThemeContent(themeFile);
    QRegularExpression re("(\\w+)\\s*=\\s*\"\\s*(.*)\\s*\"");
    themeSettings.clear();
    for (QString tLine : lines)
    {
        QRegularExpressionMatchIterator matches = re.globalMatch(tLine);

        while (matches.hasNext())
        {
            QRegularExpressionMatch match = matches.next();
            themeSettings.append(Parameter(match.captured(1), match.captured(2)));
        }
    }
}

int ThemeManager::countThemes()
{
    return themeFiles.size();
}

QString ThemeManager::currentThemeName()
{
    return themeSettings[0].getValue();
}

QString ThemeManager::currentThemeFile()
{
    return fileName;
}

void ThemeManager::setCurrentThemeFile(QString fileName)
{
    this->fileName = fileName;
}

QString ThemeManager::getThemeName(int index)
{
    return themeNames[index];
}

QString ThemeManager::getParameterValue(QString name)
{
    for (Parameter param : themeSettings)
    {
        if (QString::compare(QString(param.getVariable()), name) == 0)
            return param.getValue();
    }
}

QString ThemeManager::getFileName(int index)
{
    return themeFiles[index];
}

void ThemeManager::setUI(QWidget *body, void *btnNumPointer[], void *btnActPointer[], void *btnTopPointer[], void *contextMenus[], QPushButton *btnFloat, QLineEdit *calcDisplay)
{
    this->body = static_cast<QWidget*>(body);
    this->btnFloat = static_cast<QPushButton*>(btnFloat);
    this->calcDisplay = static_cast<QLineEdit*>(calcDisplay);

    for (int i = 0; i < (sizeof(this->btnNumPointer) / sizeof(int*)); i++)
        this->btnNumPointer[i] = btnNumPointer[i];

    for (int i = 0; i < (sizeof(this->btnActPointer) / sizeof(int*)); i++)
        this->btnActPointer[i] = btnActPointer[i];

    for (int i = 0; i < (sizeof(this->btnTopPointer) / sizeof(int*)); i++)
        this->btnTopPointer[i] = btnTopPointer[i];

    for (int i = 0; i < (sizeof(this->contextMenus) / sizeof(int*)); i++)
        this->contextMenus[i] = contextMenus[i];
}

QStringList ThemeManager::fileThemeContent(QString fileName)
{
    QFile file(directory.absolutePath() + "/" + fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QStringList();

    QTextStream in(&file);
    QString line = in.readLine();
    QStringList lines;
    while (!line.isNull()) {
        lines.append(line);
        line = in.readLine();
    }

    return lines;
}
