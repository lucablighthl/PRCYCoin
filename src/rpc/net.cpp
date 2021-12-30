<<<<<<< HEAD
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
=======
// Copyright (c) 2009-2015 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2019 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "rpc/server.h"

#include "clientversion.h"
<<<<<<< HEAD
#include "main.h"
#include "net.h"
#include "netbase.h"
=======
#include "net.h"
#include "netbase.h"
#include "net_processing.h"
#include "optional.h"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#include "protocol.h"
#include "sync.h"
#include "timedata.h"
#include "guiinterface.h"
<<<<<<< HEAD
#include "util.h"
#include "version.h"

#include <univalue.h>

using namespace std;

UniValue getconnectioncount(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
            "getconnectioncount\n"
            "\nReturns the number of connections to other nodes.\n"
            "\nbResult:\n"
            "n          (numeric) The connection count\n"
            "\nExamples:\n" +
            HelpExampleCli("getconnectioncount", "") + HelpExampleRpc("getconnectioncount", ""));

    LOCK2(cs_main, cs_vNodes);
    return (int)vNodes.size();
}

UniValue ping(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
=======
#include "util/system.h"
#include "version.h"
#include "validation.h"
#include "warnings.h"

#include <univalue.h>


UniValue getconnectioncount(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() != 0)
        throw std::runtime_error(
            "getconnectioncount\n"
            "\nReturns the number of connections to other nodes.\n"

            "\nbResult:\n"
            "n          (numeric) The connection count\n"

            "\nExamples:\n" +
            HelpExampleCli("getconnectioncount", "") + HelpExampleRpc("getconnectioncount", ""));

    if(!g_connman)
        throw JSONRPCError(RPC_CLIENT_P2P_DISABLED, "Error: Peer-to-peer functionality missing or disabled");

    return (int)g_connman->GetNodeCount(CConnman::CONNECTIONS_ALL);
}

UniValue ping(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() != 0)
        throw std::runtime_error(
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "ping\n"
            "\nRequests that a ping be sent to all other nodes, to measure ping time.\n"
            "Results provided in getpeerinfo, pingtime and pingwait fields are decimal seconds.\n"
            "Ping command is handled in queue with all other commands, so it measures processing backlog, not just network ping.\n"
<<<<<<< HEAD
            "\nExamples:\n" +
            HelpExampleCli("ping", "") + HelpExampleRpc("ping", ""));

    // Request that each node send a ping during next message processing pass
    LOCK2(cs_main, cs_vNodes);
    for (CNode* pNode : vNodes) {
        pNode->fPingQueued = true;
    }

    return "Done";
}

static void CopyNodeStats(std::vector<CNodeStats>& vstats)
{
    vstats.clear();

    LOCK(cs_vNodes);
    vstats.reserve(vNodes.size());
    for (CNode* pnode : vNodes) {
        CNodeStats stats;
        pnode->copyStats(stats);
        vstats.push_back(stats);
    }
}

UniValue getpeerinfo(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
            "getpeerinfo\n"
            "\nReturns data about each connected network node as a json array of objects.\n"
=======

            "\nExamples:\n" +
            HelpExampleCli("ping", "") + HelpExampleRpc("ping", ""));

    if(!g_connman)
        throw JSONRPCError(RPC_CLIENT_P2P_DISABLED, "Error: Peer-to-peer functionality missing or disabled");

    // Request that each node send a ping during next message processing pass
    g_connman->ForEachNode([](CNode* pnode) {
        pnode->fPingQueued = true;
    });
    return NullUniValue;
}

UniValue getpeerinfo(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() != 0)
        throw std::runtime_error(
            "getpeerinfo\n"
            "\nReturns data about each connected network node as a json array of objects.\n"

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "\nbResult:\n"
            "[\n"
            "  {\n"
            "    \"id\": n,                   (numeric) Peer index\n"
            "    \"addr\":\"host:port\",      (string) The ip address and port of the peer\n"
            "    \"addrlocal\":\"ip:port\",   (string) local address\n"
<<<<<<< HEAD
=======
            "    \"mapped_as\":\"mapped_as\", (string) The AS in the BGP route to the peer used for diversifying\n"
                                                       "peer selection (only available if the asmap config flag is set)\n"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "    \"services\":\"xxxxxxxxxxxxxxxx\",   (string) The services offered\n"
            "    \"lastsend\": ttt,           (numeric) The time in seconds since epoch (Jan 1 1970 GMT) of the last send\n"
            "    \"lastrecv\": ttt,           (numeric) The time in seconds since epoch (Jan 1 1970 GMT) of the last receive\n"
            "    \"bytessent\": n,            (numeric) The total bytes sent\n"
            "    \"bytesrecv\": n,            (numeric) The total bytes received\n"
            "    \"conntime\": ttt,           (numeric) The connection time in seconds since epoch (Jan 1 1970 GMT)\n"
            "    \"timeoffset\": ttt,         (numeric) The time offset in seconds\n"
            "    \"pingtime\": n,             (numeric) ping time\n"
            "    \"pingwait\": n,             (numeric) ping wait\n"
            "    \"version\": v,              (numeric) The peer version, such as 7001\n"
<<<<<<< HEAD
            "    \"subver\": \"/Prcycoin Core:x.x.x.x/\",  (string) The string version\n"
            "    \"inbound\": true|false,     (boolean) Inbound (true) or Outbound (false)\n"
=======
            "    \"subver\": \"/Pivx Core:x.x.x.x/\",  (string) The string version\n"
            "    \"inbound\": true|false,     (boolean) Inbound (true) or Outbound (false)\n"
            "    \"addnode\": true|false,     (boolean) Whether connection was due to addnode and is using an addnode slot\n"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "    \"startingheight\": n,       (numeric) The starting height (block) of the peer\n"
            "    \"banscore\": n,             (numeric) The ban score\n"
            "    \"synced_headers\": n,       (numeric) The last header we have in common with this peer\n"
            "    \"synced_blocks\": n,        (numeric) The last block we have in common with this peer\n"
            "    \"inflight\": [\n"
            "       n,                        (numeric) The heights of blocks we're currently asking from this peer\n"
            "       ...\n"
            "    ]\n"
<<<<<<< HEAD
            "  }\n"
            "  ,...\n"
            "]\n"
            "\nExamples:\n" +
            HelpExampleCli("getpeerinfo", "") + HelpExampleRpc("getpeerinfo", ""));

    LOCK(cs_main);

    vector<CNodeStats> vstats;
    CopyNodeStats(vstats);
