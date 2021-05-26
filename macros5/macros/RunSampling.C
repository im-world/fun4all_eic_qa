#include <eicqa_modules/SamplingFractionReco.h>

#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllOutputManager.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libeicqa_modules.so)

void RunSampling(const std::string &detector, const std::string &fname, const int nevnt = 0, const std::string &outdir = ".")
{
  gSystem->Load("libg4dst");
  std::string outfile = outdir + "/SF_" + detector + ".root";
  Fun4AllServer *se = Fun4AllServer::instance();
  
  SamplingFractionReco *sf = new SamplingFractionReco("SF",outfile);
  sf->Detector(detector);
  se->registerSubsystem(sf);
  Fun4AllInputManager *in = new Fun4AllDstInputManager("QAin");
  in->fileopen(fname);
  se->registerInputManager(in);
  if (nevnt < 0)
  {
    return;
  }
  se->run(nevnt);
  se->End();
  delete se;
  gSystem->Exit(0);
}
