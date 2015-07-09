#/bin/bash 

mod="$1"
if [ "X$mod" = "X" ]
then
	mod="cmmsearch"
fi
echo $mod
killall -9 cmmsearch
rm data/$mod/$mod.*.bitset
rm data/$mod/$mod.cm_i2o
rm data/$mod/$mod.cm_o2i
rm data/$mod/$mod.day_*.*
rm data/$mod/$mod.mon_*.*
rm data/$mod/$mod.di_*.*
rm data/$mod/$mod.bk/*.*
rm data/$mod/$mod.mg
mkdir -p data/$mod/$mod.bk
mkdir -p limits/
mkdir -p status/$mod
mkdir -p log/
mkdir -p conf/
echo "1 : 1" >data/$mod/$mod.cm_flag
echo "0" > data/$mod/$mod.day_curdir
echo "0" > data/$mod/$mod.mon_curdir
rm -f data/$mod/$mod.need_load_day
nohup bin/cmmsearch -f $mod.conf &
