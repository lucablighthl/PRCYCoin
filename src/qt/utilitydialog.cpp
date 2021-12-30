// Copyright (c) 2011-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
<<<<<<< HEAD
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

=======
// Copyright (c) 2015-2020 The PIVX developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#if defined(HAVE_CONFIG_H)
#include "config/pivx-config.h"
#endif

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#include "utilitydialog.h"

#include "ui_helpmessagedialog.h"

<<<<<<< HEAD
#include "bitcoingui.h"
#include "clientmodel.h"
#include "guiconstants.h"
#include "intro.h"
#include "guiutil.h"

#include "clientversion.h"
#include "init.h"
#include "util.h"
=======
#include "clientmodel.h"
#include "clientversion.h"
#include "guiconstants.h"
#include "init.h"
#include "intro.h"
#include "guiutil.h"
#include "qt/pivx/qtutils.h"
#include "util/system.h"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

#include <stdio.h>

#include <QCloseEvent>
#include <QLabel>
#include <QRegExp>
#include <QTextTable>
#include <QTextCursor>
#include <QVBoxLayout>

/** "Help message" or "About" dialog box */
HelpMessageDialog::HelpMessageDialog(QWidget* parent, bool about) : QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
                                                                    ui(new Ui::HelpMessageDialog)
{
    ui->setupUi(this);
<<<<<<< HEAD
    GUIUtil::restoreWindowGeometry("nHelpMessageDialogWindow", this->size(), this);

    QString version = tr("PRCY") + " " + tr("version") + " " + QString::fromStdString(FormatFullVersion());
=======
    if (parent) this->setStyleSheet(parent->styleSheet());
    GUIUtil::restoreWindowGeometry("nHelpMessageDialogWindow", this->size(), this);

    QString version = QString{PACKAGE_NAME} + " " + tr("version") + " " + QString::fromStdString(FormatFullVersion());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
/* On x86 add a bit specifier to the version so that users can distinguish between
     * 32 and 64 bit builds. On other architectures, 32/64 bit may be more ambigious.
     */
#if defined(__x86_64__)
    version += " " + tr("(%1-bit)").arg(64);
#elif defined(__i386__)
    version += " " + tr("(%1-bit)").arg(32);
#endif

<<<<<<< HEAD
    if (about) {
        setWindowTitle(tr("About PRCY"));
=======
    setCssBtnPrimary(ui->pushButtonOk);
    connect(ui->pushButtonOk, &QPushButton::clicked, this, &HelpMessageDialog::close);
    if (about) {
        setWindowTitle(tr("About %1").arg(PACKAGE_NAME));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

        /// HTML-format the license message from the core
        QString licenseInfo = QString::fromStdString(LicenseInfo());
        QString licenseInfoHTML = licenseInfo;

        // Make URLs clickable
        QRegExp uri("<(.*)>", Qt::CaseSensitive, QRegExp::RegExp2);
        uri.setMinimal(true); // use non-greedy matching
<<<<<<< HEAD
        licenseInfoHTML.replace(uri, "<a href=\"\\1\">\\1</a>");
=======
        licenseInfoHTML.replace(uri, "<a style='color: #b088ff;text-decoration:none'  href=\"\\1\">\\1</a>");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        // Replace newlines with HTML breaks
        licenseInfoHTML.replace("\n\n", "<br><br>");

        ui->aboutMessage->setTextFormat(Qt::RichText);
        ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        text = version + "\n" + licenseInfo;
        ui->aboutMessage->setText(version + "<br><br>" + licenseInfoHTML);
        ui->aboutMessage->setWordWrap(true);
        ui->helpMessage->setVisible(false);
    } else {
        setWindowTitle(tr("Command-line options"));
<<<<<<< HEAD
        QString header = tr("Usage:") + "\n" +
                         "  prcycoin-qt [" + tr("command-line options") + "]                     " + "\n";
=======
        QString header = "Usage:  pivx-qt [command-line options]                     \n";
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        QTextCursor cursor(ui->helpMessage->document());
        cursor.insertText(version);
        cursor.insertBlock();
        cursor.insertText(header);
        cursor.insertBlock();

        std::string strUsage = HelpMessage(HMM_BITCOIN_QT);
<<<<<<< HEAD
        strUsage += HelpMessageGroup(tr("UI Options:").toStdString());
        strUsage += HelpMessageOpt("-choosedatadir", strprintf(tr("Choose data directory on startup (default: %u)").toStdString(), DEFAULT_CHOOSE_DATADIR));
        strUsage += HelpMessageOpt("-lang=<lang>", tr("Set language, for example \"de_DE\" (default: system locale)").toStdString());
        strUsage += HelpMessageOpt("-min", tr("Start minimized").toStdString());
        strUsage += HelpMessageOpt("-rootcertificates=<file>", tr("Set SSL root certificates for payment request (default: -system-)").toStdString());
        strUsage += HelpMessageOpt("-splash", strprintf(tr("Show splash screen on startup (default: %u)").toStdString(), DEFAULT_SPLASHSCREEN));
        QString coreOptions = QString::fromStdString(strUsage);
        text = version + "\n" + header + "\n" + coreOptions;
=======
        strUsage += HelpMessageGroup("UI Options:");
        strUsage += HelpMessageOpt("-choosedatadir", strprintf("Choose data directory on startup (default: %u)", DEFAULT_CHOOSE_DATADIR));
        strUsage += HelpMessageOpt("-lang=<lang>", "Set language, for example \"de_DE\" (default: system locale)");
        strUsage += HelpMessageOpt("-min", "Start minimized");
        strUsage += HelpMessageOpt("-splash", strprintf("Show splash screen on startup (default: %u)", DEFAULT_SPLASHSCREEN));
        strUsage += HelpMessageOpt("-hidecharts", strprintf("Hide QT staking charts on startup (default: %u)", false));
        QString coreOptions = QString::fromStdString(strUsage);
        text = version + "\n\n" + header + "\n" + coreOptions;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

        QTextTableFormat tf;
        tf.setBorderStyle(QTextFrameFormat::BorderStyle_None);
        tf.setCellPadding(2);
        QVector<QTextLength> widths;
        widths << QTextLength(QTextLength::PercentageLength, 35);
        widths << QTextLength(QTextLength::PercentageLength, 65);
        tf.setColumnWidthConstraints(widths);

        QTextCharFormat bold;
        bold.setFontWeight(QFont::Bold);

<<<<<<< HEAD
        Q_FOREACH (const QString &line, coreOptions.split("\n")) {
=======
        for (const QString &line : coreOptions.split("\n")) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            if (line.startsWith("  -"))
            {
                cursor.currentTable()->appendRows(1);
                cursor.movePosition(QTextCursor::PreviousCell);
                cursor.movePosition(QTextCursor::NextRow);
                cursor.insertText(line.trimmed());
                cursor.movePosition(QTextCursor::NextCell);
            } else if (line.startsWith("   ")) {
                cursor.insertText(line.trimmed()+' ');
            } else if (line.size() > 0) {
                //Title of a group
                if (cursor.currentTable())
                    cursor.currentTable()->appendRows(1);
                cursor.movePosition(QTextCursor::Down);
                cursor.insertText(line.trimmed(), bold);
                cursor.insertTable(1, 2, tf);
            }
        }

        ui->helpMessage->moveCursor(QTextCursor::Start);
        ui->scrollArea->setVisible(false);
    }
}

HelpMessageDialog::~HelpMessageDialog()
{
    GUIUtil::saveWindowGeometry("nHelpMessageDialogWindow", this);
    delete ui;
}

void HelpMessageDialog::printToConsole()
{
    // On other operating systems, the expected action is to print the message to the console.
    fprintf(stdout, "%s\n", qPrintable(text));
}

void HelpMessageDialog::showOrPrint()
{
#if defined(WIN32)
    // On Windows, show a message box, as there is no stderr/stdout in windowed applications
    exec();
#else
    // On other operating systems, print help text to console
    printToConsole();
#endif
}

<<<<<<< HEAD
void HelpMessageDialog::on_okButton_accepted()
{
    close();
}

=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

/** "Shutdown" window */
ShutdownWindow::ShutdownWindow(QWidget* parent, Qt::WindowFlags f) : QWidget(parent, f)
{
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(new QLabel(
<<<<<<< HEAD
        tr("PRCY is shutting down...") + "<br /><br />" +
=======
        tr("%1 is shutting down...").arg(PACKAGE_NAME) + "<br /><br />" +
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        tr("Do not shut down the computer until this window disappears.")));
    setLayout(layout);
}

<<<<<<< HEAD
void ShutdownWindow::showShutdownWindow(BitcoinGUI* window)
=======
void ShutdownWindow::showShutdownWindow(QMainWindow* window)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    if (!window)
        return;

    // Show a simple window indicating shutdown status
    QWidget* shutdownWindow = new ShutdownWindow();
    // We don't hold a direct pointer to the shutdown window after creation, so use
    // Qt::WA_DeleteOnClose to make sure that the window will be deleted eventually.
    shutdownWindow->setAttribute(Qt::WA_DeleteOnClose);
    shutdownWindow->setWindowTitle(window->windowTitle());

    // Center shutdown window at where main window was
    const QPoint global = window->mapToGlobal(window->rect().center());
    shutdownWindow->move(global.x() - shutdownWindow->width() / 2, global.y() - shutdownWindow->height() / 2);
    shutdownWindow->show();
}

void ShutdownWindow::closeEvent(QCloseEvent* event)
{
    event->ignore();
}
