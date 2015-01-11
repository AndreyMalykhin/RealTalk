#What u talkin bout ?
- Procedural paradigm
- Static typing
- C/Java-like gangsterish syntax
- Compilation into bytecode, execution by virtual machine

#Components
    src
        real_talk
            lexer // source code tokenizer
            parser // source code parser; syntax tree
            semantic // semantic analyzer
            code // bytecode generator (TODO)
            vm // virtual machine (TODO)

#Dependencies
- C++11
- libc++
- Clang 6.0+
- Boost 1.56.0
- Google Mock 1.7.0
- CMake 3.0.1+
- OSX 10.9+
