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
// Created by scien on 01/11/2023.
//

#include "gtest/gtest.h"
#include "asio.hpp"

#include <functional>
#include <future>
#include <thread>
#include <memory>

#include "types.h"
#include "io/udp_socket_receiver.hpp"
#include "memory/buffer.hpp"
#include "b3/protocol/sbe_message.hpp"


using namespace  b3::protocol::sbe;

struct udp_multicast_sender
{
public:
    udp_multicast_sender(
            const asio::ip::address& multicast_address,
            short port)
    {
        _endpoint = asio::ip::udp::endpoint (multicast_address, port);
        _socket = std::make_shared<asio::ip::udp::socket>(_io_context, _endpoint.protocol());
    }

    void run()
    {
        _worker = std::thread(
                [&]()
                {
                    try{
                        auto work = asio::make_work_guard(_io_context);
                        _io_context.run();
                    }
                    catch (...)
                    {
                        std::cout << "worker exception" << std::endl;
                    }

                }
                );
    }

    void stop()
    {
        _io_context.stop();
        _worker.join();
    }

    std::size_t send(char* buffer, std::size_t size)
    {
       std::future<std::size_t> task =  _socket->async_send_to(asio::buffer(buffer, size), _endpoint, asio::use_future);
       auto  ret = task.get();
       return ret;
    }
private:
    asio::ip::udp::endpoint _endpoint;
    std::shared_ptr<asio::ip::udp::socket> _socket;
    asio::io_context _io_context;
    std::thread _worker;
};
using namespace  io::socket;
class umdp_socket_receiver_test : public ::testing::Test {

protected:

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

    void create_msg()
    {
        _buffer = std::make_shared<memory::buffer>();

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
    }
    void SetUp() override {
        create_msg();
        _udp_sender = std::make_shared<udp_multicast_sender>( asio::ip::make_address(_address), port);
        _udp_sender->run();
    }

    void TearDown() override {
        _udp_sender->stop();
        _udp_sender.reset();
    }

    std::string _interface = "0.0.0.0";
    std::string _address = "233.252.8.6";
    short port = 30002;
    DefaultValues default_values;
    uint64_t timer;
    std::shared_ptr<udp_multicast_sender> _udp_sender;
    std::shared_ptr<memory::buffer> _buffer;
};

TEST_F(umdp_socket_receiver_test, SendBuffer)
{
    bool is_called = false;
    auto notify = std::make_shared<socket_notification<memory::buffer>>();
    notify->on_msg_received = [&](std::shared_ptr<memory::buffer> bf) {
        EXPECT_EQ(bf->size(), _buffer->size());
        is_called = true;
    };

    notify->on_error = [&](std::error_code error) {

    };

    auto socket = udp_multicast<socket_notification, memory::buffer>::create_socket(_interface,
                                                            _address,
                                                            port,
                                                            notify);
    auto receiver_th = std::thread([&]()
    {
        socket->start();
        int i = 0;
    });

    auto ret = _udp_sender->send(_buffer->data(), _buffer->size());

    EXPECT_EQ(ret, _buffer->size());

    if(!is_called)
        std::this_thread::sleep_for(std::chrono::seconds(1));
    EXPECT_TRUE(is_called);
    socket->stop();
    receiver_th.join();
}