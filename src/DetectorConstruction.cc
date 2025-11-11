#include "DetectorConstruction.hh"

DetectorConstruction::DetectorConstruction() {

  shellStatus = "";
  northOffset = 0;
  southOffset = 0;
  
  gretinaStatus = true;
  s3Status = false;
  chicoXStatus = false;
  laBrStatus  = false;

  targetStatus = false;
  beamTubeStatus = false;
  gretaChamberStatus = false;
  WUChamberStatus = false;
  check = false;

  US_Offset = 3.0*cm;
  DS_Offset = 3.0*cm;
  
  target_Z = 82;
  target_A = 208;
  target_density = 11.382*g/cm3;
  target_mass = 207.97665*g/mole;
  target_thickness = 882*nm;
  target_radius = 0.5*cm;
  target_mat = NULL;
  target_step = 0.0*um;

  DefineMaterials();

  myMessenger = new DetectorConstruction_Messenger(this);

}

DetectorConstruction::~DetectorConstruction() {
  delete myMessenger;
}

G4VPhysicalVolume* DetectorConstruction::Construct() {
  
  G4double expHall_x = 100.*cm;
  G4double expHall_y = 100.*cm;
  G4double expHall_z = 100.*cm;
  G4Box* ExpHall_sol = new G4Box("expHall_box",expHall_x,expHall_y,expHall_z);

  G4Material* expHall_mat = G4Material::GetMaterial("G4_Galactic");

  //G4UserLimits* ulim = new G4UserLimits(1.0*cm);
  ExpHall_log = new G4LogicalVolume(ExpHall_sol,expHall_mat,"expHall_log",0,0,0);
  ExpHall_phys = new G4PVPlacement(0,G4ThreeVector(),ExpHall_log,"expHall",0,false,0);
  
  if(targetStatus)
    PlaceTarget();

  if(beamTubeStatus) {
    Beam_Tube* bt = new Beam_Tube();
    bt->Construct(ExpHall_log);
  }
  
  if(gretaChamberStatus) {
    Greta_Chamber* gc = new Greta_Chamber();
    gc->Construct(ExpHall_log);
  }
  
  if(WUChamberStatus) {
    WU_Chamber* wc = new WU_Chamber();
    wc->Construct(ExpHall_log);
  }
  
  if(laBrStatus) {
    LaBr* lb = new LaBr();
    lb->Construct(ExpHall_log);
  }
  
  if(s3Status) {
    S3* s3 = new S3();
    s3->Placement(ExpHall_log,US_Offset,DS_Offset,check);
  }

  if(chicoXStatus) {
    ChicoX* cx = new ChicoX();
    cx->Placement(ExpHall_log,check);
  }

  if(shellStatus == "full"  || shellStatus == "north" || shellStatus == "south") {
    Gretina_NSCL_Shell* Shell = new Gretina_NSCL_Shell();
    Shell->setNorthOffset(northOffset);
    Shell->setSouthOffset(southOffset);
    Shell->Placement(shellStatus);
  }
  else if(shellStatus == "Greta" || shellStatus == "GretaLH" || shellStatus == "Greta_North" || shellStatus == "Greta_South" || shellStatus == "GretaLH_North" ||
	  shellStatus == "GretaLH_South" ) {
    Greta_Shell* Shell = new Greta_Shell();
    Shell->setNorthOffset(northOffset);
    Shell->setSouthOffset(southOffset);
    Shell->Placement(shellStatus);
  }

  if(gretinaStatus) {
    the_Gretina_Array = new Gretina_Array();
    the_Gretina_Array->SetNorthOffset(northOffset);
    the_Gretina_Array->SetSouthOffset(southOffset);
    the_Gretina_Array->Placement();
  }

  return ExpHall_phys;
}

