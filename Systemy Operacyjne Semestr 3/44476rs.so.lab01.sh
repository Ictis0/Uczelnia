#! /bin/bash
#SO IS1 210B LAB01 
#sebastian ratanczuk
#sebastian-ratanczuk@zut.edu.pl
if [ "$1" = -p ]
then
lista=$(ls /proc | grep -E "^[0-9]" | sort -n)
for p in $lista
do
if [ -e "/proc/$p" ]
then
nazwa=$(sed '1!d' /proc/$p/status | cut -d: -f2)
ppid=$(sed '6!d' /proc/$p/status | cut -d: -f2)
uid=$(sed '9!d' /proc/$p/status | cut -f2)
uid=$(id $uid | cut -d' ' -f1 | cut -d= -f2)
echo "$p $ppid $uid $nazwa"
fi
done

elif [ "$1" = -u ]
then
user=$(id | cut -d= -f2 | cut -d'(' -f1)
lista=$(ls /proc | grep -E "^[0-9]" | sort -n)

for p in $lista
do
if [ -e "/proc/$p" ]
then
uid=$(sed '9!d' /proc/$p/status | cut -f2)
uid=$(id $uid | cut -d= -f2 | cut -d'(' -f1)
if [ $user -eq $uid ]
then
nazwa=$(sed '1!d' /proc/$p/status | cut -d: -f2)
plik=$(readlink -e /proc/$p/cwd)
echo -e "$p $nazwa \t$plik"
fi
fi
done
else
echo "Użyj -p lub -u"
fi