# Lang

x86_64 single platform compiler for linux. The programming language is based on croatian language.

## Prerequisites

Requirements for the software and other tools to build, test and push 
- [NASM assembler](https://www.nasm.us/)
- [ld linker](https://linux.die.net/man/1/ld)

## Quick start

Compile sample code code.cro provided in the project

```properties
foo@bar:~$ ./lang ./code.cro
```

Run the compiled binary 

```properties
foo@bar:~$ ./build/out
```

The console application will exit with the return code 0 , exit statement is izlaz(0)

## Running the tests

Run without arguements to run testing 

```properties
foo@bar:~$ ./lang
```

## Authors

  - **skeibol** - *Author and creator* -
    [skeibol](https://github.com/skeibol)
  - **urqon** - *Contributing assembly code* -
    [urqonurqon](https://github.com/urqonurqon)

See also the list of
[contributors](https://github.com/skeibol/lang/contributors)
who participated in this project.

## License

This project is licensed under the [MIT](LICENSE.md)
Creative Commons License - see the [LICENSE.md](LICENSE.md) file for
details

## Acknowledgments

  - [Pixeled](https://www.youtube.com/@pixeled-yt)
  - [tsoding](https://www.youtube.com/@TsodingDaily)
