TFile *file = new TFile("neutronFlux.root");
        TH1F *hneutron = (TH1F *)file->Get("hneutron");
        double eng = hneutron->GetRandom();

        G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
        G4ParticleDefinition *particle = particleTable->FindParticle("n");

        fParticleGun->SetParticleDefinition(particle);
        fParticleGun->SetParticleEnergy(eng);
        fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1, 0, 0));
        fParticleGun->SetParticlePosition(G4ThreeVector(-100, 10*G4UniformRand(), 10*G4UniformRand()));