'''Author: g. karathanasis. georgios.karathanasis@cern.ch
cfg to run tag and probe ntuple for muon POG. It runs both on AOD and miniAOD
Modified by Andre Frankenthal (a.franken@cern.ch) -- September 2020
usage: cmsRun run_muonAnalyzer_cfg.py option1=value1 option2=value2
'''

from FWCore.ParameterSet.VarParsing import VarParsing
import FWCore.ParameterSet.Config as cms

options = VarParsing('python')

options.register('resonance', 'Z',
    VarParsing.multiplicity.singleton,
    VarParsing.varType.string,
    "Set resonance ('Z'/'JPsi')"
)

options.register('isFullAOD', True,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Set to False for MiniAOD datatier"
)

options.register('isMC',False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Set to True for MC"
)

options.register('isOnlySeeded', True,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Set to True for Tracker Only Seeded Tracks"   
)

options.register('globalTag', '',
    VarParsing.multiplicity.singleton,
    VarParsing.varType.string,
    "Set global tag"
)

options.register('reportEvery', 1000,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.int,
    "Report frequency"
)

options.register('numThreads', 1,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.int,
    "Number of CMSSW threads" 
)

# this parameter is added for Jet Branches (ID varies for different era)
options.register('era', 'Run2022',
    VarParsing.multiplicity.singleton,
    VarParsing.varType.string,
    "era"
)

options.register('includeJets', True,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Set to False to exclude jets information in output ntuples"
)

options.register('fromCRAB', False,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "Is config run from CRAB"
)

options.register('isStandAlone', True,
    VarParsing.multiplicity.singleton,
    VarParsing.varType.bool,
    "run StandAlone Muon Analyzers"
)
options.parseArguments()

# defaults

if options._beenSet['globalTag'] and options.globalTag != '':
    globaltag = options.globalTag
else:
    globaltag = '124X_dataRun3_v10' if not options.isMC else '124X_mcRun3_2022_realistic_v12'
    
