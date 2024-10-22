#include "info_entry.h"

namespace ccc::Dwarf {


u32 InfoEntry::get_offset()
{
	return this->offset;
}

Result<void> InfoEntry::parse(std::span<const u8> data, u32& offset)
{
	// Print debug information
	printf("Parsing DIE at offset %x\n", offset);

	// Create a new entry
	InfoEntry entry;

	// Store the offset
	this->offset = offset;

	// Read the entry length
	this->length = *reinterpret_cast<const u32*>(&data[offset]);
	offset += sizeof(u32);

	// Check if the length is 0 (null entry check)
	if (this->length == 0) {
		offset += sizeof(u32);
		return Result<void>();
	}

	// Store the entry in the map by its offset
	entry.index = entries.size(); // Assuming 'entries' is a vector of DIEs
	m_entryIndexRefMap[this->offset] = entry.index;

	// Read the tag
	this->tag = static_cast<InfoEntryTagType>(*reinterpret_cast<const u16*>(&data[offset]));
	offset += sizeof(u16);

	// Parse attributes within the entry
	while(offset < this->offset + this->length) {
		auto result = this->parse_attribute(data, offset);
		CCC_RETURN_IF_ERROR(result);
	}

	// Add entry to the vector of entries
	entries.push_back(entry);

	// If outIndex is provided, return the entry index
	if (outIndex) {
		*outIndex = entry.index;
	}

	// Print the DIE for debugging purposes
	this->print(stdout, 0);

	return Result<void>();
}


Result<void> InfoEntry::parse_attribute(std::span<const u8> data, u32& offset)
{
    Attribute attribute;
	
	this->offset = offset;

    // Read attribute name and form
    attribute.name = *reinterpret_cast<const u16*>(&data[offset]);
    offset += sizeof(u16);

	attribute.form = static_cast<AttributeForm>(*reinterpret_cast<const u16*>(&data[offset]));
    offset += sizeof(u16);

    // Process based on form
    switch (attribute.form) {
    case FORM_ADDR:
        attribute.size = sizeof(u32);  // Assuming address size
        break;
    case FORM_REF:
        attribute.size = sizeof(u32);  // Assuming reference size
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
    default:
		throw std::runtime_error("Unknown attribute form for attribute " + std::to_string(attribute.name));
        return CCC_FAILURE("Unknown attribute form: %u", attribute.form);
    }

	attribute.value = std::span<const u8>(&data[offset], attribute.size);

    // Store the attribute in the entry
    this->attributes.push_back(attribute);

    offset += attribute.size;

	attribute.print(stdout, 0);

    return Result<void>();
}

void InfoEntry::print(FILE* out, u32 depth)
{
	for(u32 i = 0; i < depth; i++) {
		fprintf(out, "  ");
	}

	fprintf(out, "DIE: %s\n", tag_to_string(this->tag));
	fprintf(out, "  offset: %u\n", this->offset);
	fprintf(out, "  index: %u\n", this->index);
	fprintf(out, "  length: %u\n", this->length);

	for(const auto& attribute : this->attributes) {
		attribute.print(out, depth + 1);
	}
}

}; // namespace ccc::Dwarf