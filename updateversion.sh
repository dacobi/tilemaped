#!/bin/bash

OLDVER=$(cat version.h|head -n 1 |awk '{print $6}')
NEWVER=$(git rev-list --all --count)

VERDIFF=$(expr $NEWVER - $OLDVER)

UPDATE="0"
UPSVER="0"
RCVER="0"
CRCVER="0"
NEWB="0"
NEWG="0"
GITRUN="0"
FVER="0"
PRINTVER="0"


while getopts r:sgfv flag
do
    case "${flag}" in
	r) RCVER=${OPTARG};;
	s) UPSVER="1";;
	g) GITRUN="1";;
	f) FVER="1";;
        v) PRINTVER="1";;
    esac
done


MAINVER=$(head -n 1 version.h | awk '{print $4}' | sed s/\"//g | tr -d ";" | tr "-" " " | tr "." " " |awk '{print $1}')
SUBVER=$(head -n 1 version.h | awk '{print $4}' | sed s/\"//g | tr -d ";" | tr "-" " " | tr "." " " |awk '{print $2}')
BVER=$(head -n 1 version.h | awk '{print $4}' | sed s/\"//g | tr -d ";" | tr "-" " " | tr "." " " |awk '{print $3}')

head -n 1 version.h | grep -v RC > /dev/null
if [ $? != "0" ]; then
	CRCVER=$(head -n 1 version.h | awk '{print $4}' | sed s/\"//g | tr -d ";" | tr "-" " " | tr "." " " |awk '{print $4}' |sed s/RC//)
fi

if [ $PRINTVER != "0" ]; then

echo "Old build count: $OLDVER"
echo "Current build count: $NEWVER"
echo "Count diff: $VERDIFF"
if [ $CRCVER != "0" ]; then
	echo "Current version: $MAINVER.$SUBVER.$BVER-RC$CRCVER"
else
        echo "Current version: $MAINVER.$SUBVER.$BVER"
fi
exit
fi

#echo $RCVER
#echo $FVER
#echo $UPSVER

NEWG=$(expr $NEWVER + 1)

if [ $FVER != "0" ]; then
	UPDATE="1"
	RCVER="0"
else
	if [ $UPSVER != "0" ]; then
		SUBVER=$(expr $SUBVER + 1)
		UPDATE="1"
	else
		if [ "$VERDIFF" -gt "0" ]; then 
			NEWB=$(expr $VERDIFF + $BVER + 1)
			UPDATE="1"

		fi
	fi

	if [ "$RCVER" != "0" ]; then 
		UPDATE="1"
		NEWB="0"
	fi
fi

#echo $NEWB
#echo $NEWG
#echo $RCVER
#echo $UPSVER
#echo $UPDATE

if [ "$UPDATE" != "0" ]; then
	if [ "$RCVER" != "0" ]; then 
		#echo "std::string TilemapEd_Version = \""$MAINVER.$SUBVER.$NEWB-RC$RCVER"\"; // "$NEWG
		echo "std::string TilemapEd_Version = \""$MAINVER.$SUBVER.$NEWB-RC$RCVER"\"; // "$NEWG > newver.tmp
		cat newver.tmp
	else
		#echo "std::string TilemapEd_Version = \""$MAINVER.$SUBVER.$NEWB"\"; // "$NEWG
		echo "std::string TilemapEd_Version = \""$MAINVER.$SUBVER.$NEWB"\"; // "$NEWG > newver.tmp
		cat newver.tmp
	fi
	
	(echo "//" | tr -d "\n"  && cat version.h) > oldver.tmp
	cat oldver.tmp >> newver.tmp
	rm oldver.tmp
	mv newver.tmp version.h

	if [ "$GITRUN" != "0" ]; then 
		if [ "$RCVER" != "0" ]; then
			echo "Running: git commit -a -m \"version $MAINVER.$SUBVER.$NEWB-RC$RCVER\""
			git commit -a -m "version $MAINVER.$SUBVER.$NEWB-RC$RCVER"
		else
			echo "Running: git commit -a -m \"version $MAINVER.$SUBVER.$NEWB\""
			git commit -a -m "version $MAINVER.$SUBVER.$NEWB"
		fi
	fi
else
	echo "No Update"
fi