# Run local test if no input files provided
if len(options.inputFiles) == 0:
    if options.resonance == 'Z':
        if options.isFullAOD:
            if options.isMC:
                options.inputFiles.append('/store/mc/Run3Summer22DRPremix/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/AODSIM/forPOG_124X_mcRun3_2022_realistic_v12-v4/70000/000f361a-3c4d-41b3-bf29-2bba3f76b470.root')
                #options.inputFiles.append('/store/mc/Run3Summer22EEDRPremix/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/AODSIM/forPOG_124X_mcRun3_2022_realistic_postEE_v1-v3/2810000/002cda4f-e18b-4323-981b-9e06c13e0d67.root')
                #options.inputFiles.append('/store/mc/Run3Summer22EEDRPremix/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/AODSIM/forPOG_124X_mcRun3_2022_realistic_postEE_v1-v3/2810000/00322bd6-3a2c-4583-bc4f-5f9a51098644.root')
                #options.inputFiles.append('/store/mc/Run3Winter22DRPremix/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/AODSIM/122X_mcRun3_2021_realistic_v9_ext2-v2/40000/0027e0ac-873f-4f49-884b-4c5b67c85724.root')
                #options.inputFiles.append('/store/mc/Run3Winter22DRPremix/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/AODSIM/122X_mcRun3_2021_realistic_v9_ext2-v2/40000/00683a59-a51d-4599-a671-7eaa225e65c4.root')
                #options.inputFiles.append('/store/mc/Run3Winter22DRPremix/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/AODSIM/122X_mcRun3_2021_realistic_v9_ext2-v2/40000/008c3741-cbee-450c-8a88-f2c90c2c2b57.root')
                #options.inputFiles.append('/store/mc/Run3Winter22DRPremix/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/AODSIM/122X_mcRun3_2021_realistic_v9_ext2-v2/40000/00a1e134-2654-4c4c-8ea5-076988272bef.root')
                #options.inputFiles.append('/store/mc/Run3Winter22DRPremix/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/AODSIM/122X_mcRun3_2021_realistic_v9_ext2-v2/40000/00adb251-7bcf-44b1-a731-bbc699ddb875.root')
                #options.inputFiles.append('/store/mc/Run3Winter22DRPremix/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/AODSIM/122X_mcRun3_2021_realistic_v9_ext2-v2/40000/01716709-aa0a-4ebf-93a4-392e3306aab7.root')
                #options.inputFiles.append('/store/mc/Run3Winter22DRPremix/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/AODSIM/122X_mcRun3_2021_realistic_v9_ext2-v2/40000/0185dc7a-3664-4f35-8f16-dc56b26ba51a.root')
                #options.inputFiles.append('/store/mc/Run3Winter22DRPremix/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/AODSIM/122X_mcRun3_2021_realistic_v9_ext2-v2/40000/01a945bb-4251-4844-b24d-0fd84254b2f8.root')
                #options.inputFiles.append('/store/mc/Run3Winter22DRPremix/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/AODSIM/122X_mcRun3_2021_realistic_v9_ext2-v2/40000/01b38d33-8e1f-499b-b38e-b68fa35fa842.root')
                #options.inputFiles.append('/store/mc/Run3Winter22DRPremix/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/AODSIM/122X_mcRun3_2021_realistic_v9_ext2-v2/40000/0225b747-e093-4f61-a464-51d3d0c9af8e.root')
                #options.inputFiles.append('/store/mc/Run3Winter22DRPremix/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/AODSIM/122X_mcRun3_2021_realistic_v9_ext2-v2/40000/0243215b-f3c2-47ce-b2f4-c5243aec53c9.root')
            else:
                #Run2022A
                #options.inputFiles.append('/store/data/Run2022A/SingleMuon/AOD/PromptReco-v1/000/352/931/00000/37f6901d-d4f8-4fd9-b81f-56b0fa9c3094.root')
                #options.inputFiles.append('/store/data/Run2022A/SingleMuon/AOD/PromptReco-v1/000/353/007/00000/1ac1e94e-67e2-46ee-bf8b-5bf091edd332.root')
                #options.inputFiles.append('/store/data/Run2022A/SingleMuon/AOD/PromptReco-v1/000/353/015/00000/8d3853ac-fcf1-429e-9c8c-85cc705fc262.root')
                #options.inputFiles.append('/store/data/Run2022A/SingleMuon/AOD/PromptReco-v1/000/353/018/00000/e47384a7-b229-41ee-ace6-46baeb95032e.root')
                #options.inputFiles.append('/store/data/Run2022A/SingleMuon/AOD/PromptReco-v1/000/353/021/00000/84f2c525-e88a-45a8-91ff-8eb43b2ec9d3.root')
                #options.inputFiles.append('/store/data/Run2022A/SingleMuon/AOD/PromptReco-v1/000/353/025/00000/84766620-cc21-4377-893c-2db7f7b1ff9c.root')
                #options.inputFiles.append('/store/data/Run2022A/SingleMuon/AOD/PromptReco-v1/000/353/060/00000/107a90fe-ff5d-437c-942b-294d80aec163.root')
                #options.inputFiles.append('/store/data/Run2022A/SingleMuon/AOD/PromptReco-v1/000/353/087/00000/7bf58d01-b804-49e4-bd9d-8f5ae5aa6e2e.root')
                #Run2022B
                #options.inputFiles.append('/store/data/Run2022B/SingleMuon/AOD/PromptReco-v1/000/355/207/00000/2e6d0ca0-4c90-43f1-8df5-78698d81258c.root')
                #options.inputFiles.append('/store/data/Run2022B/SingleMuon/AOD/PromptReco-v1/000/355/207/00000/93738e93-30c1-4a12-ba66-5c0745c8d861.root')
                #options.inputFiles.append('/store/data/Run2022B/SingleMuon/AOD/PromptReco-v1/000/355/380/00000/d7c9a51e-a9b2-41ae-9c99-50928074915a.root')
                #options.inputFiles.append('/store/data/Run2022B/SingleMuon/AOD/PromptReco-v1/000/355/380/00000/dadeb936-1761-418f-b7a4-37c21c32ee77.root')
                #options.inputFiles.append('/store/data/Run2022B/SingleMuon/AOD/PromptReco-v1/000/355/380/00000/dc413b7c-dbc5-4c6c-b17b-920154cbfcf6.root')
                #options.inputFiles.append('/store/data/Run2022B/SingleMuon/AOD/PromptReco-v1/000/355/380/00000/e0c67cee-b03a-4241-ae6d-ea84cc55af80.root')
                #options.inputFiles.append('/store/data/Run2022B/SingleMuon/AOD/PromptReco-v1/000/355/380/00000/e38a1e74-f0c2-412f-b9a4-28a95a3894d2.root')
                #options.inputFiles.append('/store/data/Run2022B/SingleMuon/AOD/PromptReco-v1/000/355/380/00000/e5ec92d3-3e4f-42d8-bb9a-bd216960fa04.root')
                #Run2022C
                #options.inputFiles.append('/store/data/Run2022C/SingleMuon/AOD/PromptReco-v1/000/355/872/00000/34c9b9c0-b01e-43c3-a300-f9a6c3343026.root')
                #options.inputFiles.append('/store/data/Run2022C/SingleMuon/AOD/PromptReco-v1/000/355/872/00000/374837c5-605a-42f3-ade1-bd1123ee2dac.root')
                #options.inputFiles.append('/store/data/Run2022C/SingleMuon/AOD/PromptReco-v1/000/355/872/00000/37b55d7d-b06d-4b2b-8836-4cb9fe882300.root')
                #options.inputFiles.append('/store/data/Run2022C/SingleMuon/AOD/PromptReco-v1/000/355/872/00000/392460cc-27d3-46b2-9e13-2717e81bd44b.root')
                #options.inputFiles.append('/store/data/Run2022C/SingleMuon/AOD/PromptReco-v1/000/355/872/00000/3e36ca67-4dfa-4b71-b280-9890a759af48.root')
                #options.inputFiles.append('/store/data/Run2022C/SingleMuon/AOD/PromptReco-v1/000/355/872/00000/3e74ccdf-0af5-4f97-940d-e02d99f2a381.root')
                #options.inputFiles.append('/store/data/Run2022C/SingleMuon/AOD/PromptReco-v1/000/355/872/00000/3f6330cf-d4f2-4b3d-8eac-2bad067c1a2c.root')
                #options.inputFiles.append('/store/data/Run2022C/SingleMuon/AOD/PromptReco-v1/000/355/872/00000/3f9e0173-a854-4ea1-ae6d-40e195382a7f.root')
                #options.inputFiles.append('/store/data/Run2022C/SingleMuon/AOD/PromptReco-v1/000/355/912/00000/6774548a-e4f3-4702-9ced-1e7d2e01cb64.root')
                #options.inputFiles.append('file:/afs/cern.ch/user/b/bdanzi/TRKPOG_22/CMSSW_12_4_6/src/MuonAnalysis/MuonAnalyzer/test/test.root')
                #options.inputFiles.append('/store/data/Run2022D/Muon/AOD/PromptReco-v2/000/357/898/00000/d3997753-5eb5-4f37-b08d-cefcc6845482.root')
                #options.inputFiles.append('/store/data/Run2022G/Muon/AOD/PromptReco-v1/000/362/433/00000/22fedd32-70ca-4555-8779-9e1ffb10ec61.root')
                #options.inputFiles.append('/store/data/Run2022G/Muon/AOD/PromptReco-v1/000/362/433/00000/445ce53a-4b6c-41cd-af8e-31e74c2b0562.root')
                #options.inputFiles.append('/store/data/Run2022G/Muon/AOD/PromptReco-v1/000/362/433/00000/96cf0f3c-8aa0-45db-b898-a3820a80ecba.root')
                #options.inputFiles.append('/store/data/Run2022G/Muon/AOD/PromptReco-v1/000/362/437/00000/3aea6403-c06c-4064-9d7a-4e2e57dbd020.root')
                options.inputFiles.append('/store/data/Run2022B/SingleMuon/AOD/27Jun2023-v1/2820000/92293570-106e-4c78-8029-c3853b8f2537.root')
        else:
            if options.isMC:
                #options.inputFiles.append('/store/mc/Run3Winter22MiniAOD/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/MINIAODSIM/122X_mcRun3_2021_realistic_v9_ext2-v2/40000/003d71c2-23aa-40cb-877a-2e6b5ac9befc.root')
                #options.inputFiles.append('/store/mc/Run3Winter22MiniAOD/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/MINIAODSIM/122X_mcRun3_2021_realistic_v9_ext2-v2/40000/004474e9-f11c-43c6-95ce-5db29a43cbaf.root')
                #options.inputFiles.append('/store/mc/Run3Winter22MiniAOD/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/MINIAODSIM/122X_mcRun3_2021_realistic_v9_ext2-v2/40000/004dd361-f49c-43fe-8344-f18aa036e286.root')
                #options.inputFiles.append('/store/mc/Run3Winter22MiniAOD/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/MINIAODSIM/122X_mcRun3_2021_realistic_v9_ext2-v2/40000/01a40e5c-4f9b-44fd-a7b2-6a67b7c628ee.root')
                #options.inputFiles.append('/store/mc/Run3Winter22MiniAOD/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/MINIAODSIM/122X_mcRun3_2021_realistic_v9_ext2-v2/40000/01a6b2f2-4769-4776-99ab-d5a6d32e574e.root')
                #options.inputFiles.append('/store/mc/Run3Winter22MiniAOD/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/MINIAODSIM/122X_mcRun3_2021_realistic_v9_ext2-v2/40000/01aba5a9-72f3-47a1-95da-54a1138e5f79.root')
                #options.inputFiles.append('/store/mc/Run3Winter22MiniAOD/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/MINIAODSIM/122X_mcRun3_2021_realistic_v9_ext2-v2/40000/01c33217-d883-4e3c-b86a-2e0060ec99e4.root')
                #options.inputFiles.append('/store/mc/Run3Winter22MiniAOD/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/MINIAODSIM/122X_mcRun3_2021_realistic_v9_ext2-v2/40000/01f6897a-7625-48f7-b36e-51680080d10c.root')
                #options.inputFiles.append('/store/mc/Run3Winter22MiniAOD/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/MINIAODSIM/122X_mcRun3_2021_realistic_v9_ext2-v2/40000/02882de0-cdfe-4718-bf7e-f2f64d4df296.root')
                #options.inputFiles.append('/store/mc/Run3Winter22MiniAOD/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/MINIAODSIM/122X_mcRun3_2021_realistic_v9_ext2-v2/40000/02ef7e63-e155-493c-960e-49fe67e4ba86.root')
                options.inputFiles.append('/store/mc/Run3Winter22MiniAOD/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/MINIAODSIM/122X_mcRun3_2021_realistic_v9_ext2-v2/40001/cfbcbccf-eb97-4883-90e7-f537a1fb9f28.root')
                options.inputFiles.append('/store/mc/Run3Winter22MiniAOD/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/MINIAODSIM/122X_mcRun3_2021_realistic_v9_ext2-v2/40001/d8ce1e4d-c3e1-434e-bebd-e2b858362091.root')
                options.inputFiles.append('/store/mc/Run3Winter22MiniAOD/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/MINIAODSIM/122X_mcRun3_2021_realistic_v9_ext2-v2/40001/d9a143a3-2895-4280-923e-b5dfaa294060.root')
                options.inputFiles.append('/store/mc/Run3Winter22MiniAOD/DYJetsToLL_M-50_TuneCP5_13p6TeV-madgraphMLM-pythia8/MINIAODSIM/122X_mcRun3_2021_realistic_v9_ext2-v2/40001/dc0c5c54-7adb-4c65-9cb2-1faad572b35d.root')

            else:
                # Run2022A
                #options.inputFiles.append('/store/data/Run2022A/SingleMuon/MINIAOD/PromptReco-v1/000/352/931/00000/1e65080b-10b6-44dd-aaae-4d29d40b2edd.root')
                #options.inputFiles.append('/store/data/Run2022A/SingleMuon/MINIAOD/PromptReco-v1/000/353/007/00000/c2560d38-2946-4a5f-ab99-e39375164f3e.root')
                #options.inputFiles.append('/store/data/Run2022A/SingleMuon/MINIAOD/PromptReco-v1/000/353/015/00000/50e04cdf-b7b1-48e8-bae3-ac9742a517f5.root')
                #options.inputFiles.append('/store/data/Run2022A/SingleMuon/MINIAOD/PromptReco-v1/000/353/018/00000/2a508c27-f684-4e5c-82d2-0a22711d7c85.root')
                #options.inputFiles.append('/store/data/Run2022A/SingleMuon/MINIAOD/PromptReco-v1/000/353/021/00000/96e77d23-3ef8-4fb7-b2fe-005e0aa026f8.root')
                #options.inputFiles.append('/store/data/Run2022A/SingleMuon/MINIAOD/PromptReco-v1/000/353/025/00000/b9e83d8b-cb2e-4a45-b57e-c41e1acb94f6.root')
                #options.inputFiles.append('/store/data/Run2022A/SingleMuon/MINIAOD/PromptReco-v1/000/353/060/00000/50a674f4-1829-4412-bb67-434573ff709b.root')
                #options.inputFiles.append('/store/data/Run2022A/SingleMuon/MINIAOD/PromptReco-v1/000/353/087/00000/c1720ddd-e145-48a0-9aa1-9e66b0f166a8.root')
                # Run2022B
                #options.inputFiles.append('/store/data/Run2022B/SingleMuon/MINIAOD/PromptReco-v1/000/355/094/00000/947e7c57-8b06-4627-bd92-e5ee0473618d.root')
                #options.inputFiles.append('/store/data/Run2022B/SingleMuon/MINIAOD/PromptReco-v1/000/355/100/00000/dd6f0295-94f9-4fc8-8a74-ed6352f9ff8b.root')
                #options.inputFiles.append('/store/data/Run2022B/SingleMuon/MINIAOD/PromptReco-v1/000/355/380/00000/3761372d-2608-49d6-8dbd-188bf25ec797.root')
                #options.inputFiles.append('/store/data/Run2022B/SingleMuon/MINIAOD/PromptReco-v1/000/355/380/00000/49ee7473-5cef-4a26-8cb3-d4faeaa08e79.root')
                #options.inputFiles.append('/store/data/Run2022B/SingleMuon/MINIAOD/PromptReco-v1/000/355/380/00000/4e2f510f-0657-4556-bb33-c657a77c62c6.root')
                #options.inputFiles.append('/store/data/Run2022B/SingleMuon/MINIAOD/PromptReco-v1/000/355/380/00000/59df275c-6a8b-4e6b-9058-2bf663b2b46c.root')
                #options.inputFiles.append('/store/data/Run2022B/SingleMuon/MINIAOD/PromptReco-v1/000/355/380/00000/65095b18-3c35-436f-b9a1-28a36385c279.root')
                #options.inputFiles.append('/store/data/Run2022B/SingleMuon/MINIAOD/PromptReco-v1/000/355/380/00000/72a7b989-5f09-4ab7-9a5a-223349fb6ab3.root')
                #options.inputFiles.append('/store/data/Run2022B/SingleMuon/MINIAOD/PromptReco-v1/000/355/380/00000/78717263-68a9-45c7-aada-129ededc9172.root')
                #options.inputFiles.append('/store/data/Run2022B/SingleMuon/MINIAOD/PromptReco-v1/000/355/380/00000/7bc4bff7-3848-41ea-bda2-8d536aeed688.root')
                #options.inputFiles.append('/store/data/Run2022B/SingleMuon/MINIAOD/PromptReco-v1/000/355/380/00000/8cb485c5-4e3f-4131-a032-195163992486.root')
                #options.inputFiles.append('/store/data/Run2022B/SingleMuon/MINIAOD/PromptReco-v1/000/355/380/00000/97f5864b-8a31-4f9b-9c86-e8e175dd2a91.root')
                # Run2022C
                options.inputFiles.append('/store/data/Run2022C/SingleMuon/MINIAOD/PromptReco-v1/000/356/383/00000/8014e371-898b-470c-8adc-91d9dc37acfe.root')
                options.inputFiles.append('/store/data/Run2022C/SingleMuon/MINIAOD/PromptReco-v1/000/356/384/00000/84eb615d-43ff-4d46-95de-b0d146147560.root')
                options.inputFiles.append('/store/data/Run2022C/SingleMuon/MINIAOD/PromptReco-v1/000/356/385/00000/6386c4ca-c997-4bde-bae7-9af8a4a508b2.root')
                options.inputFiles.append('/store/data/Run2022C/SingleMuon/MINIAOD/PromptReco-v1/000/356/385/00000/7298deee-c715-4ed3-bc2a-167781df4767.root')
                options.inputFiles.append('/store/data/Run2022C/SingleMuon/MINIAOD/PromptReco-v1/000/356/386/00000/4624d631-ed65-49b4-94d8-5d38a4ec2c56.root')
                options.inputFiles.append('/store/data/Run2022C/SingleMuon/MINIAOD/PromptReco-v1/000/356/386/00000/5496721b-125b-466d-8733-5fa9615994a0.root')
                options.inputFiles.append('/store/data/Run2022C/SingleMuon/MINIAOD/PromptReco-v1/000/356/386/00000/65d688cd-0a80-45fb-bf5a-76860992d44b.root')
                options.inputFiles.append('/store/data/Run2022C/SingleMuon/MINIAOD/PromptReco-v1/000/356/386/00000/6810114b-3345-4c34-9f01-479e6b06f440.root')
                options.inputFiles.append('/store/data/Run2022C/SingleMuon/MINIAOD/PromptReco-v1/000/356/386/00000/6ae34aaf-5d99-4010-9c66-7f22f0b6fe5b.root')
    elif options.resonance == 'JPsi':
        if options.isFullAOD:
            if options.isMC:
                options.inputFiles.append('/store/mc/RunIIAutumn18DRPremix/JpsiToMuMu_JpsiPt8_TuneCP5_13TeV-pythia8/AODSIM/102X_upgrade2018_realistic_v15-v1/270001/FFF2FC1D-18CB-7244-9663-4E36963494B7.root')
            else:
                options.inputFiles.append('/store/data/Run2018A/Charmonium/AOD/17Sep2018-v1/100001/07679496-4DEF-1B44-BA04-768765A80599.root')