=======
            "    \"bytessent_per_msg\": {\n"
            "       \"addr\": n,             (numeric) The total bytes sent aggregated by message type\n"
            "       ...\n"
            "    }\n"
            "    \"bytesrecv_per_msg\": {\n"
            "       \"addr\": n,             (numeric) The total bytes received aggregated by message type\n"
            "       ...\n"
            "    }\n"
            "  }\n"
            "  ,...\n"
            "]\n"

            "\nExamples:\n" +
            HelpExampleCli("getpeerinfo", "") + HelpExampleRpc("getpeerinfo", ""));

    if(!g_connman)
        throw JSONRPCError(RPC_CLIENT_P2P_DISABLED, "Error: Peer-to-peer functionality missing or disabled");

    std::vector<CNodeStats> vstats;
    g_connman->GetNodeStats(vstats);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    UniValue ret(UniValue::VARR);

    for (const CNodeStats& stats : vstats) {
        UniValue obj(UniValue::VOBJ);
        CNodeStateStats statestats;
        bool fStateStats = GetNodeStateStats(stats.nodeid, statestats);
<<<<<<< HEAD
        obj.push_back(Pair("id", stats.nodeid));
        obj.push_back(Pair("addr", stats.addrName));
        if (!(stats.addrLocal.empty()))
            obj.push_back(Pair("addrlocal", stats.addrLocal));
        obj.push_back(Pair("services", strprintf("%016x", stats.nServices)));
        obj.push_back(Pair("lastsend", stats.nLastSend));
        obj.push_back(Pair("lastrecv", stats.nLastRecv));
        obj.push_back(Pair("bytessent", stats.nSendBytes));
        obj.push_back(Pair("bytesrecv", stats.nRecvBytes));
        obj.push_back(Pair("conntime", stats.nTimeConnected));
        obj.push_back(Pair("timeoffset", stats.nTimeOffset));
        obj.push_back(Pair("pingtime", stats.dPingTime));
        if (stats.dPingWait > 0.0)
            obj.push_back(Pair("pingwait", stats.dPingWait));
        obj.push_back(Pair("version", stats.nVersion));
        // Use the sanitized form of subver here, to avoid tricksy remote peers from
        // corrupting or modifiying the JSON output by putting special characters in
        // their ver message.
        obj.push_back(Pair("subver", stats.cleanSubVer));
        obj.push_back(Pair("inbound", stats.fInbound));
        obj.push_back(Pair("startingheight", stats.nStartingHeight));
        if (fStateStats) {
            obj.push_back(Pair("banscore", statestats.nMisbehavior));
            obj.push_back(Pair("synced_headers", statestats.nSyncHeight));
            obj.push_back(Pair("synced_blocks", statestats.nCommonHeight));
=======
        obj.pushKV("id", stats.nodeid);
        obj.pushKV("addr", stats.addrName);
        if (!(stats.addrLocal.empty()))
            obj.pushKV("addrlocal", stats.addrLocal);
        if (stats.m_mapped_as != 0) {
            obj.pushKV("mapped_as", uint64_t(stats.m_mapped_as));
        }
        obj.pushKV("services", strprintf("%016x", stats.nServices));
        obj.pushKV("lastsend", stats.nLastSend);
        obj.pushKV("lastrecv", stats.nLastRecv);
        obj.pushKV("bytessent", stats.nSendBytes);
        obj.pushKV("bytesrecv", stats.nRecvBytes);
        obj.pushKV("conntime", stats.nTimeConnected);
        obj.pushKV("timeoffset", stats.nTimeOffset);
        obj.pushKV("pingtime", stats.dPingTime);
        if (stats.dPingWait > 0.0)
            obj.pushKV("pingwait", stats.dPingWait);
        obj.pushKV("version", stats.nVersion);
        // Use the sanitized form of subver here, to avoid tricksy remote peers from
        // corrupting or modifiying the JSON output by putting special characters in
        // their ver message.
        obj.pushKV("subver", stats.cleanSubVer);
        obj.pushKV("inbound", stats.fInbound);
        obj.pushKV("addnode", stats.fAddnode);
        obj.pushKV("startingheight", stats.nStartingHeight);
        if (fStateStats) {
            obj.pushKV("banscore", statestats.nMisbehavior);
            obj.pushKV("synced_headers", statestats.nSyncHeight);
            obj.pushKV("synced_blocks", statestats.nCommonHeight);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            UniValue heights(UniValue::VARR);
            for (int height : statestats.vHeightInFlight) {
                heights.push_back(height);
            }
<<<<<<< HEAD
            obj.push_back(Pair("inflight", heights));
        }
        obj.push_back(Pair("whitelisted", stats.fWhitelisted));
=======
            obj.pushKV("inflight", heights);
        }
        obj.pushKV("whitelisted", stats.fWhitelisted);

        UniValue sendPerMsgCmd(UniValue::VOBJ);
        for (const mapMsgCmdSize::value_type &i : stats.mapSendBytesPerMsgCmd) {
            if (i.second > 0)
                sendPerMsgCmd.pushKV(i.first, i.second);
        }
        obj.pushKV("bytessent_per_msg", sendPerMsgCmd);

        UniValue recvPerMsgCmd(UniValue::VOBJ);
        for (const mapMsgCmdSize::value_type &i : stats.mapRecvBytesPerMsgCmd) {
            if (i.second > 0)
                recvPerMsgCmd.pushKV(i.first, i.second);
        }
        obj.pushKV("bytesrecv_per_msg", recvPerMsgCmd);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

        ret.push_back(obj);
    }

    return ret;
}

