// Copyright (c) 2011-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
<<<<<<< HEAD
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "askpassphrasedialog.h"
#include "ui_askpassphrasedialog.h"

#include "guiconstants.h"
#include "walletmodel.h"

#include "allocators.h"
=======
// Copyright (c) 2015-2020 The PIVX developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#if defined(HAVE_CONFIG_H)
#include "config/pivx-config.h"
#endif

#include "askpassphrasedialog.h"
#include "ui_askpassphrasedialog.h"
#include <QGraphicsDropShadowEffect>

#include "guiconstants.h"
#include "guiutil.h"
#include "walletmodel.h"
#include "qt/pivx/qtutils.h"
#include "qt/pivx/loadingdialog.h"
#include "qt/pivx/defaultdialog.h"
#include "qt/pivx/pivxgui.h"
#include <QDebug>
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

#include <QKeyEvent>
#include <QMessageBox>
#include <QPushButton>
<<<<<<< HEAD

AskPassphraseDialog::AskPassphraseDialog(Mode mode, QWidget* parent, WalletModel* model, Context context) : QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
                                                                                           ui(new Ui::AskPassphraseDialog),
                                                                                           mode(mode),
                                                                                           model(model),
                                                                                           context(context),
                                                                                           fCapsLock(false)
{
    ui->setupUi(this);
=======
#include <QWidget>

AskPassphraseDialog::AskPassphraseDialog(Mode mode, QWidget* parent, WalletModel* model, Context context) : QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
                                                                                                            ui(new Ui::AskPassphraseDialog),
                                                                                                            mode(mode),
                                                                                                            model(model),
                                                                                                            context(context),
                                                                                                            fCapsLock(false),
                                                                                                            btnWatch(new QCheckBox())
{
    ui->setupUi(this);
    this->setStyleSheet(GUIUtil::loadStyleSheet());

    ui->left->setProperty("cssClass", "container-dialog");

    ui->labelTitle->setText("Change passphrase");
    ui->labelTitle->setProperty("cssClass", "text-title-screen");

    ui->warningLabel->setProperty("cssClass", "text-subtitle");

    ui->btnEsc->setText("");
    ui->btnEsc->setProperty("cssClass", "ic-close");

    ui->pushButtonOk->setText("OK");
    ui->pushButtonOk->setProperty("cssClass", "btn-primary");

    initCssEditLine(ui->passEdit1);
    initCssEditLine(ui->passEdit2);
    initCssEditLine(ui->passEdit3);

    ui->passLabel1->setText("Current passphrase");
    ui->passLabel1->setProperty("cssClass", "text-title");

    ui->passLabel2->setText("New passphrase");
    ui->passLabel2->setProperty("cssClass", "text-title");

    ui->passLabel3->setText("Repeat passphrase");
    ui->passLabel3->setProperty("cssClass", "text-title");

    setCssProperty(ui->passWarningLabel, "text-warning-small");
    ui->passWarningLabel->setVisible(false);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    ui->passEdit1->setMinimumSize(ui->passEdit1->sizeHint());
    ui->passEdit2->setMinimumSize(ui->passEdit2->sizeHint());
    ui->passEdit3->setMinimumSize(ui->passEdit3->sizeHint());

    ui->passEdit1->setMaxLength(MAX_PASSPHRASE_SIZE);
    ui->passEdit2->setMaxLength(MAX_PASSPHRASE_SIZE);
    ui->passEdit3->setMaxLength(MAX_PASSPHRASE_SIZE);

<<<<<<< HEAD
=======
    setShadow(ui->layoutEdit);
    setShadow(ui->layoutEdit2);

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    // Setup Caps Lock detection.
    ui->passEdit1->installEventFilter(this);
    ui->passEdit2->installEventFilter(this);
    ui->passEdit3->installEventFilter(this);

    this->model = model;

<<<<<<< HEAD
=======
    QString title;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    switch (mode) {
    case Mode::Encrypt: // Ask passphrase x2
        ui->warningLabel->setText(tr("Enter the new passphrase to the wallet.<br/>Please use a passphrase of <b>ten or more random characters</b>, or <b>eight or more words</b>."));
        ui->passLabel1->hide();
        ui->passEdit1->hide();
<<<<<<< HEAD
        setWindowTitle(tr("Encrypt Wallet"));
        break;
        ui->anonymizationCheckBox->setChecked(false);
        ui->anonymizationCheckBox->hide();
    case Mode::UnlockAnonymize:
    case Mode::Unlock: // Ask passphrase
        ui->warningLabel->setText(tr("This operation needs your wallet passphrase to unlock the wallet.<br/><br/>(Wallet may appear not responding as it rescans for all transactions)<br/><br/>"));
        ui->warningLabel->setAlignment(Qt::AlignHCenter);
        ui->passLabel2->hide();
        ui->passEdit2->hide();
        ui->passLabel3->hide();
        ui->passEdit3->hide();
        setWindowTitle(tr("Unlock Wallet"));
=======
        ui->layoutEdit->hide();
        title = tr("Encrypt wallet");
        initWatch(ui->layoutEdit2);
        break;
    case Mode::UnlockAnonymize:
        ui->warningLabel->setText(tr("This operation needs your wallet passphrase to unlock the wallet."));
        ui->passLabel2->hide();
        ui->passEdit2->hide();
        ui->layoutEdit2->hide();
        ui->passLabel3->hide();
        ui->passEdit3->hide();
        title = tr("Unlock wallet\nfor staking");
        initWatch(ui->layoutEdit);
        break;
    case Mode::Unlock: // Ask passphrase
        ui->warningLabel->setText(tr("This operation needs your wallet passphrase to unlock the wallet."));
        ui->passLabel2->hide();
        ui->passEdit2->hide();
        ui->layoutEdit2->hide();
        ui->passLabel3->hide();
        ui->passEdit3->hide();
        title = tr("Unlock wallet");
        initWatch(ui->layoutEdit);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        break;
    case Mode::Decrypt: // Ask passphrase
        ui->warningLabel->setText(tr("This operation needs your wallet passphrase to decrypt the wallet."));
        ui->passLabel2->hide();
        ui->passEdit2->hide();
<<<<<<< HEAD
        ui->passLabel3->hide();
        ui->passEdit3->hide();
        setWindowTitle(tr("Decrypt Wallet"));
        break;
    case Mode::ChangePass: // Ask old passphrase + new passphrase x2
        setWindowTitle(tr("Change Passphrase"));
        ui->warningLabel->setText(tr("Enter the old and new passphrase to the wallet."));
        break;
    }

    // Set checkbox "For anonymization, automint, and staking only" depending on from where we were called
    if (context == Context::Unlock_Menu || context == Context::BIP_38) {
        ui->anonymizationCheckBox->setChecked(true);
    }
    else {
        ui->anonymizationCheckBox->setChecked(false);
    }

    // It doesn't make sense to show the checkbox for sending PRCY because you wouldn't check it anyway.
    if (context == Context::Send) {
        ui->anonymizationCheckBox->hide();
    }

    textChanged();
    connect(ui->passEdit1, SIGNAL(textChanged(QString)), this, SLOT(textChanged()));
    connect(ui->passEdit2, SIGNAL(textChanged(QString)), this, SLOT(textChanged()));
    connect(ui->passEdit3, SIGNAL(textChanged(QString)), this, SLOT(textChanged()));
=======
        ui->layoutEdit2->hide();
        ui->passLabel3->hide();
        ui->passEdit3->hide();
        title = tr("Decrypt wallet");
        initWatch(ui->layoutEdit);
        break;
    case Mode::ChangePass: // Ask old passphrase + new passphrase x2
        title = tr("Change passphrase");
        ui->warningLabel->setText(tr("Enter the old and new passphrase to the wallet."));
        initWatch(ui->layoutEdit);
        break;
    }

    ui->labelTitle->setText(title);

    textChanged();
    connect(btnWatch, &QCheckBox::clicked, this, &AskPassphraseDialog::onWatchClicked);
    connect(ui->passEdit1, &QLineEdit::textChanged, this, &AskPassphraseDialog::textChanged);
    connect(ui->passEdit2, &QLineEdit::textChanged, this, &AskPassphraseDialog::textChanged);
    connect(ui->passEdit3, &QLineEdit::textChanged, this, &AskPassphraseDialog::textChanged);
    connect(ui->pushButtonOk, &QPushButton::clicked, this, &AskPassphraseDialog::accept);
    connect(ui->btnEsc, &QPushButton::clicked, this, &AskPassphraseDialog::close);
}

