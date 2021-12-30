// Copyright (c) 2010 Satoshi Nakamoto
<<<<<<< HEAD
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
// Copyright (c) 2020-2021 The PRCY developers
=======
// Copyright (c) 2009-2015 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2020 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "chainparamsseeds.h"
<<<<<<< HEAD
#include "util.h"
#include "utilstrencodings.h"

#include <boost/assign/list_of.hpp>

#include <assert.h>

using namespace std;
using namespace boost::assign;

std::string CDNSSeedData::getHost(uint64_t requiredServiceBits) const {
    //use default host for non-filter-capable seeds or if we use the default service bits (NODE_NETWORK)
    if (!supportsServiceBitsFiltering || requiredServiceBits == NODE_NETWORK)
        return host;

    return strprintf("x%x.%s", requiredServiceBits, host);
}

=======
#include "consensus/merkle.h"
#include "tinyformat.h"
#include "utilstrencodings.h"

#include <assert.h>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.vtx.push_back(std::make_shared<const CTransaction>(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.nVersion = nVersion;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

void CChainParams::UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
{
    assert(IsRegTestNet()); // only available for regtest
    assert(idx > Consensus::BASE_NETWORK && idx < Consensus::MAX_NETWORK_UPGRADES);
    consensus.vUpgrades[idx].nActivationHeight = nActivationHeight;
}

/**
 * Build the genesis block. Note that the output of the genesis coinbase cannot
 * be spent as it did not originally exist in the database.
 *
 * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
 *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
 *   vMerkleTree: e0028e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "U.S. News & World Report Jan 28 2016 With His Absence, Trump Dominates Another Debate";
    const CScript genesisOutputScript = CScript() << ParseHex("04c10e83b2703ccf322f7dbd62dd5855ac7c10bd055814ce121ba32607d573b8810c02c0582aed05b4deb9c4b77b26d92428c61256cd42774babea0a073b2ed0c9") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

// this one is for testing only
static Consensus::LLMQParams llmq_test = {
        .type = Consensus::LLMQ_TEST,
        .name = "llmq_test",
        .size = 3,
        .minSize = 2,
        .threshold = 2,

        .dkgInterval = 60, // one DKG per hour
        .dkgPhaseBlocks = 6,
        .dkgMiningWindowStart = 30, // dkgPhaseBlocks * 5 = after finalization
        .dkgMiningWindowEnd = 40,
        .dkgBadVotesThreshold = 2,

        .signingActiveQuorumCount = 2, // just a few ones to allow easier testing

        .keepOldConnections = 3,
        .recoveryMembers = 3,
};

static Consensus::LLMQParams llmq50_60 = {
        .type = Consensus::LLMQ_50_60,
        .name = "llmq_50_60",
        .size = 50,
        .minSize = 40,
        .threshold = 30,

        .dkgInterval = 60, // one DKG per hour
        .dkgPhaseBlocks = 6,
        .dkgMiningWindowStart = 30, // dkgPhaseBlocks * 5 = after finalization
        .dkgMiningWindowEnd = 40,
        .dkgBadVotesThreshold = 40,

        .signingActiveQuorumCount = 24, // a full day worth of LLMQs

        .keepOldConnections = 25,
        .recoveryMembers = 25,
};

static Consensus::LLMQParams llmq400_60 = {
        .type = Consensus::LLMQ_400_60,
        .name = "llmq_400_60",
        .size = 400,
        .minSize = 300,
        .threshold = 240,

        .dkgInterval = 60 * 12, // one DKG every 12 hours
        .dkgPhaseBlocks = 10,
        .dkgMiningWindowStart = 50, // dkgPhaseBlocks * 5 = after finalization
        .dkgMiningWindowEnd = 70,
        .dkgBadVotesThreshold = 300,

        .signingActiveQuorumCount = 4, // two days worth of LLMQs

        .keepOldConnections = 5,
        .recoveryMembers = 100,
};

// Used for deployment and min-proto-version signaling, so it needs a higher threshold
static Consensus::LLMQParams llmq400_85 = {
        .type = Consensus::LLMQ_400_85,
        .name = "llmq_400_85",
        .size = 400,
        .minSize = 350,
        .threshold = 340,

        .dkgInterval = 60 * 24, // one DKG every 24 hours
        .dkgPhaseBlocks = 10,
        .dkgMiningWindowStart = 50, // dkgPhaseBlocks * 5 = after finalization
        .dkgMiningWindowEnd = 70, // give it a larger mining window to make sure it is mined
        .dkgBadVotesThreshold = 300,

        .signingActiveQuorumCount = 4, // four days worth of LLMQs

        .keepOldConnections = 5,
        .recoveryMembers = 100,
};

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */
<<<<<<< HEAD
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256("000006957e238ff4e6bcf00c8a7d1b3e7249c0a2109b0391d8740821a40c1d8c"))
    (500, uint256("00214da9b906c7c14558395b9bc88293301e6e5f87a714194079dd651293fadb")) // Final POW Block
    (561, uint256("54872c72e81b34117bc5a6095d6f1b8d85746992d2513d7adc90a2aceed1651e")) // First PoA Block
    (562, uint256("0f00d3a6636c8a265724764da082fdef8106fce7057dfdda94ab6537f7211b4f")) // First Block after PoA
    (14905, uint256("6389ecdb851500d9467b41a54d02c58b1542bfc2d5c99339821c89d25135a4b0")) // Chain split
    (17128, uint256("98d76615ef96b3ced1d9902715ba432393335b791b2256936883c323f0bb91f4"))
    (17133, uint256("d9dcec83e8a675db0f7b6d28fde591f494a1b0766f7cb56ea8d8cb95348f835b"))
    (17150, uint256("bad4dccf8fd86f00d6c215802d39342e4de64e21155b76b38f0182ba7d96edd2"))
    (17153, uint256("4e63d92ac5209f0a342c2e74098778bbe36324de66734ff41132c7f3db3ad628"))
    (18375, uint256("c291cf0a7bcd73a91c6f68d28dc6ce9e1acee973fd134f4250c1ee80bf3d0c03"))
    (18813, uint256("ccf401e6fb24f34c105b2f693c2eb75c2ba7a2a4e3ffbbfe152e98ae9806d418"))
    (18814, uint256("f8092b5c474cc260edb7c378c4f7e2123b007a55018d32f97b9b729f729067ae"))
    (19317, uint256("0bd530827eff9bdc79893739c02c14c02bb35a39b943eaeeb72e04767e0597a5"))    
    (19400, uint256("cdeebfe4fdeda461ab2025d08248e89b13a1e970c76b91ac2788837c147e6f33"))
    (77852, uint256("e6ec3ddccfb6bd378ca3cf5cc292c011ec235536a9edbb8c3fdbe17abd921d44"))
    (98715, uint256("04b3cfde139af89ddacf3cb790daf734e4953119c4a58da9b8f07be888271670"))
    (98768, uint256("43f42aaba4a1e8f7a4981b7d8620e5967974b0bbdee4ae22e42f14585be3a52f"))
    (105629, uint256("82ae47c00a0338f33a41b8505162cabaa4740f2187feb07f4ee4bc6138461acb"))
    (129267, uint256("773de63a6ef8cd4d56769a0efc8625fa617bbac34802019b3a211bbe390f34a6"))
    (132020, uint256("d93d3f4b85dd3f3995d010966f8e0163f324bfe3e748507fadf472a14c76ce36"))
    (133535, uint256("33d415384dc2d181f9cd0208d6c8664dfdbfb95a061a639220b4ea253df7788c"))
    (140352, uint256("cdef2002ee6d10a0a8e85ba47329455773245e6008aa691416b63d7ec3aef78d"))
    (155115, uint256("ea78ac399244d06b407ff349ba71747d8c672ccd54216317abc28dbca04c71e5")) // A PoA block was rejected here, avoid it
    (155116, uint256("929d16db920af3df60cf2e869ee08d174f7d476d65e53cbf07d54b7d1cca2380")) // First PoA Block after fix/difficulty bump
    (193949, uint256("98ed9238e67297071a13b6e62fa17c5c992998a295ea7535cdcd4c3dda8aeab3")) // First PoA Block after fix/difficulty bump
    (260162, uint256("97d593c9ebbcc219eeed822f05ba4291e7dcc4c3667836dbfe29667fe31808a7"))
    ;
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1623031617,          // * UNIX timestamp of last checkpoint block
    533276,          // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    1440        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("000001488be8bb442cd72cb737ade49a31de90dbbe5dce36f7d7e07f5dde2b77"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    0,
    0,
    0};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("690cbb5c7ae999de1de49948a3c109d3b15fe4de4297980de8ff0cbfe3c7823a"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    0,
    0,
    0};
