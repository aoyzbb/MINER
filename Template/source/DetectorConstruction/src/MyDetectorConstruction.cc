//*********************************************
//  This is Geant4 Template
//                                  author:Qiann
//

#include "Verbose.hh"
#include "MyDetectorConstruction.hh"
#include "MySDDetector.hh"
#include "G4SDManager.hh"
#include "G4GDMLParser.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"

#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "MyDetectorReader.hh"
#include "MyDetectorMessenger.hh"
#include "MyDetectorSettings.hh"
#include "MyMagneticField.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyDetectorConstruction::MyDetectorConstruction()
    : G4VUserDetectorConstruction(),
      fReader(0), fWriter(0), fParser(0), fSettings(0)
{
    fReadFile = "./gdml/main.gdml";
    fWriteFile = "./gdml/main_writer.gdml";
    fWritingFlag = 0;

    fDetectorMessenger = new MyDetectorMessenger(this);

    fReader = new MyDetectorReader;
    fParser = new G4GDMLParser(fReader, fWriter);

    fSettings = new MyDetectorSettings;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyDetectorConstruction::~MyDetectorConstruction()
{
    delete fDetectorMessenger;
    delete fReader;
    delete fParser;
    delete fSettings;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
    fParser->Read(fReadFile, false);
    fSettings->ApplyAuxValue(fParser);

    //Get volume by it's name...
    //G4LogicalVolumeStore::GetInstance()->GetVolume("Volume Name");

    //G4cout << *(G4Material::GetMaterialTable()) << G4endl;
    return fParser->GetWorldVolume();
}

void MyDetectorConstruction::ConstructSDandField()
{
    //------------------------------------------------
    // 定义SD
    //------------------------------------------------

    /*
    G4SDManager *SDman = G4SDManager::GetSDMpointer();
    MySDRich *aRICH = new MySDRich("RICH");
    SDman->AddNewDetector(aRICH);

    //------------------------------------------------
    // Sensitive detectors GDML 定义
    //------------------------------------------------
    //  在RICH.gdml里，定义如下关键词
    //        <volume name="ReadoutBoxVol">                     定义了读出体
    //      	    <materialref ref="G4_Cu" />                 定义了读出体的材质
    //      		<solidref ref="ReadoutBox" />               定义了读出体的结构尺寸
    //      		<auxiliary auxtype="SensDet" auxvalue="RICH"/>    SensDet表示这是个读出体，其值为RICH，这个名字必须和上面的SDMananger那里对应上
    //	      </volume>
    //------------------------------------------------
    // 注意，这里RICH的logicVolume只有一个，但是放置了好几次，因此会有多个PhysicsVolume
    //
    const G4GDMLAuxMapType *auxmap = fParser->GetAuxMap();
    for (G4GDMLAuxMapType::const_iterator iter = auxmap->begin(); iter != auxmap->end(); iter++)
    {
        for (G4GDMLAuxListType::const_iterator vit = (*iter).second.begin(); vit != (*iter).second.end(); vit++)
        {
            if ((*vit).type == "SensDet")
            {
                G4cout << "Attaching sensitive detector " << (*vit).value
                       << " to volume " << ((*iter).first)->GetName()
                       << " " << ((*iter).first) << G4endl << G4endl;

                G4VSensitiveDetector *mydet = SDman->FindSensitiveDetector((*vit).value);
                if (mydet)
                {
                    G4LogicalVolume *myvol = (*iter).first;
                    myvol->SetSensitiveDetector(mydet);
                }
            }
        }
    }
    */

    //------------------------------------------------
    // Fields

    //1. 全局磁场 ------------------------------------------------
    //#MagField 0. 构造全局磁场
    // Create global magnetic field messenger.
    // Uniform magnetic field is then created automatically if
    // the field value is not zero.
    // active by mac file through:
    //       /globalField/setValue 1.0 0 0 tesla

    if (fSettings->GetUserMagFieldFlag() == 0)
    {
        G4ThreeVector fieldValue;
        fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
        fMagFieldMessenger->SetVerboseLevel(1);

        // Register the field messenger for deleting
        G4AutoDelete::Register(fMagFieldMessenger);
    }

    //2. 局部磁场 ------------------------------------------------
    //#MagField 1. 构造局部磁场
    // 这里磁场是定义在 MagTubeVol 所包含的逻辑体内部，注意这里的名字和 XML 里要相互对应

    if (fSettings->GetUserMagFieldFlag() != 0)
    {
        fMagneticField = new MyMagneticField();
        fFieldMgr = new G4FieldManager();
        fFieldMgr->SetDetectorField(fMagneticField);
        fFieldMgr->CreateChordFinder(fMagneticField);
        G4bool forceToAllDaughters = true;
        auto *fMagneticLogical = G4LogicalVolumeStore::GetInstance()->GetVolume("MagTubeVol");
        fMagneticLogical->SetFieldManager(fFieldMgr, forceToAllDaughters);

        // Register the field and its manager for deleting
        G4AutoDelete::Register(fMagneticField);
        G4AutoDelete::Register(fFieldMgr);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyDetectorConstruction::SetReadFile(const G4String &fname)
{
    fReadFile = fname;
    fWritingFlag = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyDetectorConstruction::SetWriteFile(const G4String &fname)
{
    fWriteFile = fname;
    fWritingFlag = 1;
}