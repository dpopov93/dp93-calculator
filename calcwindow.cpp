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
#include "ui_calcwindow.h"

CalcWindow::CalcWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CalcWindow)
{
    ui->setupUi(this);
    ui->calcDisplay->installEventFilter(this);

    void *btnNumPointer[] = {ui->btnNum0, ui->btnNum1,
                             ui->btnNum2, ui->btnNum3,
                             ui->btnNum4, ui->btnNum5,
                             ui->btnNum6, ui->btnNum7,
                             ui->btnNum8, ui->btnNum9
                            };

    void *btnActPointer[] = {ui->btnPlus, ui->btnMinus,
                             ui->btnMulti, ui->btnDiv,
                             ui->btnEqual
                            };

    void *btnTopPointer[] = {ui->btnAC, ui->btnNeg,
                             ui->btnPerc
                            };

    displayMenu = ui->calcDisplay->createStandardContextMenu();
    displayMenu->clear();
    actCopy = displayMenu->addAction(tr("Copy"));
    actInsert = displayMenu->addAction(tr("Insert"));
    actClear = displayMenu->addAction(tr("Clear"));
    themeMenu = new QMenu(tr("Themes"));

    void *contextMenus[] = {displayMenu, themeMenu};

    thmManager = new ThemeManager("oceanic.dp93ct");
    thmManager->setUI(ui->body, btnNumPointer, btnActPointer, btnTopPointer, contextMenus,
                     ui->btnFloat, ui->calcDisplay);

    for (int i = 0; i < thmManager->countThemes(); i++)
    {
        themeMenu->addAction(thmManager->getThemeName(i), [this, i] {thmManager->setTheme(thmManager->getFileName(i));});
    }
    displayMenu->addMenu(themeMenu);

    inputComplete = true;
    inputFloat = false;
    numInMemory = false;
    equalsPressed = false;
    memoryEquals = 0.0;
    memory = 0.0;

    for (int i = 0; i < (sizeof(btnNumPointer) / sizeof(int*)); i++)
    {
        this->btnNumPointer[i] = btnNumPointer[i];
        QPushButton *btnNum = static_cast<QPushButton*>(btnNumPointer[i]);
        connect(btnNum, SIGNAL(clicked()), this, SLOT(on_btnNum_clicked()));
    }

    for (int i = 0; i < (sizeof(btnActPointer) / sizeof(int*)); i++)
    {
        QPushButton *btnAct = static_cast<QPushButton*>(btnActPointer[i]);
        connect(btnAct, SIGNAL(clicked()), this, SLOT(on_btnActions_clicked()));
    }

    connect(ui->btnFloat, SIGNAL(clicked()), this, SLOT(on_btnFloat_clicked()));
    connect(ui->btnNeg, SIGNAL(clicked()), this, SLOT(on_btnNegative_clicked()));
    connect(ui->btnPerc, SIGNAL(clicked()), this, SLOT(on_btnPercentage_clicked()));
    connect(ui->btnAC, SIGNAL(clicked()), this, SLOT(on_btnReset_clicked()));
    connect(actCopy,  SIGNAL(triggered()), this, SLOT(on_actionCopy_clicked()));
    connect(actInsert, SIGNAL(triggered()), this, SLOT(on_actionInsert_clicked()));
    connect(actClear, SIGNAL(triggered()), this, SLOT(on_btnReset_clicked()));

    // TODO: Settings manager for storing result and selected theme
    thmManager->setTheme(thmManager->currentThemeFile());
}

CalcWindow::~CalcWindow()
{
    delete ui;
}

void CalcWindow::setDisplayValue(double value)
{
    setDisplayValue(QString::number(value));
}

void CalcWindow::setDisplayValue(QString value)
{
    ui->calcDisplay->setText(value);
}

QString CalcWindow::getDisplayValue()
{
    return ui->calcDisplay->text();
}

QString CalcWindow::getSenderButtonText(QObject *sndr)
{
    QPushButton *btn = (QPushButton *)sndr;
    return btn->text();
}

void CalcWindow::setDisplayFontSize(int fontSize)
{
    ui->calcDisplay->setStyleSheet(
                QString(ui->calcDisplay->styleSheet()
                    + "QLineEdit {font-size: %1px;}").arg(fontSize));
}

bool CalcWindow::eventFilter(QObject *object, QEvent *event)
{
    if(object == ui->calcDisplay && event->type() == QEvent::MouseButtonRelease)
        clearLastNum();

        return false;
}

void CalcWindow::on_calcDisplay_textChanged(const QString &arg1)
{
    if (ui->calcDisplay->text().size() < 12)
        setDisplayFontSize(50);
    else if (ui->calcDisplay->text().size() >= 12)
        setDisplayFontSize(50 - (ui->calcDisplay->text().size()));
}

void CalcWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Backspace)
        clearLastNum();

    if (event->key() >= 0x30 && event->key() <= 0x39)
        static_cast<QPushButton*>(btnNumPointer[event->key() - 0x30])->animateClick();

    if (event->modifiers() == Qt::ControlModifier)
        if (event->key() == Qt::Key_C)
        {
            on_actionCopy_clicked();
            return;
        }
        else if (event->key() == Qt::Key_V)
        {
            on_actionInsert_clicked();
            return;
        }
        else if (event->key() == Qt::Key_Q)
            this->close();

    switch(event->key())
    {
    case Qt::Key_Plus:
        ui->btnPlus->animateClick();
        break;
    case Qt::Key_Minus:
        ui->btnMinus->animateClick();
        break;
    case Qt::Key_Asterisk:
        ui->btnMulti->animateClick();
        break;
    case Qt::Key_Slash:
        ui->btnDiv->animateClick();
        break;
    case Qt::Key_Period:
    case Qt::Key_Comma:
        ui->btnFloat->animateClick();
        break;
    case Qt::Key_Percent:
        ui->btnPerc->animateClick();
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
    case Qt::Key_Equal:
        ui->btnEqual->animateClick();
        break;
    case Qt::Key_Escape:
        ui->btnAC->animateClick();
        break;
    }
}

