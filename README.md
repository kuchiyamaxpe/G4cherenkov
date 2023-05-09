# G4cherenkov
# 注意事項
- PMT入射までをシミュレート(PMT効率はシミュレートされていません)
- 放射体屈折率がハードコードされているため、どの放射体がシミュレートされているか注意

# 動作確認済の環境
geant4 11.0.4

# How to build
mkdir path/to/builddirectory 

cd path/to/builddirectory 

cmake path/to/G4Cherenkov 

make

# How to run
./exampleB4a -m path/to/G4Cherenkov/runtest.mac

# Output data structure
## Optical photon data detected by cherenkov detector (path/to/builddirectory/TestData/***.txt)


\#\#\#\#\#\#\#\#\#\# PhotonData \#\#\#\#\#\#\#\#\#\#

xposition(mm) yposition(mm) zposition(mm) wavelength(nm) 

. 

. 

. 

## Energy deposit data on Si Layers (path/to/builddirectory/TestData_StepInfo/***.txt)



\#\#\#\#\#\#\#\#\#\# Si Detector Data \#\#\#\#\#\#\#\#\#\#

ProcessName preVolumeName preVolumeptr postVolumeName ParticleName prexposition postxposition preyposition postyposition prezposition postzposition kineticEnergy(MeV) //cherenkov直前のparticle状態

\#\#\#\#\#\#\#\#\#\# Si layer energy deposit \#\#\#\#\#\#\#\#\#\#

No *: energy deposit(MeV) steplength(mm) //Noは照射方向から昇順

.

.

.


## Detailed track data (path/to/builddirectory/TestData_TrackInfo/***.txt)

\#\#\#\#\#\#\#\#\#\# TrackData \#\#\#\#\#\#\#\#\#\#

preVolumeName postVolumeName trackid MasterProcessName ProcessName ParticleName KineticEnergy(MeV)

.

.

.



