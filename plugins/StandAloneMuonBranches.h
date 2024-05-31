//
// Original Author:
//                george karathanasis
//         Created:  Thu, 23 Mar 2019 17:40:23 GMT
//
// filling functions for aod and miniaod tag/probe

#ifndef MuonAnalysis_MuonAnalyzer_StandAloneMuonBranches
#define MuonAnalysis_MuonAnalyzer_StandAloneMuonBranches

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/MuonReco/interface/MuonSimInfo.h"
#include "MuonAnalysis/MuonAssociators/interface/PropagateToMuon.h"
#include "MuonAnalysis/MuonAssociators/interface/PropagateToMuonSetup.h"

#include <type_traits>
#include "StandAloneNtupleContent.h"
#include "NtupleContent.h"
#include "helper.h"

inline void StandAloneFillEventInfo(NtupleContent &nt,
                                    StandAloneNtupleContent &SA_nt,
                                    const std::vector<std::string> &HLTs) {
  //std::cout<< "One event filled!" << std::endl;
  SA_nt.run = nt.run;
  SA_nt.event = nt.event;
  //std::cout<< "Event :"<< SA_nt.event << std::endl;
  SA_nt.ls = nt.ls;
  SA_nt.istlumi = nt.istlumi;
  SA_nt.genWeight = nt.genWeight;
  SA_nt.BSpot_x = nt.BSpot_x;
  SA_nt.BSpot_y = nt.BSpot_y;
  SA_nt.BSpot_z = nt.BSpot_z;
  SA_nt.pv_x = nt.pv_x;
  SA_nt.pv_y = nt.pv_y;
  SA_nt.pv_z = nt.pv_z;
  SA_nt.nvertices = nt.nvertices;
  SA_nt.trueNumInteractions = nt.trueNumInteractions;
  SA_nt.puNumInteractions = nt.puNumInteractions;
  SA_nt.Rho = nt.Rho;
  SA_nt.nmuons = nt.nmuons;
  SA_nt.ntag = nt.ntag;

  // Gens
  SA_nt.genmu1_pt = nt.genmu1_pt;
  SA_nt.genmu1_eta = nt.genmu1_eta;
  SA_nt.genmu1_phi = nt.genmu1_phi;
  SA_nt.genmu1_charge = nt.genmu1_charge;
  SA_nt.genmu2_pt = nt.genmu2_pt;
  SA_nt.genmu2_eta = nt.genmu2_eta;
  SA_nt.genmu2_phi = nt.genmu2_phi;
  SA_nt.genmu2_charge = nt.genmu2_charge;
  SA_nt.genMass = nt.genMass;

  SA_nt.genmuFSfromHP1_pt = nt.genmuFSfromHP1_pt;
  SA_nt.genmuFSfromHP1_eta = nt.genmuFSfromHP1_eta;
  SA_nt.genmuFSfromHP1_phi = nt.genmuFSfromHP1_phi;
  SA_nt.genmuFSfromHP1_charge = nt.genmuFSfromHP1_charge;
  SA_nt.genmuFSfromHP2_pt = nt.genmuFSfromHP2_pt;
  SA_nt.genmuFSfromHP2_eta = nt.genmuFSfromHP2_eta;
  SA_nt.genmuFSfromHP2_phi = nt.genmuFSfromHP2_phi;
  SA_nt.genmuFSfromHP2_charge = nt.genmuFSfromHP2_charge;
  SA_nt.genMassFSfromHP = nt.genMassFSfromHP;

  for (unsigned int ihlt = 0; ihlt < HLTs.size(); ihlt++) {
    SA_nt.trigger[ihlt] = nt.trigger[ihlt];
    SA_nt.tag_trg[ihlt] = nt.tag_trg[ihlt];
    SA_nt.tag_trg_pt[ihlt] = nt.tag_trg_pt[ihlt];
    SA_nt.tag_trg_eta[ihlt] = nt.tag_trg_eta[ihlt];
    SA_nt.tag_trg_phi[ihlt] = nt.tag_trg_phi[ihlt];
    SA_nt.tag_trg_dr[ihlt] = nt.tag_trg_dr[ihlt];
    SA_nt.probe_trg[ihlt] = nt.probe_trg[ihlt];
    SA_nt.probe_trg_pt[ihlt] = nt.probe_trg_pt[ihlt];
    SA_nt.probe_trg_eta[ihlt] = nt.probe_trg_eta[ihlt];
    SA_nt.probe_trg_phi[ihlt] = nt.probe_trg_phi[ihlt];
    SA_nt.probe_trg_dr[ihlt] = nt.probe_trg_dr[ihlt];
  }
}

