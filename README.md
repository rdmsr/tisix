# Tisix

**Tisix** is an operating system written in C++17 (I try not to overuse it though).

## Features
- Tiny microkernel (~2k LoC).
- Rich and modern C++ freestanding library.
- Portable codebase (In the architecture way).
- Permissive license.

## Why C++?
C was becoming limiting in terms of metaprogramming and namespaces. In this repo, I try not to overuse C++ features such as templates and stuff that make code totally unreadable

## MiCrOkErNelS aRe SlOw
1. "Slow" is relative.
2. A hobby operating system doesn't really need to be the fastest OS in the world.
3. Microkernels > *, btw

## Current issues with the codebase
- No SMP
- The scheduling algorithm could be greatly improved, its time_slice thing seems broken.
- The `servers`'s build system could also be improved.

## Architectures
I plan on porting tisix to multiple architectures for the fun of it but, right now, I am focusing on x86_64 as I know it better so it's easier to debug and faster to develop.

## Current goals 
- Better syscall API.
- Lots of syscalls.
- Fix current issues with the codebase (scroll up).
- Simple framebuffer server.

## Documentation
Soon I guess

