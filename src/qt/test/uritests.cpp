// Copyright (c) 2009-2014 The Bitcoin developers
<<<<<<< HEAD
=======
// Copyright (c) 2017-2019 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "uritests.h"

#include "guiutil.h"
#include "walletmodel.h"

#include <QUrl>

void URITests::uriTests()
{
    SendCoinsRecipient rv;
    QUrl uri;
<<<<<<< HEAD
    uri.setUrl(QString("prcycoin:D72dLgywmL73JyTwQBfuU29CADz9yCJ99v?req-dontexist="));
    QVERIFY(!GUIUtil::parseBitcoinURI(uri, &rv));

    uri.setUrl(QString("prcycoin:D72dLgywmL73JyTwQBfuU29CADz9yCJ99v?dontexist="));
=======
    uri.setUrl(QString("pivx:D72dLgywmL73JyTwQBfuU29CADz9yCJ99v?req-dontexist="));
    QVERIFY(!GUIUtil::parseBitcoinURI(uri, &rv));

    uri.setUrl(QString("pivx:D72dLgywmL73JyTwQBfuU29CADz9yCJ99v?dontexist="));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("D72dLgywmL73JyTwQBfuU29CADz9yCJ99v"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 0);

<<<<<<< HEAD
    uri.setUrl(QString("prcycoin:D72dLgywmL73JyTwQBfuU29CADz9yCJ99v?label=Some Example Address"));
=======
    uri.setUrl(QString("pivx:D72dLgywmL73JyTwQBfuU29CADz9yCJ99v?label=Some Example Address"));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("D72dLgywmL73JyTwQBfuU29CADz9yCJ99v"));
    QVERIFY(rv.label == QString("Some Example Address"));
    QVERIFY(rv.amount == 0);

<<<<<<< HEAD
    uri.setUrl(QString("prcycoin:D72dLgywmL73JyTwQBfuU29CADz9yCJ99v?amount=0.001"));
=======
    uri.setUrl(QString("pivx:D72dLgywmL73JyTwQBfuU29CADz9yCJ99v?amount=0.001"));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("D72dLgywmL73JyTwQBfuU29CADz9yCJ99v"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 100000);

<<<<<<< HEAD
    uri.setUrl(QString("prcycoin:D72dLgywmL73JyTwQBfuU29CADz9yCJ99v?amount=1.001"));
=======
    uri.setUrl(QString("pivx:D72dLgywmL73JyTwQBfuU29CADz9yCJ99v?amount=1.001"));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("D72dLgywmL73JyTwQBfuU29CADz9yCJ99v"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 100100000);

<<<<<<< HEAD
    uri.setUrl(QString("prcycoin:D72dLgywmL73JyTwQBfuU29CADz9yCJ99v?amount=100&label=Some Example"));
=======
    uri.setUrl(QString("pivx:D72dLgywmL73JyTwQBfuU29CADz9yCJ99v?amount=100&label=Some Example"));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("D72dLgywmL73JyTwQBfuU29CADz9yCJ99v"));
    QVERIFY(rv.amount == 10000000000LL);
    QVERIFY(rv.label == QString("Some Example"));

<<<<<<< HEAD
    uri.setUrl(QString("prcycoin:D72dLgywmL73JyTwQBfuU29CADz9yCJ99v?message=Some Example Address"));
=======
    uri.setUrl(QString("pivx:D72dLgywmL73JyTwQBfuU29CADz9yCJ99v?message=Some Example Address"));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("D72dLgywmL73JyTwQBfuU29CADz9yCJ99v"));
    QVERIFY(rv.label == QString());

<<<<<<< HEAD
    QVERIFY(GUIUtil::parseBitcoinURI("prcycoin://D72dLgywmL73JyTwQBfuU29CADz9yCJ99v?message=Some Example Address", &rv));
    QVERIFY(rv.address == QString("D72dLgywmL73JyTwQBfuU29CADz9yCJ99v"));
    QVERIFY(rv.label == QString());

    uri.setUrl(QString("prcycoin:D72dLgywmL73JyTwQBfuU29CADz9yCJ99v?req-message=Some Example Address"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));

    uri.setUrl(QString("prcycoin:D72dLgywmL73JyTwQBfuU29CADz9yCJ99v?amount=1,000&label=Some Example"));
    QVERIFY(!GUIUtil::parseBitcoinURI(uri, &rv));

    uri.setUrl(QString("prcycoin:D72dLgywmL73JyTwQBfuU29CADz9yCJ99v?amount=1,000.0&label=Some Example"));
=======
    QVERIFY(GUIUtil::parseBitcoinURI("pivx://D72dLgywmL73JyTwQBfuU29CADz9yCJ99v?message=Some Example Address", &rv));
    QVERIFY(rv.address == QString("D72dLgywmL73JyTwQBfuU29CADz9yCJ99v"));
    QVERIFY(rv.label == QString());

    uri.setUrl(QString("pivx:D72dLgywmL73JyTwQBfuU29CADz9yCJ99v?req-message=Some Example Address"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));

    uri.setUrl(QString("pivx:D72dLgywmL73JyTwQBfuU29CADz9yCJ99v?amount=1,000&label=Some Example"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));

    uri.setUrl(QString("pivx:D72dLgywmL73JyTwQBfuU29CADz9yCJ99v?amount=1,000.0&label=Some Example"));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    QVERIFY(!GUIUtil::parseBitcoinURI(uri, &rv));
}
