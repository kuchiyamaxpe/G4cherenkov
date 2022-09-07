#include "G4SidetectorHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

G4ThreadLocal G4Allocator<G4SidetectorHit> *G4SidetectorHitAllocator = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4SidetectorHit::G4SidetectorHit()
    : G4VHit(),
      fEdep(0.),
      fTrackLength(0.)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4SidetectorHit::~G4SidetectorHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4SidetectorHit::G4SidetectorHit(const G4SidetectorHit &right)
    : G4VHit()
{
  fEdep = right.fEdep;
  fTrackLength = right.fTrackLength;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const G4SidetectorHit &G4SidetectorHit::operator=(const G4SidetectorHit &right)
{
  fEdep = right.fEdep;
  fTrackLength = right.fTrackLength;

  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool G4SidetectorHit::operator==(const G4SidetectorHit &right) const
{
  return (this == &right) ? true : false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4SidetectorHit::Print()
{
  G4cout
      << "Edep: "
      << std::setw(7) << G4BestUnit(fEdep, "Energy")
      << " track length: "
      << std::setw(7) << G4BestUnit(fTrackLength, "Length")
      << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......