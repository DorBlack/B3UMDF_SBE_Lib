//
// Created by santana on 11/27/23.
//

#ifndef UMDF_B3_SBE_B3_SBE_HPP
#define UMDF_B3_SBE_B3_SBE_HPP

#include <cinttypes>
#include <optional>

struct b3_header
{
    uint8_t channel_id;
    uint8_t reserved;
    uint16_t sequence_version;
    uint32_t sequence_number;
    uint64_t sending_time;
};

struct framing_header
{
    uint16_t message_length;
    uint16_t encoding_type;
};

struct sbe_message_header
{
    uint16_t block_length;
    uint16_t template_id;
    uint16_t schema_id;
    uint16_t schema_version;
};

struct b3_packet_body_hdr
{
    framing_header frame_hdr;
    sbe_message_header sbe_msg_hdr;
};

struct b3_packet;
static uint32_t get_sbe_body_len(const b3_packet* __packet);

struct b3_packet
{
    b3_packet(char *__data, uint32_t __len) : data(__data), len(__len) {
        header = (b3_header *)((uint8_t *)data);
        current_sbe_packet = (b3_packet_body_hdr* )((uint8_t*)data + sizeof(b3_header));
        sbe_body = __data + sizeof(b3_header) + sizeof(b3_packet_body_hdr);
        offset = sizeof(b3_header) + sizeof(b3_packet_body_hdr);
        sbe_body_size = get_sbe_body_len(this);
    }

    b3_header* header;
    b3_packet_body_hdr* current_sbe_packet;
    uint32_t offset;
    char* sbe_body;
    uint32_t sbe_body_size;
    char* data;
    uint32_t len;
};

template<typename Ty>
static Ty get_sbe_message(const b3_packet* __packet)
{
    auto msg = Ty();
    msg.wrapForDecode(__packet->sbe_body, 0,
                      __packet->current_sbe_packet->sbe_msg_hdr.block_length,
                      __packet->current_sbe_packet->sbe_msg_hdr.schema_version,
                      (__packet->len - (__packet->offset)));
    return msg;
}


static uint32_t get_sbe_body_len(const b3_packet* __packet)
{
    switch(__packet->current_sbe_packet->sbe_msg_hdr.template_id)
    {
        case SecurityDefinition_4::sbeTemplateId():
        {
            auto value = get_sbe_message<SecurityDefinition_4>(__packet);
            return value.decodeLength();
        }
        case News_5::sbeTemplateId():
        {
            auto value = get_sbe_message<News_5>(__packet);
            return value.decodeLength();
        }
        case SnapshotFullRefresh_Orders_MBO_71::sbeTemplateId():
        {
            auto value = get_sbe_message<SnapshotFullRefresh_Orders_MBO_71>(__packet);
            return value.decodeLength();
        }
        default:
            return __packet->current_sbe_packet->sbe_msg_hdr.block_length;
    }
}

static bool next_sbe_message(b3_packet* __packet)
{
    if(__packet->offset + __packet->sbe_body_size >= __packet->len)
    {
        return false;
    }
    __packet->offset += __packet->sbe_body_size;
    __packet->current_sbe_packet = (b3_packet_body_hdr *)((uint8_t *)__packet->data + __packet->offset);
    __packet->offset += sizeof(b3_packet_body_hdr);
    __packet->sbe_body = (char *)((uint8_t *)__packet->current_sbe_packet + sizeof(b3_packet_body_hdr));
    __packet->sbe_body_size =  get_sbe_body_len(__packet);
    return true;
}

#endif //UMDF_B3_SBE_B3_SBE_HPP
