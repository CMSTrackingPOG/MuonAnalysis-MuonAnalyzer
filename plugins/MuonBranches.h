//
// Original Author:
//                george karathanasis
//         Created:  Thu, 23 Mar 2019 17:40:23 GMT
//
// filling functions for aod and miniaod tag/probe

#ifndef MuonAnalysis_MuonAnalyzer_MuonBranches
#define MuonAnalysis_MuonAnalyzer_MuonBranches

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/Math/interface/deltaR.h"

#include <type_traits>
#include "NtupleContent.h"
#include "helper.h"

template <typename MUON, typename TRK>
inline void FillTagBranches(const MUON &muon,
                            const std::vector<TRK> &tracks,
                            NtupleContent &nt,
                            const reco::Vertex &vertex) {
  nt.tag_pt = muon.pt();
  nt.tag_eta = muon.eta();
  nt.tag_phi = muon.phi();
  nt.tag_charge = muon.charge();
  nt.tag_pterr = muon.innerTrack()->ptError() / muon.innerTrack()->pt();
  nt.tag_dxy = muon.innerTrack()->dxy(reco::TrackBase::Point(nt.pv_x, nt.pv_y, nt.pv_z));
  nt.tag_dz = muon.innerTrack()->dz(reco::TrackBase::Point(nt.pv_x, nt.pv_y, nt.pv_z));
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
}

template <typename MUON, typename TRK>
inline void FillProbeBranches(
    const MUON &mu, const std::vector<TRK> &tracks, NtupleContent &nt, bool success, const reco::Vertex &vertex) {
  nt.probe_pt = mu.pt();
  nt.probe_eta = mu.eta();
  nt.probe_phi = mu.phi();
  nt.probe_charge = mu.charge();
  float iso04 = (TrackerEnergy04<TRK>(mu.eta(), mu.phi(), tracks) - mu.pt()) / mu.pt();
  nt.probe_relIso04 = (iso04 > 0) ? iso04 : 0;
  // success --> muon obj and track match in dR
  if (success) {
    // Use selectors instead of 'mu.passed' method which is only introduced in CMSSW_9_4_X
    nt.probe_isLoose = muon::isLooseMuon(mu);
    nt.probe_isMedium = muon::isMediumMuon(mu);
    nt.probe_isTight = muon::isTightMuon(mu, vertex);
    nt.probe_isSoft = muon::isSoftMuon(mu, vertex, false);
    nt.probe_isHighPt = muon::isHighPtMuon(mu, vertex);
    nt.probe_isPF = mu.isPFMuon();
    nt.probe_isSA = mu.isStandAloneMuon();
    nt.probe_isTracker = mu.isTrackerMuon();
    nt.probe_isGlobal = mu.isGlobalMuon();
    nt.probe_iso03_sumPt = mu.isolationR03().sumPt;
    nt.probe_pfIso04_charged = mu.pfIsolationR04().sumChargedHadronPt;
    nt.probe_pfIso04_neutral = mu.pfIsolationR04().sumNeutralHadronEt;
    nt.probe_pfIso04_photon = mu.pfIsolationR04().sumPhotonEt;
    nt.probe_pfIso04_sumPU = mu.pfIsolationR04().sumPUPt;
    nt.probe_matchedStations = mu.numberOfMatchedStations();
    nt.probe_expectedMatchedStations = mu.expectedNnumberOfMatchedStations();
    nt.probe_RPCLayers = mu.numberOfMatchedRPCLayers();
    nt.probe_stationMask = mu.stationMask();
    nt.probe_nShowers = mu.numberOfShowers();
    if (mu.globalTrack().isNonnull())
      nt.probe_trkChi2 = mu.globalTrack()->normalizedChi2();
    else
      nt.probe_trkChi2 = -99;
    if (mu.innerTrack().isNonnull() && mu.innerTrack().isAvailable()) {
      nt.probe_validFraction = mu.innerTrack()->validFraction();
      nt.probe_trackerLayers = mu.innerTrack()->hitPattern().trackerLayersWithMeasurement();
      nt.probe_pixelLayers = mu.innerTrack()->hitPattern().pixelLayersWithMeasurement();
      nt.probe_pterr = mu.innerTrack()->ptError() / mu.innerTrack()->pt();
      nt.probe_dxy = mu.innerTrack()->dxy(reco::TrackBase::Point(nt.pv_x, nt.pv_y, nt.pv_z));
      nt.probe_dz = mu.innerTrack()->dz(reco::TrackBase::Point(nt.pv_x, nt.pv_y, nt.pv_z));
      nt.probe_pixelHits = mu.innerTrack()->hitPattern().numberOfValidPixelHits();
    } else {
      nt.probe_validFraction = -99;
      nt.probe_trackerLayers = -99;
      nt.probe_pixelLayers = -99;
      nt.probe_pterr = -99;
      nt.probe_dxy = -99;
      nt.probe_dz = -99;
      nt.probe_pixelHits = -99;
    }
    if (mu.outerTrack().isNonnull() && mu.outerTrack().isAvailable()) {
      nt.probe_muonStations = mu.outerTrack()->hitPattern().muonStationsWithValidHits();
      nt.probe_muonHits = mu.outerTrack()->hitPattern().numberOfValidMuonHits();
      nt.probe_DTHits = mu.outerTrack()->hitPattern().numberOfValidMuonDTHits();
      nt.probe_CSCHits = mu.outerTrack()->hitPattern().numberOfValidMuonCSCHits();
    } else {
      nt.probe_muonStations = -99;
      nt.probe_muonHits = -99;
      nt.probe_DTHits = -99;
      nt.probe_CSCHits = -99;
    }
    if (mu.tunePMuonBestTrack().isNonnull()) {
      nt.probe_tuneP_pt = mu.tunePMuonBestTrack()->pt();
      nt.probe_tuneP_pterr = mu.tunePMuonBestTrack()->ptError();
      nt.probe_tuneP_muonHits = mu.tunePMuonBestTrack()->hitPattern().numberOfValidMuonHits();
    } else {
      nt.probe_tuneP_pt = -99;
      nt.probe_tuneP_pterr = -99;
      nt.probe_tuneP_muonHits = -99;
    }
    nt.probe_positionChi2 = mu.combinedQuality().chi2LocalPosition;
    nt.probe_trkKink = mu.combinedQuality().trkKink;
    //     nt.probe_segmentCompatibility=
    nt.probe_isMuMatched = true;
  }
  // no successs (no match)
  else {
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
    nt.probe_tuneP_pt = -99;
    nt.probe_tuneP_pterr = -99;
    nt.probe_tuneP_muonHits = -99;
    nt.l1pt = -99;
    nt.l1q = -99;
    nt.l1dr = -99;
    nt.l1ptByQ = -99;
    nt.l1qByQ = -99;
    nt.l1drByQ = -99;
  }
}

