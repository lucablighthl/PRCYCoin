// Copyright (c) 2011-2014 The Bitcoin developers
<<<<<<< HEAD
=======
// Copyright (c) 2017-2019 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_UTILITYDIALOG_H
#define BITCOIN_QT_UTILITYDIALOG_H

#include <QDialog>
#include <QObject>
<<<<<<< HEAD

class BitcoinGUI;
=======
#include <QMainWindow>

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
class ClientModel;

namespace Ui
{
class HelpMessageDialog;
}

/** "Help message" dialog box */
class HelpMessageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HelpMessageDialog(QWidget* parent, bool about);
    ~HelpMessageDialog();

    void printToConsole();
    void showOrPrint();

private:
    Ui::HelpMessageDialog* ui;
    QString text;
<<<<<<< HEAD

private Q_SLOTS:
    void on_okButton_accepted();
=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
};


/** "Shutdown" window */
class ShutdownWindow : public QWidget
{
    Q_OBJECT

public:
<<<<<<< HEAD
    ShutdownWindow(QWidget* parent = 0, Qt::WindowFlags f = 0);
    static void showShutdownWindow(BitcoinGUI* window);
=======
    ShutdownWindow(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::Widget);
    static void showShutdownWindow(QMainWindow* window);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

protected:
    void closeEvent(QCloseEvent* event);
};

#endif // BITCOIN_QT_UTILITYDIALOG_H
