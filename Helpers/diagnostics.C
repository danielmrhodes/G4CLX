#include "/home/rhodes25/programs/G4CLX/include/Data_Format.hh"
void diagnostics(std::string input_filename = "info.dat",
		 std::string output_filename = "info.root") {

  if(!strcmp(input_filename.c_str(),output_filename.c_str())) {
    std::cout << "Give your input and output files different names" << std::endl;
    return;
  }

  const int nStatesP = 20;
  const int nStatesR = 5;

  TH1* hevt = new TH1D("hevt","Event Number",10000,0.0,50000000.0);
  
  TH1* htCM = new TH1D("htCM","Theta CM",360,0.0,180.0);
  TH1* hEn = new TH1D("hEn","Reaction Energy",2000,0.0,1000.0);
  TH2* htCM_En = new TH2D("htCM_En","Reaction Energy vs. Theta CM",360,0.0,180.0,1000,0.0,1000.);
  
  TH1* hp = new TH1D("hp","Projectile population",nStatesP,0,nStatesP);
  TH1* hr = new TH1D("hr","Recoil population",nStatesR,0,nStatesR);

  TH1* htCM_pds = new TH1D("htCM_pds","Theta CM, Projectile DS Gate",360,0.0,180.0);
  TH1* hEn_pds = new TH1D("hEn_pds","Reaction Energy, Projectile DS Gate",2000,0.0,1000.0);
  TH2* htCM_En_pds = new TH2D("htCM_En_pds","Reaction Energy vs. Theta CM, Projectile DS Gate",360,0.0,180.0,1000,0.0,1000.);

  TH1* hp_pds = new TH1D("hp_pds","Projectile population, Projectile DS Gate",nStatesP,0,nStatesP);
  TH1* hr_pds = new TH1D("hr_pds","Recoil population, Projectile DS Gate",nStatesR,0,nStatesR);

  TH1* htCM_pus = new TH1D("htCM_pus","Theta CM, Projectile US Gate",360,0.0,180.0);
  TH1* hEn_pus = new TH1D("hEn_pus","Reaction Energy, Projectile US Gate",2000,0.0,1000.0);
  TH2* htCM_En_pus = new TH2D("htCM_En_pus","Reaction Energy vs. Theta CM, Projectile US Gate",360,0.0,180.0,1000,0.0,1000.);

  TH1* hp_pus = new TH1D("hp_pus","Projectile population, Projectile US Gate",nStatesP,0,nStatesP);
  TH1* hr_pus = new TH1D("hr_pus","Recoil population, Projectile US Gate",nStatesR,0,nStatesR);

  TH1* htCM_rec = new TH1D("htCM_rec","Theta CM, Recoil Gate",360,0.0,180.0);
  TH1* hEn_rec = new TH1D("hEn_rec","Reaction Energy, Recoil Gate",2000,0.0,1000.0);
  TH2* htCM_En_rec = new TH2D("htCM_En_rec","Reaction Energy vs. Theta CM, Recoil Gate",360,0.0,180.0,1000,0.0,1000.);

  TH1* hp_rec = new TH1D("hp_rec","Projectile population, Recoil Gate",nStatesP,0,nStatesP);
  TH1* hr_rec = new TH1D("hr_rec","Recoil population, Recoil Gate",nStatesR,0,nStatesR);

  TH1* htCM_both = new TH1D("htCM_both","Theta CM, Projectile and Recoil DS Gate",360,0.0,180.0);
  TH1* hEn_both = new TH1D("hEn_both","Reaction Energy, Projectile and Recoil DS Gate",2000,0.0,1000.0);
  TH2* htCM_En_both = new TH2D("htCM_En_both","Reaction Energy vs. Theta CM, Projectile and Recoil DS Gate",360,0.0,180.0,1000,0.0,1000.);

  TH1* hp_both = new TH1D("hp_both","Projectile population, Projectile and Recoil DS",nStatesP,0,nStatesP);
  TH1* hr_both = new TH1D("hr_both","Recoil population, Projectile and Recoil DS Gate",nStatesR,0,nStatesR);

  //TH3* hp3 = new TH3D("hp3","Projectile population",360,0,360,600,200,800,nStatesP,0,nStatesP);
  //TH3* hr3 = new TH3D("hr3","Recoil population",360,0,360,600,200,800,nStatesR,0,nStatesR);

  std::vector<TH2*> pGrids;
  std::vector<TH2*> pGrids_pds;
  std::vector<TH2*> pGrids_pus;
  std::vector<TH2*> pGrids_rec;
  std::vector<TH2*> pGrids_both;
  for(int i=0;i<nStatesP;i++) {
    pGrids.push_back(new TH2D(Form("pG%02d",i),Form("Projectile State %02d Population",i),
			      360,0.0,180.0,1000,0.0,1000.0));

    pGrids_pds.push_back(new TH2D(Form("pG_pds%02d",i),
				  Form("Projectile State %02d Population, Projectile DS Gate",i),
				  360,0.0,180.0,1000,0.0,1000.0));

    pGrids_pus.push_back(new TH2D(Form("pG_pus%02d",i),
				  Form("Projectile State %02d Population, Projectile US Gate",i),
				  360,0.0,180.0,1000,0.0,1000.0));

    pGrids_rec.push_back(new TH2D(Form("pG_rec%02d",i),
				  Form("Projectile State %02d Population, Recoil Gate",i),
				  360,0.0,180.0,1000,0.0,1000.0));

    pGrids_both.push_back(new TH2D(Form("pG_both%02d",i),
				  Form("Projectile State %02d Population, Projectile and Recoil DS Gate",i),
				  360,0.0,180.0,1000,0.0,1000.0));
  }

  std::vector<TH2*> rGrids;
  std::vector<TH2*> rGrids_pds;
  std::vector<TH2*> rGrids_pus;
  std::vector<TH2*> rGrids_rec;
  std::vector<TH2*> rGrids_both;
  for(int i=0;i<nStatesR;i++) {
    rGrids.push_back(new TH2D(Form("rG%02d",i),Form("Recoil State %02d Population",i),
			      360,0.0,180.0,1000,0.0,1000.0));

    rGrids_pds.push_back(new TH2D(Form("rG_pds%02d",i),
				  Form("Recoil State %02d Population, Projectile DS Gate",i),
				  360,0.0,180.0,1000,0.0,1000.0));

    rGrids_pus.push_back(new TH2D(Form("rG_pus%02d",i),
				  Form("Recoil State %02d Population, Projectile US Gate",i),
				  360,0.0,180.0,1000,0.0,1000.0));

    rGrids_rec.push_back(new TH2D(Form("rG_rec%02d",i),
				  Form("Recoil State %02d Population, Recoil Gate",i),
				  360,0.0,180.0,1000,0.0,1000.0));
    
    rGrids_both.push_back(new TH2D(Form("rG_both%02d",i),
				  Form("Recoil State %02d Population, Projectile and Recoil DS Gate",i),
				  360,0.0,180.0,1000,0.0,1000.0));
  }
  
  FILE* input_file = fopen(input_filename.c_str(),"rb");
  
  INFO info;
  while(fread(&info,info.bytes(),1,input_file)) {

    const int num = info.evtNum;
    const int pIndex = info.indexP;
    const int rIndex = info.indexR;
    const double energy = info.beamEn;
    const double thetaCM = info.thetaCM;
    
    const bool prjDS = info.projDS;
    const bool prjUS = info.projUS;
    const bool rec = info.rec;
    
    hevt->Fill(num);
    hEn->Fill(energy);
    htCM->Fill(thetaCM);
    htCM_En->Fill(thetaCM,energy);
    
    hp->Fill(pIndex);
    hr->Fill(rIndex);

    //hp3->Fill(thetaCM,energy,pIndex);
    //hr3->Fill(thetaCM,energy,rIndex);

    if(pIndex < nStatesP)
      pGrids.at(pIndex)->Fill(thetaCM,energy);

    if(rIndex < nStatesR)
      rGrids.at(rIndex)->Fill(thetaCM,energy);

    if(prjDS) {

      htCM_pds->Fill(thetaCM);
      hEn_pds->Fill(energy);
      htCM_En_pds->Fill(thetaCM,energy);

      hp_pds->Fill(pIndex);
      hr_pds->Fill(rIndex);
      
      if(pIndex < nStatesP)
	pGrids_pds.at(pIndex)->Fill(thetaCM,energy);

      if(rIndex < nStatesR)
	rGrids_pds.at(rIndex)->Fill(thetaCM,energy);
      
    }

    if(prjUS) {

      htCM_pus->Fill(thetaCM);
      hEn_pus->Fill(energy);
      htCM_En_pus->Fill(thetaCM,energy);
      
      hp_pus->Fill(pIndex);
      hr_pus->Fill(rIndex);
      
      if(pIndex < nStatesP)
	pGrids_pus.at(pIndex)->Fill(thetaCM,energy);

      if(rIndex < nStatesR)
	rGrids_pus.at(rIndex)->Fill(thetaCM,energy);
      
    }

    if(rec) {

      htCM_rec->Fill(thetaCM);
      hEn_rec->Fill(energy);
      htCM_En_rec->Fill(thetaCM,energy);
      
      hp_rec->Fill(pIndex);
      hr_rec->Fill(rIndex);

      if(pIndex < nStatesP)
	pGrids_rec.at(pIndex)->Fill(thetaCM,energy);

      if(rIndex < nStatesR)
	rGrids_rec.at(rIndex)->Fill(thetaCM,energy);
      
    }

    if(prjDS && rec) {
      
      htCM_both->Fill(thetaCM);
      hEn_both->Fill(energy);
      htCM_En_both->Fill(thetaCM,energy);

      hp_both->Fill(pIndex);
      hr_both->Fill(rIndex);

      if(pIndex < nStatesP)
	pGrids_both.at(pIndex)->Fill(thetaCM,energy);

      if(rIndex < nStatesR)
	rGrids_both.at(rIndex)->Fill(thetaCM,energy);
    }
  }

  TFile* outFile = new TFile(output_filename.c_str(),"RECREATE");
  outFile->cd();

  outFile->mkdir("Grids");
  
  outFile->mkdir("ProjectileDS");
  outFile->mkdir("ProjectileDS/Grids");

  outFile->mkdir("ProjectileUS");
  outFile->mkdir("ProjectileUS/Grids");

  outFile->mkdir("Recoil");
  outFile->mkdir("Recoil/Grids");

  outFile->mkdir("Both");
  outFile->mkdir("Both/Grids");
  
  hevt->Write();
  htCM->Write();
  hEn->Write();
  htCM_En->Write();
  
  hp->Write();
  hr->Write();

  //hp3->Write();
  //hr3->Write();

  outFile->cd("Grids");
  for(auto h : pGrids)
    h->Write();

  for(auto h : rGrids)
    h->Write();

  outFile->cd("ProjectileDS");
  htCM_pds->Write();
  hEn_pds->Write();
  htCM_En_pds->Write();
  hp_pds->Write();
  hr_pds->Write();

  outFile->cd("ProjectileDS/Grids");
  for(auto h : pGrids_pds)
    h->Write();

  for(auto h : rGrids_pds)
    h->Write();

  outFile->cd("ProjectileUS");
  htCM_pus->Write();
  hEn_pus->Write();
  htCM_En_pus->Write();
  hp_pus->Write();
  hr_pus->Write();
  
  outFile->cd("ProjectileUS/Grids");
  for(auto h : pGrids_pus)
    h->Write();

  for(auto h : rGrids_pus)
    h->Write();

  outFile->cd("Recoil");
  htCM_rec->Write();
  hEn_rec->Write();
  htCM_En_rec->Write();
  hp_rec->Write();
  hr_rec->Write();

  outFile->cd("Recoil/Grids");
  for(auto h : pGrids_rec)
    h->Write();

  for(auto h : rGrids_rec)
    h->Write();

  outFile->cd("Both");
  htCM_both->Write();
  hEn_both->Write();
  htCM_En_both->Write();
  hp_both->Write();
  hr_both->Write();

  outFile->cd("Both/Grids");
  for(auto h : pGrids_both)
    h->Write();

  for(auto h : rGrids_both)
    h->Write();
  
  outFile->Close();

  delete hevt;
  delete htCM;
  delete hEn;
  delete htCM_En;
  
  delete hp;
  delete hr;

  for(auto h : pGrids)
    delete h;

  for(auto h : rGrids)
    delete h;

  delete htCM_pds;
  delete hEn_pds;
  delete htCM_En_pds;
  
  delete hp_pds;
  delete hr_pds;

  for(auto h : pGrids_pds)
    delete h;

  for(auto h : rGrids_pds)
    delete h;

  delete htCM_pus;
  delete hEn_pus;
  delete htCM_En_pus;
  
  delete hp_pus;
  delete hr_pus;

  for(auto h : pGrids_pus)
    delete h;

  for(auto h : rGrids_pus)
    delete h;

  delete htCM_rec;
  delete hEn_rec;
  delete htCM_En_rec;
  
  delete hp_rec;
  delete hr_rec;

  for(auto h : pGrids_rec)
    delete h;

  for(auto h : rGrids_rec)
    delete h;

  delete htCM_both;
  delete hEn_both;
  delete htCM_En_both;
  
  delete hp_both;
  delete hr_both;

  for(auto h : pGrids_both)
    delete h;

  for(auto h : rGrids_both)
    delete h;
  
  return;
}

