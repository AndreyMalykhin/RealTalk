#Real talk? What you talking about?
Educational-purpose programming language with:
- Old school procedural paradigm
- Static typing
- C/Java-like gangsterish syntax
- Compilation into bytecode, execution by virtual machine

#Alright, what you got?

###What's up with control flow statements?
- conditional (aka if/else if/else)
- condition-controlled loop (aka while)
- early exit from loop (aka break)
- continuation with next iteration of loop (aka continue)
- functions with arguments and return value

###What's up with expressions?
- arithmetical: addition, subtration, division, multiplication, pre increment, pre decrement, negation
- logical: and, or, not
- comparisons: equal, not equal, greater, greater or equal, less, less or equal
- variable assignment
- function call
- array indexing
- array allocation

###Some other statements?
- variable definition
- import definitions from another file

###What's up with data types?
- 32-bit signed integer
- 64-bit signed integer
- double-precision floating point number
- 1-byte ASCII character
- sequence of characters
- 1-byte boolean value
- fixed size array of aforementioned data types

#Under the hood

###Components
    src
        real_talk
            lexer // source code tokenizer
            parser // source code parser; syntax tree
            semantic // semantic analyzer
            code // bytecode generator (TODO)
            vm // virtual machine (TODO)

###Dependencies
- C++11
- libc++
- Clang 6.0+
- CMake 3.0.1+
- Boost 1.56.0
- Google Mock 1.7.0
