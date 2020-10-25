//*********************************************
//  This is Geant4 Template
//                                  author:Qian
//


#include "Verbose.hh"
#include "MyActionInitialization.hh"
#include "MyParticleGun.hh"
//#include "MyPrimaryGeneratorGPS.hh"
#include "MyRunAction.hh"
#include "MyEvtAction.hh"
#include "MyStepAction.hh"
#include "MyStackAction.hh"
#include "MyTrackAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyActionInitialization::MyActionInitialization(MyDetectorConstruction *detConstruction)
    : G4VUserActionInitialization(),
      fDetectorConstruction(detConstruction)
{
    if (verbose)
        G4cout << "====>MyActionInitialization::MyActionInitialization()" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyActionInitialization::~MyActionInitialization()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyActionInitialization::BuildForMaster() const
{
    // No specific action for Master
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyActionInitialization::Build() const
{
    if (verbose)
        G4cout << "====>MyActionInitialization::Build()" << G4endl;

    //#PartGun 0. 选择单粒子模式, 二选一
    SetUserAction(new MyParticleGun);
    //#PartGPS 0. 选择GPS模式, 二选一
    //SetUserAction(new MyPrimaryGeneratorGPS);

    MyRunAction *runAction = new MyRunAction(fDetectorConstruction);
    SetUserAction(runAction);

    MyEvtAction *evtAction = new MyEvtAction(runAction);
    SetUserAction(evtAction);

    SetUserAction(new MyStepAction(runAction, evtAction, fDetectorConstruction));

    SetUserAction(new MyStackAction());

    SetUserAction(new MyTrackAction());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
