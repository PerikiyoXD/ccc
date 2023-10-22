# Chaos Compiler Collection

A set of tools for reverse engineering PS2 games with .mdebug symbols.

## Tools

### demangle

Demangler for the old GNU ABI.

### objdump

Half-working EE core MIPS disassembler. Probably not too interesting.

### stdump

Mdebug/STABS symbol table parser and dumper. It can extract the following information:

- Data types (structs, unions, enums, etc)
- Functions (name, return type, parameters and local variables)
- Global variables

The following output formats are supported:

- C++
- JSON

This is intended to be used with [ghidra-emotionengine-reloaded](https://github.com/chaoticgd/ghidra-emotionengine-reloaded) (>= 2.1.0 or one of the unstable builds) to import all of this information into [Ghidra](https://ghidra-sre.org/). Note that despite the name the STABS analyzer should work for the R3000 (IOP) and possibly other MIPS processors as well.

### uncc

This is similar to stdump except it organizes its output into separate source files, and has a number of extra features designed to try and make said output closer to valid source code. A `SOURCES.txt` file must be provided in the output directory, which can be generated using the `stdump files` command (you should fixup the paths manually so that they're relative to the output directory). Additionally, non-empty files that do not start with `// STATUS: NOT STARTED` will not be overwritten.

If a `FUNCTIONS.txt` file is provided in the output directory, as can be generated using the included `CCCDecompileAllFunctions.java` script for Ghidra, the code from that file will be used to populate the function bodies in the output. In this case, the first group of local variable declarations emitted will be those recovered from the symbols, and the second group will be from the code provided in the functions file. Function names are demangled.

Global variable data will be printed in a structured way based on its data type.

Data types will be sorted into their corresponding files. Since this information is not stored in the symbol table, uncc uses heuristics to map types to files. Types will be put in `.c` or `.cpp` files when there is only a single translation unit the type appears in, and `.h` files when there are multiple (and hence when heuristics must be used to determine where to put them).

Use of a code formatter such as `clang-format` or `astyle` on the output is recommended.

## Building

	cmake -B bin/
	cmake --build bin/

## Project Structure

	demangle.cpp: See above.
	objdump.cpp: See above.
	stdump.cpp: See above.
	uncc.cpp: See above.
	ccc/analysis.cpp: Runs all the different analysis passes.
	ccc/ast.cpp: Converts parsed STABS types to a C++ AST structure.
	ccc/data_refinement.cpp: Convert global variable data into a structured AST.
	ccc/dependency.cpp: Try to infer information about which types belong to which files.
	ccc/elf.cpp: Parses ELF files.
	ccc/insn.cpp: Parses EE core MIPS instructions.
	ccc/mdebug.cpp: Read the .mdebug symbol table section.
	ccc/module.cpp: Provides data structures for representing programs.
	ccc/opcodes.h: Enums for different types of EE core MIPS opcodes.
	ccc/print_cpp.cpp: Prints out AST nodes as C++ code.
	ccc/print_json.cpp: Prints out AST nodes as JSON.
	ccc/registers.cpp: Enums for EE core MIPS registers.
	ccc/stabs.cpp: Parses STABS types.
	ccc/symbols.cpp: Parses the STABS and non-STABS symbols.
	ccc/tables.cpp: Table of EE core MIPS instructions.
	ccc/util.cpp: Miscellaneous utilities.

## Resources

- [Third Eye Software and the MIPS symbol table (Peter Rowell)](http://datahedron.com/mips.html) / [in-repo mirror](docs/ThirdEyeSoftwareAndTheMIPSSymbolTable.html) / [archive.org mirror](https://web.archive.org/web/20230605005654/http://datahedron.com/mips.html)
- [MIPS Mdebug Debugging Information (David Anderson, 1996)](https://www.prevanders.net/Mdebug.ps) / [in-repo mirror](docs/Mdebug.ps) / [archive.org mirror](https://web.archive.org/web/20170305060746/https://www.prevanders.net/Mdebug.ps)
- MIPS Assembly Language Programmer's Guide, Symbol Table Chapter (Silicon Graphics, 1992) / [in-repo mirror](docs/MIPSProgrammingGuide.pdf)
- [The "stabs" representation of debugging information (Julia Menapace, Jim Kingdon, and David MacKenzie, 1992-???)](https://sourceware.org/gdb/onlinedocs/stabs.html) / [in-repo mirror](docs/STABS.html) / [archive.org mirror](https://web.archive.org/web/20230328114854/https://sourceware.org/gdb/onlinedocs/stabs.html/)
- `dbxread.c` from gdb (reading)
- `stabs.c` from binutils (reading)
- `dbxout.c` from gcc (writing)
- `stab.def` from gcc (symbol codes)

## JSON Format

### Version History

| Format Version | Release | Changes |
| - | - | - |
| 7 | v1.{0,1,2} | Base classes are now no longer doubly nested inside two JSON objects. Added acccess_specifier property. |
| 6 | | Removed order property. |
| 5 | | Added pointer_to_data_member node type. Added optional is_volatile property to all nodes. Added is_by_reference property to variable storage objects. |
| 4 | | Added optional is_const property to all nodes. Added anonymous_reference type names, where the type name is not valid but the type number is. |
| 3 | | Added optional relative_path property to function definition nodes. |
| 2 | | Added vtable_index property to function type nodes. |
| 1 | | First version. |

## License

All the code is MIT licensed, with the exception of the code in the `demanglegnu` directory, which is taken from the GNU libiberty library and is licensed under the LGPL. This license is included in the form of the `DemanglerLicense.txt` file. A copy of the GNU GPL is also included in the form of the `DemanglerLicenseSupplementGPL.txt` file.
