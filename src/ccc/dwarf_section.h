// This file is part of the Chaos Compiler Collection.
// SPDX-License-Identifier: MIT

#pragma once

#include "util.h"

namespace ccc::dwarf {








class Dwarf {
public:
    Dwarf(std::span<const u8> debug_section, std::span<const u8> line_section);
    Result<void> parse();

protected:
    std::span<const u8> m_debug;
    std::span<const u8> m_line;
    std::map<u32, InfoEntry> m_debug_information_entries;
};

const char* tag_to_string(InfoEntryTagType tag);

}
