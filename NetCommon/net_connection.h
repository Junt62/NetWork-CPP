#pragma once

#include "net_common.h"
#include "net_tsqueue.h"
#include "net_message.h"


namespace olc {

    namespace net {

        template<typename T>
        class connection : public std::enable_shared_from_this<connection<T>> {
        public:
            enum class owner {
                server,
                client
            };

            connection(owner parent, boost::asio::io_context& asioContext, boost::asio::ip::tcp::socket socket, tsqueue<owned_message<T>>& qIn)
                : m_asioContext(asioContext), m_socket(std::move(socket)), m_qMessagesIn(qIn) {
                m_nOwnerType = parent;
            }

            virtual ~connection() {
            }
        public:
            void ConnectToClient(uint32_t uid = 0) {
                if (m_nOwnerType == owner::server) {
                    if (m_socket.is_open()) {
                        id = uid;
                    }
                }
            }

            bool ConnectToServer();

            bool Disconnect();

            bool IsConnected() const {
                return m_socket.is_open();
            }


        public:
            bool Send(const message<T>& msg);

            // ASYNC - Prime context ready to read a message header
            void ReadHeader() {

            }
            // ASYNC - Prime context ready to read a message body
            void ReadBody() {
            }

            // ASYNC - Prime context to write a message header
            void WriteHeader() {
            }

            // ASYNC - Prime context to write a message body
            void WriteBody() {
            }

        protected:
            // Each connection has a unique socket to a remote
            boost::asio::ip::tcp::socket m_socket;

            // This context is shared with the whole asio instance
            boost::asio::io_context& m_asioContext;

            // This queue holds all message to be sent to the remote side
            // of this connection
            tsqueue<message<T>> m_qMessageOut;

            // This queue holds all message that have been recieved from
            // the remote side of this connection. Note it is a reference
            // as the "owner" of this connection is expected to provide a queue
            tsqueue<owned_message>& m_qMessageIn;

            // The "owner" decides how some of the connection behaves
            owner m_nOwnerType = owner::server;
            uint32_t id = 0;




        };
    }
}