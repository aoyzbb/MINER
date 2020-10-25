//*********************************************
//  This is Geant4 Template
//                                  author:Qian
//

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "Verbose.hh"

#include "g4root.hh"

#include "MyRunAction.hh"
#include "MyDetectorConstruction.hh"
#include "MyAnalysisManager.hh"
#include "MyAnalysisMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

MyRunAction::MyRunAction(MyDetectorConstruction *det)
    : fDetector(det)
{
    if (verbose)
        G4cout << "====>MyRunAction::MyRunAction()" << G4endl;

    fRunMessenger = new MyAnalysisMessenger();
    MyAnalysisManager::GetInstance()->SetDetectorConstruction(fDetector);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

MyRunAction::~MyRunAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void MyRunAction::BeginOfRunAction(const G4Run *)
{
    if (verbose)
        G4cout << "====>MyRunAction::BeginOfRunAction()" << G4endl;

    //inform the runManager to save random number seed
    //G4RunManager::GetRunManager()->SetRandomNumberStore(true);
    MyAnalysisManager::GetInstance()->BeginOfRunAction();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void MyRunAction::EndOfRunAction(const G4Run *)
{
    if (verbose)
        G4cout << "====>MyRunAction::EndOfRunAction()" << G4endl;

    MyAnalysisManager::GetInstance()->EndOfRunAction();
}
