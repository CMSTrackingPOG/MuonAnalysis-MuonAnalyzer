

//             version 2.0
//
/**\class

 Description: Ntuplizer class for full AOD files
*/
//
// Original Author:
//                george karathanasis
//         Created:  Thu, 20 feb 2020 17:40:23 GMT
//
// Modified:
//                Andre Frankenthal (Sept. 2020)
//
// Modified:
//                Minseok Oh (Feb. 2021)
//
// 

// system include files
#include <iostream>
#include <memory>
#include <random>
#include <queue>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/Association.h"
#include "DataFormats/Common/interface/AssociationMap.h"
#include "DataFormats/Common/interface/RefToPtr.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Framework/interface/Event.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "FWCore/Framework/interface/ConsumesCollector.h"

#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/L1Trigger/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include <vector>
#include "CommonTools/Statistics/interface/ChiSquaredProbability.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "DataFormats/Common/interface/AssociationMap.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/MuonReco/interface/MuonSimInfo.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "CondFormats/DataRecord/interface/JetResolutionRcd.h"
#include "CondFormats/DataRecord/interface/JetResolutionScaleFactorRcd.h"
#include "JetMETCorrections/Modules/interface/JetResolution.h"
#include "JetMETCorrections/JetCorrector/interface/JetCorrector.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "MagneticField/ParametrizedEngine/src/OAEParametrizedMagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
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
#include "MuonAnalysis/MuonAssociators/interface/PropagateToMuon.h"
#include "MuonAnalysis/MuonAssociators/interface/PropagateToMuonSetup.h"
#include "KlFitter.h"
#include "StandAloneMuonBranches.h"
#include "MuonGenAnalyzer.h"
#include "NtupleContent.h"
#include "StandAloneNtupleContent.h"
#include "helper.h"
#include "MuonMiniIsolation.h"
#include "JetsBranches.h"
#include "DataFormats/Luminosity/interface/LumiInfo.h"
#include "FWCore/Utilities/interface/InputTag.h"
using namespace std;

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.
class StandAloneMuonFullAODAnalyzer : public edm::one::EDAnalyzer<> {
public:
  typedef std::vector<std::pair<reco::Muon, reco::TransientTrack>> RecoTrkAndTransientTrkCollection;
  explicit StandAloneMuonFullAODAnalyzer(const edm::ParameterSet&);
  ~StandAloneMuonFullAODAnalyzer() override;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void beginJob() override;
  bool HLTaccept(const edm::Event&, NtupleContent&, std::vector<std::string>&);
  void fillHLTmuon(const edm::Event&,
                   std::vector<TString>&,
                   std::vector<float>&,
                   std::vector<float>&,
                   std::vector<float>&,
                   std::vector<std::string>&,
                   const int&);
  void embedTriggerMatching(const reco::Muon&,
                            std::vector<TString>&,
                            std::vector<float>&,
                            std::vector<float>&,
                            std::vector<float>&,
                            std::vector<std::string>&,
                            bool,
                            const int&);
  void StandAlone_embedTriggerMatching(const reco::Muon&,
                                       std::vector<TString>&,
                                       std::vector<float>&,
                                       std::vector<float>&,
                                       std::vector<float>&,
                                       StandAloneNtupleContent&,
                                       std::vector<std::string>&,
                                       bool,
                                       const int&);
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void endJob() override;
  edm::InputTag inputTag_;
  //edm::EDGetTokenT<LumiInfo> lumiToken_;
  edm::EDGetTokenT<GenEventInfoProduct> genEventInfoToken_;
  edm::EDGetTokenT<double> rhoToken_;
  edm::EDGetTokenT<std::vector<PileupSummaryInfo>> pileupSummaryToken_;
  edm::EDGetTokenT<reco::BeamSpot> beamSpotToken_;
  edm::EDGetTokenT<std::vector<reco::Vertex>> vtxToken_;
  edm::EDGetToken muonsToken_;
  edm::EDGetTokenT<edm::View<reco::Muon>> muonsViewToken_;
  edm::EDGetToken tracksToken_;
  edm::EDGetToken SAmuonsToken_;
  edm::EDGetToken dSAToken_;
  edm::EDGetToken dglToken_;
  edm::EDGetToken cosmicToken_;
  edm::EDGetTokenT<edm::TriggerResults> trgresultsToken_;
  edm::EDGetTokenT<trigger::TriggerEvent> trigobjectsToken_;
  edm::EDGetTokenT<pat::TriggerObjectStandAloneMatch> l1MatchesToken_;
  edm::EDGetTokenT<edm::ValueMap<int>> l1MatchesQualityToken_;
  edm::EDGetTokenT<edm::ValueMap<float>> l1MatchesDeltaRToken_;
  edm::EDGetTokenT<pat::TriggerObjectStandAloneMatch> l1MatchesByQToken_;
  edm::EDGetTokenT<edm::ValueMap<int>> l1MatchesByQQualityToken_;
  edm::EDGetTokenT<edm::ValueMap<float>> l1MatchesByQDeltaRToken_;
  edm::EDGetTokenT<edm::View<reco::GenParticle>> genToken_;
  edm::EDGetToken PFCands_;
  edm::EDGetTokenT<double> rhoJetsNC_;
  edm::EDGetToken jetsToken_;
  edm::EDGetToken jetCorrectorToken_;
  JME::JetResolution::Token jetResolutionToken_;
  JME::JetResolutionScaleFactor::Token jetResoultionScaleFactorToken_;
  edm::EDGetToken genJetsToken_;
  edm::EDGetToken deepCSVProbbToken_;
  edm::EDGetToken deepCSVProbbbToken_;
  //edm::EDGetToken deepFlavProbbToken_;
  //edm::EDGetToken deepFlavProbbbToken_;
  edm::EDGetTokenT<edm::ValueMap<reco::MuonSimInfo>> simInfoToken_;
  edm::ESGetToken<MagneticField, IdealMagneticFieldRecord> magfieldToken_;
  std::vector<std::string> HLTPaths_; // trigger fired
  std::vector<std::string> tagFilters_; // tag-trigger matching
  std::vector<std::string> probeFilters_;// probe-trigger matching
  std::vector<std::string> probeSelectorNames_;
  std::vector<unsigned> probeSelectorBits_;

  std::mt19937 m_random_generator = std::mt19937(37428479);
  const bool isMC_,isOnlySeeded_, includeJets_;
  const std::string era_;
  const double trgDRwindow_;
  const unsigned int tagQual_;
  const StringCutObjectSelector<reco::Muon> tagSelection_;  // kinematic cuts for tag
  const bool HighPurity_;
  const StringCutObjectSelector<reco::Track> probeSelection_;    // kinematic cuts for probe
  const StringCutObjectSelector<reco::Track> probeSelectionSA_;  // kinematic cuts for probe
  const bool muonOnly_;
  const StringCutObjectSelector<reco::Muon> probeMuonSelection_;
  const double pairMassMin_;
  const double pairMassMax_;
  const double pairDz_;
  const bool RequireVtxCreation_;  // if true skip pairs that do not create
                                   // that do not have a vertex
  const double minSVtxProb_;       // min probability of a vertex to be kept. If <0 inactive
  const double maxdz_trk_mu_;
  const double maxpt_relative_dif_trk_mu_;
  const double maxdr_trk_mu_;
  const double minpt_trkSA_;
  const double maxdz_trk_SAmu_;
  const double maxpt_relative_dif_trk_SAmu_;
  const double maxdr_trk_SAmu_;
  const double maxdr_trk_dsa_;
  const unsigned momPdgId_;
  const double genRecoDrMatch_;
  const bool saveStandAloneTree_;
  const bool saveTnPTree_;
  const int debug_;
  PropagateToMuon prop1_;
  PropagateToMuonSetup propSetup1_;

  edm::Service<TFileService> fs;
  TTree* t1;
  NtupleContent nt;