<<<<<<< HEAD
UniValue addnode(const UniValue& params, bool fHelp)
{
    string strCommand;
    if (params.size() == 2)
        strCommand = params[1].get_str();
    if (fHelp || params.size() != 2 ||
        (strCommand != "onetry" && strCommand != "add" && strCommand != "remove"))
        throw runtime_error(
            "addnode \"node\" \"add|remove|onetry\"\n"
            "\nAttempts add or remove a node from the addnode list.\n"
            "Or try a connection to a node once.\n"
            "\nArguments:\n"
            "1. \"node\"     (string, required) The node (see getpeerinfo for nodes)\n"
            "2. \"command\"  (string, required) 'add' to add a node to the list, 'remove' to remove a node from the list, 'onetry' to try a connection to the node once\n"
            "\nExamples:\n" +
            HelpExampleCli("addnode", "\"192.168.0.6:59682\" \"onetry\"") + HelpExampleRpc("addnode", "\"192.168.0.6:59682\", \"onetry\""));

    string strNode = params[0].get_str();

    if (strCommand == "onetry") {
        CAddress addr;
        OpenNetworkConnection(addr, NULL, strNode.c_str());
        return NullUniValue;
    }

    LOCK(cs_vAddedNodes);
    vector<string>::iterator it = vAddedNodes.begin();
    for (; it != vAddedNodes.end(); it++)
        if (strNode == *it)
            break;

    if (strCommand == "add") {
        if (it != vAddedNodes.end())
            throw JSONRPCError(RPC_CLIENT_NODE_ALREADY_ADDED, "Error: Node already added");
        vAddedNodes.push_back(strNode);
    } else if (strCommand == "remove") {
        if (it == vAddedNodes.end())
            throw JSONRPCError(RPC_CLIENT_NODE_NOT_ADDED, "Error: Node has not been added.");
        vAddedNodes.erase(it);
    }

    return "Done";
}

UniValue disconnectnode(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() != 1)
        throw runtime_error(
                "disconnectnode \"node\" \n"
                "\nImmediately disconnects from the specified node.\n"
                "\nArguments:\n"
                "1. \"node\"     (string, required) The node (see getpeerinfo for nodes)\n"
                "\nExamples:\n"
                + HelpExampleCli("disconnectnode", "\"192.168.0.6:8333\"")
                + HelpExampleRpc("disconnectnode", "\"192.168.0.6:8333\"")
        );
    CNode* pNode = FindNode(params[0].get_str());
    if (pNode == NULL)
        throw JSONRPCError(RPC_CLIENT_NODE_NOT_CONNECTED, "Node not found in connected nodes");
    pNode->CloseSocketDisconnect();
    return "Done";
}

UniValue getaddednodeinfo(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() < 1 || params.size() > 2)
        throw runtime_error(
            "getaddednodeinfo dns ( \"node\" )\n"
            "\nReturns information about the given added node, or all added nodes\n"
            "(note that onetry addnodes are not listed here)\n"
            "If dns is false, only a list of added nodes will be provided,\n"
            "otherwise connected information will also be available.\n"
            "\nArguments:\n"
            "1. dns        (boolean, required) If false, only a list of added nodes will be provided, otherwise connected information will also be available.\n"
            "2. \"node\"   (string, optional) If provided, return information about this specific node, otherwise all nodes are returned.\n"
            "\nResult:\n"
            "[\n"
            "  {\n"
            "    \"addednode\" : \"192.168.0.201\",   (string) The node ip address\n"
            "    \"connected\" : true|false,          (boolean) If connected\n"
            "    \"addresses\" : [\n"
            "       {\n"
            "         \"address\" : \"192.168.0.201:59682\",  (string) The prcycoin server host and port\n"
            "         \"connected\" : \"outbound\"           (string) connection, inbound or outbound\n"
            "       }\n"
            "       ,...\n"
=======
UniValue addnode(const JSONRPCRequest& request)
{
    std::string strCommand;
    if (request.params.size() == 2)
        strCommand = request.params[1].get_str();
    if (request.fHelp || request.params.size() != 2 ||
        (strCommand != "onetry" && strCommand != "add" && strCommand != "remove"))
        throw std::runtime_error(
            "addnode \"node\" \"add|remove|onetry\"\n"
            "\nAttempts add or remove a node from the addnode list.\n"
            "Or try a connection to a node once.\n"

            "\nArguments:\n"
            "1. \"node\"     (string, required) The node (see getpeerinfo for nodes)\n"
            "2. \"command\"  (string, required) 'add' to add a node to the list, 'remove' to remove a node from the list, 'onetry' to try a connection to the node once\n"

            "\nExamples:\n" +
            HelpExampleCli("addnode", "\"192.168.0.6:51472\" \"onetry\"") + HelpExampleRpc("addnode", "\"192.168.0.6:51472\", \"onetry\""));

    if(!g_connman)
        throw JSONRPCError(RPC_CLIENT_P2P_DISABLED, "Error: Peer-to-peer functionality missing or disabled");

    std::string strNode = request.params[0].get_str();

    if (strCommand == "onetry") {
        CAddress addr;
        g_connman->OpenNetworkConnection(addr, false, NULL, strNode.c_str());
        return NullUniValue;
    }

    if (strCommand == "add") {
        if(!g_connman->AddNode(strNode))
            throw JSONRPCError(RPC_CLIENT_NODE_ALREADY_ADDED, "Error: Node already added");
    } else if (strCommand == "remove") {
        if(!g_connman->RemoveAddedNode(strNode))
            throw JSONRPCError(RPC_CLIENT_NODE_NOT_ADDED, "Error: Node has not been added.");
    }

    return NullUniValue;
}

UniValue disconnectnode(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() != 1)
        throw std::runtime_error(
            "disconnectnode \"node\" \n"
            "\nImmediately disconnects from the specified node.\n"

            "\nArguments:\n"
            "1. \"node\"     (string, required) The node (see getpeerinfo for nodes)\n"

            "\nExamples:\n"
            + HelpExampleCli("disconnectnode", "\"192.168.0.6:8333\"")
            + HelpExampleRpc("disconnectnode", "\"192.168.0.6:8333\"")
        );

    if(!g_connman)
        throw JSONRPCError(RPC_CLIENT_P2P_DISABLED, "Error: Peer-to-peer functionality missing or disabled");

    bool ret = g_connman->DisconnectNode(request.params[0].get_str());
    if (!ret)
        throw JSONRPCError(RPC_CLIENT_NODE_NOT_CONNECTED, "Node not found in connected nodes");

    return NullUniValue;
}

