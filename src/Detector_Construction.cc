#include "Detector_Construction.hh"

#include "SeGA.hh"
#include "S3.hh"
#include "Primary_Generator.hh"
#include "GammaSD.hh"
#include "IonSD.hh"

#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"

Detector_Construction::Detector_Construction() {

  messenger = new Detector_Construction_Messenger(this);

  SeGA_Offset = 0.0*mm;
  
  US_Offset = 3.0*cm;
  DS_Offset = 3.0*cm;
  
  target_Z = 82;
  target_N = 126;
  target_density = 11.382*g/cm3;
  target_mass = 207.97665*g/mole;
  target_thickness = 882*nm;
  target_radius = 0.5*cm;
  target_mat = NULL;
  target_step = 0.0*um;

  place_sega = false;
  place_s3 = false;
  place_target = false;
  check = false;
  
}

Detector_Construction::~Detector_Construction() {

  delete messenger;
  
}

G4VPhysicalVolume* Detector_Construction::Construct() {

  //NIST database for materials
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic"); //World material
  
  //Make the world
  G4Box* solid_world = new G4Box("World_Solid",2.0*m,2.0*m,2.0*m);
  logic_world = new G4LogicalVolume(solid_world,world_mat,"World_Logical");
  world = new G4PVPlacement(0,G4ThreeVector(),logic_world,"World",0,false,0,false);

  if(place_sega)
    PlaceSeGA();

  if(place_s3)
    PlaceS3();
  
  if(place_target)
    PlaceTarget();
  
  return world;
}

void Detector_Construction::ConstructSDandField() {
  
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  
  if(place_sega) {
    
    G4String gSDname = "GammaTracker";
    GammaSD* gSD = (GammaSD*)SDman->FindSensitiveDetector(gSDname,false);
    if(!gSD) {
      gSD = new GammaSD(gSDname);
      SDman->AddNewDetector(gSD);
    }

    for(G4int det=0;det<16;det++)
      for(G4int seg=0;seg<32;seg++)
	SetSensitiveDetector("GeLog" + std::to_string(100*(det + 1) + seg + 1),gSD);

  }
  
  if(place_s3) {

    G4String iSDname = "IonTracker";
    IonSD* iSD = (IonSD*)SDman->FindSensitiveDetector(iSDname,false);
    if(!iSD) {
      iSD = new IonSD(iSDname);
      SDman->AddNewDetector(iSD);
    }
    
    for(G4int det=0;det<2;det++)
      for(G4int ring=0;ring<24;ring++)
	for(G4int sec=0;sec<32;sec++)
	  SetSensitiveDetector("S3Log" + std::to_string(det*10000 + (ring+1)*100 + sec + 1),iSD);

  }
  
  return;
  
}

void Detector_Construction::PlaceSeGA() {

  SeGA* sega = new SeGA();
  sega->Placement(logic_world,SeGA_Offset,check);
  
  return;
}

void Detector_Construction::PlaceS3() {

  S3* s3 = new S3();
  s3->Placement(logic_world,US_Offset,DS_Offset,check);
  
  return;
}

