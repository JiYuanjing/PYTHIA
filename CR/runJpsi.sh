#!/bin/bash
date
ipro=0
cd cards
let "rdsd = $(date +%s)%100000"
echo $rdsd
sh copyCardsNpro.sh Charm $1 $2 ${rdsd}
cd ..

cp run.job run_Jpsi.job
ijob=$2
((Njobs=$1+$2))
while [ $ijob -lt $Njobs ]; do
# echo $ijob
chmod a+x run.sh
 echo "Executable       = run.sh">>run_Jpsi.job
#  echo "Arguments        = cards/Charm_${ijob}.cmnd outccbar/Charm_${ijob}.root">>run_Jpsi.job
 echo "Arguments        = cards/Charm_${ijob}.cmnd out/Charm_${ijob}.root">>run_Jpsi.job
#  echo "Arguments        = cards/Charm_${ijob}.cmnd outtest/Charm_${ijob}.root">>run_Jpsi.job
 echo "Output           = log/Jpsi_${ijob}.out">>run_Jpsi.job
 echo "Error            = log/Jpsi_${ijob}.err">>run_Jpsi.job
 echo "Log              = log/Jpsi_${ijob}">>run_Jpsi.job
 echo  "Queue " >>run_Jpsi.job
 echo  "     " >>run_Jpsi.job
 let "ijob+=1";
done

condor_submit run_Jpsi.job

