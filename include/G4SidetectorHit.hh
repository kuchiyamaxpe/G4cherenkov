#ifndef G4SidetectorHit_h
#define G4SidetectorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"

class G4SidetectorHit : public G4VHit

{
public:
  G4SidetectorHit();
  G4SidetectorHit(const G4SidetectorHit &);
  virtual ~G4SidetectorHit();

  // operators
  const G4SidetectorHit &operator=(const G4SidetectorHit &);
  G4bool operator==(const G4SidetectorHit &) const;

  inline void *operator new(size_t);
  inline void operator delete(void *);

  // methods from base class
  virtual void Draw() {}
  virtual void Print();

  // methods to handle data
  void Add(G4double de, G4double dl);

  // get methods
  G4double GetEdep() const;
  G4double GetTrackLength() const;

private:
  G4double fEdep;        ///< Energy deposit in the sensitive volume
  G4double fTrackLength; ///< Track length in the  sensitive volume
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using G4SidetectorHitsCollection = G4THitsCollection<G4SidetectorHit>;

extern G4ThreadLocal G4Allocator<G4SidetectorHit> *G4SidetectorHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void *G4SidetectorHit::operator new(size_t)
{
  if (!G4SidetectorHitAllocator)
  {
    G4SidetectorHitAllocator = new G4Allocator<G4SidetectorHit>;
  }
  void *hit;
  hit = (void *)G4SidetectorHitAllocator->MallocSingle();
  return hit;
}

inline void G4SidetectorHit::operator delete(void *hit)
{
  if (!G4SidetectorHitAllocator)
  {
    G4SidetectorHitAllocator = new G4Allocator<G4SidetectorHit>;
  }
  G4SidetectorHitAllocator->FreeSingle((G4SidetectorHit *)hit);
}

inline void G4SidetectorHit::Add(G4double de, G4double dl)
{
  fEdep += de;
  fTrackLength += dl;
}

inline G4double G4SidetectorHit::GetEdep() const
{
  return fEdep;
}

inline G4double G4SidetectorHit::GetTrackLength() const
{
  return fTrackLength;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
