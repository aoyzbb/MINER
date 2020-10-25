//*********************************************
//  This is Geant4 Template
//                                  author:Qian
//

#include "MyRootBasedAnalysis.hh"
#include "SimEvent.hh"
#include "Verbose.hh"

#include "MyDetectorConstruction.hh"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "TNtuple.h"
#include "TH1.h"
#include "TH2.h"
#pragma GCC diagnostic pop

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyRootBasedAnalysis::MyRootBasedAnalysis()
{
    SetFileName("output.root");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyRootBasedAnalysis::~MyRootBasedAnalysis()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyRootBasedAnalysis::BeginOfRunAction()
{
    if (!active)
        return;

    if (verbose)
        G4cout << "====>MyRootBasedAnalysis::BeginOfRunAction()" << G4endl;

    fRootFp = new TFile(fFileName, "recreate");
    if (!fRootFp)
    {
        G4cout << "\n====>MyRootBasedAnalysis::BeginOfRunAction(): "
               << "cannot open " << fFileName << G4endl;
        return;
    }

    fEvent = new SimEvent();
    fTree = new TTree("sim", "Tree of data events");
    fTree->Branch("SimEvent", "SimEvent", &fEvent, 32000, 100);

    //------- add your codes down here
    //
    return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyRootBasedAnalysis::EndOfRunAction()
{
    if (!active)
        return;

    if (verbose)
        G4cout << "====>MyRootBasedAnalysis::EndOfRunAction()" << G4endl;

    if (!fRootFp)
    {
        G4cout << "\n====>MyRootBasedAnalysis::EndOfRunAction(): "
               << "cannot open " << fFileName << G4endl;
        return;
    }

    //------- add your codes down here
    //

    G4cout << "\n====>In total " << fTree->GetEntries() << " Events have been stored." << G4endl;
    fRootFp->Write();
    fRootFp->Close();
    return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyRootBasedAnalysis::BeginOfEventAction(const G4Event *)
{
    if (!active)
        return;

    if (verbose)
        G4cout << "====>MyRootBasedAnalysis::BeginOfEventAction()" << G4endl;

    fEvent->MyClear();

    //------- add your codes down here
    //
    return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyRootBasedAnalysis::EndOfEventAction(const G4Event *)
{
    if (!active)
        return;

    if (verbose)
        G4cout << "====>MyRootBasedAnalysis::EndOfEventAction()" << G4endl;

    fTree->Fill();
    fEvent->MyClear();

    //------- add your codes down here
    //
    return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4ClassificationOfNewTrack MyRootBasedAnalysis::ClassifyNewTrack(const G4Track *)
{
    if (!active)
        return fUrgent;

    //------- add your codes down here
    //
    return fUrgent;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MyRootBasedAnalysis::PreTrackingAction(const G4Track *)
{
    if (!active)
        return;

    //------- add your codes down here
    //
    return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MyRootBasedAnalysis::PostTrackingAction(const G4Track *)
{
    if (!active)
        return;

    //------- add your codes down here
    //
    return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MyRootBasedAnalysis::SteppingAction(const G4Step *)
{
    if (!active)
        return;

    //------- add your codes down here
    //
    return;
}