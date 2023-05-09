#include "G4SidetectorSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "B4RunAction.hh"
G4SidetectorSD::G4SidetectorSD(
    const G4String &name,
    const G4String &hitsCollectionName,
    G4int nofCells)
    : G4VSensitiveDetector(name),
      fHitsCollection(nullptr),
      fNofCells(nofCells)
{
  collectionName.insert(hitsCollectionName);
}

G4SidetectorSD::~G4SidetectorSD() {}

void G4SidetectorSD::Initialize(G4HCofThisEvent *hce)
{

  // Create hits collection
  fHitsCollection = new G4SidetectorHitsCollection(SensitiveDetectorName, collectionName[0]);

  // Add this collection in hce
  auto hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcID, fHitsCollection);

  // Create hits
  // fNofCells for cells
  G4cout << "NofCells" << fNofCells << G4endl;
  for (G4int i = 0; i < fNofCells + 1; i++)
  {
    fHitsCollection->insert(new G4SidetectorHit());
  }
}
G4bool G4SidetectorSD::ProcessHits(G4Step *step,
                                   G4TouchableHistory *)
{
  auto edep = step->GetTotalEnergyDeposit();
  G4double steplength = 0.;
  if (step->GetTrack()->GetDefinition()->GetPDGCharge() != 0.)
  {
    steplength = step->GetStepLength();
  }
  if (edep == 0. && steplength == 0.)
    return false;

  auto touchable = (step->GetPreStepPoint()->GetTouchable());

  auto layerNumber = touchable->GetReplicaNumber(1);

  G4cout << "layerNumber: " << layerNumber << " edep: " << edep << " steplength: " << steplength << G4endl;

  auto hit = (*fHitsCollection)[layerNumber];

  hit->Add(edep, steplength);
}

void G4SidetectorSD::EndOfEvent(G4HCofThisEvent *)
{
  auto runmanager = G4RunManager::GetRunManager();
  auto runaction = const_cast<G4UserRunAction *>(runmanager->GetUserRunAction());
  runaction_u = dynamic_cast<B4RunAction *>(runaction);

  G4cout
      << G4endl
      << "-------->Hits Collection: in this event they are " << fNofCells
      << " hits in the tracker chambers: " << G4endl;
  for (std::size_t i = 0; i < fNofCells; ++i)
    (*fHitsCollection)[i]->Print();

  auto filepath_step = runaction_u->GetDirName() + "_SiDetector/" + runaction_u->GetFileName();
  std::ofstream writing_file_step;
  writing_file_step.open(filepath_step, std::ios::app);
  writing_file_step << "########## Si layer energy deposit ##########" << std::endl;
  for (std::size_t i = 0; i < fNofCells; ++i)
    writing_file_step
        << "No " << i << ": " << (*fHitsCollection)[i]->GetEdep() / MeV << " " << (*fHitsCollection)[i]->GetTrackLength() << std::endl;

  writing_file_step.close();
}