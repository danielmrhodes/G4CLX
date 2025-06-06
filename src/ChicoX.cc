#include "ChicoX.hh"

#include "G4SystemOfUnits.hh"
#include "G4PVPlacement.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4NistManager.hh"
//#include "G4RunManager.hh"
//#include "G4AssemblyVolume.hh"
#include "G4Trd.hh"
//#include "G4Cons.hh"
//#include "G4Sphere.hh"
//#include "G4SubtractionSolid.hh"

ChicoX::ChicoX() {;}
ChicoX::~ChicoX() {;}

void ChicoX::Placement(G4LogicalVolume* world, G4bool check) {
  
  //Visualization
  G4VisAttributes* vis1 = new G4VisAttributes(G4Color::Yellow());
  vis1->SetVisibility(true);
  vis1->SetForceSolid(true);

  //From CHICOX drawing
  G4double dx1 = 0.5*2.987*2.54*cm;
  G4double dx2 = 0.5*(1.077*2.54 + 0.2)*cm; //not precise
  G4double dy1 = 0.5*0.063*2.54*cm;
  G4double dy2 = dy1;
  G4double dz = 0.5*((5.787 - 0.377)*2.54 - 0.4)*cm; //not precise
  G4Trd* ppac_sol = new G4Trd("ppac_sol",dx1,dx2,dy1,dy2,dz);
  
  G4Material* g10 = G4Material::GetMaterial("G10");

  G4LogicalVolume* ppac_log = new G4LogicalVolume(ppac_sol,g10,"ppac_log");
  ppac_log->SetVisAttributes(vis1);

  G4int nSects = 10; //half of the array
  G4double dphi = 360.0*deg/G4double(nSects);

  G4double dist = 12.8*cm;
  G4double ang = 49.0*deg;
  G4double tilt = 90.0*deg - ang;
  
  G4double th1 = ang;
  for(int i=0;i<nSects;i++) { //Downstream
    
    G4double phi = i*dphi; 
    
    G4ThreeVector offset;
    offset.setRThetaPhi(dist,th1,phi);
    
    G4RotationMatrix* rotZ = new G4RotationMatrix(G4RotationMatrix::IDENTITY);
    rotZ->rotateZ(90.0*deg - phi);

    G4ThreeVector axis(0.0,1.0*mm,0.0); //y-axis
    axis.setPhi(axis.getPhi() + phi);

    G4RotationMatrix* rotA = new G4RotationMatrix(G4RotationMatrix::IDENTITY);
    rotA->rotate(tilt,axis);
    
    G4RotationMatrix* rot = new G4RotationMatrix((*rotZ)*(*rotA));

    G4int copy = i + 1;
    new G4PVPlacement(rot,offset,ppac_log,"PPAC",world,false,copy,check);
    
  }
  
  G4double th2 = 180.0*deg - ang;
  for(int i=0;i<nSects;i++) { //Upstream
    
    G4double phi = i*dphi; 
    
    G4ThreeVector offset;
    offset.setRThetaPhi(dist,th2,phi);
    
    G4RotationMatrix* rotZ = new G4RotationMatrix(G4RotationMatrix::IDENTITY);
    rotZ->rotateZ(90.0*deg - phi);

    G4RotationMatrix* rotX = new G4RotationMatrix(G4RotationMatrix::IDENTITY);
    rotX->rotateX(180*deg);

    G4ThreeVector axis(0.0,1.0*mm,0.0); //y-axis
    axis.setPhi(axis.getPhi() + phi);

    G4RotationMatrix* rotA = new G4RotationMatrix(G4RotationMatrix::IDENTITY);
    rotA->rotate(-tilt,axis);
    
    G4RotationMatrix* rot = new G4RotationMatrix((*rotX)*(*rotZ)*(*rotA));

    G4int copy = nSects + i + 1;
    new G4PVPlacement(rot,offset,ppac_log,"PPAC",world,false,copy,check);

  }
   
  return;
}

/*
void ChicoX::MakeG10() {
  
  G4NistManager* nist = G4NistManager::Instance();
  
  G4Element* H = nist->FindOrBuildElement("H");
  G4Element* C = nist->FindOrBuildElement("C");
  G4Element* O = nist->FindOrBuildElement("O");
  G4Element* Cl = nist->FindOrBuildElement("Cl");
  
  G4Material* Epoxy = new G4Material("Epoxy",1.3*g/cm3,3);
  Epoxy->AddElement(H,44);
  Epoxy->AddElement(C,15);
  Epoxy->AddElement(O,7);
  
  G4Material* GTen = new G4Material("G10",1.7*g/cm3,3);
  GTen->AddMaterial(nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE"),0.773);
  GTen->AddMaterial(Epoxy,0.147);
  GTen->AddElement(Cl,0.08);

  return;

}
*/
