#!/usr/local/bin/tcsh
 setenv HOME /eic/u/$LOGNAME
 source /etc/csh.login
 foreach i (/etc/profile.d/*.csh)
   source $i
 end
 source $HOME/.login
 source /cvmfs/eic.opensciencegrid.org/default/opt/fun4all/core/bin/eic_setup.csh -n
  
 printenv
  # this is how you run your Fun4All_G4_sPHENIX.C macro in batch: 
 root.exe -q -b Fun4All_G4_EICDetector.C\(nEvents\)

 root.exe -b RunEval.C\(\"EEMC\",\"G4EICDetector.root\"\)
 root.exe -b RunEval.C\(\"CEMC\",\"G4EICDetector.root\"\)
 root.exe -b RunEval.C\(\"FEMC\",\"G4EICDetector.root\"\)
 root.exe -b RunEval.C\(\"HCALIN\",\"G4EICDetector.root\"\)
 root.exe -b RunEval.C\(\"HCALOUT\",\"G4EICDetector.root\"\)
 root.exe -q -b RunEval.C\(\"FHCAL\",\"G4EICDetector.root\"\)

echo condorjob done
