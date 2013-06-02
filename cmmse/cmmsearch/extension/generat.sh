#!/bin/bash
#===============================================================================
#
#          FILE:  generat.sh
# 
#         USAGE:  ./generat.sh
# 
#   DESCRIPTION:  获取模块名和路径，并对echo目录下的文件和目录作相应的更改
# 
#       OPTIONS:  ---
#  REQUIREMENTS:  ---
#          BUGS:  ---
#         NOTES:  ---
#        AUTHOR:  yanghy, yanghuaiyuan@baidu.com 
#       COMPANY:  Baidu, Inc.
#       VERSION:  1.0
#       CREATED:  2008年08月20日 
#      REVISION:  ---
#===============================================================================
if [ -d demo.bak ] 
then
	rm -r demo.bak
fi

cp -r demo demo.bak
cd demo.bak
rm -rf CVS
rm -rf conf/CVS

echo -e "Please enter the module name(ex: dbshell):"
read MODULE_NAME
MODULE_NAME_UPPER=`echo $MODULE_NAME | sed 'y/abcdefghijklmnopqrstuvwxyz/ABCDEFGHIJKLMNOPQRSTUVWXYZ/'`

for i in `find . -type f`
do
	sed -e "s/demo/$MODULE_NAME/g" -e "s/ECHO/$MODULE_NAME_UPPER/g" $i >  $i.new
	mv $i.new $i
	rename demo $MODULE_NAME $i
done

echo -e "Please enter the program running PATH(ex: app/search/passport):"
read MODULE_PATH
WORKROOT=`echo $MODULE_PATH | sed 's/[[:alnum:]]/\.\.\./g'`
for i in `seq 1 128`
do
	WORKROOT=`echo $WORKROOT | sed 's/\.\.\./\.\./g'`
done

WORKROOT="$WORKROOT/../"

WORKROOT=`echo $WORKROOT | sed -e 's/\./\\\./g' -e 's/\//\\\\\//g'`

echo $WORKROOT
#sed --posix -i "s/WORKROOT=..\/..\/..\/../WORKROOT=$WORKROOT\//g" Makefile
#sed "s/WORKROOT=\.\.\/\.\.\/\.\.\/\.\./WORKROOT=$WORKROOT\//g" Makefile > Makefile.new
sed "s/WORKROOT=.*/WORKROOT=$WORKROOT\//g" Makefile > Makefile.new
mv Makefile.new Makefile

cd ..
mkdir -p $HOME/$MODULE_PATH
cp -r demo.bak $HOME/$MODULE_PATH/$MODULE_NAME 

rm -rf demo.bak

echo "DONE!"
echo "Run \"cd $HOME/$MODULE_PATH/$MODULE_NAME; make\" to get binary file"