UniValue getaddednodeinfo(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() < 1 || request.params.size() > 2)
        throw std::runtime_error(
            "getaddednodeinfo dummy ( \"node\" )\n"
            "\nReturns information about the given added node, or all added nodes\n"
            "(note that onetry addnodes are not listed here)\n"

            "\nArguments:\n"
            "1. dummy      (boolean, required) Kept for historical purposes but ignored\n"
            "2. \"node\"   (string, optional) If provided, return information about this specific node, otherwise all nodes are returned.\n"

            "\nResult:\n"
            "[\n"
            "  {\n"
            "    \"addednode\" : \"192.168.0.201\",   (string) The node ip address or name (as provided to addnode)\n"
            "    \"connected\" : true|false,          (boolean) If connected\n"
            "    \"addresses\" : [                    (list of objects) Only when connected = true\n"
            "       {\n"
            "         \"address\" : \"192.168.0.201:51472\",  (string) The pivx server IP and port we're connected to\n"
            "         \"connected\" : \"outbound\"           (string) connection, inbound or outbound\n"
            "       }\n"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "     ]\n"
            "  }\n"
            "  ,...\n"
            "]\n"
<<<<<<< HEAD
            "\nExamples:\n" +
            HelpExampleCli("getaddednodeinfo", "true") + HelpExampleCli("getaddednodeinfo", "true \"192.168.0.201\"") + HelpExampleRpc("getaddednodeinfo", "true, \"192.168.0.201\""));

    bool fDns = params[0].get_bool();

    list<string> laddedNodes(0);
    if (params.size() == 1) {
        LOCK(cs_vAddedNodes);
        for (string& strAddNode : vAddedNodes)
            laddedNodes.push_back(strAddNode);
    } else {
        string strNode = params[1].get_str();
        LOCK(cs_vAddedNodes);
        for (string& strAddNode : vAddedNodes)
            if (strAddNode == strNode) {
                laddedNodes.push_back(strAddNode);
                break;
            }
        if (laddedNodes.size() == 0)
            throw JSONRPCError(RPC_CLIENT_NODE_NOT_ADDED, "Error: Node has not been added.");
    }

    UniValue ret(UniValue::VARR);
    if (!fDns) {
        for (string& strAddNode : laddedNodes) {
            UniValue obj(UniValue::VOBJ);
            obj.push_back(Pair("addednode", strAddNode));
            ret.push_back(obj);
        }
        return ret;
    }

    list<pair<string, vector<CService> > > laddedAddreses(0);
    for (string& strAddNode : laddedNodes) {
        vector<CService> vservNode(0);
        if (Lookup(strAddNode.c_str(), vservNode, Params().GetDefaultPort(), fNameLookup, 0))
            laddedAddreses.push_back(make_pair(strAddNode, vservNode));
        else {
            UniValue obj(UniValue::VOBJ);
            obj.push_back(Pair("addednode", strAddNode));
            obj.push_back(Pair("connected", false));
            UniValue addresses(UniValue::VARR);
            obj.push_back(Pair("addresses", addresses));
        }
    }

    LOCK(cs_vNodes);
    for (list<pair<string, vector<CService> > >::iterator it = laddedAddreses.begin(); it != laddedAddreses.end(); it++) {
        UniValue obj(UniValue::VOBJ);
        obj.push_back(Pair("addednode", it->first));

        UniValue addresses(UniValue::VARR);
        bool fConnected = false;
        for (CService& addrNode : it->second) {
            bool fFound = false;
            UniValue node(UniValue::VOBJ);
            node.push_back(Pair("address", addrNode.ToString()));
            for (CNode* pnode : vNodes)
                if (pnode->addr == addrNode) {
                    fFound = true;
                    fConnected = true;
                    node.push_back(Pair("connected", pnode->fInbound ? "inbound" : "outbound"));
                    break;
                }
            if (!fFound)
                node.push_back(Pair("connected", "false"));
            addresses.push_back(node);
        }
        obj.push_back(Pair("connected", fConnected));
        obj.push_back(Pair("addresses", addresses));
=======

            "\nExamples:\n" +
            HelpExampleCli("getaddednodeinfo", "true") + HelpExampleCli("getaddednodeinfo", "true \"192.168.0.201\"") + HelpExampleRpc("getaddednodeinfo", "true, \"192.168.0.201\""));

    if(!g_connman)
        throw JSONRPCError(RPC_CLIENT_P2P_DISABLED, "Error: Peer-to-peer functionality missing or disabled");

    std::vector<AddedNodeInfo> vInfo = g_connman->GetAddedNodeInfo();

    if (request.params.size() == 2) {
        bool found = false;
        for (const AddedNodeInfo& info : vInfo) {
            if (info.strAddedNode == request.params[1].get_str()) {
                vInfo.assign(1, info);
                found = true;
                break;
            }
        }
        if (!found) {
            throw JSONRPCError(RPC_CLIENT_NODE_NOT_ADDED, "Error: Node has not been added.");
        }
    }

    UniValue ret(UniValue::VARR);

    for (const AddedNodeInfo& info : vInfo) {
        UniValue obj(UniValue::VOBJ);
        obj.pushKV("addednode", info.strAddedNode);
        obj.pushKV("connected", info.fConnected);
        UniValue addresses(UniValue::VARR);
        if (info.fConnected) {
            UniValue address(UniValue::VOBJ);
            address.pushKV("address", info.resolvedAddress.ToString());
            address.pushKV("connected", info.fInbound ? "inbound" : "outbound");
            addresses.push_back(address);
        }
        obj.pushKV("addresses", addresses);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        ret.push_back(obj);
    }

    return ret;
}

