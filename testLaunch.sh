#!/usr/bin/env bash
rm output.txt
sleep 5000000 &

proxy_pid="$!"
echo $proxy_pid >> $PWD/output.txt

#wait ${proxy_pid}
