#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <utility>
#include <sstream>
#include <vector>

using namespace boost::asio;
using namespace boost::asio::ip;
using boost::asio::ip::tcp;
using namespace std;

struct Request {
    const string env_Variables[9] = {
        "REQUEST_METHOD",  "REQUEST_URI", "QUERY_STRING",
        "SERVER_PROTOCOL", "HTTP_HOST",   "SERVER_ADDR",
        "SERVER_PORT",     "REMOTE_ADDR", "REMOTE_PORT"};
    string value[9];
};

const string env_Variables[9] = {
    "REQUEST_METHOD",  "REQUEST_URI", "QUERY_STRING",
    "SERVER_PROTOCOL", "HTTP_HOST",   "SERVER_ADDR",
    "SERVER_PORT",     "REMOTE_ADDR", "REMOTE_PORT"};


class session : public std::enable_shared_from_this<session> {
  public:
    session(tcp::socket socket) : socket_(std::move(socket)) {}

    void start() { do_read(); }

  private:


    void do_read() {
        auto self(shared_from_this());

        socket_.async_read_some(
            boost::asio::buffer(data_, max_length),
            [this, self](boost::system::error_code ec, std::size_t length) {
                if (!ec) {

                    data_[length] = '\0';
                    
                    int child_pid;
                    child_pid = fork();
                    switch (child_pid)
                    {
                    case 0:
                        http_request_parser();
                        //cout << data_ << endl;
                        memset(data_, '\0', length);
                        dup_to_child();
                        cout << "HTTP/1.1 200 OK\r\n" << flush;
                        
                        break;
                    
                    default:
                        while(waitpid(-1, NULL, WNOHANG) > 0);
                        break;
                    }
                }
            });
    }

    void dup_to_child(){
        dup2(socket_.native_handle(), STDIN_FILENO);
        dup2(socket_.native_handle(), STDOUT_FILENO);
        dup2(socket_.native_handle(), STDERR_FILENO);
    }

    void http_request_parser() {
        stringstream ss;
        ss << string(data_);
        string token;
        vector<string> tmp;
        while(ss >> token) tmp.push_back(token);
        for(int i = 0;i<tmp.size();++i) cout << tmp[i] << "\n";
        cout << "=================\n";
    }

    void set_env(){

    }

    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
    pair<string, string> env[9];
};

class server {
  public:
    server(boost::asio::io_context &io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        do_accept();
    }

  private:
    void do_accept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::make_shared<session>(std::move(socket))->start();
                }

                do_accept();
            });
    }

    tcp::acceptor acceptor_;
};

int main(int argc, char *argv[]) {

    try {
        if (argc != 2) {
            std::cerr << "Usage: async_tcp_echo_server <port>\n";
            return 1;
        }

        boost::asio::io_context io_context;

        server s(io_context, std::atoi(argv[1]));

        io_context.run();
    } catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}