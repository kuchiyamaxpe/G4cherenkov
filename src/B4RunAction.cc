//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B4RunAction.cc
/// \brief Implementation of the B4RunAction class

#include "B4RunAction.hh"
#include "B4Analysis.hh"
#include "Randomize.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "B4PrimaryGeneratorAction.hh"
#include <sys/stat.h> 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4RunAction::B4RunAction()
    : G4UserRunAction(),
      fMessenger(nullptr),
      dirname("Data"),
      outputname("Test.txt"),
      energy(1500 * MeV)
{
  DefineCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B4RunAction::~B4RunAction()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4RunAction::BeginOfRunAction(const G4Run * /*run*/)
{

  if (IsMaster())
  {
    auto runmanager = G4RunManager::GetRunManager();

    std::cout << "Master Thread Start" << std::endl;
    struct stat st;

    // auto dirname_trackinfo = dirname + "_TrackInfo";
    auto dirname_trackinfo = GetTrackInfoDirName();
    auto dirname_stepinfo = GetStepInfoDirName();
    int ret = stat(dirname.c_str(), &st);

    if (0 == ret)
    {
      std::cout << "Data Directory is already existed" << std::endl;
    }
    else
    {
      std::cout << "make Data Directory" << std::endl;
      mkdir(dirname.c_str(), 0777);
    }

    if (0 == stat(dirname_trackinfo.c_str(), &st))
    {
      std::cout << "Data Directory is already existed" << std::endl;
    }
    else
    {
      std::cout << "make Data Directory" << std::endl;
      mkdir(dirname_trackinfo.c_str(), 0777);
    }


    auto Nevent = runmanager->GetCurrentRun()->GetNumberOfEventToBeProcessed();

    auto filename = outputname;
    std::ofstream writing_file;
    writing_file.open(dirname + "/" + filename, std::ios::trunc);
    writing_file << "########## GeneratedPhotonInfo ##########" << std::endl;
    writing_file << "seed: " << G4Random::getTheSeed() << std::endl;
    writing_file << "TotalProton: " << Nevent << std::endl;
    writing_file << "SettingEnergy[MeV]: " << energy << std::endl;
    writing_file << "########## PhotonData ##########" << std::endl;
    writing_file.close();

    ///.................Writing Track Info...........................

    writing_file.open(dirname_trackinfo + "/" + filename, std::ios::trunc);
    writing_file << "########## GeneratedPhotonInfo ##########" << std::endl;
    writing_file << "seed: " << G4Random::getTheSeed() << std::endl;
    writing_file << "TotalProton: " << Nevent << std::endl;
    writing_file << "SettingEnergy[MeV]: " << energy << std::endl;
    writing_file << "########## TrackData ##########" << std::endl;
    writing_file.close();

    ///.................Writing Step Info...........................

    writing_file.open(dirname_stepinfo + "/" + filename, std::ios::trunc);
    writing_file << "########## GeneratedPhotonInfo ##########" << std::endl;
    writing_file << "seed: " << G4Random::getTheSeed() << std::endl;
    writing_file << "TotalProton: " << Nevent << std::endl;
    writing_file << "SettingEnergy[MeV]: " << energy << std::endl;
    writing_file << "########## Si Detector Data ##########" << std::endl;
    writing_file.close();

    ///.................Writing Step Info...........................
  }
  else
  {
    auto runmanager = G4RunManager::GetRunManager();

    G4VUserPrimaryGeneratorAction *pgene = const_cast<G4VUserPrimaryGeneratorAction *>(runmanager->GetUserPrimaryGeneratorAction());
    B4PrimaryGeneratorAction *pgenec = dynamic_cast<B4PrimaryGeneratorAction *>(pgene);
    pgenec->SetEnergy(energy);
  }
  // inform the runManager to save random number seed
  // G4RunManager::GetRunManager()->SetRandomNumberStore(true);

  // Get analysis manager
  // auto analysisManager = G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4RunAction::EndOfRunAction(const G4Run * /*run*/)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B4RunAction::DefineCommands()
{

  fMessenger = new G4GenericMessenger(this,
                                      "/G4Cherenkov/runaction/",
                                      "Primary stepping action control");
  auto &outdirCmd = fMessenger->DeclareProperty("dirname", dirname,
                                                "output file name.");

  outdirCmd.SetParameterName("t", true);

  auto &outnameCmd = fMessenger->DeclareProperty("outputname", outputname,
                                                 "output file name.");

  outnameCmd.SetParameterName("t", true);
  outnameCmd.SetDefaultValue("test.txt");

  // momentum command
  auto &momentumCmd = fMessenger->DeclarePropertyWithUnit("energy", "MeV", energy,
                                                          "Mean momentum of primaries.");

  momentumCmd.SetParameterName("p", true);
  momentumCmd.SetRange("p>=0.");
  momentumCmd.SetDefaultValue("0.5");
}