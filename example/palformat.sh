#!/bin/bash
echo "unsigned char palette[256][3] = {"
echo $(cat $1 |tail -n +5 |head -n 256 | sed -e s/UNTITLED// | xargs -n3 bash -c 'echo "{$0,$1,$2},"') | sed '$ s/.$//' |tr -d '\n'
echo "};"
