# Carpet SRV

## A little minecraft server 1.4.7 written in C++

This is a little minecraft server 1.4.7 written in C++. It's mainly made for experimentation, while allowing me to train myself to C++.

### Authors 
- [tetram26](<mailto://dev@tetram26.org>)

### Special thanks
- [ht06](<https://github.com/hthug06>) for being a big help on this project ^^ 
### Features 

- Basic ping response 
- Basic login procedure
- Packet encryption (very important)
- Basic command terminal (/stop, /kickall, /list)
### Dependencies

- Cmake
- Openssl 3.0 minimum
- Zlib
- Asio

( I recommend you using vcpkg to install dependencies !)

### Installing

First clone this repository 

```
git clone https://github.com/tetram2674562/Carpet_srv.git
```
Then enter the directory

```
cd Carpet_srv
```

Create a build directory and enter it 
```
mkdir build && cd build
```
Then generate the makefile

```
cmake ..
```

End up with this command to compile
```
make -j <number of cores>
```

To start it just do the following

```
./CARPET_SRV
```

### TODO 

- Add more packet support
- Read / Write anvil format
- Adding comments to the code... Yeah this is unreadable
- Refactoring the whole project (I like to do this kind of things)

### Tests

In order to execute tests that are in the project you will need to proceed as the following

```
make run_tests
./run_tests
```


### Contributing

As of now, I just want to keep this as a little personnal project, to learn the C++.
However when the structure will be better (as of now It's absolute garbage), I will allow contributions.

This project follow a few rules : 

- No external library other than openssl (maybe libnoise ? I'll try to take a look, but I want to keep it with less dependancies as possible) 

I had for first idea to use Cpp 98 but I stepped against cross platform compilation problems.

*I hate data races and memory leak >:(*

### About AI 
I used AI in the past, but now I'm trying to get rid of it entirely so I am trying as much as possible to not use it. Only when I'm stuck on an unsolvable problem.
