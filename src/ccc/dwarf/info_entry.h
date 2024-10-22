// This file is part of the Chaos Compiler Collection.
// SPDX-License-Identifier: MIT

#include "../util.h"
#include "attribute.h"

namespace ccc::Dwarf {

enum InfoEntryTagType : u16 {
    TAG_padding = 0x0000,
    TAG_array_type = 0x0001,
    TAG_class_type = 0x0002,
    TAG_entry_point = 0x0003,
    TAG_enumeration_type = 0x0004,
    TAG_formal_parameter = 0x0005,
    TAG_global_subroutine = 0x0006,
    TAG_global_variable = 0x0007,
    TAG_label = 0x000a,
    TAG_lexical_block = 0x000b,
    TAG_local_variable = 0x000c,
    TAG_member = 0x000d,
    TAG_pointer_type = 0x000f,
    TAG_reference_type = 0x0010,
    TAG_compile_unit = 0x0011,
    TAG_source_file = 0x0011,
    TAG_string_type = 0x0012,
    TAG_structure_type = 0x0013,
    TAG_subroutine = 0x0014,
    TAG_subroutine_type = 0x0015,
    TAG_typedef = 0x0016,
    TAG_union_type = 0x0017,
    TAG_unspecified_parameters = 0x0018,
    TAG_variant = 0x0019,
    TAG_common_block = 0x001a,
    TAG_common_inclusion = 0x001b,
    TAG_inheritance = 0x001c,
    TAG_inlined_subroutine = 0x001d,
    TAG_module = 0x001e,
    TAG_ptr_to_member_type = 0x001f,
    TAG_set_type = 0x0020,
    TAG_subrange_type = 0x0021,
    TAG_with_stmt = 0x0022,
    TAG_lo_user = 0x4080,
    TAG_hi_user = 0xffff,

    TAG_format_label = 0x8000, /* for FORTRAN 77 and Fortran 90 */
    TAG_namelist = 0x8001, /* For Fortran 90 */
    TAG_function_template = 0x8002, /* for C++ */
    TAG_class_template = 0x8003 /* for C++ */
};

class InfoEntry {
    u32 offset;
    u32 index;
    u32 length;
    InfoEntryTagType tag;
    std::vector<Attribute> attributes;

public:
    Result<void> parse(std::span<const u8> data, u32& offset);
    Result<void> parse_attribute(std::span<const u8> data, u32& offset);
    u32 get_offset();
    void print(FILE* out, u32 depth);
};

} // namespace ccc::Dwarf