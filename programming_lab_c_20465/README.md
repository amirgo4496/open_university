---

# Assembler Input and Output Files

When running the assembler, you need to pass input files via command line arguments containing the source code in the assembler language defined in this project.

The assembler operates on each source file separately and generates the following output files:

- **.am File**
  - Contains the source file post-macro expansion stage of the assembler(pre-assembler stage).

- **.object File**
  - Contains the assembled code.

- **.externals File**
  - Details all the places (addresses) in the assembled code where external symbols are referenced. This includes information encoded with the value of a symbol declared as an external (appearing as `.extern` in the source).

- **.entries File**
  - Details all symbols declared as entry points in the assembled code (appearing as `.entry` in the source).

If there are no symbols declared as externals in the source file, the assembler does not create the `.externals` output file. Similarly, if there are no symbols declared as entries (`entry` directive), the `.entries` output file is not created.

Source file names must end with the extension ".as". For example, valid filenames could be `x.as`, `y.as`, or `hello.as`. These filenames should be passed as arguments to the assembler without specifying the extension.

For example, if your assembler executable is named `assembler`, running the following command:

```
assembler hello y x
```

will execute the assembler on the files `hello.as`, `y.as`, and `x.as`.

Output filenames are based on the input filename as it appears in the command line, appended with the appropriate extension:
- `.am` for the macro expanded source file.
- `.ob` for the object file.
- `.ext` for the externals file.
- `.ent` for the entries file.

For instance, executing the assembler with:

```
assembler x
```

will produce the output files `x.ob`, `x.ext`, and `x.ent` if there are entry symbols (`entry` directive) or external symbols (`extern` directive) in the source file `x.as`. If there are no external symbols in `x.as`, the output file `x.ext` will not be created, and if there are no entry symbols, `x.ent` will not be created.

