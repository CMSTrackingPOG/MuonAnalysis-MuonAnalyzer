// Package:    HLTAnalysis/TriggerAnalyzer
// Class:      TriggerAnalyzer
//
/**\class TriggerAnalyzer TriggerAnalyzer.cc
 HLTAnalysis/TriggerAnalyzer/plugins/TriggerAnalyzer.cc

 Description: Ntuplizer for miniAOD files
*/
//
// Original Author:
//                george karathanasis
//         Created:  Thu, 23 Mar 2017 17:40:23 GMT
//
// Modified:
//                Minseok Oh (Feb. 2021)
//
//

// system include files
#include <iostream>
#include <memory>
#include <random>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/Association.h"
#include "DataFormats/Common/interface/AssociationMap.h"
#include "DataFormats/Common/interface/RefToPtr.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "JetMETCorrections/Modules/interface/JetResolution.h"
#include "DataFormats/JetReco/interface/GenJet.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/L1Trigger/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "HLTrigger/HLTcore/interface/defaultModuleLabel.h"

#include <iostream>
#include <string>
#include <vector>
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidateIsolation.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "KlFitter.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/ParametrizedEngine/src/OAEParametrizedMagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/MuonReco/interface/MuonSimInfo.h"

#include "JetMETCorrections/Modules/interface/JetResolution.h"
#include "JetMETCorrections/JetCorrector/interface/JetCorrector.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "TLorentzVector.h"
#include "TTree.h"
#include "TrackingTools/PatternTools/interface/ClosestApproachInRPhi.h"
#include "TrackingTools/PatternTools/interface/TwoTrackMinimumDistance.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"

//#include "MuonBranches.h"
#include "MuonGenAnalyzer.h"
#include "NtupleContent.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "helper.h"
#include "MuonMiniIsolation.h"
#include "JetsBranches.h"

#include "StandAloneMuonBranches.h"
#include "StandAloneNtupleContent.h"

using namespace std;
// using namespace edm;

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.
class StandAloneMuonMiniAODAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
  typedef std::vector<std::pair<pat::Muon, reco::TransientTrack>> RecoTrkAndTransientTrkCollection;
  explicit StandAloneMuonMiniAODAnalyzer(const edm::ParameterSet&);
  ~StandAloneMuonMiniAODAnalyzer() override;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void beginJob() override;
  bool HLTaccept(const edm::Event&, NtupleContent&, std::vector<std::string>&);
  void embedTriggerMatching(const edm::Event&,
                            edm::Handle<edm::TriggerResults>&,
                            const pat::Muon&,
                            NtupleContent&,
                            std::vector<std::string>&,
                            bool);

  void StandAlone_embedTriggerMatching(const edm::Event&,
                                       edm::Handle<edm::TriggerResults>&,
                                       const pat::Muon&,
                                       StandAloneNtupleContent&,
                                       std::vector<std::string>&,
                                       bool);

  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void endJob() override;

  edm::EDGetTokenT<GenEventInfoProduct> genEventInfoToken_;
  edm::EDGetTokenT<double> rhoToken_;
  edm::EDGetTokenT<std::vector<PileupSummaryInfo>> pileupSummaryToken_;
  edm::EDGetTokenT<reco::BeamSpot> beamSpotToken_;
  edm::EDGetTokenT<std::vector<reco::Vertex>> vtxToken_;
  edm::EDGetToken muonsToken_;
  edm::EDGetTokenT<edm::View<reco::Muon>> muonsViewToken_;
  edm::EDGetToken PFCands_;
  edm::EDGetToken LostTracks_;
  const edm::ESGetToken<MagneticField, IdealMagneticFieldRecord> bFieldToken_;
  edm::EDGetToken SAmuonsToken_;

  edm::EDGetTokenT<edm::TriggerResults> trgresultsToken_;
  // edm::EDGetTokenT<pat::TriggerObjectStandAloneMatch> l1MatchesToken_;
  // edm::EDGetTokenT<edm::ValueMap<int>> l1MatchesQualityToken_;
  // edm::EDGetTokenT<edm::ValueMap<float>> l1MatchesDeltaRToken_;
  // edm::EDGetTokenT<pat::TriggerObjectStandAloneMatch> l1MatchesByQToken_;
  // edm::EDGetTokenT<edm::ValueMap<int>> l1MatchesByQQualityToken_;
  // edm::EDGetTokenT<edm::ValueMap<float>> l1MatchesByQDeltaRToken_;
  edm::EDGetTokenT<edm::View<reco::GenParticle>> genToken_;
  edm::EDGetTokenT<double> rhoJetsNC_;
  edm::EDGetToken jetsToken_;
  edm::EDGetToken genJetsToken_;
  std::vector<std::string> HLTPaths_;
  std::vector<std::string> tagFilters_;
  std::vector<std::string> probeFilters_;
  std::vector<std::string> probeSelectorNames_;
  std::vector<unsigned> probeSelectorBits_;
  const unsigned int tagQual_;
  const StringCutObjectSelector<pat::Muon> tagSelection_;  // kinematic cuts for tag
  const bool HighPurity_;
  const StringCutObjectSelector<pat::PackedCandidate> probeSelection_;  // kinematic cuts for probe
  const StringCutObjectSelector<reco::Track> probeSelectionSA_;         // kinematic cuts for SA probe
  const bool muonOnly_;
  const StringCutObjectSelector<pat::Muon> probeMuonSelection_;
  const double pairMassMin_;
  const double pairMassMax_;
  const double pairDz_;
  const bool RequireVtxCreation_;  // if true skip pairs that do not create
                                   // that do not have a vertex
  const double minSVtxProb_;       // min probability of a vertex to be kept. If < 0 inactive
  const double maxdz_trk_mu_;
  const double maxpt_relative_dif_trk_mu_;
  const double maxdr_trk_mu_;
  const double minpt_trkSA_;
  const double maxdz_trk_SAmu_;
  const double maxpt_relative_dif_trk_SAmu_;
  const double maxdr_trk_SAmu_;
  const unsigned momPdgId_;
  const double genRecoDrMatch_;
  const bool saveStandAloneTree_;
  const bool saveTnPTree_;
  //PropagateToMuon prop1_;

  edm::Service<TFileService> fs;
  TTree* t1;
  NtupleContent nt;

  //SA
  TTree* StandAlone_t1;
  StandAloneNtupleContent StandAlone_nt;

  std::mt19937 m_random_generator = std::mt19937(37428479);
  const bool isMC_, includeJets_;
  const std::string era_;

  // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//

