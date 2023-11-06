// Copyright (c)  2023-${current_year},  Fábio da Silva Santana
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

//
// Created by scien on 30/10/2023.
//

#ifndef B3MKTDATA_SBE_MESSAGE_HPP
#define B3MKTDATA_SBE_MESSAGE_HPP
#include <memory>
#include <variant>

#include "b3_umdf_mbo_sbe/MessageHeader.h"
#include "b3_umdf_mbo_sbe/Order_MBO_50.h"
#include "b3_umdf_mbo_sbe/DeleteOrder_MBO_51.h"

using namespace b3::umdf::mbo::sbe;

namespace b3::protocol::sbe
{
    struct b3_header
    {
        b3_header(const char* data, std::size_t offset, std::size_t size) :
        _data(data), _offset(offset), _size(size)
        {}

        uint8_t get_channel_id() const
        {
            uint8_t value = 0x0;
            std::memcpy(&value, _data, sizeof(value));
            return value;
        }
        uint16_t get_sequence_version() const
        {
            uint16_t value = 0x0;
            std::memcpy(&value, _data + 2, sizeof(value));
            return value;
        }
        uint32_t get_sequence_number() const
        {
            uint32_t value = 0x0;
            std::memcpy(&value, _data + 4, sizeof(value));
            return value;
        }

        uint64_t get_sending_time() const
        {
            uint64_t value = 0x0;
            std::memcpy(&value, _data + 8, sizeof(value));
            return value;
        }

        static  std::uint32_t encoded_lenght()
        {
            return 0x10;
        }

    private:
        const char* _data;
        const std::size_t _offset;
        const std::size_t _size;
    };

    template<typename Buffer>
    struct message
    {
        std::unique_ptr<b3_header> b3header;
        std::unique_ptr<MessageHeader> header;
        std::variant<Order_MBO_50,
                            DeleteOrder_MBO_51> body;

        template<typename Ty>
        static std::shared_ptr<message> create_message(std::shared_ptr<Ty> buffer, std::size_t offset)
        {
            return std::make_shared<message>(buffer, offset);
        }

        message(std::shared_ptr<Buffer> buffer, std::size_t offset) : _buffer(buffer) {
            auto current_offset = offset;
            b3header = std::make_unique<b3_header>(buffer->data(), current_offset, buffer->size());
            current_offset += b3_header::encoded_lenght();
            header = std::make_unique<MessageHeader>();
            header->wrap(buffer->data(), current_offset, 0, buffer->size());
            current_offset += header->encodedLength();
            decoder_body(buffer, current_offset);
        }

    private:

        void decoder_body(std::shared_ptr<Buffer> buffer, std::size_t offset)
        {
            auto template_id = header->templateId();
            switch (template_id) {
            case Order_MBO_50::SBE_TEMPLATE_ID: {
                body = Order_MBO_50();
                auto ptr = std::get_if<Order_MBO_50>(&body);
                ptr->wrapForDecode(buffer->data(), offset, header->blockLength(),
                        header->actingVersion(), buffer->size());
                break;
            }
            case DeleteOrder_MBO_51::SBE_TEMPLATE_ID: {
                body = DeleteOrder_MBO_51();
                auto ptr = std::get_if<DeleteOrder_MBO_51>(&body);
                ptr->wrapForDecode(buffer->data(), offset, header->blockLength(),
                        header->actingVersion(), buffer->size());
                break;
            }
            default: break;
            }
        }
        std::shared_ptr<Buffer> _buffer;
    };
}

#endif //B3MKTDATA_SBE_MESSAGE_HPP