void AskPassphraseDialog::onWatchClicked()
{
    int state = btnWatch->checkState();
    ui->passEdit3->setEchoMode(state == Qt::Checked ? QLineEdit::Normal : QLineEdit::Password );
    ui->passEdit2->setEchoMode(state== Qt::Checked ? QLineEdit::Normal : QLineEdit::Password );
    ui->passEdit1->setEchoMode(state == Qt::Checked ? QLineEdit::Normal : QLineEdit::Password );
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

AskPassphraseDialog::~AskPassphraseDialog()
{
    // Attempt to overwrite text so that they do not linger around in memory
    ui->passEdit1->setText(QString(" ").repeated(ui->passEdit1->text().size()));
    ui->passEdit2->setText(QString(" ").repeated(ui->passEdit2->text().size()));
    ui->passEdit3->setText(QString(" ").repeated(ui->passEdit3->text().size()));
    delete ui;
}

<<<<<<< HEAD
=======
void AskPassphraseDialog::showEvent(QShowEvent *event)
{
    if (mode == Mode::Encrypt && ui->passEdit2) ui->passEdit2->setFocus();
    else if (ui->passEdit1) ui->passEdit1->setFocus();
}

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
void AskPassphraseDialog::accept()
{
    SecureString oldpass, newpass1, newpass2;
    if (!model)
        return;
    oldpass.reserve(MAX_PASSPHRASE_SIZE);
    newpass1.reserve(MAX_PASSPHRASE_SIZE);
    newpass2.reserve(MAX_PASSPHRASE_SIZE);
    // TODO: get rid of this .c_str() by implementing SecureString::operator=(std::string)
    // Alternately, find a way to make this input mlock()'d to begin with.
    oldpass.assign(ui->passEdit1->text().toStdString().c_str());
    newpass1.assign(ui->passEdit2->text().toStdString().c_str());
    newpass2.assign(ui->passEdit3->text().toStdString().c_str());

    switch (mode) {
    case Mode::Encrypt: {
        if (newpass1.empty() || newpass2.empty()) {
            // Cannot encrypt with empty passphrase
            break;
        }
<<<<<<< HEAD
        QMessageBox::StandardButton retval = QMessageBox::question(this, tr("Confirm Wallet Encryption"),
            tr("Warning: If you encrypt your wallet and lose your passphrase, you will <b>LOSE ALL OF YOUR PRCY</b>!") + "<br><br>" + tr("Are you sure you wish to encrypt your wallet?"),
            QMessageBox::Yes | QMessageBox::Cancel,
            QMessageBox::Cancel);
        if (retval == QMessageBox::Yes) {
            if (newpass1 == newpass2) {
                if (model->setWalletEncrypted(true, newpass1)) {
                    QMessageBox::warning(this, tr("Wallet Encrypted"),
                        "<qt>" +
                            tr("PRCY will close now to finish the encryption process. "
                               "Remember that encrypting your wallet cannot fully protect "
                               "your PRCYs from being stolen by malware infecting your computer.") +
                            "<br><br><b>" +
                            tr("IMPORTANT: Any previous backups you have made of your wallet file "
                               "should be replaced with the newly generated, encrypted wallet file. "
                               "For security reasons, previous backups of the unencrypted wallet file "
                               "will become useless as soon as you start using the new, encrypted wallet.") +
                            "</b></qt>");
                    QApplication::quit();
                } else {
                    QMessageBox msgBox;
                    msgBox.setWindowTitle("Wallet Encryption Failed");
                    msgBox.setText("Wallet encryption failed due to an internal error. Your wallet was not encrypted. Please try again.");
                    msgBox.setStyleSheet(GUIUtil::loadStyleSheet());
                    msgBox.setIcon(QMessageBox::Critical);
                    msgBox.exec();
                }
                QDialog::accept(); // Success
            } else {
                QMessageBox msgBox;
                msgBox.setWindowTitle("Wallet Encryption Failed");
                msgBox.setText("The supplied passphrases do not match. Please try again.");
                msgBox.setStyleSheet(GUIUtil::loadStyleSheet());
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.exec();
            }
=======
        hide();
        bool ret = openStandardDialog(
                tr("Confirm wallet encryption"),
                "<b>" + tr("WARNING") + ":</b> " + tr("If you encrypt your wallet and lose your passphrase, you will") +
                " <b>" + tr("LOSE ALL OF YOUR COINS") + "</b>!<br><br>" + tr("Are you sure you wish to encrypt your wallet?"),
                tr("ENCRYPT"), tr("CANCEL")
        );
        if (ret) {
            newpassCache = newpass1;
            PIVXGUI* window = static_cast<PIVXGUI*>(parentWidget());
            LoadingDialog *dialog = new LoadingDialog(window);
            dialog->execute(this, 1);
            openDialogWithOpaqueBackgroundFullScreen(dialog, window);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        } else {
            QDialog::reject(); // Cancelled
        }
    } break;
    case Mode::UnlockAnonymize:
<<<<<<< HEAD
    case Mode::Unlock:
        if (!model->setWalletLocked(false, oldpass, ui->anonymizationCheckBox->isChecked())) {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Wallet Unlock Failed");
            msgBox.setText("The passphrase entered for the wallet unlock was incorrect. Please try again.");
            msgBox.setStyleSheet(GUIUtil::loadStyleSheet());
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
=======
        if (!model->setWalletLocked(false, oldpass, true)) {
            QMessageBox::critical(this, tr("Wallet unlock failed"),
                                  tr("The passphrase entered for the wallet decryption was incorrect."));
        } else {
            QDialog::accept(); // Success
        }
        break;
    case Mode::Unlock:
        if (!model->setWalletLocked(false, oldpass, false)) {
            QMessageBox::critical(this, tr("Wallet unlock failed"),
                tr("The passphrase entered for the wallet decryption was incorrect."));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        } else {
            QDialog::accept(); // Success
        }
        break;
    case Mode::Decrypt:
        if (!model->setWalletEncrypted(false, oldpass)) {
<<<<<<< HEAD
            QMessageBox msgBox;
            msgBox.setWindowTitle("Wallet Decryption Failed");
            msgBox.setText("The passphrase entered for the wallet decryption was incorrect. Please try again.");
            msgBox.setStyleSheet(GUIUtil::loadStyleSheet());
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
=======
            QMessageBox::critical(this, tr("Wallet decryption failed"),
                tr("The passphrase entered for the wallet decryption was incorrect."));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        } else {
            QDialog::accept(); // Success
        }
        break;
    case Mode::ChangePass:
        if (newpass1 == newpass2) {
            if (model->changePassphrase(oldpass, newpass1)) {
<<<<<<< HEAD
                QMessageBox msgBox;
                msgBox.setWindowTitle("Passphrase Change Successful");
                msgBox.setText("Wallet passphrase was successfully changed.\nPlease remember your passphrase as there is no way to recover it.");
                msgBox.setStyleSheet(GUIUtil::loadStyleSheet());
                msgBox.setIcon(QMessageBox::Information);
                msgBox.exec();
                QDialog::accept(); // Success
            } else {
                QMessageBox msgBox;
                msgBox.setWindowTitle("Wallet Encryption Failed");
                msgBox.setText("The passphrase entered for the wallet decryption was incorrect. Please try again.");
                msgBox.setStyleSheet(GUIUtil::loadStyleSheet());
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.exec();
            }
        } else {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Wallet Encryption Failed");
            msgBox.setText("The supplied passphrases do not match. Please try again.");
            msgBox.setStyleSheet(GUIUtil::loadStyleSheet());
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
=======
                hide();
                openStandardDialog(tr("Wallet encrypted"),tr("Wallet passphrase was successfully changed."));
                QDialog::accept(); // Success
            } else {
                QMessageBox::critical(this, tr("Wallet encryption failed"),
                    tr("The passphrase entered for the wallet decryption was incorrect."));
            }
        } else {
            QMessageBox::critical(this, tr("Wallet encryption failed"),
                tr("The supplied passphrases do not match."));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        }
        break;
    }
}

void AskPassphraseDialog::textChanged()
{
    // Validate input, set Ok button to enabled when acceptable
    bool acceptable = false;
    switch (mode) {
    case Mode::Encrypt: // New passphrase x2
<<<<<<< HEAD
        acceptable = !ui->passEdit2->text().isEmpty() && !ui->passEdit3->text().isEmpty();
=======
        acceptable = !ui->passEdit2->text().isEmpty() && !ui->passEdit3->text().isEmpty() && // Passphrases are not empty
                     ui->passEdit2->text() == ui->passEdit3->text();                         // Passphrases match eachother
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        break;
    case Mode::UnlockAnonymize: // Old passphrase x1
    case Mode::Unlock:          // Old passphrase x1
    case Mode::Decrypt:
        acceptable = !ui->passEdit1->text().isEmpty();
        break;
    case Mode::ChangePass: // Old passphrase x1, new passphrase x2
<<<<<<< HEAD
        acceptable = !ui->passEdit1->text().isEmpty() && !ui->passEdit2->text().isEmpty() && !ui->passEdit3->text().isEmpty();
        break;
    }
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(acceptable);
=======
        acceptable = !ui->passEdit2->text().isEmpty() && !ui->passEdit3->text().isEmpty() && // New passphrases are not empty
                     ui->passEdit2->text() == ui->passEdit3->text() &&                       // New passphrases match eachother
                     !ui->passEdit1->text().isEmpty();                                       // Old passphrase is not empty
        break;
    }
    ui->pushButtonOk->setEnabled(acceptable);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

bool AskPassphraseDialog::event(QEvent* event)
{
<<<<<<< HEAD
    // Detect Caps Lock key press.
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* ke = static_cast<QKeyEvent*>(event);
        if (ke->key() == Qt::Key_CapsLock) {
            fCapsLock = !fCapsLock;
        }
        if (fCapsLock) {
            ui->capsLabel->setText(tr("Warning: The Caps Lock key is on!"));
        } else {
            ui->capsLabel->clear();
        }
    }
    return QWidget::event(event);
=======
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* ke = static_cast<QKeyEvent*>(event);
        // Detect Caps Lock key press.
        if (ke->key() == Qt::Key_CapsLock) {
            fCapsLock = !fCapsLock;
        }

        updateWarningsLabel();

        // Detect Enter key press
        if ((ke->key() == Qt::Key_Enter || ke->key() == Qt::Key_Return) && ui->pushButtonOk->isEnabled()) {
            accept();
        }
    }
    return QDialog::event(event);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

bool AskPassphraseDialog::eventFilter(QObject* object, QEvent* event)
{
    /* Detect Caps Lock.
     * There is no good OS-independent way to check a key state in Qt, but we
     * can detect Caps Lock by checking for the following condition:
     * Shift key is down and the result is a lower case character, or
     * Shift key is not down and the result is an upper case character.
     */
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* ke = static_cast<QKeyEvent*>(event);
        QString str = ke->text();
        if (str.length() != 0) {
            const QChar* psz = str.unicode();
            bool fShift = (ke->modifiers() & Qt::ShiftModifier) != 0;
            if ((fShift && *psz >= 'a' && *psz <= 'z') || (!fShift && *psz >= 'A' && *psz <= 'Z')) {
                fCapsLock = true;
<<<<<<< HEAD
                ui->capsLabel->setText(tr("Warning: The Caps Lock key is on!"));
            } else if (psz->isLetter()) {
                fCapsLock = false;
                ui->capsLabel->clear();
            }
        }
    }
    return QDialog::eventFilter(object, event);
}
=======
            } else if (psz->isLetter()) {
                fCapsLock = false;
            }
        }
    }
    updateWarningsLabel();

    return QDialog::eventFilter(object, event);
}