void DetectorConstruction::ConstructSDandField() {
  
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  
  if(gretinaStatus) {
    
    G4String gSDname = "GammaTracker";
    GammaSD* gSD = (GammaSD*)SDman->FindSensitiveDetector(gSDname,false);
    if(!gSD) {
      gSD = new GammaSD(gSDname);
      SDman->AddNewDetector(gSD);
    }

    char sName[50];
    for(G4int i=0;i<2;i++) {
      sprintf(sName,"geDetCapsL%2.2d",i);
      SetSensitiveDetector(sName,gSD);
    }
  }
  
  if(s3Status) {
    
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

  if(chicoXStatus) {
    
    G4String pSDname = "PPAC_Tracker";
    PPAC_SD* pSD = (PPAC_SD*)SDman->FindSensitiveDetector(pSDname,false);
    if(!pSD) {
      pSD = new PPAC_SD(pSDname);
      SDman->AddNewDetector(pSD);
    }
    
    SetSensitiveDetector("ppac_log",pSD);
  }
  
  return;
}

void DetectorConstruction::PlaceTarget() {

  if(target_Z != 90) {
    //Target material (isotopically pure)
    target_mat = new G4Material("target_mat",target_density,1); //Bulk material
    G4Element* target_ele = new G4Element("target_ele","target_symbol",1); //Element
    G4Isotope* target_iso = new G4Isotope("target_iso",target_Z,target_A,target_mass); //Isotope
    target_ele->AddIsotope(target_iso,1.0);
    target_mat->AddElement(target_ele,1.0);
  }
  else { //ThF4 target
    G4NistManager* man = G4NistManager::Instance();
    G4Element* F = man->FindOrBuildElement("F");
    G4Element* Th = man->FindOrBuildElement("Th");

    target_mat = new G4Material("target_mat",target_density,2);
    target_mat->AddElement(Th,1);
    target_mat->AddElement(F,4);
  }

  G4Tubs* solid_target = new G4Tubs("Target_Sol",0*cm,target_radius,
				    target_thickness/2.0,0.0*deg,360.0*deg);

  G4UserLimits* uLim = NULL;
  if(target_step > 0.0)
    uLim = new G4UserLimits(target_step);
  else
    uLim = new G4UserLimits(0.2*target_thickness);
  
  G4LogicalVolume* logic_target = new G4LogicalVolume(solid_target,target_mat,"Target_Logical",0,0,uLim);

  //Visualization
  G4VisAttributes* vis1 = new G4VisAttributes(G4Colour::Cyan());
  vis1->SetVisibility(true);
  vis1->SetForceSolid(true);
  logic_target->SetVisAttributes(vis1);					      

  new G4PVPlacement(0,G4ThreeVector(),logic_target,"Target",ExpHall_log,false,0,check);

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
		      ExpHall_log,false,0,check);

    G4double al_width = 0.0222*um;
    G4Tubs* alS = new G4Tubs("AlLayerS",0*cm,target_radius,al_width/2.0,0.0*deg,360.0*deg);

    G4Material* al_mat = nist->FindOrBuildMaterial("G4_Al");
    G4LogicalVolume* alL = new G4LogicalVolume(alS,al_mat,"AlLayerL",0,0,uLim);
    alL->SetVisAttributes(vis2);

    new G4PVPlacement(0,G4ThreeVector(0,0,(target_thickness+al_width)/2.0),alL,"AluminiumLayer",
		      ExpHall_log,false,0,check);
    
  }
  */
  
  return;

}

void DetectorConstruction::SetTarget(G4String target) {
  
  if(target == "48Ti" || target == "Ti48" || target == "ti48" || target == "48ti") {
    target_Z = 22;
    target_A = 48;
    target_density = 4.515*g/cm3;
    target_mass = 47.9475*g/mole;
    target_thickness = 2.20*um;
    target_radius = 0.5*cm;
  }
  else if(target == "208Pb" || target == "Pb208" || target == "pb208" || target == "208pb") {
    target_Z = 82;
    target_A = 208;
    target_density = 11.382*g/cm3;
    target_mass = 207.97665*g/mole;
    target_thickness = 882*nm;
    target_radius = 0.5*cm;
  }
  else if(target == "196Pt" || target == "Pt196" || target == "pt196" || target == "196pt") {
    target_Z = 78;
    target_A = 196;
    target_density = 21.547*g/cm3;
    target_mass = 195.9650*g/mole;
    target_thickness = 738*nm;
    target_radius = 0.5*cm;
  }
  else if(target == "194Pt" || target == "Pt194" || target == "pt194" || target == "194pt") {
    target_Z = 78;
    target_A = 194;
    target_density = 21.327*g/cm3;
    target_mass = 193.963*g/mole;
    target_thickness = 483*nm;
    target_radius = 0.5*cm;
  }
  else if(target == "110Pd" || target == "Pd110" || target == "pd110" || target == "110pd") {
    target_Z = 46;
    target_A = 110;
    target_density = 12.417*g/cm3;
    target_mass = 109.905*g/mole;
    target_thickness = 805*nm;
    target_radius = 0.5*cm;
  }
  else if(target == "197Au" || target == "Au197" || target == "au197" || target == "197au") {
    target_Z = 79;
    target_A = 197;
    target_density = 19.3*g/cm3;
    target_mass = 196.97*g/mole;
    target_thickness = 984*nm;
    target_radius = 0.5*cm;
  }
  else if(target == "232Th" || target == "Th232" || target == "Th" || target == "ThF4" || target == "ThF") {
    target_Z = 90; //ThF4 target made later
    target_A = 232;
    target_density = 6.3*g/cm3;
    target_mass = 308.03*g/mole;
    target_thickness = 635*nm;
    target_radius = 0.5*cm;
  }	
  else
    std::cout << "\033[1;31mUnrecognized target " << target << ". Defaulting to Pb208\033[m" << std::endl;

  PrintTarget();
  
  return;
}

