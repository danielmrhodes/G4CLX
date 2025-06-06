#include "helper.C"

int main(int argc, char** argv) {
  
  if(argc < 3) {
    std::cerr << "Usage: correlator INPUT_FILE OUTPUT_FILE" << std::endl;
    return 1;
  }

  const char* input_filename = argv[1];
  const char* output_filename = argv[2];
  if(!strcmp(input_filename,output_filename)) {
    std::cout << "Give your input and output files different names" << std::endl;
    return 1;
  }

  FILE* input_file = fopen(input_filename,"rb");
  if(input_file == NULL) {
    std::cout << "Could not open file " << input_filename << std::endl;
    return 1;
  }

  //S3 Singles
  TH2* ringSec0 = new TH2D("RingSec_Det0","Upstream Sector vs Ring",24,1,25,32,1,33);
  TH2* ringSec1 = new TH2D("RingSec_Det1","Downstream Sector vs Ring",24,1,25,32,1,33);

  TH2* ringSec1_p = new TH2D("RingSec_Det1_proj","Downstream Projectile Sector vs Ring",24,1,25,32,1,33);
  TH2* ringSec1_r = new TH2D("RingSec_Det1_rec","Downstream Recoil Sector vs Ring",24,1,25,32,1,33);

  TH2* rPid0 = new TH2D("RingPID_Det0","Upstream Ring Energy PID",24,1,25,500,0,500);
  TH2* rPid1 = new TH2D("RingPID_Det1","Downstream Ring Energy PID",24,1,25,1000,0,1000);

  TH2* sPid0 = new TH2D("SecPID_Det0","Upstream Sector Energy PID",24,1,25,500,0,500);
  TH2* sPid1 = new TH2D("SecPID_Det1","Downstream Sector Energy PID",24,1,25,1000,0,1000);

  TH2* sPid1_p = new TH2D("SecPID_Det1_proj","Downstream Sector Energy Projectile PID",24,1,25,1000,0,1000);
  TH2* sPid1_r = new TH2D("SecPID_Det1_rec","Downsream Sector Energy Recoil PID",24,1,25,1000,0,1000);
  
  TH2* sPid1_m1 = new TH2D("SecPID_Det1_m1","Downstream Sector Energy PID, Mult 1",24,1,25,1000,0,1000);
  TH2* sPid1_m1_p = new TH2D("SecPID_Det1_m1_proj","Downstream Sector Energy Projectile PID, Mult 1",24,1,25,1000,0,1000);
  TH2* sPid1_m1_r = new TH2D("SecPID_Det1_m1_rec","Downsream Sector Energy Recoil PID, Mult 1",24,1,25,1000,0,1000);

  TH2* sPid1_m2 = new TH2D("SecPID_Det1_m2","Downstream Sector Energy PID, Mult 2",24,1,25,1000,0,1000);
  TH2* sPid1_m2_p = new TH2D("SecPID_Det1_m2_proj","Downstream Sector Energy Projectile PID, Mult 2",24,1,25,1000,0,1000);
  TH2* sPid1_m2_r = new TH2D("SecPID_Det1_m2_rec","Downsream Sector Energy Recoil PID, Mult 2",24,1,25,1000,0,1000);

  //Greta Singles
  TH1* coreEnergy = new TH1D("Core_Energy","Greta Core Energy",16000,0,4000);
  TH2* coreSum = new TH2D("Core_Summary","Core Energy Summary",nDets,1,nDets+1,8000,0,4000);
  
  TH1* segEnergy = new TH1D("Seg_Energy","Greta Segment Energy",16000,0,4000);
  TH2* segSum = new TH2D("Seg_Summary","Segment Energy Summary",nDets*nSegs,1,nDets*nSegs+1,4000,0,4000);

  TH1* coreEn_Fep = new TH1D("FEP","Greta FEP",16000,0,4000);
  TH1* coreEn_NotFep = new TH1D("nFEP","Greta Not FEP",16000,0,4000);

  TH2* FEPsum = new TH2D("FEP_Summary","Core Energy Summary FEP",nDets,1,nDets+1,8000,0,4000);
  TH2* nFEPsum = new TH2D("nFEP_Summary","Core Energy Summary Not FEP",nDets,1,nDets+1,8000,0,4000);

  TH2* detSeg = new TH2D("detSeg","Geta Segment Number vs Detector Number",nDets,1,nDets+1,nSegs,1,nSegs+1);
  TH2* posTP = new TH2D("posTP","Greta Theta-Phi Surface",720,0.0,180.0,1440,0.0,360.0);

  TH1* gamGate = new TH1D("gamGate","CoreEn with 1332 keV Coinc",16000,0,4000);
  TH1* gammaAngFEP = new TH1D("gamAngFEP","Gamma Gamma Angle",2200,-1.1,1.1);
  
  //Coincidences
  //Projectile DS
  TH2* sPidDS = new TH2D("SecPID_DS","DS SectorEn PID",24,1,25,1000,0,1000);
  TH2* rPidDS = new TH2D("RingPID_DS","DS RingEn PID",24,1,25,1000,0,1000);
  
  TH1* pCoreEnergyDS = new TH1D("Core_EnergyDS","Greta Core Energy",16000,0,4000);
  TH2* pCoreSumDS = new TH2D("Core_SummaryDS","Core Energy Summary",nDets,1,nDets+1,4000,0,4000);

  TH1* pDopEnergyDS = new TH1D("Dop_EnergyDS","Doppler Energy",16000,0,4000);
  TH2* pDopSumDS = new TH2D("Dop_SummaryDS","Doppler Energy Summary",nDets,1,nDets+1,8000,0,4000);
  
  TH1* pCoreEnergyDS_fep = new TH1D("Core_EnergyDS_fep","Greta Core Energy FEP",16000,0,4000);
  TH1* pCoreEnergyDS_pfep = new TH1D("Core_EnergyDS_pfep","Greta Core Energy Projectile FEP",16000,0,4000);
  TH1* pCoreEnergyDS_rfep = new TH1D("Core_EnergyDS_rfep","Greta Core Energy Recoil FEP",16000,0,4000);
  TH1* pCoreEnergyDS_nfep = new TH1D("Core_EnergyDS_nfep","Greta Core Energy Not FEP",16000,0,4000);

  TH1* pDopEnergyDS_fep = new TH1D("Dop_EnergyDS_fep","Doppler Energy FEP",16000,0,4000);
  TH1* pDopEnergyDS_pfep = new TH1D("Dop_EnergyDS_pfep","Doppler Energy Projectile FEP",16000,0,4000);
  TH1* pDopEnergyDS_rfep = new TH1D("Dop_EnergyDS_rfep","Doppler Energy Recoil FEP",16000,0,4000);
  TH1* pDopEnergyDS_nfep = new TH1D("Dop_EnergyDS_nfep","Doppler Energy Not FEP",16000,0,4000);

  TH2* pThCorDS = new TH2D("Theta_CorrDS","Theta Correlation",4000,0,4000,180,0.0,180.0);
  TH2* pThCrtDS = new TH2D("Theta_CrctDS","Theta Correction",8000,0,4000,180,0.0,180.0);
  
  TH2* pPhCorDS = new TH2D("Phi_CorrDS","Phi Correlation",4000,0,4000,90,0.0,360.0);
  TH2* pPhCrtDS = new TH2D("Phi_CrctDS","Phi Correction",8000,0,4000,90,0.0,360.0);

  TH1* pReconEnergyDS = new TH1D("Recon_EnergyDS","Recon Energy",16000,0,4000);
  TH2* pReconSumDS = new TH2D("Recon_SummaryDS","Recon Energy Summary",nDets,1,nDets+1,8000,0,4000);

  TH1* pReconEnergyDS_fep = new TH1D("Recon_EnergyDS_fep","Recon Energy FEP",16000,0,4000);
  TH1* pReconEnergyDS_pfep = new TH1D("Recon_EnergyDS_pfep","Recon Energy Projectile FEP",1600,0,4000);
  TH1* pReconEnergyDS_rfep = new TH1D("Recon_EnergyDS_rfep","Recon Energy Recoil FEP",16000,0,4000);
  TH1* pReconEnergyDS_nfep = new TH1D("Recon_EnergyDS_nfep","Recon Energy Not FEP",16000,0,4000);

  TH2* pReconThCorDS = new TH2D("ReconTheta_CorrDS","Recon Theta Correlation",4000,0,4000,180,0.0,180.0);
  TH2* pReconThCrtDS = new TH2D("ReconTheta_CrctDS","Recon Theta Correction",8000,0,4000,180,0.0,180.0);

  TH2* pReconPhCorDS = new TH2D("ReconPhi_CorrDS","Recon Phi Correlation",4000,0,4000,90,0.0,360.0);
  TH2* pReconPhCrtDS = new TH2D("ReconPhi_CrctDS","Recon Phi Correction",8000,0,4000,90,0.0,360.0);
  
  //Recoil
  TH2* sPidRec = new TH2D("SecPID_Rec","DS SectorEn PID",24,1,25,1000,0,1000);
  TH2* rPidRec = new TH2D("RingPID_Rec","DS RingEn PID",24,1,25,1000,0,1000);
  
  TH1* rCoreEnergy = new TH1D("Core_EnergyRec","Greta Core Energy",16000,0,4000);
  TH2* rCoreSum = new TH2D("Core_SummaryRec","Core Energy Summary",nDets,1,nDets+1,8000,0,4000);
  
  TH1* rDopEnergy = new TH1D("Dop_EnergyRec","Doppler Energy",16000,0,4000);
  TH2* rDopSum = new TH2D("Dop_SummaryRec","Doppler Energy Summary",nDets,1,nDets+1,8000,0,4000);
  
  TH1* rCoreEnergy_fep = new TH1D("Core_EnergyRec_fep","Greta Core Energy FEP",16000,0,4000);
  TH1* rCoreEnergy_pfep = new TH1D("Core_EnergyRec_pfep","Greta Core Energy Projectile FEP",16000,0,4000);
  TH1* rCoreEnergy_rfep = new TH1D("Core_EnergyRec_rfep","Greta Core Energy Recoil FEP",16000,0,4000);
  TH1* rCoreEnergy_nfep = new TH1D("Core_EnergRec_nfep","Greta Core Energy Not FEP",16000,0,4000);

  TH1* rDopEnergy_fep = new TH1D("Dop_EnergyRec_fep","Doppler Energy FEP",16000,0,4000);
  TH1* rDopEnergy_pfep = new TH1D("Dop_EnergyRec_pfep","Doppler Energy Projectile FEP",16000,0,4000);
  TH1* rDopEnergy_rfep = new TH1D("Dop_EnergyRec_rfep","Doppler Energy Recoil FEP",16000,0,4000);
  TH1* rDopEnergy_nfep = new TH1D("Dop_EnergyRec_nfep","Doppler Energy Not FEP",16000,0,4000);

  TH2* rThCor = new TH2D("Theta_CorrRec","Theta Correlation",4000,0,4000,180,0.0,180.0);
  TH2* rThCrt = new TH2D("Theta_CrctRec","Theta Correction",8000,0,4000,180,0.0,180.0);
  
  TH2* rPhCor = new TH2D("Phi_CorrRec","Phi Correlation",4000,0,4000,90,0.0,360.0);
  TH2* rPhCrt = new TH2D("Phi_CrctRec","Phi Correction",8000,0,4000,90,0.0,360.0);

  TH1* rReconEnergy = new TH1D("Recon_EnergyRec","Recon Energy",16000,0,4000);
  TH2* rReconSum = new TH2D("Recon_SummaryRec","Recon Energy Summary",nDets,1,nDets+1,8000,0,4000);
  
  TH1* rReconEnergy_fep = new TH1D("Recon_EnergyRec_fep","Recon Energy FEP",16000,0,4000);
  TH1* rReconEnergy_pfep = new TH1D("Recon_EnergyRec_pfep","Recon Energy Projectile FEP",16000,0,4000);
  TH1* rReconEnergy_rfep = new TH1D("Recon_EnergyRec_rfep","Recon Energy Recoil FEP",16000,0,4000);
  TH1* rReconEnergy_nfep = new TH1D("Recon_EnergyRec_nfep","Recon Energy Not FEP",16000,0,4000);
  
  TH2* rReconThCor = new TH2D("ReconTheta_CorrRec","Recon Theta Correlation",4000,0,4000,180,0.0,180.0);
  TH2* rReconThCrt = new TH2D("ReconTheta_CrctRec","Recon Theta Correction",8000,0,4000,180,0.0,180.0);

  TH2* rReconPhCor = new TH2D("ReconPhi_CorrRec","Recon Phi Correlation",4000,0,4000,90,0.0,360.0);
  TH2* rReconPhCrt = new TH2D("ReconPhi_CrctRec","Recon Phi Correction",8000,0,4000,90,0.0,360.0);

  //Projectile US
  TH2* sPidUS = new TH2D("SecPID_US","DS SectorEn PID",24,1,25,500,0,500);
  TH2* rPidUS = new TH2D("RingPID_US","DS RingEn PID",24,1,25,500,0,500);
  
  TH1* pCoreEnergyUS = new TH1D("Core_EnergyUS","Greta Core Energy",16000,0,4000);
  TH2* pCoreSumUS = new TH2D("Core_SummaryUS","Core Energy Summary",nDets,1,nDets+1,4000,0,4000);
  
  TH1* pDopEnergyUS = new TH1D("Dop_EnergyUS","Doppler Energy",16000,0,4000);
  TH2* pDopSumUS = new TH2D("Dop_SummaryUS","Doppler Energy Summary",nDets,1,nDets+1,8000,0,4000);
  
  TH1* pCoreEnergyUS_fep = new TH1D("Core_EnergyUS_fep","Greta Core Energy FEP",16000,0,4000);
  TH1* pCoreEnergyUS_pfep = new TH1D("Core_EnergyUS_pfep","Greta Core Energy Projectile FEP",16000,0,4000);
  TH1* pCoreEnergyUS_rfep = new TH1D("Core_EnergyUS_rfep","Greta Core Energy Recoil FEP",16000,0,4000);
  TH1* pCoreEnergyUS_nfep = new TH1D("Core_EnergyUS_nfep","Greta Core Energy Not FEP",16000,0,4000);

  TH1* pDopEnergyUS_fep = new TH1D("Dop_EnergyUS_fep","Doppler Energy FEP",16000,0,4000);
  TH1* pDopEnergyUS_pfep = new TH1D("Dop_EnergyUS_pfep","Doppler Energy Projectile FEP",16000,0,4000);
  TH1* pDopEnergyUS_rfep = new TH1D("Dop_EnergyUS_rfep","Doppler Energy Recoil FEP",16000,0,4000);
  TH1* pDopEnergyUS_nfep = new TH1D("Dop_EnergyUS_nfep","Doppler Energy Not FEP",16000,0,4000);

  TH2* pThCorUS = new TH2D("Theta_CorrUS","Theta Correlation",2000,0,4000,180,0.0,180.0);
  TH2* pThCrtUS = new TH2D("Theta_CrctUS","Theta Correction",4000,0,4000,180,0.0,180.0);
  
  TH2* pPhCorUS = new TH2D("Phi_CorrUS","Phi Correlation",2000,0,4000,90,0.0,360.0);
  TH2* pPhCrtUS = new TH2D("Phi_CrctUS","Phi Correction",4000,0,4000,90,0.0,360.0);

  TH1* pReconEnergyUS = new TH1D("Recon_EnergyUS","Recon Energy",16000,0,4000);
  TH2* pReconSumUS = new TH2D("Recon_SummaryUS","Recon Energy Summary",nDets,1,nDets+1,8000,0,4000);
  
  TH1* pReconEnergyUS_fep = new TH1D("Recon_EnergyUS_fep","Recon Energy FEP",16000,0,4000);
  TH1* pReconEnergyUS_pfep = new TH1D("Recon_EnergyUS_pfep","Recon Energy Projectile FEP",16000,0,4000);
  TH1* pReconEnergyUS_rfep = new TH1D("Recon_EnergyUS_rfep","Recon Energy Recoil FEP",16000,0,4000);
  TH1* pReconEnergyUS_nfep = new TH1D("Recon_EnergyUS_nfep","Recon Energy Not FEP",16000,0,4000);
  
  TH2* pReconThCorUS = new TH2D("ReconTheta_CorrUS","Recon Theta Correlation",4000,0,4000,180,0.0,180.0);
  TH2* pReconThCrtUS = new TH2D("ReconTheta_CrctUS","Recon Theta Correction",8000,0,4000,180,0.0,180.0);

  TH2* pReconPhCorUS = new TH2D("ReconPhi_CorrUS","Recon Phi Correlation",4000,0,4000,90,0.0,360.0);
  TH2* pReconPhCrtUS = new TH2D("ReconPhi_CrctUS","Recon Phi Correction",8000,0,4000,90,0.0,360.0);
  
  const TVector3 incBeam = TVector3(0.0,0.0,1.0);
  const double Sol2_En = KE_LAB(Theta_CM_FP(Theta_LAB_Max(beam_en),beam_en),beam_en);
  const double r2d = TMath::RadToDeg();

  TRandom* rand = new TRandom(50747227);

  std::cout << "Correlating and histograming data..." << std::endl;
  
  Header header;
  RawData raw_data;
  while(fread(&header,header.bytes(),1,input_file)) {

    const int nE = header.evtNum;
    const int nS = header.nSdata;
    const int nG = header.nGdata;
  
    fread(&raw_data.sData,sizeof(S3Data),nS,input_file);
    fread(&raw_data.gData,sizeof(GretaData),nG,input_file);
    BuiltData data = BuildData(nE,nS,nG,raw_data);


    //S3 singles
    for(int i=0;i<data.nS3;i++) {

      int det = data.s3[i].det;
      int ring = data.s3[i].ring;
      int sec = data.s3[i].sector;

      if(ring < Ri || ring > Rf)
	continue;
      
      double ring_en = data.s3[i].rEn;
      double sec_en = data.s3[i].sEn;

      TVector3 segPos = GetPos(det,ring,sec);
      TVector3 pos = data.s3[i].sPos;

      segPos.SetX(segPos.X() - beam_X);
      segPos.SetY(segPos.Y() - beam_Y);
      pos.SetX(pos.X() - beam_X);
      pos.SetY(pos.Y() - beam_Y);

      if(det == 0) { //Upstream

	ringSec0->Fill(ring,sec);
	rPid0->Fill(ring,ring_en);
	sPid0->Fill(ring,sec_en);
	
      }
      else { //Downstream
	
	ringSec1->Fill(ring,sec);
	rPid1->Fill(ring,ring_en);
	sPid1->Fill(ring,sec_en);

	if(data.s3[i].rP && data.s3[i].sP) { //Projectile

	  ringSec1_p->Fill(ring,sec);
	  sPid1_p->Fill(ring,sec_en);
	  
	} //End projectile gate
	
	if(data.s3[i].rR && data.s3[i].sR) { //Recoil

	  ringSec1_r->Fill(ring,sec);
	  sPid1_r->Fill(ring,sec_en);
	  
	} //End recoil gate

        if(data.nS3 == 1 && det == 1) {

          sPid1_m1->Fill(ring,sec_en);
	  if(data.s3[i].rR && data.s3[i].sR)
            sPid1_m1_r->Fill(ring,sec_en);
  	  if(data.s3[i].rP && data.s3[i].sP)
            sPid1_m1_p->Fill(ring,sec_en);

       }
      } //End downstream gate
     } //End S3 singles
    
    if(data.nS3 == 2) {
    
      int det0 = data.s3[0].det;
      int det1 = data.s3[1].det;
      if(det0 == det1) {

      int ring0 = data.s3[0].ring;
      //int sec0 = data.s3[0].sector;
      //double ring_en0 = data.s3[0].rEn;
      double sec_en0 = data.s3[0].sEn;
      

      int ring1 = data.s3[1].ring;
      //int sec1 = data.s3[1].sector;
      //double ring_en1 = data.s3[1].rEn;
      double sec_en1 = data.s3[1].sEn;

      sPid1_m2->Fill(ring0,sec_en0);
      sPid1_m2->Fill(ring1,sec_en1);
      
      if(data.s3[0].rR && data.s3[0].sR)
        sPid1_m2_r->Fill(ring0,sec_en0);

      if(data.s3[0].rP && data.s3[0].sP)
        sPid1_m2_p->Fill(ring0,sec_en0);
      
     if(data.s3[1].rR && data.s3[1].sR)
       sPid1_m2_r->Fill(ring1,sec_en1);

      if(data.s3[1].rP && data.s3[1].sP)
        sPid1_m2_p->Fill(ring1,sec_en1);

      }
    } //End S3 Mult2
    
    //Greta singles
    for(int i=0;i<data.nGr;i++) {

      int det = data.greta[i].det;
      double en = data.greta[i].cEn;
      double core_en = rand->Gaus(en,Sigma(en));
      bool fep = data.greta[i].fep;
      
      coreEnergy->Fill(core_en);
      coreSum->Fill(det,core_en);
      
      for(int j=0;j<data.greta[i].nsegs;j++) {

	int seg = data.greta[i].segs[j];
	int num = seg + nSegs*(det-1);
	double seg_en = data.greta[i].sEn[j];

	//double exactX = data.greta[i].x[j];
	//double exactY = data.greta[i].y[j];
	//double exactZ = data.greta[i].z[j];
	TVector3 exact_pos = data.greta[i].MainPos();

	double theta = exact_pos.Theta()*r2d;
	double phi = exact_pos.Phi();
	if(phi < 0.0)
	  phi += TMath::TwoPi();
	phi *= r2d;	
	
	segEnergy->Fill(seg_en);
	segSum->Fill(num,seg_en);
	
	detSeg->Fill(det,seg);
	posTP->Fill(theta,phi);
	
      }
      
      if(fep) {
	coreEn_Fep->Fill(core_en);
	FEPsum->Fill(det,core_en);
      }
      else {
	coreEn_NotFep->Fill(core_en);
	nFEPsum->Fill(det,core_en);
      }
    } //End Greta singles

    //Mult2
    if(data.nGr == 2) {
      
      double en1 = data.greta[0].cEn;
      double en2 = data.greta[1].cEn;
      
      double core_en1 = rand->Gaus(en1,Sigma(en1));
      double core_en2 = rand->Gaus(en2,Sigma(en2));
      
      if(core_en1 > 1330.0 && core_en1 < 1335.0)
	gamGate->Fill(core_en2);
      else if(core_en2 > 1330.0 && core_en2 < 1335.0)
	gamGate->Fill(core_en1);

      if(data.greta[0].fep && data.greta[1].fep) {
	
	TVector3 pos1 = data.greta[0].MainPos();
	TVector3 pos2 = data.greta[1].MainPos();
	double theta = pos1.Angle(pos2);
	
	gammaAngFEP->Fill(TMath::Cos(theta));
	
      }
    } //End Greta mult2

    //Coincidences
    if(data.nS3 > 0 && data.nGr > 0) {
      
      for(int i=0;i<data.nS3;i++) {

	int bDet = data.s3[i].det;
	int ring = data.s3[i].ring;
	int sector = data.s3[i].sector;

	if(ring < Ri || ring > Rf)
	  continue;

	double ring_en = data.s3[i].rEn;
        double sec_en = data.s3[i].sEn;

	TVector3 bPos = GetPos(bDet,ring,sector);
	bPos.SetX(bPos.X() - beam_X);
	bPos.SetY(bPos.Y() - beam_Y);	

	if(bDet && data.s3[i].rP && data.s3[i].sP) { //Projectile DS gate

	  sPidDS->Fill(ring,sec_en);
	  rPidDS->Fill(ring,ring_en);

	  bool sol2 = false;
	  if(sec_en < Sol2_En)
	    sol2 = true;
	  
	  double thetaCM = Theta_CM_FP(bPos.Theta(),beam_en,sol2);
	  
	  double energy = KE_LAB(thetaCM,beam_en);
	  double gam = (energy/beam_mass) + 1.0;
	  double beta = TMath::Sqrt(1.0 - 1.0/(gam*gam));
	  
	  double recon_energy = Recoil_KE_LAB(thetaCM,beam_en);
	  double recon_gam = (recon_energy)/targ_mass + 1.0;
	  double recon_beta = TMath::Sqrt(1.0 - 1.0/(recon_gam*recon_gam));
	  
	  TVector3 rPos(0,0,1); //Reconstructed position of the recoil
	  rPos.SetTheta(Recoil_Theta_LAB(thetaCM,beam_en));
	  rPos.SetPhi(bPos.Phi() - TMath::Pi());
	  
	  for(int j=0;j<data.nGr;j++) {

	    int det = data.greta[j].det;
	    //int seg = data.greta[j].MainSeg();
	    
	    double en = data.greta[j].cEn;
	    double coreEn = rand->Gaus(en,Sigma(en));
	    
	    bool FEP = data.greta[j].fep;
	    bool PFEP = data.greta[j].pfep;
	  
	    //TVector3 sPos = GetPos(det,seg);
	    TVector3 sPos = data.greta[j].MainPos();
	    sPos.SetX(sPos.X() - beam_X);
	    sPos.SetY(sPos.Y() - beam_Y);

	    double theta = bPos.Angle(sPos);
	    double dopEn = gam*(1 - beta*TMath::Cos(theta))*coreEn;
	    
	    TVector3 reacPlane = bPos.Cross(incBeam);
	    TVector3 detPlane = sPos.Cross(incBeam);

	    double reac_phi = reacPlane.Phi();
	    if(reac_phi < 0)
	      reac_phi += TMath::TwoPi();

	    double det_phi = detPlane.Phi();
	    if(det_phi < 0)
	      det_phi += TMath::TwoPi();

	    double planeAng = reac_phi - det_phi;
	    if(planeAng < 0)
	      planeAng += TMath::TwoPi();
	    
	    double recon_theta = rPos.Angle(sPos);
	    double recon_en = recon_gam*(1 - recon_beta*TMath::Cos(recon_theta))*coreEn;
	    
	    TVector3 reconPlane = rPos.Cross(incBeam);

	    double recon_phi = reconPlane.Phi();
	    if(recon_phi < 0)
	      recon_phi += TMath::TwoPi();

	    double reconAng = recon_phi - det_phi;
	    if(reconAng < 0)
	      reconAng += TMath::TwoPi();
	    
	    pCoreEnergyDS->Fill(coreEn);
	    pCoreSumDS->Fill(det,coreEn);

	    if(FEP) {
	      
	      pCoreEnergyDS_fep->Fill(coreEn);
	      pDopEnergyDS_fep->Fill(dopEn);
	      pReconEnergyDS_fep->Fill(recon_en);

	      if(PFEP) {
		pCoreEnergyDS_pfep->Fill(coreEn);
		pDopEnergyDS_pfep->Fill(dopEn);
		pReconEnergyDS_pfep->Fill(recon_en);
	      }
	      else {
		pCoreEnergyDS_rfep->Fill(coreEn);
		pDopEnergyDS_rfep->Fill(dopEn);
		pReconEnergyDS_rfep->Fill(recon_en);
	      }
	    }
	    else {
	      pCoreEnergyDS_nfep->Fill(coreEn);
	      pDopEnergyDS_nfep->Fill(dopEn);
	      pReconEnergyDS_nfep->Fill(recon_en);
	    }
	    
	    pDopEnergyDS->Fill(dopEn);
	    pDopSumDS->Fill(det,dopEn);
	  
	    pThCorDS->Fill(coreEn,theta*r2d);
	    pThCrtDS->Fill(dopEn,theta*r2d);

	    pPhCorDS->Fill(coreEn,planeAng*r2d);
	    pPhCrtDS->Fill(dopEn,planeAng*r2d);

	    pReconEnergyDS->Fill(recon_en);
	    pReconSumDS->Fill(det,recon_en);

	    pReconThCorDS->Fill(coreEn,recon_theta*r2d);
	    pReconThCrtDS->Fill(recon_en,recon_theta*r2d);

	    pReconPhCorDS->Fill(coreEn,reconAng*r2d);
	    pReconPhCrtDS->Fill(recon_en,reconAng*r2d);   
  
	  } //End Greta loop
	} //End projectile DS gate

	else if(!bDet && data.s3[i].rP && data.s3[i].sP) { //Projectile US gate

	  sPidUS->Fill(ring,sec_en);
	  rPidUS->Fill(ring,ring_en);
	  
	  double thetaCM = Theta_CM_FP(bPos.Theta(),beam_en,false);
	  
	  double energy = KE_LAB(thetaCM,beam_en);
	  double gam = (energy)/beam_mass + 1.0;
	  double beta = TMath::Sqrt(1.0 - 1.0/(gam*gam));

	  double recon_energy = Recoil_KE_LAB(thetaCM,beam_en);
	  double recon_gam = (recon_energy)/targ_mass + 1.0;
	  double recon_beta = TMath::Sqrt(1.0 - 1.0/(recon_gam*recon_gam));

	  TVector3 rPos(0,0,1); //Reconstructed position of the recoil
	  rPos.SetTheta(Recoil_Theta_LAB(thetaCM,beam_en));
	  rPos.SetPhi(bPos.Phi() - TMath::Pi());
	  
	  for(int j=0;j<data.nGr;j++) {

	    int det = data.greta[j].det;
	    //int seg = data.greta[j].MainSeg();
	    double en = data.greta[j].cEn;
	    double coreEn = rand->Gaus(en,Sigma(en));
	    bool FEP = data.greta[j].fep;
	    bool PFEP = data.greta[j].pfep;
	  
	    //TVector3 sPos = GetPos(det,seg);
	    TVector3 sPos = data.greta[j].MainPos();
	    sPos.SetX(sPos.X() - beam_X);
	    sPos.SetY(sPos.Y() - beam_Y);

	    double theta = bPos.Angle(sPos);
	    double dopEn = gam*(1 - beta*TMath::Cos(theta))*coreEn;
	  
	    TVector3 reacPlane = bPos.Cross(incBeam);
	    TVector3 detPlane = sPos.Cross(incBeam);

	    double reac_phi = reacPlane.Phi();
	    if(reac_phi < 0)
	      reac_phi += TMath::TwoPi();

	    double det_phi = detPlane.Phi();
	    if(det_phi < 0)
	      det_phi += TMath::TwoPi();

	    double planeAng = reac_phi - det_phi;
	    if(planeAng < 0)
	      planeAng += TMath::TwoPi();

	    double recon_theta = rPos.Angle(sPos);
	    double recon_en = recon_gam*(1 - recon_beta*TMath::Cos(recon_theta))*coreEn;

	    TVector3 reconPlane = rPos.Cross(incBeam);

	    double recon_phi = reconPlane.Phi();
	    if(recon_phi < 0)
	      recon_phi += TMath::TwoPi();

	    double reconAng = recon_phi - det_phi;
	    if(reconAng < 0)
	      reconAng += TMath::TwoPi();

	    pCoreEnergyUS->Fill(coreEn);
	    pCoreSumUS->Fill(det,coreEn);
	    
	    pDopEnergyUS->Fill(dopEn);
	    pDopSumUS->Fill(det,dopEn);

	    if(FEP) {
	      
	      pCoreEnergyUS_fep->Fill(coreEn);
	      pDopEnergyUS_fep->Fill(dopEn);
	      pReconEnergyUS_fep->Fill(recon_en);

	      if(PFEP) {
		pCoreEnergyUS_pfep->Fill(coreEn);
		pDopEnergyUS_pfep->Fill(dopEn);
		pReconEnergyUS_pfep->Fill(recon_en);
	      }
	      else {
		pCoreEnergyUS_rfep->Fill(coreEn);
		pDopEnergyUS_rfep->Fill(dopEn);
		pReconEnergyUS_rfep->Fill(recon_en);
	      }
	    }
	    else {
	      pCoreEnergyUS_nfep->Fill(coreEn);
	      pDopEnergyUS_nfep->Fill(dopEn);
	      pReconEnergyUS_nfep->Fill(recon_en);
	    }

	    pThCorUS->Fill(coreEn,theta*r2d);
	    pThCrtUS->Fill(dopEn,theta*r2d);

	    pPhCorUS->Fill(coreEn,planeAng*r2d);
	    pPhCrtUS->Fill(dopEn,planeAng*r2d);

	    pReconEnergyUS->Fill(recon_en);
	    pReconSumUS->Fill(det,recon_en);
	    
	    pReconThCorUS->Fill(coreEn,recon_theta*r2d);
	    pReconThCrtUS->Fill(recon_en,recon_theta*r2d);

	    pReconPhCorUS->Fill(coreEn,reconAng*r2d);
	    pReconPhCrtUS->Fill(recon_en,reconAng*r2d);
	  
	  } //End Greta loop
	} //End Projectile US gate

	else if(data.s3[i].rR && data.s3[i].sR) { //Recoil gate

	  sPidRec->Fill(ring,sec_en);
	  rPidRec->Fill(ring,ring_en);

	  double thetaCM = Theta_CM_FR(bPos.Theta(),beam_en);
	  
	  double energy = Recoil_KE_LAB(thetaCM,beam_en);
	  double gam = (energy)/targ_mass + 1.0;
	  double beta = TMath::Sqrt(1.0 - 1.0/(gam*gam));

	  double recon_energy = KE_LAB(thetaCM,beam_en);
	  double recon_gam = (recon_energy)/beam_mass + 1.0;
	  double recon_beta = TMath::Sqrt(1.0 - 1.0/(recon_gam*recon_gam));

	  TVector3 rPos(0,0,1); //Reconstructed position of the projectile
	  rPos.SetTheta(Theta_LAB(thetaCM,beam_en));
	  rPos.SetPhi(bPos.Phi() - TMath::Pi());

	  for(int j=0;j<data.nGr;j++) {

	    int det = data.greta[j].det;
	    //int seg = data.greta[j].MainSeg();
	    double en = data.greta[j].cEn;
	    double coreEn = rand->Gaus(en,Sigma(en));
	    bool FEP = data.greta[j].fep;
	    bool PFEP = data.greta[j].pfep;
	  
	    //TVector3 sPos = GetPos(det,seg);
	    TVector3 sPos = data.greta[j].MainPos();
	    sPos.SetX(sPos.X() - beam_X);
	    sPos.SetY(sPos.Y() - beam_Y);

	    double theta = bPos.Angle(sPos);
	    double dopEn = gam*(1 - beta*TMath::Cos(theta))*coreEn;
	  
	    TVector3 reacPlane = bPos.Cross(incBeam);
	    TVector3 detPlane = sPos.Cross(incBeam);

	    double reac_phi = reacPlane.Phi();
	    if(reac_phi < 0)
	      reac_phi += TMath::TwoPi();

	    double det_phi = detPlane.Phi();
	    if(det_phi < 0)
	      det_phi += TMath::TwoPi();

	    double planeAng = reac_phi - det_phi;
	    if(planeAng < 0)
	      planeAng += TMath::TwoPi();

	    double recon_theta = rPos.Angle(sPos);
	    double recon_en = recon_gam*(1 - recon_beta*TMath::Cos(recon_theta))*coreEn;

	    TVector3 reconPlane = rPos.Cross(incBeam);

	    double recon_phi = reconPlane.Phi();
	    if(recon_phi < 0)
	      recon_phi += TMath::TwoPi();

	    double reconAng = recon_phi - det_phi;
	    if(reconAng < 0)
	      reconAng += TMath::TwoPi();
	    
	    rCoreEnergy->Fill(coreEn);
	    rCoreSum->Fill(det,coreEn);
	    
	    rDopEnergy->Fill(dopEn);
	    rDopSum->Fill(det,dopEn);

	    if(FEP) {
	      
	      rCoreEnergy_fep->Fill(coreEn);
	      rDopEnergy_fep->Fill(dopEn);
	      rReconEnergy_fep->Fill(recon_en);

	      if(PFEP) {
		rCoreEnergy_pfep->Fill(coreEn);
		rDopEnergy_pfep->Fill(dopEn);
		rReconEnergy_pfep->Fill(recon_en);
	      }
	      else {
		rCoreEnergy_rfep->Fill(coreEn);
		rDopEnergy_rfep->Fill(dopEn);
		rReconEnergy_rfep->Fill(recon_en);
	      }
	    }
	    else {
	      rCoreEnergy_nfep->Fill(coreEn);
	      rDopEnergy_nfep->Fill(dopEn);
	      rReconEnergy_nfep->Fill(recon_en);
	    }
	    
	    rThCor->Fill(coreEn,theta*r2d);
	    rThCrt->Fill(dopEn,theta*r2d);
	    
	    rPhCor->Fill(coreEn,planeAng*r2d);
	    rPhCrt->Fill(dopEn,planeAng*r2d);

	    rReconEnergy->Fill(recon_en);
	    rReconSum->Fill(det,recon_en);
	    
	    rReconThCor->Fill(coreEn,recon_theta*r2d);
	    rReconThCrt->Fill(recon_en,recon_theta*r2d);

	    rReconPhCor->Fill(coreEn,reconAng*r2d);
	    rReconPhCrt->Fill(recon_en,reconAng*r2d);
	    
	  } //End Tigress loop
	} //End Recoil Gate

      } //End S3 loop
    } //End Coincidences

  } //End while loop
  std::cout << "Writing histograms to file..." << std::endl;

  TFile* outFile = new TFile(output_filename,"RECREATE");
  
  outFile->mkdir("Greta");
  outFile->mkdir("S3");
  
  outFile->mkdir("Coincidence/ProjectileDS");
  outFile->mkdir("Coincidence/ProjectileDS/Doppler");
  outFile->mkdir("Coincidence/ProjectileDS/Recon");
  
  outFile->mkdir("Coincidence/ProjectileUS");
  outFile->mkdir("Coincidence/ProjectileUS/Doppler");
  outFile->mkdir("Coincidence/ProjectileUS/Recon");
  
  outFile->mkdir("Coincidence/Recoil");
  outFile->mkdir("Coincidence/Recoil/Doppler");
  outFile->mkdir("Coincidence/Recoil/Recon");

  outFile->cd("S3");
     
  ringSec0->Write();
  ringSec1->Write();

  ringSec1_p->Write();
  ringSec1_r->Write();
  
  rPid0->Write();
  sPid0->Write();
  
  rPid1->Write();
  sPid1->Write();
  
  sPid1_p->Write();
  sPid1_r->Write();

  sPid1_m1->Write();
  sPid1_m1_p->Write();
  sPid1_m1_r->Write();

  sPid1_m2->Write();
  sPid1_m2_p->Write();
  sPid1_m2_r->Write();

  outFile->cd("Greta");

  coreEnergy->Write();
  coreSum->Write();
  
  segEnergy->Write();
  segSum->Write();

  coreEn_Fep->Write();
  coreEn_NotFep->Write();

  FEPsum->Write();
  nFEPsum->Write();

  detSeg->Write();
  posTP->Write();

  gamGate->Write();
  gammaAngFEP->Write();

  outFile->cd("Coincidence/ProjectileDS");

  sPidDS->Write();
  rPidDS->Write();

  pCoreEnergyDS->Write();
  pCoreSumDS->Write();
  
  pCoreEnergyDS_fep->Write();
  pCoreEnergyDS_pfep->Write();
  pCoreEnergyDS_rfep->Write();
  pCoreEnergyDS_nfep->Write();

  outFile->cd("Coincidence/ProjectileDS/Doppler");
  
  pDopEnergyDS->Write();
  pDopSumDS->Write();
  
  pDopEnergyDS_fep->Write();
  pDopEnergyDS_pfep->Write();
  pDopEnergyDS_rfep->Write();
  pDopEnergyDS_nfep->Write();

  pThCorDS->Write();
  pThCrtDS->Write();

  pPhCorDS->Write();
  pPhCrtDS->Write();

  outFile->cd("Coincidence/ProjectileDS/Recon");

  pReconEnergyDS->Write();
  pReconSumDS->Write();
  
  pReconEnergyDS_fep->Write();
  pReconEnergyDS_pfep->Write();
  pReconEnergyDS_rfep->Write();
  pReconEnergyDS_nfep->Write();

  pReconThCorDS->Write();
  pReconThCrtDS->Write();

  pReconPhCorDS->Write();
  pReconPhCrtDS->Write();

  outFile->cd("Coincidence/ProjectileUS");

  sPidUS->Write();
  rPidUS->Write();

  pCoreEnergyUS->Write();
  pCoreSumUS->Write();

  pCoreEnergyUS_fep->Write();
  pCoreEnergyUS_pfep->Write();
  pCoreEnergyUS_rfep->Write();
  pCoreEnergyUS_nfep->Write();

  outFile->cd("Coincidence/ProjectileUS/Doppler");
  
  pDopEnergyUS->Write();
  pDopSumUS->Write();
  
  pDopEnergyUS_fep->Write();
  pDopEnergyUS_pfep->Write();
  pDopEnergyUS_rfep->Write();
  pDopEnergyUS_nfep->Write();
  
  pThCorUS->Write();
  pThCrtUS->Write();

  pPhCorUS->Write();
  pPhCrtUS->Write();

  outFile->cd("Coincidence/ProjectileUS/Recon");

  pReconEnergyUS->Write();
  pReconSumUS->Write();
  
  pReconEnergyUS_fep->Write();
  pReconEnergyUS_pfep->Write();
  pReconEnergyUS_rfep->Write();
  pReconEnergyUS_nfep->Write();
  
  pReconThCorUS->Write();
  pReconThCrtUS->Write();

  pReconPhCorUS->Write();
  pReconPhCrtUS->Write();
  
  outFile->cd("Coincidence/Recoil");

  sPidRec->Write();
  rPidRec->Write();

  rCoreEnergy->Write();
  rCoreSum->Write();

  rCoreEnergy_fep->Write();
  rCoreEnergy_pfep->Write();
  rCoreEnergy_rfep->Write();
  rCoreEnergy_nfep->Write();

  outFile->cd("Coincidence/Recoil/Doppler");
  
  rDopEnergy->Write();
  rDopSum->Write();
  
  rDopEnergy_fep->Write();
  rDopEnergy_pfep->Write();
  rDopEnergy_rfep->Write();
  rDopEnergy_nfep->Write();
  
  rThCor->Write();
  rThCrt->Write();

  rPhCor->Write();
  rPhCrt->Write();

  outFile->cd("Coincidence/Recoil/Recon");

  rReconEnergy->Write();
  rReconSum->Write();
  
  rReconEnergy_fep->Write();
  rReconEnergy_pfep->Write();
  rReconEnergy_rfep->Write();
  rReconEnergy_nfep->Write();
  
  rReconThCor->Write();
  rReconThCrt->Write();

  rReconPhCor->Write();
  rReconPhCrt->Write();
  
  outFile->Close();
  std::cout << "Done!" << std::endl;

  return 0;
}