<<<<<<< HEAD
UniValue getnettotals(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() > 0)
        throw runtime_error(
            "getnettotals\n"
            "\nReturns information about network traffic, including bytes in, bytes out,\n"
            "and current time.\n"
=======
UniValue getnettotals(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() > 0)
        throw std::runtime_error(
            "getnettotals\n"
            "\nReturns information about network traffic, including bytes in, bytes out,\n"
            "and current time.\n"

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "\nResult:\n"
            "{\n"
            "  \"totalbytesrecv\": n,   (numeric) Total bytes received\n"
            "  \"totalbytessent\": n,   (numeric) Total bytes sent\n"
            "  \"timemillis\": t        (numeric) Total cpu time\n"
            "}\n"
<<<<<<< HEAD
            "\nExamples:\n" +
            HelpExampleCli("getnettotals", "") + HelpExampleRpc("getnettotals", ""));

    UniValue obj(UniValue::VOBJ);
    obj.push_back(Pair("totalbytesrecv", CNode::GetTotalBytesRecv()));
    obj.push_back(Pair("totalbytessent", CNode::GetTotalBytesSent()));
    obj.push_back(Pair("timemillis", GetTimeMillis()));
=======

            "\nExamples:\n" +
            HelpExampleCli("getnettotals", "") + HelpExampleRpc("getnettotals", ""));

    if(!g_connman)
        throw JSONRPCError(RPC_CLIENT_P2P_DISABLED, "Error: Peer-to-peer functionality missing or disabled");

    UniValue obj(UniValue::VOBJ);
    obj.pushKV("totalbytesrecv", g_connman->GetTotalBytesRecv());
    obj.pushKV("totalbytessent", g_connman->GetTotalBytesSent());
    obj.pushKV("timemillis", GetTimeMillis());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    return obj;
}

static UniValue GetNetworksInfo()
{
    UniValue networks(UniValue::VARR);
    for (int n = 0; n < NET_MAX; ++n) {
        enum Network network = static_cast<enum Network>(n);
<<<<<<< HEAD
        if (network == NET_UNROUTABLE)
            continue;
        proxyType proxy;
        UniValue obj(UniValue::VOBJ);
        GetProxy(network, proxy);
        obj.push_back(Pair("name", GetNetworkName(network)));
        obj.push_back(Pair("limited", IsLimited(network)));
        obj.push_back(Pair("reachable", IsReachable(network)));
        obj.push_back(Pair("proxy", proxy.IsValid() ? proxy.proxy.ToStringIPPort() : string()));
        obj.push_back(Pair("proxy_randomize_credentials", proxy.randomize_credentials));
=======
        if (network == NET_UNROUTABLE || network == NET_I2P || network == NET_CJDNS || network == NET_INTERNAL) continue;
        proxyType proxy;
        UniValue obj(UniValue::VOBJ);
        GetProxy(network, proxy);
        obj.pushKV("name", GetNetworkName(network));
        obj.pushKV("limited", !IsReachable(network));
        obj.pushKV("reachable", IsReachable(network));
        obj.pushKV("proxy", proxy.IsValid() ? proxy.proxy.ToStringIPPort() : std::string());
        obj.pushKV("proxy_randomize_credentials", proxy.randomize_credentials);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        networks.push_back(obj);
    }
    return networks;
}

<<<<<<< HEAD
UniValue getnetworkinfo(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
            "getnetworkinfo\n"
            "Returns an object containing various state info regarding P2P networking.\n"
            "\nResult:\n"
            "{\n"
            "  \"version\": xxxxx,                      (numeric) the server version\n"
            "  \"subversion\": \"/Prcycoin Core:x.x.x.x/\",     (string) the server subversion string\n"
=======
UniValue getnetworkinfo(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() != 0)
        throw std::runtime_error(
            "getnetworkinfo\n"
            "\nReturns an object containing various state info regarding P2P networking.\n"

            "\nResult:\n"
            "{\n"
            "  \"version\": xxxxx,                      (numeric) the server version\n"
            "  \"subversion\": \"/Pivx Core:x.x.x.x/\",     (string) the server subversion string\n"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "  \"protocolversion\": xxxxx,              (numeric) the protocol version\n"
            "  \"localservices\": \"xxxxxxxxxxxxxxxx\", (string) the services we offer to the network\n"
            "  \"timeoffset\": xxxxx,                   (numeric) the time offset\n"
            "  \"connections\": xxxxx,                  (numeric) the number of connections\n"
<<<<<<< HEAD
=======
            "  \"networkactive\": true|false,           (boolean) the network activity status\n"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "  \"networks\": [                          (array) information per network\n"
            "  {\n"
            "    \"name\": \"xxx\",                     (string) network (ipv4, ipv6 or onion)\n"
            "    \"limited\": true|false,               (boolean) is the network limited using -onlynet?\n"
            "    \"reachable\": true|false,             (boolean) is the network reachable?\n"
            "    \"proxy\": \"host:port\"               (string) the proxy that is used for this network, or empty if none\n"
            "  }\n"
            "  ,...\n"
            "  ],\n"
<<<<<<< HEAD
            "  \"relayfee\": x.xxxxxxxx,                (numeric) minimum relay fee for non-free transactions in prcycoin/kb\n"
=======
            "  \"relayfee\": x.xxxxxxxx,                (numeric) minimum relay fee for transactions in " + CURRENCY_UNIT + "/kB\n"
            "  \"incrementalfee\": x.xxxxxxxx,          (numeric) minimum fee increment for mempool limiting or BIP 125 replacement in " + CURRENCY_UNIT + "/kB\n"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "  \"localaddresses\": [                    (array) list of local addresses\n"
            "  {\n"
            "    \"address\": \"xxxx\",                 (string) network address\n"
            "    \"port\": xxx,                         (numeric) network port\n"
            "    \"score\": xxx                         (numeric) relative score\n"
            "  }\n"
            "  ,...\n"
            "  ]\n"
<<<<<<< HEAD
            "}\n"
=======
            "  \"warnings\": \"...\"                    (string) any network and blockchain warnings\n"
            "}\n"

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "\nExamples:\n" +
            HelpExampleCli("getnetworkinfo", "") + HelpExampleRpc("getnetworkinfo", ""));

    LOCK(cs_main);
<<<<<<< HEAD

    UniValue obj(UniValue::VOBJ);
    obj.push_back(Pair("version", CLIENT_VERSION));
    obj.push_back(Pair("subversion", strSubVersion));
    obj.push_back(Pair("protocolversion", PROTOCOL_VERSION));
    obj.push_back(Pair("localservices", strprintf("%016x", nLocalServices)));
    obj.push_back(Pair("timeoffset", GetTimeOffset()));
    obj.push_back(Pair("connections", (int)vNodes.size()));
    obj.push_back(Pair("networks", GetNetworksInfo()));
    obj.push_back(Pair("relayfee", ValueFromAmount(::minRelayTxFee.GetFeePerK())));
=======
    UniValue obj(UniValue::VOBJ);
    obj.pushKV("version", CLIENT_VERSION);
    obj.pushKV("subversion",    strSubVersion);
    obj.pushKV("protocolversion", PROTOCOL_VERSION);
    if (g_connman)
        obj.pushKV("localservices", strprintf("%016x", g_connman->GetLocalServices()));
    obj.pushKV("timeoffset", GetTimeOffset());
    if (g_connman) {
        obj.pushKV("networkactive", g_connman->GetNetworkActive());
        obj.pushKV("connections",   (int)g_connman->GetNodeCount(CConnman::CONNECTIONS_ALL));
    }
    obj.pushKV("networks", GetNetworksInfo());
    obj.pushKV("relayfee", ValueFromAmount(::minRelayTxFee.GetFeePerK()));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    UniValue localAddresses(UniValue::VARR);
    {
        LOCK(cs_mapLocalHost);
        for (const std::pair<const CNetAddr, LocalServiceInfo> &item : mapLocalHost) {
            UniValue rec(UniValue::VOBJ);
<<<<<<< HEAD
            rec.push_back(Pair("address", item.first.ToString()));
            rec.push_back(Pair("port", item.second.nPort));
            rec.push_back(Pair("score", item.second.nScore));
            localAddresses.push_back(rec);
        }
    }
    obj.push_back(Pair("localaddresses", localAddresses));
    return obj;
}

