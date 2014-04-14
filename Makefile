all: rubik
OBJS = sbmptexture.o sbmpfont.o main.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -O3 -c $(DEBUG) -std=c++11 -pthread -D_GLIBCXX_USE_NANOSLEEP -D_GLIBCXX_USE_SCHED_YIELD
LDFLAGS = -Wall $(DEBUG) -lpthread -framework OpenGL -framework GLUT -framework SDL2
INCLUDES = -I/Library/Frameworks/SDL2.framework/Headers/ 
INCLUDES += -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk/System/Library/Frameworks/GLUT.framework/Headers/ 
INCLUDES += -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk/System/Library/Frameworks/OpenGL.framework/Headers/ 

rubik : $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o rubik

sbmptexture.o : sbmptexture.h
	$(CC) $(CFLAGS) sbmptexture.cpp $(INCLUDES)
	
sbmpfont.o : sbmpfont.h
	$(CC) $(CFLAGS) sbmpfont.cpp $(INCLUDES)

main.o : sbmpfont.h sbmptexture.h
	$(CC) $(CFLAGS) main.cpp $(INCLUDES)
	    
clean:
	rm -f *.o rubik
