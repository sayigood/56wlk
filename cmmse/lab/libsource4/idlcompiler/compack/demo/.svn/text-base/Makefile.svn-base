#COMAKE2 edit-mode: -*- Makefile -*-
####################64Bit Mode####################
ifeq ($(shell uname -m),x86_64)
.PHONY:all
all:comake2_makefile_check demo 
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mall[0m']"
	@echo "make all done"

.PHONY:comake2_makefile_check
comake2_makefile_check:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mcomake2_makefile_check[0m']"
	@if [ "COMAKE" -nt "Makefile" ] ; then echo "[ERROR]PLEASE UPDATE 'Makefile' BY 'comake2'" ; exit 250 ;  fi

.PHONY:clean
clean:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mclean[0m']"
	rm -rf demo
	rm -rf ./output/bin/demo
	rm -rf demo_demo.o

.PHONY:dist
dist:clean
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mdist[0m']"
	cd ..;tar czvf demo.tar.gz demo;cd demo

.PHONY:love
love:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mlove[0m']"
	@echo "make love done"

demo:demo_demo.o
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mdemo[0m']"
	g++ demo_demo.o -Xlinker "-("  ../../../../lib2-64/bsl/lib/libbsl.a \
  ../../../../lib2-64/bsl/lib/libbsl_ResourcePool.a \
  ../../../../lib2-64/bsl/lib/libbsl_archive.a \
  ../../../../lib2-64/bsl/lib/libbsl_buffer.a \
  ../../../../lib2-64/bsl/lib/libbsl_check_cast.a \
  ../../../../lib2-64/bsl/lib/libbsl_exception.a \
  ../../../../lib2-64/bsl/lib/libbsl_pool.a \
  ../../../../lib2-64/bsl/lib/libbsl_utils.a \
  ../../../../lib2-64/bsl/lib/libbsl_var.a \
  ../../../../lib2-64/bsl/lib/libbsl_var_implement.a \
  ../../../../lib2-64/bsl/lib/libbsl_var_utils.a  -Xlinker "-)" -o demo
	mkdir -p ./output/bin
	cp -f demo ./output/bin

demo_demo.o:demo.cpp \
  demo.cpp
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mdemo_demo.o[0m']"
	g++ -c -I../../ -I../../../../lib2-64/bsl \
  -I../../../../lib2-64/bsl/include \
  -I../../../../lib2-64/bsl/output \
  -I../../../../lib2-64/bsl/output/include  -g  -o demo_demo.o demo.cpp

endif #ifeq ($(shell uname -m),x86_64)