UniValue setban(const UniValue& params, bool fHelp)
{
    string strCommand;
    if (params.size() >= 2)
        strCommand = params[1].get_str();
    if (fHelp || params.size() < 2 ||
        (strCommand != "add" && strCommand != "remove"))
        throw runtime_error(
                "setban \"ip(/netmask)\" \"add|remove\" (bantime) (absolute)\n"
                "\nAttempts add or remove a IP/Subnet from the banned list.\n"
                "\nArguments:\n"
                "1. \"ip(/netmask)\" (string, required) The IP/Subnet (see getpeerinfo for nodes ip) with a optional netmask (default is /32 = single ip)\n"
                "2. \"command\"      (string, required) 'add' to add a IP/Subnet to the list, 'remove' to remove a IP/Subnet from the list\n"
                "3. \"bantime\"      (numeric, optional) time in seconds how long (or until when if [absolute] is set) the ip is banned (0 or empty means using the default time of 24h which can also be overwritten by the -bantime startup argument)\n"
                "4. \"absolute\"     (boolean, optional) If set, the bantime must be a absolute timestamp in seconds since epoch (Jan 1 1970 GMT)\n"
                "\nExamples:\n"
                + HelpExampleCli("setban", "\"192.168.0.6\" \"add\" 86400")
                + HelpExampleCli("setban", "\"192.168.0.0/24\" \"add\"")
                + HelpExampleRpc("setban", "\"192.168.0.6\", \"add\" 86400")
        );
    CSubNet subNet;
    CNetAddr netAddr;
    bool isSubnet = false;
    if (params[0].get_str().find("/") != string::npos)
        isSubnet = true;
    if (!isSubnet)
        netAddr = CNetAddr(params[0].get_str());
    else
        subNet = CSubNet(params[0].get_str());
    if (! (isSubnet ? subNet.IsValid() : netAddr.IsValid()) )
        throw JSONRPCError(RPC_CLIENT_NODE_ALREADY_ADDED, "Error: Invalid IP/Subnet");
    if (strCommand == "add")
    {
        if (isSubnet ? CNode::IsBanned(subNet) : CNode::IsBanned(netAddr))
            throw JSONRPCError(RPC_CLIENT_NODE_ALREADY_ADDED, "Error: IP/Subnet already banned");
        int64_t banTime = 0; //use standard bantime if not specified
        if (params.size() >= 3 && !params[2].isNull())
            banTime = params[2].get_int64();
        bool absolute = false;
        if (params.size() == 4)
            absolute = params[3].get_bool();
        isSubnet ? CNode::Ban(subNet, BanReasonManuallyAdded, banTime, absolute) : CNode::Ban(netAddr, BanReasonManuallyAdded, banTime, absolute);
        //disconnect possible nodes
        while(CNode *bannedNode = (isSubnet ? FindNode(subNet) : FindNode(netAddr)))
            bannedNode->CloseSocketDisconnect();
    }
    else if(strCommand == "remove")
    {
        if (!( isSubnet ? CNode::Unban(subNet) : CNode::Unban(netAddr) ))
            throw JSONRPCError(RPC_MISC_ERROR, "Error: Unban failed");
    }
    DumpBanlist(); //store banlist to disk
    uiInterface.BannedListChanged();
    return "Done";
}

UniValue listbanned(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
                "listbanned\n"
                "\nList all banned IPs/Subnets.\n"
                "\nExamples:\n"
                + HelpExampleCli("listbanned", "")
                + HelpExampleRpc("listbanned", "")
        );
    banmap_t banMap;
    CNode::GetBanned(banMap);
    UniValue bannedAddresses(UniValue::VARR);
    for (banmap_t::iterator it = banMap.begin(); it != banMap.end(); it++)
    {
        CBanEntry banEntry = (*it).second;
        UniValue rec(UniValue::VOBJ);
        rec.push_back(Pair("address", (*it).first.ToString()));
        rec.push_back(Pair("banned_until", banEntry.nBanUntil));
        rec.push_back(Pair("ban_created", banEntry.nCreateTime));
        rec.push_back(Pair("ban_reason", banEntry.banReasonToString()));
        bannedAddresses.push_back(rec);
    }
    return bannedAddresses;
}

UniValue clearbanned(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
                "clearbanned\n"
                "\nClear all banned IPs.\n"
                "\nExamples:\n"
                + HelpExampleCli("clearbanned", "")
                + HelpExampleRpc("clearbanned", "")
        );
    CNode::ClearBanned();
    DumpBanlist(); //store banlist to disk
    uiInterface.BannedListChanged();
    return "Done";