StandAloneMuonMiniAODAnalyzer::StandAloneMuonMiniAODAnalyzer(const edm::ParameterSet& iConfig)
    : genEventInfoToken_(consumes<GenEventInfoProduct>(iConfig.getParameter<edm::InputTag>("genEventInfo"))),
      rhoToken_(consumes<double>(iConfig.getParameter<edm::InputTag>("Rho"))),
      pileupSummaryToken_(consumes<std::vector<PileupSummaryInfo>>(iConfig.getParameter<edm::InputTag>("pileupInfo"))),
      beamSpotToken_(consumes<reco::BeamSpot>(iConfig.getParameter<edm::InputTag>("beamSpot"))),
      vtxToken_(consumes<std::vector<reco::Vertex>>(iConfig.getParameter<edm::InputTag>("vertices"))),
      muonsToken_(consumes<std::vector<pat::Muon>>(iConfig.getParameter<edm::InputTag>("muons"))),
      muonsViewToken_(consumes<edm::View<reco::Muon>>(iConfig.getParameter<edm::InputTag>("muons"))),
      PFCands_(consumes<std::vector<pat::PackedCandidate>>(iConfig.getParameter<edm::InputTag>("PFCands"))),
      LostTracks_(consumes<std::vector<pat::PackedCandidate>>(iConfig.getParameter<edm::InputTag>("lostTracks"))),
      bFieldToken_(esConsumes<MagneticField, IdealMagneticFieldRecord>()),
      SAmuonsToken_(consumes<std::vector<pat::PackedCandidate>>(iConfig.getParameter<edm::InputTag>("SAmuons"))),

      trgresultsToken_(consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("triggerResults"))),
      // l1MatchesToken_(consumes<pat::TriggerObjectStandAloneMatch>(iConfig.getParameter<edm::InputTag>("l1Matches"))),
      // l1MatchesQualityToken_(consumes<edm::ValueMap<int>>(iConfig.getParameter<edm::InputTag>("l1MatchesQuality"))),
      // l1MatchesDeltaRToken_(consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("l1MatchesDeltaR"))),
      // l1MatchesByQToken_(
          // consumes<pat::TriggerObjectStandAloneMatch>(iConfig.getParameter<edm::InputTag>("l1MatchesByQ"))),
      // l1MatchesByQQualityToken_(
          // consumes<edm::ValueMap<int>>(iConfig.getParameter<edm::InputTag>("l1MatchesByQQuality"))),
      // l1MatchesByQDeltaRToken_(
          // consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("l1MatchesByQDeltaR"))),
      genToken_(consumes<edm::View<reco::GenParticle>>(iConfig.getParameter<edm::InputTag>("gen"))),
      rhoJetsNC_(consumes<double>(iConfig.getParameter<edm::InputTag>("rhoJetsNC"))),
      jetsToken_(consumes<std::vector<pat::Jet>>(iConfig.getParameter<edm::InputTag>("jets"))),
      genJetsToken_(consumes<std::vector<reco::GenJet>>(iConfig.getParameter<edm::InputTag>("genJets"))),
      HLTPaths_(iConfig.getParameter<std::vector<std::string>>("triggerPaths")),
      tagFilters_(iConfig.getParameter<std::vector<std::string>>("tagFilters")),
      probeFilters_(iConfig.getParameter<std::vector<std::string>>("probeFilters")),
      probeSelectorNames_(iConfig.getParameter<std::vector<std::string>>("probeSelectorNames")),
      probeSelectorBits_(iConfig.getParameter<std::vector<unsigned>>("probeSelectorBits")),
      tagQual_(iConfig.getParameter<unsigned>("tagQuality")),
      tagSelection_(iConfig.getParameter<std::string>("tagSelection")),
      HighPurity_(iConfig.getParameter<bool>("ProbeHPurity")),
      probeSelection_(iConfig.getParameter<std::string>("probeSelection")),
      probeSelectionSA_(iConfig.getParameter<std::string>("probeSelectionSA")),
      muonOnly_(iConfig.getParameter<bool>("muonOnly")),
      probeMuonSelection_(iConfig.getParameter<std::string>("probeMuonSelection")),
      pairMassMin_(iConfig.getParameter<double>("pairMassMin")),
      pairMassMax_(iConfig.getParameter<double>("pairMassMax")),
      pairDz_(iConfig.getParameter<double>("pairDz")),
      RequireVtxCreation_(iConfig.getParameter<bool>("RequireVtxCreation")),
      minSVtxProb_(iConfig.getParameter<double>("minSVtxProb")),
      maxdz_trk_mu_(iConfig.getParameter<double>("maxDzProbeTrkMuon")),
      maxpt_relative_dif_trk_mu_(iConfig.getParameter<double>("maxRelPtProbeTrkMuon")),
      maxdr_trk_mu_(iConfig.getParameter<double>("maxDRProbeTrkMuon")),
      minpt_trkSA_(iConfig.getParameter<double>("minPtTrkSA")),
      maxdz_trk_SAmu_(iConfig.getParameter<double>("maxDzProbeTrkSAMuon")),
      maxpt_relative_dif_trk_SAmu_(iConfig.getParameter<double>("maxRelPtProbeTrkSAMuon")),
      maxdr_trk_SAmu_(iConfig.getParameter<double>("maxDRProbeTrkSAMuon")),
      momPdgId_(iConfig.getParameter<unsigned>("momPdgId")),
      genRecoDrMatch_(iConfig.getParameter<double>("genRecoDrMatch")),
      saveStandAloneTree_(iConfig.getParameter<bool>("saveStandAloneTree")),
      saveTnPTree_(iConfig.getParameter<bool>("saveTnPTree")),
      //prop1_(iConfig.getParameter<edm::ParameterSet>("propM1")),
      isMC_(iConfig.getParameter<bool>("isMC")),
      includeJets_(iConfig.getParameter<bool>("includeJets")),
      era_(iConfig.getParameter<std::string>("era")) {
  //  edm::ParameterSet
  //  runParameters=iConfig.getParameter<edm::ParameterSet>("RunParameters");

  if (probeSelectorNames_.size() != probeSelectorBits_.size()) {
    throw cms::Exception("ParameterError")
        << "length of probeSelectorNames and probeSelectorBits should be identical\n";
  }
}

StandAloneMuonMiniAODAnalyzer::~StandAloneMuonMiniAODAnalyzer() {
  // cout << "total " << trg_counter << " fires " << fire_counter << " l3"
  // << l3_counter << endl; do anything here that needs to be done at
  // desctruction time
}

