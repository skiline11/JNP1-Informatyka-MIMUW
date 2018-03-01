if [ "$1" != "test" ]
then
    if [ "$1" == "debug" ]
    then g++ -c -Wall -Wextra -O2 -DNDEBUG -std=c++17 dict.cc -o dict.o
    else g++ -c -Wall -Wextra -O2 -std=c++17 dict.cc -o dict.o
    fi
    g++ -c -Wall -Wextra -O2 -std=c++17 dictglobal.cc -o dictglobal.o
fi
gcc -c -Wall -Wextra -O2 dict_test1.c -o dict_test1.o
g++ dict.o dictglobal.o dict_test1.o -o program