if options.outputFile=="":
    options.outputFile="output"
    if options.isMC:
        options.outputFile+="_mc"
    else:
        options.outputFile+="_data" 
    if options.isFullAOD:
        options.outputFile+="_full"
    else:
        options.outputFile+="_mini"
    options.outputFile+=".root"


process = cms.Process("MuonAnalysis")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load('Configuration.StandardSequences.EndOfProcess_cff')

process.MessageLogger.cerr.FwkReport.reportEvery = options.reportEvery

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag,globaltag, '')

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000))

process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring(options.inputFiles),
        secondaryFileNames=cms.untracked.vstring(),
        inputCommands=cms.untracked.vstring(
            'keep *',
            'drop *_ctppsPixelClusters_*_*'
        )
)

if options.includeJets:
    # for b-tagging
    process.load("RecoBTag.ImpactParameter.impactParameter_cff")
    process.load("RecoBTag.SecondaryVertex.secondaryVertex_cff")
    process.load("RecoBTag.SoftLepton.softLepton_cff")
    process.load("RecoBTag.Combined.combinedMVA_cff")
    process.load("RecoBTag.CTagging.cTagging_cff")
    process.load("RecoBTag.Combined.deepFlavour_cff")
    process.load("JetMETCorrections.Configuration.JetCorrectors_cff")

