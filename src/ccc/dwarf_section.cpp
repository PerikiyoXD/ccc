// This file is part of the Chaos Compiler Collection.
// SPDX-License-Identifier: MIT

#include "dwarf_section.h"
#include "util.h"

namespace ccc::dwarf {

SectionReader::SectionReader(std::span<const u8> debug, std::span<const u8> line)
    : m_debug(debug)
    , m_line(line)
{
}

Result<void> SectionReader::parse()
{
    u32 offset = 0;

    DebuggingInformationEntry die;
    CCC_RETURN_IF_ERROR(die.parse(m_debug, offset));
    die.print(stdout, 0);

	return Result<void>();
}

std::string Attribute::to_string() const
{
	switch(this->form) {
		case FORM_ADDR: return "FORM_ADDR";
		case FORM_REF: return "FORM_REF";
		case FORM_BLOCK2: return "FORM_BLOCK2";
		case FORM_BLOCK4: return "FORM_BLOCK4";
		case FORM_DATA2: return "FORM_DATA2";
		case FORM_DATA4: return "FORM_DATA4";
		case FORM_DATA8: return "FORM_DATA8";
		case FORM_STRING: return "FORM_STRING";
		default: return "unknown form";
	}
}

Result<void> DebuggingInformationEntry::parse(std::span<const u8> data, u32& offset)
{
    // Length reads the first u32
    this->length = *reinterpret_cast<const u32*>(&data[offset]);
    offset += sizeof(u32);

    // Check if the length is 0
    CCC_CHECK(this->length != 0, "DIE length is 0.");

    // Tag reads the second u16
    this->tag = static_cast<TagType>(*reinterpret_cast<const u16*>(&data[offset]));
    offset += sizeof(u16);

    // Print it.
    this->print(stdout, 0);

    return Result<void>();
}

void DebuggingInformationEntry::print(FILE* out, u32 depth)
{
	for(u32 i = 0; i < depth; i++) {
		fprintf(out, "  ");
	}

	fprintf(out, "DIE: %s\n", tag_to_string(this->tag));
	fprintf(out, "  offset: %u\n", this->offset);
	fprintf(out, "  index: %u\n", this->index);
	fprintf(out, "  length: %u\n", this->length);

	for(const auto& [key, value] : this->attributes) {
		fprintf(out, "  attribute[%u]: %s\n", key, value.to_string().c_str());
	}
}

const char* tag_to_string(TagType tag)
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