=======
            rec.pushKV("address", item.first.ToString());
            rec.pushKV("port", item.second.nPort);
            rec.pushKV("score", item.second.nScore);
            localAddresses.push_back(rec);
        }
    }
    obj.pushKV("localaddresses", localAddresses);
    obj.pushKV("warnings", GetWarnings("statusbar"));
    return obj;
}

UniValue setban(const JSONRPCRequest& request)
{
    std::string strCommand;
    if (request.params.size() >= 2)
        strCommand = request.params[1].get_str();
    if (request.fHelp || request.params.size() < 2 ||
        (strCommand != "add" && strCommand != "remove"))
        throw std::runtime_error(
            "setban \"subnet\" \"add|remove\" ( bantime absolute )\n"
            "\nAttempts add or remove a IP/Subnet from the banned list.\n"

            "\nArguments:\n"
            "1. \"subnet\"       (string, required) The IP/Subnet (see getpeerinfo for nodes ip) with a optional netmask (default is /32 = single ip)\n"
            "2. \"command\"      (string, required) 'add' to add a IP/Subnet to the list, 'remove' to remove a IP/Subnet from the list\n"
            "3. \"bantime\"      (numeric, optional) time in seconds how long (or until when if [absolute] is set) the ip is banned (0 or empty means using the default time of 24h which can also be overwritten by the -bantime startup argument)\n"
            "4. \"absolute\"     (boolean, optional) If set, the bantime must be a absolute timestamp in seconds since epoch (Jan 1 1970 GMT)\n"

            "\nExamples:\n"
            + HelpExampleCli("setban", "\"192.168.0.6\" \"add\" 86400")
            + HelpExampleCli("setban", "\"192.168.0.0/24\" \"add\"")
            + HelpExampleRpc("setban", "\"192.168.0.6\", \"add\" 86400"));

    if (!g_connman)
        throw JSONRPCError(RPC_CLIENT_P2P_DISABLED, "Error: Peer-to-peer functionality missing or disabled");

    CSubNet subNet;
    CNetAddr netAddr;
    bool isSubnet = false;

    if (request.params[0].get_str().find('/') != std::string::npos)
        isSubnet = true;

    if (!isSubnet) {
        CNetAddr resolved;
        LookupHost(request.params[0].get_str(), resolved, false);
        netAddr = resolved;
    } else
        LookupSubNet(request.params[0].get_str(), subNet);

    if (! (isSubnet ? subNet.IsValid() : netAddr.IsValid()) )
        throw JSONRPCError(RPC_CLIENT_NODE_ALREADY_ADDED, "Error: Invalid IP/Subnet");

    if (strCommand == "add")
    {
        if (isSubnet ? g_connman->IsBanned(subNet) : g_connman->IsBanned(netAddr))
            throw JSONRPCError(RPC_CLIENT_NODE_ALREADY_ADDED, "Error: IP/Subnet already banned");

        int64_t banTime = 0; //use standard bantime if not specified
        if (request.params.size() >= 3 && !request.params[2].isNull())
            banTime = request.params[2].get_int64();

        bool absolute = false;
        if (request.params.size() == 4)
            absolute = request.params[3].get_bool();

        isSubnet ? g_connman->Ban(subNet, BanReasonManuallyAdded, banTime, absolute) : g_connman->Ban(netAddr, BanReasonManuallyAdded, banTime, absolute);
    }
    else if(strCommand == "remove")
    {
        if (!( isSubnet ? g_connman->Unban(subNet) : g_connman->Unban(netAddr) ))
            throw JSONRPCError(RPC_MISC_ERROR, "Error: Unban failed");
    }
    return NullUniValue;
}

UniValue listbanned(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() != 0)
        throw std::runtime_error(
            "listbanned\n"
            "\nList all banned IPs/Subnets.\n"

            "\nResult:\n"
            "[\n"
            "  {\n"
            "    \"address\": \"xxx\",          (string) Network address of banned client.\n"
            "    \"banned_until\": nnn,         (numeric) Timestamp when the ban is lifted.\n"
            "    \"ban_created\": nnn,          (numeric) Timestamp when the ban was created.\n"
            "    \"ban_reason\": \"xxx\"        (string) Reason for banning.\n"
            "  }\n"
            "  ,...\n"
            "]\n"

            "\nExamples:\n"
            + HelpExampleCli("listbanned", "")
            + HelpExampleRpc("listbanned", ""));

    if (!g_connman)
        throw JSONRPCError(RPC_CLIENT_P2P_DISABLED, "Error: Peer-to-peer functionality missing or disabled");

    banmap_t banMap;
    g_connman->GetBanned(banMap);

    UniValue bannedAddresses(UniValue::VARR);
    for (const auto& entry : banMap)
    {
        const CBanEntry& banEntry = entry.second;
        UniValue rec(UniValue::VOBJ);
        rec.pushKV("address", entry.first.ToString());
        rec.pushKV("banned_until", banEntry.nBanUntil);
        rec.pushKV("ban_created", banEntry.nCreateTime);
        rec.pushKV("ban_reason", banEntry.banReasonToString());

        bannedAddresses.push_back(rec);
    }

    return bannedAddresses;
}

UniValue clearbanned(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() != 0)
        throw std::runtime_error(
            "clearbanned\n"
            "\nClear all banned IPs.\n"

            "\nExamples:\n"
            + HelpExampleCli("clearbanned", "")
            + HelpExampleRpc("clearbanned", ""));

    if (!g_connman)
        throw JSONRPCError(RPC_CLIENT_P2P_DISABLED, "Error: Peer-to-peer functionality missing or disabled");

    g_connman->ClearBanned();

    return NullUniValue;
}

