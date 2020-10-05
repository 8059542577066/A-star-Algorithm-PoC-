@echo off
rem gcc 9.2.0 (tdm64) win10
g++ graph.cpp -O3 -std=c++98 -Wall -c
pause
g++ file.cpp -O3 -std=c++98 -Wall -c
pause
g++ graph.o file.o main.cpp -O3 -std=c++98 -Wall -o a-star.exe
pause