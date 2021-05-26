#ifndef MACRO_G4QAEIC_C
#define MACRO_G4QAEIC_C

#include <GlobalVariables.C>

#include <G4_CEmc_EIC.C>
#include <G4_EEMC.C>
#include <G4_FEMC_EIC.C>
#include <G4_FHCAL.C>
#include <G4_HcalIn_ref.C>
#include <G4_HcalOut_ref.C>

#include <eicqa_modules/QAExample.h>
#include <eicqa_modules/QAG4SimulationEicCalorimeter.h>
#include <eicqa_modules/QAG4SimulationEicCalorimeterSum.h>

R__LOAD_LIBRARY(libeicqa_modules.so)

void QAInit()
{
  Fun4AllServer *se = Fun4AllServer::instance();
  if (Enable::CEMC)
  {
    se->registerSubsystem(new QAG4SimulationEicCalorimeter("CEMC"));
  }
  if (Enable::HCALIN)
  {
    se->registerSubsystem(new QAG4SimulationEicCalorimeter("HCALIN"));
  }
  if (Enable::HCALOUT)
  {
    se->registerSubsystem(new QAG4SimulationEicCalorimeter("HCALOUT"));
  }
  if (Enable::FEMC)
  {
    se->registerSubsystem(new QAG4SimulationEicCalorimeter("FEMC", QAG4SimulationEicCalorimeter::kProcessG4Hit));
  }
  if (Enable::FHCAL)
  {
    se->registerSubsystem(new QAG4SimulationEicCalorimeter("FHCAL", QAG4SimulationEicCalorimeter::kProcessG4Hit));
  }
  if (Enable::EEMC)
  {
    se->registerSubsystem(new QAG4SimulationEicCalorimeter("EEMC", QAG4SimulationEicCalorimeter::kProcessG4Hit));
  }
  if (Enable::CEMC && Enable::HCALIN && Enable::HCALOUT)
  {
    // The QA modules also deals with tracking which we do not apply right now
    // so we need to call the module with kProcessCluster
    QAG4SimulationEicCalorimeterSum *calo_qa = nullptr;
    if (Enable::TRACKING)
    {
      calo_qa = new QAG4SimulationEicCalorimeterSum();
      calo_qa->set_track_nodename(TRACKING::TrackNodeName);
    }
    else
    {
      calo_qa = new QAG4SimulationEicCalorimeterSum(QAG4SimulationEicCalorimeterSum::kProcessCluster);
    }
    //    calo_qa->Verbosity(10);
    se->registerSubsystem(calo_qa);
  }
  QAExample *qa = new QAExample();
  //qa->Verbosity(2);
  se->registerSubsystem(qa);
}

#endif  // MACRO_G4QAEIC_C
