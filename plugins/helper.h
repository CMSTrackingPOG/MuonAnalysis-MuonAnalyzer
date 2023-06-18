//
// Original Author:
//                george karathanasis
//         Created:  Thu, 23 Mar 2019 17:40:23 GMT
//
// helper functions

#ifndef MuonAnalysis_MuonAnalyzer_plugins_helper
#define MuonAnalysis_MuonAnalyzer_plugins_helper
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/TrackReco/interface/Track.h"

const float MU_MASS = 0.10565837;
inline float DimuonMass(float mu1pt, float mu1eta, float mu1phi, float mu2pt, float mu2eta, float mu2phi) {
  math::PtEtaPhiMLorentzVector mu1(mu1pt, mu1eta, mu1phi, MU_MASS);
  math::PtEtaPhiMLorentzVector mu2(mu2pt, mu2eta, mu2phi, MU_MASS);
  return (mu1 + mu2).mass();
}

template <typename TRK>
inline float TrackerEnergy04(const float eta_muon, const float phi_muon, const std::vector<TRK> tracks) {
  float energy = 0;
  for (const auto& trk : tracks) {
    if (deltaR(eta_muon, phi_muon, trk.eta(), trk.phi()) > 0.4)
      continue;
    energy += trk.pt();
  }
  return energy;
}

template <typename TRK>
std::pair<bool, unsigned> MatchReco(
    const std::vector<TRK>& tracks, const float& eta, const float& phi, const int charge, const double& dr_max) {
  double minDR = 100;
  unsigned idx = 0;
  for (const auto& trk : tracks) {
    if (charge != trk.charge())
      continue;
    if (minDR < deltaR(eta, phi, trk.eta(), trk.phi()))
      continue;
    minDR = deltaR(eta, phi, trk.eta(), trk.phi());
    idx = &trk - &tracks[0];
  }
  if (minDR < dr_max)
    return std::make_pair(true, idx);
  else
    return std::make_pair(false, 0);
}

#endif