bool AskPassphraseDialog::openStandardDialog(QString title, QString body, QString okBtn, QString cancelBtn)
{
    PIVXGUI* gui = static_cast<PIVXGUI*>(parentWidget());
    DefaultDialog *confirmDialog = new DefaultDialog(gui);
    confirmDialog->setText(title, body, okBtn, cancelBtn);
    confirmDialog->adjustSize();
    openDialogWithOpaqueBackground(confirmDialog, gui);
    bool ret = confirmDialog->isOk;
    confirmDialog->deleteLater();
    return ret;
}

void AskPassphraseDialog::updateWarningsLabel()
{
    // Merge warning labels together if there's two warnings
    bool validPassphrases = false;
    validPassphrases = ui->passEdit2->text() == ui->passEdit3->text();
    QString warningStr = "";
    if (fCapsLock || !validPassphrases) warningStr += tr("WARNING:") + "<br>";
    if (fCapsLock) warningStr += "* " + tr("The caps lock key is on!");
    if (fCapsLock && !validPassphrases) warningStr += "<br>";
    if (!validPassphrases) warningStr += "* " + tr("Passphrases do not match!");

    if (warningStr.isEmpty()) {
        ui->passWarningLabel->clear();
        ui->passWarningLabel->setVisible(false);
    } else {
        ui->passWarningLabel->setText(warningStr);
        ui->passWarningLabel->setVisible(true);
    }
}

