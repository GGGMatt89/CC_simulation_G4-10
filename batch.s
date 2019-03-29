#version 09-2016 fontana
#!/bin/csh

# exec_path : folder of the executable file of the G4 simulation
# store_path : folder where to save the simulation output (where the ROOT file is saved)
# logdir : folder where all the infos about the job will be saved : stopped job, problems, etc.
# macro_path : folder where is set the simulation macro
# exec_name : name of the simulation executable file
# macro_name : name of the macro
# outfile_prefix : name of the output root file 
# job_prefix : prefix for the job name (not so important)


set exec_path=/afs/in2p3.fr/home/m/mfontana/Simulations/Geant4/CC_Geant4.10_simu/
set store_path=/sps/hep/hadronth/mfontana/SimulationOutput/Geant4/CC_Geant4.10_simu/simu_output/

set i=1

set logdir=/sps/hep/hadronth/mfontana/SimulationOutput/Geant4/CC_Geant4.10_simu/logdir/
set job_path=/afs/in2p3.fr/home/m/mfontana/Simulations/Geant4/CC_Geant4.10_simu/job/
set exec_name="ComptonCamera"

set today=`date +%y-%m-%d_%H-%M`	
set job_prefix="G4_CC_geom"



# Printf to check the information inserted are ok before the job starts

printf "Executable path is:\n"
printf "$exec_path\n"
printf "Storing output in:\n"
printf "$store_path\n"
printf "Executable name is $exec_name\n"   

	
# allow to use a different seed at each job
foreach j ( 245 )
   echo $i
   echo $j	
   set outfile_prefix="$today"_"10MGamma_keV$j"	
   printf "Configuring job No $i\n"
   set outfilename="$store_path"/"$outfile_prefix"_"$i"
   printf "Output file: $outfilename\n"
   set jobid="$job_prefix"_ID_"$today"   
   printf "Job ID: $jobid\n"
   set jobname="$job_path"/"$job_prefix"_"$today"

     
# Store the command line used to send a job in the variable "jobname". this variable is used as a script to send the simulation on the CC.
   printf "$exec_path"/"$exec_name -m test.mac -c standard.ini\n" > $jobname
   

#job permissions
 chmod u+x $jobname

# Command line who sends the job. ct is the time estimate for the job and vmem the job size
 qsub -l sps=1 -V -l ct=20:00:00,vmem=2048M -e $logdir -o $logdir -j y -N $jobid $jobname

# Variable i incremented

 @ i=$i + 1

end 

