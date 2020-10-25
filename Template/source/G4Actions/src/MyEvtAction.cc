//*********************************************
//  This is Geant4 Template
//                                  author:Qian
//

#include "G4Event.hh"
#include "Verbose.hh"

#include "MyRunAction.hh"
#include "MyEvtAction.hh"
#include "MyAnalysisManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

MyEvtAction::MyEvtAction(MyRunAction *run)
    : G4UserEventAction(),
      fRun(run)
{
    if (verbose)
        G4cout << "====>MyEvtAction::MyEvtAction()" << G4endl;
    
    MyAnalysisManager::GetInstance()->SetRunAction(fRun);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....v

MyEvtAction::~MyEvtAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....v

void MyEvtAction::BeginOfEventAction(const G4Event *evt)
{
    if (verbose)
        G4cout << "====>MyEvtAction::BeginOfEventAction()" << G4endl;
    
    G4int eventId = evt->GetEventID();
    if (eventId % 1000 == 0)
        G4cout << "\n---> Begin of Event: " << eventId << G4endl;

    MyAnalysisManager::GetInstance()->BeginOfEventAction(evt);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void MyEvtAction::EndOfEventAction(const G4Event *evt)
{
    if (verbose)
        G4cout << "====>MyEvtAction::EndOfEventAction()" << G4endl;
    MyAnalysisManager::GetInstance()->EndOfEventAction(evt);
}