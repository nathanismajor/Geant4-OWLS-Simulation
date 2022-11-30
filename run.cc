#include "run.hh"

MyRunAction::MyRunAction()
{}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run*)
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	
	man->OpenFile("position.root");
	
	man->CreateNtuple("Positions", "Positions");
	man->CreateNtupleDColumn("fEvent");
	man->CreateNtupleDColumn("fX");
	man->CreateNtupleDColumn("fY");
	man->CreateNtupleDColumn("fZ");
	man->FinishNtuple(0);
	
	/*G4AnalysisManager *mgr = G4AnalysisManager::Instance();
	
	mgr->OpenFile("energy.root");
	
	mgr->CreateNtuple("Energies", "Energies");
	mgr->CreateNtupleDColumn("fEvent");
	mgr->CreateNtupleDColumn("fX");
	mgr->FinishNtuple(0);*/
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	
	man->Write();
	man->CloseFile();
	/*
	G4AnalysisManager *mgr = G4AnalysisManager::Instance();
	
	mgr->Write();
	mgr->CloseFile();*/
}