void DetectorConstruction::PrintTarget() {

  std::cout << "\t Z: " << target_Z << "\n\t A: " << target_A
	    << "\n\t Molar Mass: " << G4BestUnit(target_mass,"Mass")
	    <<  "\n\t Density: "<< G4BestUnit(target_density,"Volumic Mass")
	    << "\n\t Thickness: " << G4BestUnit(target_thickness,"Length")
	    << "\n\t Radius: " << G4BestUnit(target_radius,"Length")
	    << std::endl;

  return;
}

void DetectorConstruction::DefineMaterials() {
  
  // Elements
  G4Element* elementH  = new G4Element("Hydrogen","H",1.,1.0079*g/mole);
  G4Element* elementD  = new G4Element("Deuterium","D",1.,2.0141*g/mole);
  G4Element* elementC  = new G4Element("Carbon","C",6.,12.011*g/mole);
  G4Element* elementN  = new G4Element("Nitrogen","N",7.,14.00674*g/mole);
  G4Element* elementO  = new G4Element("Oxygen","O",8.,15.9994*g/mole);
  //G4Element* elementF  = new G4Element("Flourine","F",9.,18.998*g/mole);
  G4Element* elementMg = new G4Element("Magnesium", "Mg", 12., 24.3050*g/mole);
  //G4Element* elementAl = new G4Element("Aluminum","Al",13.,26.9815*g/mole);
  G4Element* elementSi = new G4Element("Silicon","Si",14.,28.0855*g/mole);
  G4Element* elementP = new G4Element("Phosphorous","P",15.,30.974*g/mole);
  G4Element* elementS = new G4Element("Sulphur","S",16,32.065*g/mole);
  //G4Element* elementTi = new G4Element("Titanium","Ti",22.,47.90*g/mole);
  //G4Element* elementV  = new G4Element("Vanadium","V",23.,50.9415*g/mole);
  G4Element* elementCr = new G4Element("Chromium","Cr",24.,51.996*g/mole);
  G4Element* elementFe = new G4Element("Iron","Fe",26.,55.845*g/mole);
  G4Element* elementCo = new G4Element("Cobalt","Co",27.,58.9332*g/mole);
  G4Element* elementNi = new G4Element("Nickel","Ni",28.,58.6934*g/mole);
  G4Element* elementCu = new G4Element("Copper","Cu",29.,63.55*g/mole);
  G4Element* elementBr = new G4Element("Bromine","Br",35.,79.904 *g/mole);
  //G4Element* elementMo = new G4Element("Molybdenum","Mo",42.,95.94*g/mole);
  //G4Element* elementSn = new G4Element("Tin","Sn", 50.,118.710*g/mole);
  G4Element* elementLa = new G4Element("Lanthanum","La",57.,138.90547*g/mole);
  //G4Element* elementTa = new G4Element("Tantalum","Ta",73.,180.94788*g/mole);
  G4Element* elementW  = new G4Element("Tungsten","W",74.,183.84*g/mole);
  //G4Element* elementPt = new G4Element("Platinum","Pt",78.,195.08*g/mole);
  //G4Element* elementAu = new G4Element("Gold","Au",79.,196.97*g/mole);
  //G4Element* elementPb = new G4Element("Lead","Pb",82.,207.2*g/mole);
  //G4Element* elementBi = new G4Element("Bismuth","Bi",83.,208.98*g/mole);
  //G4Element* elementTh  = new G4Element("Thorium","Th",90.,232.038*g/mole);

  // Germanium isotopes
  G4Isotope* Ge70 = new G4Isotope("Ge70",32,70,69.9242*g/mole);
  G4Isotope* Ge72 = new G4Isotope("Ge72",32,72,71.9221*g/mole);
  G4Isotope* Ge73 = new G4Isotope("Ge73",32,73,72.9235*g/mole);
  G4Isotope* Ge74 = new G4Isotope("Ge74",32,74,73.9212*g/mole);
  G4Isotope* Ge76 = new G4Isotope("Ge76",32,76,75.9214*g/mole);

  // Germanium defined via its isotopes
  G4Element* elGe = new G4Element("Germanium", "Ge", 5);
  elGe->AddIsotope(Ge70,0.2057);
  elGe->AddIsotope(Ge72,0.2745);
  elGe->AddIsotope(Ge73,0.0775);
  elGe->AddIsotope(Ge74,0.3650);
  elGe->AddIsotope(Ge76,0.0773);

  // Germanium without 74Ge
  //G4Element* elGe = new G4Element("Germanium","Ge",4);
  //elGe->AddIsotope(Ge70,0.3239);
  //elGe->AddIsotope(Ge72,0.4323);
  //elGe->AddIsotope(Ge73,0.1220);
  //elGe->AddIsotope(Ge76,0.1217);
  
  //Materials
  G4Material* CD2 = new G4Material("CD2",1.08*g/cm3,2);
  CD2->AddElement(elementC,1);
  CD2->AddElement(elementD,2);

  G4Material* G10 = new G4Material("G10",1.70*g/cm3,4);
  G10->AddElement(elementSi,1);
  G10->AddElement(elementO,2);
  G10->AddElement(elementC,3);
  G10->AddElement(elementH,3);
  
  //G4Material* HpGe = new G4Material("HpGe",32.,72.61*g/mole,5.323*g/cm3);
  G4Material* Ge = new G4Material("Germanium",5.323*g/cm3,1);
  Ge->AddElement(elGe,1);

  G4Material* Hevimet = new G4Material("Hevimet",17.0*g/cm3,3);
  Hevimet->AddElement(elementW,0.90);
  Hevimet->AddElement(elementNi,0.06);
  Hevimet->AddElement(elementCu,0.04);

  G4Material* kapton = new G4Material("Kapton",1.42*g/cm3,4);
  kapton->AddElement(elementC,0.691099);
  kapton->AddElement(elementN,0.073298);
  kapton->AddElement(elementO,0.209424);
  kapton->AddElement(elementH,0.026178);

  G4Material* mylar = new G4Material("Mylar", 1.39*g/cm3, 3);
  mylar->AddElement(elementC,10);
  mylar->AddElement(elementH,8);
  mylar->AddElement(elementO,4);

  G4Material* LaBr3 = new G4Material("LaBr3",5.08*g/cm3,2);
  LaBr3->AddElement(elementLa,0.25);
  LaBr3->AddElement(elementBr,0.75);

  new G4Material("preampMat",13.,26.982*g/mole,1.35*g/cm3); //LR  (Air, Cu, and Al?)

  G4Material* Steel = new G4Material("Steel",7.86*g/cm3,2);
  Steel->AddElement(elementFe,98.5*perCent);
  Steel->AddElement(elementC,1.5*perCent);
  
  G4Material* ssteel = new G4Material("ssteel",7.7*g/cm3,3);
  ssteel->AddElement(elementC,0.04);
  ssteel->AddElement(elementFe,0.88);
  ssteel->AddElement(elementCo,0.08);

  G4Material* stainless = new G4Material("Stainless", 8.0*g/cm3, 7);
  stainless->AddElement(elementFe,0.705);
  stainless->AddElement(elementCr,0.18);
  stainless->AddElement(elementNi,0.09);
  stainless->AddElement(elementMg,0.02);
  stainless->AddElement(elementP,0.002);
  stainless->AddElement(elementC,0.0015);
  stainless->AddElement(elementS,0.0015);

  new G4Material("Be",4.,9.012182*g/mole,1.84*g/cm3);
  new G4Material("C",6.,12.011*g/mole,2.15*g/cm3);
  new G4Material("glassyC",6.,12.011*g/mole,1.54*g/cm3);
  new G4Material("Al",13.,26.98154*g/mole,2.70*g/cm3);
  new G4Material("Si",14.,28.0855*g/mole,2.33*g/cm3);
  new G4Material("Ti",22.,47.90*g/mole,4.506*g/cm3);
  new G4Material("Fe",26.,55.85*g/mole,7.87*g/cm3);
  new G4Material("Cu",29.,63.55*g/mole,8.96*g/cm3);
  new G4Material("Nb",41.,92.90638*g/mole,8.57*g/cm3);
  new G4Material("Au",79.,196.9*g/mole,19.32*g/cm3);   
  new G4Material("Ni",28.,58.6934*g/mole,8.908*g/cm3); 
  new G4Material("Sn",50.,118.710*g/mole,7.365*g/cm3);
  new G4Material("Ir",77.,192.217*g/mole,22.65*g/cm3);  
  new G4Material("Pb",82.,207.2*g/mole,11.34*g/cm3);
  new G4Material("Bi",83.,208.98*g/mole,9.747*g/cm3);
  
  // define materials from the G4 NIST database
  G4NistManager* NISTman = G4NistManager::Instance();

  NISTman->FindOrBuildMaterial("G4_CESIUM_IODIDE");
  NISTman->FindOrBuildMaterial("G4_MAGNESIUM_OXIDE");
  NISTman->FindOrBuildMaterial("G4_Galactic");
  NISTman->FindOrBuildMaterial("G4_POLYETHYLENE");
  NISTman->FindOrBuildMaterial("G4_POLYPROPYLENE");
  NISTman->FindOrBuildMaterial("G4_TEFLON");
  NISTman->FindOrBuildMaterial("G4_lH2");
  NISTman->FindOrBuildMaterial("G4_CONCRETE");
  NISTman->FindOrBuildMaterial("G4_AIR");
  NISTman->FindOrBuildMaterial("G4_Mg");
  
  return;
  
}
