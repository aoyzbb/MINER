//*********************************************
//  This is Geant4 Template
//                                  author:Qian
//

#include "MyParticleGunMessenger.hh"
#include "MyParticleGun.hh"
#include "MyRunAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4ios.hh"
#include "Randomize.hh"

MyParticleGunMessenger::MyParticleGunMessenger(MyParticleGun *gun)
    : G4UImessenger(),
      gunAction(gun)
{
    //#PartGun 5. 初始化粒子枪的命令
    fBGFileNameCmd = new G4UIcmdWithAString("/MyGun/SetGunBGFile", this);
    fBGFileNameCmd->SetGuidance("set the background filename, read it and generate the gun accordingly.");
    fBGFileNameCmd->SetParameterName("fileName", true);
    fBGFileNameCmd->SetDefaultValue("backgrounddata.root");
    fBGFileNameCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fGunTypeCmd = new G4UIcmdWithoutParameter("/MyGun/SimpleGun", this);
    fGunTypeCmd->SetGuidance("use the G4 particle gun");
    fGunTypeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

MyParticleGunMessenger::~MyParticleGunMessenger()
{
    delete fBGFileNameCmd;
    delete fGunTypeCmd;
}

void MyParticleGunMessenger::SetNewValue(G4UIcommand *command, G4String newValues)
{
    //#PartGun 6. 执行粒子枪的命令
    if (command == fBGFileNameCmd)
    {
        gunAction->OpenFile(newValues);
    }

    if (command == fGunTypeCmd)
    {
        gunAction->UseSimpleGun();
    }
}
