<<<<<<< HEAD
// Copyright (c) 2011-2013 The Bitcoin developers
=======
// Copyright (c) 2011-2020 The Bitcoin developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_MACDOCKICONHANDLER_H
#define BITCOIN_QT_MACDOCKICONHANDLER_H

#include <QMainWindow>
#include <QObject>

<<<<<<< HEAD
QT_BEGIN_NAMESPACE
class QIcon;
class QMenu;
class QWidget;
QT_END_NAMESPACE

=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
/** Macintosh-specific dock icon handler.
 */
class MacDockIconHandler : public QObject
{
    Q_OBJECT

public:
<<<<<<< HEAD
    ~MacDockIconHandler();

    QMenu* dockMenu();
    void setIcon(const QIcon& icon);
    void setMainWindow(QMainWindow* window);
    static MacDockIconHandler* instance();
    static void cleanup();
    void handleDockIconClickEvent();
=======
    static MacDockIconHandler* instance();
    static void cleanup();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

Q_SIGNALS:
    void dockIconClicked();

private:
    MacDockIconHandler();
<<<<<<< HEAD

    QWidget* m_dummyWidget;
    QMenu* m_dockMenu;
    QMainWindow* mainWindow;
=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
};

#endif // BITCOIN_QT_MACDOCKICONHANDLER_H
