// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
<<<<<<< HEAD
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
=======
// Copyright (c) 2015-2019 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#if defined(HAVE_CONFIG_H)
<<<<<<< HEAD
#include "config/prcycoin-config.h"
#endif

#include "util.h"
#include "uritests.h"

#ifdef ENABLE_WALLET
#include "paymentservertests.h"
#endif

=======
#include "config/pivx-config.h"
#endif

#include "util/system.h"
#include "uritests.h"

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#include <QCoreApplication>
#include <QObject>
#include <QTest>

<<<<<<< HEAD
#include <openssl/ssl.h>

=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#if defined(QT_STATICPLUGIN)
#include <QtPlugin>
#if defined(QT_QPA_PLATFORM_MINIMAL)
Q_IMPORT_PLUGIN(QMinimalIntegrationPlugin);
#endif
#if defined(QT_QPA_PLATFORM_XCB)
Q_IMPORT_PLUGIN(QXcbIntegrationPlugin);
#elif defined(QT_QPA_PLATFORM_WINDOWS)
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);
#elif defined(QT_QPA_PLATFORM_COCOA)
Q_IMPORT_PLUGIN(QCocoaIntegrationPlugin);
#endif
#endif

extern void noui_connect();

// This is all you need to run all the tests
int main(int argc, char *argv[])
{
    SetupEnvironment();
    bool fInvalid = false;

    // Don't remove this, it's needed to access
    // QCoreApplication:: in the tests
    QCoreApplication app(argc, argv);
<<<<<<< HEAD
    app.setApplicationName("Prcycoin-Qt-test");

    SSL_library_init();
=======
    app.setApplicationName("Pivx-Qt-test");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    URITests test1;
    if (QTest::qExec(&test1) != 0)
        fInvalid = true;
<<<<<<< HEAD
#ifdef ENABLE_WALLET
    PaymentServerTests test2;
    if (QTest::qExec(&test2) != 0)
        fInvalid = true;
#endif
=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    return fInvalid;
}
