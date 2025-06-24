#include "ChicoX.hh"

#include "G4SystemOfUnits.hh"
#include "G4PVPlacement.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4NistManager.hh"
//#include "G4RunManager.hh"
//#include "G4AssemblyVolume.hh"
#include "G4Trd.hh"
#include "G4Polyhedra.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Sphere.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4MultiUnion.hh"

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
   
  MakeChamber(world,check);

  return;
}

void ChicoX::MakeChamber(G4LogicalVolume* world,  G4bool check) {

  G4double inch = 25.4*mm;
  G4double rTarRingInner = 5.73*inch;
  G4double rTarRingOuter = 6.267*inch;
  G4double halfZTarRing = 0.1875*inch;

  // Rails
  G4double rRailOuter = (0.125/2.)*inch;
  G4double halfZRail = 5.55*inch;
  G4RotationMatrix alignY = G4RotationMatrix();
  alignY.rotateX(90.*deg);
  G4double railXOffset = 1.0315*inch;

  // L shape holder (subdivide into boxes 1-5)
  // Box 1: long side, 2: short side, 3: small bit that touches the rail on box 1, 4: the small bit at the base of the L, 5: the small bit that touches the rail on box 2.
  G4double halfBox1Z = (5.5/2.)*inch;
  G4double halfBox1X = (0.125/2.)*inch;
  G4double halfBox1Y = (0.188/2.)*inch;
  G4double holderY = 0.*inch; // Change this to move the L piece up and down along Y.
  G4ThreeVector posBox1(-railXOffset + halfBox1Y + 0.125*inch, holderY , 0); // not as offset from centre as the rail
  G4RotationMatrix alignYSpinOwnAxis = G4RotationMatrix();
  alignYSpinOwnAxis.rotateX(90.*deg);
  alignYSpinOwnAxis.rotateY(90.*deg);

  // Box 2
  G4double halfBox2Z = (1.81/2.)*inch;
  G4double halfBox2X = halfBox1X;
  G4double halfBox2Y = halfBox1Y;
  G4ThreeVector posBox2(0, holderY - halfBox1Z + halfBox2Y, 0);
  G4RotationMatrix alignXSpinOwnAxis = G4RotationMatrix();
  alignXSpinOwnAxis.rotateY(90.*deg);

  // Box 3, 4, 5
  G4double halfBox3Z = (0.188/2.)*inch;
  G4double halfBox3Y = 0.125*inch;
  G4double halfBox3X = halfBox3Y;
  G4ThreeVector posBox3(-railXOffset, holderY + halfBox1Z - halfBox3Z, 0);
  G4ThreeVector posBox4(-railXOffset, holderY - halfBox1Z + halfBox3Z, 0);
  G4ThreeVector posBox5(+railXOffset, holderY - halfBox1Z + halfBox3Z, 0);

  // Box 6 = foil holder (box with poly cutout)
  G4double halfBox6X = (1.25/2.)*inch;
  G4double halfBox6Y = 0.2*inch;
  G4double halfBox6Z = 0.0125*inch;
  G4double box6XCutout = 0.29*inch;
  //G4double box6CutCentre = halfBox6X - 0.96*inch;
  G4double rBox6Cutout = (0.375/2.)*inch;
  G4double foilY = -0.0*inch;

  G4double box6PolyZ[] = {-0.1*inch, 0.1*inch};
  G4double box6PolyR0[] = {0, 0};
  G4double box6PolyR1[] = {rBox6Cutout, rBox6Cutout};

  // -----------
  // Hemisphere
  // -----------

  // Hemisphere to target ring cylinder (Cyl 1)
  G4double rCyl1Inner = 6.1952*inch;
  G4double rCyl1Outer = 6.2552*inch;
  G4double halfZCyl1 = 0.2155*inch;
  G4ThreeVector posCyl1(0, 0, -halfZTarRing -halfZCyl1);

  // Cylinder inside cyl1 (Cyl 2)
  G4double rCyl2Inner = 5.7602*inch;
  G4double rCyl2Outer = 6.1952*inch;
  G4double halfZCyl2 = 0.136*inch;
  G4ThreeVector posCyl2(0, 0, -halfZTarRing -(0.1*inch) -halfZCyl2);

  // Cone that forms 45deg chamfer to connect cyl 2 to hemisphere (Con 1)
  G4double rCon1Inner1 = 6.1*inch; // Can't be the 6.1952 it should be as it makes the edge impossibly thin
  G4double rCon1Inner2 = rCyl2Inner;
  G4double rCon1Outer = rCon1Inner1; // Both outer R's are the same
  G4double halfZCon1 = 0.2175*inch;
  G4ThreeVector posCon1(0, 0, -halfZTarRing -(0.1*inch) -2*halfZCyl2 -halfZCon1);

  // Hemisphere
  G4double rHemSphInner = 6.21*inch;
  G4double rHemSphOuter = 6.27*inch;
  G4ThreeVector posHemSph(0, 0, halfZTarRing);

  // Cylinder cutout at the bottom of the sphere (Cyl 3)
  // No inner radius
  G4double rCyl3Outer = 1.4615*inch;
  G4double zBaseCyl3 = -6.09729*inch; // Where the bottom surface of cyl 3 should be, excluding the extra 0.5 inches on the bottom
  G4double halfZCyl3 = (0.0415+0.25)*inch; // need plenty of extra cutout along Z to make sure the bottom of the sph is pierced
  G4ThreeVector posCyl3(0, 0, zBaseCyl3 + (2*0.0415*inch) - 0.0415*inch - 0.25*inch + (halfZTarRing)); // Extra term on the end as the subtraction is made with the sphere at the origin: it hasn't been moved behind the target ring yet

  // Cylinder cutout above Cyl 3 (Cyl 4)
  // No inner radius
  G4double rCyl4Outer = 0.8865*inch;
  G4double halfZCyl4 = (0.025)*inch; // need plenty of extra cutout along Z to make sure the bottom of the sph is pierced
  G4ThreeVector posCyl4(0, 0, zBaseCyl3 + (2*0.0415*inch) + halfZCyl4 + (halfZTarRing)); // Extra term on the end as the subtraction is made with the sphere at the origin: it hasn't been moved behind the target ring yet

  // Cone that fills the bottom of the hemisphere (Con 2)
  G4double rCon2Inner1 = 0.7749*inch;
  G4double rCon2Inner2 = 1.1508*inch;
  G4double rCon2Outer = 2.327*inch; // Both outer R's are the same
  G4double halfZCon2 = 0.224*inch;
  G4ThreeVector posCon2(0, 0, zBaseCyl3 + 2*halfZCon2 + 0.05*inch); // This volume is added, so it needs to go in its final position (zBaseCyl3 includes Z of target ring)

  // Vectors for subtraction of Cyl 3, 4 from Con 2 (Centre of Con 2 will be at origin)
  G4ThreeVector posCyl3Subtraction(0, 0, -halfZCon2 + (2*0.0415*inch) - 0.0415*inch - 0.25*inch); // Have to take into account the extra lump on the bottom of cyl 3
  G4ThreeVector posCyl4Subtraction(0, 0, -halfZCon2 + 2*halfZCyl3 + halfZCyl4);

  // Sphere of atmospheric air who's inner surface matches the outer surface of the hemisphere (AirSph)
  G4ThreeVector posAirSphSubtraction(0, 0, -zBaseCyl3 -halfZCon2);

  // -------------
  // Window frame
  // -------------
  // Window Con 1 (starting from narrow side)
  G4double rWinCon1Inner = (1.244/2.)*inch;
  G4double rWinCon1Outer1 = 0.75*inch;
  G4double rWinCon1Outer2 = 1.6759*inch;
  G4double halfZWinCon1 = (0.337/2.)*inch;
  G4double zWinCon1Base = 4.992*inch;
  G4ThreeVector posWinCon1(0, 0, -(zWinCon1Base + halfZWinCon1));

  // Win Con 2 (below Con1)
  G4double rWinCon2Inner1 = 1.22*inch;
  G4double rWinCon2Inner2 = 1.82829*inch;
  G4double rWinCon2Outer1 = 1.6759*inch;
  G4double rWinCon2Outer2 = 1.87584*inch;
  G4double halfZWinCon2 = (0.23/2.)*inch;
  G4ThreeVector posWinCon2(0, 0, -zWinCon1Base + halfZWinCon2);

  // Win Con 3 (main conic part of window)
  G4double rWinCon3Inner1 = rWinCon2Inner2;
  G4double rWinCon3Inner2 = 5.5725*inch;
  G4double rWinCon3Outer1 = rWinCon2Outer2;
  G4double rWinCon3Outer2 = 5.703*inch;
  G4double halfZWinCon3 = (4.347/2.)*inch;
  G4double zWinCon3Base = 0.415*inch;
  G4ThreeVector posWinCon3(0, 0, -zWinCon3Base - halfZWinCon3);

  // Win Cyl 1 (below Con3)
  G4double rWinCyl1Inner = 5.5725*inch;
  G4double rWinCyl1Outer = 5.703*inch;
  G4double halfZWinCyl1 = (0.315/2.)*inch;
  G4ThreeVector posWinCyl1(0, 0, -zWinCon3Base + halfZWinCyl1);

  // Win Cyl 2 (below Cyl1)
  G4double rWinCyl2Inner = 5.5725*inch;
  G4double rWinCyl2Outer = 6.14*inch;
  G4double halfZWinCyl2 = 0.05*inch;
  G4ThreeVector posWinCyl2(0, 0, -halfZWinCyl2);

  // Mylar window (basically Con3 + 1 um)
  // Cutouts in the aluminium lampshade (Con3)
  G4double halfZCutout = 0.5*1.9985*inch;
  G4double spokePhi = 4.*deg;
  G4double cutoutPhi = 32.*deg;
  G4double zGapToGap = 0.1*inch;

  // ---------------
  // Target chamber
  // ---------------
  G4Tubs* solidTarRing = new G4Tubs("TargetRing", rTarRingInner, rTarRingOuter, halfZTarRing, 0, 360.*deg);

  // Cylindrical rails that the chamber sits on
  G4Tubs* solidRail = new G4Tubs("Rail", 0, rRailOuter, halfZRail, 0, 360.*deg);

  // Boxes that make the L shape holder
  G4Box* solidBox1 = new G4Box("Box1", halfBox1X, halfBox1Y, halfBox1Z);
  G4Box* solidBox2 = new G4Box("Box2", halfBox2X, halfBox2Y, halfBox2Z);
  G4Box* solidBox3 = new G4Box("Box3", halfBox3X, halfBox3Y, halfBox3Z);
  G4Box* solidBox4 = new G4Box("Box4", halfBox3X, halfBox3Y, halfBox3Z);
  G4Box* solidBox5 = new G4Box("Box5", halfBox3X, halfBox3Y, halfBox3Z);

  // Box 6 (box - cyl: where the foil goes)
  G4Box* solidBox6 = new G4Box("Box6", halfBox6X, halfBox6Y, halfBox6Z);
  G4Polyhedra* solidBox6Poly = new G4Polyhedra("B6Cut", 0, 360.*deg, 36, 2, box6PolyZ, box6PolyR0, box6PolyR1); // Polyhedra over Tubs as the verticies to draw the tubs are too small = crash
  G4SubtractionSolid* solidBox6Cutout = new G4SubtractionSolid("Box6Cutout", solidBox6, solidBox6Poly, 0, G4ThreeVector(box6XCutout, 0, 0));

  G4MultiUnion* solidTarChamber = new G4MultiUnion("TargetChamber");
  solidTarChamber->AddNode(solidTarRing, G4Transform3D());
  // Rails
  solidTarChamber->AddNode(solidRail, G4Transform3D(alignY, G4ThreeVector(railXOffset, 0, 0)));
  solidTarChamber->AddNode(solidRail, G4Transform3D(alignY, G4ThreeVector(-railXOffset, 0, 0)));
  // Boxes
  solidTarChamber->AddNode(solidBox1, G4Transform3D(alignYSpinOwnAxis, posBox1));
  solidTarChamber->AddNode(solidBox2, G4Transform3D(alignXSpinOwnAxis, posBox2));
  solidTarChamber->AddNode(solidBox3, G4Transform3D(alignYSpinOwnAxis, posBox3));
  solidTarChamber->AddNode(solidBox4, G4Transform3D(alignYSpinOwnAxis, posBox4));
  solidTarChamber->AddNode(solidBox5, G4Transform3D(alignYSpinOwnAxis, posBox5));
  // Foil holder
  solidTarChamber->AddNode(solidBox6Cutout, G4Transform3D(G4RotationMatrix(), G4ThreeVector(-box6XCutout, holderY + foilY, 0)));
  solidTarChamber->Voxelize();

  G4Material* stainless = G4Material::GetMaterial("Stainless");
  G4LogicalVolume* logicTarChamber = new G4LogicalVolume(solidTarChamber, stainless, "TargetChamber");
  new G4PVPlacement(0, G4ThreeVector(), logicTarChamber, "TargetChamber", world, 0, 0, check);
  
  // ----------------------------
  // Upstream CHICO-X Hemisphere
  // ----------------------------

  // Hemisphere to target ring cylinder (Cyl 1) 
  G4Tubs* solidCyl1 = new G4Tubs("Cyl1", rCyl1Inner, rCyl1Outer, halfZCyl1, 0, 360.*deg);

  // Cylinder that sits inside cyl1 and forms part of the vacuum seal with the target ring (Cyl 2)
  G4Tubs* solidCyl2 = new G4Tubs("Cyl2", rCyl2Inner, rCyl2Outer, halfZCyl2, 0, 360.*deg);

  // Cone connecting Cyl 2 to the hemisphere (Con 1)
  G4Cons* solidCon1 = new G4Cons("Con1", rCon1Inner1, rCon1Outer, rCon1Inner2, rCon1Outer, halfZCon1, 0, 360.*deg); // Args: RMin1, RMax1, RMin2, RMax2, HalfZ, start ang, ang range

  // Hemisphere
  G4Sphere* solidHemSph = new G4Sphere("HemiSphere", rHemSphInner, rHemSphOuter, 0, 360*deg, (180.-86.0625)*deg, 86.0625*deg);

  // Cylinder cutout at the bottom of the hemisphere (Cyl 3)
  G4Tubs* solidCyl3 = new G4Tubs("Cyl3", 0, rCyl3Outer, halfZCyl3, 0, 360.*deg);

  // Cylinder cutout above Cyl 3 (Cyl 4)
  G4Tubs* solidCyl4 = new G4Tubs("Cyl4", 0, rCyl4Outer, halfZCyl4, 0, 360.*deg);

  // Cone filling to add to the bottom of the sphere (Con 2)
  G4Cons* solidCon2 = new G4Cons("Con2", rCon2Inner1, rCon2Outer, rCon2Inner2, rCon2Outer, halfZCon2, 0, 360.*deg);

  // Sph of atmospheric air that matches the outer surface of the hemispheres (AirSph)
  G4Sphere* solidAirSph = new G4Sphere("HemiSphere", rHemSphOuter, rHemSphOuter+(6*inch), 0, 360*deg, (180.-86.0625)*deg, 86.0625*deg);

  // Con 2 - Cyl 3 = Con2Cut1
  G4SubtractionSolid* solidCon2Cut1 = new G4SubtractionSolid("Con2Cut1", solidCon2, solidCyl3, 0, posCyl3Subtraction);

  // Con2Cut1 - Cyl4 = Con2Cut2
  G4SubtractionSolid* solidCon2Cut2 = new G4SubtractionSolid("Con2Cut2", solidCon2Cut1, solidCyl4, 0, posCyl4Subtraction);

  // Con2Cut2 - AirSph = Con2Cut3
  G4SubtractionSolid* solidCon2Cut3 = new G4SubtractionSolid("Con2Cut3", solidCon2Cut2, solidAirSph, 0, posAirSphSubtraction);

  // Hemisphere - Cyl 3
  G4SubtractionSolid* solidHemSphCut1 = new G4SubtractionSolid("HemiSphere2", solidHemSph, solidCyl3, 0, posCyl3);

  // G4MultiUnion the CHICO-X half
  G4MultiUnion* solidCaseHalf = new G4MultiUnion("CaseHalf");
  solidCaseHalf->AddNode(solidCyl1, G4Transform3D(G4RotationMatrix(), posCyl1));
  solidCaseHalf->AddNode(solidCyl2, G4Transform3D(G4RotationMatrix(), posCyl2));
  solidCaseHalf->AddNode(solidCon1, G4Transform3D(G4RotationMatrix(), posCon1));
  solidCaseHalf->AddNode(solidHemSphCut1, G4Transform3D(G4RotationMatrix(), posHemSph));
  solidCaseHalf->AddNode(solidCon2Cut3, G4Transform3D(G4RotationMatrix(), posCon2));
  solidCaseHalf->Voxelize();

  G4LogicalVolume* logicCaseHalf = new G4LogicalVolume(solidCaseHalf, stainless, "CaseHalf");
  new G4PVPlacement(0, G4ThreeVector(), logicCaseHalf, "CaseHalf", world, 0, 0, check);

  // -------------
  // Window frame
  // -------------
  G4MultiUnion* solidWinFrame = new G4MultiUnion("WinFrame");

  // Window Cone 1, starting from narrow side of frame
  G4Cons* solidWinCon1 = new G4Cons("WinCon1", rWinCon1Inner, rWinCon1Outer1, rWinCon1Inner, rWinCon1Outer2, halfZWinCon1, 0, 360.*deg);
  // Window Cone 2, just below Con1
  G4Cons* solidWinCon2 = new G4Cons("WinCon2", rWinCon2Inner1, rWinCon2Outer1, rWinCon2Inner2, rWinCon2Outer2, halfZWinCon2, 0, 360.*deg);
  // Window Cone 3, main conic part
  G4Cons* solidWinCon3 = new G4Cons("WinCon3", rWinCon3Inner1, rWinCon3Outer1, rWinCon3Inner2, rWinCon3Outer2, halfZWinCon3, 0, 360.*deg);
  // Window Cylinder 1, below Con3
  G4Tubs* solidWinCyl1 = new G4Tubs("WinCyl1", rWinCyl1Inner, rWinCyl1Outer, halfZWinCyl1, 0, 360.*deg);
  // Window Cylinder 2, below Cyl1
  G4Tubs* solidWinCyl2 = new G4Tubs("WinCyl2", rWinCyl2Inner, rWinCyl2Outer, halfZWinCyl2, 0, 360.*deg);

  // Cutouts in the window frame. Two shapes, the upper and lower rows. Use angular ranges to repeat 10 times per row
  G4Tubs* solidWinCutOut1 = new G4Tubs("WinCut1", 2.*inch, 6.*inch, halfZCutout, 0., cutoutPhi); // radii are small and large compared to Con3
  G4SubtractionSolid* solidWinCon3Final;
  G4RotationMatrix* rotCutout1 = new G4RotationMatrix();
  rotCutout1->rotateZ(+88.*deg); // +90 so it starts in +y direction, -2 so the spoke middle is aligned to +y dir

  G4Tubs* solidWinCutOut2 = new G4Tubs("WinCut2", 3.*inch, 7.*inch, halfZCutout, 0., cutoutPhi); // radii are small and large compared to Con3

  G4ThreeVector zPosCutout1(0., 0., -zGapToGap/2. - halfZCutout); // mid point of the cone is the origin this will be used on
  G4ThreeVector zPosCutout2(0., 0., +zGapToGap/2. + halfZCutout);

  // Cuts outs on the main cone
  for(G4int i=0; i<10; i++){
    rotCutout1->rotateZ(spokePhi + cutoutPhi); // first spoke already accounted for
    if(i==0){
      solidWinCon3Final = new G4SubtractionSolid("Con3Final", solidWinCon3, solidWinCutOut1, rotCutout1, zPosCutout1); // first operation so the G4Cons is used, not the G4Subtraction going into itself
      solidWinCon3Final = new G4SubtractionSolid("Con3Final", solidWinCon3Final, solidWinCutOut2, rotCutout1, zPosCutout2);
    }
    else{
      solidWinCon3Final = new G4SubtractionSolid("Con3Final", solidWinCon3Final, solidWinCutOut1, rotCutout1, zPosCutout1);
      solidWinCon3Final = new G4SubtractionSolid("Con3Final", solidWinCon3Final, solidWinCutOut2, rotCutout1, zPosCutout2);
    }
    // G4cout << rotCutout1->getPhi() << G4endl;
  }

  solidWinFrame->AddNode(solidWinCon1, G4Transform3D(G4RotationMatrix(), posWinCon1));
  solidWinFrame->AddNode(solidWinCon2, G4Transform3D(G4RotationMatrix(), posWinCon2));
  solidWinFrame->AddNode(solidWinCon3Final, G4Transform3D(G4RotationMatrix(), posWinCon3));
  solidWinFrame->AddNode(solidWinCyl1, G4Transform3D(G4RotationMatrix(), posWinCyl1));
  solidWinFrame->AddNode(solidWinCyl2, G4Transform3D(G4RotationMatrix(), posWinCyl2));
  solidWinFrame->Voxelize();

  G4LogicalVolume* logicWinFrame = new G4LogicalVolume(solidWinFrame, stainless, "WinFrame");
  new G4PVPlacement(0, G4ThreeVector(0, 0, -halfZTarRing), logicWinFrame, "WinFrame", world, 0, 0, check);

  // -------
  // Mylar
  // -------
  G4Cons* solidMylar = new G4Cons("MylarS", rWinCon3Outer1+um, rWinCon3Outer1+1.755*um, rWinCon3Outer2+um, rWinCon3Outer2+1.755*um, halfZWinCon3, 0, 360.*deg);

  G4Material* mylar = G4Material::GetMaterial("Mylar");
  G4LogicalVolume* logicMylar = new G4LogicalVolume(solidMylar, mylar, "MylarL");
  new G4PVPlacement(0, G4ThreeVector(0, 0, -zWinCon3Base - halfZWinCon3 - halfZTarRing), logicMylar, "MylarP", world, 0, 0, check);

  // DUPLICATE CHICO
  G4RotationMatrix* rot180 = new G4RotationMatrix();
  rot180->rotateX(180.*deg);

  // Place Aluminium casing (multiunion's origin is supposed to be the world origin so a rotation is sufficient)
  new G4PVPlacement(rot180, G4ThreeVector(), logicCaseHalf, "CaseHalf", world, 0, 0, check);
  // Place Mylar (not a g4multiunion so the pos vector doesn't fix itself when rotated)
  new G4PVPlacement(rot180, G4ThreeVector(0, 0, +zWinCon3Base + halfZWinCon3 + halfZTarRing), logicMylar, "MylarP", world, 0, 0, check);
  // Place Window frame (again the position vector needs a bit of negative-to-positive)
  new G4PVPlacement(rot180, G4ThreeVector(0, 0, +halfZTarRing), logicWinFrame, "WinFrame", world, 0, 0, check);

  //Visualization
  G4VisAttributes* vis1 = new G4VisAttributes(G4Color::Red());
  vis1->SetVisibility(true);
  vis1->SetForceSolid(true);

  /*
  G4VisAttributes* vis2 = new G4VisAttributes(G4Color::Blue());
  vis2->SetVisibility(true);
  vis2->SetForceSolid(true);

  G4VisAttributes* vis3 = new G4VisAttributes(G4Color::Green());
  vis3->SetVisibility(true);
  vis3->SetForceSolid(true);
  */
  
  logicWinFrame->SetVisAttributes(vis1);
  //logicCaseHalf->SetVisAttributes(vis2);
  //logicMylar->SetVisAttributes(vis3);
  
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
