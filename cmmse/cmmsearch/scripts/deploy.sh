#/bin/bash 

killall -9 cmmsearch
ulimit -c unlimited
rm ~/cmmsearch/bin/cmmsearch 
cp cmmsearch ~/cmmsearch/bin 
rm ~/cmmsearch/modules/*
cp extension/cmmsearch/libcmmsearch.so ~/cmmsearch/modules/
cd ~/cmmsearch
./bin/cmmsearch
cd -
