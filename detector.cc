#include "detector.hh"
#include "G4Track.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4AnalysisManager.hh"
#include "G4OpticalPhoton.hh"


	//Sensitive Detector Measuring Transmission

MyLensDetector::MyLensDetector(G4String name) : G4VSensitiveDetector(name)
{}

MyLensDetector::~MyLensDetector()
{}
// What Parameters to Measure

G4bool MyLensDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROHist)
{
	G4Track *track = aStep->GetTrack();
	track->SetTrackStatus(fStopAndKill);
	G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
	G4StepPoint *postStepPoint = aStep->GetPostStepPoint();
	
	G4ThreeVector posPhoton = preStepPoint->GetPosition();
	G4cout << "Photon Arrived At Detector, Position = " << posPhoton << G4endl;
	
	G4double depositedEnergy = postStepPoint->GetTotalEnergy();
	G4cout << "Photon Arrived At Detector, Energy Deposited = " << depositedEnergy << G4endl;
	
	//Storing Data In ROOT File
	
//Assigning Data to Columns and Rows of ROOT File
	
	G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
	G4AnalysisManager* man = G4AnalysisManager::Instance();
	man->FillNtupleIColumn(0,evt);
	man->FillNtupleDColumn(1, posPhoton[0]);
	man->FillNtupleDColumn(2, posPhoton[1]);
	man->FillNtupleDColumn(3, posPhoton[2]);
	man->AddNtupleRow(0);
	G4cout << "Reached" << G4endl;
	
	/*G4int nrg = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
	G4AnalysisManager* mgr = G4AnalysisManager::Instance();
	mgr->FillNtupleIColumn(0,nrg);
	mgr->FillNtupleDColumn(1, depositedEnergy);
	mgr->AddNtupleRow;
	*/
	exit(0);
}
