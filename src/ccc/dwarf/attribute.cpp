#include "attribute.h"
#include <cstdio>
#include <stdexcept>

namespace ccc::Dwarf {

Result<const char*> attribute_form_to_string(AttributeForm form)
{
    switch (form) {
		case FORM_ADDR: return "ADDR";
		case FORM_REF: return "REF";
		case FORM_BLOCK2: return "BLOCK2";
		case FORM_BLOCK4: return "BLOCK4";
		case FORM_DATA2: return "DATA2";
		case FORM_DATA4: return "DATA4";
		case FORM_DATA8: return "DATA8";
		case FORM_STRING: return "STRING";
		default: return CCC_FAILURE("Unknown attribute form: 0x%X", form);
    }
}

Result<const char*> attribute_name_to_string(AttributeName name)
{
	switch (name) {
		case AT_sibling: return "sibling";
		case AT_location: return "location";
		case AT_name: return "name";
		case AT_fund_type: return "fund_type";
		case AT_mod_fund_type: return "mod_fund_type";
		case AT_user_def_type: return "user_def_type";
		case AT_mod_u_d_type: return "mod_u_d_type";
		case AT_ordering: return "ordering";
		case AT_subscr_data: return "subscr_data";
		case AT_byte_size: return "byte_size";
		case AT_bit_offset: return "bit_offset";
		case AT_bit_size: return "bit_size";
		case AT_element_list: return "element_list";
		case AT_stmt_list: return "stmt_list";
		case AT_low_pc: return "low_pc";
		case AT_high_pc: return "high_pc";
		case AT_language: return "language";
		case AT_member: return "member";
		case AT_discr: return "discr";
		case AT_discr_value: return "discr_value";
		case AT_string_length: return "string_length";
		case AT_common_reference: return "common_reference";
		case AT_comp_dir: return "comp_dir";
		case AT_const_value_string: return "const_value_string";
		case AT_const_value_data2: return "const_value_data2";
		case AT_const_value_data4: return "const_value_data4";
		case AT_const_value_data8: return "const_value_data8";
		case AT_const_value_block2: return "const_value_block2";
		case AT_const_value_block4: return "const_value_block4";
		case AT_containing_type: return "containing_type";
		case AT_default_value_addr: return "default_value_addr";
		case AT_default_value_data2: return "default_value_data2";
		case AT_default_value_data8: return "default_value_data8";
		case AT_default_value_string: return "default_value_string";
		case AT_friends: return "friends";
		case AT_inline: return "inline";
		case AT_is_optional: return "is_optional";
		case AT_lower_bound_ref: return "lower_bound_ref";
		case AT_lower_bound_data2: return "lower_bound_data2";
		case AT_lower_bound_data4: return "lower_bound_data4";
		case AT_lower_bound_data8: return "lower_bound_data8";
		case AT_program: return "program";
		case AT_private: return "private";
		case AT_producer: return "producer";
		case AT_protected: return "protected";
		case AT_prototyped: return "prototyped";
		case AT_public: return "public";
		case AT_pure_virtual: return "pure_virtual";
		case AT_return_addr: return "return_addr";
		case AT_specification: return "specification";
		case AT_start_scope: return "start_scope";
		case AT_stride_size: return "stride_size";
		case AT_upper_bound_ref: return "upper_bound_ref";
		case AT_upper_bound_data2: return "upper_bound_data2";
		case AT_upper_bound_data4: return "upper_bound_data4";
		case AT_upper_bound_data8: return "upper_bound_data8";
		case AT_virtual: return "virtual";
		case AT_lo_user: return "lo_user";
		case AT_hi_user: return "hi_user";
		default: return CCC_FAILURE("Unknown attribute name: 0x%X", name);
	}
}

Result<const char*> attribute_to_string(Attribute attribute)
{
	auto name_result = attribute_name_to_string(attribute.name);
	if(!name_result.success())
		throw std::runtime_error("Failed to convert attribute name to string");
	
	auto form_result = attribute_form_to_string(attribute.form);
	if(!form_result.success())
		throw std::runtime_error("Failed to convert attribute form to string");

	static char buffer[256];
	snprintf(buffer, sizeof(buffer), "Attribute: %s\n  offset: %u\n  index: %u\n  size: %u\n  form: %s\n", *name_result, attribute.offset, attribute.index, attribute.size, *form_result);
	return buffer;
}

}; // namespace ccc::Dwarf