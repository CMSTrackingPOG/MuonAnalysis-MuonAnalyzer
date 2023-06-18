#include "KlFitter.h"

KlFitter::KlFitter(std::vector<reco::TransientTrack> &vecttrk) {
  KalmanVertexFitter vtxFitter(true);
  dimuvtx = vtxFitter.vertex(vecttrk);
  if (!dimuvtx.isValid())
    status_ = false;
  else {
    refited = dimuvtx.refittedTracks();
    prob_ = ChiSquaredProbability(dimuvtx.totalChiSquared(), dimuvtx.degreesOfFreedom());
    normalchi2_ = dimuvtx.totalChiSquared() / dimuvtx.degreesOfFreedom();
  }
}

KlFitter::~KlFitter(){};

void KlFitter::fillNtuple(NtupleContent &nt, bool isTuneP) {
  if (isTuneP) {
    if (status_) {
      nt.pair_tuneP_svprob = prob_;
      nt.pair_tuneP_fit_mass = DimuonMass(refited[0].track().pt(),
                                          refited[0].track().eta(),
                                          refited[0].track().phi(),
                                          refited[1].track().pt(),
                                          refited[1].track().eta(),
                                          refited[1].track().phi());
      nt.pair_tuneP_normalchi2 = normalchi2_;
    } else {
      nt.pair_tuneP_svprob = -1;
      nt.pair_tuneP_fit_mass = -1;
      nt.pair_tuneP_normalchi2 = -1;
    }
  } else {
    if (status_) {
      nt.pair_svprob = prob_;
      nt.pair_fit_mass = DimuonMass(refited[0].track().pt(),
                                    refited[0].track().eta(),
                                    refited[0].track().phi(),
                                    refited[1].track().pt(),
                                    refited[1].track().eta(),
                                    refited[1].track().phi());
      nt.pair_normalchi2 = normalchi2_;
    } else {
      nt.pair_svprob = -1;
      nt.pair_fit_mass = -1;
      nt.pair_normalchi2 = -1;
    }
  }
}
