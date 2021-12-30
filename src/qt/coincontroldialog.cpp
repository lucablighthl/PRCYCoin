// Copyright (c) 2011-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
<<<<<<< HEAD
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
=======
// Copyright (c) 2015-2020 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "coincontroldialog.h"
#include "ui_coincontroldialog.h"

#include "addresstablemodel.h"
#include "bitcoinunits.h"
<<<<<<< HEAD
#include "guiutil.h"
#include "init.h"
#include "optionsmodel.h"
#include "walletmodel.h"

#include "coincontrol.h"
#include "main.h"
#include "obfuscation.h"
#include "wallet/wallet.h"

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
=======
#include "coincontrol.h"
#include "guiutil.h"
#include "optionsmodel.h"
#include "policy/policy.h"
#include "txmempool.h"
#include "wallet/fees.h"
#include "wallet/wallet.h"
#include "walletmodel.h"

#include "qt/pivx/qtutils.h"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

#include <QApplication>
#include <QCheckBox>
#include <QCursor>
#include <QDialogButtonBox>
#include <QFlags>
#include <QIcon>
#include <QSettings>
<<<<<<< HEAD
#include <QString>
#include <QTreeWidget>

using namespace std;
QList<CAmount> CoinControlDialog::payAmounts;
int CoinControlDialog::nSplitBlockDummy;
CCoinControl* CoinControlDialog::coinControl = new CCoinControl();
=======
#include <QTreeWidget>

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

bool CCoinControlWidgetItem::operator<(const QTreeWidgetItem &other) const {
    int column = treeWidget()->sortColumn();
    if (column == CoinControlDialog::COLUMN_AMOUNT || column == CoinControlDialog::COLUMN_DATE || column == CoinControlDialog::COLUMN_CONFIRMATIONS)
        return data(column, Qt::UserRole).toLongLong() < other.data(column, Qt::UserRole).toLongLong();
    return QTreeWidgetItem::operator<(other);
}

<<<<<<< HEAD
CoinControlDialog::CoinControlDialog(QWidget* parent) : QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
                                                        ui(new Ui::CoinControlDialog),
                                                        model(0)
{
=======

CoinControlDialog::CoinControlDialog(QWidget* parent, bool _forDelegation) : QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
                                                        ui(new Ui::CoinControlDialog),
                                                        model(0),
                                                        forDelegation(_forDelegation)
{
    coinControl = new CCoinControl();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    ui->setupUi(this);

    /* Open CSS when configured */
    this->setStyleSheet(GUIUtil::loadStyleSheet());
<<<<<<< HEAD
=======
    setCssProperty({ui->frameContainer,
                    ui->layoutAmount,
                    ui->layoutAfter,
                    ui->layoutBytes,
                    ui->layoutChange,
                    ui->layoutDust,
                    ui->layoutFee,
                    ui->layoutQuantity
                    }, "container-border-purple");

    // Title
    ui->labelTitle->setProperty("cssClass", "text-title-dialog");

    // Label Style
    setCssProperty({ui->labelCoinControlAfterFeeText,
                    ui->labelCoinControlAmountText,
                    ui->labelCoinControlBytesText,
                    ui->labelCoinControlChangeText,
                    ui->labelCoinControlLowOutputText,
                    ui->labelCoinControlFeeText,
                    ui->labelCoinControlQuantityText
                    }, "text-main-purple");

    // Value Style
    setCssProperty({ui->labelCoinControlAfterFee,
                    ui->labelCoinControlAmount,
                    ui->labelCoinControlBytes,
                    ui->labelCoinControlChange,
                    ui->labelCoinControlLowOutput,
                    ui->labelCoinControlFee,
                    ui->labelCoinControlQuantity
                    }, "text-main-purple");

    ui->groupBox_2->setProperty("cssClass", "group-box");
    ui->treeWidget->setProperty("cssClass", "table-tree");
    ui->labelLocked->setProperty("cssClass", "text-main-purple");

    // Buttons
    setCssProperty({ui->pushButtonSelectAll, ui->pushButtonToggleLock}, "btn-check");
    ui->pushButtonOk->setProperty("cssClass", "btn-primary");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    // context menu actions
    QAction* copyAddressAction = new QAction(tr("Copy address"), this);
    QAction* copyLabelAction = new QAction(tr("Copy label"), this);
    QAction* copyAmountAction = new QAction(tr("Copy amount"), this);
    copyTransactionHashAction = new QAction(tr("Copy transaction ID"), this); // we need to enable/disable this
    lockAction = new QAction(tr("Lock unspent"), this);                       // we need to enable/disable this
    unlockAction = new QAction(tr("Unlock unspent"), this);                   // we need to enable/disable this

    // context menu
    contextMenu = new QMenu();
    contextMenu->addAction(copyAddressAction);
    contextMenu->addAction(copyLabelAction);
    contextMenu->addAction(copyAmountAction);
    contextMenu->addAction(copyTransactionHashAction);
    contextMenu->addSeparator();
    contextMenu->addAction(lockAction);
    contextMenu->addAction(unlockAction);

    // context menu signals
<<<<<<< HEAD
    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showMenu(QPoint)));
    connect(copyAddressAction, SIGNAL(triggered()), this, SLOT(copyAddress()));
    connect(copyLabelAction, SIGNAL(triggered()), this, SLOT(copyLabel()));
    connect(copyAmountAction, SIGNAL(triggered()), this, SLOT(copyAmount()));
    connect(copyTransactionHashAction, SIGNAL(triggered()), this, SLOT(copyTransactionHash()));
    connect(lockAction, SIGNAL(triggered()), this, SLOT(lockCoin()));
    connect(unlockAction, SIGNAL(triggered()), this, SLOT(unlockCoin()));

    // clipboard actions
    QAction* clipboardQuantityAction = new QAction(tr("Copy quantity"), this);
    QAction* clipboardAmountAction = new QAction(tr("Copy amount"), this);
    QAction* clipboardFeeAction = new QAction(tr("Copy fee"), this);
    QAction* clipboardAfterFeeAction = new QAction(tr("Copy after fee"), this);
    QAction* clipboardBytesAction = new QAction(tr("Copy bytes"), this);
    QAction* clipboardPriorityAction = new QAction(tr("Copy priority"), this);
    QAction* clipboardLowOutputAction = new QAction(tr("Copy dust"), this);
    QAction* clipboardChangeAction = new QAction(tr("Copy change"), this);

    connect(clipboardQuantityAction, SIGNAL(triggered()), this, SLOT(clipboardQuantity()));
    connect(clipboardAmountAction, SIGNAL(triggered()), this, SLOT(clipboardAmount()));
    connect(clipboardFeeAction, SIGNAL(triggered()), this, SLOT(clipboardFee()));
    connect(clipboardAfterFeeAction, SIGNAL(triggered()), this, SLOT(clipboardAfterFee()));
    connect(clipboardBytesAction, SIGNAL(triggered()), this, SLOT(clipboardBytes()));
    connect(clipboardPriorityAction, SIGNAL(triggered()), this, SLOT(clipboardPriority()));
    connect(clipboardLowOutputAction, SIGNAL(triggered()), this, SLOT(clipboardLowOutput()));
    connect(clipboardChangeAction, SIGNAL(triggered()), this, SLOT(clipboardChange()));

    ui->labelCoinControlQuantity->addAction(clipboardQuantityAction);
    ui->labelCoinControlAmount->addAction(clipboardAmountAction);
    ui->labelCoinControlFee->addAction(clipboardFeeAction);
    ui->labelCoinControlAfterFee->addAction(clipboardAfterFeeAction);
    ui->labelCoinControlBytes->addAction(clipboardBytesAction);
    ui->labelCoinControlPriority->addAction(clipboardPriorityAction);
    ui->labelCoinControlLowOutput->addAction(clipboardLowOutputAction);
    ui->labelCoinControlChange->addAction(clipboardChangeAction);

    // toggle tree/list mode
    connect(ui->radioTreeMode, SIGNAL(toggled(bool)), this, SLOT(radioTreeMode(bool)));
    connect(ui->radioListMode, SIGNAL(toggled(bool)), this, SLOT(radioListMode(bool)));

    // click on checkbox
    connect(ui->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(viewItemChanged(QTreeWidgetItem*, int)));

    // click on header
    ui->treeWidget->header()->setSectionsClickable(true);
    connect(ui->treeWidget->header(), SIGNAL(sectionClicked(int)), this, SLOT(headerSectionClicked(int)));

    // ok button
    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonBoxClicked(QAbstractButton*)));

    // (un)select all
    connect(ui->pushButtonSelectAll, SIGNAL(clicked()), this, SLOT(buttonSelectAllClicked()));

    // Toggle lock state
    connect(ui->pushButtonToggleLock, SIGNAL(clicked()), this, SLOT(buttonToggleLockClicked()));

    // change coin control first column label due Qt4 bug.
    // see https://github.com/bitcoin/bitcoin/issues/5716
    ui->treeWidget->headerItem()->setText(COLUMN_CHECKBOX, QString());

    ui->treeWidget->setColumnWidth(COLUMN_CHECKBOX, 84);
    ui->treeWidget->setColumnWidth(COLUMN_AMOUNT, 100);
    ui->treeWidget->setColumnWidth(COLUMN_LABEL, 170);
    ui->treeWidget->setColumnWidth(COLUMN_ADDRESS, 190);
    ui->treeWidget->setColumnWidth(COLUMN_DATE, 80);
    ui->treeWidget->setColumnWidth(COLUMN_CONFIRMATIONS, 100);
    ui->treeWidget->setColumnWidth(COLUMN_PRIORITY, 100);
    ui->treeWidget->setColumnHidden(COLUMN_TXHASH, true);         // store transacton hash in this column, but dont show it
    ui->treeWidget->setColumnHidden(COLUMN_VOUT_INDEX, true);     // store vout index in this column, but dont show it

