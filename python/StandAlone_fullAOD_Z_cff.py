'''author gkarathanasis
option for AOD run'''

import FWCore.ParameterSet.Config as cms

muon = cms.EDAnalyzer('StandAloneMuonFullAODAnalyzer',
                      inputTag = cms.InputTag("LumiInfo", "brilcalc"),
                      isMC=cms.bool(False),
                      isOnlySeeded=cms.bool(False),
                      includeJets=cms.bool(True),
                      era = cms.string('dummy'), # updated in run_muonAnalyzer_cfg.py
                      genEventInfo = cms.InputTag('generator'),
                      pileupInfo=cms.InputTag('addPileupInfo'),
                      Rho=cms.InputTag('fixedGridRhoFastjetAll'),
                      beamSpot=cms.InputTag('offlineBeamSpot'),
                      vertices=cms.InputTag("offlinePrimaryVertices"),
                      muons=cms.InputTag("muons"),
                      tracks=cms.InputTag("generalTracks"),
                      SAmuons=cms.InputTag("standAloneMuons"),
                      dSAmuons=cms.InputTag("displacedStandAloneMuons"),
                      dGlmuons=cms.InputTag("displacedGlobalMuons"),
                      staCosmic=cms.InputTag("cosmicMuons"),
                      triggerResults=cms.InputTag("TriggerResults::HLT"),
                      triggerObjects=cms.InputTag('hltTriggerSummaryAOD'),
                      l1Matches = cms.InputTag("muonL1Info"),
                      l1MatchesQuality = cms.InputTag("muonL1Info", "quality"),
                      l1MatchesDeltaR = cms.InputTag("muonL1Info", "deltaR"),
                      l1MatchesByQ = cms.InputTag("muonL1InfoByQ"),
                      l1MatchesByQQuality = cms.InputTag("muonL1InfoByQ", "quality"),
                      l1MatchesByQDeltaR = cms.InputTag("muonL1InfoByQ", "deltaR"),
                      muonSimInfo = cms.InputTag("muonSimClassifier"),
                      triggerPaths=cms.vstring(), # updated in run_muonAnalyzer_cfg.py
                      tagFilters=cms.vstring(), # updated in run_muonAnalyzer_cfg.py
                      probeFilters=cms.vstring(), # updated in run_muonAnalyzer_cfg.py
                      probeSelectorNames=cms.vstring(), # updated in run_muonAnalyzer_cfg.py
                      probeSelectorBits=cms.vuint32(), # updated in run_muonAnalyzer_cfg.py
                      gen = cms.InputTag("genParticles"),
                      rhoJetsNC = cms.InputTag("fixedGridRhoFastjetAll"),
                      PFCands = cms.InputTag("particleFlow"),
                      jets = cms.InputTag("ak4PFJetsCHS"),
                      jetCorrector = cms.InputTag("ak4PFCHSL1FastL2L3Corrector"),
                      jetResType = cms.string("AK4PFchs_pt"),
                      jetResSFType = cms.string("AK4PFchs"),
                      genJets = cms.InputTag("ak4GenJets"),
                      deepCSVProbb = cms.InputTag("pfDeepCSVJetTags:probb"),
                      deepCSVProbbb = cms.InputTag("pfDeepCSVJetTags:probbb"),
                      deepFlavProbb = cms.InputTag("pfDeepFlavourJetTags:probb"),
                      deepFlavProbbb = cms.InputTag("pfDeepFlavourJetTags:probbb"),
                      trgDRwindow= cms.double(0.5), # dr window hlt mu/offline
                      tagQuality = cms.uint32(0),
                      tagSelection = cms.string("pt()>27"),
                      probeHPurity = cms.bool(False),
                      probeSelection = cms.string("pt()>10"),
                      probeSelectionSA = cms.string("pt()>10"),
                      muonOnly = cms.bool(True), # allow only reco or pat Muon for probes
                      probeMuonSelection = cms.string("pt()>10"), #string for probe (reco or pMuon)
                      pairMassMin = cms.double(40.0),
                      pairMassMax = cms.double(150.0), # 9999.0 for high mass C&C
                      pairDz = cms.double(1e99),
                      RequireVtxCreation = cms.bool(False),
                      minSVtxProb = cms.double(0.01),
                      maxDzProbeTrkMuon = cms.double(1.0), # max Dz(mu1,mu2)
                      maxRelPtProbeTrkMuon = cms.double(2.0),# max [pt(mu)-pt(trk)]/pt(trk) for probe/offline
                      maxDRProbeTrkMuon =  cms.double(0.3), # max DR for probe/offline
                      minPtTrkSA=cms.double(2.0),  # min Pt for trk matching for SA muons
                      maxDzProbeTrkSAMuon = cms.double(1.0), # max Dz(trk,SAmu)
                      maxRelPtProbeTrkSAMuon = cms.double(2.0),# max [pt(SAmu)-pt(trk)]/pt(trk) for probe/offline
                      maxDRProbeTrkSAMuon =  cms.double(0.3), # max DR for trk / SA muon
                      maxDRProbeTrkDSA =  cms.double(0.3), # max DR for general track and dSA
                      momPdgId= cms.uint32(23),
                      genRecoDrMatch = cms.double(0.03),
                      saveStandAloneTree = cms.bool(True),
                      saveTnPTree = cms.bool(True),
                      debug = cms.int32(0),
                      #propM1 = cms.PSet(
                      useStation2 = cms.bool(False),
                      useTrack = cms.string("tracker"),
                      useState = cms.string("atVertex"),  # in AOD
                      useSimpleGeometry = cms.bool(True), # use just one cylinder and two planes, not all the fancy chambers 
                      #),                                                        
                  )

fullAODSequence=cms.Sequence(muon)
