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

  //ChicoX Singles
  TH1* chico_mult = new TH1D("chico_mult","ChicoX Multiplicity",10,0,10);
  TH2* chico_sum = new TH2D("chico_sum","ChicoX Summary Spectrum",20,1,21,10000,0.0,1000.0);

  TH2* chico_kin = new TH2D("chico_kin","ChicoX Energy vs Angle",720,0.0,180.,2000,0.0,1000.0);
  TH2* chico_kin_proj = new TH2D("chico_kin_proj","ChicoX Energy vs Angle, Projectile Gate",720,0.0,180.,2000,0.0,1000.0);
  TH2* chico_kin_rec = new TH2D("chico_kin_rec","ChicoX Energy vs Angle, Recoil Gate",720,0.0,180.,2000,0.0,1000.0);

  TH2* chicoTP = new TH2D("chicoTP","ChicoX Theta-Phi Surface",720,0.0,180.0,1440,0.0,360.0);
  TH2* chicoTP_proj = new TH2D("chicoTP_proj","ChicoX Theta-Phi Surface, Projectile Gate",720,0.0,180.0,1440,0.0,360.0);
  TH2* chicoTP_rec = new TH2D("chicoTP_rec","ChicoX Theta-Phi Surface, Recoil Gate",720,0.0,180.0,1440,0.0,360.0);

  TH2* chicoTP_m1 = new TH2D("chicoTP_m1","ChicoX Theta-Phi Surface Mult 1",720,0.0,180.0,1440,0.0,360.0);
  TH2* chicoTP_m1_proj = new TH2D("chicoTP_m1_proj","ChicoX Theta-Phi Surface Mult 1, Projectile Gate",720,0.0,180.0,1440,0.0,360.0);
  TH2* chicoTP_m1_rec = new TH2D("chicoTP_m1_rec","ChicoX Theta-Phi Surface Mult 1, Recoil Gate",720,0.0,180.0,1440,0.0,360.0);

  TH2* chicoTR = new TH2D("chicoTR","ChicoX Theta-R Surface",720,0.0,180.0,1000,10,20);
  TH2* chicoPR = new TH2D("chicoPR","ChicoX Phi-R Surface",1440,0.0,360.0,1000,10,20);

  //ChicoX Mult2
  TH2* chico_m2 = new TH2D("chico_m2","ChicoX Mult2 Hit Pattern",20,1,21,20,1,21);
  TH1* chico_dphi = new TH1D("chico_dphi","ChicoX Mult2 Phi Difference",720,0.0,360.0);
  
  TH2* chicoTP_m2 = new TH2D("chicoTP_m2","ChicoX Mult2 Theta-Phi Surface (Both Particles)",720,0.0,180.0,1440,0.0,360.0);
  TH2* chicoTP_m2_proj = new TH2D("chicoTP_m2_proj","ChicoX Mult2 Theta-Phi Surface, Projectile Gate",720,0.0,180.0,1440,0.0,360.0);
  TH2* chicoTP_m2_rec = new TH2D("chicoTP_m2_rec","ChicoX Mult2 Theta-Phi Surface, Recoil Gate",720,0.0,180.0,1440,0.0,360.0);

  TH2* chico_pid = new TH2D("chico_pid","ChicoX Time Differnce vs Scattering Angle",720,0.0,90.0,10000,-250,250);
  TH2* chico_pid_proj = new TH2D("chico_pid_proj","ChicoX Time Differnce vs Scattering Angle, Projectile Gate",720,0.0,90.0,10000,-250,250);
  TH2* chico_pid_rec = new TH2D("chico_pid_rec","ChicoX Time Differnce vs Scattering Angle, Recoil Gate",720,0.0,90.0,10000,-250,250);

  TH2* chico_pid2 = new TH2D("chico_pid2","ChicoX Time Differnce vs Scattering Angle Difference",720,-90.0,90.0,10000,-250,250);
  TH2* chico_pid2_proj = new TH2D("chico_pid2_proj","ChicoX Time Differnce vs Scattering Angle Difference, Projectile Gate",
                                  720,-90.0,90.0,10000,-250,250);
  TH2* chico_pid2_rec = new TH2D("chico_pid2_rec","ChicoX Time Differnce vs Scattering Angle Difference, Recoil Gate",720,-90.0,90.0,10000,-250,250);

  TH2* chico_ang = new TH2D("chico_ang","ChicoX Angles",720,0.0,90.0,720,0.0,90.0);
  TH2* chico_ang_proj = new TH2D("chico_ang_proj","ChicoX Angles, Projectile Gate",720,0.0,90.0,720,0.0,90.0);
  TH2* chico_ang_rec = new TH2D("chico_ang_rec","ChicoX Angles, Recoil Gate",720,0.0,90.0,720,0.0,90.0);
  
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
  //const double Sol2_En = KE_LAB(Theta_CM_FP(Theta_LAB_Max(beam_en),beam_en),beam_en);
  const double r2d = TMath::RadToDeg();

  const double sigma_theta = 1.56*TMath::DegToRad()/2.355; //rad
  const double sigma_phi = 2.47*TMath::DegToRad()/2.355; //rad
  const double sigma_tm = 1.2/2.355; //ns

  const double sigma_pos = 0.12; //cm

  TRandom* rand = new TRandom(50747227);
  std::cout << "Correlating and histograming data..." << std::endl;
  
  Header header;
  RawDataX raw_data;
  while(fread(&header,header.bytes(),1,input_file)) {

    const int nE = header.evtNum;
    const int nC = header.nSdata;
    const int nG = header.nGdata;

    if(nC > 2) {
      std::cout << "Too many PPAC hits (" << nC << ") in event " << nE << "! Ending sort..." << std::endl;
      break;
    }
    if(nG > 100) {
      std::cout << "Too many Greta hits (" << nG << ") in event " << nE << "! Ending sort..." << std::endl;
      break;
    }
  
    fread(&raw_data.cData,sizeof(ChicoXData),nC,input_file);
    fread(&raw_data.gData,sizeof(GretaData),nG,input_file);
    BuiltDataX data = BuildDataX(nE,nC,nG,raw_data);

    //ChicoX
    chico_mult->Fill(data.nCh);
    for(int i=0;i<data.nCh;i++) {

      ChicoXData chic = data.chico[i];
      int id = chic.id;
      float en = chic.en;
      //float tm = rand->Gaus(chic.tm,sigma_tm);
      float x = chic.x;
      float y = chic.y;
      float z = chic.z;
      bool proj = chic.proj;
      bool rec = chic.rec;
      
      TVector3 pos(x,y,z);
      //pos.SetTheta(rand->Gaus(pos.Theta(),sigma_theta));
      //pos.SetPhi(rand->Gaus(pos.Phi(),sigma_phi));
      pos.SetX(pos.X() - beam_X);
      pos.SetY(pos.Y() - beam_Y); 

      double mag = pos.Mag();
      double theta = pos.Theta()*r2d;
      double phi = pos.Phi();
      if(phi < 0.0)
	  phi += TMath::TwoPi();
	phi *= r2d;

      chico_sum->Fill(id,en);
      chico_kin->Fill(theta,en);

      chicoTP->Fill(theta,phi);
      chicoTR->Fill(theta,mag);
      chicoPR->Fill(phi,mag);

      if(proj) {
	chico_kin_proj->Fill(theta,en);
	chicoTP_proj->Fill(theta,phi);
      }
      if(rec) {
	chico_kin_rec->Fill(theta,en);
	chicoTP_rec->Fill(theta,phi);
      }

      if(data.nCh == 1 && id < 11) {
        chicoTP_m1->Fill(theta,phi);
      
        if(proj)
          chicoTP_m1_proj->Fill(theta,phi);
        if(rec)
          chicoTP_m1_rec->Fill(theta,phi);
      }
    
    } //End ChicoX singles

    //ChicoX Mult2
    if(data.nCh == 2) {
      
      ChicoXData chic0 = data.chico[0];
      ChicoXData chic1 = data.chico[1];

      int id0 = chic0.id;
      int id1 = chic1.id;
      if(id0 < 11 && id1 < 11) { //Downstream
	
	if(id1 < 6) { //Use boards 1-5 as reference
	  std::swap(id0,id1);
	  chic1 = data.chico[0];
	  chic0 = data.chico[1];
	}

	bool proj0 = chic0.proj;
	bool rec0 = chic0.rec;
	double tm0 = rand->Gaus(chic0.tm,sigma_tm);
	double tm1 = rand->Gaus(chic1.tm,sigma_tm);

	TVector3 pos0(chic0.x,chic0.y,chic0.z);
	pos0.SetTheta(rand->Gaus(pos0.Theta(),sigma_theta));
	pos0.SetPhi(rand->Gaus(pos0.Phi(),sigma_phi));
	pos0.SetX(pos0.X() - beam_X);
	pos0.SetY(pos0.Y() - beam_Y);

	double theta0 = pos0.Theta()*r2d;
	double phi0 = pos0.Phi();
	if(phi0 < 0.0)
	  phi0 += TMath::TwoPi();
	phi0 *= r2d;
	
	TVector3 pos1(chic1.x,chic1.y,chic1.z);
	pos1.SetTheta(rand->Gaus(pos1.Theta(),sigma_theta));
	pos1.SetPhi(rand->Gaus(pos1.Phi(),sigma_phi));
	pos1.SetX(pos1.X() - beam_X);
	pos1.SetY(pos1.Y() - beam_Y);

	double theta1 = pos1.Theta()*r2d;
	double phi1 = pos1.Phi();
	if(phi1 < 0.0)
	  phi1 += TMath::TwoPi();
	phi1 *= r2d;
	
	double tdiff = tm0 - tm1;
	double phi_diff = phi0 - phi1;
        double theta_diff = theta0 - theta1;

	chico_m2->Fill(id0,id1);
	chico_m2->Fill(id1,id0);
	chicoTP_m2->Fill(theta0,phi0);
	chicoTP_m2->Fill(theta1,phi1);
	
	chico_dphi->Fill(std::abs(phi_diff));
	chico_pid->Fill(theta0,tdiff);
	chico_pid2->Fill(theta_diff,tdiff);
        chico_ang->Fill(theta0,theta1);

	if(proj0) {

          chicoTP_m2_proj->Fill(theta0,phi0); 
	  chico_pid_proj->Fill(theta0,tdiff);
 	  chico_pid2_proj->Fill(theta_diff,tdiff);
	  chico_ang_proj->Fill(theta0,theta1);
	
        }
	if(rec0) {
	  
          chicoTP_m2_rec->Fill(theta0,phi0);
          chico_pid_rec->Fill(theta0,tdiff);
	  chico_pid2_rec->Fill(theta_diff,tdiff);
	  chico_ang_rec->Fill(theta0,theta1);
	
        }

      }
      
    } //End ChicoX Mult2

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
    
    //Chico Mult2 Coincidences
    if(data.nCh == 2 && data.nGr > 0) {
      
      ChicoXData chic0 = data.chico[0];
      ChicoXData chic1 = data.chico[1];

      int id0 = chic0.id;
      int id1 = chic1.id;
      if(id0 < 11 && id1 < 11) { //Downstream
	
	if(id1 < 6) { //Use boards 1-5 as reference
	  std::swap(id0,id1);
	  chic1 = data.chico[0];
	  chic0 = data.chico[1];
	}

	bool proj = chic0.proj;
	bool rec = chic0.rec;
	//double tm = chic0.tm;
	
	TVector3 posC(chic0.x,chic0.y,chic0.z);
	posC.SetTheta(rand->Gaus(posC.Theta(),sigma_theta));
	posC.SetPhi(rand->Gaus(posC.Phi(),sigma_phi));
	posC.SetX(posC.X() - beam_X);
	posC.SetY(posC.Y() - beam_Y);	

	if(proj) { //Projectile DS gate

	  bool sol2 = false;
	  //if(sec_en < Sol2_En)
	  //sol2 = true;
	  
	  double thetaCM = Theta_CM_FP(posC.Theta(),beam_en,sol2);
	  
	  double energy = KE_LAB(thetaCM,beam_en);
	  double gam = (energy/beam_mass) + 1.0;
	  double beta = TMath::Sqrt(1.0 - 1.0/(gam*gam));
	  
	  double recon_energy = Recoil_KE_LAB(thetaCM,beam_en);
	  double recon_gam = (recon_energy)/targ_mass + 1.0;
	  double recon_beta = TMath::Sqrt(1.0 - 1.0/(recon_gam*recon_gam));
	  
	  TVector3 rPos(0,0,1); //Reconstructed position of the recoil
	  rPos.SetTheta(Recoil_Theta_LAB(thetaCM,beam_en));
	  rPos.SetPhi(posC.Phi() - TMath::Pi());
	  
	  for(int j=0;j<data.nGr;j++) {

	    int det = data.greta[j].det;
	    //int seg = data.greta[j].MainSeg();
	    
	    double en = data.greta[j].cEn;
	    double coreEn = rand->Gaus(en,Sigma(en));
	    
	    bool FEP = data.greta[j].fep;
	    bool PFEP = data.greta[j].pfep;
	  
	    //TVector3 sPos = GetPos(det,seg);
	    TVector3 sPos = data.greta[j].MainPos();
	    sPos.SetXYZ(rand->Gaus(sPos.X(),sigma_pos) - beam_X,
			rand->Gaus(sPos.Y(),sigma_pos) - beam_Y,
			rand->Gaus(sPos.Z(),sigma_pos));

	    double theta = posC.Angle(sPos);
	    double dopEn = gam*(1 - beta*TMath::Cos(theta))*coreEn;
	    
	    TVector3 reacPlane = posC.Cross(incBeam);
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

	if(rec) { //Recoil gate

	  double thetaCM = Theta_CM_FR(posC.Theta(),beam_en);
	  
	  double energy = Recoil_KE_LAB(thetaCM,beam_en);
	  double gam = (energy)/targ_mass + 1.0;
	  double beta = TMath::Sqrt(1.0 - 1.0/(gam*gam));

	  double recon_energy = KE_LAB(thetaCM,beam_en);
	  double recon_gam = (recon_energy)/beam_mass + 1.0;
	  double recon_beta = TMath::Sqrt(1.0 - 1.0/(recon_gam*recon_gam));

	  TVector3 rPos(0,0,1); //Reconstructed position of the projectile
	  rPos.SetTheta(Theta_LAB(thetaCM,beam_en));
	  rPos.SetPhi(posC.Phi() - TMath::Pi());

	  for(int j=0;j<data.nGr;j++) {

	    int det = data.greta[j].det;
	    //int seg = data.greta[j].MainSeg();
	    double en = data.greta[j].cEn;
	    double coreEn = rand->Gaus(en,Sigma(en));
	    bool FEP = data.greta[j].fep;
	    bool PFEP = data.greta[j].pfep;
	  
	    //TVector3 sPos = GetPos(det,seg);
	    TVector3 sPos = data.greta[j].MainPos();
	    sPos.SetXYZ(rand->Gaus(sPos.X(),sigma_pos) - beam_X,
			rand->Gaus(sPos.Y(),sigma_pos) - beam_Y,
			rand->Gaus(sPos.Z(),sigma_pos));

	    double theta = posC.Angle(sPos);
	    double dopEn = gam*(1 - beta*TMath::Cos(theta))*coreEn;
	  
	    TVector3 reacPlane = posC.Cross(incBeam);
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
      } //End DS Gate
    } //End Mult2 Coincidences
    
    for(int i=0;i<data.nCh;i++) {
      
      ChicoXData chic = data.chico[i];
      int id = chic.id;
      if(id < 11) //Upstream gate
	continue;

      //float en = chic.en;
      //float tm = chic.tm;
      //bool proj = chic.proj;
      //bool rec = chic.rec;
      
      TVector3 posC(chic.x,chic.y,chic.z);
      posC.SetTheta(rand->Gaus(posC.Theta(),sigma_theta));
      posC.SetPhi(rand->Gaus(posC.Phi(),sigma_phi));
      posC.SetX(posC.X() - beam_X);
      posC.SetY(posC.Y() - beam_Y);
	  
      double thetaCM = Theta_CM_FP(posC.Theta(),beam_en,false);
	
      double energy = KE_LAB(thetaCM,beam_en);
      double gam = (energy)/beam_mass + 1.0;
      double beta = TMath::Sqrt(1.0 - 1.0/(gam*gam));
	
      double recon_energy = Recoil_KE_LAB(thetaCM,beam_en);
      double recon_gam = (recon_energy)/targ_mass + 1.0;
      double recon_beta = TMath::Sqrt(1.0 - 1.0/(recon_gam*recon_gam));
	
      TVector3 rPos(0,0,1); //Reconstructed position of the recoil
      rPos.SetTheta(Recoil_Theta_LAB(thetaCM,beam_en));
      rPos.SetPhi(posC.Phi() - TMath::Pi());
	
      for(int j=0;j<data.nGr;j++) {
	  
	int det = data.greta[j].det;
	//int seg = data.greta[j].MainSeg();
	double en = data.greta[j].cEn;
	double coreEn = rand->Gaus(en,Sigma(en));
	bool FEP = data.greta[j].fep;
	bool PFEP = data.greta[j].pfep;
	  
	//TVector3 sPos = GetPos(det,seg);
	TVector3 sPos = data.greta[j].MainPos();
	sPos.SetXYZ(rand->Gaus(sPos.X(),sigma_pos) - beam_X,
			rand->Gaus(sPos.Y(),sigma_pos) - beam_Y,
			rand->Gaus(sPos.Z(),sigma_pos));

	double theta = posC.Angle(sPos);
	double dopEn = gam*(1 - beta*TMath::Cos(theta))*coreEn;
	  
	TVector3 reacPlane = posC.Cross(incBeam);
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
    } //End ChicoX loop
  } //End while loop
  std::cout << "Writing histograms to file..." << std::endl;

  TFile* outFile = new TFile(output_filename,"RECREATE");
  
  outFile->mkdir("Greta");
  outFile->mkdir("ChicoX");
  
  outFile->mkdir("Coincidence/ProjectileDS");
  outFile->mkdir("Coincidence/ProjectileDS/Doppler");
  outFile->mkdir("Coincidence/ProjectileDS/Recon");
  
  outFile->mkdir("Coincidence/ProjectileUS");
  outFile->mkdir("Coincidence/ProjectileUS/Doppler");
  outFile->mkdir("Coincidence/ProjectileUS/Recon");
  
  outFile->mkdir("Coincidence/Recoil");
  outFile->mkdir("Coincidence/Recoil/Doppler");
  outFile->mkdir("Coincidence/Recoil/Recon");

  outFile->cd("ChicoX");
  
  chico_mult->Write();
  chico_sum->Write();
  
  chico_kin->Write();  
  chico_kin_proj->Write();
  chico_kin_rec->Write();

  chicoTP->Write();
  chicoTP_proj->Write();
  chicoTP_rec->Write();
  
  chicoTP_m1->Write();
  chicoTP_m1_proj->Write();
  chicoTP_m1_rec->Write();

  chicoTR->Write();
  chicoPR->Write();
  
  chico_m2->Write();
  chico_dphi->Write();

  chicoTP_m2->Write();
  chicoTP_m2_proj->Write();
  chicoTP_m2_rec->Write();
  
  chico_pid->Write();
  chico_pid_proj->Write();
  chico_pid_rec->Write();
 
  chico_pid2->Write();
  chico_pid2_proj->Write();
  chico_pid2_rec->Write();
 
  chico_ang->Write();
  chico_ang_proj->Write();
  chico_ang_rec->Write();
  
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
