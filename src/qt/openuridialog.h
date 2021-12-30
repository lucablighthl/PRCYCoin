<<<<<<< HEAD
// Copyright (c) 2011-2013 The Bitcoin developers
=======
// Copyright (c) 2019 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_OPENURIDIALOG_H
#define BITCOIN_QT_OPENURIDIALOG_H

#include <QDialog>
<<<<<<< HEAD
=======
#include "qt/pivx/snackbar.h"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

namespace Ui
{
class OpenURIDialog;
}

class OpenURIDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenURIDialog(QWidget* parent);
    ~OpenURIDialog();

    QString getURI();
<<<<<<< HEAD

protected Q_SLOTS:
    void accept();

private Q_SLOTS:
    void on_selectFileButton_clicked();

private:
    Ui::OpenURIDialog* ui;
=======
    void showEvent(QShowEvent *event) override;

protected Q_SLOTS:
    void accept() override;

private:
    Ui::OpenURIDialog* ui;
    SnackBar *snackBar = nullptr;
    void inform(const QString& str);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
};

#endif // BITCOIN_QT_OPENURIDIALOG_H
