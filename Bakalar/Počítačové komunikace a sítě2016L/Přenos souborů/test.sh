#!/bin/bash 
hostname="127.0.0.1"
port=25889
file="someFiles/some_text.txt"
# echo -e "\n-----------------------------------------------------"
# echo -e "----------------------run 'make'---------------------"
# echo -e "-----------------------------------------------------"
code=0
make >/dev/null
code=$?  
if [ "$code" -ne 0 ]; then
	echo -e "-----------------------------------------------------\n------------------------stop-------------------------\n-----------------------------------------------------"
else
	# echo -e "\n-----------------------------------------------------"
	# echo -e "-------------------'make' complete-------------------"
	# echo -e "-----------------------------------------------------\n"
	
	# echo -e "\n-----------------------------------------------------"
	# echo -e "--------------------Spustam server-------------------"
	# echo -e "-----------------------------------------------------\n"
	cd server
	./server -p "$port"  & export APP_PID=$!
	sleep 2
	cd ..
	# echo -e "\n\n-----------------------------------------------------"
	# echo -e "--------------------spustam clienta-1----------------"
	# echo -e "-----------------------------------------------------\n\n"
	
	./client/client -h "$hostname" -p "$port" -u "$file" 
	# echo -e "\n\n-----------------------------------------------------"
	# echo -e "--------------------spustam clienta-2----------------"
	# echo -e "-----------------------------------------------------\n\n"
	
	file="someFiles/hledani_min.png"
	./client/client -h "$hostname" -p "$port" -u "$file" 
	# echo -e "\n\n-----------------------------------------------------"
	# echo -e "--------------------spustam clienta-3----------------"
	# echo -e "-----------------------------------------------------\n\n"
	
	file="someFiles/fitnew.png"
	./client/client -h "$hostname" -p "$port" -u "$file" 
	
	# echo -e "\n\n-----------------------------------------------------"
	# echo -e "--------------------spustam clienta-4----------------"
	# echo -e "-----------------------------------------------------\n\n"
	
	file="snorlax.jpg"
	./client/client -h "$hostname" -p "$port" -d "$file" 
	
	
	
	
	
	
	
	
	counter=1
	
	# while [ $counter -ne 0 ] ;do
		# counter=0
		# for KILLPID in `ps | grep 'client' | awk ' { print $1;}'`; do 
		  # counter=$((COUNTER+1)) 
		  # # kill -9 $KILLPID 2>/dev/null;
		# done
		# # echo "Pocet ostavajucich clientov $counter"
	# done
	# #kill processes
	# echo "vypinam server"
	
	# for KILLPID in `ps | grep 'server' | awk ' { print $1;}'`; do 
	 # echo "$KILLPID"
	  # kill -9 $KILLPID 2>/dev/null;
	# done
	kill $APP_PID
fi