
// This file is part of the Chaos Compiler Collection.
// SPDX-License-Identifier: MIT

#include "dwarf.h"
#include "util.h"

namespace ccc::dwarf {

Result<const char*> attribute_form_to_string(AttributeForm form)
{
    switch (form) {
        case FORM_ADDR:    return "ADDR";
        case FORM_REF:     return "REF";
        case FORM_BLOCK2:  return "BLOCK2";
        case FORM_BLOCK4:  return "BLOCK4";
        case FORM_DATA2:   return "DATA2";
        case FORM_DATA4:   return "DATA4";
        case FORM_DATA8:   return "DATA8";
        case FORM_STRING:  return "STRING";
        default:           return CCC_FAILURE("Unknown attribute form: 0x%X", form);
    }
}

Result<const char*> info_entry_tag_to_string(InfoEntryTag tag)
{
    switch (tag) {
        case TAG_padding:               return "padding";
        case TAG_array_type:            return "array_type";
        case TAG_class_type:            return "class_type";
        case TAG_entry_point:           return "entry_point";
        case TAG_enumeration_type:      return "enumeration_type";
        case TAG_formal_parameter:      return "formal_parameter";
        case TAG_global_subroutine:     return "global_subroutine";
        case TAG_global_variable:       return "global_variable";
        case TAG_label:                 return "label";
        case TAG_lexical_block:         return "lexical_block";
        case TAG_local_variable:        return "local_variable";
        case TAG_member:                return "member";
        case TAG_pointer_type:          return "pointer_type";
        case TAG_reference_type:        return "reference_type";
        case TAG_compile_unit:          return "compile_unit";
        case TAG_string_type:           return "string_type";
        case TAG_structure_type:        return "structure_type";
        case TAG_subroutine:            return "subroutine";
        case TAG_subroutine_type:       return "subroutine_type";
        case TAG__typedef:              return "typedef";
        case TAG_union_type:            return "union_type";
        case TAG_unspecified_parameters:return "unspecified_parameters";
        case TAG_variant:               return "variant";
        case TAG_common_block:          return "common_block";
        case TAG_common_inclusion:      return "common_inclusion";
        case TAG_inheritance:           return "inheritance";
        case TAG_inlined_subroutine:    return "inlined_subroutine";
        case TAG_module:                return "module";
        case TAG_ptr_to_member_type:    return "ptr_to_member_type";
        case TAG_set_type:              return "set_type";
        case TAG_subrange_type:         return "subrange_type";
        case TAG_with_stmt:             return "with_stmt";
        case TAG_format_label:          return "format_label";
        case TAG_namelist:              return "namelist";
        case TAG_function_template:     return "function_template";
        case TAG_class_template:        return "class_template";
        case TAG_lo_user:               return "lo_user";
        case TAG_hi_user:               return "hi_user";
        default:                        return CCC_FAILURE("Unknown tag: 0x%X", tag);
    }
}

Result<const char*> attribute_name_to_string(AttributeName name)
{
    switch (name) {
        case AT_sibling:               return "sibling";
        case AT_location:              return "location";
        case AT_name:                  return "name";
        case AT_fund_type:             return "fund_type";
        case AT_mod_fund_type:         return "mod_fund_type";
        case AT_user_def_type:         return "user_def_type";
        case AT_mod_u_d_type:          return "mod_u_d_type";
        case AT_ordering:              return "ordering";
        case AT_subscr_data:           return "subscr_data";
        case AT_byte_size:             return "byte_size";
        case AT_bit_offset:            return "bit_offset";
        case AT_bit_size:              return "bit_size";
        case AT_element_list:          return "element_list";
        case AT_stmt_list:             return "stmt_list";
        case AT_low_pc:                return "low_pc";
        case AT_high_pc:               return "high_pc";
        case AT_language:              return "language";
        case AT_member:                return "member";
        case AT_discr:                 return "discr";
        case AT_discr_value:           return "discr_value";
        case AT_string_length:         return "string_length";
        case AT_common_reference:      return "common_reference";
        case AT_comp_dir:              return "comp_dir";
        case AT_const_value_string:    return "const_value_string";
        case AT_const_value_data2:     return "const_value_data2";
        case AT_const_value_data4:     return "const_value_data4";
        case AT_const_value_data8:     return "const_value_data8";
        case AT_const_value_block2:    return "const_value_block2";
        case AT_const_value_block4:    return "const_value_block4";
        case AT_containing_type:       return "containing_type";
        case AT_default_value_addr:    return "default_value_addr";
        case AT_default_value_data2:   return "default_value_data2";
        case AT_default_value_data8:   return "default_value_data8";
        case AT_default_value_string:  return "default_value_string";
        case AT_friends:               return "friends";
        case AT_inline:                return "inline";
        case AT_is_optional:           return "is_optional";
        case AT_lower_bound_ref:       return "lower_bound_ref";
        case AT_lower_bound_data2:     return "lower_bound_data2";
        case AT_lower_bound_data4:     return "lower_bound_data4";
        case AT_lower_bound_data8:     return "lower_bound_data8";
        case AT_program:               return "program";
        case AT_private:               return "private";
        case AT_producer:              return "producer";
        case AT_protected:             return "protected";
        case AT_prototyped:            return "prototyped";
        case AT_public:                return "public";
        case AT_pure_virtual:          return "pure_virtual";
        case AT_return_addr:           return "return_addr";
        case AT_specification:         return "specification";
        case AT_start_scope:           return "start_scope";
        case AT_stride_size:           return "stride_size";
        case AT_upper_bound_ref:       return "upper_bound_ref";
        case AT_upper_bound_data2:     return "upper_bound_data2";
        case AT_upper_bound_data4:     return "upper_bound_data4";
        case AT_upper_bound_data8:     return "upper_bound_data8";
        case AT_virtual:               return "virtual";
        case AT_lo_user:               return "lo_user";
        case AT_hi_user:               return "hi_user";
        default:                       return CCC_FAILURE("Unknown attribute name: 0x%X", name);
    }
}

Result<Attribute> parse_attribute(std::span<const u8> data, u32& offset)
{
    printf("Parsing attribute at offset %x\n", offset);
    Attribute attribute{};

    attribute.offset = offset;

    // Read attribute name and form
    attribute.name = static_cast<AttributeName>(*reinterpret_cast<const u16*>(&data[offset]));
    offset += sizeof(u16);

	auto attribute_name_result = attribute_name_to_string(attribute.name);
	if (!attribute_name_result.success()) {
		return Result<Attribute>::failure(attribute_name_result.error());
	}
	printf("Attribute name: %s\n", *attribute_name_result);

    attribute.form = static_cast<AttributeForm>(*reinterpret_cast<const u16*>(&data[offset]));
    offset += sizeof(u16);

	auto attribute_form_result = attribute_form_to_string(attribute.form);
	if (!attribute_form_result.success()) {
		return Result<Attribute>::failure(attribute_form_result.error());
	}
	printf("Attribute form: %s\n", *attribute_form_result);

    // Process based on form
    switch (attribute.form) {
    case FORM_ADDR:
        attribute.size = sizeof(u32);
        break;
    case FORM_REF:
        attribute.size = sizeof(u32);
        break;
    case FORM_BLOCK2:
        attribute.size = *reinterpret_cast<const u16*>(&data[offset]);
        offset += sizeof(u16);
        break;
    case FORM_BLOCK4:
        attribute.size = *reinterpret_cast<const u32*>(&data[offset]);
        offset += sizeof(u32);
        break;
    case FORM_STRING:
        attribute.size = strlen(reinterpret_cast<const char*>(&data[offset])) + 1;
        break;
    case FORM_DATA2:
        attribute.size = sizeof(u16);
        break;
    case FORM_DATA4:
        attribute.size = sizeof(u32);
        break;
    case FORM_DATA8:
        attribute.size = sizeof(u64);
        break;
    default: return CCC_FAILURE("Unknown attribute form: 0x%X", attribute.form);
    }

    attribute.value = std::span<const u8>(&data[offset], attribute.size);
    offset += attribute.size;

	auto attribute_value_result = [attribute]() -> Result<std::string> {
        switch (attribute.form) {
        case FORM_ADDR:
            return "0x" + std::to_string(*reinterpret_cast<const u32*>(attribute.value.data()));
        case FORM_REF:
            return "0x" + std::to_string(*reinterpret_cast<const u32*>(attribute.value.data()));
        case FORM_BLOCK2:
            return "0x" + std::to_string(*reinterpret_cast<const u16*>(attribute.value.data()));
        case FORM_BLOCK4:
            return "0x" + std::to_string(*reinterpret_cast<const u32*>(attribute.value.data()));
        case FORM_STRING:
            return std::string(reinterpret_cast<const char*>(attribute.value.data()));
        case FORM_DATA2:
            return "0x" + std::to_string(*reinterpret_cast<const u16*>(attribute.value.data()));
        case FORM_DATA4:
            return "0x" + std::to_string(*reinterpret_cast<const u32*>(attribute.value.data()));
        case FORM_DATA8:
            return "0x" + std::to_string(*reinterpret_cast<const u64*>(attribute.value.data()));
        default:
            return CCC_FAILURE("Unknown attribute form: 0x%X", attribute.form);
        }
    }();
	if (!attribute_value_result.success()) {
		return CCC_FAILURE("Failed to convert attribute to string");
	}
	printf("Attribute value: %s\n", attribute_value_result->c_str());

    printf("END\n");

    return attribute;
}

Result<std::string> attribute_to_string(Attribute attribute)
{
    auto name = attribute_name_to_string(attribute.name);
    CCC_RETURN_IF_ERROR(name);

    auto form = attribute_form_to_string(attribute.form);
    CCC_RETURN_IF_ERROR(form);

    std::string result;
    result += *name;
    result += " (";
    result += *form;
    result += "): ";

    return result;
}

Result<std::string> attribute_value_to_string(Attribute attribute)
{
	std::string result;

	switch (attribute.form) {
	case FORM_ADDR:
		result += "0x";
        result += std::to_string(*reinterpret_cast<const u32*>(attribute.value.data()));
		break;
	case FORM_REF:
		result += "0x";
        result += std::to_string(*reinterpret_cast<const u32*>(attribute.value.data()));
		break;
	case FORM_BLOCK2:
		result += "0x";
		result += std::to_string(*reinterpret_cast<const u16*>(attribute.value.data()));
		break;
	case FORM_BLOCK4:
		result += "0x";
		result += std::to_string(*reinterpret_cast<const u32*>(attribute.value.data()));
		break;
	case FORM_STRING:
		result += reinterpret_cast<const char*>(attribute.value.data());
		break;
	case FORM_DATA2:
		result += "0x";
		result += std::to_string(*reinterpret_cast<const u16*>(attribute.value.data()));
		break;
	case FORM_DATA4:
		result += "0x";
		result += std::to_string(*reinterpret_cast<const u32*>(attribute.value.data()));
		break;
	case FORM_DATA8:
		result += "0x";
		result += std::to_string(*reinterpret_cast<const u64*>(attribute.value.data()));
		break;
	default:
		return CCC_FAILURE("Unknown attribute form: 0x%X", attribute.form);
	}

	return result;
}

Result<InfoEntry> parse_info_entry(std::span<const u8> data, u32& offset)
{
    printf("Parsing debug information entry at offset %x\n", offset);
    InfoEntry entry{};

    entry.offset = offset;

    entry.length = *reinterpret_cast<const u32*>(&data[offset]);
    offset += sizeof(u32);

    entry.tag = static_cast<InfoEntryTag>(*reinterpret_cast<const u16*>(&data[offset]));
    offset += sizeof(u16);

	auto tag_result = info_entry_tag_to_string(entry.tag);
	if (!tag_result.success()) {
		return CCC_FAILURE("Failed to convert tag to string");
	}
	printf("Tag: %s\n", *tag_result);

    // Parse attributes within the entry
    while (offset < entry.offset + entry.length) {
        Attribute attribute;

        auto result = parse_attribute(data, offset);
        CCC_RETURN_IF_ERROR(result);
        entry.attributes.push_back(attribute);
    }

    return entry;
}

Result<std::string> info_entry_to_string(InfoEntry entry)
{
    auto tag = info_entry_tag_to_string(entry.tag);
    CCC_RETURN_IF_ERROR(tag);

    std::string result;
    result += *tag;
    result += ":\n";

    for (const auto& attribute : entry.attributes) {
        auto attribute_string = attribute_to_string(attribute);
        CCC_RETURN_IF_ERROR(attribute_string);
        result += *attribute_string;
        result += "\n";
    }

    return result;
}

Result<DebugInformation> parse_debug_information(std::span<const u8> debug_section, std::span<const u8> line_section)
{
    DebugInformation debug_information(debug_section, line_section);

    u32 offset = 0;
    while (offset < debug_section.size()) {
        auto result = parse_info_entry(debug_section, offset);

        // Retrieve the InfoEntry from the Result and store it
        if (!result.success()) {
            return Result<DebugInformation>::failure(result.error());
        }
        InfoEntry entry = *result;
        debug_information.entries[entry.index] = entry;
    }

    return debug_information;
}


} // namespace ccc::dwarf