# Include pat:packedCandidateCollection in AOD for miniPFIsolation
if options.isFullAOD:   
    process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")
    process.load("CommonTools.RecoAlgos.primaryVertexAssociation_cfi")
    process.load("PhysicsTools.PatAlgos.slimming.offlineSlimmedPrimaryVertices_cfi")
    process.load("PhysicsTools.PatAlgos.slimming.packedPFCandidates_cfi")
    from PhysicsTools.PatAlgos.slimming.packedPFCandidates_cfi import packedPFCandidates
    process.packedCandsForMuons = packedPFCandidates.clone()
    process.packedCandsForMuons.PuppiSrc=cms.InputTag("")
    process.packedCandsForMuons.PuppiNoLepSrc=cms.InputTag("")

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
    numberOfThreads = cms.untracked.uint32(options.numThreads)
)

from MuonAnalysis.MuonAnalyzer.tools.ntuple_tools import *
if options.isStandAlone:
    if options.isFullAOD:
        if options.resonance == 'Z':
            process = muonAnalysis_customizeStandAloneFullAOD_Z(process)
        else:
            process = muonAnalysis_customizeFullAOD_JPsi(process) # No JPsi Standalone config set yet
        if not options.isMC:
            process.muon.jetCorrector = cms.InputTag(
                "ak4PFCHSL1FastL2L3ResidualCorrector")
    else:
        if options.resonance == 'Z':
            process = muonAnalysis_customizeStandAloneMiniAOD_Z(process)
        else:
            process = muonAnalysis_customizeMiniAOD(process)  # No JPsi Standalone config set yet

