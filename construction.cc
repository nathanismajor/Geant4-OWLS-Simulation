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

	//Construction of Sample Materials
	
//Titanium Dioxide (TiO2) and Water (H2O)

	G4double a = 47.867*g/mole;
	G4Element* Ti = new G4Element("Titanium", "Ti", 1., a);

	G4double b = 16.00*g/mole;
	G4Element* O = new G4Element("Oxygen", "O", 1., b);
	
	G4double c = 1.01*g/mole;
	G4Element* H = new G4Element("Hydrogen", "H", 1., c);

	G4double tio2density = 4.23*g/cm3;
	G4double h2odensity = 1*g/cm3;

	G4Material* TiO2 = new G4Material("Titanium Dioxide", tio2density, 2);
	TiO2->AddElement(Ti, 1);
	TiO2->AddElement(O, 2);

	G4Material* H2O = new G4Material("Water", h2odensity, 2);
	H2O->AddElement(H, 2);
	H2O->AddElement(O, 1);
	
G4Material* opqblnd =
	new G4Material("opaqueblend", h2odensity, 2);
	opqblnd->AddMaterial(TiO2, 1*perCent);
	opqblnd->AddMaterial(H2O, 99*perCent);

	//Linear Alkyl Benzene - 2,5-Diphenyloxazole (LAB-PPO)

//PPO
	G4double d = 12.011*g/mole;
	G4Element* C = new G4Element("Carbon", "C", 1., d);
	G4double ppodensity = 1.06*g/cm3;
	
	G4double e = 14.007*g/mole;
	G4Element* N = new G4Element("Nitrogen", "C", 1., e);
	
	G4Material* PPO = new G4Material("PPO", ppodensity, 4);
	PPO->AddElement(C, 15);
	PPO->AddElement(H, 11);
	PPO->AddElement(N, 1);
	PPO->AddElement(O, 1);
	
//LAB
	G4double labdensity = 0.863*g/cm3;
	G4Material* LAB = new G4Material("LAB", labdensity, 4);
	LAB->AddElement(C, 6);
	LAB->AddElement(H, 5);
	LAB->AddElement(C, 12);
	LAB->AddElement(H, 25);
	