=======
static MapCheckpoints mapCheckpoints = {
    { 259201, uint256S("1c9121bf9329a6234bfd1ea2d91515f19cd96990725265253f4b164283ade5dd")},
    { 424998, uint256S("f31e381eedb0ed3ed65fcc98cc71f36012bee32e8efd017c4f9fb0620fd35f6b")},
    { 616764, uint256S("29dd0bd1c59484f290896687b4ffb6a49afa5c498caf61967c69a541f8191557")}, //!< First block to use new modifierV1
    { 623933, uint256S("c7aafa648a0f1450157dc93bd4d7448913a85b7448f803b4ab970d91fc2a7da7")},
    { 791150, uint256S("8e76f462e4e82d1bd21cb72e1ce1567d4ddda2390f26074ffd1f5d9c270e5e50")},
    { 795000, uint256S("4423cceeb9fd574137a18733416275a70fdf95283cc79ad976ca399aa424a443")},
    { 863787, uint256S("5b2482eca24caf2a46bb22e0545db7b7037282733faa3a42ec20542509999a64")},
    { 863795, uint256S("2ad866818c4866e0d555181daccc628056216c0db431f88a825e84ed4f469067")},
    { 863805, uint256S("a755bd9a22b63c70d3db474f4b2b61a1f86c835b290a081bb3ec1ba2103eb4cb")},
    { 867733, uint256S("03b26296bf693de5782c76843d2fb649cb66d4b05550c6a79c047ff7e1c3ae15")},
    { 879650, uint256S("227e1d2b738b6cd83c46d1d64617934ec899d77cee34336a56e61b71acd10bb2")},
    { 895400, uint256S("7796a0274a608fac12d400198174e50beda992c1d522e52e5b95b884bc1beac6")}, //!< Block that serial# range is enforced
    { 895991, uint256S("d53013ed7ea5c325b9696c95e07667d6858f8ff7ee13fecfa90827bf3c9ae316")}, //!< Network split here
    { 908000, uint256S("202708f8c289b676fceb832a079ff6b308a28608339acbf7584de533619d014d")},
    {1142400, uint256S("98aff9d605bf123247f98b1e3a02567eb5799d208d78ec30fb89737b1c1f79c5")},
    {1679090, uint256S("f747ce055ba1b12e1f2e842bd480bc647210799359cb2e553ab292065e3419d6")}, //!< First block with a "wrapped" serial spend
    {1686229, uint256S("bb42bf1e886a7c23474634c90893dd3d68a6ccbfea4ac92a98da5cad0c6a6cb7")}, //!< Last block in the "wrapped" serial attack range
    {1778954, uint256S("0d3241268264a2908d6babf00d9cd1ffb83d93d7bb4e428820127fe227c2029c")}, //!< Network split here
    {1788528, uint256S("ea9243ff8fc079fdd7a04f11fac415de4d98e1bb0dc38db6f79f8f8bbfdbe496")}, //!< Network split here
    {2153200, uint256S("14e477e597d24549cac5e59d97d32155e6ec2861c1003b42d0566f9bf39b65d5")}, //!< First v7 block
    {2356049, uint256S("62e80d8e193bca84655fb78893b20f54a79f2d71124c4ea37b7ef51a0d5451c4")}, //!< Network split here
    {2365700, uint256S("b5d0beead57735539abc2db2b0b08cd65db3e5928efd3c3bf3182d5bf013f36c")}, //!< PIVX v4.1.1 enforced
    {2678402, uint256S("580a26ff0a45177a7a6f387f009c5b26140ea48b4790a857d9a796f8b3c25899")}, //!< Network split here
    {3014000, uint256S("78ad99b7225f73c42238bd7ca841ff700542b92bba75a0ef2ed351caa560f87f")}, //!< PIVX v5.3.0 enforced
    {3024000, uint256S("be4bc75afcfb9136924810f7483b2695089a366cc4ee27fd6dc3ecd5396e1f0f")}, //!< Superblock
};

