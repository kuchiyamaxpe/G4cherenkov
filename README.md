# G4cherenkov

# How to build
mkdir path/to/builddirectory \n
cd path/to/builddirectory \n
cmake path/to/G4Cherenkov \n
make

# How to run
./exampleB4a -m path/to/G4Cherenkov/runtest.mac

# Output data structure
## Optical photon data detected by cherenkov detector (path/to/builddirectory/TestData/***.txt)


\#\#\#\#\#\#\#\#\#\# PhotonData \#\#\#\#\#\#\#\#\#\#

xposition(mm) yposition(mm) zposition(mm) wavelength(nm) \n
. \n
. \n
. \n

## Energy deposit data on Si Layers (path/to/builddirectory/TestData_StepInfo/***.txt)



\#\#\#\#\#\#\#\#\#\# Si Detector Data \#\#\#\#\#\#\#\#\#\#

ProcessName preVolumeName preVolumeptr postVolumeName ParticleName prexposition postxposition preyposition postyposition prezposition postzposition kineticEnergy(MeV) //cherenkov直前のparticle状態

\#\#\#\#\#\#\#\#\#\# Si layer energy deposit \#\#\#\#\#\#\#\#\#\#

No *: energy deposit(MeV) steplength(mm) //Noは照射方向から昇順
.
.


## Detailed track data (path/to/builddirectory/TestData_TrackInfo/***.txt)

\#\#\#\#\#\#\#\#\#\# TrackData \#\#\#\#\#\#\#\#\#\#

preVolumeName postVolumeName trackid MasterProcessName ProcessName ParticleName KineticEnergy(MeV)
.
.
.



