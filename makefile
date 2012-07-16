CC=g++
SOURCE_FILES=main.cpp \
	     world.cpp
TEAM_CODE=96495776

.PHONY: submit

submit: 
	tar -czf icfp-$(TEAM_CODE).tgz install lifter README PACKAGES-TESTING src maps
