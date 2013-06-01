#COMAKE2 edit-mode: -*- Makefile -*-
.PHONY:all
all:lib compack release 
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mall[0m']"
	@echo "make all done"

.PHONY:clean
clean:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mclean[0m']"
	$(MAKE) -C lib clean
	$(MAKE) -C compack clean
	rm -rf release
	rm -rf release
	rm -rf output

.PHONY:dist
dist:clean
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mdist[0m']"
	cd ..;tar czvf idlcompiler.tar.gz idlcompiler;cd idlcompiler

.PHONY:love
love:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mlove[0m']"
	@echo "make love done"

.PHONY:lib
lib:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mlib[0m']"
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Entering directory:'[1;32;40mlib[0m']"
	$(MAKE) -C lib
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Leaving directory:'[1;32;40mlib[0m']"

.PHONY:compack
compack:
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mcompack[0m']"
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Entering directory:'[1;32;40mcompack[0m']"
	$(MAKE) -C compack
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Leaving directory:'[1;32;40mcompack[0m']"

release:lib \
  compack
	@echo "[[1;32;40mCOMAKE:BUILD[0m][Target:'[1;32;40mrelease[0m']"
	touch release
	mkdir -p output/bin
	cp -r ../../com/idlcompiler/output/bin/* output/bin

