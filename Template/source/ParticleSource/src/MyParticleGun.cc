//*********************************************
//  This is Geant4 Template
//                                  author:Qian
//

#include "MyParticleGun.hh"
#include "MyParticleGunMessenger.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "Verbose.hh"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#pragma GCC diagnostic pop

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyParticleGun::MyParticleGun()
    : G4VUserPrimaryGeneratorAction(),
      fParticleGun(0)
{
    if (verbose)
        G4cout << "====>MyParticleGun::MyParticleGun()" << G4endl;

    fGunMessenger = new MyParticleGunMessenger(this);

    G4int n_particle = 1;
    fParticleGun = new G4ParticleGun(n_particle);

    //#PartGun 2. 初始化变量
    file = new TFile("neutronFlux.root");
    engHist = (TH1F *)file->Get("fhist");

    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition *particle = particleTable->FindParticle("gamma");

    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(1.);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, 1));
    fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, 0));
    GunType = 0; //a simple flag for gun type: 0 for simple gun, 1 for read from root file.
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//根据rootfile分布来设置
void MyParticleGun::OpenFile(G4String fname)
{
    GunType = 1;

    // 从用户提供的root文件来生成粒子，需要根据root文件来修改下面的内容。
    //
    /*
    rootfile = new TFile(fname);
    if (!rootfile->IsOpen())
    {
        G4cout << "####> Can't open the " << fname << ", will use simple-Gun instead." << G4endl;
        GunType = 0;
        return;
    }

    tree = (TTree *)rootfile->Get("datatree");
    tree->SetBranchAddress("XPosition", &x);
    tree->SetBranchAddress("YPosition", &y);
    tree->SetBranchAddress("ZPosition", &z);
    tree->SetBranchAddress("XMomentum", &px);
    tree->SetBranchAddress("YMomentum", &py);
    tree->SetBranchAddress("ZMomentum", &pz);
    tree->SetBranchAddress("BackgroundFrequency", &freq);

    Nentries = tree->GetEntries();
    */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyParticleGun::~MyParticleGun()
{
    delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyParticleGun::GeneratePrimaries(G4Event *anEvent)
{
    if (verbose)
        G4cout << "====>MyParticleGun::GeneratePrimaries()" << G4endl;

    //#PartGun 3. 对粒子枪进行抽样
    if (GunType == 0)
    {
        //单粒子控制, 可配合随机抽样函数来进行： G4UniformRand()给出[0~1]随机数
        double eng = engHist->GetRandom();
        fParticleGun->SetParticleEnergy(eng);
        fParticleGun->SetParticlePosition(G4ThreeVector(G4UniformRand()*150, G4UniformRand()*150, -132.8));
        fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, 1));
    }
    else
    {
        //根据root文件产生
        /*
        G4int i = anEvent->GetEventID();
        if (i < Nentries)
            tree->GetEntry(i);
        if (i % 1000 == 0)
            G4cout << i << G4endl;

        G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
        G4ParticleDefinition *particle = particleTable->FindParticle("e-");

        fParticleGun->SetParticleDefinition(particle);
        fParticleGun->SetParticleEnergy(sqrt(px * px + py * py + pz * pz));
        fParticleGun->SetParticlePosition(G4ThreeVector(x, y, z));
        fParticleGun->SetParticleMomentumDirection(G4ThreeVector(px, py, pz));
        //G4cout<<"ID: "<<i<<": "<<sqrt(px * px + py * py + pz * pz)<<G4endl;
        */
    }

    fParticleGun->GeneratePrimaryVertex(anEvent);
}
