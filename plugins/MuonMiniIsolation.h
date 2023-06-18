//
// Original Author:
//                wing yan wong (jess)
//         Created:  Wed, 14 Oct 2020 19:40:23 GMT
//
// functions for getting miniIsolation of muons

#ifndef MuonAnalysis_MuonAnalyzer_MuonMiniIsolation
#define MuonAnalysis_MuonAnalyzer_MuonMiniIsolation

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/Math/interface/deltaR.h"

#include <type_traits>
#include "NtupleContent.h"
#include "helper.h"

template <typename PFCANDS>
inline bool is_fromPV(PFCANDS &pfc) {
  return false;
}

template <>
inline bool is_fromPV<reco::PFCandidate>(reco::PFCandidate &pfc) {
  if (pfc.bestTrack())
    return (pfc.bestTrack()->dz() < 0.0 ? true : false);
  else
    return false;
}

template <>
inline bool is_fromPV<pat::PackedCandidate>(pat::PackedCandidate &pfc) {
  return (pfc.fromPV() > 1 ? true : false);
}

template <typename MUON, typename PFCANDS>
inline void FillMiniIso(
    const std::vector<PFCANDS> &pfcands, const MUON &mu, const double rho, NtupleContent &nt, bool isTag) {
  // define variables for Muons
  double r_iso_min = 0.05, r_iso_max = 0.2, kt_scale = 10.;
  double deadcone_ch = 0.0001, deadcone_pu = 0.01;
  double deadcone_ph = 0.01, deadcone_nh = 0.01;
  double ptThresh(0.5);

  // initialize variables
  double iso(-1.), iso_nh(0.), iso_ch(0.), iso_ph(0.), iso_pu(0.);

  //if (mu.pt()<5.){
  //  iso = -1.;
  //  std::cout<< "Muon pT < 5 "<<std::endl;
  //}
  //else{
  double r_iso = std::max(r_iso_min, std::min(r_iso_max, kt_scale / mu.pt()));
  double riso2 = r_iso * r_iso;

  for (const auto &pfc : pfcands) {
    //std::cout<< "pfc id " << pfc.pdgId()<< " from PV "<< pfc.fromPV() <<std::endl;
    if (abs(pfc.pdgId()) < 7)
      continue;

    double dr = deltaR(pfc, mu);
    //std::cout<< "muon pt eta phi " << mu.pt() <<" " <<mu.eta() << " "<<mu.phi() <<std::endl;
    //std::cout<< "pfc pt eta phi " << pfc.pt() <<" " << pfc.eta() << " "<<pfc.phi() <<std::endl;
    //std::cout<<  " dr " <<  dr << " v.s. riso " << r_iso <<std::endl;
    if (dr > r_iso)
      continue;

    //std::cout<< " chg " << pfc.charge() << std::endl;
    if (pfc.charge() == 0) {
      //////////////////  NEUTRALS  //////////////////
      //std::cout<< " pt " << pfc.pt() << " v.s. ptThresh " << ptThresh << std::endl;
      if (pfc.pt() > ptThresh) {
        /////////// PHOTONS ////////////
        if (abs(pfc.pdgId()) == 22) {
          //std::cout<< " 22 dr " << dr << " v.s. deadcone_ph "<<deadcone_ph <<std::endl;
          if (dr < deadcone_ph)
            continue;
          iso_ph += pfc.pt();
        }
        /////////// NEUTRAL HADRONS ////////////
        if (abs(pfc.pdgId()) == 130) {
          //std::cout<< " 130 dr " << dr << " v.s. deadcone_nh "<<deadcone_nh <<std::endl;
          if (dr < deadcone_nh)
            continue;
          iso_nh += pfc.pt();
        }
      }
    } else {
      if (is_fromPV(pfc)) {
        //////////////////  CHARGED from PV  //////////////////
        if (abs(pfc.pdgId()) == 211) {
          if (dr < deadcone_ch)
            continue;
          iso_ch += pfc.pt();
        }
      } else {
        //////////////////  CHARGED from PU  //////////////////
        if (pfc.pt() > ptThresh) {
          if (dr < deadcone_pu)
            continue;
          iso_pu += pfc.pt();
        }
      }
    }
    //std::cout<< "ph "<< iso_ph << " nh " <<iso_nh << " ch " <<iso_ch<<std::endl;
  }
  //std::cout<< "rho "<< rho << " riso2 " << riso2 <<std::endl;
  //double Aeff_Fall17Anal[2][7] = {
  //  { 0.1440, 0.1562, 0.1032, 0.0859, 0.1116, 0.1321, 0.1654 },
  //  { 0.0735, 0.0619, 0.0465, 0.0433, 0.0577 , 0.0,0.0}
  //};
  double Aeff_Fall17Anal[5] = {0.0566, 0.0562, 0.0363, 0.0119, 0.0064};

  double CorrectedTerm = 0.0;
  if (TMath::Abs(mu.eta()) < 0.8)
    CorrectedTerm = rho * Aeff_Fall17Anal[0] * (riso2 / 0.09);
  else if (TMath::Abs(mu.eta()) > 0.8 && TMath::Abs(mu.eta()) < 1.3)
    CorrectedTerm = rho * Aeff_Fall17Anal[1] * (riso2 / 0.09);
  else if (TMath::Abs(mu.eta()) > 1.3 && TMath::Abs(mu.eta()) < 2.0)
    CorrectedTerm = rho * Aeff_Fall17Anal[2] * (riso2 / 0.09);
  else if (TMath::Abs(mu.eta()) > 2.0 && TMath::Abs(mu.eta()) < 2.2)
    CorrectedTerm = rho * Aeff_Fall17Anal[3] * (riso2 / 0.09);
  else if (TMath::Abs(mu.eta()) > 2.2 && TMath::Abs(mu.eta()) < 2.5)
    CorrectedTerm = rho * Aeff_Fall17Anal[4] * (riso2 / 0.09);

  iso = iso_ch + TMath::Max(0.0, iso_ph + iso_nh - CorrectedTerm) / mu.pt();
  //} //end if not mu pt < 5

  if (isTag) {
    nt.tag_miniIso = iso;
    nt.tag_miniIsoCharged = iso_ch;
    nt.tag_miniIsoPhotons = iso_ph;
    nt.tag_miniIsoNeutrals = iso_nh;
  } else {
    nt.probe_miniIso = iso;
    nt.probe_miniIsoCharged = iso_ch;
    nt.probe_miniIsoPhotons = iso_ph;
    nt.probe_miniIsoNeutrals = iso_nh;
  }
}

#endif