static UniValue getnodeaddresses(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() > 2) {
        throw std::runtime_error(
            "getnodeaddresses ( count \"network\" )\n"
            "\nReturn known addresses which can potentially be used to find new nodes in the network\n"

            "\nArguments:\n"
            "1. count        (numeric, optional) The maximum number of addresses to return. Specify 0 to return all known addresses.\n"
            "2. \"network\"  (string, optional) Return only addresses of the specified network. Can be one of: ipv4, ipv6, onion."

            "\nResult:\n"
            "[\n"
            "  {\n"
            "    \"time\": ttt,                (numeric) Timestamp in seconds since epoch (Jan 1 1970 GMT) when the node was last seen\n"
            "    \"services\": n,              (numeric) The services offered by the node\n"
            "    \"address\": \"host\",        (string) The address of the node\n"
            "    \"port\": n,                  (numeric) The port number of the node\n"
            "    \"network\": \"xxxx\"         (string) The network (ipv4, ipv6, onion) the node connected through\n"
            "  }\n"
            "  ,...\n"
            "]\n"

            "\nExamples:\n"
            + HelpExampleCli("getnodeaddresses", "8")
            + HelpExampleCli("getnodeaddresses", "4 \"ipv4\"")
            + HelpExampleRpc("getnodeaddresses", "8")
            + HelpExampleRpc("getnodeaddresses", "4 \"ipv4\"")
        );
    }
    if (!g_connman) {
        throw JSONRPCError(RPC_CLIENT_P2P_DISABLED, "Error: Peer-to-peer functionality missing or disabled");
    }

    const int count{request.params[0].isNull() ? 1 : request.params[0].get_int()};
    if (count < 0) throw JSONRPCError(RPC_INVALID_PARAMETER, "Address count out of range");

    const Optional<Network> network{request.params[1].isNull() ? nullopt : Optional<Network>{ParseNetwork(request.params[1].get_str())}};
    if (network == NET_UNROUTABLE) {
        throw JSONRPCError(RPC_INVALID_PARAMETER, strprintf("Network not recognized: %s", request.params[1].get_str()));
    }

    // returns a shuffled list of CAddress
    const std::vector<CAddress> vAddr{g_connman->GetAddresses(count, /* max_pct */ 0, network)};
    UniValue ret(UniValue::VARR);

    for (const CAddress& addr : vAddr) {
        UniValue obj(UniValue::VOBJ);
        obj.pushKV("time", (int)addr.nTime);
        obj.pushKV("services", (uint64_t)addr.nServices);
        obj.pushKV("address", addr.ToStringIP());
        obj.pushKV("port", addr.GetPort());
        obj.pushKV("network", GetNetworkName(addr.GetNetClass()));
        ret.push_back(obj);
    }
    return ret;
}

static UniValue addpeeraddress(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() != 2) {
        throw std::runtime_error(
            "addpeeraddress \"address\" port\n"
            "\nAdd the address of a potential peer to the address manager. This RPC is for testing only.\n"

            "\nArguments\n"
            "1. \"address\"     (string, required) The IP address of the peer\n"
            "2. port            (numeric, required) The port of the peer\n"

            "\nResult:\n"
            "{\n"
            "  \"success\": true|false      (boolean) Whether the peer address was successfully added to the address manager\n"
            "}\n"

            "\nExamples:\n"
            + HelpExampleCli("addpeeraddress", "\"1.2.3.4\" 51472")
            + HelpExampleRpc("addpeeraddress", "\"1.2.3.4\", 51472"));
    }
    if (!g_connman) {
        throw JSONRPCError(RPC_CLIENT_P2P_DISABLED, "Error: Peer-to-peer functionality missing or disabled");
    }

    UniValue obj(UniValue::VOBJ);

    std::string addr_string = request.params[0].get_str();
    uint16_t port = request.params[1].get_int();

    CNetAddr net_addr;
    if (!LookupHost(addr_string, net_addr, false)) {
        obj.pushKV("success", false);
        return obj;
    }
    CAddress address = CAddress({net_addr, port}, ServiceFlags(NODE_NETWORK));
    address.nTime = GetAdjustedTime();
    // The source address is set equal to the address. This is equivalent to the peer
    // announcing itself.
    if (!g_connman->AddNewAddresses({address}, address)) {
        obj.pushKV("success", false);
        return obj;
    }

    obj.pushKV("success", true);
    return obj;
}

UniValue setnetworkactive(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() != 1) {
        throw std::runtime_error(
                "setnetworkactive \"true|false\"\n"
                "Disable/enable all p2p network activity.\n"

                "\nResult:\n"
                "status    (boolean) The final network activity status\n"
                "\nExamples:\n" +
                HelpExampleCli("setnetworkactive", "true") + HelpExampleRpc("setnetworkactive", "true"));
    }
    if (!g_connman) {
        throw JSONRPCError(RPC_CLIENT_P2P_DISABLED, "Error: Peer-to-peer functionality missing or disabled");
    }
    g_connman->SetNetworkActive(request.params[0].get_bool());
    return g_connman->GetNetworkActive();
}

static const CRPCCommand commands[] =
{ //  category              name                      actor (function)         okSafe argNames
  //  --------------------- ------------------------  -----------------------  ------ --------
    { "network",            "addnode",                &addnode,                true,  {"node","command"} },
    { "network",            "clearbanned",            &clearbanned,            true,  {} },
    { "network",            "disconnectnode",         &disconnectnode,         true,  {"node"} },
    { "network",            "getaddednodeinfo",       &getaddednodeinfo,       true,  {"dummy","node"} },
    { "network",            "getconnectioncount",     &getconnectioncount,     true,  {} },
    { "network",            "getnettotals",           &getnettotals,           true,  {} },
    { "network",            "getnetworkinfo",         &getnetworkinfo,         true,  {} },
    { "network",            "getnodeaddresses",       &getnodeaddresses,       true,  {"count"} },
    { "network",            "getpeerinfo",            &getpeerinfo,            true,  {} },
    { "network",            "listbanned",             &listbanned,             true,  {} },
    { "network",            "ping",                   &ping,                   true,  {} },
    { "network",            "setban",                 &setban,                 true,  {"subnet", "command", "bantime", "absolute"} },
    { "network",            "setnetworkactive",       &setnetworkactive,       true,  {"active"}},

    // Hidden, for testing only
    { "hidden",             "addpeeraddress",         &addpeeraddress,         true,  {"address", "port"} },
};

void RegisterNetRPCCommands(CRPCTable &tableRPC)
{
    for (unsigned int vcidx = 0; vcidx < ARRAYLEN(commands); vcidx++)
        tableRPC.appendCommand(commands[vcidx].name, &commands[vcidx]);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}
