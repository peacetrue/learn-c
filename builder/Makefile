library.o: team-a/src/main/cpp/library.cpp
	gcc -I team-a/src/main/public -c team-a/src/main/cpp/library.cpp
application.o: team-b/src/main/cpp/application.cpp team-a/src/main/public/library.h
	gcc -I team-a/src/main/public -c team-b/src/main/cpp/application.cpp
application: application.o library.o
	gcc -o application application.o library.o
clean:
	rm -rf *.o application
