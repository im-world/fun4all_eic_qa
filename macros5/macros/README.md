# fun4all_eic_qa/macros

First you need to source the eic setup script to get your environment and set up your local installation (if you have one). If you use csh/tcsh as your shell, use the .csh scripts, if you have bash use the .sh scripts:

* csh/tcsh:
```
  source /cvmfs/eic.opensciencegrid.org/x8664_sl7/opt/fun4all/core/bin/eic_setup.csh
  source $OPT_FUN4ALL/bin/setup_local.csh <install area>
```


* bash:

```
  source /cvmfs/eic.opensciencegrid.org/x8664_sl7/opt/fun4all/core/bin/eic_setup.sh
  source $OPT_FUN4ALL/bin/setup_local.sh <install area>
```

In rcf accounts typically are set up by their experiment (STAR, PHENIX) and their definitions of e.g. ROOTSYS interferes with ours. Adding a -n flag to the script will reset existing environment variables which interfere with our settings. Also use this if you want to reset your environment. Using -n does not have negative effects if it is not needed - it just resets your environment to the default:
```
  source /cvmfs/eic.opensciencegrid.org/x8664_sl7/opt/fun4all/core/bin/eic_setup.csh -n
```

## Run the macro:
```
  root.exe
  .x Fun4All_G4_EICDetector.C(<number of events>)
```

By default all detectors and evaluators are enabled as is the dumping of the output data into a ROOT TTree (DSTReader). Commenting out the Enable setting will disable a given module. Modules which depend on it will also be disabled. Absorbers (passive volumes) can either be enabled by a setting for each detector or globally by 
```
  Enable::ABSORBER = true;
```

The QA is configured in the macro G4_QA.C it is activated by setting in the Fun4All_G4_EICDetector.C:
```
  Enable::QA = true;
```

The outputs are

  * G4EICDetector_DSTReader.root : DST Reader output (all Hits and Towers), this file gets very large quickly. Comment out the  Enable::DSTREADER = true; to disable it

  * G4EICDetector_g4cemc_eval.root : Central Barrel EMC evaluation

  * G4EICDetector_g4eemc_eval.root : Forward Electron going EMC evaluation

  * G4EICDetector_g4femc_eval.root : Forward hadron going EMC evaluation

  * G4EICDetector_g4fhcal_eval.root : Forward hadron going HCal evaluation

  * G4EICDetector_g4fwdjet_eval.root : Forward jet evaluation

  * G4EICDetector_g4hcalin_eval.root : Central Barrel Inner Hcal evaluation

  * G4EICDetector_g4hcalout_eval.root : Central Barrel Outer Hcal evaluation

  * G4EICDetector_g4jet_eval.root : Central Barrel jet evaluation

  * G4EICDetector_g4tracking_eval.root : Tracking evaluation

  * G4EICDetector_qa.root : QA histograms (from the modules in G4_QA.C)