//
// member functions
bool StandAloneMuonMiniAODAnalyzer::HLTaccept(const edm::Event& iEvent,
                                              NtupleContent& nt,
                                              std::vector<std::string>& HLTPaths) {
  edm::Handle<edm::TriggerResults> trigResults;
  iEvent.getByToken(trgresultsToken_, trigResults);
  edm::TriggerNames trigName;
  trigName = iEvent.triggerNames(*trigResults);
  bool EvtFire = false;
  unsigned int ipath = 0;
  for (auto path : HLTPaths) {
    bool TrgFire = false;
    for (unsigned int itrg = 0; itrg < trigResults->size(); ++itrg) {
      TString TrigPath = trigName.triggerName(itrg);
      if (!trigResults->accept(itrg))
        continue;
      if (!TrigPath.Contains(path))
        continue;
      EvtFire = true;
      TrgFire = true;
    }
    nt.trigger[ipath] = TrgFire;
    ipath++;
  }
  return EvtFire;
}

void StandAloneMuonMiniAODAnalyzer::embedTriggerMatching(const edm::Event& iEvent,
                                                         edm::Handle<edm::TriggerResults>& trigResults,
                                                         const pat::Muon& mu,
                                                         NtupleContent& nt,
                                                         std::vector<std::string>& Triggers,
                                                         bool isTag) {
  for (const auto& trg : Triggers) {
    TString trg_tstr = TString(trg);
    bool matched = false;
    float matched_pt = -99;
    float matched_eta = -99;
    float matched_phi = -99;
    float matched_dr = 99;
    for (auto trigobj : mu.triggerObjectMatches()) {
      trigobj.unpackNamesAndLabels(iEvent, *trigResults);
      float dR_tmp = deltaR(mu.eta(), mu.phi(), trigobj.eta(), trigobj.phi());
      float dptrel_tmp = fabs(mu.pt()-trigobj.pt())/(trigobj.pt());
      // check path names
      if (trg_tstr.Contains("HLT_")) {
        for (auto path : trigobj.pathNames(true, true)) {
          TString path_tstr = TString(path);
          if (path_tstr.Contains(trg_tstr) && dR_tmp < matched_dr && dR_tmp < 0.5 && dptrel_tmp < 10.) {
            matched = true;
            matched_pt = trigobj.pt();
            matched_eta = trigobj.eta();
            matched_phi = trigobj.phi();
            matched_dr = dR_tmp;
          }
        }
      }
      // check filters
      else {
        for (auto filter : trigobj.filterLabels()) {
          TString filter_tstr = TString(filter);
          if (filter_tstr.Contains(trg_tstr) && dR_tmp < matched_dr && dR_tmp < 0.5 && dptrel_tmp < 10.) {
            matched = true;
            matched_pt = trigobj.pt();
            matched_eta = trigobj.eta();
            matched_phi = trigobj.phi();
            matched_dr = dR_tmp;
          }
        }
      }
    }
    if (isTag) {
      nt.tag_trg[&trg - &Triggers[0]] = matched;
      nt.tag_trg_pt[&trg - &Triggers[0]] = matched_pt;
      nt.tag_trg_eta[&trg - &Triggers[0]] = matched_eta;
      nt.tag_trg_phi[&trg - &Triggers[0]] = matched_phi;
      nt.tag_trg_dr[&trg - &Triggers[0]] = matched_dr;
    } else {
      nt.probe_trg[&trg - &Triggers[0]] = matched;
      nt.probe_trg_pt[&trg - &Triggers[0]] = matched_pt;
      nt.probe_trg_eta[&trg - &Triggers[0]] = matched_eta;
      nt.probe_trg_phi[&trg - &Triggers[0]] = matched_phi;
      nt.probe_trg_dr[&trg - &Triggers[0]] = matched_dr;
    }
  }
  return;
}

void StandAloneMuonMiniAODAnalyzer::StandAlone_embedTriggerMatching(const edm::Event& iEvent,
                                                                    edm::Handle<edm::TriggerResults>& trigResults,
                                                                    const pat::Muon& mu,
                                                                    StandAloneNtupleContent& StandAlone_nt,
                                                                    std::vector<std::string>& Triggers,
                                                                    bool isTag) {
  for (const auto& trg : Triggers) {
    TString trg_tstr = TString(trg);
    bool matched = false;
    float matched_pt = -99;
    float matched_eta = -99;
    float matched_phi = -99;
    float matched_dr = 99;
    for (auto trigobj : mu.triggerObjectMatches()) {
      trigobj.unpackNamesAndLabels(iEvent, *trigResults);
      float dR_tmp = deltaR(mu.eta(), mu.phi(), trigobj.eta(), trigobj.phi());
      float dptrel_tmp = fabs(mu.pt()-trigobj.pt())/(trigobj.pt());
      // check path names
      if (trg_tstr.Contains("HLT_")) {
        for (auto path : trigobj.pathNames(true, true)) {
          TString path_tstr = TString(path);
          if (path_tstr.Contains(trg_tstr) && dR_tmp < matched_dr && dR_tmp < 0.5 && dptrel_tmp < 10.) {
            matched = true;
            matched_pt = trigobj.pt();
            matched_eta = trigobj.eta();
            matched_phi = trigobj.phi();
            matched_dr = dR_tmp;
          }
        }
      }
      // check filters
      else {
        for (auto filter : trigobj.filterLabels()) {
          TString filter_tstr = TString(filter);
          if (filter_tstr.Contains(trg_tstr) && dR_tmp < matched_dr && dR_tmp < 0.5 && dptrel_tmp < 10.) {
            matched = true;
            matched_pt = trigobj.pt();
            matched_eta = trigobj.eta();
            matched_phi = trigobj.phi();
            matched_dr = dR_tmp;
          }
        }
      }
    }
    if (isTag) {
      StandAlone_nt.tag_trg[&trg - &Triggers[0]] = matched;
      StandAlone_nt.tag_trg_pt[&trg - &Triggers[0]] = matched_pt;
      StandAlone_nt.tag_trg_eta[&trg - &Triggers[0]] = matched_eta;
      StandAlone_nt.tag_trg_phi[&trg - &Triggers[0]] = matched_phi;
      StandAlone_nt.tag_trg_dr[&trg - &Triggers[0]] = matched_dr;
    }
    else {
    StandAlone_nt.probe_trg[&trg - &Triggers[0]] = matched;
    StandAlone_nt.probe_trg_pt[&trg - &Triggers[0]] = matched_pt;
    StandAlone_nt.probe_trg_eta[&trg - &Triggers[0]] = matched_eta;
    StandAlone_nt.probe_trg_phi[&trg - &Triggers[0]] = matched_phi;
    StandAlone_nt.probe_trg_dr[&trg - &Triggers[0]] = matched_dr;
    }
  }
  return;
}
// ------------ method called for each event  ------------

void StandAloneMuonMiniAODAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  
  // edm::ESHandle<MagneticField> magfield_;
  // edm::ESHandle<Propagator> propagator_;
  // edm::ESHandle<Propagator> propagatorOpposite_;
  // edm::ESHandle<Propagator> propagatorAny_;
  // //edm::ESHandle<MuonDetLayerGeometry> muonGeometry_;
  // //iSetup.get<IdealMagneticFieldRecord>().get(magfield_);
  // bool useSimpleGeometry_ = 1;
  // bool useMB2_ = 0;
  // bool fallbackToME1_ = 0;
  // bool cosmicPropagation_ = 0;
  // bool useMB2InOverlap_ = 0;

  //iSetup.get<TrackingComponentsRecord>().get("SteppingHelixPropagatorAlong", propagator_);
  //iSetup.get<TrackingComponentsRecord>().get("SteppingHelixPropagatorOpposite", 	propagatorOpposite_);
  //iSetup.get<TrackingComponentsRecord>().get("SteppingHelixPropagatorAny", 	propagatorAny_);
  //iSetup.get<MuonRecoGeometryRecord>().get(muonGeometry_);
  //prop1_(magfield_, propagator_, propagatorOpposite_, propagatorAny_, muonGeometry_, 
  //useSimpleGeometry_, useMB2_, fallbackToME1_, "TrackerTk","AtVertex",cosmicPropagation_,useMB2InOverlap_);
  

  edm::Handle<reco::BeamSpot> theBeamSpot;
  iEvent.getByToken(beamSpotToken_, theBeamSpot);
  edm::Handle<reco::VertexCollection> vertices;
  iEvent.getByToken(vtxToken_, vertices);

  // Skip evts if there are no vertices
  if (vertices->empty())
    return;

  edm::Handle<std::vector<pat::PackedCandidate>> SAmuons;
  iEvent.getByToken(SAmuonsToken_, SAmuons);

  edm::Handle<std::vector<pat::Muon>> muons;
  iEvent.getByToken(muonsToken_, muons);
  edm::Handle<edm::View<reco::Muon>> muonsView;
  iEvent.getByToken(muonsViewToken_, muonsView);
  edm::Handle<std::vector<pat::PackedCandidate>> pfcands;
  iEvent.getByToken(PFCands_, pfcands);
  edm::Handle<std::vector<pat::PackedCandidate>> lostTracks;
  iEvent.getByToken(LostTracks_, lostTracks);
  //edm::ESHandle<MagneticField> bField;
  //iSetup.get<IdealMagneticFieldRecord>().get(bField);
  
  const auto& bField = iSetup.getData(bFieldToken_);
  edm::Handle<double> rhoJetsNC;
  iEvent.getByToken(rhoJetsNC_, rhoJetsNC);
  edm::Handle<std::vector<pat::Jet>> jets;
  iEvent.getByToken(jetsToken_, jets);
  edm::Handle<std::vector<reco::GenJet>> genJets;
  iEvent.getByToken(genJetsToken_, genJets);
  JME::JetResolution resolution;
  //resolution = JME::JetResolution::get(iSetup, "AK4PFchs_pt");
  JME::JetResolutionScaleFactor resolution_sf;
  //resolution_sf = JME::JetResolutionScaleFactor::get(iSetup, "AK4PFchs");

  edm::Handle<edm::TriggerResults> trigResults;
  iEvent.getByToken(trgresultsToken_, trigResults);
  // edm::Handle<pat::TriggerObjectStandAloneMatch> l1Matches;
  // iEvent.getByToken(l1MatchesToken_, l1Matches);
  // edm::Handle<edm::ValueMap<int>> l1Qualities;
  // iEvent.getByToken(l1MatchesQualityToken_, l1Qualities);
  // edm::Handle<edm::ValueMap<float>> l1Drs;
  // iEvent.getByToken(l1MatchesDeltaRToken_, l1Drs);
  // edm::Handle<pat::TriggerObjectStandAloneMatch> l1MatchesByQ;
  // iEvent.getByToken(l1MatchesByQToken_, l1MatchesByQ);
  // edm::Handle<edm::ValueMap<int>> l1QualitiesByQ;
  // iEvent.getByToken(l1MatchesByQQualityToken_, l1QualitiesByQ);
  // edm::Handle<edm::ValueMap<float>> l1DrsByQ;
  // iEvent.getByToken(l1MatchesByQDeltaRToken_, l1DrsByQ);

  //SA
  StandAlone_nt.ClearBranches();

  // Information about run
  nt.ClearBranches();
  nt.run = iEvent.id().run();
  nt.ls = iEvent.luminosityBlock();
  nt.event = iEvent.id().event();
  nt.fromFullAOD = false;
  nt.BSpot_x = theBeamSpot->x0();
  nt.BSpot_y = theBeamSpot->y0();
  nt.BSpot_z = theBeamSpot->z0();
  nt.nvertices = vertices->size();

  // Gen weights
  if (!iEvent.isRealData()) {
    edm::Handle<GenEventInfoProduct> genEventInfoHandle;
    iEvent.getByToken(genEventInfoToken_, genEventInfoHandle);
    nt.genWeight = genEventInfoHandle->weight();
  } else {  // data
    nt.genWeight = 1.;
  }

  // Pileup information
  edm::Handle<double> rhoHandle;
  iEvent.getByToken(rhoToken_, rhoHandle);
  nt.Rho = *rhoHandle;

  float trueNumInteractions = -1;
  int puNumInteractions = -1;
  if (isMC_) {
    edm::Handle<std::vector<PileupSummaryInfo>> PupInfo;
    iEvent.getByToken(pileupSummaryToken_, PupInfo);

    for (auto PVI : *PupInfo) {
      int BX = PVI.getBunchCrossing();
      if (BX == 0) {
        trueNumInteractions = PVI.getTrueNumInteractions();
        puNumInteractions = PVI.getPU_NumInteractions();
        continue;
      }
    }
  }

  nt.trueNumInteractions = trueNumInteractions;
  nt.puNumInteractions = puNumInteractions;

  bool goodVtx = false;
  reco::Vertex const* pv;
  for (const reco::Vertex& vtx : *vertices) {
    if ((vtx.isFake() || !vtx.isValid()) || (vtx.ndof() <= 4) || abs(vtx.z()) > 25 || ((vtx.position()).Rho() > 2 ))
      continue;
    nt.pv_x = vtx.x();
    nt.pv_y = vtx.y();
    nt.pv_z = vtx.z();
    goodVtx = true;
    pv = &vtx;
    break;
  }
  if (!goodVtx){
    //std::cout << "Event: " << iEvent.id().event() << " No goodVtx found!" << std::endl;
    return;  // skipping in absence of good vertex
  }
  // check if path fired, if so save hlt muons
  if (!HLTaccept(iEvent, nt, HLTPaths_)){
    //std::cout <<  "Event: " << iEvent.id().event() << " No goodVtx found!" << "No HLT found!" << std::endl;
    return;
  }
  //else{
  //  std::cout <<  "Event: " << iEvent.id().event() << " GoodVtx found!" << "Good HLT found!" << std::endl;
  //}
  //  HLTaccept(iEvent, nt.doublemu_trg, DoubleMuPaths_);

  // Gen information
  MuonGenAnalyzer genmu;
  std::vector<unsigned> matched_muon_idx;
  if (!iEvent.isRealData()) {
    genmu.SetInputs(iEvent, genToken_, momPdgId_);
    genmu.FillNtuple(nt);
    auto reco_match_genmu1 =
        MatchReco<pat::Muon>(*muons, nt.genmu1_eta, nt.genmu1_phi, nt.genmu1_charge, genRecoDrMatch_);
    auto reco_match_genmu2 =
        MatchReco<pat::Muon>(*muons, nt.genmu2_eta, nt.genmu2_phi, nt.genmu2_charge, genRecoDrMatch_);
    if (reco_match_genmu1.first)
      matched_muon_idx.push_back(reco_match_genmu1.second);
    if (reco_match_genmu2.first)
      matched_muon_idx.push_back(reco_match_genmu2.second);
  }

  // Find triggering muon
  std::vector<unsigned> tag_muon_map;  // idx of tag muon in muons
  RecoTrkAndTransientTrkCollection tag_muon_ttrack;
  std::vector<bool> genmatched_tag;
  for (const pat::Muon& mu : *muons) {
    if (mu.selectors() != 0) {  // Only 9_4_X and later have selector bits
      if (!mu.passed(pow(2, tagQual_)))
        continue;
    } else {  // For 2016, assume loose ID on the tag (can be tightened at spark level)
      if (!muon::isLooseMuon(mu))
        continue;
    }
    bool fired = false;
    for (const std::string path : HLTPaths_) {
      char cstr[(path + "*").size() + 1];
      strcpy(cstr, (path + "*").c_str());
      if (!mu.triggered(cstr))
        continue;
      fired = true;
      break;
    }
    if (!fired)
      continue;
    if (!tagSelection_(mu))
      continue;
    tag_muon_ttrack.emplace_back(std::make_pair(mu, reco::TransientTrack(*mu.bestTrack(), &(bField))));
    tag_muon_map.push_back(&mu - &muons->at(0));
    if (std::find(matched_muon_idx.begin(), matched_muon_idx.end(), &mu - &muons->at(0)) != matched_muon_idx.end())
      genmatched_tag.push_back(true);
    else
      genmatched_tag.push_back(false);
  }
  if (tag_muon_ttrack.empty()){
    return;
  }
  //else{
  //  std::cout<< "Tag Track is NOT empty!" << std::endl;
  //}
  nt.nmuons = muons->size();
  nt.ntag = tag_muon_ttrack.size();

  // Add Lost Tracks to Packed cands

  // MN added nocuts tracks vector for matching to SA muons
  std::vector<pat::PackedCandidate> tracks;
  std::vector<pat::PackedCandidate> nocut_tracks;
  for (const auto container : {pfcands, lostTracks}) {
    for (const pat::PackedCandidate& trk : *container) {
      if (!trk.hasTrackDetails())
        continue;
      nocut_tracks.push_back(trk);
      if (!probeSelection_(trk))
        continue;
      if (HighPurity_ && !trk.trackHighPurity())
        continue;
      tracks.emplace_back(trk);
    }
  }
  std::vector<unsigned> matched_track_idx;
  if (!iEvent.isRealData()) {
    auto reco_match_genmu1 =
        MatchReco<pat::PackedCandidate>(tracks, nt.genmu1_eta, nt.genmu1_phi, nt.genmu1_charge, genRecoDrMatch_);
    auto reco_match_genmu2 =
        MatchReco<pat::PackedCandidate>(tracks, nt.genmu2_eta, nt.genmu2_phi, nt.genmu2_charge, genRecoDrMatch_);
    if (reco_match_genmu1.first)
      matched_track_idx.push_back(reco_match_genmu1.second);
    if (reco_match_genmu2.first)
      matched_track_idx.push_back(reco_match_genmu2.second);
  }
  std::pair<std::vector<unsigned>, std::vector<unsigned>> trk_muon_map;
  for (const auto& mu : *muons) {
    if (muonOnly_ && !probeMuonSelection_(mu))
      continue;
    float minDR = 1000;
    unsigned int idx_trk;
    for (const auto& trk : tracks) {
      if (mu.charge() != trk.charge())
        continue;
      if ((trk.pt() <= minpt_trkSA_) && (abs(trk.eta()) <= 1. || trk.p() <= 2.))
        continue;
      if (fabs(mu.vz() - trk.vz()) > maxdz_trk_mu_ && maxdz_trk_mu_ > 0)
        continue;
      if (fabs(mu.eta() - trk.eta()) > 0.4)
        continue;
      if (fabs(mu.pt() - trk.pt()) / mu.pt() > maxpt_relative_dif_trk_mu_ && maxpt_relative_dif_trk_mu_ > 0)
        continue;
      float DR = deltaR(mu.eta(), mu.phi(), trk.eta(), trk.phi());
      if (minDR < DR)
        continue;
      minDR = DR;
      idx_trk = &trk - &tracks[0];
    }
    if (minDR > maxdr_trk_mu_)
      continue;
    trk_muon_map.first.push_back(idx_trk);
    trk_muon_map.second.push_back(&mu - &muons->at(0));
  }

  // Muon collection for jet cleaning
  std::vector<reco::Muon> muForJetCleaning;
  for (const auto& mu : *muons) {
    if (!muon::isLooseMuon(mu))
      continue;
    muForJetCleaning.push_back(mu);
  }

  // MN loop to match standalone muons to closest trk in DR and associated trk if exists
  std::pair<std::vector<unsigned>, std::vector<unsigned>> trk_SAmuon_map;
  std::pair<std::vector<unsigned>, std::vector<unsigned>> associatedtrk_muon_map;

  // cout << " muon size " << muons->size() << endl;
  for (const auto& mu : *muons) {
    if (!mu.isStandAloneMuon())
      continue;
    if (muonOnly_ && !probeMuonSelection_(mu))
      continue;
    const reco::Track SA_mu = *mu.standAloneMuon();
    float minDR = 1000;
    float minDR_assoc = 1000;
    unsigned int idx_trk;
    unsigned int idx_associatedtrk;
    int N_muontracks = mu.numberOfSourceCandidatePtrs();
    float eps = 1e-2;  // for associated trk to be matched DR and pt diff less than eps
    bool hasAssociatedTrkMatch = false;
    bool isAssoc = false;
    //reco::CandidatePtr trk_mu = mu.originalObjectRef();
    const reco::Track trk_mu = *mu.bestTrack();
    for (const auto& trk : nocut_tracks) {
      if (N_muontracks > 0) {
     // float charge_diff = trk_mu->charge() - trk.charge();
     // float pt_diff = trk_mu->pt() - trk.pt();
     // float eta_diff = trk_mu->eta() - trk.eta(); 
     // float phi_diff = trk_mu->phi() - trk.phi();
        for (const auto& tag : tag_muon_ttrack) {
      if (fabs(tag.first.vz() - trk.vz()) < maxdz_trk_SAmu_ ) { isAssoc = true; break; }
      }
        bool charge_match = trk_mu.charge() == trk.charge();
        //bool pt_match = fabs(trk_mu.pt() - trk.pt())/trk.pt() < maxpt_relative_dif_trk_SAmu_ && maxpt_relative_dif_trk_SAmu_ > 0;
        bool pt_match = fabs(trk_mu.pt() - trk.pt()) < 0.01;
        //bool DeltaR_match = deltaR(trk_mu.eta(), trk_mu.phi(), trk.eta(), trk.phi()) < maxdr_trk_SAmu_;
        bool DeltaR_match = deltaR(trk_mu.eta(), trk_mu.phi(), trk.eta(), trk.phi()) < 0.01;
        //bool DeltaEta_match = fabs(trk_mu.eta() - trk.eta()) < 0.4;
        //bool DeltaZ_match = fabs(trk_mu.vz() - trk.vz()) < maxdz_trk_SAmu_ && maxdz_trk_SAmu_ > 0;
        if ((trk.pt() <= minpt_trkSA_) && (abs(trk.eta()) <= 1. || trk.p() <= 2.))
        continue;
        // if(charge_match && DeltaR_match && !pt_match){
     // cout << " Failed pt Event " << nt.event << " diff charge " <<  charge_diff << 
     // " pt " <<  pt_diff <<  " eta " <<  eta_diff << " phi " <<  phi_diff << endl; 
        // }
        // if(charge_match && !DeltaR_match && pt_match){
     // cout << " Failed deltaR Event " << nt.event << "  Check diff charge " <<  charge_diff << 
     // " pt " <<  pt_diff <<  " eta " <<  eta_diff << " phi " <<  phi_diff << endl; 
        // }
        // if(!charge_match && DeltaR_match && pt_match){
     // cout << " Failed charge Event " << nt.event << "  Check diff charge " <<  charge_diff << 
     // " pt " <<  pt_diff <<  " eta " <<  eta_diff << " phi " <<  phi_diff << endl; 
        // }
        //if(!(trk.isAlgoInMask(trk.initialStep) || trk.isAlgoInMask(trk.lowPtTripletStep) || trk.isAlgoInMask(trk.pixelPairStep) || trk.isAlgoInMask(trk.detachedTripletStep) ||
        //  trk.isAlgoInMask(trk.mixedTripletStep) || trk.isAlgoInMask(trk.pixelLessStep) || trk.isAlgoInMask(trk.tobTecStep) || trk.isAlgoInMask(trk.jetCoreRegionalStep)))
        //continue;
        if (charge_match && pt_match && DeltaR_match && isAssoc) {
     // cout << " Passed Check Event " << nt.event << " diff charge " <<  charge_diff << 
     // " pt " <<  pt_diff <<  " eta " <<  eta_diff << " phi " <<  phi_diff << endl;  
          if (minDR_assoc < deltaR(trk_mu.eta(), trk_mu.phi(), trk.eta(), trk.phi()))
          continue;
        minDR_assoc = deltaR(trk_mu.eta(), trk_mu.phi(), trk.eta(), trk.phi());
        idx_associatedtrk = &trk - &nocut_tracks[0];
        hasAssociatedTrkMatch = true;
        }
      }
      if (SA_mu.charge() != trk.charge())
        continue;
      if ((trk.pt() <= minpt_trkSA_) && (abs(trk.eta()) <= 1. || trk.p() <= 2.))
        continue;
      if (isAssoc)
        continue;
      if (fabs(SA_mu.eta() - trk.eta()) > 0.4)
        continue;
      if (fabs(SA_mu.pt() - trk.pt()) / trk.pt() > maxpt_relative_dif_trk_SAmu_ && maxpt_relative_dif_trk_SAmu_ > 0)
        continue;
      float DR = deltaR(SA_mu.eta(), SA_mu.phi(), trk.eta(), trk.phi());
      if (minDR < DR)
        continue;
      minDR = DR;
      idx_trk = &trk - &nocut_tracks[0];
    }
    if (hasAssociatedTrkMatch) {
      associatedtrk_muon_map.first.push_back(idx_associatedtrk);
      associatedtrk_muon_map.second.push_back(&mu - &muons->at(0));
    }
    if (minDR > maxdr_trk_SAmu_)
      continue;
    trk_SAmuon_map.first.push_back(idx_trk);
    trk_SAmuon_map.second.push_back(&mu - &muons->at(0));
  }
  
  // cout << " Check Assoc Vec Size " << associatedtrk_muon_map.first.size() << " Track Match Size" 
 // << trk_SAmuon_map.first.size()  << endl;

  std::vector<pat::Jet> corrJets;
  /* if (includeJets_) {
    for (const auto& jet : *jets) {
      if (CrossClean(jet, muForJetCleaning))
        continue;
      std::unique_ptr<pat::Jet> corrJet(jet.clone());
      // slimmed jets have corrections applied (L1FastJet, L2, L3) with pT cut at 10 GeV
      double jec = 1.0;
      corrJet->scaleEnergy(jec);
      // JER
      double smearFactor = 1.0;
      if (isMC_) {
        // Gen Jet Info
        for (const auto& genJet : *genJets) {
          nt.genJets_pt.push_back(genJet.pt());
          nt.genJets_eta.push_back(genJet.eta());
          nt.genJets_phi.push_back(genJet.phi());
          nt.genJets_mass.push_back(genJet.mass());
        }

        double jet_resolution = resolution.getResolution({{JME::Binning::JetPt, corrJet->pt()},
                                                          {JME::Binning::JetEta, corrJet->eta()},
                                                          {JME::Binning::Rho, *rhoHandle}});
        double jer_sf = resolution_sf.getScaleFactor({{JME::Binning::JetPt, corrJet->pt()},
                                                      {JME::Binning::JetEta, corrJet->eta()},
                                                      {JME::Binning::Rho, *rhoHandle}},
                                                     Variation::NOMINAL);
        // gen matching
        double min_dR = std::numeric_limits<double>::infinity();
        const reco::GenJet* matched_genJet = nullptr;
        for (const auto& genJet : *genJets) {
          double dR = deltaR(genJet, *corrJet);
          if (dR > min_dR)
            continue;
          if (dR >= 0.2)
            continue;
          min_dR = dR;
          matched_genJet = &genJet;
        }
        if (matched_genJet) {
          double dPt = corrJet->pt() - matched_genJet->pt();
          smearFactor = 1 + (jer_sf - 1.) * dPt / corrJet->pt();
        } else if (jer_sf > 1) {
          double sigma = jet_resolution * std::sqrt(jer_sf * jer_sf - 1);
          std::normal_distribution<> d(0, sigma);
          smearFactor = 1. + d(m_random_generator);
        }
        if (corrJet->pt() * smearFactor < 0.01) {
          smearFactor = 0.01 / corrJet->energy();
        }
      }
      corrJet->scaleEnergy(smearFactor);
      FillJetBranches(jet, *corrJet, nt, era_);
      float deepCSVprobb = -9999., deepCSVprobbb = -9999.;
      float deepFlavprobb = -9999., deepFlavprobbb = -9999.;
      float deepFlavproblepb = -9999.;
      for (const auto& pair : jet.getPairDiscri()) {
        if (pair.first == "pfDeepCSVJetTags:probb") {
          deepCSVprobb = pair.second;
        }
        if (pair.first == "pfDeepCSVJetTags:probbb") {
          deepCSVprobbb = pair.second;
        }
        if (pair.first == "pfDeepFlavourJetTags:probb") {
          deepFlavprobb = pair.second;
        }
        if (pair.first == "pfDeepFlavourJetTags:probbb") {
          deepFlavprobbb = pair.second;
        }
        if (pair.first == "pfDeepFlavourJetTags:problepb") {
          deepFlavproblepb = pair.second;
        }
      }
      if (deepCSVprobb != -9999. && deepCSVprobbb != -9999.) {
        nt.jets_bTag_deepCSV.push_back(deepCSVprobb + deepCSVprobbb);
      } else
        nt.jets_bTag_deepCSV.push_back(-9999.);
      if (deepFlavprobb != -9999. && deepFlavprobbb != -9999. && deepFlavproblepb != -9999.) {
        nt.jets_bTag_deepFlav.push_back(deepFlavprobb + deepFlavprobbb + deepFlavproblepb);
      } else
        nt.jets_bTag_deepFlav.push_back(-9999.);
    }
  } */

  // run over tracks and probes once prior to filling tree to determine ordering of pairs
  // this is necessary to use tag-probe pair with highest "quality" later on in spark_tnp
  // using t_pair_prob = std::pair<std::pair<int, int>, float>;
  // std::vector<t_pair_prob> pair_vtx_probs;
  // // loop over tags
  // for (const auto& tag : tag_muon_ttrack) {
  //   auto tag_idx = &tag - &tag_muon_ttrack[0];
  //   // loop over probes
  //   for (const auto& probe : tracks) {
  //     auto probe_idx = &probe - &tracks[0];

  //     // apply cuts on pairs
  //     if (tag.first.charge() == probe.charge())
  //       continue;
  //     if (fabs(tag.first.vz() - probe.vz()) > pairDz_ && pairDz_ > 0)
  //       continue;

  //     float mass = DimuonMass(tag.first.pt(), tag.first.eta(), tag.first.phi(), probe.pt(), probe.eta(), probe.phi());
  //     if (mass < pairMassMin_ || mass > pairMassMax_)
  //       continue;

  //     std::vector<reco::TransientTrack> trk_pair = {tag.second, reco::TransientTrack(probe.pseudoTrack(), &(*bField))};
  //     KlFitter vtx(trk_pair);
  //     if (RequireVtxCreation_ && !vtx.status())
  //       continue;
  //     if (minSVtxProb_ > 0 && vtx.prob() < minSVtxProb_)
  //       continue;

  //     // save vtx prob to sort later
  //     pair_vtx_probs.emplace_back(std::make_pair(std::make_pair(tag_idx, probe_idx), vtx.prob()));
  //   }
  // }
  // nt.npairs = pair_vtx_probs.size();

  // // reverse sort vertices by probability
  // auto compare_vtx = [=](t_pair_prob& a, t_pair_prob& b) { return a.second > b.second; };
  // std::sort(pair_vtx_probs.begin(), pair_vtx_probs.end(), compare_vtx);
  // // assign sorted vtx indices to ranking
  // map<std::pair<int, int>, int> pair_rank_vtx_prob;
  // for (size_t i = 0; i < pair_vtx_probs.size(); i++)
  //   pair_rank_vtx_prob[pair_vtx_probs[i].first] = i;

  // std::vector<t_pair_prob> StandAlone_pair_vtx_probs;
  // // loop over tags
  // for (const auto& tag : tag_muon_ttrack) {
  //   auto tag_idx = &tag - &tag_muon_ttrack[0];
  //   // loop over probes
  //   for (const pat::Muon& tmp_probe : *muons) {
  //     auto probe_idx = &tmp_probe - &muons->at(0);
  //     if (!tmp_probe.isStandAloneMuon())
  //       continue;
  //     const reco::Track probe = *tmp_probe.standAloneMuon();
  //     //apply cuts on probe
  //     if (!probeSelectionSA_(probe))
  //       continue;

  //     // apply cuts on pairs; selected will be saved
  //     if (tag.first.charge() == probe.charge())
  //       continue;

  //     float mass = DimuonMass(tag.first.pt(), tag.first.eta(), tag.first.phi(), probe.pt(), probe.eta(), probe.phi());
  //     if (mass < pairMassMin_ || mass > pairMassMax_)
  //       continue;

  //     std::vector<reco::TransientTrack> trk_pair = {tag.second, reco::TransientTrack(probe, &(*bField))};
  //     KlFitter vtx(trk_pair);
  //     // if (RequireVtxCreation_ && !vtx.status())
  //     // continue;
  //     // if (minSVtxProb_ > 0 && vtx.prob() < minSVtxProb_)
  //     // continue;

  //     // save vtx prob to sort later
  //     StandAlone_pair_vtx_probs.emplace_back(std::make_pair(std::make_pair(tag_idx, probe_idx), vtx.prob()));
  //   }
  // }
  // StandAlone_nt.npairs = StandAlone_pair_vtx_probs.size();

  // // reverse sort vertices by probability
  // auto StandAlone_compare_vtx = [=](t_pair_prob& a, t_pair_prob& b) { return a.second > b.second; };
  // std::sort(StandAlone_pair_vtx_probs.begin(), StandAlone_pair_vtx_probs.end(), StandAlone_compare_vtx);
  // // assign sorted vtx indices to ranking
  // map<std::pair<int, int>, int> StandAlone_pair_rank_vtx_prob;
  // for (size_t i = 0; i < StandAlone_pair_vtx_probs.size(); i++)
  //   StandAlone_pair_rank_vtx_prob[StandAlone_pair_vtx_probs[i].first] = i;
  // adding event info to standalone ntuple
  StandAloneFillEventInfo(nt, StandAlone_nt, HLTPaths_);

  // Final pair selection
  // loop over tags
  for (const auto& tag : tag_muon_ttrack) {
    // loop over Standalone muons
    if (saveStandAloneTree_) {
      for (const pat::Muon& tmp_probe : *muons) {
        if (!tmp_probe.isStandAloneMuon())
          continue;
        const reco::Track probe = *tmp_probe.standAloneMuon();
        //apply cuts on probe
        if (!probeSelectionSA_(probe))
          continue;
        if (muonOnly_ && !probeMuonSelection_(tmp_probe))
            continue;
        // apply cuts on pairs; selected will be saved
        if (tag.first.charge() == probe.charge())
          continue;

        float mass = DimuonMass(tag.first.pt(), tag.first.eta(), tag.first.phi(), probe.pt(), probe.eta(), probe.phi());
        if (mass < pairMassMin_ || mass > pairMassMax_)
          continue;
        math::PtEtaPhiMLorentzVector mu1(tag.first.pt(), tag.first.eta(), tag.first.phi(), MU_MASS);
        math::PtEtaPhiMLorentzVector mu2(probe.pt(), probe.eta(), probe.phi(), MU_MASS);

        // filling Standalone ntuple
        StandAlone_embedTriggerMatching(iEvent, trigResults, tag.first, StandAlone_nt, tagFilters_, true);
        StandAlone_nt.tag_isMatchedGen = genmatched_tag[&tag - &tag_muon_ttrack[0]];
        StandAloneFillTagBranches<pat::Muon, pat::PackedCandidate>(tag.first, tracks, StandAlone_nt, *pv);
        StandAloneFillPairBranches<pat::Muon, reco::Track>(tag.first, probe, StandAlone_nt);

        // finding trk idx for associated trk and matched trk via dR
        auto it = std::find(trk_SAmuon_map.second.begin(), trk_SAmuon_map.second.end(), &tmp_probe - &muons->at(0));
        auto assoc_it = std::find(
            associatedtrk_muon_map.second.begin(), associatedtrk_muon_map.second.end(), &tmp_probe - &muons->at(0));
        reco::Muon fakeMuon;
        fakeMuon.setP4(mu2);
        fakeMuon.setCharge(probe.charge());
        int match_trk_idx = -99;
        int assoc_trk_idx = -99;
        int match_muon_idx = &tmp_probe - &muons->at(0);
        std::pair<std::vector<bool>, std::vector<reco::Track>> match_tracks;
        if (it != trk_SAmuon_map.second.end()) {
          unsigned idx = std::distance(trk_SAmuon_map.second.begin(), it);
          match_trk_idx = trk_SAmuon_map.first[idx];
          match_tracks.first.push_back(true);
          match_tracks.second.push_back(nocut_tracks.at(match_trk_idx).pseudoTrack());
        }
        if (it == trk_SAmuon_map.second.end()) {
          match_tracks.first.push_back(false);
          match_tracks.second.push_back(nocut_tracks.at(0).pseudoTrack());
        }

        if (assoc_it != associatedtrk_muon_map.second.end()) {
          unsigned assoc_idx = std::distance(associatedtrk_muon_map.second.begin(), assoc_it);
          assoc_trk_idx = associatedtrk_muon_map.first[assoc_idx];
          match_tracks.first.push_back(true);         
          match_tracks.second.push_back(nocut_tracks.at(assoc_trk_idx).pseudoTrack());
        }
        if (assoc_it == associatedtrk_muon_map.second.end()) {
          match_tracks.first.push_back(false);         
          match_tracks.second.push_back(nocut_tracks.at(0).pseudoTrack());
        }
        

        //filling standalone probe ntuple info
        StandAloneFillProbeBranches<reco::Muon, pat::Muon, pat::PackedCandidate>(
            fakeMuon, *muons, tracks, StandAlone_nt, match_muon_idx, *pv, match_tracks);
        StandAlone_nt.probe_numofassoctrks = tmp_probe.numberOfSourceCandidatePtrs();
        if (iEvent.isRealData())
          StandAloneFillSimMatchingBranchesDummy(StandAlone_nt, true);
        else
          StandAloneFillSimMatchingBranches(tag.first, StandAlone_nt, true);
        StandAlone_nt.iprobe++;

        // if (iEvent.isRealData())
        //   //FillSimMatchingBranchesDummy(nt, false);
        // else
        //   //FillSimMatchingBranches(tmp_probe, nt, false);
       // StandAlone_nt.pair_rank_vtx_prob =
        //    StandAlone_pair_rank_vtx_prob[{&tag - &tag_muon_ttrack[0], &tmp_probe - &muons->at(0)}];
        // // StandAlone_nt.pair_rank_dz_PV_SV = SA_pair_rank_dz_PV_SV[{&tag - &tag_trkttrk[0], &tmp_probe - &muons->at(0)}];
        // StandAlone_nt.pair_rank_dPhi_muons = SA_pair_rank_dPhi_muons[{&tag - &tag_trkttrk[0], &tmp_probe - &muons->at(0)}];
        // StandAlone_nt.pair_rank_dM_Z_Mmumu = SA_pair_rank_dM_Z_Mmumu[{&tag - &tag_trkttrk[0], &tmp_probe - &muons->at(0)}];

        StandAlone_t1->Fill();
      }
    }
  }
}

