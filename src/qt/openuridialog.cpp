<<<<<<< HEAD
// Copyright (c) 2011-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
=======
// Copyright (c) 2019 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "openuridialog.h"
#include "ui_openuridialog.h"

#include "guiutil.h"
#include "walletmodel.h"
<<<<<<< HEAD

#include <QUrl>
=======
#include "qt/pivx/qtutils.h"

#include <QUrl>
#include <QFile>
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

OpenURIDialog::OpenURIDialog(QWidget* parent) : QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
                                                ui(new Ui::OpenURIDialog)
{
    ui->setupUi(this);
<<<<<<< HEAD
    ui->uriEdit->setPlaceholderText("prcycoin:");
=======
    this->setStyleSheet(parent->styleSheet());
    ui->uriEdit->setPlaceholderText("pivx:");

    ui->labelSubtitle->setText("URI");
    setCssProperty(ui->labelSubtitle, "text-title2-dialog");
    setCssProperty(ui->frame, "container-dialog");
    setCssProperty(ui->labelTitle, "text-title-dialog");

    setCssBtnPrimary(ui->pushButtonOK);
    setCssProperty(ui->pushButtonCancel, "btn-dialog-cancel");

    initCssEditLine(ui->uriEdit, true);
    connect(ui->pushButtonOK, &QPushButton::clicked, this, &OpenURIDialog::accept);
    connect(ui->pushButtonCancel, &QPushButton::clicked, this, &OpenURIDialog::close);
}

void OpenURIDialog::showEvent(QShowEvent *event)
{
    ui->uriEdit->setFocus();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

OpenURIDialog::~OpenURIDialog()
{
    delete ui;
}

QString OpenURIDialog::getURI()
{
    return ui->uriEdit->text();
}

void OpenURIDialog::accept()
{
    SendCoinsRecipient rcp;
    if (GUIUtil::parseBitcoinURI(getURI(), &rcp)) {
        /* Only accept value URIs */
        QDialog::accept();
    } else {
<<<<<<< HEAD
        ui->uriEdit->setValid(false);
    }
}

void OpenURIDialog::on_selectFileButton_clicked()
{
    QString filename = GUIUtil::getOpenFileName(this, tr("Select payment request file to open"), "", "", NULL);
    if (filename.isEmpty())
        return;
    QUrl fileUri = QUrl::fromLocalFile(filename);
    ui->uriEdit->setText("prcycoin:?r=" + QUrl::toPercentEncoding(fileUri.toString()));
}
=======
        setCssEditLineDialog(ui->uriEdit, false, true);
    }
}

void OpenURIDialog::inform(const QString& str) {
    if (!snackBar) snackBar = new SnackBar(nullptr, this);
    snackBar->setText(str);
    snackBar->resize(this->width(), snackBar->height());
    openDialog(snackBar, this);
}
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