=======
    connect(ui->treeWidget, &QWidget::customContextMenuRequested, this, &CoinControlDialog::showMenu);
    connect(copyAddressAction, &QAction::triggered, this, &CoinControlDialog::copyAddress);
    connect(copyLabelAction, &QAction::triggered, this, &CoinControlDialog::copyLabel);
    connect(copyAmountAction, &QAction::triggered, this, &CoinControlDialog::copyAmount);
    connect(copyTransactionHashAction, &QAction::triggered, this, &CoinControlDialog::copyTransactionHash);
    connect(lockAction, &QAction::triggered, this, &CoinControlDialog::lockCoin);
    connect(unlockAction, &QAction::triggered, this, &CoinControlDialog::unlockCoin);

    // clipboard actions
    setCssProperty({
        ui->pushButtonAmount,
        ui->pushButtonQuantity,
        ui->pushButtonFee,
        ui->pushButtonAlterFee,
        ui->pushButtonBytes,
        ui->pushButtonChange,
        ui->pushButtonDust
        }, "ic-copy-big"
    );

    connect(ui->pushButtonQuantity, &QPushButton::clicked, this, &CoinControlDialog::clipboardQuantity);
    connect(ui->pushButtonAmount, &QPushButton::clicked, this, &CoinControlDialog::clipboardAmount);
    connect(ui->pushButtonFee, &QPushButton::clicked, this, &CoinControlDialog::clipboardFee);
    connect(ui->pushButtonAlterFee, &QPushButton::clicked, this, &CoinControlDialog::clipboardAfterFee);
    connect(ui->pushButtonBytes, &QPushButton::clicked, this, &CoinControlDialog::clipboardBytes);
    connect(ui->pushButtonDust, &QPushButton::clicked, this, &CoinControlDialog::clipboardLowOutput);
    connect(ui->pushButtonChange, &QPushButton::clicked, this, &CoinControlDialog::clipboardChange);

    if (ui->pushButtonSelectAll->isChecked()) {
        ui->pushButtonSelectAll->setText(tr("Unselect all"));
    } else {
        ui->pushButtonSelectAll->setText(tr("Select all"));
    }

    // toggle tree/list mode
    connect(ui->radioTreeMode, &QRadioButton::toggled, this, &CoinControlDialog::radioTreeMode);
    connect(ui->radioListMode, &QRadioButton::toggled, this, &CoinControlDialog::radioListMode);

    // click on checkbox
    connect(ui->treeWidget, &QTreeWidget::itemChanged, this, &CoinControlDialog::viewItemChanged);

    // click on header
    ui->treeWidget->header()->setSectionsClickable(true);
    connect(ui->treeWidget->header(), &QHeaderView::sectionClicked, this, &CoinControlDialog::headerSectionClicked);

    // ok button
    connect(ui->pushButtonOk, &QPushButton::clicked, this, &CoinControlDialog::accept);

    // (un)select all
    connect(ui->pushButtonSelectAll, &QPushButton::clicked, this, &CoinControlDialog::buttonSelectAllClicked);

    // Toggle lock state
    connect(ui->pushButtonToggleLock, &QPushButton::clicked, this, &CoinControlDialog::buttonToggleLockClicked);

    ui->treeWidget->setColumnWidth(COLUMN_CHECKBOX, colCheckBoxWidth_treeMode);
    ui->treeWidget->setColumnWidth(COLUMN_AMOUNT, 110);
    ui->treeWidget->setColumnWidth(COLUMN_LABEL, 160);
    ui->treeWidget->setColumnWidth(COLUMN_ADDRESS, 310);
    ui->treeWidget->setColumnWidth(COLUMN_DATE, 145);
    ui->treeWidget->setColumnWidth(COLUMN_CONFIRMATIONS, 65);
    ui->treeWidget->setColumnHidden(COLUMN_TXHASH, true);         // store transacton hash in this column, but dont show it
    ui->treeWidget->setColumnHidden(COLUMN_VOUT_INDEX, true);     // store vout index in this column, but dont show it

    ui->treeWidget->header()->setStretchLastSection(true);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    // default view is sorted by amount desc
    sortView(COLUMN_AMOUNT, Qt::DescendingOrder);

    // restore list mode and sortorder as a convenience feature
    QSettings settings;
<<<<<<< HEAD
    if (settings.contains("nCoinControlMode") && !settings.value("nCoinControlMode").toBool())
        ui->radioTreeMode->click();
    if (settings.contains("nCoinControlSortColumn") && settings.contains("nCoinControlSortOrder"))
        sortView(settings.value("nCoinControlSortColumn").toInt(), ((Qt::SortOrder)settings.value("nCoinControlSortOrder").toInt()));
=======
    if (settings.contains("nCoinControlMode") && !settings.value("nCoinControlMode").toBool()) {
        ui->radioTreeMode->setChecked(true);
        ui->treeWidget->setRootIsDecorated(true);
        ui->radioTreeMode->click();
    }else{
        ui->radioListMode->setChecked(true);
        ui->treeWidget->setRootIsDecorated(false);
    }
    if (settings.contains("nCoinControlSortColumn") && settings.contains("nCoinControlSortOrder"))
        sortView(settings.value("nCoinControlSortColumn").toInt(), (static_cast<Qt::SortOrder>(settings.value("nCoinControlSortOrder").toInt())));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

CoinControlDialog::~CoinControlDialog()
{
    QSettings settings;
    settings.setValue("nCoinControlMode", ui->radioListMode->isChecked());
    settings.setValue("nCoinControlSortColumn", sortColumn);
    settings.setValue("nCoinControlSortOrder", (int)sortOrder);

    delete ui;
<<<<<<< HEAD
}

void CoinControlDialog::setModel(WalletModel* model)
{
    this->model = model;
=======
    delete coinControl;
}

void CoinControlDialog::setModel(WalletModel* _model)
{
    this->model = _model;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    if (model && model->getOptionsModel() && model->getAddressTableModel()) {
        updateView();
        updateLabelLocked();
<<<<<<< HEAD
        CoinControlDialog::updateLabels(model, this);
        updateDialogLabels();
    }
}

// ok button
void CoinControlDialog::buttonBoxClicked(QAbstractButton* button)
{
    if (ui->buttonBox->buttonRole(button) == QDialogButtonBox::AcceptRole)
        done(QDialog::Accepted); // closes the dialog
}

=======
        updateLabels();
    }
}

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// (un)select all
void CoinControlDialog::buttonSelectAllClicked()
{
    // "Select all": if some entry is unchecked, then check it
    // "Unselect all": if some entry is checked, then uncheck it
<<<<<<< HEAD
    Qt::CheckState wantedState = fSelectAllToggled ? Qt::Checked : Qt::Unchecked;
=======
    const bool fSelectAll = ui->pushButtonSelectAll->isChecked();
    Qt::CheckState wantedState = fSelectAll ? Qt::Checked : Qt::Unchecked;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    ui->treeWidget->setEnabled(false);
    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i++)
        if (ui->treeWidget->topLevelItem(i)->checkState(COLUMN_CHECKBOX) != wantedState)
            ui->treeWidget->topLevelItem(i)->setCheckState(COLUMN_CHECKBOX, wantedState);
    ui->treeWidget->setEnabled(true);
<<<<<<< HEAD
    if (!fSelectAllToggled) {
        coinControl->UnSelectAll(); // just to be sure
        ui->pushButtonSelectAll->setText(tr("Select all"));
    } else {
        ui->pushButtonSelectAll->setText(tr("Unselect all"));
    }
    fSelectAllToggled = !fSelectAllToggled;
    CoinControlDialog::updateLabels(model, this);
    updateDialogLabels();
