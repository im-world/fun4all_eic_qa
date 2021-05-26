#!/usr/local/bin/tcsh
 setenv HOME /eic/u/$LOGNAME
 source /etc/csh.login
 foreach i (/etc/profile.d/*.csh)
   source $i
 end
 source $HOME/.login
 source /cvmfs/eic.opensciencegrid.org/default/opt/fun4all/core/bin/eic_setup.csh -n


set particle = pi-                    #e-, pi-, etc.
set email = me190003061@iiti.ac.in    #email address to notify condor job completion
set events = 10000                    #events per batch (10 batches)

 sed -i "s/pi-/$particle/g" macros/Fun4All_G4_EICDetector.C
 sed -i "s/emailHere/$email/g" macros/condor.job
# sed -i "s/emailHere/$email/g" hadd.job
 sed -i "s/10/$events/g" macros/myscript.csh

cp -r macros macros1
cp -r macros macros2
cp -r macros macros3
cp -r macros macros4
cp -r macros macros5
cp -r macros macros6
cp -r macros macros7
cp -r macros macros8
cp -r macros macros9

cd macros
condor_submit condor.job
cd ../macros1
condor_submit condor.job
cd ../macros2
condor_submit condor.job
cd ../macros3
condor_submit condor.job
cd ../macros4
condor_submit condor.job
cd ../macros5
condor_submit condor.job
cd ../macros6
condor_submit condor.job
cd ../macros7
condor_submit condor.job
cd ../macros8
condor_submit condor.job
cd ../macros9
condor_submit condor.job
cd ../

echo Jobs Submitted
