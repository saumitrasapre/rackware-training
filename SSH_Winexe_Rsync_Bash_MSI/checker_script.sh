# #!/bin/bash

separator="================================="
echo "Checking ssh reachable machines"

echo "Checking root@172.29.40.113"
ssh_status1=`sshpass -p rackware123 ssh -o StrictHostKeyChecking=no -o BatchMode=yes -o ConnectTimeout=5 root@172.29.40.113 echo PASS 2>&1`
echo "Checking root@172.29.42.169"
ssh_status2=`sshpass -p rackware ssh -o StrictHostKeyChecking=no -o BatchMode=yes -o ConnectTimeout=5 root@172.29.42.169 echo PASS 2>&1`
echo "Checking Administrator@172.16.40.225"
ssh_status3=`sshpass -p Rackware123 ssh -o StrictHostKeyChecking=no -o BatchMode=yes -o ConnectTimeout=5 Administrator@172.16.40.225 echo PASS 2>&1`

echo $separator

if [ "$ssh_status1" != "PASS" ] 
then
    ssh_status1="FAIL"
    echo "Machine 1: $ssh_status1"
else
    echo "Machine 1: $ssh_status1"
    echo "Checking OS version on root@172.29.40.113"
    ssh_ver1=`sshpass -p rackware123 ssh -o StrictHostKeyChecking=no -o BatchMode=yes -o ConnectTimeout=5 -o RemoteCommand="cat /etc/system-release && exit;bash" root@172.29.40.113`
    echo "Machine 1: $ssh_ver1"
fi

if [ "$ssh_status2" != "PASS" ]
then
    ssh_status2="FAIL"
    echo "Machine 2: $ssh_status2"
else
    echo "Machine 2: $ssh_status2"
    echo "Checking OS version on root@172.29.42.169"
    ssh_ver2=`sshpass -p rackware123 ssh -o StrictHostKeyChecking=no -o BatchMode=yes -o ConnectTimeout=5 -o RemoteCommand="cat /etc/system-release && exit;bash" root@172.29.42.169`
    echo "Machine 2: $ssh_ver2"

fi

if [ "$ssh_status3" != "PASS" ]
then
    ssh_status3="FAIL"
    echo "Machine 3: $ssh_status3"
else
    echo "Machine 3: $ssh_status3"
    echo "Checking OS version on Administrator@172.16.40.225"
    ssh_ver3=`sshpass -p rackware123 ssh -o StrictHostKeyChecking=no -o BatchMode=yes -o ConnectTimeout=5 -o RemoteCommand="cat /etc/system-release && exit;bash" root@172.16.40.225`
    echo "Machine 3: $ssh_ver3"
fi

echo $separator


echo "Checking winexe reachable machines"

echo "Checking root@172.29.40.113"
winexe_status1=`winexe -U root%rackware123 //172.29.40.113 "cmd /c ver"`
echo "Checking root@172.29.42.169"
winexe_status2=`winexe -U root%rackware //172.16.40.225 "cmd /c ver"`
echo "Checking Administrator@172.16.40.225"
winexe_status3=`winexe -U Administrator%Rackware123 //172.16.40.225 "cmd /c ver" `

echo $separator

if [ "$winexe_status1" = "" ] 
then
    winexe_status1="FAIL"
    echo "Machine 1: $winexe_status1"
else
    echo "Machine1: PASS"
    echo "Machine1 OS: $winexe_status1"
fi

if [ "$winexe_status2" = "" ]
then
    winexe_status2="FAIL"
    echo "Machine 2: $winexe_status2"
else
    echo "Machine2: PASS"
    echo "Machine2 OS: $winexe_status2"
fi

if [ "$winexe_status3" = "" ]
then
    winexe_status3="FAIL"
    echo "Machine 3: $winexe_status3"
else
    echo "Machine3: PASS"
    echo "Machine3 OS: $winexe_status3"
fi







