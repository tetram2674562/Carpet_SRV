# Carpet SRV

## A little minecraft server 1.4.7 written in C++

This is a little minecraft server 1.4.7 written in C++ 98. It's mainly made for experimentation, while allowing me to train myself to C++.

### Authors 
- [tetram26](<mailto://dev@tetram26.org>)

### Features 

- Basic ping response 
- Basic login procedure
- Packet encryption (very important)
- Basic command terminal (/stop, /kickall, /list)
### Dependencies

- Cmake
- Openssl 3.0 minimum

### Installing

First clone this repository 

```
git clone https://github.com/tetram2674562/Carpet_srv.git
```
Then enter the directory

```
cd Carpet_srv
```

Then generate the makefile

```
cmake .
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

### Contributing

As of now, I just want to keep this as a little personnal project, to learn the C++.
However when the structure will be better (as of now It's absolute garbage), I will allow contributions.

This project follow a few rules : 

- No external library other than openssl (maybe libnoise ? I'll try to take a look, but I want to keep it with less dependancies as possible) 
- C++98 is the standard. This won't change.


*I hate data races and memory leak >:(*