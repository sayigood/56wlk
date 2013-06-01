#!/bin/sh

function usage()
{
cat <<HELP_STR
用法：sh build.sh options
  help h		打印帮助
  make_all		co+clean+make_all
  for_rd_ci     rd ci之前用，用于清理环境，上传output
HELP_STR
}
case $1 in
	help|h)
		usage;
		exit 0
	;;
	make_all)
        cs co lab/libsource4/csnlp/mappb/trunk;
        cd lab/libsource4/csnlp/mappb/trunk;
		pgmake --checkout mappb.prj;
		make clean;
        make
	;;
    for_rd_ci)
        rm -rf test_type test_addr test_adcode test_extend;
        rm -rf *.o;
        rm -rf *.a Makefile *.deps;
        rm -rf output/bin/.svn output/data/.svn output/conf/.svn output/include/.svn output/lib/.svn output/modules/.svn
    ;;

esac