static const CCheckpointData data = {
    &mapCheckpoints,
    1591401645, // * UNIX timestamp of last checkpoint block
    5607713,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the UpdateTip debug.log lines)
    3000        // * estimated number of transactions per day after checkpoint
};

static MapCheckpoints mapCheckpointsTestnet = {
    {0, uint256S("0x001")},
    //{    201, uint256S("6ae7d52092fd918c8ac8d9b1334400387d3057997e6e927a88e57186dc395231")},     // v5 activation (PoS/Sapling)
};

static const CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1454124731,
    0,
    3000};

static MapCheckpoints mapCheckpointsRegtest = {{0, uint256S("0x001")}};
static const CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1454124731,
    0,
    100};
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
<<<<<<< HEAD
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
=======
        strNetworkID = "main";

        genesis = CreateGenesisBlock(1454124731, 2402015, 0x1e0ffff0, 1, 250 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x0000041e482b9b9691d98eefb48473405c0b8ec31b76df3797c74a78680ef818"));
        assert(genesis.hashMerkleRoot == uint256S("0x1b2ef6e2f28be914103a277377ae7729dcd125dfeb8bf97bd5964ba72b6dc39b"));

        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.powLimit   = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimitV1 = uint256S("0x000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimitV2 = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nBudgetCycleBlocks = 43200;       // approx. 1 every 30 days
        consensus.nBudgetFeeConfirmations = 6;      // Number of confirmations for the finalization fee
        consensus.nCoinbaseMaturity = 100;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMaxMoneyOut = 21000000 * COIN;
        consensus.nMNCollateralAmt = 10000 * COIN;
        consensus.nMNBlockReward = 3 * COIN;
        consensus.nProposalEstablishmentTime = 60 * 60 * 24;    // must be at least a day old to make it into a budget
        consensus.nStakeMinAge = 60 * 60;
        consensus.nStakeMinDepth = 600;
        consensus.nTargetTimespan = 40 * 60;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 60;
        consensus.nTimeSlotLength = 15;
        consensus.nMaxProposalPayments = 6;

        // spork keys
        consensus.strSporkPubKey = "0410050aa740d280b134b40b40658781fc1116ba7700764e0ce27af3e1737586b3257d19232e0cb5084947f5107e44bcd577f126c9eb4a30ea2807b271d2145298";
        consensus.strSporkPubKeyOld = "040F129DE6546FE405995329A887329BED4321325B1A73B0A257423C05C1FCFE9E40EF0678AEF59036A22C42E61DFD29DF7EFB09F56CC73CADF64E05741880E3E7";
        consensus.nTime_EnforceNewSporkKey = 1608512400;    //!> December 21, 2020 01:00:00 AM GMT
        consensus.nTime_RejectOldSporkKey = 1614560400;     //!> March 1, 2021 01:00:00 AM GMT

        // height-based activations
        consensus.height_last_invalid_UTXO = 894538;
        consensus.height_last_ZC_AccumCheckpoint = 1686240;
        consensus.height_last_ZC_WrappedSerials = 1686229;

        // validation by-pass
        consensus.nPivxBadBlockTime = 1471401614;    // Skip nBit validation of Block 259201 per PR #915
        consensus.nPivxBadBlockBits = 0x1c056dac;    // Skip nBit validation of Block 259201 per PR #915

        // Zerocoin-related params
        consensus.ZC_Modulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
                "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
                "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
                "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
                "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
                "31438167899885040445364023527381951378636564391212010397122822120720357";
        consensus.ZC_MaxPublicSpendsPerTx = 637;    // Assume about 220 bytes each input
        consensus.ZC_MaxSpendsPerTx = 7;            // Assume about 20kb each input
        consensus.ZC_MinMintConfirmations = 20;
        consensus.ZC_MinMintFee = 1 * CENT;
        consensus.ZC_MinStakeDepth = 200;
        consensus.ZC_TimeStart = 1508214600;        // October 17, 2017 4:30:00 AM
        consensus.ZC_HeightStart = 863735;

        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_POS].nActivationHeight           = 259201;
        consensus.vUpgrades[Consensus::UPGRADE_POS_V2].nActivationHeight        = 615800;
        consensus.vUpgrades[Consensus::UPGRADE_ZC].nActivationHeight            = 863787;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].nActivationHeight         = 1153160;
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].nActivationHeight         = 1808634;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_PUBLIC].nActivationHeight     = 1880000;
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].nActivationHeight          = 1967000;
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].nActivationHeight          = 2153200;
        consensus.vUpgrades[Consensus::UPGRADE_V5_0].nActivationHeight          = 2700500;
        consensus.vUpgrades[Consensus::UPGRADE_V5_2].nActivationHeight          = 2927000;
        consensus.vUpgrades[Consensus::UPGRADE_V5_3].nActivationHeight          = 3014000;
        consensus.vUpgrades[Consensus::UPGRADE_V6_0].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;

        consensus.vUpgrades[Consensus::UPGRADE_ZC].hashActivationBlock =
                uint256S("0x5b2482eca24caf2a46bb22e0545db7b7037282733faa3a42ec20542509999a64");
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].hashActivationBlock =
                uint256S("0x37ea75fe1c9314171cff429a91b25b9f11331076d1c9de50ee4054d61877f8af");
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].hashActivationBlock =
                uint256S("0x82629b7a9978f5c7ea3f70a12db92633a7d2e436711500db28b97efd48b1e527");
        consensus.vUpgrades[Consensus::UPGRADE_ZC_PUBLIC].hashActivationBlock =
                uint256S("0xe2448b76d88d37aba4194ffed1041b680d779919157ddf5cbf423373d7f8078e");
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].hashActivationBlock =
                uint256S("0x0ef2556e40f3b9f6e02ce611b832e0bbfe7734a8ea751c7b555310ee49b61456");
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].hashActivationBlock =
                uint256S("0x14e477e597d24549cac5e59d97d32155e6ec2861c1003b42d0566f9bf39b65d5");

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
<<<<<<< HEAD
        pchMessageStart[0] = 0xf8;
        pchMessageStart[1] = 0x8e;
        pchMessageStart[2] = 0xa6;
        pchMessageStart[3] = 0x90;
        nDefaultPort = 59682;
        bnProofOfWorkLimit = ~uint256(0) >> 1; // PRCYcoin starting difficulty is 1 / 2^12
        nSubsidyHalvingInterval = 210000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 8100; // 75%
        nRejectBlockOutdatedMajority = 10260; // 95%
        nToCheckBlockUpgradeMajority = 10800; // Approximate expected amount of blocks in 7 days (1440*7.5)
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // PRCYcoin: 1 day
        nTargetSpacing = 1 * 60;  // PRCYcoin: 1 minute
        nMaturity = 100;
        nMasternodeCountDrift = 20;
        MAX_MONEY = 70000000.0;
        nMaxMoneyOut = MAX_MONEY * COIN;
        nMNCollateralAmt = 5000 * COIN;
        nMinimumStakeAmount = 2500 * COIN;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 500;
        nStartPOABlock = 560;
        nModifierUpdateBlock = 615800;
        nPoABlockTime = 60 * 60; //a PoA block every 60 minutes
        nMinNumPoSBlocks = 59;
        nMaxNumPoSBlocks = 65;
        nSoftForkBlock = 120000; // Soft fork block for difficulty change
        nPoANewDiff = 150000; // New PoA difficulty
        nBIP65ActivationHeight = 125000; // Last v3 block was 124712, leave a bit of padding
        nPoAFixTime = 1616716800; // Fork time for PoA fix - Friday, March 26, 2021 12:00:00 AM (GMT)
        nPoAPaddingBlock = 169869; // Last block with 120 PoS blocks in a PoA Audit
        nPoAPadding = 10; // Current PoA Padding
        nHardForkBlock = 375000; // Add hard fork block for Consensus/PoA Padding

        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         *
         * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
         *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
         *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
         *   vMerkleTree: e0028e
         */
        const char* pszTimestamp = "12 January 2021 PRCY Coin Development Team - Privacy is Your Right";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 0 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04b78f63269234b741668d85b57ba11edec2ee20f15719db180d5d6a37c4e9db0c494390fb54925934bc7b29f148a372c00273bbd5c939830d7d2941de6ce44b8b") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1610409600; // 1/12/2021 @ 12:00am (GMT)
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 28141687;

        //change blockhash from 0x00000e9468bba3df97334bfb5015543488a0fb38193c82b101472937590e3037 because of transaction structure change
        if(genesis.GetHash()!=uint256("000006957e238ff4e6bcf00c8a7d1b3e7249c0a2109b0391d8740821a40c1d8c"))
        {
            printf("Searchingforgenesisblock...\n");
            uint256 hashTarget=uint256().SetCompact(genesis.nBits);

            printf("hashTarget:%s\n",hashTarget.ToString().c_str());

            while(uint256(genesis.GetHash())>hashTarget)
            {
                printf("loop:%s\n",genesis.GetHash().ToString().c_str());
                ++genesis.nNonce;
                if(genesis.nNonce==0)
                {
                    printf("NONCEWRAPPED,incrementingtime");
                    std::cout<<std::string("NONCEWRAPPED,incrementingtime:\n");
                    ++genesis.nTime;
                }
                if(genesis.nNonce%10000==0)
                {
                    printf("Mainnet:nonce%08u:hash=%s\n",genesis.nNonce,genesis.GetHash().ToString().c_str());
                }
            }
            printf("block.nTime=%u\n",genesis.nTime);
            printf("block.nNonce=%u\n",genesis.nNonce);
            printf("block.GetHash=%s\n",genesis.GetHash().ToString().c_str());
            printf("hashMerkleRoot=%s\n",genesis.hashMerkleRoot.ToString().c_str());

        }



        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("000006957e238ff4e6bcf00c8a7d1b3e7249c0a2109b0391d8740821a40c1d8c"));
        assert(genesis.hashMerkleRoot == uint256("cd01f1ca20c22b336f1ee83af9fd8b7facbf42083bf3bed49af045f5cadc9cd4"));

        // nodes with support for servicebits filtering should be at the top
        vSeeds.push_back(CDNSSeedData("seed.prcycoin.com", "seed.prcycoin.com"));          // Single node address
        vSeeds.push_back(CDNSSeedData("seed1.prcycoin.com", "seed1.prcycoin.com"));        // Single node address
        vSeeds.push_back(CDNSSeedData("seed2.prcycoin.com", "seed2.prcycoin.com"));        // Single node address
        vSeeds.push_back(CDNSSeedData("seed3.prcycoin.com", "seed3.prcycoin.com"));        // Single node address
        vSeeds.push_back(CDNSSeedData("seed4.prcycoin.com", "seed4.prcycoin.com"));        // Single node address
        vSeeds.push_back(CDNSSeedData("seed5.prcycoin.com", "seed5.prcycoin.com"));        // Single node address
        vSeeds.push_back(CDNSSeedData("seed6.prcycoin.com", "seed6.prcycoin.com"));        // Single node address
        vSeeds.push_back(CDNSSeedData("seed7.prcycoin.com", "seed7.prcycoin.com"));        // Single node address
        vSeeds.push_back(CDNSSeedData("seed8.prcycoin.com", "seed8.prcycoin.com"));        // Single node address
        vSeeds.push_back(CDNSSeedData("seed9.prcycoin.com", "seed9.prcycoin.com"));        // Single node address
        vSeeds.push_back(CDNSSeedData("vps.prcycoin.com", "vps.prcycoin.com"));            // Single node address
        vSeeds.push_back(CDNSSeedData("vps1.prcycoin.com", "vps1.prcycoin.com"));          // Single node address
        vSeeds.push_back(CDNSSeedData("vps2.prcycoin.com", "vps2.prcycoin.com"));          // Single node address
        vSeeds.push_back(CDNSSeedData("vps3.prcycoin.com", "vps3.prcycoin.com"));          // Single node address
        vSeeds.push_back(CDNSSeedData("vps4.prcycoin.com", "vps4.prcycoin.com"));          // Single node address
        vSeeds.push_back(CDNSSeedData("vps5.prcycoin.com", "vps5.prcycoin.com"));          // Single node address
        vSeeds.push_back(CDNSSeedData("vps6.prcycoin.com", "vps6.prcycoin.com"));          // Single node address
        vSeeds.push_back(CDNSSeedData("vps7.prcycoin.com", "vps7.prcycoin.com"));          // Single node address
        vSeeds.push_back(CDNSSeedData("vps8.prcycoin.com", "vps8.prcycoin.com"));          // Single node address
        vSeeds.push_back(CDNSSeedData("vps9.prcycoin.com", "vps9.prcycoin.com"));          // Single node address

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 55);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 61);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 28);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x57)(0x4D)(0x12)(0x23).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x57)(0x41)(0x71)(0x65).convert_to_container<std::vector<unsigned char> >();
        //  BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        nExtCoinType = 0x80000355;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strObfuscationPoolDummyAddress = "D87q2gC9j6nNrnzCsg4aY6bHMLsT9nUhEw";
        nStartMasternodePayments = 1546809115; //Wed, 25 Jun 2014 20:36:16 GMT
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
=======
        pchMessageStart[0] = 0x90;
        pchMessageStart[1] = 0xc4;
        pchMessageStart[2] = 0xfd;
        pchMessageStart[3] = 0xe9;
        nDefaultPort = 51472;

        // Note that of those with the service bits flag, most only support a subset of possible options
        vSeeds.emplace_back("pivx.seed.fuzzbawls.pw", true);     // Primary DNS Seeder from Fuzzbawls
        vSeeds.emplace_back("pivx.seed2.fuzzbawls.pw", true);    // Secondary DNS Seeder from Fuzzbawls
        vSeeds.emplace_back("pivx-seed.furszy.net", true);     // Primary DNS Seeder from furszy

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 30);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 13);
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 63);     // starting with 'S'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 212);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x02, 0x2D, 0x25, 0x33};
        base58Prefixes[EXT_SECRET_KEY] = {0x02, 0x21, 0x31, 0x2B};
        // BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = {0x80, 0x00, 0x00, 0x77};

        vFixedSeeds = std::vector<uint8_t>(std::begin(chainparams_seed_main), std::end(chainparams_seed_main));

        // Reject non-standard transactions by default
        fRequireStandard = true;

        // Sapling
        bech32HRPs[SAPLING_PAYMENT_ADDRESS]      = "ps";
        bech32HRPs[SAPLING_FULL_VIEWING_KEY]     = "pviews";
        bech32HRPs[SAPLING_INCOMING_VIEWING_KEY] = "pivks";
        bech32HRPs[SAPLING_EXTENDED_SPEND_KEY]   = "p-secret-spending-key-main";
        bech32HRPs[SAPLING_EXTENDED_FVK]         = "pxviews";

        // long living quorum params
        consensus.llmqs[Consensus::LLMQ_50_60] = llmq50_60;
        consensus.llmqs[Consensus::LLMQ_400_60] = llmq400_60;
        consensus.llmqs[Consensus::LLMQ_400_85] = llmq400_85;
    }

    const CCheckpointData& Checkpoints() const
    {
        return data;
    }

};

