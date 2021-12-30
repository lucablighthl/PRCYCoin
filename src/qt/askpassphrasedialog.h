// Copyright (c) 2011-2013 The Bitcoin developers
<<<<<<< HEAD
=======
// Copyright (c) 2017-2019 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_ASKPASSPHRASEDIALOG_H
#define BITCOIN_QT_ASKPASSPHRASEDIALOG_H

#include <QDialog>
<<<<<<< HEAD

class WalletModel;
=======
#include "qt/pivx/prunnable.h"
#include "support/allocators/secure.h"
#include <QCheckBox>

class WalletModel;
class PIVXGUI;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

namespace Ui
{
class AskPassphraseDialog;
<<<<<<< HEAD
=======
class QCheckBox;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

/** Multifunctional dialog to ask for passphrases. Used for encryption, unlocking, and changing the passphrase.
 */
<<<<<<< HEAD
class AskPassphraseDialog : public QDialog
=======
class AskPassphraseDialog : public QDialog, public Runnable
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    Q_OBJECT

public:
    enum class Mode {
        Encrypt,         /**< Ask passphrase twice and encrypt */
        UnlockAnonymize, /**< Ask passphrase and unlock only for anonymization */
        Unlock,          /**< Ask passphrase and unlock */
        ChangePass,      /**< Ask old passphrase + new passphrase twice */
        Decrypt          /**< Ask passphrase and decrypt wallet */
    };

<<<<<<< HEAD
     // Context from where / for what the passphrase dialog was called to set the status of the checkbox
     // Partly redundant to Mode above, but offers more flexibility for future enhancements
     enum class Context {
         Unlock_Menu,    /** Unlock wallet from menu     */
         Unlock_Full,    /** Wallet needs to be fully unlocked */
         Encrypt,        /** Encrypt unencrypted wallet */
         ToggleLock,     /** Toggle wallet lock state */
         ChangePass,     /** Change passphrase */
         Send,           /** Send PRCY */
         BIP_38,         /** BIP38 menu */
         Multi_Sig,      /** Multi-Signature dialog */
         Sign_Message    /** Sign/verify message dialog */
     };

     explicit AskPassphraseDialog(Mode mode, QWidget* parent, WalletModel* model, Context context);
    ~AskPassphraseDialog();

    void accept();
=======
    // Context from where / for what the passphrase dialog was called to set the status of the checkbox
    // Partly redundant to Mode above, but offers more flexibility for future enhancements
    enum class Context {
        Unlock_Menu,    /** Unlock wallet from menu     */
        Unlock_Full,    /** Wallet needs to be fully unlocked */
        Encrypt,        /** Encrypt unencrypted wallet */
        ToggleLock,     /** Toggle wallet lock state */
        ChangePass,     /** Change passphrase */
        Send_PIV,       /** Send PIV */
        BIP_38,         /** BIP38 menu */
        Multi_Sig,      /** Multi-Signature dialog */
        Sign_Message,   /** Sign/verify message dialog */
        UI_Vote,        /** Governance Tab UI Voting */
    };

    explicit AskPassphraseDialog(Mode mode, QWidget* parent, WalletModel* model, Context context);
    ~AskPassphraseDialog();

    void showEvent(QShowEvent *event) override;
    void accept() override;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

private:
    Ui::AskPassphraseDialog* ui;
    Mode mode;
    WalletModel* model;
    Context context;
    bool fCapsLock;
<<<<<<< HEAD

private Q_SLOTS:
    void textChanged();

protected:
    bool event(QEvent* event);
    bool eventFilter(QObject* object, QEvent* event);
=======
    SecureString newpassCache = "";

    void updateWarningsLabel();
    void run(int type) override;
    void onError(QString error, int type) override;
    QCheckBox *btnWatch;

    void initWatch(QWidget *parent);

private Q_SLOTS:
    void onWatchClicked();
    void textChanged();
    void warningMessage();
    void errorEncryptingWallet();
    bool openStandardDialog(QString title = "", QString body = "", QString okBtn = "OK", QString cancelBtn = "");

protected:
    bool event(QEvent* event) override ;
    bool eventFilter(QObject* object, QEvent* event) override;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
};

#endif // BITCOIN_QT_ASKPASSPHRASEDIALOG_H