//LAB-PPO Blend

	G4Material* LAB_PPO = new G4Material("LABPPOblend", labdensity,2);
	LAB_PPO->AddMaterial(LAB, 50*perCent);
	LAB_PPO->AddMaterial(PPO, 50*perCent);

	//Sample (Inside Cuvette)
	
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
        
	//Scintillation Properties of the Control Sample (Water)
	
	G4Material* waterSample = nist->FindOrBuildMaterial("G4_WATER"); 
	G4double energy[2] = {1.239841939*eV/0.2, 1.239841939*eV/0.9};
	G4double rindexwtrsmpl[2] = {1.33,1.33};
	G4MaterialPropertiesTable *wtrsmpl = new G4MaterialPropertiesTable();
	wtrsmpl->AddProperty("RINDEX", energy, rindexwtrsmpl, 2);
	G4double wtrsmpl_Energy[3] = {9.61*eV, 9.68*eV, 9.77*eV};
	G4double wtrsmpl_SCINT[3] = {0.1,1.0,0.1};
	G4double wtrsmpl_abslen[3] = {227*m,227*m,227*m};
	wtrsmpl->AddProperty("SCINTILLATIONCOMPONENT1", wtrsmpl_Energy, wtrsmpl_SCINT, 3);
	wtrsmpl->AddConstProperty("SCINTILLATIONYIELD", 127. / MeV);
	wtrsmpl->AddConstProperty("RESOLUTIONSCALE",1.);
	wtrsmpl->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 6. *ns);
	wtrsmpl->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 1500. *ns);
	wtrsmpl->AddConstProperty("SCINTILLATIONYIELD1", 1.0);
	wtrsmpl->AddProperty("ABSLENGTH", wtrsmpl_Energy, wtrsmpl_abslen, 3);
	waterSample->SetMaterialPropertiesTable(wtrsmpl);

	//Scintillation Properties of the Sample (Titanium Dioxide Concentrated in Water)
	/*
	G4Material* tio2_Sample = nist->FindOrBuildMaterial("opqblnd"); 
	//G4double energy[2] = {1.239841939*eV/0.2, 1.239841939*eV/0.9};
	G4double rindextio2_smpl[2] = {1.342842,1.342842};
	G4MaterialPropertiesTable *tio2_smpl = new G4MaterialPropertiesTable();
	wtrsmpl->AddProperty("RINDEX", energy, rindextio2_smpl, 2);
	G4double tio2_smpl_Energy[3] = {9.61*eV, 9.68*eV, 9.77*eV};
	G4double tio2_smpl_SCINT[3] = {0.1,1.0,0.1};
	G4double tio2_smpl_abslen[3] = {227*m,227*m,227*m};
	tio2_smpl->AddProperty("SCINTILLATIONCOMPONENT1", tio2_smpl_Energy, tio2_smpl_SCINT, 3);
	tio2_smpl->AddConstProperty("SCINTILLATIONYIELD", 40000. / MeV);
	tio2_smpl->AddConstProperty("RESOLUTIONSCALE",1.);
	tio2_smpl->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 6. *ns);
	tio2_smpl->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 1500. *ns);
	tio2_smpl->AddConstProperty("SCINTILLATIONYIELD1", 1.0);
	tio2_smpl->AddProperty("ABSLENGTH", tio2_smpl_Energy, tio2_smpl_abslen, 3);
	tio2_Sample->SetMaterialPropertiesTable(tio2_smpl);
	*/
	
	//Scintillation Properties of LAB-PPO
	
	G4Material* labSampleMaterial = nist->FindOrBuildMaterial("LAB_PPO");
	G4double rindexlab[2] = {1.525,1.525};
	G4MaterialPropertiesTable *labSample = new G4MaterialPropertiesTable();
	labSample->AddProperty("RINDEX", energy, rindexlab, 2);
	G4double labSample_Energy[3] = {9.61*eV, 9.68*eV, 9.77*eV};
	G4double labSample_abslen[3] = {26.8*m, 16.9*m, 25.8*m};
	labSample->AddProperty("ABSLENGTH", labSample_Energy, labSample_abslen, 3);
	labSample->AddConstProperty("SCINTILLATIONYIELD", 127. / MeV);
	labSample->AddConstProperty("SCINTILLATIONYIELD1", 1.0);
	labSample->AddConstProperty("RESOLUTIONSCALE",1.);
	
	
	//Material Properties of the Cuvette Glass (Quartz)
	
	G4Material* vesselMaterial = nist->FindOrBuildMaterial("G4_GLASS_PLATE");
	G4double rindexvessel[2] = {1.54425,1.55338};
	G4MaterialPropertiesTable *vessel = new G4MaterialPropertiesTable();
	vessel->AddProperty("RINDEX", energy, rindexvessel, 2);
	G4double vessel_Energy[3] = {9.61*eV, 9.68*eV, 9.77*eV};
	G4double vessel_abslen[3] = {0.2*m,0.2*m,0.2*m};
	vessel->AddProperty("ABSLENGTH", vessel_Energy, vessel_abslen,3);
	vesselMaterial->SetMaterialPropertiesTable(vessel);

	//Material Properties of The World Material (Air)
	
	G4Material* worldMaterial = nist->FindOrBuildMaterial("G4_AIR");
	G4double rindexworld[2] = {1,1.1};
	G4MaterialPropertiesTable *world = new G4MaterialPropertiesTable();
	world->AddProperty("RINDEX", energy, rindexworld, 2);
	G4double world_Energy[3] = {9.61*eV, 9.68*eV, 9.77*eV};
	G4double world_abslen[3] = {0.2*m,0.2*m,0.2*m};
	world->AddProperty("ABSLENGTH", world_Energy, world_abslen,3);
	worldMaterial->SetMaterialPropertiesTable(world);
	
	//Fiber with Cylindrical Aperture for Detection of Transmission
	
	//G4double innerRadius = 0.0002*m;
	G4double innerRadius = 0*m;
	//G4double outerRadius = 0.0015*m;
	G4double outerRadius = 0.0002*m;
	G4double cylinderHeight = 0.00005*m;
	G4double startAngle = 0*deg;
	G4double spanAngle = 360.*deg;
	
	G4Tubs *absorbCylinder = new G4Tubs("absorbCylinder", innerRadius, outerRadius, cylinderHeight, startAngle, spanAngle);
	
	logicTransmission = new G4LogicalVolume(absorbCylinder, worldMat, "logicTransmission");
	
	G4VPhysicalVolume *physTransmission = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicTransmission, "physCylinder", 0, false, 0, true);
	
	new G4PVPlacement(0, G4ThreeVector(0.,0.,-0.02025*m), logicTransmission, "TransmissionDetector", logicWorld, false, 0); 
	
	G4VisAttributes* orange = new G4VisAttributes(G4Colour(255/255., 165/255., 0/255.));
        
        orange->SetVisibility(true);
        orange->SetForceSolid(true);
       
        logicTransmission->SetVisAttributes(orange);
	
	
	//Fiber with Cylindrical Aperture for Detection of Scattering
	
	G4Tubs *scatterCylinder = new G4Tubs("scatterCylinder", innerRadius, outerRadius, cylinderHeight, startAngle, spanAngle);
	
	logicScattering = new G4LogicalVolume(scatterCylinder, worldMat, "logicScattering");
	
	G4VPhysicalVolume *physScattering = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicScattering, "physCylinder", 0, false, 0, true);
	
	G4RotationMatrix *rot = new G4RotationMatrix;
	rot->rotateY(90*deg);
	
	new G4PVPlacement(rot, G4ThreeVector(-0.02025*m,0.,0.), logicScattering, "ScatteringDetector", logicWorld, false, 0); 
	
	G4VisAttributes* green = new G4VisAttributes(G4Colour(0/255., 255/255., 0/255.));
        
        green->SetVisibility(true);
        green->SetForceSolid(true);
       
        logicScattering->SetVisAttributes(green);
	
	//CVH100 Lens
	
	G4double iRadius = 0*m;
	G4double oRadius = 0.008*m;
	G4double cHeight = 0.00005*m;
	G4double sAngle = 0*deg;
	G4double spAngle = 360.*deg;
	
	G4Tubs *detectingLens = new G4Tubs("detectingLens", iRadius, oRadius, cHeight, sAngle, spAngle);
	
	logicLens = new G4LogicalVolume(detectingLens, worldMat, "logicLens");
	
	G4VPhysicalVolume *physLens = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicLens, "physLens", 0, false, 0, true);
	
	new G4PVPlacement(rot, G4ThreeVector(0.02480*m,0.,0.), logicLens, "DetectingLens", logicWorld, false, 0); 
       
	G4VisAttributes* quitewhite = new G4VisAttributes(G4Colour(255/255., 255/255., 255/255.));
        
        quitewhite->SetVisibility(true);
        quitewhite->SetForceSolid(true);
       
        logicLens->SetVisAttributes(quitewhite);
	
	return physWorld;
}

//Assigning Sensitive Detectors to Detector Logical Volumes

void MyDetectorConstruction::ConstructSDandField()
{
	MyLensDetector *lensDet = new MyLensDetector("LensDetector");
	
	logicLens->SetSensitiveDetector(lensDet);
}