template <typename MUON>
inline void FillProbeBranchesSelector(const MUON &mu,
                                      NtupleContent &nt,
                                      const std::vector<unsigned> selectorBits,
                                      bool success) {
  for (unsigned int ibit = 0; ibit < selectorBits.size(); ++ibit) {
    if (success && mu.selectors() != 0) {
      unsigned bit = selectorBits.at(ibit);
      nt.probe_selectors[ibit] = mu.passed(1UL << bit);
    } else {
      nt.probe_selectors[ibit] = false;
    }
  }
}

template <typename TRK>
inline void FillProbeBranchesdSA(const TRK &trk, NtupleContent &nt, bool passdSA) {
  nt.probe_isdSA = passdSA;

  nt.probe_dsa_pt = trk.pt();
  nt.probe_dsa_eta = trk.eta();
  nt.probe_dsa_phi = trk.phi();
  nt.probe_dsa_charge = trk.charge();

  if (passdSA) {
    nt.probe_dsa_dxy = trk.dxy(reco::TrackBase::Point(nt.pv_x, nt.pv_y, nt.pv_z));
    nt.probe_dsa_dz = trk.dz(reco::TrackBase::Point(nt.pv_x, nt.pv_y, nt.pv_z));
    nt.probe_dsa_muonStations = trk.hitPattern().muonStationsWithValidHits();
    nt.probe_dsa_muonHits = trk.hitPattern().numberOfValidMuonHits();
    nt.probe_dsa_DTHits = trk.hitPattern().numberOfValidMuonDTHits();
    nt.probe_dsa_CSCHits = trk.hitPattern().numberOfValidMuonCSCHits();
    nt.probe_dsa_pterr = trk.ptError() / trk.pt();
    nt.probe_dsa_trkChi2 = trk.normalizedChi2();
  } else {
    nt.probe_dsa_dxy = -99;
    nt.probe_dsa_dz = -99;
    nt.probe_dsa_muonStations = -99;
    nt.probe_dsa_muonHits = -99;
    nt.probe_dsa_DTHits = -99;
    nt.probe_dsa_CSCHits = -99;
    nt.probe_dsa_pterr = -99;
    nt.probe_dsa_trkChi2 = -99;
  }
}