void AskPassphraseDialog::warningMessage()
{
    hide();
    static_cast<PIVXGUI*>(parentWidget())->showHide(true);
    openStandardDialog(
            tr("Wallet encrypted"),
            "<qt>" +
            tr("%1 will close now to finish the encryption process. "
               "Remember that encrypting your wallet cannot fully protect "
               "your PIVs from being stolen by malware infecting your computer.").arg(PACKAGE_NAME) +
            "<br><br><b>" +
            tr("IMPORTANT: Any previous backups you have made of your wallet file "
               "should be replaced with the newly generated, encrypted wallet file. "
               "For security reasons, previous backups of the unencrypted wallet file "
               "will become useless as soon as you start using the new, encrypted wallet.") +
            "</b></qt>",
            tr("OK")
            );
    QApplication::quit();
}

void AskPassphraseDialog::errorEncryptingWallet()
{
    QMessageBox::critical(this, tr("Wallet encryption failed"),
                          tr("Wallet encryption failed due to an internal error. Your wallet was not encrypted."));
}

void AskPassphraseDialog::run(int type)
{
    if (type == 1) {
        if (!newpassCache.empty()) {
            QMetaObject::invokeMethod(this, "hide", Qt::QueuedConnection);
            if (model->setWalletEncrypted(true, newpassCache)) {
                QMetaObject::invokeMethod(this, "warningMessage", Qt::QueuedConnection);
            } else {
                QMetaObject::invokeMethod(this, "errorEncryptingWallet", Qt::QueuedConnection);
            }
            newpassCache.clear();
            QDialog::accept(); // Success
        }
    }
}
void AskPassphraseDialog::onError(QString error, int type)
{
    newpassCache.clear();
    LogPrintf("Error encrypting wallet, %s\n", error.toStdString());
    QMetaObject::invokeMethod(this, "errorEncryptingWallet", Qt::QueuedConnection);
}

void AskPassphraseDialog::initWatch(QWidget *parent)
{
    btnWatch = new QCheckBox(parent);
    setCssProperty(btnWatch, "btn-watch-password");
    btnWatch->setChecked(false);
    QSize BUTTON_CONTACT_SIZE = QSize(24, 24);
    btnWatch->setMinimumSize(BUTTON_CONTACT_SIZE);
    btnWatch->setMaximumSize(BUTTON_CONTACT_SIZE);
    btnWatch->show();
    btnWatch->raise();

    int posYY = 8;
    btnWatch->move(450, posYY);
}
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