template <typename MUON, typename TRK>
inline void StandAloneFillTagBranches(const MUON &muon,
                                      const std::vector<TRK> &tracks,
                                      StandAloneNtupleContent &nt,
                                      const reco::Vertex &vertex) {
  nt.tag_pt = muon.pt();
  nt.tag_eta = muon.eta();
  nt.tag_phi = muon.phi();
  nt.tag_charge = muon.charge();
  nt.tag_pterr = muon.innerTrack()->ptError() / muon.innerTrack()->pt();
  nt.tag_dxy = muon.innerTrack()->dxy(reco::TrackBase::Point(nt.pv_x, nt.pv_y, nt.pv_z));
  nt.tag_dz = muon.vz();//innerTrack()->dz(reco::TrackBase::Point(nt.pv_x, nt.pv_y, nt.pv_z));
  nt.tag_isPF = muon.isPFMuon();
  nt.tag_isSA = muon.isStandAloneMuon();
  nt.tag_isTracker = muon.isTrackerMuon();
  nt.tag_isGlobal = muon.isGlobalMuon();
  // Use selectors instead of 'muon.passed' method which is only introduced in CMSSW_9_4_X
  nt.tag_isLoose = muon::isLooseMuon(muon);
  nt.tag_isMedium = muon::isMediumMuon(muon);
  nt.tag_isTight = muon::isTightMuon(muon, vertex);
  nt.tag_isSoft = muon::isSoftMuon(muon, vertex, false);
  nt.tag_isHighPt = muon::isHighPtMuon(muon, vertex);
  float iso04 = (TrackerEnergy04<TRK>(muon.eta(), muon.phi(), tracks) - muon.pt()) / muon.pt();
  nt.tag_relIso04 = (iso04 > 0) ? iso04 : 0;
  nt.tag_iso03_sumPt = muon.isolationR03().sumPt;
  nt.tag_pfIso04_charged = muon.pfIsolationR04().sumChargedHadronPt;
  nt.tag_pfIso04_neutral = muon.pfIsolationR04().sumNeutralHadronEt;
  nt.tag_pfIso04_photon = muon.pfIsolationR04().sumPhotonEt;
  nt.tag_pfIso04_sumPU = muon.pfIsolationR04().sumPUPt;
  if (muon.tunePMuonBestTrack().isNonnull()) {
    nt.tag_tuneP_pt = muon.tunePMuonBestTrack()->pt();
    nt.tag_tuneP_pterr = muon.tunePMuonBestTrack()->ptError();
  } else {
    nt.tag_tuneP_pt = -99.;
    nt.tag_tuneP_pterr = -99.;
  }
  int nsegments = 0;
  for (auto &chamber : muon.matches()) {
    if (chamber.id.det() != DetId::Muon)
      continue;
    if (chamber.id.subdetId() != MuonSubdetId::DT && chamber.id.subdetId() != MuonSubdetId::CSC)
      continue;
    nsegments += chamber.segmentMatches.size();
  }
  nt.tag_nsegments = nsegments;
}

