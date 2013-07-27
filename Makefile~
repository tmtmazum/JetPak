LIB = -lglut -lGLU

JETPAK = JetPak.cc headers/*.h src/*.cpp

TESTORTHO = testOrtho.cpp headers/*.h src/*.cpp

TESTSMALL = testSmall.cpp headers/Util.h

JetPak: $(JETPAK)
	g++ $(JETPAK) $(LIB) -o $@

testOrtho: $(TESTORTHO)
	g++ $(TESTORTHO) $(LIB) -o $@
	
testSmall: $(TESTSMALL)
	g++ $(TESTSMALL) $(LIB) -o $@