else:
    if options.isFullAOD:
        if options.resonance == 'Z':
            process = muonAnalysis_customizeFullAOD_Z(process)
        else:
            process = muonAnalysis_customizeFullAOD_JPsi(process)
        if not options.isMC:
            process.muon.jetCorrector = cms.InputTag(
                "ak4PFCHSL1FastL2L3ResidualCorrector")
    else:
        if options.resonance == 'Z':
            process = muonAnalysis_customizeMiniAOD_Z(process)
        else:
            process = muonAnalysis_customizeMiniAOD(process)

process.muon.isMC = options.isMC
process.muon.includeJets = options.includeJets
process.muon.era = options.era
process.muon.isOnlySeeded=options.isOnlySeeded

# Trigger matching
muonSrc = "muons" if options.isFullAOD else "slimmedMuons"
from MuonAnalysis.MuonAssociators.muonL1Match_cfi import muonL1Match as _muonL1Match
process.muonL1Info = _muonL1Match.clone(
    src = cms.InputTag(muonSrc),
    useMB2InOverlap = cms.bool(True),
    useStage2L1 = cms.bool(True),
    preselection = cms.string(""),
    matched = cms.InputTag("gmtStage2Digis:Muon:"),
    
    useStation2 = cms.bool(True),
    cosmicPropagationHypothesis = cms.bool(False),
    propagatorAlong = cms.ESInputTag("", "SteppingHelixPropagatorAlong"),
    propagatorAny = cms.ESInputTag("", "SteppingHelixPropagatorAny"),
    propagatorOpposite = cms.ESInputTag("", "SteppingHelixPropagatorOpposite"),
    fallbackToME1 = cms.bool(False)
)
process.muonL1InfoByQ = process.muonL1Info.clone(
    sortBy = cms.string("quality"),
    sortByQuality  = cms.bool(True),
    sortByDeltaPhi = cms.bool(False),
    sortByDeltaEta = cms.bool(False),
    sortByPt       = cms.bool(False)
)

