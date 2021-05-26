#include <eicqa_modules/EvalRootTTree.h>
#include <eicqa_modules/EvalHit.h>

R__LOAD_LIBRARY(libeicqa_modules.so)

void LoopEval(const std::string &filename)
{
  TFile *f1 = new TFile(filename.c_str(),"READ"); //change the input evaluator root file here
  
  TTree* T = (TTree*)f1->Get("T");
  EvalRootTTree *evaltree = nullptr;
  
 
  T->SetBranchAddress("DST#EvalTTree_HCALOUT",&evaltree);
  for(int i=0; i<T->GetEntries(); i++)
  {
    T->GetEntry(i);
    cout << "Number of Hits: " << evaltree->get_nhits() << endl;
    for (int i=0; i<evaltree->get_nhits(); i++)
    {
      EvalHit *hit = evaltree->get_hit(i);
      if (hit)
      {
//	cout << "xin: " << hit->get_xin() << endl;
      }
    }
    cout << "Number of towers: " << evaltree->get_ntowers() << endl;
    for (int i=0; i<evaltree->get_ntowers(); i++)
    {
      EvalTower *twr = evaltree->get_tower(i);
      if (twr)
      {
//	cout << "tz: " << twr->get_tz() << endl;
      }
    }
    cout << "Number of clusters: " << evaltree->get_nclusters() << endl;
    for (int i=0; i<evaltree->get_nclusters(); i++)
    {
      EvalCluster *clus = evaltree->get_cluster(i);
      if (clus)
      {
//	cout << "cphi: " << clus->get_cphi() << endl;
      }
    }
    // cout << "ce: " << ce[0] << endl;
  }
}
