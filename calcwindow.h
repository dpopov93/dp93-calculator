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

#ifndef CALCWINDOW_H
#define CALCWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QObject>

QT_BEGIN_NAMESPACE
namespace Ui { class CalcWindow; }
QT_END_NAMESPACE

class CalcWindow : public QMainWindow
{
    Q_OBJECT

public:
    CalcWindow(QWidget *parent = nullptr);
    ~CalcWindow();

    void setDisplayValue(double value);
    void setDisplayValue(QString value);
    QString getDisplayValue();

private slots:
    void on_btnNum_clicked();
    void on_btnFloat_clicked();
    void on_btnNegative_clicked();
    void on_btnPercentage_clicked();
    void on_btnReset_clicked();
    void on_btnActions_clicked();

    void on_calcDisplay_textChanged(const QString &arg1);

private:
    Ui::CalcWindow *ui;

    QString getSenderButtonText(QObject *sndr);
    void setDisplayFontSize(int fontSize);

    bool inputComplete;
    bool inputFloat;
    bool numInMemory;
    bool equalsPressed;
    double memoryEquals;
    double memory;
    enum ProcessingActions{NONE=0, PLUS, MINUS, MULTIPLY, DIVISION, EQUAL};
    ProcessingActions pActions;
    ProcessingActions currentAction;
    ProcessingActions fromStringToPA(QString str);

};
#endif // CALCWINDOW_H