// ------------ method called once each job just before starting event loop
// ------------
void StandAloneMuonMiniAODAnalyzer::beginJob() {
  t1 = fs->make<TTree>("Events", "Events");
  nt.SetTree(t1);
  nt.CreateBranches(HLTPaths_, probeSelectorNames_);
  //SA
  StandAlone_t1 = fs->make<TTree>("StandAloneEvents", "StandAloneEvents");
  StandAlone_nt.SetTree(StandAlone_t1);
  StandAlone_nt.CreateBranches(HLTPaths_);
  if (!tagFilters_.empty()){
    nt.CreateExtraTrgBranches(tagFilters_, true);
    StandAlone_nt.CreateExtraTrgBranches(tagFilters_, true);
  }
  if (!probeFilters_.empty()){
    nt.CreateExtraTrgBranches(probeFilters_, false);
    StandAlone_nt.CreateExtraTrgBranches(probeFilters_, false);
  }
}

// ------------ method called once each job just after ending the event loop
// ------------
void StandAloneMuonMiniAODAnalyzer::endJob() {}

// ------------ method fills 'descriptions' with the allowed parameters for the
// module  ------------
void StandAloneMuonMiniAODAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  // The following says we do not know what parameters are allowed so do no
  // validation
  // Please change this to state exactly what you do use, even if it is no
  // parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

///////////////////////

// define this as a plug-in
DEFINE_FWK_MODULE(StandAloneMuonMiniAODAnalyzer);