process.muon.fallbackToME1 = cms.bool(False)
process.muon.cosmicPropagationHypothesis = cms.bool(False)
process.muon.useMB2InOverlap = cms.bool(True)
process.muon.propagatorAlong = cms.ESInputTag("", "SteppingHelixPropagatorAlong")
process.muon.propagatorAny = cms.ESInputTag("", "SteppingHelixPropagatorAny")
process.muon.propagatorOpposite = cms.ESInputTag("", "SteppingHelixPropagatorOpposite")

from MuonAnalysis.MuonAnalyzer.hltInfo_cff import getHLTInfo, selectTriggers
hltInfo = getHLTInfo(options.resonance, options.era)
#excludeDSA = (not options.isFullAOD)
excludeDSA = True
process.muon.triggerPaths = cms.vstring(selectTriggers(hltInfo['triggerPaths'], True, False, excludeDSA))
process.muon.tagFilters = cms.vstring(selectTriggers(hltInfo['tagFilters'], not options.isFullAOD, True, excludeDSA))
process.muon.probeFilters = cms.vstring(selectTriggers(hltInfo['probeFilters'], not options.isFullAOD, True, excludeDSA))
#process.muon.tagFilters = cms.vstring(selectTriggers(hltInfo['tagFilters'], True, True, excludeDSA))
#process.muon.probeFilters = cms.vstring(selectTriggers(hltInfo['probeFilters'], True, True, excludeDSA))

