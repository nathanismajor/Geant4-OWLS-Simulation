#ifndef DETECTOR_HH
#define DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"

#include "G4VHit.hh"
#include "G4Allocator.hh"
#include "G4THitsCollection.hh"

	//Class For Sensitive Detector

class MyLensDetector : public G4VSensitiveDetector
{
public:
	MyLensDetector(G4String name);
	~MyLensDetector();
	
private:
	virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
};
	
	
	
	
	
	
	




#endif