template <typename TRK>
inline void StandAloneFillProbeBranches(
                                        const std::vector<TRK> &tracks,
                                        StandAloneNtupleContent &nt,
                                        const int match_muon_idx,
                                        const std::pair<std::vector<bool>, std::vector<reco::Track>> &match_tracks) {
  
  // Boolean success --> muon obj and track match in dR
  if (match_tracks.first.size() >= 1) { //more than one track
    if (match_tracks.first.at(0)) { //match with the first reco track
      reco::Track match_track = match_tracks.second.at(0); // store the first track
      nt.probe_isTrkMatch = true;
      nt.probe_trkPt = match_track.pt();
      nt.probe_trkEta = match_track.eta();
      nt.probe_trkPhi = match_track.phi();
      nt.probe_trkCharge = match_track.charge();
      nt.probe_trkDxy = match_track.dxy();
      nt.probe_trkDz = match_track.dz();
      nt.probe_trkHits = match_track.numberOfValidHits();
      nt.probe_trkStripHits = match_track.hitPattern().numberOfValidStripHits();
      nt.probe_trkPixelHits = match_track.hitPattern().numberOfValidPixelHits();
      
    }
  }
  if (match_tracks.first.size() >= 2) {
    if (match_tracks.first.at(1)) {
      reco::Track match_track = match_tracks.second.at(1);
      nt.probeSA_isTrkMatch = true;
      nt.probeSA_trkPt = match_track.pt();
      nt.probeSA_trkEta = match_track.eta();
      nt.probeSA_trkPhi = match_track.phi();
      nt.probeSA_trkCharge = match_track.charge();
      nt.probeSA_trkDxy = match_track.dxy();
      nt.probeSA_trkDz = match_track.dz();
      nt.probeSA_trkHits = match_track.numberOfValidHits();
      nt.probeSA_trkStripHits = match_track.hitPattern().numberOfValidStripHits();
      nt.probeSA_trkPixelHits = match_track.hitPattern().numberOfValidPixelHits();
      
    }
  }
  if (match_muon_idx <= -1) {

    nt.probe_mupt = -99;
    nt.probe_mueta = -99;
    nt.probe_muphi = -99;
    nt.probe_mucharge = -99;
    nt.probe_mu_SAmu_DeltaR = -99;
    nt.probe_isLoose = false;
    nt.probe_isMedium = false;
    nt.probe_isTight = false;
    nt.probe_isSoft = false;
    nt.probe_isHighPt = false;
    nt.probe_isMuMatched = false;
    nt.probe_isPF = false;
    nt.probe_isSA = false;
    nt.probe_isTracker = false;
    nt.probe_isGlobal = false;
    nt.probe_validFraction = -99;
    nt.probe_trkChi2 = -99;
    nt.probe_positionChi2 = -99;
    nt.probe_trkKink = -99;
    nt.probe_trackerLayers = -99;
    nt.probe_pixelLayers = -99;
    nt.probe_dxy = -99;
    nt.probe_dz = -99;
    nt.probe_muonStations = -99;
    nt.probe_muonHits = -99;
    nt.probe_DTHits = -99;
    nt.probe_CSCHits = -99;
    nt.probe_pterr = -99;
    nt.probe_iso03_sumPt = -99;
    nt.probe_pfIso04_charged = -99;
    nt.probe_pfIso04_neutral = -99;
    nt.probe_pfIso04_photon = -99;
    nt.probe_pfIso04_sumPU = -99;
    nt.probe_pixelHits = -99;
    nt.probe_matchedStations = -99;
    nt.probe_expectedMatchedStations = -99;
    nt.probe_RPCLayers = -99;
    nt.probe_stationMask = 0;
    nt.probe_nShowers = -99;
    nt.probe_nsegments = -99;
  }
}

template <typename MUO, typename TRK>
inline void StandAloneFillPairBranches(const MUO &muon, const TRK &trk, StandAloneNtupleContent &nt) {
  math::PtEtaPhiMLorentzVector mu1(muon.pt(), muon.eta(), muon.phi(), MU_MASS);
  math::PtEtaPhiMLorentzVector mu2(trk.pt(), trk.eta(), trk.phi(), MU_MASS);
  nt.pair_pt = (mu1 + mu2).pt();
  nt.pair_mass = (mu1 + mu2).mass();
  nt.pair_eta = (mu1 + mu2).eta();
  nt.pair_phi = (mu1 + mu2).phi();
  nt.pair_dz = muon.vz() - trk.vz();
  nt.pair_dR = deltaR(muon.eta(), muon.phi(), trk.eta(), trk.phi());
}

