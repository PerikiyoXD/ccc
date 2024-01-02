// This file is part of the Chaos Compiler Collection.
// SPDX-License-Identifier: MIT

#pragma once

#include "mdebug_analysis.h"
#include "mdebug_section.h"
#include "symbol_database.h"

namespace ccc::mdebug {

// Perform all the main analysis passes on the mdebug symbol table and convert
// it to a set of C++ ASTs.
Result<void> import_symbol_table(
	SymbolDatabase& database,
	std::span<const u8> elf,
	s32 section_offset,
	SymbolSourceHandle source,
	u32 importer_flags,
	const DemanglerFunctions& demangler);
Result<void> import_files(SymbolDatabase& database, const AnalysisContext& context);
Result<void> import_file(SymbolDatabase& database, const mdebug::File& input, const AnalysisContext& context);

// Try to add pointers from member function declarations to their definitions
// using a heuristic.
void fill_in_pointers_to_member_function_definitions(SymbolDatabase& database);

}
