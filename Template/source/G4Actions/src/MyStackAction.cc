//*********************************************
//  This is Geant4 Template
//                                  author:Qian
//

#include "G4RunManager.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"

#include "MyStackAction.hh"
#include "MyDetectorConstruction.hh"
#include "MyAnalysisManager.hh"
#include "Verbose.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

MyStackAction::MyStackAction()
    : G4UserStackingAction(), fKillSecondary(0)
{
    if (verbose)
        G4cout << "====>MyStackAction::MyStackAction()" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

MyStackAction::~MyStackAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4ClassificationOfNewTrack MyStackAction::ClassifyNewTrack(const G4Track *aTrack)
{
    if (verbose)
        G4cout << "====>MyStackAction::ClassifyNewTrack(const G4Track* aTrack)" << G4endl;

    return MyAnalysisManager::GetInstance()->ClassifyNewTrack(aTrack);
    //return fUrgent;
}
