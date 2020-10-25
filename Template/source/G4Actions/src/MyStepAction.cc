//*********************************************
//  This is Geant4 Template
//                                  author:Qian
//

#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

#include "MyStepAction.hh"
#include "MyEvtAction.hh"
#include "MyDetectorConstruction.hh"
#include "MyAnalysisManager.hh"
#include "Verbose.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

MyStepAction::MyStepAction(MyRunAction *run, MyEvtAction *evt, MyDetectorConstruction *det)
    : G4UserSteppingAction(),
      fRun(run),
      fEvt(evt),
      fDetector(det)
{
    if (verbose)
        G4cout << "====>MyStepAction::MyStepAction()" << G4endl;

    MyAnalysisManager::GetInstance()->SetRunAction(fRun);
    MyAnalysisManager::GetInstance()->SetEvtAction(fEvt);
    MyAnalysisManager::GetInstance()->SetDetectorConstruction(fDetector);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

MyStepAction::~MyStepAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void MyStepAction::UserSteppingAction(const G4Step *aStep)
{
    if (verbose)
        G4cout << "====>MyStepAction::UserSteppingAction()" << G4endl;

    MyAnalysisManager::GetInstance()->SteppingAction(aStep);
}