=======
    if (!fSelectAll)
        coinControl->UnSelectAll(); // just to be sure
    updateLabels();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

// Toggle lock state
void CoinControlDialog::buttonToggleLockClicked()
{
<<<<<<< HEAD
=======
    if (!fSelectTransparent) return; // todo: implement locked notes
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    QTreeWidgetItem* item;
    // Works in list-mode only
    if (ui->radioListMode->isChecked()) {
        ui->treeWidget->setEnabled(false);
        for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i++) {
            item = ui->treeWidget->topLevelItem(i);

<<<<<<< HEAD
            if (item->text(COLUMN_TYPE) == "MultiSig")
                continue;

            COutPoint outpt(uint256(item->text(COLUMN_TXHASH).toStdString()), item->text(COLUMN_VOUT_INDEX).toUInt());
            if (model->isLockedCoin(uint256(item->text(COLUMN_TXHASH).toStdString()), item->text(COLUMN_VOUT_INDEX).toUInt())) {
                model->unlockCoin(outpt);
                item->setDisabled(false);
                item->setIcon(COLUMN_CHECKBOX, QIcon());
=======
            COutPoint outpt(uint256S(item->text(COLUMN_TXHASH).toStdString()), item->text(COLUMN_VOUT_INDEX).toUInt());
            if (model->isLockedCoin(uint256S(item->text(COLUMN_TXHASH).toStdString()), item->text(COLUMN_VOUT_INDEX).toUInt())) {
                model->unlockCoin(outpt);
                item->setDisabled(false);
                // restore cold-stake snowflake icon for P2CS which were previously locked
                if (item->data(COLUMN_CHECKBOX, Qt::UserRole) == QString("Delegated"))
                    item->setIcon(COLUMN_CHECKBOX, QIcon("://ic-check-cold-staking-off"));
                else
                    item->setIcon(COLUMN_CHECKBOX, QIcon());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            } else {
                model->lockCoin(outpt);
                item->setDisabled(true);
                item->setIcon(COLUMN_CHECKBOX, QIcon(":/icons/lock_closed"));
            }
            updateLabelLocked();
        }
        ui->treeWidget->setEnabled(true);
<<<<<<< HEAD
        CoinControlDialog::updateLabels(model, this);
        updateDialogLabels();
=======
        updateLabels();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    } else {
        QMessageBox msgBox;
        msgBox.setObjectName("lockMessageBox");
        msgBox.setStyleSheet(GUIUtil::loadStyleSheet());
        msgBox.setText(tr("Please switch to \"List mode\" to use this function."));
        msgBox.exec();
    }
}

