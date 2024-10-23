// This file is part of the Chaos Compiler Collection.
// SPDX-License-Identifier: MIT

#include "util.h"
#include <span>
#include <string>
#include <vector>
#include <map>


namespace ccc::dwarf {

typedef unsigned int OffsetIndex;

// Enumerations --

// Dwarf Attribute Form
typedef u8 AttributeForm;
#define FORM_ADDR   0x1
#define FORM_REF    0x2
#define FORM_BLOCK2 0x3
#define FORM_BLOCK4 0x4
#define FORM_DATA2  0x5
#define FORM_DATA4  0x6
#define FORM_DATA8  0x7
#define FORM_STRING 0x8
Result<const char*> attribute_form_to_string(AttributeForm form);

// Dwarf Information Entry Tag
typedef u16 InfoEntryTag;
#define TAG_padding                 0x0000
#define TAG_array_type              0x0001
#define TAG_class_type              0x0002
#define TAG_entry_point             0x0003
#define TAG_enumeration_type        0x0004
#define TAG_formal_parameter        0x0005
#define TAG_global_subroutine       0x0006
#define TAG_global_variable         0x0007
#define TAG_label                   0x000a
#define TAG_lexical_block           0x000b
#define TAG_local_variable          0x000c
#define TAG_member                  0x000d
#define TAG_pointer_type            0x000f
#define TAG_reference_type          0x0010
#define TAG_compile_unit            0x0011
#define TAG_source_file             0x0011
#define TAG_string_type             0x0012
#define TAG_structure_type          0x0013
#define TAG_subroutine              0x0014
#define TAG_subroutine_type         0x0015
#define TAG__typedef                0x0016
#define TAG_union_type              0x0017
#define TAG_unspecified_parameters  0x0018
#define TAG_variant                 0x0019
#define TAG_common_block            0x001a
#define TAG_common_inclusion        0x001b
#define TAG_inheritance             0x001c
#define TAG_inlined_subroutine      0x001d
#define TAG_module                  0x001e
#define TAG_ptr_to_member_type      0x001f
#define TAG_set_type                0x0020
#define TAG_subrange_type           0x0021
#define TAG_with_stmt               0x0022
#define TAG_lo_user                 0x4080
#define TAG_hi_user                 0xffff
#define TAG_format_label            0x8000
#define TAG_namelist                0x8001
#define TAG_function_template       0x8002
#define TAG_class_template          0x8003
Result<const char*> info_entry_tag_to_string(InfoEntryTag tag);

// Dwarf Attribute Name Type
typedef u16 AttributeName;
#define AT_sibling              (0x0010 | FORM_REF)
#define AT_location             (0x0020 | FORM_BLOCK2)
#define AT_name                 (0x0030 | FORM_STRING)
#define AT_fund_type            (0x0050 | FORM_DATA2)
#define AT_mod_fund_type        (0x0060 | FORM_BLOCK2)
#define AT_user_def_type        (0x0070 | FORM_REF)
#define AT_mod_u_d_type         (0x0080 | FORM_BLOCK2)
#define AT_ordering             (0x0090 | FORM_DATA2)
#define AT_subscr_data          (0x00a0 | FORM_BLOCK2)
#define AT_byte_size            (0x00b0 | FORM_DATA4)
#define AT_bit_offset           (0x00c0 | FORM_DATA2)
#define AT_bit_size             (0x00d0 | FORM_DATA4)
#define AT_element_list         (0x00f0 | FORM_BLOCK4)
#define AT_stmt_list            (0x0100 | FORM_DATA4)
#define AT_low_pc               (0x0110 | FORM_ADDR)
#define AT_high_pc              (0x0120 | FORM_ADDR)
#define AT_language             (0x0130 | FORM_DATA4)
#define AT_member               (0x0140 | FORM_REF)
#define AT_discr                (0x0150 | FORM_REF)
#define AT_discr_value          (0x0160 | FORM_BLOCK2)
#define AT_string_length        (0x0190 | FORM_BLOCK2)
#define AT_common_reference     (0x01a0 | FORM_REF)
#define AT_comp_dir             (0x01b0 | FORM_STRING)
#define AT_const_value_string   (0x01c0 | FORM_STRING)
#define AT_const_value_data2    (0x01c0 | FORM_DATA2)
#define AT_const_value_data4    (0x01c0 | FORM_DATA4)
#define AT_const_value_data8    (0x01c0 | FORM_DATA8)
#define AT_const_value_block2   (0x01c0 | FORM_BLOCK2)
#define AT_const_value_block4   (0x01c0 | FORM_BLOCK4)
#define AT_containing_type      (0x01d0 | FORM_REF)
#define AT_default_value_addr   (0x01e0 | FORM_ADDR)
#define AT_default_value_data2  (0x01e0 | FORM_DATA2)
#define AT_default_value_data8  (0x01e0 | FORM_DATA8)
#define AT_default_value_string (0x01e0 | FORM_STRING)
#define AT_friends              (0x01f0 | FORM_BLOCK2)
#define AT_inline               (0x0200 | FORM_STRING)
#define AT_is_optional          (0x0210 | FORM_STRING)
#define AT_lower_bound_ref      (0x0220 | FORM_REF)
#define AT_lower_bound_data2    (0x0220 | FORM_DATA2)
#define AT_lower_bound_data4    (0x0220 | FORM_DATA4)
#define AT_lower_bound_data8    (0x0220 | FORM_DATA8)
#define AT_program              (0x0230 | FORM_STRING)
#define AT_private              (0x0240 | FORM_STRING)
#define AT_producer             (0x0250 | FORM_STRING)
#define AT_protected            (0x0260 | FORM_STRING)
#define AT_prototyped           (0x0270 | FORM_STRING)
#define AT_public               (0x0280 | FORM_STRING)
#define AT_pure_virtual         (0x0290 | FORM_STRING)
#define AT_return_addr          (0x02a0 | FORM_BLOCK2)
#define AT_specification        (0x02b0 | FORM_REF)
#define AT_start_scope          (0x02c0 | FORM_DATA4)
#define AT_stride_size          (0x02e0 | FORM_DATA4)
#define AT_upper_bound_ref      (0x02f0 | FORM_REF)
#define AT_upper_bound_data2    (0x02f0 | FORM_DATA2)
#define AT_upper_bound_data4    (0x02f0 | FORM_DATA4)
#define AT_upper_bound_data8    (0x02f0 | FORM_DATA8)
#define AT_virtual              (0x0300 | FORM_STRING)
#define AT_lo_user              (0x2000)
#define AT_hi_user              (0x3ff0)
Result<const char*> attribute_name_to_string(AttributeName name);

// Structures --

// Dwarf Attribute
typedef struct Attribute {
	u32 offset; // Internal
	u32 index; // Internal
	AttributeName name;
	u32 size;
    AttributeForm form;
    // Value
    std::span<const u8> value;
} Attribute;
Result<Attribute> parse_attribute(std::span<const u8> data, u32& offset);
Result<std::string> attribute_to_string(Attribute attribute);
Result<std::string> attribute_value_to_string(Attribute attribute);

// Dwarf Information Entry
typedef struct InfoEntry {
    u32 offset; // Internal
    u32 index; // Internal
    // Data
    u32 length;
    InfoEntryTag tag;
    std::vector<Attribute> attributes;
} InfoEntry;
Result<InfoEntry> parse_info_entry(std::span<const u8> data, u32& offset);
Result<std::string> info_entry_to_string(InfoEntry entry);

// Dwarf Debug Information
typedef struct DebugInformation {
    std::span<const u8> debug_section;
    std::span<const u8> line_section;
    std::map<OffsetIndex, InfoEntry> entries;

    DebugInformation() {}
    DebugInformation(std::span<const u8> debug_section, std::span<const u8> line_section)
        : debug_section(debug_section), line_section(line_section) {}
    
} DebugInformation;
Result<DebugInformation> parse_debug_information(std::span<const u8> debug_section, std::span<const u8> line_section);

} // namespace ccc::dwarf