inline void StandAloneFillSimMatchingBranches(const pat::Muon &mu, StandAloneNtupleContent &nt, bool isTag) {
  if (isTag) {
    nt.tag_simType = mu.simType();
    nt.tag_simExtType = mu.simExtType();
    nt.tag_simFlavour = mu.simFlavour();
    nt.tag_simHeaviestMotherFlavour = mu.simHeaviestMotherFlavour();
    nt.tag_simPdgId = mu.simPdgId();
    nt.tag_simMotherPdgId = mu.simMotherPdgId();
    nt.tag_simBX = mu.simBX();
    nt.tag_simProdRho = mu.simProdRho();
    nt.tag_simProdZ = mu.simProdZ();
    nt.tag_simPt = mu.simPt();
    nt.tag_simEta = mu.simEta();
    nt.tag_simPhi = mu.simPhi();
  } else {
    nt.probe_simType = mu.simType();
    nt.probe_simExtType = mu.simExtType();
    nt.probe_simFlavour = mu.simFlavour();
    nt.probe_simHeaviestMotherFlavour = mu.simHeaviestMotherFlavour();
    nt.probe_simPdgId = mu.simPdgId();
    nt.probe_simMotherPdgId = mu.simMotherPdgId();
    nt.probe_simBX = mu.simBX();
    nt.probe_simProdRho = mu.simProdRho();
    nt.probe_simProdZ = mu.simProdZ();
    nt.probe_simPt = mu.simPt();
    nt.probe_simEta = mu.simEta();
    nt.probe_simPhi = mu.simPhi();
  }
}

inline void StandAloneFillSimMatchingBranchesDummy(StandAloneNtupleContent &nt, bool isTag) {
  if (isTag) {
    nt.tag_simType = -99;
    nt.tag_simExtType = -99;
    nt.tag_simFlavour = -99;
    nt.tag_simHeaviestMotherFlavour = -99;
    nt.tag_simPdgId = -99;
    nt.tag_simMotherPdgId = -99;
    nt.tag_simBX = -99;
    nt.tag_simProdRho = -99;
    nt.tag_simProdZ = -99;
    nt.tag_simPt = -99;
    nt.tag_simEta = -99;
    nt.tag_simPhi = -99;
  } else {
    nt.probe_simType = -99;
    nt.probe_simExtType = -99;
    nt.probe_simFlavour = -99;
    nt.probe_simHeaviestMotherFlavour = -99;
    nt.probe_simPdgId = -99;
    nt.probe_simMotherPdgId = -99;
    nt.probe_simBX = -99;
    nt.probe_simProdRho = -99;
    nt.probe_simProdZ = -99;
    nt.probe_simPt = -99;
    nt.probe_simEta = -99;
    nt.probe_simPhi = -99;
  }
}

inline void StandAloneFillSimMatchingBranchesAOD(const reco::MuonSimInfo &msi,
                                                 StandAloneNtupleContent &nt,
                                                 bool isTag) {
  if (isTag) {
    nt.tag_simType = msi.primaryClass;
    nt.tag_simExtType = msi.extendedClass;
    nt.tag_simFlavour = msi.flavour;
    nt.tag_simHeaviestMotherFlavour = msi.heaviestMotherFlavour;
    nt.tag_simPdgId = msi.pdgId;
    nt.tag_simMotherPdgId = msi.motherPdgId;
    nt.tag_simBX = msi.tpBX;
    nt.tag_simProdRho = msi.vertex.Rho();
    nt.tag_simProdZ = msi.vertex.Z();
    nt.tag_simPt = msi.p4.pt();
    nt.tag_simEta = msi.p4.eta();
    nt.tag_simPhi = msi.p4.phi();
  } else {
    nt.probe_simType = msi.primaryClass;
    nt.probe_simExtType = msi.extendedClass;
    nt.probe_simFlavour = msi.flavour;
    nt.probe_simHeaviestMotherFlavour = msi.heaviestMotherFlavour;
    nt.probe_simPdgId = msi.pdgId;
    nt.probe_simMotherPdgId = msi.motherPdgId;
    nt.probe_simBX = msi.tpBX;
    nt.probe_simProdRho = msi.vertex.Rho();
    nt.probe_simProdZ = msi.vertex.Z();
    nt.probe_simPt = msi.p4.pt();
    nt.probe_simEta = msi.p4.eta();
    nt.probe_simPhi = msi.p4.phi();
  }
}

#endif