// context menu
void CoinControlDialog::showMenu(const QPoint& point)
{
    QTreeWidgetItem* item = ui->treeWidget->itemAt(point);
    if (item) {
        contextMenuItem = item;

        // disable some items (like Copy Transaction ID, lock, unlock) for tree roots in context menu
<<<<<<< HEAD
        if (item->text(COLUMN_TXHASH).length() == 64) // transaction hash is 64 characters (this means its a child node, so its not a parent node in tree mode)
        {
            copyTransactionHashAction->setEnabled(true);
            if (model->isLockedCoin(uint256(item->text(COLUMN_TXHASH).toStdString()), item->text(COLUMN_VOUT_INDEX).toUInt())) {
=======
        if (item->text(COLUMN_TXHASH).length() == 64) { // transaction hash is 64 characters (this means its a child node, so its not a parent node in tree mode)
            copyTransactionHashAction->setEnabled(true);
            if (model->isLockedCoin(uint256S(item->text(COLUMN_TXHASH).toStdString()), item->text(COLUMN_VOUT_INDEX).toUInt())) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                lockAction->setEnabled(false);
                unlockAction->setEnabled(true);
            } else {
                lockAction->setEnabled(true);
                unlockAction->setEnabled(false);
            }
<<<<<<< HEAD
        } else // this means click on parent node in tree mode -> disable all
        {
=======
        } else { // this means click on parent node in tree mode -> disable all
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            copyTransactionHashAction->setEnabled(false);
            lockAction->setEnabled(false);
            unlockAction->setEnabled(false);
        }

        // show context menu
        contextMenu->exec(QCursor::pos());
    }
}

// context menu action: copy amount
void CoinControlDialog::copyAmount()
{
    GUIUtil::setClipboard(BitcoinUnits::removeSpaces(contextMenuItem->text(COLUMN_AMOUNT)));
}

// context menu action: copy label
void CoinControlDialog::copyLabel()
{
    if (ui->radioTreeMode->isChecked() && contextMenuItem->text(COLUMN_LABEL).length() == 0 && contextMenuItem->parent())
        GUIUtil::setClipboard(contextMenuItem->parent()->text(COLUMN_LABEL));
    else
        GUIUtil::setClipboard(contextMenuItem->text(COLUMN_LABEL));
}

// context menu action: copy address
void CoinControlDialog::copyAddress()
{
    if (ui->radioTreeMode->isChecked() && contextMenuItem->text(COLUMN_ADDRESS).length() == 0 && contextMenuItem->parent())
        GUIUtil::setClipboard(contextMenuItem->parent()->text(COLUMN_ADDRESS));
    else
        GUIUtil::setClipboard(contextMenuItem->text(COLUMN_ADDRESS));
}

// context menu action: copy transaction id
void CoinControlDialog::copyTransactionHash()
{
    GUIUtil::setClipboard(contextMenuItem->text(COLUMN_TXHASH));
}

// context menu action: lock coin
void CoinControlDialog::lockCoin()
{
<<<<<<< HEAD
    if (contextMenuItem->checkState(COLUMN_CHECKBOX) == Qt::Checked)
        contextMenuItem->setCheckState(COLUMN_CHECKBOX, Qt::Unchecked);

    COutPoint outpt(uint256(contextMenuItem->text(COLUMN_TXHASH).toStdString()), contextMenuItem->text(COLUMN_VOUT_INDEX).toUInt());
=======
    if (!fSelectTransparent) return; // todo: implement locked notes
    if (contextMenuItem->checkState(COLUMN_CHECKBOX) == Qt::Checked)
        contextMenuItem->setCheckState(COLUMN_CHECKBOX, Qt::Unchecked);

    COutPoint outpt(uint256S(contextMenuItem->text(COLUMN_TXHASH).toStdString()), contextMenuItem->text(COLUMN_VOUT_INDEX).toUInt());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    model->lockCoin(outpt);
    contextMenuItem->setDisabled(true);
    contextMenuItem->setIcon(COLUMN_CHECKBOX, QIcon(":/icons/lock_closed"));
    updateLabelLocked();
}

// context menu action: unlock coin
void CoinControlDialog::unlockCoin()
{
<<<<<<< HEAD
    COutPoint outpt(uint256(contextMenuItem->text(COLUMN_TXHASH).toStdString()), contextMenuItem->text(COLUMN_VOUT_INDEX).toUInt());
    model->unlockCoin(outpt);
    contextMenuItem->setDisabled(false);
    contextMenuItem->setIcon(COLUMN_CHECKBOX, QIcon());
=======
    if (!fSelectTransparent) return; // todo: implement locked notes
    COutPoint outpt(uint256S(contextMenuItem->text(COLUMN_TXHASH).toStdString()), contextMenuItem->text(COLUMN_VOUT_INDEX).toUInt());
    model->unlockCoin(outpt);
    contextMenuItem->setDisabled(false);
    // restore cold-stake snowflake icon for P2CS which were previously locked
    if (contextMenuItem->data(COLUMN_CHECKBOX, Qt::UserRole) == QString("Delegated"))
        contextMenuItem->setIcon(COLUMN_CHECKBOX, QIcon("://ic-check-cold-staking-off"));
    else
        contextMenuItem->setIcon(COLUMN_CHECKBOX, QIcon());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    updateLabelLocked();
}

// copy label "Quantity" to clipboard
void CoinControlDialog::clipboardQuantity()
{
    GUIUtil::setClipboard(ui->labelCoinControlQuantity->text());
<<<<<<< HEAD
=======
    inform(tr("Quantity Copied"));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

// copy label "Amount" to clipboard
void CoinControlDialog::clipboardAmount()
{
<<<<<<< HEAD
    GUIUtil::setClipboard(ui->labelCoinControlAmount->text().left(ui->labelCoinControlAmount->text().indexOf(" ")));
=======
    GUIUtil::setClipboard(BitcoinUnits::removeSpaces(ui->labelCoinControlAmount->text().left(ui->labelCoinControlAmount->text().indexOf(" "))));
    inform(tr("Amount Copied"));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

// copy label "Fee" to clipboard
void CoinControlDialog::clipboardFee()
{
<<<<<<< HEAD
    GUIUtil::setClipboard(ui->labelCoinControlFee->text().left(ui->labelCoinControlFee->text().indexOf(" ")).replace("~", ""));
=======
    GUIUtil::setClipboard(BitcoinUnits::removeSpaces(ui->labelCoinControlFee->text().left(ui->labelCoinControlFee->text().indexOf(" ")).replace("~", "")));
    inform(tr("Fee Copied"));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

// copy label "After fee" to clipboard
void CoinControlDialog::clipboardAfterFee()
{
<<<<<<< HEAD
    GUIUtil::setClipboard(ui->labelCoinControlAfterFee->text().left(ui->labelCoinControlAfterFee->text().indexOf(" ")).replace("~", ""));
=======
    GUIUtil::setClipboard(BitcoinUnits::removeSpaces(ui->labelCoinControlAfterFee->text().left(ui->labelCoinControlAfterFee->text().indexOf(" ")).replace("~", "")));
    inform(tr("After Fee Copied"));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

// copy label "Bytes" to clipboard
void CoinControlDialog::clipboardBytes()
{
    GUIUtil::setClipboard(ui->labelCoinControlBytes->text().replace("~", ""));
<<<<<<< HEAD
}

// copy label "Priority" to clipboard
void CoinControlDialog::clipboardPriority()
{
    GUIUtil::setClipboard(ui->labelCoinControlPriority->text());
=======
    inform(tr("Bytes Copied"));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

// copy label "Dust" to clipboard
void CoinControlDialog::clipboardLowOutput()
{
    GUIUtil::setClipboard(ui->labelCoinControlLowOutput->text());
<<<<<<< HEAD
=======
    inform(tr("Dust Copied"));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

// copy label "Change" to clipboard
void CoinControlDialog::clipboardChange()
{
<<<<<<< HEAD
    GUIUtil::setClipboard(ui->labelCoinControlChange->text().left(ui->labelCoinControlChange->text().indexOf(" ")).replace("~", ""));
=======
    GUIUtil::setClipboard(BitcoinUnits::removeSpaces(ui->labelCoinControlChange->text().left(ui->labelCoinControlChange->text().indexOf(" ")).replace("~", "")));
    inform(tr("Change Copied"));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

// treeview: sort
void CoinControlDialog::sortView(int column, Qt::SortOrder order)
{
    sortColumn = column;
    sortOrder = order;
    ui->treeWidget->sortItems(column, order);
    ui->treeWidget->header()->setSortIndicator(sortColumn, sortOrder);
}

// treeview: clicked on header
void CoinControlDialog::headerSectionClicked(int logicalIndex)
{
<<<<<<< HEAD
    if (logicalIndex == COLUMN_CHECKBOX) // click on most left column -> do nothing
    {
=======
    if (logicalIndex == COLUMN_CHECKBOX) { // click on most left column -> do nothing
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        ui->treeWidget->header()->setSortIndicator(sortColumn, sortOrder);
    } else {
        if (sortColumn == logicalIndex)
            sortOrder = ((sortOrder == Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder);
        else {
            sortColumn = logicalIndex;
            sortOrder = ((sortColumn == COLUMN_LABEL || sortColumn == COLUMN_ADDRESS) ? Qt::AscendingOrder : Qt::DescendingOrder); // if label or address then default => asc, else default => desc
        }

        sortView(sortColumn, sortOrder);
    }
}

// toggle tree mode
void CoinControlDialog::radioTreeMode(bool checked)
{
    if (checked && model)
        updateView();
}

// toggle list mode
void CoinControlDialog::radioListMode(bool checked)
{
    if (checked && model)
        updateView();
}

// checkbox clicked by user
void CoinControlDialog::viewItemChanged(QTreeWidgetItem* item, int column)
{
<<<<<<< HEAD
    if (column == COLUMN_CHECKBOX && item->text(COLUMN_TXHASH).length() == 64) // transaction hash is 64 characters (this means its a child node, so its not a parent node in tree mode)
    {
        COutPoint outpt(uint256(item->text(COLUMN_TXHASH).toStdString()), item->text(COLUMN_VOUT_INDEX).toUInt());

=======
    if (column == COLUMN_CHECKBOX && item->text(COLUMN_TXHASH).length() == 64) { // transaction hash is 64 characters (this means its a child node, so its not a parent node in tree mode)
        BaseOutPoint outpt(uint256S(item->text(COLUMN_TXHASH).toStdString()),
                           item->text(COLUMN_VOUT_INDEX).toUInt(),
                           fSelectTransparent);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        if (item->checkState(COLUMN_CHECKBOX) == Qt::Unchecked)
            coinControl->UnSelect(outpt);
        else if (item->isDisabled()) // locked (this happens if "check all" through parent node)
            item->setCheckState(COLUMN_CHECKBOX, Qt::Unchecked);
<<<<<<< HEAD
        else
            coinControl->Select(outpt);

        // selection changed -> update labels
        if (ui->treeWidget->isEnabled()){ // do not update on every click for (un)select all
            CoinControlDialog::updateLabels(model, this);
            updateDialogLabels();
        }
    }

    // TODO: Remove this temporary qt5 fix after Qt5.3 and Qt5.4 are no longer used.
    //       Fixed in Qt5.5 and above: https://bugreports.qt.io/browse/QTBUG-43473
    else if (column == COLUMN_CHECKBOX && item->childCount() > 0)
    {
        if (item->checkState(COLUMN_CHECKBOX) == Qt::PartiallyChecked && item->child(0)->checkState(COLUMN_CHECKBOX) == Qt::PartiallyChecked)
            item->setCheckState(COLUMN_CHECKBOX, Qt::Checked);
    }
}

// return human readable label for priority number
QString CoinControlDialog::getPriorityLabel(double dPriority, double mempoolEstimatePriority)
{
    double dPriorityMedium = mempoolEstimatePriority;

    if (dPriorityMedium <= 0)
        dPriorityMedium = AllowFreeThreshold(); // not enough data, back to hard-coded

    if (dPriority / 1000000 > dPriorityMedium)
        return tr("highest");
    else if (dPriority / 100000 > dPriorityMedium)
        return tr("higher");
    else if (dPriority / 10000 > dPriorityMedium)
        return tr("high");
    else if (dPriority / 1000 > dPriorityMedium)
        return tr("medium-high");
    else if (dPriority > dPriorityMedium)
        return tr("medium");
    else if (dPriority * 10 > dPriorityMedium)
        return tr("low-medium");
    else if (dPriority * 100 > dPriorityMedium)
        return tr("low");
    else if (dPriority * 1000 > dPriorityMedium)
        return tr("lower");
    else
        return tr("lowest");
=======
        else {
            const CAmount value = static_cast<CAmount>(item->data(CoinControlDialog::COLUMN_AMOUNT, Qt::UserRole).toLongLong());
            bool isP2CS = item->data(COLUMN_CHECKBOX, Qt::UserRole) == QString("Delegated");
            coinControl->Select(outpt, value, isP2CS);
        }

        // selection changed -> update labels
        if (ui->treeWidget->isEnabled()) { // do not update on every click for (un)select all
            updateLabels();
        }
    }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

// shows count of locked unspent outputs
void CoinControlDialog::updateLabelLocked()
{
<<<<<<< HEAD
    vector<COutPoint> vOutpts;
    model->listLockedCoins(vOutpts);
    if (vOutpts.size() > 0) {
        ui->labelLocked->setText(tr("(%1 locked)").arg(vOutpts.size()));
        ui->labelLocked->setVisible(true);
    } else
        ui->labelLocked->setVisible(false);
}

void CoinControlDialog::updateDialogLabels()
{

    if (this->parentWidget() == nullptr) {
        CoinControlDialog::updateLabels(model, this);
        return;
    }

    vector<COutPoint> vCoinControl;
    std::vector<COutput> vOutputs;
    coinControl->ListSelected(vCoinControl);
    model->getOutputs(vCoinControl, vOutputs);

    CAmount nAmount = 0;
    unsigned int nQuantity = 0;
    for (const COutput& out : vOutputs) {
        // unselect already spent, very unlikely scenario, this could happen
        // when selected are spent elsewhere, like rpc or another computer
        uint256 txhash = out.tx->GetHash();
        COutPoint outpt(txhash, out.i);
        if(model->isSpent(outpt)) {
            coinControl->UnSelect(outpt);
            continue;
        }

        // Quantity
        nQuantity++;

        // Amount
        nAmount += model->getCWallet()->getCTxOutValue(*out.tx, out.tx->vout[out.i]);
    }
}

void CoinControlDialog::updateLabels(WalletModel* model, QDialog* dialog)
{
    if (!model)
        return;

    // nPayAmount
    CAmount nPayAmount = 0;
    bool fDust = false;
    CMutableTransaction txDummy;
   Q_FOREACH (const CAmount& amount, CoinControlDialog::payAmounts) {
        nPayAmount += amount;

        if (amount > 0) {
            CTxOut txout(amount, (CScript)vector<unsigned char>(24, 0));
            txDummy.vout.push_back(txout);
            if (txout.IsDust(::minRelayTxFee))
                fDust = true;
        }
    }

    QString sPriorityLabel = tr("none");
    CAmount nAmount = 0;
    CAmount nPayFee = 0;
    CAmount nAfterFee = 0;
    CAmount nChange = 0;
    unsigned int nBytes = 0;
    unsigned int nBytesInputs = 0;
    double dPriority = 0;
    double dPriorityInputs = 0;
    unsigned int nQuantity = 0;
    int nQuantityUncompressed = 0;
    bool fAllowFree = false;

    vector<COutPoint> vCoinControl;
    std::vector<COutput> vOutputs;
    coinControl->ListSelected(vCoinControl);
    model->getOutputs(vCoinControl, vOutputs);

    for (const COutput& out : vOutputs) {
        // unselect already spent, very unlikely scenario, this could happen
        // when selected are spent elsewhere, like rpc or another computer
        uint256 txhash = out.tx->GetHash();
        COutPoint outpt(txhash, out.i);
        if (model->isSpent(outpt)) {
            coinControl->UnSelect(outpt);
            continue;
        }

        // Quantity
        nQuantity++;

        // Amount
        nAmount += model->getCWallet()->getCTxOutValue(*out.tx, out.tx->vout[out.i]);

        // Priority
        dPriorityInputs += (double)(model->getCWallet()->getCTxOutValue(*out.tx, out.tx->vout[out.i])) * (out.nDepth + 1);

        // Bytes
        CTxDestination address;
        if (ExtractDestination(out.tx->vout[out.i].scriptPubKey, address)) {
            CPubKey pubkey;
            CKeyID* keyid = boost::get<CKeyID>(&address);
            if (keyid && model->getPubKey(*keyid, pubkey)) {
                nBytesInputs += (pubkey.IsCompressed() ? 148 : 180);
                if (!pubkey.IsCompressed())
                    nQuantityUncompressed++;
            } else
                nBytesInputs += 148; // in all error cases, simply assume 148 here
        } else
            nBytesInputs += 148;
    }

    // calculation
    if (nQuantity > 0) {
        // Bytes
        nBytes = nBytesInputs + ((CoinControlDialog::payAmounts.size() > 0 ? CoinControlDialog::payAmounts.size() + max(1, CoinControlDialog::nSplitBlockDummy) : 2) * 34) + 10; // always assume +1 output for change here

        // Priority
        double mempoolEstimatePriority = mempool.estimatePriority(nTxConfirmTarget);
        dPriority = dPriorityInputs / (nBytes - nBytesInputs + (nQuantityUncompressed * 29)); // 29 = 180 - 151 (uncompressed public keys are over the limit. max 151 bytes of the input are ignored for priority)
        sPriorityLabel = CoinControlDialog::getPriorityLabel(dPriority, mempoolEstimatePriority);

        // Fee
        nPayFee = CWallet::GetMinimumFee(nBytes, nTxConfirmTarget, mempool);

        // IX Fee
        if (coinControl->useSwiftTX) nPayFee = max(nPayFee, CENT);
        // Allow free?
        double dPriorityNeeded = mempoolEstimatePriority;
        if (dPriorityNeeded <= 0)
            dPriorityNeeded = AllowFreeThreshold(); // not enough data, back to hard-coded
        fAllowFree = (dPriority >= dPriorityNeeded);

        if (fSendFreeTransactions)
            if (fAllowFree && nBytes <= MAX_FREE_TRANSACTION_CREATE_SIZE)
                nPayFee = 0;

        if (nPayAmount > 0) {
            nChange = nAmount - nPayFee - nPayAmount;

            // Never create dust outputs; if we would, just add the dust to the fee.
            if (nChange > 0 && nChange < CENT) {
                CTxOut txout(nChange, (CScript)vector<unsigned char>(24, 0));
                if (txout.IsDust(::minRelayTxFee)) {
                    nPayFee += nChange;
                    nChange = 0;
                }
            }

            if (nChange == 0)
                nBytes -= 34;
        }

        // after fee
        nAfterFee = nAmount - nPayFee;
        if (nAfterFee < 0)
            nAfterFee = 0;
    }

    // actually update labels
    int nDisplayUnit = BitcoinUnits::PRCY;
    if (model && model->getOptionsModel())
        nDisplayUnit = model->getOptionsModel()->getDisplayUnit();

    QLabel* l1 = dialog->findChild<QLabel*>("labelCoinControlQuantity");
    QLabel* l2 = dialog->findChild<QLabel*>("labelCoinControlAmount");
    QLabel* l3 = dialog->findChild<QLabel*>("labelCoinControlFee");
    QLabel* l4 = dialog->findChild<QLabel*>("labelCoinControlAfterFee");
    QLabel* l5 = dialog->findChild<QLabel*>("labelCoinControlBytes");
    QLabel* l6 = dialog->findChild<QLabel*>("labelCoinControlPriority");
    QLabel* l7 = dialog->findChild<QLabel*>("labelCoinControlLowOutput");
    QLabel* l8 = dialog->findChild<QLabel*>("labelCoinControlChange");

    // enable/disable "dust" and "change"
    dialog->findChild<QLabel*>("labelCoinControlLowOutputText")->setEnabled(nPayAmount > 0);
    dialog->findChild<QLabel*>("labelCoinControlLowOutput")->setEnabled(nPayAmount > 0);
    dialog->findChild<QLabel*>("labelCoinControlChangeText")->setEnabled(nPayAmount > 0);
    dialog->findChild<QLabel*>("labelCoinControlChange")->setEnabled(nPayAmount > 0);

    // stats
    l1->setText(QString::number(nQuantity));                            // Quantity
    l2->setText(BitcoinUnits::formatWithUnit(nDisplayUnit, nAmount));   // Amount
    l3->setText(BitcoinUnits::formatWithUnit(nDisplayUnit, nPayFee));   // Fee
    l4->setText(BitcoinUnits::formatWithUnit(nDisplayUnit, nAfterFee)); // After Fee
    l5->setText(((nBytes > 0) ? "~" : "") + QString::number(nBytes));   // Bytes
    l6->setText(sPriorityLabel);                                        // Priority
    l7->setText(fDust ? tr("yes") : tr("no"));                          // Dust
    l8->setText(BitcoinUnits::formatWithUnit(nDisplayUnit, nChange));   // Change
    if (nPayFee > 0 && !(payTxFee.GetFeePerK() > 0 && fPayAtLeastCustomFee && nBytes < 1000)) {
        l3->setText("~" + l3->text());
        l4->setText("~" + l4->text());
        if (nChange > 0)
            l8->setText("~" + l8->text());
    }

    // turn labels "red"
    l5->setStyleSheet((nBytes >= MAX_FREE_TRANSACTION_CREATE_SIZE) ? "color:red;" : ""); // Bytes >= 1000
    l6->setStyleSheet((dPriority > 0 && !fAllowFree) ? "color:red;" : "");               // Priority < "medium"
    l7->setStyleSheet((fDust) ? "color:red;" : "");                                      // Dust = "yes"

    // tool tips
    QString toolTip1 = tr("This label turns red, if the transaction size is greater than 1000 bytes.") + "<br /><br />";
    toolTip1 += tr("This means a fee of at least %1 per kB is required.").arg(BitcoinUnits::formatWithUnit(nDisplayUnit, CWallet::minTxFee.GetFeePerK())) + "<br /><br />";
    toolTip1 += tr("Can vary +/- 1 byte per input.");

    QString toolTip2 = tr("Transactions with higher priority are more likely to get included into a block.") + "<br /><br />";
    toolTip2 += tr("This label turns red, if the priority is smaller than \"medium\".") + "<br /><br />";
    toolTip2 += tr("This means a fee of at least %1 per kB is required.").arg(BitcoinUnits::formatWithUnit(nDisplayUnit, CWallet::minTxFee.GetFeePerK()));

    QString toolTip3 = tr("This label turns red, if any recipient receives an amount smaller than %1.").arg(BitcoinUnits::formatWithUnit(nDisplayUnit, ::minRelayTxFee.GetFee(546)));
=======
    if (fSelectTransparent) {
        std::set<COutPoint> vOutpts = model->listLockedCoins();
        if (!vOutpts.empty()) {
            ui->labelLocked->setText(tr("(%1 locked)").arg(vOutpts.size()));
            ui->labelLocked->setVisible(true);
        } else
            ui->labelLocked->setVisible(false);
    } else {
        ui->labelLocked->setVisible(false);
        // TODO: implement locked notes functionality inside the wallet..
    }
}

// serialized int size
static int GetCompactSize(uint64_t nSize)
{
    if (nSize < 253) {
        return 1;
    } else if (nSize <= std::numeric_limits<unsigned short>::max()) {
        return 3;
    } else if (nSize <= std::numeric_limits<unsigned int>::max()) {
        return 5;
    }
    return 9;
}

TotalAmounts CoinControlDialog::getTotals() const
{
    TotalAmounts t;

    std::vector<OutPointWrapper> vCoinControl;
    coinControl->ListSelected(vCoinControl);

    for (const OutPointWrapper& out : vCoinControl) {
        // Quantity
        t.nQuantity++;
        // Amount
        t.nAmount += out.value;
        // Bytes
        t.nBytes += (fSelectTransparent ? (CTXIN_SPEND_DUST_SIZE + (out.isP2CS ? 1 : 0))
                                      : SPENDDESCRIPTION_SIZE);
    }

    // selected inputs
    int nTransIns, nShieldIns;
    if (fSelectTransparent) {
        nTransIns = t.nQuantity;
        nShieldIns = 0;
    } else {
        nTransIns = 0;
        nShieldIns = t.nQuantity;
    }

    // calculation
    const int P2CS_OUT_SIZE = 61;
    int nTransOuts = 0, nShieldOuts = 0;
    if (t.nQuantity > 0) {
        // Bytes: nBytesInputs + (sum of nBytesOutputs)
        // always assume +1 (p2pkh) output for change here
        t.nBytes += (fSelectTransparent ? CTXOUT_REGULAR_SIZE : OUTPUTDESCRIPTION_SIZE);
        for (const auto& a : payAmounts) {
            t.nPayAmount += a.first;
            bool shieldedOut = a.second;
            if (shieldedOut) nShieldOuts++;
            else nTransOuts++;
            if (a.first > 0 && !t.fDust) {
                if (a.first < (shieldedOut ? GetShieldedDustThreshold(dustRelayFee) : GetDustThreshold(dustRelayFee)))
                    t.fDust = true;
            }
            t.nBytes += (shieldedOut ? OUTPUTDESCRIPTION_SIZE
                                   : (forDelegation ? P2CS_OUT_SIZE : CTXOUT_REGULAR_SIZE));
        }

        // Shielded txes must include binding sig and valueBalance
        bool isShieldedTx = (nShieldIns + nShieldOuts > 0);
        if (isShieldedTx) {
            t.nBytes += (BINDINGSIG_SIZE + 8);
            // shielded in/outs len sizes
            t.nBytes += (GetCompactSize(nShieldIns) + GetCompactSize(nShieldOuts));
        }

        // nVersion, nType, nLockTime
        t.nBytes += 8;

        // vin/vout len sizes
        t.nBytes += (GetCompactSize(nTransIns) +  GetCompactSize(nTransOuts));

        // Fee (default K fixed for shielded fee for now)
        t.nPayFee = GetMinRelayFee(t.nBytes) * (isShieldedTx ? DEFAULT_SHIELDEDTXFEE_K : 1);

        if (t.nPayAmount > 0) {
            t.nChange = t.nAmount - t.nPayFee - t.nPayAmount;

            // Never create dust outputs; if we would, just add the dust to the fee.
            CAmount dustThreshold = fSelectTransparent ? GetDustThreshold(dustRelayFee)
                                                       : GetShieldedDustThreshold(dustRelayFee);
            if (t.nChange > 0 && t.nChange < dustThreshold) {
                t.nPayFee += t.nChange;
                t.nChange = 0;
            }

            if (t.nChange == 0)
                t.nBytes -= (fSelectTransparent ? CTXOUT_REGULAR_SIZE : SPENDDESCRIPTION_SIZE);
        }

        // after fee
        t.nAfterFee = std::max<CAmount>(t.nAmount - t.nPayFee, 0);
    }
    return t;
}

void CoinControlDialog::updateLabels()
{
    if (!model)
        return;

    ui->labelTitle->setText(fSelectTransparent ?
            "Select PIV Outputs to Spend" :
            "Select Shielded PIV to Spend");

    const TotalAmounts& t = getTotals();

    // update SelectAll button state
    // if inputs selected > inputs unselected, set checked (label "Unselect All")
    // if inputs selected <= inputs unselected, set unchecked (label "Select All")
    updatePushButtonSelectAll(coinControl->QuantitySelected() * 2 > nSelectableInputs);

    // actually update labels
    int nDisplayUnit = BitcoinUnits::PIV;
    if (model && model->getOptionsModel())
        nDisplayUnit = model->getOptionsModel()->getDisplayUnit();

    // enable/disable "dust" and "change"
    const bool hasPayAmount = t.nPayAmount > 0;
    ui->labelCoinControlLowOutputText->setEnabled(hasPayAmount);
    ui->labelCoinControlLowOutput->setEnabled(hasPayAmount);
    ui->labelCoinControlChangeText->setEnabled(hasPayAmount);
    ui->labelCoinControlChange->setEnabled(hasPayAmount);

    // stats
    ui->labelCoinControlQuantity->setText(QString::number(t.nQuantity));
    ui->labelCoinControlAmount->setText(BitcoinUnits::formatWithUnit(nDisplayUnit, t.nAmount));
    ui->labelCoinControlFee->setText(BitcoinUnits::formatWithUnit(nDisplayUnit, t.nPayFee));
    ui->labelCoinControlAfterFee->setText(BitcoinUnits::formatWithUnit(nDisplayUnit, t.nAfterFee));
    ui->labelCoinControlBytes->setText(((t.nBytes > 0) ? "~" : "") + QString::number(t.nBytes));
    ui->labelCoinControlLowOutput->setText(t.fDust ? tr("yes") : tr("no"));
    ui->labelCoinControlChange->setText(BitcoinUnits::formatWithUnit(nDisplayUnit, t.nChange));
    if (t.nPayFee > 0 && !(payTxFee.GetFeePerK() > 0 && fPayAtLeastCustomFee && t.nBytes < 1000)) {
        ui->labelCoinControlFee->setText("~" + ui->labelCoinControlFee->text());
        ui->labelCoinControlAfterFee->setText("~" + ui->labelCoinControlAfterFee->text());
        if (t.nChange > 0)
            ui->labelCoinControlChange->setText("~" + ui->labelCoinControlChange->text());
    }

    // turn labels "red"
    ui->labelCoinControlLowOutput->setStyleSheet((t.fDust) ? "color:red;" : "");                                      // Dust = "yes"

    // tool tips
    QString toolTip3 = tr("This label turns red, if recipient receives an amount smaller than %1 (transparent) / %2 (shield)."
            ).arg(BitcoinUnits::formatWithUnit(nDisplayUnit, GetDustThreshold(dustRelayFee))).arg(BitcoinUnits::formatWithUnit(nDisplayUnit, GetShieldedDustThreshold(dustRelayFee)));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    // how many satoshis the estimated fee can vary per byte we guess wrong
    double dFeeVary;
    if (payTxFee.GetFeePerK() > 0)
<<<<<<< HEAD
        dFeeVary = (double)std::max(CWallet::minTxFee.GetFeePerK(), payTxFee.GetFeePerK()) / 1000;
    else
        dFeeVary = (double)std::max(CWallet::minTxFee.GetFeePerK(), mempool.estimateFee(nTxConfirmTarget).GetFeePerK()) / 1000;
    QString toolTip4 = tr("Can vary +/- %1 duff(s) per input.").arg(dFeeVary);

    l3->setToolTip(toolTip4);
    l4->setToolTip(toolTip4);
    l5->setToolTip(toolTip1);
    l6->setToolTip(toolTip2);
    l7->setToolTip(toolTip3);
    l8->setToolTip(toolTip4);
    dialog->findChild<QLabel*>("labelCoinControlFeeText")->setToolTip(l3->toolTip());
    dialog->findChild<QLabel*>("labelCoinControlAfterFeeText")->setToolTip(l4->toolTip());
    dialog->findChild<QLabel*>("labelCoinControlBytesText")->setToolTip(l5->toolTip());
    dialog->findChild<QLabel*>("labelCoinControlPriorityText")->setToolTip(l6->toolTip());
    dialog->findChild<QLabel*>("labelCoinControlLowOutputText")->setToolTip(l7->toolTip());
    dialog->findChild<QLabel*>("labelCoinControlChangeText")->setToolTip(l8->toolTip());

    // Insufficient funds
    QLabel* label = dialog->findChild<QLabel*>("labelCoinControlInsuffFunds");
    if (label)
        label->setVisible(nChange < 0);
=======
        dFeeVary = (double)std::max(GetRequiredFee(1000), payTxFee.GetFeePerK()) / 1000;
    else
        dFeeVary = (double)std::max(GetRequiredFee(1000), mempool.estimateSmartFee(nTxConfirmTarget).GetFeePerK()) / 1000;
    QString toolTip4 = tr("Can vary +/- %1 u%2 per input.").arg(dFeeVary).arg(CURRENCY_UNIT.c_str());

    ui->labelCoinControlFee->setToolTip(toolTip4);
    ui->labelCoinControlAfterFee->setToolTip(toolTip4);
    ui->labelCoinControlLowOutput->setToolTip(toolTip3);
    ui->labelCoinControlChange->setToolTip(toolTip4);
    ui->labelCoinControlFeeText->setToolTip(ui->labelCoinControlFee->toolTip());
    ui->labelCoinControlAfterFeeText->setToolTip(ui->labelCoinControlAfterFee->toolTip());
    ui->labelCoinControlBytesText->setToolTip(ui->labelCoinControlBytes->toolTip());
    ui->labelCoinControlLowOutputText->setToolTip(ui->labelCoinControlLowOutput->toolTip());
    ui->labelCoinControlChangeText->setToolTip(ui->labelCoinControlChange->toolTip());

    // Insufficient funds
    QLabel* label = findChild<QLabel*>("labelCoinControlInsuffFunds");
    if (label)
        label->setVisible(t.nChange < 0);
}

void CoinControlDialog::loadAvailableCoin(bool treeMode,
                                          CCoinControlWidgetItem* itemWalletAddress,
                                          QFlags<Qt::ItemFlag> flgCheckbox,
                                          QFlags<Qt::ItemFlag> flgTristate,
                                          int nDisplayUnit,
                                          const QString& sWalletAddress,
                                          const Optional<QString>& stakerAddress,
                                          const QString& sWalletLabel,
                                          const uint256& txhash,
                                          const uint32_t outIndex,
                                          const CAmount nValue,
                                          const int64_t nTime,
                                          const int nDepth,
                                          const bool isChange)
{
    CCoinControlWidgetItem* itemOutput;
    if (treeMode)
        itemOutput = new CCoinControlWidgetItem(itemWalletAddress);
    else
        itemOutput = new CCoinControlWidgetItem(ui->treeWidget);
    itemOutput->setFlags(flgCheckbox);
    itemOutput->setCheckState(COLUMN_CHECKBOX, Qt::Unchecked);

    // if listMode or change => show PIVX address. In tree mode, address is not shown again for direct wallet address outputs
    if (!treeMode) {
        itemOutput->setText(COLUMN_ADDRESS, sWalletAddress);
    }else {
        itemOutput->setToolTip(COLUMN_ADDRESS, sWalletAddress);
    }

    // label
    if (isChange) {
        // tooltip stating where the change is being stored.
        itemOutput->setToolTip(COLUMN_LABEL, tr("change in %1").arg(sWalletAddress));
        itemOutput->setText(COLUMN_LABEL, tr("(change)"));
    } else if (!treeMode) {
        itemOutput->setText(COLUMN_LABEL, sWalletLabel);
    }

    // amount
    itemOutput->setText(COLUMN_AMOUNT, BitcoinUnits::format(nDisplayUnit, nValue));
    itemOutput->setToolTip(COLUMN_AMOUNT, BitcoinUnits::format(nDisplayUnit, nValue));
    itemOutput->setData(COLUMN_AMOUNT, Qt::UserRole, QVariant((qlonglong) nValue));

    // date
    itemOutput->setText(COLUMN_DATE, GUIUtil::dateTimeStr(nTime));
    itemOutput->setToolTip(COLUMN_DATE, GUIUtil::dateTimeStr(nTime));
    itemOutput->setData(COLUMN_DATE, Qt::UserRole, QVariant((qlonglong) nTime));

    // confirmations
    itemOutput->setText(COLUMN_CONFIRMATIONS, QString::number(nDepth));
    itemOutput->setData(COLUMN_CONFIRMATIONS, Qt::UserRole, QVariant((qlonglong) nDepth));

    // transaction hash
    itemOutput->setText(COLUMN_TXHASH, QString::fromStdString(txhash.GetHex()));

    // vout index
    itemOutput->setText(COLUMN_VOUT_INDEX, QString::number(outIndex));

    // disable locked coins (!TODO: implement locked notes)
    bool isLockedCoin{false};
    if (fSelectTransparent) {
        isLockedCoin = model->isLockedCoin(txhash, outIndex);
        if (isLockedCoin) {
            --nSelectableInputs;
            coinControl->UnSelect({txhash, outIndex}); // just to be sure
            itemOutput->setDisabled(true);
            itemOutput->setIcon(COLUMN_CHECKBOX, QIcon(":/icons/lock_closed"));
        }
    }

    // set checkbox
    if (coinControl->IsSelected(COutPoint(txhash, outIndex)))
        itemOutput->setCheckState(COLUMN_CHECKBOX, Qt::Checked);

    // outputs delegated (for cold staking)
    if (stakerAddress) {
        itemOutput->setData(COLUMN_CHECKBOX, Qt::UserRole, QString("Delegated"));
        if (!isLockedCoin)
            itemOutput->setIcon(COLUMN_CHECKBOX, QIcon("://ic-check-cold-staking-off"));
        itemOutput->setToolTip(COLUMN_CHECKBOX, tr("delegated to %1 for cold staking").arg(*stakerAddress));
    }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

void CoinControlDialog::updateView()
{
    if (!model || !model->getOptionsModel() || !model->getAddressTableModel())
        return;

    bool treeMode = ui->radioTreeMode->isChecked();
<<<<<<< HEAD

    ui->treeWidget->clear();
    ui->treeWidget->setEnabled(false); // performance, otherwise updateLabels would be called for every checked checkbox
    ui->treeWidget->setAlternatingRowColors(!treeMode);
=======
    ui->treeWidget->setRootIsDecorated(treeMode);

    ui->treeWidget->clear();
    ui->treeWidget->setEnabled(false); // performance, otherwise updateLabels would be called for every checked checkbox
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    QFlags<Qt::ItemFlag> flgCheckbox = Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
    QFlags<Qt::ItemFlag> flgTristate = Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsTristate;

    int nDisplayUnit = model->getOptionsModel()->getDisplayUnit();
<<<<<<< HEAD
    double mempoolEstimatePriority = mempool.estimatePriority(nTxConfirmTarget);

    map<QString, vector<COutput>> mapCoins;
    model->listCoins(mapCoins);

    for (PAIRTYPE(QString, vector<COutput>) coins : mapCoins) {
        CCoinControlWidgetItem* itemWalletAddress = new CCoinControlWidgetItem();
        itemWalletAddress->setCheckState(COLUMN_CHECKBOX, Qt::Unchecked);
        QString sWalletAddress = coins.first;
=======
    nSelectableInputs = 0;
    std::map<WalletModel::ListCoinsKey, std::vector<WalletModel::ListCoinsValue>> mapCoins;
    model->listCoins(mapCoins, fSelectTransparent);

    for (const auto& coins : mapCoins) {
        CCoinControlWidgetItem* itemWalletAddress = new CCoinControlWidgetItem();
        itemWalletAddress->setCheckState(COLUMN_CHECKBOX, Qt::Unchecked);
        const WalletModel::ListCoinsKey& keys = coins.first;
        const QString& sWalletAddress = keys.address;
        const Optional<QString>& stakerAddress = keys.stakerAddress;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        QString sWalletLabel = model->getAddressTableModel()->labelForAddress(sWalletAddress);
        if (sWalletLabel.isEmpty())
            sWalletLabel = tr("(no label)");

        if (treeMode) {
            // wallet address
            ui->treeWidget->addTopLevelItem(itemWalletAddress);

            itemWalletAddress->setFlags(flgTristate);
            itemWalletAddress->setCheckState(COLUMN_CHECKBOX, Qt::Unchecked);

            // label
            itemWalletAddress->setText(COLUMN_LABEL, sWalletLabel);
            itemWalletAddress->setToolTip(COLUMN_LABEL, sWalletLabel);

            // address
            itemWalletAddress->setText(COLUMN_ADDRESS, sWalletAddress);
<<<<<<< HEAD
            itemWalletAddress->setToolTip(COLUMN_ADDRESS, sWalletAddress);
        }

        CAmount nSum = 0;
        double dPrioritySum = 0;
        int nChildren = 0;
        int nInputSum = 0;
        for(const COutput& out: coins.second) {
            isminetype mine = pwalletMain->IsMine(out.tx->vout[out.i]);
            int nInputSize = 0;
            nSum += model->getCWallet()->getCTxOutValue(*out.tx, out.tx->vout[out.i]);
            nChildren++;

            CCoinControlWidgetItem* itemOutput;
            if (treeMode)
                itemOutput = new CCoinControlWidgetItem(itemWalletAddress);
            else
                itemOutput = new CCoinControlWidgetItem(ui->treeWidget);
            itemOutput->setFlags(flgCheckbox);
            itemOutput->setCheckState(COLUMN_CHECKBOX, Qt::Unchecked);

            // address
            CTxDestination outputAddress;
            QString sAddress = "";
            if (ExtractDestination(out.tx->vout[out.i].scriptPubKey, outputAddress)) {
                sAddress = QString::fromStdString(CBitcoinAddress(outputAddress).ToString());

                // if listMode or change => show PRCY address. In tree mode, address is not shown again for direct wallet address outputs
                if (!treeMode || (!(sAddress == sWalletAddress)))
                    itemOutput->setText(COLUMN_ADDRESS, sAddress);

                itemOutput->setToolTip(COLUMN_ADDRESS, sAddress);

                CPubKey pubkey;
                CKeyID* keyid = boost::get<CKeyID>(&outputAddress);
                if (keyid && model->getPubKey(*keyid, pubkey) && !pubkey.IsCompressed())
                    nInputSize = 29; // 29 = 180 - 151 (public key is 180 bytes, priority free area is 151 bytes)
            }

            // label
            if (!(sAddress == sWalletAddress)) // change
            {
                // tooltip from where the change comes from
                itemOutput->setToolTip(COLUMN_LABEL, tr("change from %1 (%2)").arg(sWalletLabel).arg(sWalletAddress));
                itemOutput->setText(COLUMN_LABEL, tr("(change)"));
            } else if (!treeMode) {
                QString sLabel = model->getAddressTableModel()->labelForAddress(sAddress);
                if (sLabel.isEmpty())
                    sLabel = tr("(no label)");
                itemOutput->setText(COLUMN_LABEL, sLabel);
            }

            // amount
            itemOutput->setText(COLUMN_AMOUNT, BitcoinUnits::format(nDisplayUnit, model->getCWallet()->getCTxOutValue(*out.tx, out.tx->vout[out.i])));
            itemOutput->setToolTip(COLUMN_AMOUNT, BitcoinUnits::format(nDisplayUnit, model->getCWallet()->getCTxOutValue(*out.tx, out.tx->vout[out.i])));
            itemOutput->setData(COLUMN_AMOUNT, Qt::UserRole, QVariant((qlonglong) model->getCWallet()->getCTxOutValue(*out.tx, out.tx->vout[out.i]))); // padding so that sorting works correctly

            // date
            itemOutput->setText(COLUMN_DATE, GUIUtil::dateTimeStr(out.tx->GetTxTime()));
            itemOutput->setToolTip(COLUMN_DATE, GUIUtil::dateTimeStr(out.tx->GetTxTime()));
            itemOutput->setData(COLUMN_DATE, Qt::UserRole, QVariant((qlonglong) out.tx->GetTxTime()));

            // confirmations
            itemOutput->setText(COLUMN_CONFIRMATIONS, QString::number(out.nDepth));
            itemOutput->setData(COLUMN_CONFIRMATIONS, Qt::UserRole, QVariant((qlonglong) out.nDepth));

            // priority
            double dPriority = ((double)(model->getCWallet()->getCTxOutValue(*out.tx, out.tx->vout[out.i])) / (nInputSize + 78)) * (out.nDepth + 1); // 78 = 2 * 34 + 10
            itemOutput->setText(COLUMN_PRIORITY, CoinControlDialog::getPriorityLabel(dPriority, mempoolEstimatePriority));
            itemOutput->setData(COLUMN_PRIORITY, Qt::UserRole, QVariant((qlonglong) dPriority));
            dPrioritySum += (double)(model->getCWallet()->getCTxOutValue(*out.tx, out.tx->vout[out.i])) * (out.nDepth + 1);
            nInputSum += nInputSize;

            // transaction hash
            uint256 txhash = out.tx->GetHash();
            itemOutput->setText(COLUMN_TXHASH, QString::fromStdString(txhash.GetHex()));

            // vout index
            itemOutput->setText(COLUMN_VOUT_INDEX, QString::number(out.i));

            // disable locked coins
            if (model->isLockedCoin(txhash, out.i)) {
                COutPoint outpt(txhash, out.i);
                coinControl->UnSelect(outpt); // just to be sure
                itemOutput->setDisabled(true);
                itemOutput->setIcon(COLUMN_CHECKBOX, QIcon(":/icons/lock_closed"));
            }

            // set checkbox
            if (coinControl->IsSelected(txhash, out.i))
                itemOutput->setCheckState(COLUMN_CHECKBOX, Qt::Checked);
=======
            if (stakerAddress != nullopt) {
                itemWalletAddress->setIcon(COLUMN_CONFIRMATIONS, QIcon("://ic-check-cold-staking-off"));
                QString label = tr("Delegated to %1").arg(*stakerAddress);
                itemWalletAddress->setToolTip(COLUMN_ADDRESS, label);
                itemWalletAddress->setToolTip(COLUMN_CONFIRMATIONS, label);
            } else {
                itemWalletAddress->setToolTip(COLUMN_ADDRESS, sWalletAddress);
            }
        }

        CAmount nSum = 0;
        int nChildren = 0;
        for (const WalletModel::ListCoinsValue& out : coins.second) {
            ++nSelectableInputs;
            nSum += out.nValue;
            nChildren++;

            loadAvailableCoin(treeMode, itemWalletAddress, flgCheckbox, flgTristate,
                              nDisplayUnit, sWalletAddress, stakerAddress, sWalletLabel,
                              out.txhash, out.outIndex, out.nValue, out.nTime, out.nDepth,
                              keys.isChange);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        }

        // amount
        if (treeMode) {
<<<<<<< HEAD
            dPrioritySum = dPrioritySum / (nInputSum + 78);
=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            itemWalletAddress->setText(COLUMN_CHECKBOX, "(" + QString::number(nChildren) + ")");
            itemWalletAddress->setText(COLUMN_AMOUNT, BitcoinUnits::format(nDisplayUnit, nSum));
            itemWalletAddress->setToolTip(COLUMN_AMOUNT, BitcoinUnits::format(nDisplayUnit, nSum));
            itemWalletAddress->setData(COLUMN_AMOUNT, Qt::UserRole, QVariant((qlonglong) nSum));
<<<<<<< HEAD
            itemWalletAddress->setText(COLUMN_PRIORITY, CoinControlDialog::getPriorityLabel(dPrioritySum, mempoolEstimatePriority));
            itemWalletAddress->setData(COLUMN_PRIORITY, Qt::UserRole, QVariant((qlonglong) dPrioritySum));
=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        }
    }

    // expand all partially selected
    if (treeMode) {
        for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i++)
            if (ui->treeWidget->topLevelItem(i)->checkState(COLUMN_CHECKBOX) == Qt::PartiallyChecked)
                ui->treeWidget->topLevelItem(i)->setExpanded(true);
<<<<<<< HEAD
=======
        // restore saved width for COLUMN_CHECKBOX
        ui->treeWidget->setColumnWidth(COLUMN_CHECKBOX, colCheckBoxWidth_treeMode);
    } else {
        // save COLUMN_CHECKBOX width for tree-mode
        colCheckBoxWidth_treeMode = std::max(110, ui->treeWidget->columnWidth(COLUMN_CHECKBOX));
        // minimize COLUMN_CHECKBOX width in list-mode (need to display only the check box)
        ui->treeWidget->setColumnWidth(COLUMN_CHECKBOX, 70);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    // sort view
    sortView(sortColumn, sortOrder);
    ui->treeWidget->setEnabled(true);
<<<<<<< HEAD
=======

    // TODO: Remove this once note locking is functional
    // Hide or show locking button and context menu items
    lockAction->setVisible(fSelectTransparent);
    unlockAction->setVisible(fSelectTransparent);
    ui->pushButtonToggleLock->setVisible(fSelectTransparent);
}

void CoinControlDialog::refreshDialog()
{
    updateView();
    updateLabelLocked();
    updateLabels();
}

void CoinControlDialog::inform(const QString& text)
{
    if (!snackBar) snackBar = new SnackBar(nullptr, this);
    snackBar->setText(text);
    snackBar->resize(this->width(), snackBar->height());
    openDialog(snackBar, this);
}

void CoinControlDialog::clearPayAmounts()
{
    payAmounts.clear();
}

void CoinControlDialog::addPayAmount(const CAmount& amount, bool isShieldedRecipient)
{
    payAmounts.emplace_back(amount, isShieldedRecipient);
}

void CoinControlDialog::updatePushButtonSelectAll(bool checked)
{
    ui->pushButtonSelectAll->setChecked(checked);
    ui->pushButtonSelectAll->setText(checked ? tr("Unselect all") : tr("Select All"));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}
