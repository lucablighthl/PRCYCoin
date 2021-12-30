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

#ifndef BITCOIN_QT_CLIENTMODEL_H
#define BITCOIN_QT_CLIENTMODEL_H

#include "uint256.h"
#include "chain.h"
<<<<<<< HEAD
#include <QObject>
#include <QDateTime>

=======

#include <QObject>
#include <QDateTime>

#include <memory>

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
class AddressTableModel;
class BanTableModel;
class OptionsModel;
class PeerTableModel;
class TransactionTableModel;

<<<<<<< HEAD
class CWallet;
=======
namespace interfaces {
    class Handler;
}
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

QT_BEGIN_NAMESPACE
class QDateTime;
class QTimer;
QT_END_NAMESPACE

enum BlockSource {
    BLOCK_SOURCE_NONE,
    BLOCK_SOURCE_REINDEX,
    BLOCK_SOURCE_DISK,
    BLOCK_SOURCE_NETWORK
};

enum NumConnections {
    CONNECTIONS_NONE = 0,
    CONNECTIONS_IN = (1U << 0),
    CONNECTIONS_OUT = (1U << 1),
    CONNECTIONS_ALL = (CONNECTIONS_IN | CONNECTIONS_OUT),
};

<<<<<<< HEAD
/** Model for PRCY network client. */
=======
/** Model for PIVX network client. */
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
class ClientModel : public QObject
{
    Q_OBJECT

public:
    explicit ClientModel(OptionsModel* optionsModel, QObject* parent = 0);
    ~ClientModel();

    OptionsModel* getOptionsModel();
    PeerTableModel* getPeerTableModel();
    BanTableModel *getBanTableModel();

    //! Return number of connections, default is in- and outbound (total)
    int getNumConnections(unsigned int flags = CONNECTIONS_ALL) const;
    int getNumBlocksAtStartup();
<<<<<<< HEAD
    QString getMasternodeCountString() const;
=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    // from cached block index
    int getNumBlocks();
    QDateTime getLastBlockDate() const;
    QString getLastBlockHash() const;
<<<<<<< HEAD
    double getVerificationProgress() const;
=======
    uint256 getLastBlockProcessed() const;
    int getLastBlockProcessedHeight() const;
    int64_t getLastBlockProcessedTime() const;
    double getVerificationProgress() const;
    bool isTipCached() const;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    quint64 getTotalBytesRecv() const;
    quint64 getTotalBytesSent() const;

    //! Return true if core is doing initial block download
    bool inInitialBlockDownload() const;
    //! Return true if core is importing blocks
    enum BlockSource getBlockSource() const;
<<<<<<< HEAD
=======
    //! Return true if network activity in core is enabled
    bool getNetworkActive() const;
    //! Toggle network activity state in core
    void setNetworkActive(bool active);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    //! Return warnings to be displayed in status bar
    QString getStatusBarWarnings() const;

    QString formatFullVersion() const;
<<<<<<< HEAD
    QString formatBuildDate() const;
=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    bool isReleaseVersion() const;
    QString clientName() const;
    QString formatClientStartupTime() const;
    QString dataDir() const;

<<<<<<< HEAD
    bool getTorInfo(std::string& ip_port) const;

=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    void setCacheTip(const CBlockIndex* const tip) { cacheTip = tip; }
    void setCacheReindexing(bool reindex) { cachedReindexing = reindex; }
    void setCacheImporting(bool import) { cachedImporting = import; }
    void setCacheInitialSync(bool _initialSync) { cachedInitialSync = _initialSync; }

<<<<<<< HEAD
private:
=======
    bool getTorInfo(std::string& ip_port) const;

    //! Set the automatic port mapping options
    static void mapPort(bool use_upnp, bool use_natpmp);

    // Start/Stop the masternode polling timer
    void startMasternodesTimer();
    void stopMasternodesTimer();
    // Force a MN count update calling mnmanager directly locking its internal mutex.
    // Future todo: implement an event based update and remove the lock requirement.
    QString getMasternodesCount();

    // Return the specific chain amount value for the MN collateral output.
    CAmount getMNCollateralRequiredAmount();

private:
    // Listeners
    std::unique_ptr<interfaces::Handler> m_handler_show_progress;
    std::unique_ptr<interfaces::Handler> m_handler_notify_num_connections_changed;
    std::unique_ptr<interfaces::Handler> m_handler_notify_net_activity_changed;
    std::unique_ptr<interfaces::Handler> m_handler_notify_alert_changed;
    std::unique_ptr<interfaces::Handler> m_handler_banned_list_changed;
    std::unique_ptr<interfaces::Handler> m_handler_notify_block_tip;

    QString getMasternodeCountString() const;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    OptionsModel* optionsModel;
    PeerTableModel* peerTableModel;
    BanTableModel *banTableModel;

    const CBlockIndex* cacheTip{nullptr};
    QString cachedMasternodeCountString;
    bool cachedReindexing;
    bool cachedImporting;
<<<<<<< HEAD
    bool cachedInitialSync;
=======
    std::atomic<bool> cachedInitialSync{false};
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    int numBlocksAtStartup;

    QTimer* pollTimer;
    QTimer* pollMnTimer;

    void subscribeToCoreSignals();
    void unsubscribeFromCoreSignals();

Q_SIGNALS:
    void numConnectionsChanged(int count);
    void numBlocksChanged(int count);
<<<<<<< HEAD
=======
    void networkActiveChanged(bool networkActive);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    void strMasternodesChanged(const QString& strMasternodes);
    void alertsChanged(const QString& warnings);
    void bytesChanged(quint64 totalBytesIn, quint64 totalBytesOut);

    //! Fired when a message should be reported to the user
<<<<<<< HEAD
    void message(const QString& title, const QString& message, unsigned int style);
=======
    void message(const QString& title, const QString& message, unsigned int style, bool* ret = nullptr);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    // Show progress dialog e.g. for verifychain
    void showProgress(const QString& title, int nProgress);

public Q_SLOTS:
    void updateTimer();
    void updateMnTimer();
    void updateNumConnections(int numConnections);
<<<<<<< HEAD
=======
    void updateNetworkActive(bool networkActive);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    void updateAlert();
    void updateBanlist();
};

#endif // BITCOIN_QT_CLIENTMODEL_H
