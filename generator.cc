#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
	fParticleGun = new G4ParticleGun(1);
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
	delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	G4String particleName="opticalphoton";
	G4ParticleDefinition *particle = particleTable->FindParticle("opticalphoton");
	
//Use This for 0 Degree Measurement
	//G4ThreeVector pos(0.02025*m,0.,0.);
	
//Use This for 90 Degree Measurement
	G4ThreeVector pos(-0.02025*m,0.,0.);
	
	G4ThreeVector mom(1,0.,0.);
	
	fParticleGun->SetParticlePosition(pos);
	fParticleGun->SetParticleMomentumDirection(mom);
	fParticleGun->SetParticleMomentum(0.00000000000001103*MeV);
	fParticleGun->SetParticleDefinition(particle);
	
	fParticleGun->GeneratePrimaryVertex(anEvent);
}
