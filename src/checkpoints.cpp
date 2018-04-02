// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "txdb.h"
#include "main.h"
#include "uint256.h"


static const int nCheckpointSpan = 5000;

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
		(0, uint256("0x00003f25e94400925d8d6d4237c28556f99e5580764e1ce1bc841dd4033b22f0"))
		(3, uint256("0xdcc5d3973d6144d2da7c77b3c0e5f7160a4c00690e45b06f70766b421e3afc57"))
		(7, uint256("0x491b37199781fc444c91fb19f1250a86e5306d3e1aa5ac98883f4f6bac5ca3c5"))
		(9, uint256("0x52a9681e8405d937c49dab154e1d4ef1e10afb1769bf59a0300a4a8e86b619d5"))
		(70, uint256("0x2589f4203f62726bb029a2a71e1ece5c26dd6426bfc1f2971faf3d66243db835"))	
		(83, uint256("0x4cb30dc9d91108f2e7f0a00280d2f40799dcffe485fbfe2e2e3ece601f153879"))
		(92, uint256("0x8ba4492b248810a460eb0902c621bb129d31e892201e566f7749fc892b1dc8d4"))
		(348, uint256("0xf8f9fe5aa6cf4f0279536f57c7b41565b5db7803da962003986a7fa78d811637"))
		(1987, uint256("0x5397308a7b797b1b809a1e9d869e3c3291297dcfe92d55749cd4f4523d3fca54"))
		(3216, uint256("0xe77711dc00547b7ff9588b1ee4c96ceb7be9aaa0d0af7edb4169fc9e2c050ee0"))
		(5519, uint256("0xf666fb193b57faeb5b10eb1467941afdfc48a90dfbeb937139bdbabd9cd7fdcc"))
		(6511, uint256("0x2b448a6a5326db8854340ed7595a4ffc7866964b5803932c84ba7edfe6fbcf98"))		
		
 ;

    // TestNet has no checkpoints
    static MapCheckpoints mapCheckpointsTestnet;

    bool CheckHardened(int nHeight, const uint256& hash)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        if (checkpoints.empty())
            return 0;
        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }

    // Automatically select a suitable sync-checkpoint 
    const CBlockIndex* AutoSelectSyncCheckpoint()
    {
        const CBlockIndex *pindex = pindexBest;
        // Search backward for a block within max span and maturity window
        while (pindex->pprev && pindex->nHeight + nCheckpointSpan > pindexBest->nHeight)
            pindex = pindex->pprev;
        return pindex;
    }

    // Check against synchronized checkpoint
    bool CheckSync(int nHeight)
    {
        const CBlockIndex* pindexSync = AutoSelectSyncCheckpoint();
        if (nHeight <= pindexSync->nHeight){
            return false;
        }
        return true;
    }
}