  TTree* StandAlone_t1;
  StandAloneNtupleContent StandAlone_nt;
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
StandAloneMuonFullAODAnalyzer::StandAloneMuonFullAODAnalyzer(const edm::ParameterSet& iConfig)
  :  // inputs
  inputTag_(iConfig.getParameter<edm::InputTag>("inputTag")), 
  //lumiToken_(consumes<LumiInfo>(inputTag_)),
  genEventInfoToken_(consumes<GenEventInfoProduct>(iConfig.getParameter<edm::InputTag>("genEventInfo"))),
  rhoToken_(consumes<double>(iConfig.getParameter<edm::InputTag>("Rho"))),
  pileupSummaryToken_(consumes<std::vector<PileupSummaryInfo>>(iConfig.getParameter<edm::InputTag>("pileupInfo"))),
  beamSpotToken_(consumes<reco::BeamSpot>(iConfig.getParameter<edm::InputTag>("beamSpot"))),
  vtxToken_(consumes<std::vector<reco::Vertex>>(iConfig.getParameter<edm::InputTag>("vertices"))),
  muonsToken_(consumes<std::vector<reco::Muon>>(iConfig.getParameter<edm::InputTag>("muons"))),
  muonsViewToken_(consumes<edm::View<reco::Muon>>(iConfig.getParameter<edm::InputTag>("muons"))),
  tracksToken_(consumes<std::vector<reco::Track>>(iConfig.getParameter<edm::InputTag>("tracks"))),
  SAmuonsToken_(consumes<std::vector<reco::Track>>(iConfig.getParameter<edm::InputTag>("SAmuons"))),
  dSAToken_(consumes<std::vector<reco::Track>>(iConfig.getParameter<edm::InputTag>("dSAmuons"))),
  dglToken_(consumes<std::vector<reco::Track>>(iConfig.getParameter<edm::InputTag>("dGlmuons"))),
  cosmicToken_(consumes<std::vector<reco::Track>>(iConfig.getParameter<edm::InputTag>("staCosmic"))),
  trgresultsToken_(consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("triggerResults"))),
  trigobjectsToken_(consumes<trigger::TriggerEvent>(iConfig.getParameter<edm::InputTag>("triggerObjects"))),
  l1MatchesToken_(consumes<pat::TriggerObjectStandAloneMatch>(iConfig.getParameter<edm::InputTag>("l1Matches"))),
  l1MatchesQualityToken_(consumes<edm::ValueMap<int>>(iConfig.getParameter<edm::InputTag>("l1MatchesQuality"))),
  l1MatchesDeltaRToken_(consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("l1MatchesDeltaR"))),
  l1MatchesByQToken_(consumes<pat::TriggerObjectStandAloneMatch>(iConfig.getParameter<edm::InputTag>("l1MatchesByQ"))),
  l1MatchesByQQualityToken_(consumes<edm::ValueMap<int>>(iConfig.getParameter<edm::InputTag>("l1MatchesByQQuality"))),
  l1MatchesByQDeltaRToken_(consumes<edm::ValueMap<float>>(iConfig.getParameter<edm::InputTag>("l1MatchesByQDeltaR"))),
  genToken_(consumes<edm::View<reco::GenParticle>>(iConfig.getParameter<edm::InputTag>("gen"))),
  PFCands_(consumes<std::vector<reco::PFCandidate>>(iConfig.getParameter<edm::InputTag>("PFCands"))),
  rhoJetsNC_(consumes<double>(iConfig.getParameter<edm::InputTag>("rhoJetsNC"))),
  jetsToken_(consumes<std::vector<reco::PFJet>>(iConfig.getParameter<edm::InputTag>("jets"))),
  jetCorrectorToken_(consumes<reco::JetCorrector>(iConfig.getParameter<edm::InputTag>("jetCorrector"))),
  jetResolutionToken_(esConsumes(edm::ESInputTag("", iConfig.getParameter<std::string>("jetResType")))),
  jetResoultionScaleFactorToken_(esConsumes(edm::ESInputTag("", iConfig.getParameter<std::string>("jetResSFType")))),
  genJetsToken_(consumes<std::vector<reco::GenJet>>(iConfig.getParameter<edm::InputTag>("genJets"))),
  deepCSVProbbToken_(consumes<reco::JetTagCollection>(iConfig.getParameter<edm::InputTag>("deepCSVProbb"))),
  deepCSVProbbbToken_(consumes<reco::JetTagCollection>(iConfig.getParameter<edm::InputTag>("deepCSVProbbb"))),
  simInfoToken_(consumes<edm::ValueMap<reco::MuonSimInfo>>(iConfig.getParameter<edm::InputTag>("muonSimInfo"))),
  HLTPaths_(iConfig.getParameter<std::vector<std::string>>("triggerPaths")),
  tagFilters_(iConfig.getParameter<std::vector<std::string>>("tagFilters")),
  probeFilters_(iConfig.getParameter<std::vector<std::string>>("probeFilters")),
  probeSelectorNames_(iConfig.getParameter<std::vector<std::string>>("probeSelectorNames")),
  probeSelectorBits_(iConfig.getParameter<std::vector<unsigned>>("probeSelectorBits")),
  isMC_(iConfig.getParameter<bool>("isMC")),
  isOnlySeeded_(iConfig.getParameter<bool>("isOnlySeeded")),
  includeJets_(iConfig.getParameter<bool>("includeJets")),
  era_(iConfig.getParameter<std::string>("era")),
  trgDRwindow_(iConfig.getParameter<double>("trgDRwindow")),
  tagQual_(iConfig.getParameter<unsigned>("tagQuality")),
  tagSelection_(iConfig.getParameter<std::string>("tagSelection")),
  HighPurity_(iConfig.getParameter<bool>("probeHPurity")),
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
  maxdr_trk_dsa_(iConfig.getParameter<double>("maxDRProbeTrkDSA")),
  momPdgId_(iConfig.getParameter<unsigned>("momPdgId")),
  genRecoDrMatch_(iConfig.getParameter<double>("genRecoDrMatch")),
  saveStandAloneTree_(iConfig.getParameter<bool>("saveStandAloneTree")),
  saveTnPTree_(iConfig.getParameter<bool>("saveTnPTree")),
  debug_(iConfig.getParameter<int>("debug")),
  propSetup1_(iConfig, consumesCollector()) {
  edm::ConsumesCollector iC = consumesCollector();
  magfieldToken_ = iC.esConsumes<MagneticField, IdealMagneticFieldRecord>();
  if (probeSelectorNames_.size() != probeSelectorBits_.size()) {
    throw cms::Exception("ParameterError")
      << "length of probeSelectorNames and probeSelectorBits should be identical\n";
  }
}

StandAloneMuonFullAODAnalyzer::~StandAloneMuonFullAODAnalyzer() {
  // cout << "total " << trg_counter << " fires " << fire_counter << " l3"
  // << l3_counter << endl; do anything here that needs to be done at
  // desctruction time
}

