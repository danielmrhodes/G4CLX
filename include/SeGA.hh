#ifndef SeGA_h
#define SeGA_h 1

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4RotationMatrix.hh"
#include "globals.hh"

class SeGA {

public:

  SeGA();
  ~SeGA();
  
  void Placement(G4LogicalVolume* world, G4double zoff, G4bool check);

private:

  void PlaceDetector(G4LogicalVolume* world, G4int det, G4double zoff, G4bool check);
  
  G4Material*  HpGe;
  G4Material* Al;

  G4int phisegs, zsegs;

  G4double Length, outerRadius, fingerRadius;
  G4double DLinnerRadius, DLouterRadius;
  G4double iCanOuterRadius, iCanInnerRadius, iCanLength;
  G4double oCanThickness, oCanOuterRadius, oCanInnerRadius, oCanLength;
  G4double preampRadius, preampLength;
  G4double neckRadius, neckLength;
  G4double cryoThickness, cryoOuterRadius, cryoInnerRadius, cryoBaseThickness, cryoLength;

  G4ThreeVector cryoBaseOffset, cryoOffset, neckOffset, oCanOffset, preampOffset;

  G4VisAttributes* Vis1;
  G4VisAttributes* Vis2;
  G4VisAttributes* Vis3;
  G4VisAttributes* Vis4;
  G4VisAttributes* Vis5;
  
};

#endif

