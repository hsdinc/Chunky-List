all: stringtest messagepasser stringtest-ours test 

messagepasser: message-passer.o noisy-transmission.o
	clang++ -ldl -rdynamic -g -stdlib=libc++ -std=c++11 -Wall -Wextra -pedantic -o messagepasser message-passer.o noisy-transmission.o

stringtest: stringtest.o testing-logger.o
	clang++ -ldl -rdynamic -g -stdlib=libc++ -std=c++11 -Wall -Wextra -pedantic -o stringtest  stringtest.o testing-logger.o

stringtest-old: stringtest-old.o testing-logger.o
	clang++ -ldl -rdynamic -g -stdlib=libc++ -std=c++11 -Wall -Wextra -pedantic -o stringtest-old  stringtest-old.o testing-logger.o

stringtest-ours: stringtest-ours.o testing-logger.o \
    chunkylist.hpp chunkylist-private.hpp
	clang++ -ldl -rdynamic -stdlib=libc++ \
	 -g -stdlib=libc++ -std=c++11 -Wall -Wextra -pedantic -o stringtest-ours \
	 stringtest-ours.o testing-logger.o 
   
stringtest.o: stringtest.cpp chunkylist.hpp chunkystring.hpp chunkylist-private.hpp
	clang++ -c -g -stdlib=libc++ -std=c++11 -Wall -Wextra -pedantic  stringtest.cpp testing-logger.cpp 

stringtest-ours.o: stringtest-ours.cpp chunkystring.hpp
	clang++ -c -g -stdlib=libc++ -std=c++11 -Wall -Wextra -pedantic \
		stringtest-ours.cpp testing-logger.cpp

stringtest-old.o: stringtest-old.cpp chunkystring.hpp chunkystring.hpp chunkylist.hpp chunkylist-private.hpp
	clang++ -c -g -stdlib=libc++ -std=c++11 -Wall -Wextra -pedantic  stringtest-old.cpp

testing-logger.o: testing-logger.cpp testing-logger.hpp
	clang++ -c -g -stdlib=libc++ -std=c++11 -Wall -Wextra -pedantic  testing-logger.cpp

noisy-transmission.o: noisy-transmission.cpp noisy-transmission.hpp chunkystring.hpp chunkylist.hpp chunkylist-private.hpp
	clang++ -c -g -stdlib=libc++ -std=c++11 -Wall -Wextra -pedantic  noisy-transmission.cpp

message-passer.o: message-passer.cpp chunkystring.hpp chunkylist.hpp chunkylist-private.hpp noisy-transmission.cpp noisy-transmission.hpp
	clang++ -c -g -stdlib=libc++ -std=c++11 -Wall -Wextra -pedantic  message-passer.cpp  

doxygen:
	doxygen doxygen.config

test: stringtest stringtest-ours
	./stringtest; ./stringtest-ours

clean:
	rm -f messagepasser stringtest stringtest-old stringtest-ours *.o