# Standard selectors
from MuonAnalysis.MuonAnalyzer.selectorInfo_cff import getSelectorNamesAndBits
selectorNames, selectorBits = getSelectorNamesAndBits(options.era, options.isFullAOD)
process.muon.probeSelectorNames = cms.vstring(selectorNames)
process.muon.probeSelectorBits = cms.vuint32(selectorBits)
#if not options.isMC:
#    process.LumiInfo = cms.EDProducer('LumiProducerFromBrilcalc',
#                                      lumiFile = cms.string("lumiData.csv"),
#                                      throwIfNotFound = cms.bool(False),
#                                      doBunchByBunch = cms.bool(False)
#                                      )
#else:
#    process.LumiInfo = cms.EDProducer('LumiProducerFromBrilcalc',
#                                      lumiFile = cms.string("lumiMC.csv"),
#                                      throwIfNotFound = cms.bool(False),
#                                      doBunchByBunch = cms.bool(False)
#                                      )
if options.isFullAOD:
    if options.includeJets:
        if not options.isMC:
	        process.analysis_step = cms.Path(
#                process.LumiInfo +
                process.primaryVertexAssociation +
                process.offlineSlimmedPrimaryVertices +
                process.packedCandsForMuons +
                process.muonL1Info +
                process.muonL1InfoByQ +
                process.ak4PFCHSL1FastL2L3ResidualCorrectorChain +
                process.muSequence
            )
        else:
            process.analysis_step = cms.Path(
#                process.LumiInfo +
                process.primaryVertexAssociation +
                process.offlineSlimmedPrimaryVertices +
                process.packedCandsForMuons +
                process.muonL1Info +
                process.muonL1InfoByQ +
                process.ak4PFCHSL1FastL2L3CorrectorChain +
                process.muSequence
	    )
    else:
        process.analysis_step = cms.Path(
#            process.LumiInfo +
            process.primaryVertexAssociation +
            process.offlineSlimmedPrimaryVertices +
            process.packedCandsForMuons +
            process.muonL1Info +
	        process.muonL1InfoByQ +
            process.muSequence
        )
else:
    if options.includeJets:
        if not options.isMC:
            process.analysis_step = cms.Path(
#                process.LumiInfo+
                process.muonL1Info +
                process.muonL1InfoByQ +
                process.ak4PFCHSL1FastL2L3ResidualCorrectorChain +
                process.muSequence
            )
        else:
            process.analysis_step = cms.Path(
 #               process.LumiInfo+
                process.muonL1Info +
                process.muonL1InfoByQ +
                process.ak4PFCHSL1FastL2L3CorrectorChain +
                process.muSequence
            )
    else:
        process.analysis_step = cms.Path(
#            process.LumiInfo+
            process.muonL1Info +
            process.muonL1InfoByQ +
            process.muSequence
        )



process.TFileService = cms.Service("TFileService",
        fileName = cms.string(options.outputFile)
)
process.endjob_step = cms.EndPath(process.endOfProcess)

# process.fevt = cms.OutputModule("PoolOutputModule",
#     outputCommands = cms.untracked.vstring(),
#     fileName = cms.untracked.string("edm_output.root")
# )
process.schedule = cms.Schedule(process.analysis_step, process.endjob_step)

from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