/**
 * Testnet (v5)
 */
class CTestNetParams : public CChainParams
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
public:
    CTestNetParams()
    {
<<<<<<< HEAD
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0xc1;
        pchMessageStart[1] = 0xaa;
        pchMessageStart[2] = 0xb2;
        pchMessageStart[3] = 0xe9;
        nDefaultPort = 59684;
        nEnforceBlockUpgradeMajority = 4320; // 75%
        nRejectBlockOutdatedMajority = 5472; // 95%
        nToCheckBlockUpgradeMajority = 5760; // 4 days
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // PRCYcoin: 1 day
        nTargetSpacing = 1 * 60;  // PRCYcoin: 1 minute
        nLastPOWBlock = 500;
        nStartPOABlock = 560;
        nPoABlockTime = 30 * 60;  //1 PoA block every 30 minutes
        nMinNumPoSBlocks = 29;
        nMaxNumPoSBlocks = 33;
        nMaturity = 15;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 51197; //approx Mon, 17 Apr 2017 04:00:00 GMT
        MAX_MONEY = 70000000.0;
        nMaxMoneyOut = MAX_MONEY * COIN;
        nMNCollateralAmt = 5000 * COIN;
        nSoftForkBlock = 600; // Soft fork block for difficulty change - testnet started with it
        nPoANewDiff = 650;
        nBIP65ActivationHeight = 0;
        nPoAFixTime = 1616277580; // Fork time for PoA fix - Saturday, March 20, 2021 22:00:00 AM (GMT)
        nPoAPaddingBlock = 0;
        nPoAPadding = 5; // Current PoA Padding
        nHardForkBlock = 700; // Add hard fork block for Consensus/PoA Padding

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1608422400;
        genesis.nNonce = 23323155;

        if(genesis.GetHash()!=uint256("000001488be8bb442cd72cb737ade49a31de90dbbe5dce36f7d7e07f5dde2b77"))
        {
            printf("Searchingforgenesisblock...\n");
            uint256 hashTarget=uint256().SetCompact(genesis.nBits);

            printf("hashTarget:%s\n",hashTarget.ToString().c_str());

            while(uint256(genesis.GetHash())>hashTarget)
            {
                printf("loop:%s\n",genesis.GetHash().ToString().c_str());
                ++genesis.nNonce;
                if(genesis.nNonce==0)
                {
                    printf("NONCEWRAPPED,incrementingtime");
                    std::cout<<std::string("NONCEWRAPPED,incrementingtime:\n");
                    ++genesis.nTime;
                }
                if(genesis.nNonce%10000==0)
                {
                    printf("Mainnet:nonce%08u:hash=%s\n",genesis.nNonce,genesis.GetHash().ToString().c_str());
                }
            }
            printf("block.nTime=%u\n",genesis.nTime);
            printf("block.nNonce=%u\n",genesis.nNonce);
            printf("block.GetHash=%s\n",genesis.GetHash().ToString().c_str());
            printf("hashMerkleRoot=%s\n",genesis.hashMerkleRoot.ToString().c_str());

        }

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("000001488be8bb442cd72cb737ade49a31de90dbbe5dce36f7d7e07f5dde2b77"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.push_back(CDNSSeedData("testnet.prcycoin-seeds.com", "testnet.prcycoin-seeds.com"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 139); // Testnet prcycoin addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // Testnet prcycoin script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet prcycoin BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        // Testnet prcycoin BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        // Testnet prcycoin BIP44 coin type is '1' (All coin's testnet default)
        nExtCoinType = 0x80000001;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strObfuscationPoolDummyAddress = "y57cqfGRkekRyDRNeJiLtYVEbvhXrNbmox";
        nStartMasternodePayments = 1420837558; //Fri, 09 Jan 2015 21:05:58 GMT
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
=======
        strNetworkID = "test";

        genesis = CreateGenesisBlock(1454124731, 2402015, 0x1e0ffff0, 1, 250 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x0000041e482b9b9691d98eefb48473405c0b8ec31b76df3797c74a78680ef818"));
        assert(genesis.hashMerkleRoot == uint256S("0x1b2ef6e2f28be914103a277377ae7729dcd125dfeb8bf97bd5964ba72b6dc39b"));

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.powLimit   = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimitV1 = uint256S("0x000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimitV2 = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nBudgetCycleBlocks = 144;         // approx 10 cycles per day
        consensus.nBudgetFeeConfirmations = 3;      // (only 8-blocks window for finalization on testnet)
        consensus.nCoinbaseMaturity = 15;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMaxMoneyOut = 21000000 * COIN;
        consensus.nMNCollateralAmt = 10000 * COIN;
        consensus.nMNBlockReward = 3 * COIN;
        consensus.nProposalEstablishmentTime = 60 * 5;  // at least 5 min old to make it into a budget
        consensus.nStakeMinAge = 60 * 60;
        consensus.nStakeMinDepth = 100;
        consensus.nTargetTimespan = 40 * 60;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 60;
        consensus.nTimeSlotLength = 15;
        consensus.nMaxProposalPayments = 20;

        // spork keys
        consensus.strSporkPubKey = "04677c34726c491117265f4b1c83cef085684f36c8df5a97a3a42fc499316d0c4e63959c9eca0dba239d9aaaf72011afffeb3ef9f51b9017811dec686e412eb504";
        consensus.strSporkPubKeyOld = "04E88BB455E2A04E65FCC41D88CD367E9CCE1F5A409BE94D8C2B4B35D223DED9C8E2F4E061349BA3A38839282508066B6DC4DB72DD432AC4067991E6BF20176127";
        consensus.nTime_EnforceNewSporkKey = 1608512400;    //!> December 21, 2020 01:00:00 AM GMT
        consensus.nTime_RejectOldSporkKey = 1614560400;     //!> March 1, 2021 01:00:00 AM GMT

        // height based activations
        consensus.height_last_invalid_UTXO = -1;
        consensus.height_last_ZC_AccumCheckpoint = -1;
        consensus.height_last_ZC_WrappedSerials = -1;
        consensus.ZC_HeightStart = 0;

        // Zerocoin-related params
        consensus.ZC_Modulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
                "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
                "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
                "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
                "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
                "31438167899885040445364023527381951378636564391212010397122822120720357";
        consensus.ZC_MaxPublicSpendsPerTx = 637;    // Assume about 220 bytes each input
        consensus.ZC_MaxSpendsPerTx = 7;            // Assume about 20kb each input
        consensus.ZC_MinMintConfirmations = 20;
        consensus.ZC_MinMintFee = 1 * CENT;
        consensus.ZC_MinStakeDepth = 200;
        consensus.ZC_TimeStart = 1508214600;        // October 17, 2017 4:30:00 AM

        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_POS].nActivationHeight           = 201;
        consensus.vUpgrades[Consensus::UPGRADE_POS_V2].nActivationHeight        = 201;
        consensus.vUpgrades[Consensus::UPGRADE_ZC].nActivationHeight            = 201;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].nActivationHeight         = 201;
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].nActivationHeight         = 201;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_PUBLIC].nActivationHeight     = 201;
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].nActivationHeight          = 201;
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].nActivationHeight          = 201;
        consensus.vUpgrades[Consensus::UPGRADE_V5_0].nActivationHeight          = 201;
        consensus.vUpgrades[Consensus::UPGRADE_V5_2].nActivationHeight          = 262525;
        consensus.vUpgrades[Consensus::UPGRADE_V5_3].nActivationHeight          = 332300;
        consensus.vUpgrades[Consensus::UPGRADE_V6_0].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xf5;
        pchMessageStart[1] = 0xe6;
        pchMessageStart[2] = 0xd5;
        pchMessageStart[3] = 0xca;
        nDefaultPort = 51474;

        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("pivx-testnet.seed.fuzzbawls.pw", true);
        vSeeds.emplace_back("pivx-testnet.seed2.fuzzbawls.pw", true);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 139); // Testnet pivx addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // Testnet pivx script addresses start with '8' or '9'
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 73);     // starting with 'W'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet pivx BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = {0x3a, 0x80, 0x61, 0xa0};
        // Testnet pivx BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = {0x3a, 0x80, 0x58, 0x37};
        // Testnet pivx BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = {0x80, 0x00, 0x00, 0x01};

        vFixedSeeds = std::vector<uint8_t>(std::begin(chainparams_seed_test), std::end(chainparams_seed_test));

        fRequireStandard = false;

        // Sapling
        bech32HRPs[SAPLING_PAYMENT_ADDRESS]      = "ptestsapling";
        bech32HRPs[SAPLING_FULL_VIEWING_KEY]     = "pviewtestsapling";
        bech32HRPs[SAPLING_INCOMING_VIEWING_KEY] = "pivktestsapling";
        bech32HRPs[SAPLING_EXTENDED_SPEND_KEY]   = "p-secret-spending-key-test";
        bech32HRPs[SAPLING_EXTENDED_FVK]         = "pxviewtestsapling";

        // long living quorum params
        consensus.llmqs[Consensus::LLMQ_50_60] = llmq50_60;
        consensus.llmqs[Consensus::LLMQ_400_60] = llmq400_60;
        consensus.llmqs[Consensus::LLMQ_400_85] = llmq400_85;
    }

    const CCheckpointData& Checkpoints() const
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    {
        return dataTestnet;
    }
};
<<<<<<< HEAD
static CTestNetParams testNetParams;
=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

