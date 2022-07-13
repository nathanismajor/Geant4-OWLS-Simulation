#include "detector.hh"
#include "G4Track.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4AnalysisManager.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{}

MySensitiveDetector::~MySensitiveDetector()
{}

	//Naming What Parameters to Measure

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROHist)
{
	G4Track *track = aStep->GetTrack();
	track->SetTrackStatus(fStopAndKill);
	
	G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
	G4StepPoint *postStepPoint = aStep->GetPostStepPoint();
	
	G4ThreeVector posPhoton = preStepPoint->GetPosition();
	G4cout << "Photon position =  " << posPhoton << G4endl;
	
	//G4double depositedEnergy = preStepPoint->GetTotalEnergy();
	//if (0 == depositedEnergy) return true;
	//G4cout << "Energy Deposited =  " << depositedEnergy << G4endl;
	
	//G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
	
	//Storing Data In ROOT File
	
/*
	//Assigning Data to Columns and Rows of ROOT File
	
	G4AnalysisManager = G4AnalysisManager::Instance();
	man->FillNtupleIColumn(0,evt);
	man->FillNtupleDColumn(1, posDetector[0]);
	man->FillNtupleDColumn(2, posDetector[1]);
	man->FillNtupleDColumn(3, posDetector[2]);
	man->AddNtupleRow(0);
*/
}
