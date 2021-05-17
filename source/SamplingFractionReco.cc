
#include "SamplingFractionReco.h"


#include <g4main/PHG4Hit.h>
#include <g4main/PHG4HitContainer.h>
#include <g4main/PHG4Particle.h>
#include <g4main/PHG4TruthInfoContainer.h>

#include <fun4all/Fun4AllReturnCodes.h>

#include <phool/PHCompositeNode.h>
#include <phool/getClass.h>

#include <TFile.h>
#include <TNtuple.h>
#include <TSystem.h>

#include <iostream>  // for operator<<, endl, basic_ost...

//____________________________________________________________________________..
SamplingFractionReco::SamplingFractionReco(const std::string &name, const std::string &filename)
  : SubsysReco(name)
  , outfilename(filename)
{
}

//____________________________________________________________________________..
SamplingFractionReco::~SamplingFractionReco()
{
}

//____________________________________________________________________________..
int SamplingFractionReco::Init(PHCompositeNode *topNode)
{
  if (m_Detector.empty())
  {
    std::cout << "Detector not set via Detector(<name>) method" << std::endl;
    std::cout << "(it is the name appended to the G4HIT_<name> nodename)" << std::endl;
    std::cout << "you do not want to run like this, exiting now" << std::endl;
    gSystem->Exit(1);
  }
  outfile = new TFile(outfilename.c_str(), "RECREATE");
  std::string title = "Sampling Fraction " + m_Detector;
  ntup = new TNtuple("sfntup", title.c_str(), "theta:phi:eta:p:escin:eabs:eion:light:esum");
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int SamplingFractionReco::InitRun(PHCompositeNode *topNode)
{
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int SamplingFractionReco::process_event(PHCompositeNode *topNode)
{
  PHG4TruthInfoContainer *truthinfo = findNode::getClass<PHG4TruthInfoContainer>(topNode, "G4TruthInfo");
  double phi = NAN;
  double eta = NAN;
  double theta = NAN;
  double mom = NAN;
  if (truthinfo)
  {
    PHG4TruthInfoContainer::ConstRange range = truthinfo->GetPrimaryParticleRange();
    int justone = 0;

    for (PHG4TruthInfoContainer::ConstIterator iter = range.first;
	 iter != range.second;
	 ++iter)
    {
      justone++;
      PHG4Particle *primary = iter->second;
      double gpx = primary->get_px();
      double gpy = primary->get_py();
      phi = atan2(gpy, gpx)*180./M_PI;
      double gpz = primary->get_pz();
      double gpt = std::sqrt(gpx * gpx + gpy * gpy);
      mom = std::sqrt(gpx * gpx + gpy * gpy + gpz * gpz);
      if (gpt > 0)
      {
	eta = asinh(gpz / gpt);
      }
    }
    if (justone > 1)
    {
      std::cout << "this only works for single particle events"
		<< " here I see " << justone << " primaries" << std::endl;
      gSystem->Exit(1);
    }
  }
  // add hits
  PHG4HitContainer *g4hits = findNode::getClass<PHG4HitContainer>(topNode, m_HitNodeName);

  double escin = 0;
  double eabs = 0.;
  double eion = 0.;
  double light = 0.;
  double esum = 0.;
  if (g4hits)
  {
    PHG4HitContainer::ConstRange hit_range = g4hits->getHits();
    for (PHG4HitContainer::ConstIterator hit_iter = hit_range.first; hit_iter != hit_range.second; hit_iter++)
    {
      escin += hit_iter->second->get_edep();
      eion += hit_iter->second->get_eion();
      light += hit_iter->second->get_light_yield();
    }
  }
  else
  {
    std::cout << "could not find " << m_HitNodeName << std::endl;
  }
  g4hits = findNode::getClass<PHG4HitContainer>(topNode, m_AbsorberNodeName);
  if (g4hits)
  {
    PHG4HitContainer::ConstRange hit_range = g4hits->getHits();
    for (PHG4HitContainer::ConstIterator hit_iter = hit_range.first; hit_iter != hit_range.second; hit_iter++)
    {
      eabs += hit_iter->second->get_edep();
    }
  }
  else
  {
    std::cout << "could not find " << m_AbsorberNodeName << std::endl;
  }
  esum = escin + eabs;
  ntup->Fill(theta,phi,eta,mom,escin,eabs,eion,light,esum);
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int SamplingFractionReco::ResetEvent(PHCompositeNode *topNode)
{
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int SamplingFractionReco::EndRun(const int runnumber)
{
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int SamplingFractionReco::End(PHCompositeNode *topNode)
{
  outfile->cd();
  ntup->Write();
  outfile->Write();
  outfile->Close();
  delete outfile;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int SamplingFractionReco::Reset(PHCompositeNode *topNode)
{
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
void SamplingFractionReco::Print(const std::string &what) const
{
  std::cout << "SamplingFractionReco::Print(const std::string &what) const Printing info for " << what << std::endl;
}

void SamplingFractionReco::Detector(const std::string &name)
{
  m_Detector = name;
  m_HitNodeName = "G4HIT_" + name;
  m_AbsorberNodeName = "G4HIT_ABSORBER_" + name;
}
