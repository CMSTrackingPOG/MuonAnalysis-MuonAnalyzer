'''author: g karathanasis
load the AOD and miniAOD selections
'''
import FWCore.ParameterSet.Config as cms


def muonAnalysis_customizeFullAOD_Z(process):
   process.load("RecoBTag.ImpactParameter.impactParameter_cff")
   process.load("RecoBTag.SecondaryVertex.secondaryVertex_cff")
   process.load("RecoBTag.SoftLepton.softLepton_cff")
   process.load("RecoBTag.Combined.combinedMVA_cff")
   process.load("RecoBTag.CTagging.cTagging_cff")
   process.load("RecoBTag.Combined.deepFlavour_cff")

   process.load("MuonAnalysis.MuonAnalyzer.fullAOD_Z_cff")
   process.muSequence = cms.Sequence(process.fullAODSequence)
   return process
   
def muonAnalysis_customizeStandAloneFullAOD_Z(process):
   process.load("RecoBTag.ImpactParameter.impactParameter_cff")
   process.load("RecoBTag.SecondaryVertex.secondaryVertex_cff")
   process.load("RecoBTag.SoftLepton.softLepton_cff")
   process.load("RecoBTag.Combined.combinedMVA_cff")
   process.load("RecoBTag.CTagging.cTagging_cff")
   process.load("RecoBTag.Combined.deepFlavour_cff")

   #process.load("MuonAnalysis.MuonAnalyzer.StandAlone_fullAOD_Z_cff")
   process.load("MuonAnalysis.MuonAnalyzer.StandAlone_fullAOD_Z_cff")
   process.muSequence = cms.Sequence(process.fullAODSequence)
   return process

def muonAnalysis_customizeFullAOD_JPsi(process):
   process.load("MuonAnalysis.MuonAnalyzer.fullAOD_JPsi_cff")
   process.muSequence = cms.Sequence(process.fullAODSequence)
   return process

def muonAnalysis_customizeMiniAOD_Z(process):
   process.load("MuonAnalysis.MuonAnalyzer.miniAOD_Z_cff")
   process.muSequence = cms.Sequence(process.miniAODSequence)
   return process
   
def muonAnalysis_customizeStandAloneMiniAOD_Z(process):
   process.load("MuonAnalysis.MuonAnalyzer.StandAlone_miniAOD_Z_cff")
   process.muSequence = cms.Sequence(process.miniAODSequence)
   return process

def muonAnalysis_customizeMiniAOD(process):
   process.load("MuonAnalysis.MuonAnalyzer.miniAOD_JPsi_cff")
   process.muSequence = cms.Sequence(process.miniAODSequence)
   return process
