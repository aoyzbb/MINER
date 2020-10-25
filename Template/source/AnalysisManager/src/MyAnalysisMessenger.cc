//*********************************************
//  This is Geant4 Template
//                                  author:Qian
//

#include "MyAnalysisMessenger.hh"
#include "MyAnalysisManager.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4ios.hh"
#include "Randomize.hh"

MyAnalysisMessenger::MyAnalysisMessenger()
    : G4UImessenger()
{
    fFileNameCmd1 = new G4UIcmdWithAString("/MyRun/SetG4BasedFileName", this);
    fFileNameCmd1->SetGuidance("set the output filename using G4AnalysisManager");
    fFileNameCmd1->SetParameterName("fileName", true);
    fFileNameCmd1->SetDefaultValue("output.root");
    fFileNameCmd1->AvailableForStates(G4State_PreInit, G4State_Idle);

    fFileNameCmd2 = new G4UIcmdWithAString("/MyRun/SetRootBasedFileName", this);
    fFileNameCmd2->SetGuidance("set the output filename using root");
    fFileNameCmd2->SetParameterName("fileName", true);
    fFileNameCmd2->SetDefaultValue("output.root");
    fFileNameCmd2->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSelectCmd1 = new G4UIcmdWithoutParameter("/MyRun/ActiveG4BasedManager", this);
    fSelectCmd1->SetGuidance("use the G4AnalysisManager to store the data");
    fSelectCmd1->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSelectCmd2 = new G4UIcmdWithoutParameter("/MyRun/DeactiveG4BasedManager", this);
    fSelectCmd2->SetGuidance("do not use the G4AnalysisManager to store the data");
    fSelectCmd2->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSelectCmd3 = new G4UIcmdWithoutParameter("/MyRun/ActiveRootBasedManager", this);
    fSelectCmd3->SetGuidance("use the ROOT tree to store the data");
    fSelectCmd3->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSelectCmd4 = new G4UIcmdWithoutParameter("/MyRun/DeactiveRootBasedManager", this);
    fSelectCmd4->SetGuidance("do not use the ROOT tree to store the data");
    fSelectCmd4->AvailableForStates(G4State_PreInit, G4State_Idle);
}

MyAnalysisMessenger::~MyAnalysisMessenger()
{
    delete fFileNameCmd1;
    delete fFileNameCmd2;
    delete fSelectCmd1;
    delete fSelectCmd2;
    delete fSelectCmd3;
    delete fSelectCmd4;

}

void MyAnalysisMessenger::SetNewValue(G4UIcommand *command, G4String newValues)
{
    if (command == fFileNameCmd1)
    {
        G4cout << "\n---> Set ntuple output file name: " << newValues << G4endl;
        MyAnalysisManager::GetInstance()->SetG4BasedFileName(newValues);
    }

    if (command == fFileNameCmd2)
    {
        G4cout << "\n---> Set analysis output root-file name: " << newValues << G4endl;
        MyAnalysisManager::GetInstance()->SetRootBasedFileName(newValues);
    }

    if (command == fSelectCmd1)
    {
        G4cout << "\n---> Use the G4AnalysisManager to store the data. " << G4endl;
        MyAnalysisManager::GetInstance()->G4BasedActivated();
    }

    if (command == fSelectCmd2)
    {
        G4cout << "\n---> Do not use the G4AnalysisManager to store the data. " << G4endl;
        MyAnalysisManager::GetInstance()->G4BasedDeactivated();
    }

    if (command == fSelectCmd3)
    {
        G4cout << "\n---> Use the root tree to store the data. " << G4endl;
        MyAnalysisManager::GetInstance()->RootBasedActivated();
    }

    if (command == fSelectCmd4)
    {
        G4cout << "\n---> Do not use the root tree to store the data. " << G4endl;
        MyAnalysisManager::GetInstance()->RootBasedDeactivated();
    }
}
