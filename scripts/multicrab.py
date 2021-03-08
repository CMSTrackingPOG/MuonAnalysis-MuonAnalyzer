#!/usr/bin/env python

"""
This is a small script that does the equivalent of multicrab.
"""

import os
import sys
from optparse import OptionParser
import json
import copy
from multiprocessing import Process

from CRABAPI.RawCommand import crabCommand
from CRABClient.ClientExceptions import ClientException
from httplib import HTTPException


def merge_dicts(*dict_args):
    """
    Given any number of dicts, shallow copy and merge into a new dict,
    precedence goes to key value pairs in latter dicts.
    """
    result = {}
    for dictionary in dict_args:
        result.update(dictionary)
    return result

def getOptions():
    """
    Parse and return the arguments provided by the user.
    """
    usage = ("Usage: %prog --crabCmd CMD [--workArea WAD --crabCmdOpts OPTS --sampleType TYPE --era ERA --subEra SUBERA --dataTier DATATIER]"
             "\nThe multicrab command executes 'crab CMD OPTS' for each project directory contained in WAD"
             "\nUse multicrab -h for help")

    parser = OptionParser(usage=usage)

    parser.add_option('-c', '--crabCmd',
                      dest = 'crabCmd',
                      default = '',
                      help = "crab command",
                      metavar = 'CMD')

    parser.add_option('-w', '--workArea',
                      dest = 'workArea',
                      default = 'crab',
                      help = "work area directory. Default: 'crab'.",
                      metavar = 'WAD')

    parser.add_option('-o', '--crabCmdOpts',
                      dest = 'crabCmdOpts',
                      default = '',
                      help = "options for crab command CMD",
                      metavar = 'OPTS')

    parser.add_option('-p', '--particle',
                      dest = 'particle',
                      default = 'muon',
                      help = "Particle to run T&P on. Currently 'muon' is the only supported option.",
                      metavar = 'PART')

    parser.add_option('-r', '--resonance',
                      dest = 'resonance',
                      default = 'Z',
                      help = "Resonance to run T&P with. Options are 'Z' (default) and 'JPsi'.",
                      metavar = 'RESON')

    parser.add_option('-e', '--era',
                      dest = 'era',
                      default = 'Run2018',
                      help = "Era to run samples over. Options are 'Run2018'/'Run2017'/'Run2016'/'Run2018_UL'/'Run2017_UL'/'Run2016_UL'/'Run2016_UL_HIPM'. Default is 'Run2018'.",
                      metavar = 'ERA')

    parser.add_option('-s', '--subEra',
                      dest = 'subEra',
                      default = 'all',
                      help = "Sub-era to process: 'all' (default), custom (e.g. 'Run2016B').",
                      metavar = 'SUBERA')

    parser.add_option('-d', '--dataTier',
                      dest = 'dataTier',
                      default = 'AOD',
                      help = "Data tier: AOD or MINIAOD.",
                      metavar = 'DATATIER')

    parser.add_option('-t', '--sampleType',
                      dest = 'sampleType',
                      default = 'all',
                      help = "Samples to process: 'all' (default), 'data', 'mc'. Only allowed if subEra='all'.",
                      metavar = 'TYPE')

    parser.add_option('-k', '--storageSite',
                      dest = 'storageSite',
                      default = 'CERN',
                      help = "Storage site: 'CERN' (default), 'CERNBOX' (note: requires permission), 'FNAL'.",
                      metavar = 'STORAGE')

    parser.add_option('-n', '--numThreads',
                      dest = 'numThreads',
                      default = 1,
                      help = "Number of CMSSW threads (default 1)",
                      metavar = 'NUMTHRD')

    parser.add_option('-f', '--configFile',
                      dest = 'configFile',
                      default = os.path.join(os.environ['CMSSW_BASE'], 'src/MuonAnalysis/MuonAnalyzer/test/run_muonAnalyzer_cfg.py'),
                      help = "CMSSW cfg file to use (default 'run_muonAnalyzer_cfg.py' in /test).",
                      metavar = 'CFG')

    parser.add_option('-x', '--customSuffix',
                      dest = 'customSuffix',
                      default = '',
                      help = "Custom CRAB name suffix to output dataset tag",
                      metavar = 'SUFFIX')

    parser.add_option('-b', '--eraDB',
                      dest = 'eraDB',
                      default = '',
                      help = "Database file. default: data/samples/PART/RESON/ERA/database.json",
                      metavar = 'ERA_DB_FILE')

    (options, arguments) = parser.parse_args()

    if arguments:
        parser.error("Found positional argument(s): %s." % (arguments))
    if not options.crabCmd:
        parser.error("(-c CMD, --crabCmd=CMD) option not provided.")
    if options.crabCmd != 'submit':
        if not os.path.isdir(options.workArea):
            parser.error("'%s' is not a valid directory." % (options.workArea))
    if options.crabCmd == 'submit' and options.dataTier not in ['AOD', 'MINIAOD']
        parser.error('dataTier must be "AOD" or "MINIAOD"')

    return options


