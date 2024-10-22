// This file is part of the Chaos Compiler Collection.
// SPDX-License-Identifier: MIT

#include "../util.h"

namespace ccc::Dwarf {

enum AttributeForm : u8 {
	FORM_ADDR = 0x1,
	FORM_REF = 0x2,
	FORM_BLOCK2 = 0x3,
	FORM_BLOCK4 = 0x4,
	FORM_DATA2 = 0x5,
	FORM_DATA4 = 0x6,
	FORM_DATA8 = 0x7,
	FORM_STRING = 0x8
};
Result<const char*> attribute_form_to_string(AttributeForm form);

enum AttributeName : u16 {
    AT_sibling              = (0x0010|FORM_REF),
    AT_location             = (0x0020|FORM_BLOCK2),
    AT_name                 = (0x0030|FORM_STRING),
    AT_fund_type            = (0x0050|FORM_DATA2),
    AT_mod_fund_type        = (0x0060|FORM_BLOCK2),
    AT_user_def_type        = (0x0070|FORM_REF),
    AT_mod_u_d_type         = (0x0080|FORM_BLOCK2),
    AT_ordering             = (0x0090|FORM_DATA2),
    AT_subscr_data          = (0x00a0|FORM_BLOCK2),
    AT_byte_size            = (0x00b0|FORM_DATA4),
    AT_bit_offset           = (0x00c0|FORM_DATA2),
    AT_bit_size             = (0x00d0|FORM_DATA4),
    AT_element_list         = (0x00f0|FORM_BLOCK4),
    AT_stmt_list            = (0x0100|FORM_DATA4),
    AT_low_pc               = (0x0110|FORM_ADDR),
    AT_high_pc              = (0x0120|FORM_ADDR),
    AT_language             = (0x0130|FORM_DATA4),
    AT_member               = (0x0140|FORM_REF),
    AT_discr                = (0x0150|FORM_REF),
    AT_discr_value          = (0x0160|FORM_BLOCK2),
    AT_string_length        = (0x0190|FORM_BLOCK2),
    AT_common_reference     = (0x01a0|FORM_REF),
    AT_comp_dir             = (0x01b0|FORM_STRING),
    AT_const_value_string   = (0x01c0|FORM_STRING),
    AT_const_value_data2    = (0x01c0|FORM_DATA2),
    AT_const_value_data4    = (0x01c0|FORM_DATA4),
    AT_const_value_data8    = (0x01c0|FORM_DATA8),
    AT_const_value_block2   = (0x01c0|FORM_BLOCK2),
    AT_const_value_block4   = (0x01c0|FORM_BLOCK4),
    AT_containing_type      = (0x01d0|FORM_REF),
    AT_default_value_addr   = (0x01e0|FORM_ADDR),
    AT_default_value_data2  = (0x01e0|FORM_DATA2),
    AT_default_value_data8  = (0x01e0|FORM_DATA8),
    AT_default_value_string = (0x01e0|FORM_STRING),
    AT_friends              = (0x01f0|FORM_BLOCK2),
    AT_inline               = (0x0200|FORM_STRING),
    AT_is_optional          = (0x0210|FORM_STRING),
    AT_lower_bound_ref      = (0x0220|FORM_REF),
    AT_lower_bound_data2    = (0x0220|FORM_DATA2),
    AT_lower_bound_data4    = (0x0220|FORM_DATA4),
    AT_lower_bound_data8    = (0x0220|FORM_DATA8),
    AT_program              = (0x0230|FORM_STRING),
    AT_private              = (0x0240|FORM_STRING),
    AT_producer             = (0x0250|FORM_STRING),
    AT_protected            = (0x0260|FORM_STRING),
    AT_prototyped           = (0x0270|FORM_STRING),
    AT_public               = (0x0280|FORM_STRING),
    AT_pure_virtual         = (0x0290|FORM_STRING),
    AT_return_addr          = (0x02a0|FORM_BLOCK2),
    AT_specification        = (0x02b0|FORM_REF),
    AT_start_scope          = (0x02c0|FORM_DATA4),
    AT_stride_size          = (0x02e0|FORM_DATA4),
    AT_upper_bound_ref      = (0x02f0|FORM_REF),
    AT_upper_bound_data2    = (0x02f0|FORM_DATA2),
    AT_upper_bound_data4    = (0x02f0|FORM_DATA4),
    AT_upper_bound_data8    = (0x02f0|FORM_DATA8),
    AT_virtual              = (0x0300|FORM_STRING),
    AT_lo_user              = (0x2000),
    AT_hi_user              = (0x3ff0),
};
Result<const char*> attribute_name_to_string(AttributeName name);


class Attribute {
public:
	u32 offset;
	u32 index;
	AttributeName name;
	u32 size;
	void* data;
    AttributeForm form;
    // Value
    std::span<const u8> value;

public:
	Result<void> parse(std::span<const u8> data, u32& offset);
	void print(FILE* out, u32 depth) const;
	Result<AttributeForm> get_form();
};
Result<const char*> attribute_to_string(Attribute attribute);


} // namespace ccc::Dwarf