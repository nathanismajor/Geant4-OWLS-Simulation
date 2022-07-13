#include "construction.hh"
#include "G4VisAttributes.hh"
#include "G4Scintillation.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4RotationMatrix.hh"


MyDetectorConstruction::MyDetectorConstruction()
{}

MyDetectorConstruction::~MyDetectorConstruction()
{}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
	
	//World
	
	G4NistManager *nist = G4NistManager::Instance();
	G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");
	
	G4Box *solidWorld = new G4Box("solidWorld", 1*m, 1*m, 1*m);
	
	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
	
	G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);
	
	G4VisAttributes* white = new G4VisAttributes(G4Colour(255/255., 255/255., 255/255.));
        
        white->SetVisibility(false);
        white->SetForceSolid(true);
       
        logicWorld->SetVisAttributes(white); 
	
	
	//Cuvette

	G4Material* vesselMat = nist->FindOrBuildMaterial("G4_GLASS_PLATE");
	
	G4Box *solidVessel = new G4Box("solidVessel", 0.0125*m, 0.045*m, 0.0125*m);
	
	G4LogicalVolume *logicVessel = new G4LogicalVolume(solidVessel, vesselMat, "logicVessel");
	
	G4VPhysicalVolume *physVessel = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicVessel, "physVessel", 0, false, 0, true);
	  
	new G4PVPlacement(0, G4ThreeVector(), logicVessel, "Vessel", logicWorld, false, 0);

	//Sample
	
	G4Material* env_mat = nist->FindOrBuildMaterial("G4_WATER");
	
	G4Box *solidEnv = new G4Box("Sample", 0.01125*m, 0.04375*m, 0.01125*m);
	
	G4LogicalVolume* logicEnv =
    		new G4LogicalVolume(solidEnv, env_mat,"Sample");
    		
    	G4VPhysicalVolume *physSample = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicEnv, "physSample", 0, false, 0, true);
    
    	new G4PVPlacement(0, G4ThreeVector(), logicEnv, "Sample", logicVessel, false, 0);         
	

	G4VisAttributes* blue = new G4VisAttributes(G4Colour(0/255., 0/255., 255/255.));
        
        blue->SetVisibility(true);
        blue->SetForceSolid(true);
       
        logicEnv->SetVisAttributes(blue); 
        
	//Scintillation Properties
	
	G4Material* smplMaterial = nist->FindOrBuildMaterial("G4_WATER");   //change this line depending on the sample material
	G4double energy[2] = {1.239841939*eV/0.2, 1.239841939*eV/0.9};
	G4double rindexsmpl[2] = {1.33,1.33};
	G4MaterialPropertiesTable *smpl = new G4MaterialPropertiesTable();
	smpl->AddProperty("RINDEX", energy, rindexsmpl, 2);
	G4double smpl_Energy[3] = {9.61*eV, 9.68*eV, 9.77*eV};
	G4double smpl_SCINT[3] = {0.1,1.0,0.1};
	G4double smpl_abslen[3] = {0.2*m,0.2*m,0.2*m};
	smpl->AddProperty("SCINTILLATIONCOMPONENT1", smpl_Energy, smpl_SCINT, 3);
	smpl->AddConstProperty("SCINTILLATIONYIELD", 40000. / MeV);
	smpl->AddConstProperty("RESOLUTIONSCALE",1.);
	smpl->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 6. *ns);
	smpl->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 1500. *ns);
	smpl->AddConstProperty("SCINTILLATIONYIELD1", 1.0);
	smpl->AddProperty("ABSLENGTH", smpl_Energy, smpl_abslen, 3);
	smplMaterial->SetMaterialPropertiesTable(smpl);

	//Index of Refraction of the Cuvette Glass (Quartz)
	
	G4Material* vesselMaterial = nist->FindOrBuildMaterial("G4_GLASS_PLATE");
	G4double rindexvessel[2] = {1.54425,1.55338};
	G4MaterialPropertiesTable *vessel = new G4MaterialPropertiesTable();
	vessel->AddProperty("RINDEX", energy, rindexvessel, 2);
	G4double vessel_Energy[3] = {9.61*eV, 9.68*eV, 9.77*eV};
	G4double vessel_abslen[3] = {0.2*m,0.2*m,0.2*m};
	vessel->AddProperty("ABSLENGTH", vessel_Energy, vessel_abslen,3);
	vesselMaterial->SetMaterialPropertiesTable(vessel);

	//Index of Refraction of The World Material (Air)
	
	G4Material* worldMaterial = nist->FindOrBuildMaterial("G4_AIR");
	G4double rindexworld[2] = {1,1.1};
	G4MaterialPropertiesTable *world = new G4MaterialPropertiesTable();
	world->AddProperty("RINDEX", energy, rindexworld, 2);
	G4double world_Energy[3] = {9.61*eV, 9.68*eV, 9.77*eV};
	G4double world_abslen[3] = {0.2*m,0.2*m,0.2*m};
	world->AddProperty("ABSLENGTH", world_Energy, world_abslen,3);
	worldMaterial->SetMaterialPropertiesTable(world);
	
	//Fiber with Cylindrical Aperture for Detection of Absorption
	
	//G4double innerRadius = 0.0002*m;
	G4double innerRadius = 0*m;
	G4double outerRadius = 0.0015*m;
	G4double cylinderHeight = 0.00005*m;
	G4double startAngle = 0*deg;
	G4double spanAngle = 360.*deg;
	
	G4Tubs *absorbCylinder = new G4Tubs("absorbCylinder", innerRadius, outerRadius, cylinderHeight, startAngle, spanAngle);
	
	logicAbsorption = new G4LogicalVolume(absorbCylinder, worldMat, "logicAbsorption");
	
	G4VPhysicalVolume *physAbsorption = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicAbsorption, "physCylinder", 0, false, 0, true);
	
	new G4PVPlacement(0, G4ThreeVector(0.,0.,0.02*m), logicAbsorption, "AbsorptionDetector", logicWorld, false, 0); 
	
	G4VisAttributes* orange = new G4VisAttributes(G4Colour(255/255., 165/255., 0/255.));
        
        orange->SetVisibility(true);
        orange->SetForceSolid(true);
       
        logicAbsorption->SetVisAttributes(orange); 
	
	
	//Fiber with Cylindrical Aperture for Detection of Scattering
	
	G4Tubs *scatterCylinder = new G4Tubs("scatterCylinder", innerRadius, outerRadius, cylinderHeight, startAngle, spanAngle);
	
	logicScattering = new G4LogicalVolume(scatterCylinder, worldMat, "logicScattering");
	
	G4VPhysicalVolume *physScattering = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicScattering, "physCylinder", 0, false, 0, true);
	
	G4RotationMatrix *rot = new G4RotationMatrix;
	rot->rotateY(90*deg);
	
	new G4PVPlacement(rot, G4ThreeVector(-0.02*m,0.,0.), logicScattering, "ScatteringDetector", logicWorld, false, 0); 
	
	G4VisAttributes* green = new G4VisAttributes(G4Colour(0/255., 255/255., 0/255.));
        
        green->SetVisibility(true);
        green->SetForceSolid(true);
       
        logicScattering->SetVisAttributes(green);
	
	return physWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{
	MySensitiveDetector *absDet = new MySensitiveDetector("AbsorptionDetector");
	//MySensitiveDetector *scaDet = new MySensitiveDetector("ScatteringDetector");
	
	logicAbsorption->SetSensitiveDetector(absDet);
	//logicScattering->SetSensitiveDetector(scaDet);
}
