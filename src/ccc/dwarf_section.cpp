// This file is part of the Chaos Compiler Collection.
// SPDX-License-Identifier: MIT

#include "dwarf_section.h"
#include "dwarf/info_entry.h"
#include "util.h"
#include <stdexcept>

namespace ccc::dwarf {

Dwarf::Dwarf(std::span<const u8> debug, std::span<const u8> line)
    : m_debug(debug)
    , m_line(line)
{
}

Result<void> Dwarf::parse()
{
    u32 offset = 0;
	std::vector<InfoEntry> dies;

	for(offset = 0; offset < m_debug.size(); ) {	
		InfoEntry die;
		CCC_RETURN_IF_ERROR(die.parse(m_debug, offset));
		die.print(stdout, 0);

		// It should've advanced the offset. Calculate if it's wrong
		if(offset == die.get_offset()) {
			return CCC_FAILURE("Offset did not advance.");
		}
	}

	return Result<void>();
}



void Attribute::print(FILE* out, u32 depth) const
{
	printf("Printing attribute\n");

	for(u32 i = 0; i < depth; i++) {
		fprintf(out, "  ");
	}

	auto attribute_name_result = attribute_name_to_string(static_cast<AttributeName>(this->name));
	fprintf(out, "Attribute: %s\n", attribute_name_result.success() ? *attribute_name_result : "unknown");
	fprintf(out, "  offset: %u\n", this->offset);
	fprintf(out, "  index: %u\n", this->index);
	fprintf(out, "  size: %u\n", this->size);
	auto attribute_form_result = attribute_form_to_string(this->form);
	fprintf(out, "  form: %s\n", attribute_form_result.success() ? *attribute_form_result : "unknown");
}





const char* tag_to_string(InfoEntryTagType tag)
{
	switch(tag) {
		case TAG_padding: return "padding";
		case TAG_array_type: return "array_type";
		case TAG_class_type: return "class_type";
		case TAG_entry_point: return "entry_point";
		case TAG_enumeration_type: return "enumeration_type";
		case TAG_formal_parameter: return "formal_parameter";
		case TAG_global_subroutine: return "global_subroutine";
		case TAG_global_variable: return "global_variable";
		case TAG_label: return "label";
		case TAG_lexical_block: return "lexical_block";
		case TAG_local_variable: return "local_variable";
		case TAG_member: return "member";
		case TAG_pointer_type: return "pointer_type";
		case TAG_reference_type: return "reference_type";
		case TAG_compile_unit: return "compile_unit";
		case TAG_string_type: return "string_type";
		case TAG_structure_type: return "structure_type";
		case TAG_subroutine: return "subroutine";
		case TAG_subroutine_type: return "subroutine_type";
		case TAG_typedef: return "typedef";
		case TAG_union_type: return "union_type";
		case TAG_unspecified_parameters: return "unspecified_parameters";
		case TAG_variant: return "variant";
		case TAG_common_block: return "common_block";
		case TAG_common_inclusion: return "common_inclusion";
		case TAG_inheritance: return "inheritance";
		case TAG_inlined_subroutine: return "inlined_subroutine";
		case TAG_module: return "module";
		case TAG_ptr_to_member_type: return "ptr_to_member_type";
		case TAG_set_type: return "set_type";
		case TAG_subrange_type: return "subrange_type";
		case TAG_with_stmt: return "with_stmt";
		case TAG_format_label: return "format_label";
		case TAG_namelist: return "namelist";
		case TAG_function_template: return "function_template";
		case TAG_class_template: return "class_template";
		case TAG_lo_user: return "lo_user";
		case TAG_hi_user: return "hi_user";
		default: return "unknown tag";
	}
}

}; // namespace ccc::dwarf