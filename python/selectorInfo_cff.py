
# The standard muon selectors in reco::Muon::Selector
# Current version:
#    https://github.com/cms-sw/cmssw/blob/CMSSW_10_6_18/DataFormats/MuonReco/interface/Muon.h#L192-L227
# Description:
#    https://twiki.cern.ch/twiki/bin/viewauth/CMS/SWGuideMuonIdRun2#Muon_selectors_Since_9_4_X

import copy

muonSelectors = [
    ('CutBasedIdLoose'        ,  0),
    ('CutBasedIdMedium'       ,  1),
    ('CutBasedIdMediumPrompt' ,  2),
    ('CutBasedIdTight'        ,  3),
    ('CutBasedIdGlobalHighPt' ,  4),
    ('CutBasedIdTrkHighPt'    ,  5),
    ('PFIsoVeryLoose'         ,  6),
    ('PFIsoLoose'             ,  7),
    ('PFIsoMedium'            ,  8),
    ('PFIsoTight'             ,  9),
    ('PFIsoVeryTight'         , 10),
    ('TkIsoLoose'             , 11),
    ('TkIsoTight'             , 12),
    ('SoftCutBasedId'         , 13),
    ('SoftMvaId'              , 14),
    ('MvaLoose'               , 15),
    ('MvaMedium'              , 16),
    ('MvaTight'               , 17),
    ('MiniIsoLoose'           , 18),
    ('MiniIsoMedium'          , 19),
    ('MiniIsoTight'           , 20),
    ('MiniIsoVeryTight'       , 21),
    ('TriggerIdLoose'         , 22),
    ('InTimeMuon'             , 23),
    ('PFIsoVeryVeryTight'     , 24),
    ('MultiIsoLoose'          , 25),
    ('MultiIsoMedium'         , 26),
    ('PuppiIsoLoose'          , 27),
    ('PuppiIsoMedium'         , 28),
    ('PuppiIsoTight'          , 29),
    ('MvaVTight'              , 30),
    ('MvaVVTight'             , 31),
    ('LowPtMvaLoose'          , 32),
    ('LowPtMvaMedium'         , 33),
]

def getSelectorNamesAndBits(era, isFullAOD):
    _selectors = copy.deepcopy(muonSelectors)
    if era == 'Run2016':
        if isFullAOD:
            # 80X, no standard selector
            return [], []
        else:
            # 94X, MiniAOD
            _selectors = _selectors[:14]+_selectors[15:22]
    elif era == 'Run2017':
        if isFullAOD:
            # 94X, AOD
            _selectors = _selectors[:14]
        else:
            # 94X, MiniAOD
            _selectors = _selectors[:14]+_selectors[15:22]
    elif era == 'Run2018':
        if isFullAOD:
            # 102X, AOD
            _selectors = _selectors[:14]+_selectors[22:25]
        else:
            # 102X, MiniAOD
            _selectors = _selectors[:27]
    elif era == 'Run2016_UL_HIPM':
        pass
    elif era == 'Run2016_UL':
        pass
    elif era == 'Run2017_UL':
        pass
    elif era == 'Run2018_UL':
        pass
    elif era == 'Run2022':
        pass
    else:
        #print 'getSelectorNamesAndBits: undefined era "{}" -> return empty lists'.format(era)
        return [], []

    return list(map(list, zip(*_selectors)))

