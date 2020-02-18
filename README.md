TARGET SIMULATIONS


How to run: ./radioprotection Macro.mac outputFileName_WithoutExtensions

---------------------
GEOMETRY -> DetectorConstruction

-World

-Target of variable size given by macro parameter.
	The material can be given by parameter only if it is inside G4 NIST database 

/changeTarget/Size 100 1000 1000 um
/changeTarget/Material G4_BRAIN_ICRP

---------------------
BEAM -> GeneralParticleSource

-Given by macro parameters

/gps/particle proton
/gps/ene/mono 6 MeV
/gps/position -0.5 0 0 cm
/gps/direction 1 0 0

---------------------
SCORING -> SteppingAction

-kinetic energy of gamma produced by protonInelastic interaction
-Atomic mass of recoil atom
-Charge of recoil atom
-proton steplength before inelastic interaction
-proton energy at interaction point

Output: ROOT file with a TTree

---------------------
PHYSICS -> PhyscsList

-HADRONTHERAPY1:
Same as Hadrontherapy advanced example

-HADRONTERAPY2:
Same as HADRONTHERAPY1 but with G4HadronPhysicsQGSP_BIC_AllHP.
In the range within 0-200 MeV it uses ParticleHP models and cross sections, it takes data from TENDL database
To run simulations using HADRONTHERAPY2, download G4TENDL1.3.2 folder from http://geant4.web.cern.ch/support/download
Then, set the following envairomental variable:

export G4PARTICLEHPDATA=/PathToFolder/G4TENDL1.3.2



