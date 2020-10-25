//*********************************************
//  This is Geant4 Template
//                                  author:Qian
//

#include "globals.hh"

#include "MyDetectorMessenger.hh"
#include "MyDetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyDetectorMessenger::MyDetectorMessenger(MyDetectorConstruction *myDet)
    : G4UImessenger(),
      fTheDetector(myDet),
      fTheDetectorDir(0),
      fTheReadCommand(0),
      fTheWriteCommand(0)
{
    fTheDetectorDir = new G4UIdirectory("/MyDet/");
    fTheDetectorDir->SetGuidance("Detector control.");

    fTheReadCommand = new G4UIcmdWithAString("/MyDet/readFile", this);
    fTheReadCommand->SetGuidance("READ GDML file with given name");
    fTheReadCommand->SetParameterName("FileRead", false);
    fTheReadCommand->SetDefaultValue("main.gdml");
    fTheReadCommand->AvailableForStates(G4State_PreInit);

    fTheWriteCommand = new G4UIcmdWithAString("/MyDet/writeFile", this);
    fTheWriteCommand->SetGuidance("WRITE GDML file with given name");
    fTheWriteCommand->SetParameterName("FileWrite", false);
    fTheWriteCommand->SetDefaultValue("main_writer.gdml");
    fTheWriteCommand->AvailableForStates(G4State_PreInit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyDetectorMessenger::~MyDetectorMessenger()
{
    delete fTheReadCommand;
    delete fTheWriteCommand;
    delete fTheDetectorDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyDetectorMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    if (command == fTheReadCommand)
    {
        fTheDetector->SetReadFile(newValue);
    }
    if (command == fTheWriteCommand)
    {
        fTheDetector->SetWriteFile(newValue);
    }
}
