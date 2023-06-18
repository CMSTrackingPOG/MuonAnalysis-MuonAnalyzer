#include "MuonGenAnalyzer.h"

MuonGenAnalyzer::MuonGenAnalyzer(){};

MuonGenAnalyzer::~MuonGenAnalyzer(){};

void MuonGenAnalyzer::SetInputs(const edm::Event& iEvent,
                                const edm::EDGetTokenT<edm::View<reco::GenParticle>>& gens_,
                                const int& momPdg_) {
  iEvent.getByToken(gens_, gens);
  const reco::Candidate* mother1st = nullptr;
  bool found1 = false;
  bool found2 = false;
  gmuon1.SetPtEtaPhiM(0, 0, 0, 0);
  gmuon2.SetPtEtaPhiM(0, 0, 0, 0);
  bool foundFSfromHP1 = false;
  bool foundFSfromHP2 = false;
  gmuonFSfromHP1.SetPtEtaPhiM(0, 0, 0, 0);
  gmuonFSfromHP2.SetPtEtaPhiM(0, 0, 0, 0);
  for (const auto& gen : *gens) {
    if (fabs(gen.pdgId()) != 13)
      continue;

    // look for final state muons from hard process
    if (gen.fromHardProcessFinalState()) {
      if (gen.charge() < 0) {
        if (foundFSfromHP1) {
          std::cout
              << "Warning|MuonGenAnalyzer::SetInputs| there are more than 2 final state mu- from hard process -> skip"
              << std::endl;
        } else {
          gmuonFSfromHP1.SetPtEtaPhiM(gen.pt(), gen.eta(), gen.phi(), MU_MASS);
          foundFSfromHP1 = true;
        }
      } else {
        if (foundFSfromHP2) {
          std::cout
              << "Warning|MuonGenAnalyzer::SetInputs| there are more than 2 final state mu+ from hard process -> skip"
              << std::endl;
        } else {
          gmuonFSfromHP2.SetPtEtaPhiM(gen.pt(), gen.eta(), gen.phi(), MU_MASS);
          foundFSfromHP2 = true;
        }
      }
    }

    // look for final state muons
    if (gen.status() != 1)
      continue;

    bool fromSameMother = false;
    const reco::Candidate* mother_tmp = gen.mother();
    while (mother_tmp) {
      if (mother_tmp->pdgId() == momPdg_) {
        // if there are more than 2 resonances, take the first one for now...
        if (mother1st == nullptr) {
          mother1st = mother_tmp;
        }
        fromSameMother = (mother1st == mother_tmp);
        break;
      }
      mother_tmp = mother_tmp->mother();
    }
    if (!fromSameMother)
      continue;

    if (gen.charge() < 0.) {
      if (found1) {
        std::cout << "Warning|MuonGenAnalyzer::SetInputs| there are more than 2 final state mu- from the same "
                     "resonance -> skip"
                  << std::endl;
      } else {
        gmuon1.SetPtEtaPhiM(gen.pt(), gen.eta(), gen.phi(), MU_MASS);
        found1 = true;
      }
    } else {
      if (found2) {
        std::cout << "Warning|MuonGenAnalyzer::SetInputs| there are more than 2 final state mu+ from the same "
                     "resonance -> skip"
                  << std::endl;
      } else {
        gmuon2.SetPtEtaPhiM(gen.pt(), gen.eta(), gen.phi(), MU_MASS);
        found2 = true;
      }
    }
  }
}

void MuonGenAnalyzer::FillNtuple(NtupleContent& nt) {
  if (gmuon1.Pt() > 0. && gmuon2.Pt() > 0.) {
    nt.genmu1_pt = gmuon1.Pt();
    nt.genmu1_eta = gmuon1.Eta();
    nt.genmu1_phi = gmuon1.Phi();
    nt.genmu1_charge = -1;
    nt.genmu2_pt = gmuon2.Pt();
    nt.genmu2_eta = gmuon2.Eta();
    nt.genmu2_phi = gmuon2.Phi();
    nt.genmu2_charge = 1;
    nt.genMass = (gmuon1 + gmuon2).M();
  } else {
    nt.genmu1_pt = -99;
    nt.genmu1_eta = -99;
    nt.genmu1_phi = -99;
    nt.genmu1_charge = -99;
    nt.genmu2_pt = -99;
    nt.genmu2_eta = -99;
    nt.genmu2_phi = -99;
    nt.genmu2_charge = -99;
    nt.genMass = -99;
  }

  // Fill final state muons from hard process
  if (gmuonFSfromHP1.Pt() > 0. && gmuonFSfromHP2.Pt() > 0.) {
    nt.genmuFSfromHP1_pt = gmuonFSfromHP1.Pt();
    nt.genmuFSfromHP1_eta = gmuonFSfromHP1.Eta();
    nt.genmuFSfromHP1_phi = gmuonFSfromHP1.Phi();
    nt.genmuFSfromHP1_charge = -1;
    nt.genmuFSfromHP2_pt = gmuonFSfromHP2.Pt();
    nt.genmuFSfromHP2_eta = gmuonFSfromHP2.Eta();
    nt.genmuFSfromHP2_phi = gmuonFSfromHP2.Phi();
    nt.genmuFSfromHP2_charge = 1;
    nt.genMassFSfromHP = (gmuonFSfromHP1 + gmuonFSfromHP2).M();
  } else {
    nt.genmuFSfromHP1_pt = -99;
    nt.genmuFSfromHP1_eta = -99;
    nt.genmuFSfromHP1_phi = -99;
    nt.genmuFSfromHP1_charge = -99;
    nt.genmuFSfromHP2_pt = -99;
    nt.genmuFSfromHP2_eta = -99;
    nt.genmuFSfromHP2_phi = -99;
    nt.genmuFSfromHP2_charge = -99;
    nt.genMassFSfromHP = -99;
  }
}