/**
 * Regression test
 */
<<<<<<< HEAD
class CRegTestParams : public CTestNetParams
=======
class CRegTestParams : public CChainParams
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
public:
    CRegTestParams()
    {
<<<<<<< HEAD
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xc6;
        pchMessageStart[1] = 0xb3;
        pchMessageStart[2] = 0x97;
        pchMessageStart[3] = 0xd1;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // Prcycoin: 1 day
        nTargetSpacing = 1 * 60;        // Prcycoin: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1608422399;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 12361;

        if(genesis.GetHash()!=uint256("690cbb5c7ae999de1de49948a3c109d3b15fe4de4297980de8ff0cbfe3c7823a"))
        {
            printf("Searchingforgenesisblock...\n");
            uint256 hashTarget=uint256().SetCompact(genesis.nBits);

            printf("hashTarget:%s\n",hashTarget.ToString().c_str());

            while(uint256(genesis.GetHash())>hashTarget)
            {
                printf("loop:%s\n",genesis.GetHash().ToString().c_str());
                ++genesis.nNonce;
                if(genesis.nNonce==0)
                {
                    printf("NONCEWRAPPED,incrementingtime");
                    std::cout<<std::string("NONCEWRAPPED,incrementingtime:\n");
                    ++genesis.nTime;
                }
                if(genesis.nNonce%10000==0)
                {
                    printf("Mainnet:nonce%08u:hash=%s\n",genesis.nNonce,genesis.GetHash().ToString().c_str());
                }
            }
            printf("block.nTime=%u\n",genesis.nTime);
            printf("block.nNonce=%u\n",genesis.nNonce);
            printf("block.GetHash=%s\n",genesis.GetHash().ToString().c_str());
            printf("hashMerkleRoot=%s\n",genesis.hashMerkleRoot.ToString().c_str());

        }

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 51476;

        assert(hashGenesisBlock == uint256("690cbb5c7ae999de1de49948a3c109d3b15fe4de4297980de8ff0cbfe3c7823a"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
=======
        strNetworkID = "regtest";

        genesis = CreateGenesisBlock(1454124731, 1, 0x207fffff, 1, 250 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x7445589c4c8e52b105247b13373e5ee325856aa05d53f429e59ea46b7149ae3f"));
        assert(genesis.hashMerkleRoot == uint256S("0x1b2ef6e2f28be914103a277377ae7729dcd125dfeb8bf97bd5964ba72b6dc39b"));

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.powLimit   = uint256S("0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimitV1 = uint256S("0x000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimitV2 = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nBudgetCycleBlocks = 144;         // approx 10 cycles per day
        consensus.nBudgetFeeConfirmations = 3;      // (only 8-blocks window for finalization on regtest)
        consensus.nCoinbaseMaturity = 100;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMaxMoneyOut = 43199500 * COIN;
        consensus.nMNCollateralAmt = 100 * COIN;
        consensus.nMNBlockReward = 3 * COIN;
        consensus.nProposalEstablishmentTime = 60 * 5;  // at least 5 min old to make it into a budget
        consensus.nStakeMinAge = 0;
        consensus.nStakeMinDepth = 20;
        consensus.nTargetTimespan = 40 * 60;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 60;
        consensus.nTimeSlotLength = 15;
        consensus.nMaxProposalPayments = 20;

        /* Spork Key for RegTest:
        WIF private key: 932HEevBSujW2ud7RfB1YF91AFygbBRQj3de3LyaCRqNzKKgWXi
        private key hex: bd4960dcbd9e7f2223f24e7164ecb6f1fe96fc3a416f5d3a830ba5720c84b8ca
        Address: yCvUVd72w7xpimf981m114FSFbmAmne7j9
        */
        consensus.strSporkPubKey = "043969b1b0e6f327de37f297a015d37e2235eaaeeb3933deecd8162c075cee0207b13537618bde640879606001a8136091c62ec272dd0133424a178704e6e75bb7";
        consensus.strSporkPubKeyOld = "";
        consensus.nTime_EnforceNewSporkKey = 0;
        consensus.nTime_RejectOldSporkKey = 0;

        // height based activations
        consensus.height_last_invalid_UTXO = -1;
        consensus.height_last_ZC_AccumCheckpoint = 310;     // no checkpoints on regtest
        consensus.height_last_ZC_WrappedSerials = -1;

        // Zerocoin-related params
        consensus.ZC_Modulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
                "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
                "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
                "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
                "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
                "31438167899885040445364023527381951378636564391212010397122822120720357";
        consensus.ZC_MaxPublicSpendsPerTx = 637;    // Assume about 220 bytes each input
        consensus.ZC_MaxSpendsPerTx = 7;            // Assume about 20kb each input
        consensus.ZC_MinMintConfirmations = 10;
        consensus.ZC_MinMintFee = 1 * CENT;
        consensus.ZC_MinStakeDepth = 10;
        consensus.ZC_TimeStart = 0;                 // not implemented on regtest
        consensus.ZC_HeightStart = 0;

        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_POS].nActivationHeight           = 251;
        consensus.vUpgrades[Consensus::UPGRADE_POS_V2].nActivationHeight        = 251;
        consensus.vUpgrades[Consensus::UPGRADE_ZC].nActivationHeight            = 300;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].nActivationHeight         = 300;
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].nActivationHeight         =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_PUBLIC].nActivationHeight     = 400;
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].nActivationHeight          = 251;
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].nActivationHeight          =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_V5_0].nActivationHeight          = 300;
        consensus.vUpgrades[Consensus::UPGRADE_V5_2].nActivationHeight          = 300;
        consensus.vUpgrades[Consensus::UPGRADE_V5_3].nActivationHeight          = 251;
        consensus.vUpgrades[Consensus::UPGRADE_V6_0].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xa1;
        pchMessageStart[1] = 0xcf;
        pchMessageStart[2] = 0x7e;
        pchMessageStart[3] = 0xac;
        nDefaultPort = 51476;

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 139); // Testnet pivx addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // Testnet pivx script addresses start with '8' or '9'
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 73);     // starting with 'W'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet pivx BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = {0x3a, 0x80, 0x61, 0xa0};
        // Testnet pivx BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = {0x3a, 0x80, 0x58, 0x37};
        // Testnet pivx BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = {0x80, 0x00, 0x00, 0x01};

        // Reject non-standard transactions by default
        fRequireStandard = true;

        // Sapling
        bech32HRPs[SAPLING_PAYMENT_ADDRESS]      = "ptestsapling";
        bech32HRPs[SAPLING_FULL_VIEWING_KEY]     = "pviewtestsapling";
        bech32HRPs[SAPLING_INCOMING_VIEWING_KEY] = "pivktestsapling";
        bech32HRPs[SAPLING_EXTENDED_SPEND_KEY]   = "p-secret-spending-key-test";
        bech32HRPs[SAPLING_EXTENDED_FVK]         = "pxviewtestsapling";

        // long living quorum params
        consensus.llmqs[Consensus::LLMQ_TEST] = llmq_test;
    }

    const CCheckpointData& Checkpoints() const
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    {
        return dataRegtest;
    }
};
<<<<<<< HEAD
static CRegTestParams regTestParams;

static CChainParams* pCurrentParams = 0;

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
=======

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params()
{
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
{
    globalChainParams->UpdateNetworkUpgradeParameters(idx, nActivationHeight);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}