template <typename TRK>
inline void FillProbeBranchesdgl(const TRK &trk, NtupleContent &nt, bool passdgl) {
  nt.probe_isdGlobal = passdgl;

  nt.probe_dgl_pt = trk.pt();
  nt.probe_dgl_eta = trk.eta();
  nt.probe_dgl_phi = trk.phi();
  nt.probe_dgl_charge = trk.charge();

  if (passdgl) {
    nt.probe_dgl_dxy = trk.dxy(reco::TrackBase::Point(nt.pv_x, nt.pv_y, nt.pv_z));
    nt.probe_dgl_dz = trk.dz(reco::TrackBase::Point(nt.pv_x, nt.pv_y, nt.pv_z));
    nt.probe_dgl_muonStations = trk.hitPattern().muonStationsWithValidHits();
    nt.probe_dgl_muonHits = trk.hitPattern().numberOfValidMuonHits();
    nt.probe_dgl_outerTrackerHits = trk.hitPattern().numberOfValidStripHits();
    nt.probe_dgl_trackerHits = trk.hitPattern().numberOfValidTrackerHits();
    nt.probe_dgl_totalHits = trk.hitPattern().numberOfValidHits();
    nt.probe_dgl_DTHits = trk.hitPattern().numberOfValidMuonDTHits();
    nt.probe_dgl_CSCHits = trk.hitPattern().numberOfValidMuonCSCHits();
    nt.probe_dgl_pterr = trk.ptError() / trk.pt();
    nt.probe_dgl_trkChi2 = trk.normalizedChi2();
  } else {
    nt.probe_dgl_dxy = -99;
    nt.probe_dgl_dz = -99;
    nt.probe_dgl_muonStations = -99;
    nt.probe_dgl_muonHits = -99;
    nt.probe_dgl_DTHits = -99;
    nt.probe_dgl_CSCHits = -99;
    nt.probe_dgl_pterr = -99;
    nt.probe_dgl_trkChi2 = -99;
  }
}

template <typename TRK>
inline void FillProbeBranchesCosmic(const TRK &trk, NtupleContent &nt, bool passcosmic) {
  nt.probe_isCosmic = passcosmic;
}

template <typename MUO, typename TRK>
inline void FillPairBranches(const MUO &muon, const TRK &trk, NtupleContent &nt) {
  math::PtEtaPhiMLorentzVector mu1(muon.pt(), muon.eta(), muon.phi(), MU_MASS);
  math::PtEtaPhiMLorentzVector mu2(trk.pt(), trk.eta(), trk.phi(), MU_MASS);
  nt.pair_pt = (mu1 + mu2).pt();
  nt.pair_mass = (mu1 + mu2).mass();
  nt.pair_eta = (mu1 + mu2).eta();
  nt.pair_phi = (mu1 + mu2).phi();
  nt.pair_dz = muon.vz() - trk.vz();
  nt.pair_dR = deltaR(muon.eta(), muon.phi(), trk.eta(), trk.phi());
}

template <typename MUO, typename TRK>
inline void FillTunePPairBranches(const MUO &muon, const TRK &trk, NtupleContent &nt) {
  math::PtEtaPhiMLorentzVector mu1(muon.pt(), muon.eta(), muon.phi(), MU_MASS);
  math::PtEtaPhiMLorentzVector mu2(trk.pt(), trk.eta(), trk.phi(), MU_MASS);
  nt.pair_tuneP_pt = (mu1 + mu2).pt();
  nt.pair_tuneP_mass = (mu1 + mu2).mass();
  nt.pair_tuneP_eta = (mu1 + mu2).eta();
  nt.pair_tuneP_phi = (mu1 + mu2).phi();
  nt.pair_tuneP_dz = muon.vz() - trk.vz();
  nt.pair_tuneP_dR = deltaR(muon.eta(), muon.phi(), trk.eta(), trk.phi());
}

inline void FillTunePPairBranchesDummy(NtupleContent &nt) {
  nt.pair_tuneP_pt = -99;
  nt.pair_tuneP_mass = -99;
  nt.pair_tuneP_eta = -99;
  nt.pair_tuneP_phi = -99;
  nt.pair_tuneP_dz = -99;
  nt.pair_tuneP_dR = -99;
  nt.pair_tuneP_fit_mass = -99;
  nt.pair_tuneP_svprob = -99;
  nt.pair_tuneP_normalchi2 = -99;
}

#endif