def main():

    # Read options
    options = getOptions()

    doData = options.sampleType in ['all', 'data']
    doMC = options.sampleType in ['all', 'mc']

    particle = options.particle
    resonance = options.resonance
    era = options.era
    subEra = options.subEra
    dataTier = options.dataTier

    customSuffix = options.customSuffix
    numThreads = options.numThreads
    storageSite = options.storageSite

    crabCmd = options.crabCmd
    crabCmdOpts = options.crabCmdOpts
    workArea = options.workArea
    configFile = options.configFile

    # The submit command needs special treatment.
    if crabCmd == 'submit':

        #--------------------------------------------------------
        # This is the base config:
        #--------------------------------------------------------

        from CRABClient.UserUtilities import config, getUsername
        config = config()

        config.General.workArea = workArea
        config.General.transferOutputs = True
        config.General.transferLogs = True

        config.JobType.pluginName = 'Analysis'
        config.JobType.psetName = configFile
        config.JobType.numCores = numThreads
        config.JobType.allowUndistributedCMSSW = True
        #config.JobType.maxMemoryMB = 4000

        #config.Data.splitting = 'Automatic' # Not working after rucio transition
        config.Data.splitting = 'LumiBased'
        config.Data.unitsPerJob = 100
        config.Data.publication = False
        config.Data.allowNonValidInputDataset = True # for validation samples

        if storageSite == 'FNAL':
            # Requires write access to FNAL EOS space
            config.Site.storageSite = 'T3_US_FNALLPC'
            config.Data.outLFNDirBase = '/store/user/%s/TnP_ntuples/%s/%s/%s' % (getUsername(), particle, resonance, era)
        elif storageSite == 'CERN': # default option
            # Requires write access to Muon POG EOS space at CERN
            config.Site.storageSite = 'T2_CH_CERN'
            config.Data.outLFNDirBase = '/store/group/phys_muon/%s/TnP_ntuples/%s/%s/%s' % (getUsername(), particle, resonance, era)
        elif storageSite == 'CERNBOX':
            # CERNBOX write access from CRAB requires special permission from CERN IT
            # See https://twiki.cern.ch/twiki/bin/view/CMSPublic/CRAB3FAQ#Can_I_send_CRAB_output_to_CERNBO
            # and to ask permission: https://cern.service-now.com/service-portal?id=sc_cat_item&name=request-map-dn-to-gridmap&se=CERNBox-Service
            config.Site.storageSite = 'T2_CH_CERNBOX'
            config.Data.outLFNDirBase = '/store/user/%s/TnP_ntuples/%s/%s/%s' % (getUsername(), particle, resonance, era)

        #config.Site.ignoreGlobalBlacklist = True
        #config.Data.ignoreLocality = True
        #config.Site.whitelist = ['T2_US_*']

        #--------------------------------------------------------

        if options.eraDB != '':
            if not os.path.isfile(options.eraDB):
                print 'Error!! database file "{}" does not exist. Please check argument.'.format(options.eraDB)
            else:
                sample_db = options.eraDB
        else:
            sample_db = os.path.join("data/samples", particle, resonance, era, "database.json")

        with open(sample_db, 'r') as db_file:

            db = json.load(db_file)
            suberas = db['suberas'][dataTier]

            samples = {}
            try:
                if subEra == 'all':
                    samples = suberas
                else:
                    samples = dict({subEra: suberas[subEra]})
            except:
                print "Error!! Requested era+sub-era is likely not valid. Please check argument."
                sys.exit()

        for subera_name, subera_cfg in samples.items():
            
            isData = 'Run' in subera_name
            globalTag = subera_cfg['globalTag'] if 'globalTag' in subera_cfg else ''
            input_dataset = subera_cfg['dataset']
            datatier = input_dataset.split('/')[-1]
            if 'AOD' not in datatier or 'NANOAOD' in datatier:
                print 'Input dataset is not AOD(SIM) or MINIAOD(SIM). Ignoring...'
                continue
            isFullAOD = False if 'MINIAOD' in datatier else True

            if isData and not doData: continue
            if not isData and not doMC: continue

            config.Data.lumiMask = ''
            if isData:
                if '2018' in era:
                    config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions18/13TeV/ReReco/Cert_314472-325175_13TeV_17SeptEarlyReReco2018ABC_PromptEraD_Collisions18_JSON.txt'
                elif '2017' in era:
                    config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions17/13TeV/ReReco/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1.txt'
                elif '2016' in era:
                    config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_ReReco_07Aug2017_Collisions16_JSON.txt'

            config.JobType.pyCfgParams = [
                    'resonance={}'.format(resonance),
                    'isFullAOD={}'.format(isFullAOD),
                    'isMC={}'.format(not isData),
                    'globalTag={}'.format(globalTag),
                    'numThreads={}'.format(numThreads),
                    'era={}'.format(era),
                    'includeJets={}'.format(True),
                    'fromCRAB={}'.format(True)
                    ]

            config.Data.inputDataset = input_dataset

            requestName = '_'.join(['TnP_ntuplizer', particle, resonance, era, subera_name])
            config.General.requestName = '_'.join([requestName, customSuffix]) if customSuffix != '' else requestName
            #config.Data.outputDatasetTag = sample (default CRAB dataset tag is 'crab_' + requestName)

            # If we need to pull input files from a list file instead of CRAB:
            # config.Data.userInputFiles = open(basedir + sample + '.list').readlines()

            # Submit.
            def submit(config, options):
                try:
                    print "Submitting for input dataset %s with options %s" % (input_dataset, options.crabCmdOpts)
                    crabCommand(options.crabCmd, config = config, *options.crabCmdOpts.split())
                except HTTPException as hte:
                    print "Submission for input dataset %s failed: %s" % (input_dataset, hte.headers)
                except ClientException as cle:
                    print "Submission for input dataset %s failed: %s" % (input_dataset, cle)

            # Need to submit using multiprocessing module because of CRAB issue with different configs
            p = Process(target=submit, args=(config,options,))
            p.start()
            p.join()

    # All other commands can be simply executed.
    elif workArea:

        for dir in os.listdir(workArea):
            projDir = os.path.join(workArea, dir)
            if not os.path.isdir(projDir):
                continue
            # Execute the crab command.
            msg = "Executing (the equivalent of): crab %s --dir %s %s" % (crabCmd, projDir, crabCmdOpts)
            print "-"*len(msg)
            print msg
            print "-"*len(msg)
            try:
                crabCommand(crabCmd, dir = projDir, *crabCmdOpts.split())
            except HTTPException as hte:
                print "Failed executing command %s for task %s: %s" % (crabCmd, projDir, hte.headers)
            except ClientException as cle:
                print "Failed executing command %s for task %s: %s" % (crabCmd, projDir, cle)

if __name__ == '__main__':
    main()