void Detector_Construction::PlaceTarget() {

  /*
  //Target material (isotopically pure)
  target_mat = new G4Material("target_mat",target_density,1); //Bulk material
  G4Element* target_ele = new G4Element("target_ele","target_symbol",1); //Element
  G4Isotope* target_iso = new G4Isotope("target_iso",target_Z,target_N,target_mass); //Isotope
  target_ele->AddIsotope(target_iso,1.0);
  target_mat->AddElement(target_ele,1.0);
  */

  target_mat = new G4Material("target_mat",target_Z,target_Z+target_N,target_density);

  G4Tubs* solid_target = new G4Tubs("Target_Sol",0*cm,target_radius,
				    target_thickness/2.0,0.0*deg,360.0*deg);

  G4UserLimits* uLim = NULL;
  if(target_step > 0.0)
    uLim = new G4UserLimits(target_step);
  else
    uLim = new G4UserLimits(0.05*target_thickness);
  
  G4LogicalVolume* logic_target = new G4LogicalVolume(solid_target,target_mat,"Target_Logical",
						      0,0,uLim);

  //Visualization
  G4VisAttributes* vis1 = new G4VisAttributes(G4Colour::Cyan());
  vis1->SetVisibility(true);
  vis1->SetForceSolid(true);
  logic_target->SetVisAttributes(vis1);					      

  new G4PVPlacement(0,G4ThreeVector(),logic_target,"Target",logic_world,false,0,check);
  /*
  if(target_Z == 82) { //make backings for 208Pb target (here the C is upstream)
    
    G4double carbon_width = 0.1775*um;
    G4Tubs* carbonS = new G4Tubs("CarbonLayerS",0*cm,target_radius,carbon_width/2.0,0.0*deg,360.0*deg);

    G4NistManager* nist = G4NistManager::Instance();
    G4Material* carbon_mat = nist->FindOrBuildMaterial("G4_GRAPHITE");

    G4LogicalVolume* carbonL = new G4LogicalVolume(carbonS,carbon_mat,"CarbonLayerL",0,0,uLim);

    G4VisAttributes* vis2 = new G4VisAttributes(G4Colour::Grey());
    vis2->SetVisibility(true);
    //vis2->SetForceSolid(true);
    carbonL->SetVisAttributes(vis2);	

    new G4PVPlacement(0,G4ThreeVector(0,0,-(target_thickness+carbon_width)/2.0),carbonL,"CarbonLayer",
		      logic_world,false,0,check);

    G4double al_width = 0.0222*um;
    G4Tubs* alS = new G4Tubs("AlLayerS",0*cm,target_radius,al_width/2.0,0.0*deg,360.0*deg);

    G4Material* al_mat = nist->FindOrBuildMaterial("G4_Al");
    G4LogicalVolume* alL = new G4LogicalVolume(alS,al_mat,"AlLayerL",0,0,uLim);
    alL->SetVisAttributes(vis2);

    new G4PVPlacement(0,G4ThreeVector(0,0,(target_thickness+al_width)/2.0),alL,"AluminiumLayer",
		      logic_world,false,0,check);
    
  }
  */
  
  return;
}

void Detector_Construction::SetTarget(G4String target) {
  
  if(target == "48Ti" || target == "Ti48" || target == "ti48" || target == "48ti") {
    target_Z = 22;
    target_N = 26;
    target_density = 4.515*g/cm3;
    target_mass = 47.9475*g/mole;
    target_thickness = 2.20*um;
    target_radius = 0.5*cm;
  }
  else if(target == "208Pb" || target == "Pb208" || target == "pb208" || target == "208pb") {
    target_Z = 82;
    target_N = 126;
    target_density = 11.382*g/cm3;
    target_mass = 207.97665*g/mole;
    target_thickness = 882*nm;
    target_radius = 0.5*cm;
  }
  else if(target == "196Pt" || target == "Pt196" || target == "pt196" || target == "196pt") {
    target_Z = 78;
    target_N = 118;
    target_density = 21.547*g/cm3;
    target_mass = 195.9650*g/mole;
    target_thickness = 738*nm;
    target_radius = 0.5*cm;
  }
  else if(target == "194Pt" || target == "Pt194" || target == "pt194" || target == "194pt") {
    target_Z = 78;
    target_N = 116;
    target_density = 21.327*g/cm3;
    target_mass = 193.963*g/mole;
    target_thickness = 483*nm;
    target_radius = 0.5*cm;
  }
  else if(target == "110Pd" || target == "Pd110" || target == "pd110" || target == "110pd") {
    target_Z = 46;
    target_N = 64;
    target_density = 12.417*g/cm3;
    target_mass = 109.905*g/mole;
    target_thickness = 805*nm;
    target_radius = 0.5*cm;
  }
  else if(target == "197Au" || target == "Au197" || target == "au197" || target == "197au") {
    target_Z = 79;
    target_N = 118;
    target_density = 19.3*g/cm3;
    target_mass = 196.97*g/mole;
    target_thickness = 984*nm;
    target_radius = 0.5*cm;
  }
  else
    G4cout << "\033[1;31mUnrecognized target " << target << ". Defaulting to Pb208\033[m" << G4endl;

  PrintTarget();
  
  return;
}

void Detector_Construction::PrintTarget() {

  G4cout << "\t Z: " << target_Z << "\n\t N: " << target_N
	 << "\n\t Molar Mass: " << G4BestUnit(target_mass,"Mass")
	 <<  "\n\t Density: "<< G4BestUnit(target_density,"Volumic Mass")
	 << "\n\t Thickness: " << G4BestUnit(target_thickness,"Length")
	 << "\n\t Radius: " << G4BestUnit(target_radius,"Length")
	 << G4endl;

  return;
}
