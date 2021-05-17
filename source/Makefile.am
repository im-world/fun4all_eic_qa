AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = \
  -I$(includedir) \
  -I$(OFFLINE_MAIN)/include \
  -I`root-config --incdir`

lib_LTLIBRARIES = \
   libeicqa_modules.la

libeicqa_modules_la_LDFLAGS = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib

libeicqa_modules_la_LIBADD = \
   -lCLHEP \
  -lqa_modules

pkginclude_HEADERS = \
  EvalCluster.h \
  EvalHit.h \
  EvalRootTTree.h \
  EvalRootTTreeReco.h \
  EvalTower.h \
  QAExample.h \
  QAG4SimulationEicCalorimeter.h \
  QAG4SimulationEicCalorimeterSum.h \
  SamplingFractionReco.h

ROOTDICTS = \
  EvalCluster_Dict.cc \
  EvalHit_Dict.cc \
  EvalTower_Dict.cc \
  EvalRootTTree_Dict.cc

pcmdir = $(libdir)
nobase_dist_pcm_DATA = \
  EvalCluster_Dict_rdict.pcm \
  EvalHit_Dict_rdict.pcm \
  EvalTower_Dict_rdict.pcm \
  EvalRootTTree_Dict_rdict.pcm

libeicqa_modules_la_SOURCES = \
  $(ROOTDICTS) \
  EvalHit.cc \
  EvalCluster.cc \
  EvalTower.cc \
  EvalRootTTree.cc \
  EvalRootTTreeReco.cc \
  QAExample.cc \
  QAG4SimulationEicCalorimeter.cc \
  QAG4SimulationEicCalorimeterSum.cc \
  SamplingFractionReco.cc

# Rule for generating table CINT dictionaries.
%_Dict.cc: %.h %LinkDef.h
	rootcint -f $@ @CINTDEFS@ $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

#just to get the dependency
%_Dict_rdict.pcm: %_Dict.cc ;

################################################
# linking tests

noinst_PROGRAMS = testexternals

BUILT_SOURCES = \
  testexternals.cc

testexternals_SOURCES = \
  testexternals.cc

testexternals_LDADD = \
  libeicqa_modules.la

testexternals.cc:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

clean-local:
	rm -f *Dict* testexternals.cc
