#include "mdebug.h"

namespace ccc::mdebug {

packed_struct(SymbolicHeader,
	/* 0x00 */ s16 magic;
	/* 0x02 */ s16 version_stamp;
	/* 0x04 */ s32 line_number_count;
	/* 0x08 */ s32 line_numbers_size_bytes;
	/* 0x0c */ s32 line_numbers_offset;
	/* 0x10 */ s32 dense_numbers_max_index;
	/* 0x14 */ s32 dense_numbers_offset;
	/* 0x18 */ s32 procedure_descriptor_count;
	/* 0x1c */ s32 procedure_descriptors_offset;
	/* 0x20 */ s32 local_symbol_count;
	/* 0x24 */ s32 local_symbols_offset;
	/* 0x28 */ s32 optimization_symbols_max_index;
	/* 0x2c */ s32 optimization_symbols_offset;
	/* 0x30 */ s32 auxiliary_symbol_count;
	/* 0x34 */ s32 auxiliary_symbols_offset;
	/* 0x38 */ s32 local_strings_max_index;
	/* 0x3c */ s32 local_strings_offset;
	/* 0x40 */ s32 external_strings_max_index;
	/* 0x44 */ s32 external_strings_offset;
	/* 0x48 */ s32 file_descriptor_count;
	/* 0x4c */ s32 file_descriptors_offset;
	/* 0x50 */ s32 relative_file_descriptor_count;
	/* 0x54 */ s32 relative_file_descriptors_offset;
	/* 0x58 */ s32 external_symbols_max_index;
	/* 0x5c */ s32 external_symbols_offset;
)

packed_struct(FileDescriptor,
	/* 0x00 */ u32 address;
	/* 0x04 */ s32 file_path_string_offset;
	/* 0x08 */ s32 strings_offset;
	/* 0x0c */ s32 cb_ss;
	/* 0x10 */ s32 isym_base;
	/* 0x14 */ s32 symbol_count;
	/* 0x18 */ s32 iline_base;
	/* 0x1c */ s32 cline;
	/* 0x20 */ s32 iopt_base;
	/* 0x24 */ s32 copt;
	/* 0x28 */ s16 ipd_first;
	/* 0x2a */ s16 cpd;
	/* 0x2c */ s32 iaux_base;
	/* 0x30 */ s32 caux;
	/* 0x34 */ s32 rfd_base;
	/* 0x38 */ s32 crfd;
	/* 0x3c */ u32 lang : 5;
	/* 0x3c */ u32 f_merge : 1;
	/* 0x3c */ u32 f_readin : 1;
	/* 0x3c */ u32 f_big_endian : 1;
	/* 0x4c */ u32 reserved_1 : 22;
	/* 0x40 */ s32 cb_line_offset;
	/* 0x44 */ s32 cb_line;
)
static_assert(sizeof(FileDescriptor) == 0x48);

packed_struct(ProcedureDescriptor,
	/* 0x00 */ u32 address;
	/* 0x04 */ s32 isym;
	/* 0x08 */ s32 iline;
	/* 0x0c */ s32 regmask;
	/* 0x10 */ s32 regoffset;
	/* 0x14 */ s32 iopt;
	/* 0x18 */ s32 fregmask;
	/* 0x1c */ s32 fregoffset;
	/* 0x20 */ s32 frameoffset;
	/* 0x24 */ s16 framereg;
	/* 0x26 */ s16 pcreg;
	/* 0x28 */ s32 ln_low;
	/* 0x2c */ s32 ln_high;
	/* 0x30 */ s32 cb_line_offset;
)

packed_struct(SymbolEntry,
	u32 iss;
	s32 value;
	u32 st : 6;
	u32 sc : 5;
	u32 reserved : 1;
	u32 index : 20;
)
static_assert(sizeof(SymbolEntry) == 0xc);

SymbolTable parse_symbol_table(const Module& module, const ModuleSection& section) {
	SymbolTable symbol_table;
	
	const auto& hdrr = get_packed<SymbolicHeader>(module.image, section.file_offset, "MIPS debug section");
	verify(hdrr.magic == 0x7009, "Invalid symbolic header.");
	
	symbol_table.header = &hdrr;
	symbol_table.procedure_descriptor_table_offset = hdrr.procedure_descriptors_offset;
	symbol_table.local_symbol_table_offset = hdrr.local_symbols_offset;
	symbol_table.file_descriptor_table_offset = hdrr.file_descriptors_offset;
	
	// Iterate over file descriptors.
	for(s64 i = 0; i < hdrr.file_descriptor_count; i++) {
		u64 fd_offset = hdrr.file_descriptors_offset + i * sizeof(FileDescriptor);
		const auto& fd_header = get_packed<FileDescriptor>(module.image, fd_offset, "file descriptor");
		verify(fd_header.f_big_endian == 0, "Not little endian or bad file descriptor table.");
		
		SymFileDescriptor fd;
		fd.header = &fd_header;
		fd.raw_path = get_string(module.image, hdrr.local_strings_offset + fd_header.strings_offset + fd_header.file_path_string_offset);
		
		// Try to detect the source language.
		std::string lower_name = fd.raw_path;
		for(char& c : lower_name) c = tolower(c);
		if(lower_name.ends_with(".c")) {
			fd.detected_language = SourceLanguage::C;
		} else if(lower_name.ends_with(".cpp") || lower_name.ends_with(".cc") || lower_name.ends_with(".cxx")) {
			fd.detected_language = SourceLanguage::CPP;
		} else if(lower_name.ends_with(".s") || lower_name.ends_with(".asm")) {
			fd.detected_language = SourceLanguage::ASSEMBLY;
		}
		
		// Read symbols.
		for(s64 j = 0; j < fd_header.symbol_count; j++) {
			u64 sym_offset = hdrr.local_symbols_offset + (fd_header.isym_base + j) * sizeof(SymbolEntry);
			const auto& sym_entry = get_packed<SymbolEntry>(module.image, sym_offset, "local symbol");
			Symbol& sym = fd.symbols.emplace_back();
			sym.string = get_string(module.image, hdrr.local_strings_offset + fd_header.strings_offset + sym_entry.iss);
			sym.value = sym_entry.value;
			sym.storage_type = (SymbolType) sym_entry.st;
			sym.storage_class = (SymbolClass) sym_entry.sc;
			sym.index = sym_entry.index;
			
			if(fd.base_path.empty() && sym_entry.iss == fd_header.file_path_string_offset && sym.storage_type == SymbolType::LABEL && fd.symbols.size() > 2) {
				const Symbol& base_path = fd.symbols[fd.symbols.size() - 2];
				if(base_path.storage_type == SymbolType::LABEL) {
					fd.base_path = base_path.string;
				}
			}
		}
		
		// Determine the full path.
		std::string base_path = fd.base_path;
		std::string raw_path = fd.raw_path;
		for(char& c : base_path) if(c == '\\') c = '/';
		for(char& c : raw_path) if(c == '\\') c = '/';
		if(base_path.empty() || raw_path[0] == '/' || (raw_path[1] == ':' && raw_path[2] == '/')) {
			fd.full_path = raw_path;
		} else {
			fd.full_path = fs::weakly_canonical(fs::path(base_path)/fs::path(raw_path));
		}
		
		// Read procedure descriptors.
		// This is buggy.
		//for(s64 j = 0; j < fd_header.cpd; j++) {
		//	u64 pd_offset = hdrr.cb_pd_offset + (fd_header.ipd_first + j) * sizeof(ProcedureDescriptor);
		//	auto pd_entry = get_packed<ProcedureDescriptor>(module.image, pd_offset, "procedure descriptor");
		//	
		//	u64 sym_offset = hdrr.cb_sym_offset + (fd_header.isym_base + pd_entry.isym) * sizeof(SymbolEntry);
		//	const auto& sym_entry = get_packed<SymbolEntry>(module.image, sym_offset, "local symbol");
		//	
		//	SymProcedureDescriptor& pd = fd.procedures.emplace_back();
		//	pd.name = get_string(module.image, hdrr.strings_base_offset + fd_header.strings_offset + sym_entry.iss);
		//	pd.address = pd_entry.address;
		//}
		
		symbol_table.files.emplace_back(fd);
	}
	
	return symbol_table;
}

void print_headers(FILE* dest, const SymbolTable& symbol_table) {
	const SymbolicHeader& hdrr = *symbol_table.header;
	fprintf(dest, "SYMBOLIC HEADER: magic=%hx vstamp=%hx\n", hdrr.magic, hdrr.version_stamp);
	fprintf(dest, "\n");
	fprintf(dest, "                              Count     Offset    Size      Max Index\n");
	fprintf(dest, "                              -----     ------    ----      ---------\n");
	fprintf(dest, "  Line Numbers                %-8x  %-8x  %-8x  -\n",
		hdrr.line_number_count, hdrr.line_numbers_offset, hdrr.line_numbers_size_bytes);
	fprintf(dest, "  Dense Numbers               -         %-8x  -         %-8x\n",
		hdrr.dense_numbers_offset, hdrr.dense_numbers_max_index);
	fprintf(dest, "  Procedure Descriptors       %-8x  %-8x  -         -\n",
		hdrr.procedure_descriptor_count, hdrr.procedure_descriptors_offset);
	fprintf(dest, "  Local Symbols               %-8x  %-8x  -         -\n",
		hdrr.local_symbol_count, hdrr.local_symbols_offset);
	fprintf(dest, "  Optimization Symbols        -         %-8x  -         %-8x\n",
		hdrr.optimization_symbols_offset, hdrr.optimization_symbols_max_index);
	fprintf(dest, "  Auxiliary Symbols           %-8x  %-8x  -         -\n",
		hdrr.auxiliary_symbol_count, hdrr.auxiliary_symbols_offset);
	fprintf(dest, "  Local Strings               -         %-8x  -         %-8x\n",
		hdrr.local_strings_offset, hdrr.local_strings_max_index);
	fprintf(dest, "  External Strings            -         %-8x  -         %-8x\n",
		hdrr.external_strings_offset, hdrr.external_strings_max_index);
	fprintf(dest, "  File Descriptors            %-8x  %-8x  -         -\n",
		hdrr.file_descriptor_count, hdrr.file_descriptors_offset);
	fprintf(dest, "  Relative Files Descriptors  %-8x  %-8x  -         -\n",
		hdrr.relative_file_descriptor_count, hdrr.relative_file_descriptors_offset);
	fprintf(dest, "  External Symbols            -         %-8x  -         %-8x\n",
		hdrr.external_symbols_offset, hdrr.external_symbols_max_index);
}

const char* symbol_type(SymbolType type) {
	switch(type) {
		case SymbolType::NIL: return "NIL";
		case SymbolType::GLOBAL: return "GLOBAL";
		case SymbolType::STATIC: return "STATIC";
		case SymbolType::PARAM: return "PARAM";
		case SymbolType::LOCAL: return "LOCAL";
		case SymbolType::LABEL: return "LABEL";
		case SymbolType::PROC: return "PROC";
		case SymbolType::BLOCK: return "BLOCK";
		case SymbolType::END: return "END";
		case SymbolType::MEMBER: return "MEMBER";
		case SymbolType::TYPEDEF: return "TYPEDEF";
		case SymbolType::FILE_SYMBOL: return "FILE_SYMBOL";
		case SymbolType::STATICPROC: return "STATICPROC";
		case SymbolType::CONSTANT: return "CONSTANT";
	}
	return nullptr;
}

const char* symbol_class(SymbolClass symbol_class) {
	switch(symbol_class) {
		case SymbolClass::NIL: return "NIL";
		case SymbolClass::TEXT: return "TEXT";
		case SymbolClass::DATA: return "DATA";
		case SymbolClass::BSS: return "BSS";
		case SymbolClass::REGISTER: return "REGISTER";
		case SymbolClass::ABS: return "ABS";
		case SymbolClass::UNDEFINED: return "UNDEFINED";
		case SymbolClass::LOCAL: return "LOCAL";
		case SymbolClass::BITS: return "BITS";
		case SymbolClass::DBX: return "DBX";
		case SymbolClass::REG_IMAGE: return "REG_IMAGE";
		case SymbolClass::INFO: return "INFO";
		case SymbolClass::USER_STRUCT: return "USER_STRUCT";
		case SymbolClass::SDATA: return "SDATA";
		case SymbolClass::SBSS: return "SBSS";
		case SymbolClass::RDATA: return "RDATA";
		case SymbolClass::VAR: return "VAR";
		case SymbolClass::COMMON: return "COMMON";
		case SymbolClass::SCOMMON: return "SCOMMON";
		case SymbolClass::VAR_REGISTER: return "VAR_REGISTER";
		case SymbolClass::VARIANT: return "VARIANT";
		case SymbolClass::SUNDEFINED: return "SUNDEFINED";
		case SymbolClass::INIT: return "INIT";
		case SymbolClass::BASED_VAR: return "BASED_VAR";
		case SymbolClass::XDATA: return "XDATA";
		case SymbolClass::PDATA: return "PDATA";
		case SymbolClass::FINI: return "FINI";
		case SymbolClass::NONGP: return "NONGP";
	}
	return nullptr;
}

}