bool StandAloneMuonFullAODAnalyzer::HLTaccept(const edm::Event& iEvent,
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

void StandAloneMuonFullAODAnalyzer::fillHLTmuon(const edm::Event& iEvent,
                                                std::vector<TString>& trg_filter,
                                                std::vector<float>& trg_pt,
                                                std::vector<float>& trg_eta,
                                                std::vector<float>& trg_phi,
                                                std::vector<std::string>& HLTFilters,
                                                const int& debug_) {
  edm::Handle<trigger::TriggerEvent> triggerObjects;
  iEvent.getByToken(trigobjectsToken_, triggerObjects);
  trigger::TriggerObjectCollection allTriggerObjects = triggerObjects->getObjects();
  for (auto ifilter : HLTFilters) {
    size_t filterIndex = (*triggerObjects).filterIndex(edm::InputTag(ifilter, "", "HLT"));
    if (filterIndex < (*triggerObjects).sizeFilters()) {
      const trigger::Keys& keys = (*triggerObjects).filterKeys(filterIndex);
      for (size_t j = 0; j < keys.size(); j++) {
        trigger::TriggerObject foundObject = (allTriggerObjects)[keys[j]];
        if (fabs(foundObject.id()) != 13)
          continue;
        trg_filter.push_back(TString(ifilter));
        trg_pt.push_back(foundObject.pt());
        trg_eta.push_back(foundObject.eta());
        trg_phi.push_back(foundObject.phi());
       // if (debug_ > 0)
       //   std::cout << "Trg muon " << foundObject.pt() << std::endl;
      }
    }
  }
}

void StandAloneMuonFullAODAnalyzer::embedTriggerMatching(const reco::Muon& mu,
                                                         std::vector<TString>& trg_filter,
                                                         std::vector<float>& trg_pt,
                                                         std::vector<float>& trg_eta,
                                                         std::vector<float>& trg_phi,
                                                         std::vector<std::string>& HLTFilters,
                                                         bool isTag,
                                                         const int& debug_ = 0) {
  for (const auto& thefilter : HLTFilters) {
    TString thefilter_tstr = TString(thefilter);
    // temporary method to tag L2 filters for dSA paths...
    
    bool matched = false;
    float matched_pt = -99;
    float matched_eta = -99;
    float matched_phi = -99;
    float matched_dr = 99;
    for (unsigned itrg = 0; itrg < trg_filter.size(); ++itrg) {
      TString filter_tstr = TString(trg_filter.at(itrg));
      if (!filter_tstr.Contains(thefilter_tstr))
        continue;
      float dptrel_tmp = fabs(mu.pt()-trg_pt.at(itrg))/(trg_pt.at(itrg)); //[pt(mu)-pt(trk)]/pt(trk)
      float dR_tmp = deltaR(mu.eta(), mu.phi(), trg_eta.at(itrg), trg_phi.at(itrg));
      if ((dR_tmp < matched_dr && dR_tmp < trgDRwindow_ && dptrel_tmp < 10.) ) {//||
         // (isL2DSA && dR_tmp < matched_dr && dR_tmp < maxdr_trk_dsa_ && dptrel_tmp < 10.)) {
        matched = true;
        matched_pt = trg_pt.at(itrg);
        matched_eta = trg_eta.at(itrg);
        matched_phi = trg_phi.at(itrg);
        matched_dr = dR_tmp;

       // if (debug_ > 0) {
       //   std::cout << "embedTriggerMatching: isTag=" << isTag << "  filter=" << thefilter_tstr << "  dR=" << dR_tmp
       //             << "  matched=" << matched << std::endl;
        //}
      }
    }

    if (isTag) {
      nt.tag_trg[&thefilter - &HLTFilters[0]] = matched;
      nt.tag_trg_pt[&thefilter - &HLTFilters[0]] = matched_pt;
      nt.tag_trg_eta[&thefilter - &HLTFilters[0]] = matched_eta;
      nt.tag_trg_phi[&thefilter - &HLTFilters[0]] = matched_phi;
      nt.tag_trg_dr[&thefilter - &HLTFilters[0]] = matched_dr;
    } else {
      nt.probe_trg[&thefilter - &HLTFilters[0]] = matched;
      nt.probe_trg_pt[&thefilter - &HLTFilters[0]] = matched_pt;
      nt.probe_trg_eta[&thefilter - &HLTFilters[0]] = matched_eta;
      nt.probe_trg_phi[&thefilter - &HLTFilters[0]] = matched_phi;
      nt.probe_trg_dr[&thefilter - &HLTFilters[0]] = matched_dr;
    }
  }  
    return;
  
}

void StandAloneMuonFullAODAnalyzer::StandAlone_embedTriggerMatching(const reco::Muon& mu,
                                                                    std::vector<TString>& trg_filter,
                                                                    std::vector<float>& trg_pt,
                                                                    std::vector<float>& trg_eta,
                                                                    std::vector<float>& trg_phi,
                                                                    StandAloneNtupleContent& StandAlone_nt,
                                                                    std::vector<std::string>& HLTFilters,
                                                                    bool isTag,
                                                                    const int& debug_ = 0) {
  for (const auto& thefilter : HLTFilters) {
    TString thefilter_tstr = TString(thefilter);
    // check matching between tag / probe and trigger objects
    
    bool matched = false;
    float matched_pt = -99;
    float matched_eta = -99;
    float matched_phi = -99;
    float matched_dr = 99;
    for (unsigned itrg = 0; itrg < trg_filter.size(); ++itrg) {
      TString filter_tstr = TString(trg_filter.at(itrg));
      float dR_tmp = deltaR(mu.eta(), mu.phi(), trg_eta.at(itrg), trg_phi.at(itrg));
      float dptrel_tmp = fabs(mu.pt()-trg_pt.at(itrg))/(trg_pt.at(itrg));
      if (!filter_tstr.Contains(thefilter_tstr))
        continue;
       if ((dR_tmp < matched_dr && dR_tmp < trgDRwindow_ && dptrel_tmp < 10.) ) {//||
         // (isL2DSA && dR_tmp < matched_dr && dR_tmp < maxdr_trk_dsa_ && dptrel_tmp < 10.)) {
        matched = true;
        matched_pt = trg_pt.at(itrg);
        matched_eta = trg_eta.at(itrg);
        matched_phi = trg_phi.at(itrg);
        matched_dr = dR_tmp;
        //if (debug_ > 0) {
        //  std::cout << "StandAlone_embedTriggerMatching: isTag=" << isTag << "  filter=" << thefilter_tstr
        //            << "  dR=" << dR_tmp << "  matched=" << matched << std::endl;
        //}
      }
    }
    if (isTag) {
      StandAlone_nt.tag_trg[&thefilter - &HLTFilters[0]] = matched;
      StandAlone_nt.tag_trg_pt[&thefilter - &HLTFilters[0]] = matched_pt;
      StandAlone_nt.tag_trg_eta[&thefilter - &HLTFilters[0]] = matched_eta;
      StandAlone_nt.tag_trg_phi[&thefilter - &HLTFilters[0]] = matched_phi;
      StandAlone_nt.tag_trg_dr[&thefilter - &HLTFilters[0]] = matched_dr;
    }
     else {
     StandAlone_nt.probe_trg[&thefilter - &HLTFilters[0]] = matched;
     StandAlone_nt.probe_trg_pt[&thefilter - &HLTFilters[0]] = matched_pt;
     StandAlone_nt.probe_trg_eta[&thefilter - &HLTFilters[0]] = matched_eta;
     StandAlone_nt.probe_trg_phi[&thefilter - &HLTFilters[0]] = matched_phi;
     StandAlone_nt.probe_trg_dr[&thefilter - &HLTFilters[0]] = matched_dr;
     }
  }
  return;
  
}
//
void StandAloneMuonFullAODAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace std;
  using namespace edm;
  using namespace reco;
  using namespace trigger;

   prop1_ = propSetup1_.init(iSetup);
  
  // Get data
  edm::Handle<reco::BeamSpot> theBeamSpot;
  iEvent.getByToken(beamSpotToken_, theBeamSpot);
  edm::Handle<reco::VertexCollection> vertices;
  iEvent.getByToken(vtxToken_, vertices);

  // Skip evts if there are no vertices
  if (vertices->empty())
    return;

  edm::Handle<std::vector<reco::Track>> SAmuons;
  iEvent.getByToken(SAmuonsToken_, SAmuons);
  edm::Handle<std::vector<reco::Muon>> muons;
  iEvent.getByToken(muonsToken_, muons);
  edm::Handle<edm::View<reco::Muon>> muonsView;
  iEvent.getByToken(muonsViewToken_, muonsView);
  edm::Handle<std::vector<reco::Track>> tracks;
  iEvent.getByToken(tracksToken_, tracks);
  edm::Handle<std::vector<reco::Track>> dSAmuons;
  iEvent.getByToken(dSAToken_, dSAmuons);
  edm::Handle<std::vector<reco::Track>> dGlmuons;
  iEvent.getByToken(dglToken_, dGlmuons);
  edm::Handle<std::vector<reco::Track>> staCosmic;
  iEvent.getByToken(cosmicToken_, staCosmic);
  edm::ESHandle<MagneticField> bField;
  bField = iSetup.getHandle(magfieldToken_);
  // mini isolation
  edm::Handle<std::vector<reco::PFCandidate>> pfcands;
  iEvent.getByToken(PFCands_, pfcands);
  edm::Handle<double> rhoJetsNC;
  iEvent.getByToken(rhoJetsNC_, rhoJetsNC);
  // jets
  edm::Handle<std::vector<reco::PFJet>> jets;
  edm::Handle<reco::JetCorrector> jetCorrector;
  edm::Handle<reco::JetTagCollection> deepCSVProbb;
  edm::Handle<reco::JetTagCollection> deepCSVProbbb;
  JME::JetResolution resolution;
  JME::JetResolutionScaleFactor resolution_sf;
  if (includeJets_) {
    iEvent.getByToken(jetsToken_, jets);
    iEvent.getByToken(jetCorrectorToken_, jetCorrector);
    iEvent.getByToken(deepCSVProbbToken_, deepCSVProbb);
    iEvent.getByToken(deepCSVProbbbToken_, deepCSVProbbb);
    //  edm::Handle<reco::JetTagCollection> deepFlavProbb;
    //  iEvent.getByToken(deepFlavProbbToken_, deepFlavProbb);
    //  edm::Handle<reco::JetTagCollection> deepFlavProbbb;
    //  iEvent.getByToken(deepFlavProbbbToken_, deepFlavProbbb);
    resolution    = iSetup.getData(jetResolutionToken_);
    resolution_sf = iSetup.getData(jetResoultionScaleFactorToken_);
  }  
  edm::Handle<trigger::TriggerEvent> triggerObjects;
  iEvent.getByToken(trigobjectsToken_, triggerObjects);
  edm::Handle<pat::TriggerObjectStandAloneMatch> l1Matches;
  iEvent.getByToken(l1MatchesToken_, l1Matches);
  edm::Handle<edm::ValueMap<int>> l1Qualities;
  iEvent.getByToken(l1MatchesQualityToken_, l1Qualities);
  edm::Handle<edm::ValueMap<float>> l1Drs;
  iEvent.getByToken(l1MatchesDeltaRToken_, l1Drs);
  edm::Handle<pat::TriggerObjectStandAloneMatch> l1MatchesByQ;
  iEvent.getByToken(l1MatchesByQToken_, l1MatchesByQ);
  edm::Handle<edm::ValueMap<int>> l1QualitiesByQ;
  iEvent.getByToken(l1MatchesByQQualityToken_, l1QualitiesByQ);
  edm::Handle<edm::ValueMap<float>> l1DrsByQ;
  iEvent.getByToken(l1MatchesByQDeltaRToken_, l1DrsByQ);


  //SA
  StandAlone_nt.ClearBranches();

  // Information about run
  nt.ClearBranches();
  nt.run = iEvent.id().run();
  nt.ls = iEvent.luminosityBlock();
  
  //const LumiInfo& lumi = iEvent.get(lumiToken_);

  //  nt.istlumi = lumi.getTotalInstLumi();
  //std::cout << "Luminosity for " << iEvent.run() << " LS " << iEvent.luminosityBlock() << " is "
  //          << lumi.getTotalInstLumi() << std::endl;
  //edm::Handle<LumiSummary> l;
  ////l.getByLabel(iEvent.getLuminosityBlock(),"lumiProducer");
  ////std::cout << " Inst.  Luminosity = " << l->avgInsDelLumi()<<std::endl;
  ////nt.istlumi = l->avgInsDelLumi();
  //iEvent.getLuminosityBlock().getByLabel("lumiProducer", l);
  //nt.istlumi = l->avgInsDelLumi();
  //if (l.isValid()){
  //  float Avginstlumi=l->avgInsDelLumi();
   // std::cout << "Instataneous luminosity= " << l->avgInsDelLumi() << std::endl;
  //}
  //else { std::cout << "lumisummary not valid" << std::endl;}

  nt.event = iEvent.id().event();
  nt.fromFullAOD = true;
  nt.BSpot_x = theBeamSpot->x0();
  nt.BSpot_y = theBeamSpot->y0();
  nt.BSpot_z = theBeamSpot->z0();
  nt.nvertices = vertices->size();

  // Gen weights, sim info
  bool simInfoIsAvailalbe = false;
  edm::Handle<edm::ValueMap<reco::MuonSimInfo>> simInfo;
  if (!iEvent.isRealData()) {
    edm::Handle<GenEventInfoProduct> genEventInfoHandle;
    iEvent.getByToken(genEventInfoToken_, genEventInfoHandle);
    nt.genWeight = genEventInfoHandle->weight();

    simInfoIsAvailalbe = iEvent.getByToken(simInfoToken_, simInfo);
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

 // if (debug_ > 0)
 //   std::cout << "New Evt " << nt.run << std::endl;
if (debug_ > 0) {
      std::cout << std::endl << "NUOVO EVENTO" << std::endl << std::endl;
      std::cout << "New Evt " << nt.event << std::endl;
      std::cout << "Run " << nt.run << std::endl;
      //   std::cout << "Totale dei tag muons: " << tag_trkttrk.size() << std::endl;
  }
  reco::TrackBase::Point vertex_point;
  bool goodVtx = false;
  reco::Vertex const* pv;
  for (const reco::Vertex& vtx : *vertices) {
    if ((vtx.isFake() || !vtx.isValid()) || (vtx.ndof() <= 4) || abs(vtx.z()) > 25 || ((vtx.position()).Rho() > 2 ))
      continue;
    nt.pv_x = vtx.x();
    nt.pv_y = vtx.y();
    nt.pv_z = vtx.z();
    StandAlone_nt.pv_x = vtx.x();
    StandAlone_nt.pv_y = vtx.y();
    StandAlone_nt.pv_z = vtx.z();
    goodVtx = true;
    pv = &vtx;
    break;
  }
  //if (goodVtx){
   // std::cout << "GoodVtx found!" << std::endl;  // going on in presence of good vertex
  //}
  if (!goodVtx){
    //std::cout << "No goodVtx found!" << std::endl;
    return;  // skipping in absence of good vertex
  }
  vertex_point.SetCoordinates(nt.pv_x, nt.pv_y, nt.pv_z);

  // check if path fired, if so save hlt muons
  if (!HLTaccept(iEvent, nt, HLTPaths_))
    return;
  fillHLTmuon(iEvent, nt.trg_filter, nt.trg_pt, nt.trg_eta, nt.trg_phi, tagFilters_, debug_);
  fillHLTmuon(iEvent, nt.prb_filter, nt.prb_pt, nt.prb_eta, nt.prb_phi, probeFilters_, debug_);

  // gen information
  MuonGenAnalyzer genmu;
  std::vector<unsigned> matched_muon_idx;
  std::vector<unsigned> matched_track_idx;
  if (!iEvent.isRealData()) {
    genmu.SetInputs(iEvent, genToken_, momPdgId_);
    genmu.FillNtuple(nt);

    auto reco_match_genmu1 =
        MatchReco<reco::Muon>(*muons, nt.genmu1_eta, nt.genmu1_phi, nt.genmu1_charge, genRecoDrMatch_);
    auto reco_match_genmu2 =
        MatchReco<reco::Muon>(*muons, nt.genmu2_eta, nt.genmu2_phi, nt.genmu2_charge, genRecoDrMatch_);

    if (reco_match_genmu1.first)
      matched_muon_idx.push_back(reco_match_genmu1.second);

    if (reco_match_genmu2.first)
      matched_muon_idx.push_back(reco_match_genmu2.second);

    reco_match_genmu1 =
        MatchReco<reco::Track>(*tracks, nt.genmu1_eta, nt.genmu1_phi, nt.genmu1_charge, genRecoDrMatch_);
    reco_match_genmu2 =
        MatchReco<reco::Track>(*tracks, nt.genmu2_eta, nt.genmu2_phi, nt.genmu2_charge, genRecoDrMatch_);

    if (reco_match_genmu1.first)
      matched_track_idx.push_back(reco_match_genmu1.second);
    if (reco_match_genmu2.first)
      matched_track_idx.push_back(reco_match_genmu2.second);
  }

  // match hlt with offline muon
  std::vector<unsigned> trg_idx;
  for (unsigned itrg = 0; itrg < nt.trg_pt.size(); ++itrg) {
    float minDR = 1000;
    float maxptreltrg = 10.;
    unsigned idx = 0;
    for (auto& mu : *muons) {
      if (minDR < deltaR(nt.trg_eta[itrg], nt.trg_phi[itrg], mu.eta(), mu.phi()) && (fabs(mu.pt() - nt.trg_pt[itrg])/(nt.trg_pt[itrg])) < maxptreltrg)
        continue;
      minDR = deltaR(nt.trg_eta[itrg], nt.trg_phi[itrg], mu.eta(), mu.phi());
      idx = &mu - &muons->at(0);
    }
    //if (debug_ > 0)
    //  std::cout << "Trg " << itrg << ", min DR " << minDR << std::endl;
    if (minDR < trgDRwindow_)
      trg_idx.push_back(idx);
    //if (minDR < trgDRwindow_ && debug_ > 0)
    //  std::cout << "Matched!" << std::endl;
  }
  nt.nmuons = muons->size();

  // select tags
  //std::cout << "sto entrando nel loop dei tags" << std::endl;
  int tag_muon_index = 0;
  std::vector<unsigned> tag_muon_map;  // idx of tag muon in muons
  RecoTrkAndTransientTrkCollection tag_trkttrk;
  std::vector<bool> genmatched_tag;
  for (const auto& mu : *muons) {
    tag_muon_index++;
    if (mu.selectors() != 0) {  // Only 9_4_X and later have selector bits
      if (!mu.passed(static_cast<uint64_t>(pow(2, tagQual_))))
        continue;
    } else {  // For 2016, assume loose ID on the tag (can be tightened at spark level)
      if (!muon::isLooseMuon(mu))
        continue;
    }
    if (!tagSelection_(mu))
      continue;
    if (std::find(trg_idx.begin(), trg_idx.end(), &mu - &muons->at(0)) == trg_idx.end())
      continue;
    tag_trkttrk.emplace_back(std::make_pair(mu, reco::TransientTrack(*mu.bestTrack(), &(*bField))));
    tag_muon_map.push_back(&mu - &muons->at(0));
    if (std::find(matched_muon_idx.begin(), matched_muon_idx.end(), &mu - &muons->at(0)) != matched_muon_idx.end())
      genmatched_tag.push_back(true);
    else
      genmatched_tag.push_back(false);
  }
  nt.ntag = tag_trkttrk.size();

 // if (debug_ > 0) {
 //   std::cout << "Totale dei tag muons: " << tag_trkttrk.size() << std::endl;
 // }
  // probe track mapping with muon object
  std::pair<std::vector<unsigned>, std::vector<unsigned>> trk_muon_map;
  for (const auto& mu : *muons) {
    if (muonOnly_ && !probeMuonSelection_(mu))
      continue;
    float minDR = 1000;
    unsigned int idx_trk;
    //if (debug_ > 1)
    //  std::cout << "New trk-muon map entry pt " << mu.pt() << " eta " << mu.eta() << " phi " << mu.phi() << std::endl;
    for (const reco::Track& trk : *tracks) {
      if (mu.charge() != trk.charge())
        continue;
      if ((trk.pt() <= minpt_trkSA_) && (abs(trk.eta()) <= 1. || trk.p() <= 2.))
        continue;
      if (fabs(mu.vz() - trk.vz()) > maxdz_trk_mu_ && maxdz_trk_mu_ > 0)
        continue;
      if (fabs(mu.eta() - trk.eta()) > 0.3)
        continue;
      if (fabs(mu.pt() - trk.pt()) / mu.pt() > maxpt_relative_dif_trk_mu_ && maxpt_relative_dif_trk_mu_ > 0)
        continue;
      float DR = deltaR(mu.eta(), mu.phi(), trk.eta(), trk.phi());
      //if (debug_ > 1)
      //  std::cout << "   DR " << DR << "  " << mu.eta() << "  " << mu.phi() << "  " << trk.eta() << "  " << trk.phi()
      //            << std::endl;
      if (minDR < DR)
        continue;
      minDR = DR;
      idx_trk = &trk - &tracks->at(0);
    }
    if (minDR > maxdr_trk_mu_)
      continue;
    trk_muon_map.first.push_back(idx_trk);
    trk_muon_map.second.push_back(&mu - &muons->at(0));
  }
  //if (debug_ > 0)
  //  std::cout << "Matched trk-mu " << trk_muon_map.first.size() << std::endl;

  //SA probe track mapping with StandAlone muon object
  std::pair<std::vector<unsigned>, std::vector<unsigned>> SA_trk_muon_map;
  for (const auto& tmp_mu : *muons) {
    if (!tmp_mu.isStandAloneMuon())
      continue;
    if (!((*tmp_mu.standAloneMuon()).numberOfValidHits() > 0.))
          continue;
   if (muonOnly_ && !probeMuonSelection_(tmp_mu))
      continue;
    const reco::Track mu = *tmp_mu.standAloneMuon();
    float minDR = 1000;
    unsigned int idx_trk;
   // if (debug_ > 0)
   //   std::cout << "Standalone New trk-muon map entry pt " << mu.pt() << " eta " << mu.eta() << " phi " << mu.phi()
   //             << " vz " << mu.vz() << std::endl;
    for (const reco::Track& trk : *tracks) {
     // if (debug_ > 1)
     //   std::cout << "Trk New trk-muon map entry pt " << trk.pt() << " eta " << trk.eta() << " phi " << trk.phi()
     //             << " vz " << trk.vz() << std::endl;
      if (mu.charge() != trk.charge())
        continue;
      if ((trk.pt() <= minpt_trkSA_) && (abs(trk.eta()) <= 1. || trk.p() <= 2.))
        continue;
      if (fabs(mu.vz() - trk.vz()) >= maxdz_trk_SAmu_ && maxdz_trk_SAmu_ > 0)
        continue;
      if (fabs(mu.eta() - trk.eta()) > 0.3)
        continue;
      if (fabs(mu.pt() - trk.pt()) / trk.pt() > maxpt_relative_dif_trk_SAmu_ && maxpt_relative_dif_trk_SAmu_)
        continue;
      float DR = deltaR(mu.eta(), mu.phi(), trk.eta(), trk.phi());
      if (minDR < DR)
        continue;
     // if (debug_ > 1)
     //   std::cout << "   DR " << DR << "  " << mu.eta() << "  " << mu.phi() << "  " << trk.eta() << "  " << trk.phi()
     //             << std::endl;
      minDR = DR;
      idx_trk = &trk - &tracks->at(0);
    }

    if (minDR > maxdr_trk_SAmu_)
      continue;

    SA_trk_muon_map.first.push_back(idx_trk);
    SA_trk_muon_map.second.push_back(&tmp_mu - &muons->at(0));
  }
 // if (debug_ > 0)
 //   std::cout << "Matched Standalone trk-mu " << SA_trk_muon_map.first.size() << std::endl;

  // MN loop to match standalone muons to closest trk in DR and associated trk if exists
  std::pair<std::vector<unsigned>, std::vector<unsigned>> trk_SAmuon_map;
  std::pair<std::vector<unsigned>, std::vector<unsigned>> associatedtrk_muon_map;
  

  

  // loop to compute numerator of tracking efficiency + fake rate
  for (const auto& mu : *muons) {
    if (!mu.isStandAloneMuon())
      continue;
    if (!((*mu.standAloneMuon()).numberOfValidHits() > 0.))
          continue;
    if (muonOnly_ && !probeMuonSelection_(mu))
      continue;
    const reco::Track SA_mu = *mu.standAloneMuon();
    float minDR = 1000;
    float minDR_assoc = 1000;
    unsigned int idx_trk = std::numeric_limits<unsigned int>::max();  // Initialize idx_trk
    unsigned int idx_associatedtrk = std::numeric_limits<unsigned int>::max();  // Initialize idx_associatedtrk
    unsigned int idx_tag_temp = std::numeric_limits<unsigned int>::max();  // Initialize idx_tag_temp
    unsigned int idx_tag_assoc = std::numeric_limits<unsigned int>::max();  // Initialize idx_tag_temp
    std::vector<unsigned int> idx_tag_assocVec(tag_trkttrk.size(), std::numeric_limits<unsigned int>::max());
    bool hasAssociatedTrkMatch = false;
    const reco::Track trk_mu = *mu.standAloneMuon();
    if (debug_ > 0)
      std::cout << "Looking at probe with index: " << &mu - &muons->at(0) << std::endl;
    
    for (const reco::Track& trk : *tracks) {

      std::vector<bool> isAssocVec(tag_trkttrk.size(), false); // Initialize vector of booleans
      std::vector<bool> isZmassVec(tag_trkttrk.size(), false); // Initialize vector of booleans


      if((trk.pt() <= minpt_trkSA_) && (abs(trk.eta()) <= 1. || trk.p() <= 2.)){
       std::cout << "Indice traccia " << &trk - &tracks->at(0) <<" Matching non passa per: trkPt,trkP,trkEta" << std::endl;
          continue; // requirement on track
      }
      
      for (const auto& tag : tag_trkttrk) {
	if (fabs(tag.first.vz() - trk.vz()) < maxdz_trk_SAmu_ ) { isAssocVec[&tag - &tag_trkttrk[0]] = true; idx_tag_assocVec.push_back(&tag - &tag_trkttrk[0]); if (debug_ > 0) {std::cout<< "Indice traccia: " << &trk - &tracks->at(0) << " Indice tag associato: " << &tag - &tag_trkttrk[0] << std::endl;} }
      } // check displacement of tag from tracks, we want to do the matching only with no displaced tracks

      for (const auto& tag : tag_trkttrk) {
      float mass_tagtrack = DimuonMass(tag.first.pt(), tag.first.eta(), tag.first.phi(), trk.pt(), trk.eta(), trk.phi());
      idx_tag_temp = &tag - &tag_trkttrk[0]; 
      if (mass_tagtrack >= 40 && mass_tagtrack <= 200 && std::find(idx_tag_assocVec.begin(), idx_tag_assocVec.end(), idx_tag_temp) != idx_tag_assocVec.end()) { isZmassVec[&tag - &tag_trkttrk[0]] = true;  }
      }

      bool charge_match = trk_mu.charge() == trk.charge();
      bool pt_match = fabs(trk_mu.pt() - trk.pt())/trk.pt() < maxpt_relative_dif_trk_SAmu_ && maxpt_relative_dif_trk_SAmu_ > 0;
      bool DeltaR_match = deltaR(trk_mu.eta(), trk_mu.phi(), trk.eta(), trk.phi()) < maxdr_trk_SAmu_;
      bool DeltaEta_match = fabs(trk_mu.eta() - trk.eta()) < 0.3;
      // Check association with any tag
      bool isAssoc = std::any_of(isAssocVec.begin(), isAssocVec.end(), [](bool val) { return val; });
      // Check association with any tag
      bool isZmass = std::any_of(isZmassVec.begin(), isZmassVec.end(), [](bool val) { return val; });
    
      if(charge_match && pt_match && DeltaR_match && isAssoc && DeltaEta_match && !(isZmass)) {
      
       if(minDR_assoc >= deltaR(trk_mu.eta(), trk_mu.phi(), trk.eta(), trk.phi())){
         minDR_assoc = deltaR(trk_mu.eta(), trk_mu.phi(), trk.eta(), trk.phi());
       }
       bool isTrackerOnlyseeded = trk.isAlgoInMask(trk.initialStep) || trk.isAlgoInMask(trk.lowPtTripletStep) || trk.isAlgoInMask(trk.pixelPairStep) || trk.isAlgoInMask(trk.detachedTripletStep) ||
        trk.isAlgoInMask(trk.mixedTripletStep) || trk.isAlgoInMask(trk.pixelLessStep) || trk.isAlgoInMask(trk.tobTecStep) || trk.isAlgoInMask(trk.jetCoreRegionalStep) || trk.isAlgoInMask(trk.lowPtQuadStep) || trk.isAlgoInMask(trk.highPtTripletStep) || trk.isAlgoInMask(trk.detachedQuadStep);
         if(!isTrackerOnlyseeded && isOnlySeeded_)
           continue;
         if(minDR_assoc >= deltaR(trk_mu.eta(), trk_mu.phi(), trk.eta(), trk.phi())){
           idx_associatedtrk = &trk - &tracks->at(0);
           hasAssociatedTrkMatch = true;
           minDR_assoc = deltaR(trk_mu.eta(), trk_mu.phi(), trk.eta(), trk.phi());
         }
         if(minDR_assoc < maxdr_trk_SAmu_ && hasAssociatedTrkMatch && (&trk == &tracks->back())) {
           associatedtrk_muon_map.first.push_back(idx_associatedtrk);         // stora indice traccia
           associatedtrk_muon_map.second.push_back(&mu - &muons->at(0)); // stora indice muone  
           if (debug_ > 0) {
           std::cout << " Salvato un matching fake nella mappa! " << std::endl;
           std::cout << " indice muone probe: " << &mu - &muons->at(0) << std::endl;
           std::cout << " indice traccia: " << idx_associatedtrk  << std::endl;
           }
         }
       }

      if (SA_mu.charge() != trk.charge()) {
        std::cout << "Indice traccia " << &trk - &tracks->at(0) <<" Matching non passa per: Opposite charge probe-track" << std::endl;
        continue;
      }
      bool isTrackeronlyseeded = trk.isAlgoInMask(trk.initialStep) || trk.isAlgoInMask(trk.lowPtTripletStep) || trk.isAlgoInMask(trk.pixelPairStep) || trk.isAlgoInMask(trk.detachedTripletStep) ||
      trk.isAlgoInMask(trk.mixedTripletStep) || trk.isAlgoInMask(trk.pixelLessStep) || trk.isAlgoInMask(trk.tobTecStep) || trk.isAlgoInMask(trk.jetCoreRegionalStep) || trk.isAlgoInMask(trk.lowPtQuadStep) || trk.isAlgoInMask(trk.highPtTripletStep) || trk.isAlgoInMask(trk.detachedQuadStep);
      if(!isTrackeronlyseeded && isOnlySeeded_){
        std::cout << "Indice traccia " << &trk - &tracks->at(0) <<" Matching non passa per: No OnlySeeded" << std::endl;
        continue;
      }
      if((trk.pt() <= minpt_trkSA_) && (abs(trk.eta()) <= 1. || trk.p() <= 2.))
	{if((mu.innerTrack()).isNonnull() && (mu.innerTrack()).isAvailable()){
	    std::cout << "Indice traccia " << &trk - &tracks->at(0) <<" Matching non passa per: trkPt,trkP,trkEta : YesinnerTrack" << std::endl;
	  }
	  else{std::cout << "Indice traccia " << &trk - &tracks->at(0) << " Matching non passa per: trkPt,trkP,trkEta : NoinnerTrack" << std::endl;}
	  continue;}
      if(!isAssoc)
	{if((mu.innerTrack()).isNonnull() && (mu.innerTrack()).isAvailable()){
	  std::cout << "Indice traccia " << &trk - &tracks->at(0) << " Matching non passa per: isAssoc : YesinnerTrack" << std::endl;
	  }
	  else{std::cout << "Indice traccia " << &trk - &tracks->at(0) << " Matching non passa per: isAssoc : NoinnerTrack" << std::endl;}
	  continue;}
      if(fabs(SA_mu.eta() - trk.eta()) > 0.3)
	{if((mu.innerTrack()).isNonnull() && (mu.innerTrack()).isAvailable()){
	  std::cout << "Indice traccia " << &trk - &tracks->at(0) << " Matching non passa per: deltaEta : YesinnerTrack" << std::endl;
	  }
	  else{std::cout << "Indice traccia " << &trk - &tracks->at(0) << " Matching non passa per: deltaEta : NoinnerTrack" << std::endl;}
	  continue;}
      if(fabs(SA_mu.pt() - trk.pt()) / trk.pt() > maxpt_relative_dif_trk_SAmu_ && maxpt_relative_dif_trk_SAmu_ > 0)
	{if((mu.innerTrack()).isNonnull() && (mu.innerTrack()).isAvailable()){
	  std::cout << "Indice traccia " << &trk - &tracks->at(0) << " Matching non passa per: ptRel : YesinnerTrack" << std::endl;
	  }
	  else{
	    std::cout << "Indice traccia " << &trk - &tracks->at(0) << " Matching non passa per: ptRel : NoinnerTrack" << std::endl;
	  }
	  continue;
	}
      float DR = deltaR(SA_mu.eta(), SA_mu.phi(), trk.eta(), trk.phi());
      std::cout << "   DR " << DR << "  " << SA_mu.eta() << "  " << SA_mu.phi() << "  " << trk.eta() << "  " << trk.phi() << std::endl;
      
      
      if(minDR < DR){
        std::cout << "Indice traccia " << &trk - &tracks->at(0) <<" Matching non passa per: deltaR > minDR, minDR: "<< minDR << std::endl;
        continue;
      }
      minDR = DR;
      idx_trk = &trk - &tracks->at(0);
      if((mu.innerTrack()).isNonnull() && (mu.innerTrack()).isAvailable()){
	std::cout << "Indice traccia " << &trk - &tracks->at(0) << " Traccia ha passato tutti gli step : YesinnerTrack" << std::endl;
      }
      else{
	std::cout << "Indice traccia " << &trk - &tracks->at(0) << " Traccia ha passato tutti gli step : NoinnerTrack" << std::endl;
      }
    }
   

    if(minDR > maxdr_trk_SAmu_)
      continue;
    trk_SAmuon_map.first.push_back(idx_trk);                          //stora indice traccia
    trk_SAmuon_map.second.push_back(&mu - &muons->at(0));             //stora indice muone
    //std::cout<< "primo step" << std::endl;
      std::cout << " Salvato un matching nella mappa! " << std::endl;
      std::cout << " indice muone probe: " << &mu - &muons->at(0) << std::endl;
      std::cout << " indice traccia: " <<  idx_trk << std::endl;

  }
  
  // cout << " Check Assoc Vec Size " << associatedtrk_muon_map.first.size() << " Track Match Size" 
 // << trk_SAmuon_map.first.size()  << endl;
  using map_type = std::pair<std::vector<unsigned>, std::vector<unsigned>>;

  // Generic functions to map probe track with a second collection of tracks (e.g. dSA, cosmics, dGl, etc)

  // Simple mapping (not currently used) -- just check for the closest match between collections
  // If closest match has dR < config cone value, include match in mapping
  // This is not flexible for later use in spark (see Inclusive function below)
  auto mapTrackCollectionsSimple =
      [&](const std::vector<reco::Track>& coll_1, const std::vector<reco::Track>& coll_2, map_type& coll_map) {
        for (const reco::Track& trk_1 : coll_1) {
          unsigned idx_1 = &trk_1 - &coll_1.at(0);
          float min_dR = 1000;
          unsigned min_idx_2;
          for (const reco::Track& trk_2 : coll_2) {
            unsigned idx_2 = &trk_2 - &coll_2.at(0);
            float dR = deltaR(trk_1.eta(), trk_1.phi(), trk_2.eta(), trk_2.phi());
            if (min_dR < dR)
              continue;
            min_dR = dR;
            min_idx_2 = idx_2;
          }
          if (min_dR > maxdr_trk_dsa_)
            continue;
          coll_map.first.push_back(min_idx_2);
          coll_map.second.push_back(idx_1);
        }
      };
  (void)mapTrackCollectionsSimple;  // prevent "unused method" error

  // Inclusive mapping (currently used for all) -- check and count all possible matches within a config dR cone value
  // Also pick the pair with smallest dR, *even* if it is greater than config dR cone -- this allows to customize match criteria in spark later on
  // Also this should work even for collimated muons where nmatched may be greater than 1
  auto mapTrackCollectionsInclusive = [&]<typename T>(const vector<T>& coll_1,
                                                      const vector<reco::Track>& coll_2,
                                                      map_type& coll_map,
                                                      vector<float>& min_dRs,
                                                      vector<unsigned>& n_matched) {
    // Start with probe at first loop level and dSA/cosmic/dGl at second level instead, to ensure each probe that has a match is actually recorded
    // i.e. a dSA/cosmic/dGl track can be matched to more than one generalTrack probem, but all generalTrack probes that match should be pushed to the map
    for (const T& trk_1 : coll_1) {
      unsigned idx_1 = &trk_1 - &coll_1.at(0);
      unsigned matched = 0;
      float min_dR = 1000;
      unsigned min_idx_2 = 1000;
      for (const reco::Track& trk_2 : coll_2) {
        unsigned idx_2 = &trk_2 - &coll_2.at(0);
        float dR = deltaR(trk_1.eta(), trk_1.phi(), trk_2.eta(), trk_2.phi());
        // first add to count of matched items in coll 2 within X dR cone of track in coll 1
        if (dR < maxdr_trk_dsa_)
          matched++;
        // now check if this is the closest one so far
        if (dR < min_dR) {
          min_dR = dR;
          min_idx_2 = idx_2;
        }
      }
      if (min_idx_2 == 1000)  // corner case: coll 2 is empty
        continue;
      coll_map.first.push_back(idx_1);
      coll_map.second.push_back(min_idx_2);
      min_dRs.push_back(min_dR);
      n_matched.push_back(matched);
    }
  };

  // Map probe and dGl
  map_type probe_dGl_map;
  vector<float> probe_dGl_dRs;
  vector<unsigned> probe_dGl_segmentmatches;
  // Don't use dR matching anymore for dGl, use segment matching below
  // mapTrackCollectionsInclusive(*tracks, *dGlmuons, probe_dGl_map, probe_dGl_dRs, probe_dGl_nmatched);

  // Map probe and cosmics
  map_type probe_cosmic_map;
  vector<float> probe_cosmic_dRs;
  vector<unsigned> probe_cosmic_nmatched;
  mapTrackCollectionsInclusive(*tracks, *staCosmic, probe_cosmic_map, probe_cosmic_dRs, probe_cosmic_nmatched);

  // Map probe and dSA
  map_type probe_dSA_map;
  vector<float> probe_dSA_dRs;
  vector<unsigned> probe_dSA_segmentmatches;
  // Don't use dR matching anymore for dSA, use segment matching below
  // mapTrackCollectionsInclusive(*tracks, *dSAmuons, probe_dSA_map, probe_dSA_dRs, probe_dSA_nmatched);

  // Map tag and dSA
  map_type tag_dSA_map;
  vector<float> tag_dSA_dRs;
  vector<unsigned> tag_dSA_segmentmatches;
  // Don't use dR matching anymore for dSA, use segment matching below
  // mapTrackCollectionsInclusive(*muons, *dSAmuons, tag_dSA_map, tag_dSA_dRs, tag_dSA_nmatched);

  // [Adapted from displaced dimuon analysis]
  // Segment match: consider probe reco::Muons and dSA tracks
  // matched if the segments used to build the dSA are the
  // same as or a subset of segments of the reco::Muons.
  for (const auto& track : *tracks) {
    // can only do segment matching on probe tracks that are matched to muons
    auto it = std::find(trk_muon_map.first.begin(), trk_muon_map.first.end(), &track - &tracks->at(0));
    if (it == trk_muon_map.first.end())
      continue;
    unsigned idx_map = std::distance(trk_muon_map.first.begin(), it);
    unsigned idx_track = trk_muon_map.first[idx_map];
    unsigned idx_muon = trk_muon_map.second[idx_map];
    auto muon = muons->at(idx_muon);
    // also make sure probes are arbitrated tracker muons
    if (!(muon.isTrackerMuon() && muon::isGoodMuon(muon, muon::TrackerMuonArbitrated)))
      continue;

    int max_nmatches = -1;
    float min_dR = +99.f;
    unsigned matched_idx;
    for (const auto& dsa : *dSAmuons) {
      unsigned idx_dsa = &dsa - &dSAmuons->at(0);

      float dR = deltaR(dsa.eta(), dsa.phi(), muon.eta(), muon.phi());
      // Don't waste time with far away dSA tracks and muons
      // Aug. 2021: comment out for now to test dSA-dGl correspondence
      // if (dR > 0.7)
      // continue;

      int nmatches = 0;
      for (auto& hit : dsa.recHits()) {
        if (!hit->isValid())
          continue;
        DetId id = hit->geographicalId();
        if (id.det() != DetId::Muon)
          continue;
        if (id.subdetId() == MuonSubdetId::DT || id.subdetId() == MuonSubdetId::CSC) {
          for (auto& chamber : muon.matches()) {
            if (chamber.id.rawId() != id.rawId())
              continue;
            for (auto& segment : chamber.segmentMatches) {
              if (fabs(segment.x - hit->localPosition().x()) < 1e-6 &&
                  fabs(segment.y - hit->localPosition().y()) < 1e-6) {
                //if (debug_)
                //  std::cout << "matched segment found!!! subdet "
                //            << ((chamber.id.subdetId() == MuonSubdetId::DT) ? "DT" : "CSC")
                //            << " id = " << chamber.id.rawId() << " x = " << segment.x << " y = " << segment.y
                //            << std::endl;
                nmatches++;
                break;
              }
            }
          }
        }
      }
      if (nmatches > max_nmatches) {
        max_nmatches = nmatches;
        min_dR = dR;
        matched_idx = idx_dsa;
      } else if (nmatches == max_nmatches && dR < min_dR) {
        min_dR = dR;
        matched_idx = idx_dsa;
      }
    }
    if (max_nmatches > -1) {
      probe_dSA_map.first.push_back(idx_track);
      probe_dSA_map.second.push_back(matched_idx);
      probe_dSA_dRs.push_back(min_dR);
      probe_dSA_segmentmatches.push_back(max_nmatches);
    }
  }

  // [Adapted from displaced dimuon analysis]
  // Segment match: consider probe reco::Muons and dGl tracks
  // matched if the segments used to build the dGl are the
  // same as or a subset of segments of the reco::Muons.
  for (const auto& track : *tracks) {
    // can only do segment matching on probe tracks that are matched to muons
    auto it = std::find(trk_muon_map.first.begin(), trk_muon_map.first.end(), &track - &tracks->at(0));
    if (it == trk_muon_map.first.end())
      continue;
    unsigned idx_map = std::distance(trk_muon_map.first.begin(), it);
    unsigned idx_track = trk_muon_map.first[idx_map];
    unsigned idx_muon = trk_muon_map.second[idx_map];
    auto muon = muons->at(idx_muon);
    // also make sure probes are arbitrated tracker muons
    if (!(muon.isTrackerMuon() && muon::isGoodMuon(muon, muon::TrackerMuonArbitrated)))
      continue;

    int max_nmatches = -1;
    float min_dR = +99.f;
    unsigned matched_idx;
    for (const auto& dgl : *dGlmuons) {
      unsigned idx_dgl = &dgl - &dGlmuons->at(0);

      float dR = deltaR(dgl.eta(), dgl.phi(), muon.eta(), muon.phi());
      // Don't waste time with far away dGl tracks and muons
      // Aug. 2021: comment out for now to test dSA-dGl correspondence
      // if (dR > 0.7)
      // continue;

      int nmatches = 0;
      for (auto& hit : dgl.recHits()) {
        if (!hit->isValid())
          continue;
        DetId id = hit->geographicalId();
        if (id.det() != DetId::Muon)
          continue;
        if (id.subdetId() == MuonSubdetId::DT || id.subdetId() == MuonSubdetId::CSC) {
          for (auto& chamber : muon.matches()) {
            if (chamber.id.rawId() != id.rawId())
              continue;
            for (auto& segment : chamber.segmentMatches) {
              if (fabs(segment.x - hit->localPosition().x()) < 1e-6 &&
                  fabs(segment.y - hit->localPosition().y()) < 1e-6) {
                //if (debug_)
                //  std::cout << "matched segment found!!! subdet "
                //            << ((chamber.id.subdetId() == MuonSubdetId::DT) ? "DT" : "CSC")
                //            << " id = " << chamber.id.rawId() << " x = " << segment.x << " y = " << segment.y
                //            << std::endl;
                nmatches++;
                break;
              }
            }
          }
        }
      }
      if (nmatches > max_nmatches) {
        max_nmatches = nmatches;
        min_dR = dR;
        matched_idx = idx_dgl;
      } else if (nmatches == max_nmatches && dR < min_dR) {
        min_dR = dR;
        matched_idx = idx_dgl;
      }
    }
    if (max_nmatches > -1) {
      probe_dGl_map.first.push_back(idx_track);
      probe_dGl_map.second.push_back(matched_idx);
      probe_dGl_dRs.push_back(min_dR);
      probe_dGl_segmentmatches.push_back(max_nmatches);
    }
  }
  // [Adapted from displaced dimuon analysis]
  // Segment match: consider tag reco::Muons and dSA tracks
  // matched if the segments used to build the dSA are the
  // same as or a subset of segments of the reco::Muons.
  for (const auto& muon : *muons) {
    unsigned idx_muon = &muon - &muons->at(0);

    int max_nmatches = -1;
    float min_dR = +99.f;
    unsigned matched_idx;
    for (const auto& dsa : *dSAmuons) {
      unsigned idx_dsa = &dsa - &dSAmuons->at(0);

      float dR = deltaR(dsa.eta(), dsa.phi(), muon.eta(), muon.phi());
      // Don't waste time with far away dSA tracks and muons
      // Aug. 2021: comment out for now to test dSA-dGl correspondence
      // if (dR > 0.7)
      // continue;

      int nmatches = 0;
      for (auto& hit : dsa.recHits()) {
        if (!hit->isValid())
          continue;
        DetId id = hit->geographicalId();
        if (id.det() != DetId::Muon)
          continue;
        if (id.subdetId() == MuonSubdetId::DT || id.subdetId() == MuonSubdetId::CSC) {
          for (auto& chamber : muon.matches()) {
            if (chamber.id.rawId() != id.rawId())
              continue;
            for (auto& segment : chamber.segmentMatches) {
              if (fabs(segment.x - hit->localPosition().x()) < 1e-6 &&
                  fabs(segment.y - hit->localPosition().y()) < 1e-6) {
                //if (debug_)
                //  std::cout << "matched segment found!!! subdet "
                //            << ((chamber.id.subdetId() == MuonSubdetId::DT) ? "DT" : "CSC")
                //            << " id = " << chamber.id.rawId() << " x = " << segment.x << " y = " << segment.y
                //            << std::endl;
                nmatches++;
                break;
              }
            }
          }
        }
      }
      if (nmatches > max_nmatches) {
        max_nmatches = nmatches;
        min_dR = dR;
        matched_idx = idx_dsa;
      } else if (nmatches == max_nmatches && dR < min_dR) {
        min_dR = dR;
        matched_idx = idx_dsa;
      }
    }
    if (max_nmatches > -1) {
      tag_dSA_map.first.push_back(idx_muon);
      tag_dSA_map.second.push_back(matched_idx);
      tag_dSA_dRs.push_back(min_dR);
      tag_dSA_segmentmatches.push_back(max_nmatches);
    }
  }

   // Muon collection for jet cleaning
   std::vector<reco::Muon> muForJetCleaning;
   for (const auto& mu : *muons) {
     if (!muon::isLooseMuon(mu))
       continue;
     muForJetCleaning.push_back(mu);
   }
  sort(muForJetCleaning.begin(), muForJetCleaning.end(), [](const auto& l, const auto& r) { return l.pt() > r.pt(); });
  
  std::vector<reco::PFJet> corrJets;
  if (includeJets_) {
    // Get selected jets and fill branches
    for (size_t i = 0; i < jets->size(); ++i) {
      reco::PFJetRef jet(jets, i);
      //if (CrossClean(*jet, muForJetCleaning))
      //  continue;
      std::unique_ptr<reco::PFJet> corrJet(jet->clone());
      double jec = jetCorrector->correction(*jet);
      corrJet->scaleEnergy(jec);
      double smearFactor = 1.0;
      if (isMC_) {
        // Gen Jet Info
        edm::Handle<std::vector<reco::GenJet>> genJets;
        iEvent.getByToken(genJetsToken_, genJets);
        for (const auto& genJet : *genJets) {
          StandAlone_nt.genJets_pt.push_back(genJet.pt());
          StandAlone_nt.genJets_eta.push_back(genJet.eta());
          StandAlone_nt.genJets_phi.push_back(genJet.phi());
          StandAlone_nt.genJets_mass.push_back(genJet.mass());
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
      corrJets.push_back(*corrJet);
      FillJetBranches(*jet, *corrJet, StandAlone_nt, era_);
      if (deepCSVProbb->size() > i && deepCSVProbbb->size() > i) {
        StandAlone_nt.jets_bTag_deepCSV.push_back((*deepCSVProbb)[i].second + (*deepCSVProbbb)[i].second);
      } else
        StandAlone_nt.jets_bTag_deepCSV.push_back(-9999.);
    }
  }

  StandAloneFillEventInfo(nt, StandAlone_nt, HLTPaths_);
  // run over tracks and probes once prior to filling tree to determine ordering of pairs
  // this is necessary to use tag-probe pair with highest "quality" later on in spark_tnp.
  // Also calculate the total number of pairs per event to include in ntuple
  using t_pair_prob = std::pair<float, std::pair<int, int>>;
  std::priority_queue<t_pair_prob> pair_vtx_probs;
  std::priority_queue<t_pair_prob> pair_dPhi_muons;
  std::priority_queue<t_pair_prob, vector<t_pair_prob>, std::greater<t_pair_prob>> pair_dz_PV_SV;    // inverse sort
  std::priority_queue<t_pair_prob, vector<t_pair_prob>, std::greater<t_pair_prob>> pair_dM_Z_Mmumu;  // inverse sort

  std::priority_queue<t_pair_prob> SA_pair_vtx_probs;
  std::priority_queue<t_pair_prob> SA_pair_dPhi_muons;
  std::priority_queue<t_pair_prob, vector<t_pair_prob>, std::greater<t_pair_prob>> SA_pair_dz_PV_SV;    // inverse sort
  std::priority_queue<t_pair_prob, vector<t_pair_prob>, std::greater<t_pair_prob>> SA_pair_dM_Z_Mmumu;  // inverse sort
  // // loop over tags
   for (auto& tag : tag_trkttrk) {
     auto tag_idx = &tag - &tag_trkttrk[0];
     // loop over probes
     for (const reco::Track& probe : *tracks) {
       auto probe_idx = &probe - &tracks->at(0);

       // apply cuts on probe
       if (HighPurity_ && !probe.quality(Track::highPurity))
         continue;
       if (!probeSelection_(probe))
         continue;

       // apply cuts on pairs
       if (tag.first.charge() == probe.charge())
         continue;
       if (fabs(tag.first.vz() - probe.vz()) > pairDz_ && pairDz_ > 0)
         continue;
       float mass = DimuonMass(tag.first.pt(), tag.first.eta(), tag.first.phi(), probe.pt(), probe.eta(), probe.phi());
       if (mass < pairMassMin_ || mass > pairMassMax_)
         continue;

       // compute vtx
       std::vector<reco::TransientTrack> trk_pair = {tag.second, reco::TransientTrack(probe, &(*bField))};
       KlFitter vtx(trk_pair);
       if (RequireVtxCreation_ && !vtx.status())
         continue;
       if (minSVtxProb_ > 0 && vtx.prob() < minSVtxProb_)
         continue;

       float dPhi_muons = reco::deltaPhi(tag.first.phi(), probe.phi());
       math::PtEtaPhiMLorentzVector mu1(tag.first.pt(), tag.first.eta(), tag.first.phi(), MU_MASS);
       math::PtEtaPhiMLorentzVector mu2(probe.pt(), probe.eta(), probe.phi(), MU_MASS);
       float dM_Z_Mmumu = abs(91.2 - (mu1 + mu2).mass());

       // save quantities to ordered heap
       auto pair_idx = std::make_pair(tag_idx, probe_idx);
       pair_vtx_probs.push(std::make_pair(vtx.prob(), pair_idx));
       pair_dz_PV_SV.push(std::make_pair(vtx.dz_PV_SV(nt.pv_z), pair_idx));
       pair_dPhi_muons.push(std::make_pair(dPhi_muons, pair_idx));
       pair_dM_Z_Mmumu.push(std::make_pair(dM_Z_Mmumu, pair_idx));
     }

     for (const reco::Muon& tmp_probe : *muons) {
       auto probe_idx = &tmp_probe - &muons->at(0);
       if (!tmp_probe.isStandAloneMuon())
         continue;
       if (!((*tmp_probe.standAloneMuon()).numberOfValidHits() > 0.))
          continue;
       const reco::Track probe = *tmp_probe.standAloneMuon();
       //if (debug_ > 1)
       //  std::cout << "    Probe pt " << probe.pt() << " eta " << probe.eta() << " phi " << probe.phi() << "  charge "
       //            << probe.charge() << std::endl;

       // apply cuts on probe
       if (!probeSelection_(probe))
         continue;

       // apply cuts on pairs; selected will be saved
       if (tag.first.charge() == probe.charge())
         continue;
       float mass = DimuonMass(tag.first.pt(), tag.first.eta(), tag.first.phi(), probe.pt(), probe.eta(), probe.phi());
       if (mass < pairMassMin_ || mass > pairMassMax_)
         continue;
      
       std::vector<reco::TransientTrack> trk_pair = {tag.second, reco::TransientTrack(probe, &(*bField))};
       KlFitter vtx(trk_pair);

       float dPhi_muons = reco::deltaPhi(tag.first.phi(), probe.phi());
       math::PtEtaPhiMLorentzVector mu1(tag.first.pt(), tag.first.eta(), tag.first.phi(), MU_MASS);
       math::PtEtaPhiMLorentzVector mu2(probe.pt(), probe.eta(), probe.phi(), MU_MASS);
       float dM_Z_Mmumu = abs(91.2 - (mu1 + mu2).mass());

       // save quantities to ordered heap
       auto pair_idx = std::make_pair(tag_idx, probe_idx);
       SA_pair_vtx_probs.push(std::make_pair(vtx.prob(), pair_idx));
       // SA_pair_dz_PV_SV.push(std::make_pair(vtx.dz_PV_SV(StandAlone_nt.pv_z), pair_idx));
       SA_pair_dPhi_muons.push(std::make_pair(dPhi_muons, pair_idx));
       SA_pair_dM_Z_Mmumu.push(std::make_pair(dM_Z_Mmumu, pair_idx));
     }
   }
   //std::cout<< "Sesto step" << std::endl;
   nt.npairs = pair_vtx_probs.size();
   StandAlone_nt.npairs = SA_pair_vtx_probs.size();

   // assign sorted vtx indices to ranking

   //definisce una serie di mappe che non usiamo:


   map<std::pair<int, int>, int> pair_rank_vtx_prob;
   while (!pair_vtx_probs.empty()) {
     pair_rank_vtx_prob[pair_vtx_probs.top().second] = pair_rank_vtx_prob.size();  // careful: RHS evaluated first
     pair_vtx_probs.pop();
   }
   map<std::pair<int, int>, int> pair_rank_dz_PV_SV;
   while (!pair_dz_PV_SV.empty()) {
     pair_rank_dz_PV_SV[pair_dz_PV_SV.top().second] = pair_rank_dz_PV_SV.size();  // careful: RHS evaluated first
     pair_dz_PV_SV.pop();
   }
   map<std::pair<int, int>, int> pair_rank_dPhi_muons;
   while (!pair_dPhi_muons.empty()) {
     pair_rank_dPhi_muons[pair_dPhi_muons.top().second] = pair_rank_dPhi_muons.size();  // careful: RHS evaluated first
     pair_dPhi_muons.pop();
   }
   map<std::pair<int, int>, int> pair_rank_dM_Z_Mmumu;
   while (!pair_dM_Z_Mmumu.empty()) {
     pair_rank_dM_Z_Mmumu[pair_dM_Z_Mmumu.top().second] = pair_rank_dM_Z_Mmumu.size();  // careful: RHS evaluated first
     pair_dM_Z_Mmumu.pop();
   }

   // assign sorted vtx indices to ranking for STANDALONE
   map<std::pair<int, int>, int> SA_pair_rank_vtx_prob;
   while (!SA_pair_vtx_probs.empty()) {
     SA_pair_rank_vtx_prob[SA_pair_vtx_probs.top().second] =
         SA_pair_rank_vtx_prob.size();  // careful: RHS evaluated first
     SA_pair_vtx_probs.pop();
   }
   // map<std::pair<int, int>, int> SA_pair_rank_dz_PV_SV;
   // while (!SA_pair_dz_PV_SV.empty()) {
   // SA_pair_rank_dz_PV_SV[SA_pair_dz_PV_SV.top().second] = SA_pair_rank_dz_PV_SV.size();  // careful: RHS evaluated first
   // SA_pair_dz_PV_SV.pop();
   // }
   map<std::pair<int, int>, int> SA_pair_rank_dPhi_muons;
   while (!SA_pair_dPhi_muons.empty()) {
     SA_pair_rank_dPhi_muons[SA_pair_dPhi_muons.top().second] =
         SA_pair_rank_dPhi_muons.size();  // careful: RHS evaluated first
     SA_pair_dPhi_muons.pop();
   }
   map<std::pair<int, int>, int> SA_pair_rank_dM_Z_Mmumu;
   while (!SA_pair_dM_Z_Mmumu.empty()) {
     SA_pair_rank_dM_Z_Mmumu[SA_pair_dM_Z_Mmumu.top().second] =
         SA_pair_rank_dM_Z_Mmumu.size();  // careful: RHS evaluated first
     SA_pair_dM_Z_Mmumu.pop();
   }

  // Final pair selection now run again to select probes
  // loop over tags

 
   // if (debug_ > 0) {
   //  std::cout << std::endl << "NUOVO EVENTO" << std::endl << std::endl;
   // std::cout << "New Evt " << nt.event << std::endl;
   //  std::cout << "Run " << nt.run << std::endl;
   // std::cout << "Totale dei tag muons: " << tag_trkttrk.size() << std::endl;
   //}  

  for (auto& tag : tag_trkttrk) {

    bool tagHasProbe = false;

    if (debug_ > 0) {
      std::cout << "Sto analizzando il tag di indice " << (&tag - &tag_trkttrk[0]) << std::endl;
      std::cout << "Tag: pt " << tag.first.pt() << " eta " << tag.first.eta() << " phi " << tag.first.phi()
                << std::endl;
    }

    // Loop over stand alone muons
    if (saveStandAloneTree_) {
      for (const reco::Muon& tmp_probe : *muons) {
       
        if (tagHasProbe) 
          break;   //esco dal ciclo sui probes appena riesco ad associare un probe al tag corrente   
         
        std::cout << "Sto analizzando il candidato probe numero " << ( &tmp_probe - &muons->at(0))+1 << std::endl;

        if (!tmp_probe.isStandAloneMuon())
          continue;
        if (!((*tmp_probe.standAloneMuon()).numberOfValidHits() > 0.))
          continue;
        const reco::Track probe = *tmp_probe.standAloneMuon();
        if (debug_ > 0)
          std::cout << "    Probe pt " << probe.pt() << " eta " << probe.eta() << " phi " << probe.phi() << "  charge "
                    << probe.charge() << std::endl;

        // apply cuts on probe
        if (!probeSelectionSA_(probe))
          continue;
        if (muonOnly_ && !probeMuonSelection_(tmp_probe))
           continue;
        // apply cuts on pairs; selected will be saved
        if (tag.first.charge() == probe.charge())
          continue;
        // Add pt condition to take allTracks pT instead of StandAloneMuon
        //bool MatchingSatisfied = false;
        bool TagAndTagPair = false;
        //float probePt = -999.;
        //float probeEta = -999.;
        //float probePhi = -999.;
        //float minDR = 100000;
        ////int idx_trk_changept = -1;

     //   std::cout << "   loop sulle tracce... " <<  std::endl;

      //  for (const reco::Track& trk : *tracks) {
//
      //    if (probe.charge() != trk.charge()) 
      //      continue;
      //    if ((probe.pt() <= minpt_trkSA_) && (abs(trk.eta()) <= 1. || trk.p() <= 2.))
      //      continue;
      //    if (fabs(tag.first.vz() - trk.vz()) > maxdz_trk_mu_ && maxdz_trk_mu_ > 0)
      //      continue;
      //    if (fabs(probe.eta() - trk.eta()) > 0.3)
      //      continue;
      //    if (fabs(probe.pt() - trk.pt()) / probe.pt() > maxpt_relative_dif_trk_mu_ && maxpt_relative_dif_trk_mu_ > 0)
      //      continue;
      //    bool isTrackerOnlyseeded = trk.isAlgoInMask(trk.initialStep) || trk.isAlgoInMask(trk.lowPtTripletStep) || trk.isAlgoInMask(trk.pixelPairStep) || trk.isAlgoInMask(trk.detachedTripletStep) ||
      //    trk.isAlgoInMask(trk.mixedTripletStep) || trk.isAlgoInMask(trk.pixelLessStep) || trk.isAlgoInMask(trk.tobTecStep) || trk.isAlgoInMask(trk.jetCoreRegionalStep) || trk.isAlgoInMask(trk.lowPtQuadStep) || trk.isAlgoInMask(trk.highPtTripletStep) || trk.isAlgoInMask(trk.detachedQuadStep);
      //    if(!isTrackerOnlyseeded && isOnlySeeded_)
      //    continue;
      //    float DR = deltaR(probe.eta(), probe.phi(), trk.eta(), trk.phi());
      //    if (debug_ > 0)
      //      std::cout << "   DR " << DR << "  " << probe.eta() << "  " << probe.phi() << "  " << trk.eta() << "  " << trk.phi()
      //                << std::endl;
      //    if (minDR < DR)
      //      continue;
      //    minDR = DR;
      //    //idx_trk_changept = &trk - &tracks->at(0);  
      //    probePt = probe.pt();
      //    probeEta = probe.eta();
      //    probePhi = probe.phi();
      //    
        //  MatchingSatisfied = true;

     //   }

       //   std::cout << " Fine loop tracce "<< std::endl;

    //    if (minDR > maxdr_trk_mu_ || (!MatchingSatisfied)){
    //        probePt  = probe.pt();
    //        probeEta = probe.eta();
    //        probePhi = probe.phi();
//
    //        std::cout << "  Failing. No matching. Uso pt, eta e phi dello SA"  std::endl;
    //        std::cout << "   probePt: " << probePt << std::endl;
    //        std::cout << "   probeEta: " << probeEta << std::endl;
    //        std::cout << "   probePhi: " << probePhi << std::endl;
    //  
    //    
    //    }
//
    //    else {
//
    //        std::cout << "  Passing. Matching nel DR selezionato. Uso pt, eta e phi della traccia associata" <<  std::endl;
    //        std::cout << "   probePt: " << probePt << std::endl;
    //        std::cout << "   probeEta: " << probeEta << std::endl;
    //        std::cout << "   probePhi: " << probePhi << std::endl;
//
    //    }
        
        
      //  float mass = DimuonMass(tag.first.pt(), tag.first.eta(), tag.first.phi(), probePt, probeEta, probePhi);
        float mass = DimuonMass(tag.first.pt(), tag.first.eta(), tag.first.phi(), probe.pt(), probe.eta(), probe.phi());
       // std::cout << "   Ho appena calcolato la massa: " << mass << std::endl;

       
        if (mass < pairMassMin_ || mass > pairMassMax_)
        { std::cout << "massa al di fuori del range" << std::endl;
          continue;
        }
        
        std::cout << "   Ho appena calcolato la massa: " << mass << std::endl;

       for (auto& tag : tag_trkttrk) {
         if (abs(probe.phi() - tag.first.phi()) < 1e-5 && abs(probe.eta() - tag.first.eta()) < 1e-5 && abs(probe.pt() - tag.first.pt()) < 1e-5)
         TagAndTagPair = true;
       }
       if(TagAndTagPair) {
         std::cout << "   Ho scartato una coppia tag-tag! " << std::endl;
         continue;
    
       }

      math::PtEtaPhiMLorentzVector mu2(probe.pt(), probe.eta(), probe.phi(), MU_MASS);  


      //filling tag and pair standalone ntuple info

        StandAlone_embedTriggerMatching(
            tag.first, nt.trg_filter, nt.trg_pt, nt.trg_eta, nt.trg_phi, StandAlone_nt, tagFilters_, true, debug_);

        StandAlone_nt.tag_isMatchedGen = genmatched_tag[&tag - &tag_trkttrk[0]];

        if(debug_ > 0) {
            std::cout << "Sto salvando nel tree le info del tag di indice: " << &tag - &tag_trkttrk[0] << std::endl;
        }

        StandAloneFillTagBranches<reco::Muon, reco::Track>(tag.first, *tracks, StandAlone_nt, *pv);

     //   if (!(minDR > maxdr_trk_mu_ || (!MatchingSatisfied))){
     //     for (const reco::Track& trk : *tracks) {
     //     if(idx_trk_changept == &trk - &tracks->at(0)){
          
           // if(debug_ > 0) {

          //  std::cout << "Passing. Sto salvando la coppia tag-probe seguente..." << std::endl;
          //  std::cout << "Indice del tag: " << &tag - &tag_trkttrk[0]  << std::endl;
          //  std::cout << "Indice del probe: " << &tmp_probe - &muons->at(0)<< std::endl;
          //  std::cout << "Massa invariante: " << mass << std::endl;
         //   }
     
     // StandAloneFillPairBranches<reco::Muon, reco::Track>(tag.first, tag, StandAlone_nt); 

          
    //      }
    //      }
    //    }
    //    else{
//
        if(debug_ > 0) {

          std::cout << "Sto salvando la coppia tag-probe seguente..." << std::endl;
          std::cout << "Indice del tag: " << &tag - &tag_trkttrk[0]  << std::endl;
          std::cout << "Indice del probe: " << &tmp_probe - &muons->at(0)<< std::endl;
          std::cout << "Massa invariante: " << mass << std::endl;
        }
       StandAloneFillPairBranches<reco::Muon, reco::Track>(tag.first, probe, StandAlone_nt);
   //
    //    }

        auto tagRef = muonsView->refAt(tag_muon_map[&tag - &tag_trkttrk[0]]);
        if (!simInfoIsAvailalbe) {
          StandAloneFillSimMatchingBranchesDummy(StandAlone_nt, true);
        } else {
          const auto& msi = (*simInfo)[tagRef];
          StandAloneFillSimMatchingBranchesAOD(msi, StandAlone_nt, true);
        }

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
	match_tracks.first.clear();
	match_tracks.second.clear();
        if (it != trk_SAmuon_map.second.end()) {           
          unsigned idx = std::distance(trk_SAmuon_map.second.begin(), it);
          match_trk_idx = trk_SAmuon_map.first[idx];
          match_tracks.first.push_back(true);
          match_tracks.second.push_back(tracks->at(match_trk_idx));
          std::cout << "Matching trovato nella mappa" << std::endl;
          std::cout << "Indice del probe: " << match_muon_idx<< std::endl;
          std::cout << "Indice della traccia: " << match_trk_idx << std::endl;
        }
        if (it == trk_SAmuon_map.second.end()) {     // se it punta alla fine del vettore (no matching), si prende la traccia di default
          match_tracks.first.push_back(false);
          match_tracks.second.push_back(tracks->at(0));
          std::cout << "Nessun matching trovato nella mappa" <<std::endl;
        }

        if (assoc_it != associatedtrk_muon_map.second.end()) {
          unsigned assoc_idx = std::distance(associatedtrk_muon_map.second.begin(), assoc_it);
          assoc_trk_idx = associatedtrk_muon_map.first[assoc_idx];
          match_tracks.first.push_back(true);         
          match_tracks.second.push_back(tracks->at(assoc_trk_idx));
          std::cout << "Matching FAKE trovato nella mappa" << std::endl;
          std::cout << "Indice del probe: " << match_muon_idx<< std::endl;
          std::cout << "Indice della traccia: " << assoc_trk_idx << std::endl;

        }
        if (assoc_it == associatedtrk_muon_map.second.end()) {
          match_tracks.first.push_back(false);         
          match_tracks.second.push_back(tracks->at(0));
          std::cout << "Nessun matching FAKE trovato nella mappa" <<std::endl;
        }
        //std::cout<< "Ottavo step" << std::endl;
        
      
        // filling probe standalone ntuple info
        if(debug_ > 0) {
            std::cout << "Sto salvando nel tree le info del probe di indice: " <<  &tmp_probe - &muons->at(0) << std::endl;
        }

        StandAloneFillProbeBranches<reco::Muon, reco::Muon, reco::Track>(
            fakeMuon, *muons, *tracks, StandAlone_nt, match_muon_idx, *pv, match_tracks);

        if (includeJets_)
          FindJetProbePair<reco::PFJet, reco::Muon>(corrJets, fakeMuon, StandAlone_nt);

        if (!simInfoIsAvailalbe or match_muon_idx < 0) {
          StandAloneFillSimMatchingBranchesDummy(StandAlone_nt, false);
        } else {
          auto muRef = muonsView->refAt(match_muon_idx);
          const auto& msi = (*simInfo)[muRef];
          StandAloneFillSimMatchingBranchesAOD(msi, StandAlone_nt, false);
        }

        StandAlone_nt.iprobe++;

            std::cout << "Prima del salvataggio"<< std::endl;
            std::cout << "Massa nel tree:"<< StandAlone_nt.pair_mass << std::endl;
            std::cout << "tag pt nel tree:"<< StandAlone_nt.tag_pt << std::endl;
            std::cout << "probe pt nel tree:"<< StandAlone_nt.probe_pt << std::endl;
        
        StandAlone_t1->Fill();

        if(debug_ > 0) {
            std::cout << "Salvataggio nel tree completato"<< std::endl;
            std::cout << "Massa nel tree:"<< StandAlone_nt.pair_mass << std::endl;
            std::cout << "tag pt nel tree:"<< StandAlone_nt.tag_pt << std::endl;
            std::cout << "probe pt nel tree:"<< StandAlone_nt.probe_pt << std::endl;

        }

        // Imposto la variabile a true per indicare che ho associato un probe al tag
        tagHasProbe = true;
     
      
    } //loop over probes
  } //is save standalone
} //loop tags
}


// ------------ method called once each job just before starting event loop
// ------------
void StandAloneMuonFullAODAnalyzer::beginJob() {
  t1 = fs->make<TTree>("Events", "Events");
  nt.SetTree(t1);
  StandAlone_t1 = fs->make<TTree>("StandAloneEvents", "StandAloneEvents");
  StandAlone_nt.SetTree(StandAlone_t1);
  StandAlone_nt.CreateBranches(HLTPaths_);
  nt.CreateBranches(HLTPaths_, probeSelectorNames_);
  if (!tagFilters_.empty()) {
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
void StandAloneMuonFullAODAnalyzer::endJob() {}

// ------------ method fills 'descriptions' with the allowed parameters for the
// module  ------------
void StandAloneMuonFullAODAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
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
DEFINE_FWK_MODULE(StandAloneMuonFullAODAnalyzer);
