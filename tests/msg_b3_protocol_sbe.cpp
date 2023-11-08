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
// Created by scien on 31/10/2023.
//
#include "gtest/gtest.h"
#include "b3/protocol/sbe_message.hpp"
#include "memory/buffer.hpp"


//SBE MESSAGES

using namespace  b3::protocol::sbe;

class B3_Protocol_SBE_MSG_Test : public ::testing::Test {

public:

    struct DefaultValues
    {
        std::uint16_t partNumber = 10;
        std::uint16_t partCount = 10;
        std::uint16_t totNumStats = 5;

        std::uint32_t rptSeq = 2;
        std::uint32_t totNumBids = 10;
        std::uint32_t totNumOffers = 2;
        std::uint32_t lastRptSeq = 200;
        std::uint32_t mDEntrySize = 10;
        std::uint32_t enteringFirm = 5;
        std::uint32_t totalTextLength = 5;
        std::uint32_t mDEntryPositionNo = 1;
        std::uint32_t lastMsgSeqNumProcessed = 400;

        std::uint64_t newsID = 10;
        std::uint64_t securityId = 10;
        std::uint64_t mantissa = 102000;
        std::uint64_t secondaryOrderID = 200;

        std::string securityIDSource = "8";
        std::string securityExchange = "BVMF";
        std::string expected_string_value = "Teste";
        std::string expected_string_length_2 = "Te";
    };

    std::shared_ptr<memory::buffer> __buffer;
    DefaultValues default_values;
    uint64_t timer;
};

TEST_F(B3_Protocol_SBE_MSG_Test, OrderMBO50)
{
    auto _buffer = std::make_shared<memory::buffer>();

    Order_MBO_50 encode;
    MessageHeader header;

    // Criando o encoder da HEADER
    header.wrap(_buffer->data(), b3_header::encoded_lenght() + framing_header::encoded_length(), 0, _buffer->capacity())
            .blockLength(Order_MBO_50::sbeBlockLength())
            .templateId(Order_MBO_50::sbeTemplateId())
            .schemaId(Order_MBO_50::sbeSchemaId())
            .version(Order_MBO_50::sbeSchemaVersion());

    // Fazendo o encoder do BODY
    encode.wrapForEncode(_buffer->data(), header.encodedLength() + b3_header::encoded_lenght() + framing_header::encoded_length(), _buffer->capacity());
    encode.securityID(default_values.securityId);
    encode.matchEventIndicator().clear();
    encode.mDUpdateAction(MDUpdateAction::NEW);
    encode.mDEntryType(MDEntryType::BID);
    encode.rptSeq(default_values.rptSeq);
    encode.mDEntryTimestamp().time(timer);
    encode.mDEntryPx().mantissa(default_values.mantissa); // price
    encode.mDEntrySize(default_values.mDEntrySize);
    encode.enteringFirm(default_values.enteringFirm);
    encode.mDInsertTimestamp().time(timer);
    encode.mDEntryPositionNo(default_values.mDEntryPositionNo);
    encode.secondaryOrderID(default_values.secondaryOrderID);
    _buffer->set_size(encode.encodedLength() + b3_header::encoded_lenght() + header.encodedLength() + framing_header::encoded_length());

    auto msg = message<memory::buffer>(_buffer);

    auto mbo_order_50 = std::get_if<Order_MBO_50>(&(msg.body[0]->body));

    ASSERT_TRUE(mbo_order_50 != nullptr);
    EXPECT_EQ(mbo_order_50->securityID(), default_values.securityId) << "MBO_Order_50 SecurityID Error";
}
