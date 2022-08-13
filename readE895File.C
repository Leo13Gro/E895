// General includes from std
#include <iostream>
#include <fstream>

// Includes from ROOT
#include <Rtypes.h>
#include <TString.h>
#include <TChain.h>
#include <TVector3.h>
#include <TMath.h>
#include "TH1F.h"
#include "TFile.h"

void readE895File(TString iFileListName="")
{
	// Reading files from the text list line by line
  TChain *inchain = new TChain("T");
  std::ifstream file(iFileListName.Data());
  std::string line;
  int n_files = 0;
  while (std::getline(file, line))
  {
    inchain->Add(line.c_str());
    n_files++;
  }
  Long64_t Nentries = inchain->GetEntries();
  std::cout << n_files << " have been added to the TChain. " << Nentries << " events in total." << std::endl;

	// TTree variables information
  const Int_t NtrackMax = 1000;
  Int_t           ievnt;
  Int_t           ntrks_g;
  Int_t           mfilt_g;
  Int_t           nv0_g;
  Float_t         qphi_g;
  Float_t         dqphi_g;
  Float_t         phi12_g;
  Float_t         qxcorr_g;
  Float_t         qycorr_g;
  Int_t           n;
  Int_t           itrk[NtrackMax];
  Int_t           a1[NtrackMax];
  Int_t           a2[NtrackMax];
  Int_t           a3[NtrackMax];
  Int_t           z1[NtrackMax];
  Int_t           z2[NtrackMax];
  Int_t           z3[NtrackMax];
  Float_t         p1[NtrackMax];
  Float_t         p2[NtrackMax];
  Float_t         p3[NtrackMax];
  Float_t         dedx[NtrackMax];
  Float_t         diffphi[NtrackMax];
  Float_t         pzcm[NtrackMax];
  Float_t         qphi[NtrackMax];
  Float_t         theta[NtrackMax];
  Float_t         weight[NtrackMax];
  Float_t         ylab[NtrackMax];
  Float_t         ycm[NtrackMax];
  Float_t         mass[NtrackMax];
  Float_t         nnet[NtrackMax];

  // Setting up TTree branches
  inchain->SetBranchAddress("ievnt", &ievnt);
  inchain->SetBranchAddress("ntrks_g", &ntrks_g);
  inchain->SetBranchAddress("mfilt_g", &mfilt_g);
  inchain->SetBranchAddress("nv0_g", &nv0_g);
  inchain->SetBranchAddress("qphi_g", &qphi_g);
  inchain->SetBranchAddress("dqphi_g", &dqphi_g);
  inchain->SetBranchAddress("phi12_g", &phi12_g);
  inchain->SetBranchAddress("qxcorr_g", &qxcorr_g);
  inchain->SetBranchAddress("qycorr_g", &qycorr_g);
  inchain->SetBranchAddress("n", &n);
  inchain->SetBranchAddress("itrk", itrk);
  inchain->SetBranchAddress("a1", a1);
  inchain->SetBranchAddress("a2", a2);
  inchain->SetBranchAddress("a3", a3);
  inchain->SetBranchAddress("z1", z1);
  inchain->SetBranchAddress("z2", z2);
  inchain->SetBranchAddress("z3", z3);
  inchain->SetBranchAddress("p1", p1);
  inchain->SetBranchAddress("p2", p2);
  inchain->SetBranchAddress("p3", p3);
  inchain->SetBranchAddress("dedx", dedx);
  inchain->SetBranchAddress("diffphi", diffphi);
  inchain->SetBranchAddress("pzcm", pzcm);
  inchain->SetBranchAddress("qphi", qphi);
  inchain->SetBranchAddress("theta", theta);
  inchain->SetBranchAddress("weight", weight);
  inchain->SetBranchAddress("ylab", ylab);
  inchain->SetBranchAddress("ycm", ycm);
  inchain->SetBranchAddress("mass", mass);
  inchain->SetBranchAddress("nnet", nnet);

  TH1F* hMult = new TH1F("hMult", "Multiplicity", 100, 0, 400);
  TH1F* hCharge = new TH1F("hCharge", "Charge", 4, -2, 2);
  TH1D* hP = new TH1D("hP", "Momentum", 200, 0, 12);
  TH1D* hMass = new TH1D("hMass", "Invariant mass", 200, 0.2, 1.2);
  TH1D* hPhi = new TH1D("hPhi", "Phi angle", 100, 0, 360);
  TH1D* hLRap = new TH1D("hLRap", "Rapidity in Lab", 100, 0, 3.2);
	// Loop over events
  for (Long64_t iev = 0; iev<Nentries; iev++)
  {
    if (iev % 1000 == 0) std::cout << "Event [" << iev << "/" << Nentries << "]" << std::endl;
    Long64_t ientry = inchain->GetEntry(iev);
    if (ientry < 0)
    {
      std::cerr << "Couldn't read entry. Exit." << std::endl;
    }

		// do stuff with event in general (centrality determination)
	hMult->Fill(ntrks_g);
		// Loop over tracks
    for (int itr=0; itr<n; itr++)
    {
			TVector3 momentum;
			momentum.SetXYZ(p1[itr], p2[itr], p3[itr]);
			double pt  = momentum.Pt();
			double eta = momentum.Eta();
			double p = momentum.Mag();
			// do stuff with tracks (detected charged particles in the tracking system)
			hCharge->Fill(z1[itr]);
			hP->Fill(p);
			hMass->Fill(mass[itr]);
			hPhi->Fill(qphi[itr]);
			hLRap->Fill(ylab[itr]);
	}// Close loop over tracks
  }// Close loop over events
  TFile *f = new TFile("Histo.root", "recreate");
  hMult->Write();
  hCharge->Write();
  hP->Write();
  hMass->Write();
  hPhi->Write();
  hLRap->Write();
}