void CalcWindow::contextMenuEvent(QContextMenuEvent *event)
{
    displayMenu->exec(event->globalPos());
}

CalcWindow::ProcessingActions CalcWindow::fromStringToPA(QString str)
{
    if (str == "+")
        return PLUS;
    else if (str == "-")
        return MINUS;
    else if (str == "*")
        return MULTIPLY;
    else if (str == "/")
        return DIVISION;
    else if (str == "=")
        return EQUAL;
}

void CalcWindow::on_btnNum_clicked()
{
    ui->btnAC->setText("C");

    if (inputComplete)
    {
        if (getSenderButtonText(sender()) != "0")
            inputComplete = false;
        setDisplayValue(getSenderButtonText(sender()));
    }
    else
    {
        if (getSenderButtonText(sender()) == "0" && getDisplayValue() == "0")
            return;

        if (getDisplayValue() == "0")
        {
            setDisplayValue(getSenderButtonText(sender()));
            return;
        }

        setDisplayValue(getDisplayValue() + getSenderButtonText(sender()));
    }
}

void CalcWindow::on_btnFloat_clicked()
{
    ui->btnAC->setText("C");

    if (!inputFloat)
    {
        if (inputComplete)
        {
            setDisplayValue("0.");
        }
        else
        {
            setDisplayValue(getDisplayValue() + ".");
        }

        inputFloat = true;
        inputComplete = false;
    }
}

void CalcWindow::on_btnNegative_clicked()
{
    if (getDisplayValue().startsWith("-"))
        setDisplayValue(getDisplayValue().replace("-", ""));
    else
        setDisplayValue("-" + getDisplayValue());
}

void CalcWindow::on_btnPercentage_clicked()
{
    if (currentAction == PLUS
            || currentAction == MINUS)
        setDisplayValue(getDisplayValue().toDouble() * 0.01 * memory);
    else
        setDisplayValue(getDisplayValue().toDouble() * 0.01);
}

void CalcWindow::on_btnReset_clicked()
{
    setDisplayValue("0");
    memory = 0;
    equalsPressed = false;
    memoryEquals = 0.0;
    numInMemory = false;
    inputComplete = true;
    inputFloat = false;
    ui->btnAC->setText("AC");
}

void CalcWindow::on_btnActions_clicked()
{
    pActions = fromStringToPA(getSenderButtonText(sender()));

    if (!equalsPressed)
        memoryEquals = getDisplayValue().toDouble();

    if (numInMemory)
    {
        switch (currentAction) {
        case PLUS:
            if (equalsPressed)
                memory = getDisplayValue().toDouble() + memoryEquals;
            else
                memory += getDisplayValue().toDouble();
            break;
        case MINUS:
            if (equalsPressed)
                memory = getDisplayValue().toDouble() - memoryEquals;
            else
                memory -= getDisplayValue().toDouble();
            break;
        case MULTIPLY:
            if (equalsPressed)
                memory = getDisplayValue().toDouble() * memoryEquals;
            else
                memory *= getDisplayValue().toDouble();
            break;
        case DIVISION:
            if (memory == 0.0
                    || getDisplayValue().toDouble() == 0.0
                    || memoryEquals == 0.0)
            {
                memory = 0;
                inputComplete = true;
                inputFloat = false;
                setDisplayValue(tr("Error"));
                return;
            }
            else
                if (equalsPressed)
                    memory = getDisplayValue().toDouble() / memoryEquals;
                else
                    memory /= getDisplayValue().toDouble();
            break;
        }
    }
    else
    {
        if (getDisplayValue() != "0")
            ui->btnAC->setText("C");
        memory = getDisplayValue().toDouble();
        numInMemory = true;
    }

    inputComplete = true;
    inputFloat = false;

    setDisplayValue(memory);

    if (pActions != EQUAL)
    {
        currentAction = pActions;
        equalsPressed = false;
    }
    else
    {
        equalsPressed = true;
    }
}

void CalcWindow::on_actionCopy_clicked()
{
    if (QClipboard* clp = QApplication::clipboard())
        clp->setText(getDisplayValue());
}

void CalcWindow::on_actionInsert_clicked()
{
    QClipboard* clp = QApplication::clipboard();

    if (clp->text().isEmpty() || !QRegExp("\\d*").exactMatch(clp->text()))
        return;

    if (inputComplete)
        inputComplete = false;

    setDisplayValue(clp->text());
}

void CalcWindow::clearLastNum()
{
    if (!inputComplete && getDisplayValue() != "0" && getDisplayValue().size() > 0)
    {
        if (getDisplayValue().size() == 1)
        {
            setDisplayValue("0");
            inputComplete = true;
        }
        else
        {
            if (getDisplayValue().back() == ".")
                inputFloat = false;

            setDisplayValue(getDisplayValue().left(getDisplayValue().size() - 1));
        }
    }
}
