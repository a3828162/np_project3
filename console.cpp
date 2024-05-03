#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <utility>
#include <vector>

using namespace boost::asio;
using namespace boost::asio::ip;
using boost::asio::ip::tcp;
using namespace std;

const string env_Variables[9] = {
    "REQUEST_METHOD",  "REQUEST_URI", "QUERY_STRING",
    "SERVER_PROTOCOL", "HTTP_HOST",   "SERVER_ADDR",
    "SERVER_PORT",     "REMOTE_ADDR", "REMOTE_PORT"};

struct clientInfo {
    string host;
    string port;
    string testFile;
};

map<string, string> env;
vector<clientInfo> clients;

class client : public std::enable_shared_from_this<client> {
  public:
    client(boost::asio::io_context &io_context, unsigned long int index)
        : resolver(io_context), socket_(io_context), index(index) {}

  private:
    void do_resolve() {
        auto self(shared_from_this());
        resolver.async_resolve(
            clients[index].host, clients[index].port,
            [this, self](boost::system::error_code ec,
                         tcp::resolver::results_type result) {
                if (!ec) {
                    memset(data_, '\0', sizeof(data_));
                    endpoint_ = result;
                    do_connect();
                } else {
                    cerr << "resolv" << '\n';
                    socket_.close();
                }
            });
    }

    void do_connect() {
        auto self(shared_from_this());
        boost::asio::async_connect(
            socket_, endpoint_,
            [this, self](boost::system::error_code ec, tcp::endpoint ed) {
                if (!ec) {
                    memset(data_, '\0', sizeof(data_));
                    in.open("./test_case/" + clients[index].testFile);
                    if (!in.is_open()) {
                        cout << clients[index].testFile << " open fail\n";
                        socket_.close();
                    }
                    do_read();
                } else {
                    cerr << "connect" << '\n';
                    socket_.close();
                }
            });
    }

    void do_read() {
        auto self(shared_from_this());
        socket_.async_read_some(
            boost::asio::buffer(data_, max_length),
            [this, self](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    if (length == 0)
                        return;
                    data_[length] = '\0';
                    string msg = string(data_);
                    memset(data_, '\0', sizeof(data_));
                    output_message(msg);
                    if (msg.find("% ") != string::npos) {
                        do_write();
                    } else {
                        do_read();
                    }
                } else {
                    cerr << "read" << '\n';
                    socket_.close();
                }
            });
    }

    void do_write() {
        auto self(shared_from_this());
        string cmd;
        getline(in, cmd);
        cmd.push_back('\n');
        output_command(cmd);
        boost::asio::async_write(
            socket_, boost::asio::buffer(cmd, cmd.size()),
            [this, self](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    do_read();
                }
            });
    }

    void output_message(string content) {
        escape(content);
        cout << "<script>document.getElementById('s" << index
             << "').innerHTML += '" << content << "';</script>\n"
             << flush;
    }

    void output_command(string content) {
        escape(content);
        cout << "<script>document.getElementById('s" << index
             << "').innerHTML += '<b>" << content << "</b>';</script>\n"
             << flush;
    }

    void escape(string &src) {
        boost::replace_all(src, "&", "&amp;");
        boost::replace_all(src, "\r", "");
        boost::replace_all(src, "\n", "&NewLine;");
        boost::replace_all(src, "\'", "&apos;");
        boost::replace_all(src, "\"", "&quot;");
        boost::replace_all(src, "<", "&lt;");
        boost::replace_all(src, ">", "&gt;");
    }
    tcp::resolver resolver;
    tcp::socket socket_;
    unsigned long int index;
    tcp::resolver::results_type endpoint_;
    ifstream in;
    enum { max_length = 4096 };
    char data_[max_length];
};

void spiltAndSetClientInfo() {
    stringstream ss(env[env_Variables[2]]);
    string token;

    while (getline(ss, token, '&')) {
        cerr << "==========================" << endl;
        clientInfo c;
        c.host = token;
        cerr << token << endl;
        getline(ss, token, '&');
        c.port = token;
        cerr << token << endl;
        getline(ss, token, '&');
        c.testFile = token;
        cerr << token << endl;
        cerr << "==========================" << endl;
        clients.push_back(c);
    }
}

void setEnvVar() {
    for (int i = 0; i < 9; ++i) {
        cerr << "==================\n";
        env[env_Variables[i]] = getenv(env_Variables[i].c_str());
        cerr << env_Variables[i] << " : " << env[env_Variables[i]] << endl;
        cerr << "==================\n";
    }
}

void http() {
    cout << "<!DOCTYPE html>" << '\n';
    cout << "<html lang=\"en\">" << '\n';
    cout << "  <head>" << '\n';
    cout << "    <meta charset=\"UTF-8\" />" << '\n';
    cout << "    <title>NP Project 3 Sample Console</title>" << '\n';
    cout << "    <link" << '\n';
    cout << "      rel=\"stylesheet\"" << '\n';
    cout << "      "
            "href=\"https://cdn.jsdelivr.net/npm/bootstrap@4.5.3/dist/css/"
            "bootstrap.min.css\""
         << '\n';
    cout << "      "
            "integrity=\"sha384-TX8t27EcRE3e/"
            "ihU7zmQxVncDAy5uIKz4rEkgIXeMed4M0jlfIDPvg6uqKI2xXr2\""
         << '\n';
    cout << "      crossorigin=\"anonymous\"" << '\n';
    cout << "    />" << '\n';
    cout << "    <link" << '\n';
    cout << "      "
            "href=\"https://fonts.googleapis.com/css?family=Source+Code+Pro\""
         << '\n';
    cout << "      rel=\"stylesheet\"" << '\n';
    cout << "    />" << '\n';
    cout << "    <link" << '\n';
    cout << "      rel=\"icon\"" << '\n';
    cout << "      type=\"image/png\"" << '\n';
    cout << "      "
            "href=\"https://cdn0.iconfinder.com/data/icons/small-n-flat/24/"
            "678068-terminal-512.png\""
         << '\n';
    cout << "    />" << '\n';
    cout << "    <style>" << '\n';
    cout << "      * {" << '\n';
    cout << "        font-family: 'Source Code Pro', monospace;" << '\n';
    cout << "        font-size: 1rem !important;" << '\n';
    cout << "      }" << '\n';
    cout << "      body {" << '\n';
    cout << "        background-color: #212529;" << '\n';
    cout << "      }" << '\n';
    cout << "      pre {" << '\n';
    cout << "        color: #cccccc;" << '\n';
    cout << "      }" << '\n';
    cout << "      b {" << '\n';
    cout << "        color: #01b468;" << '\n';
    cout << "      }" << '\n';
    cout << "    </style>" << '\n';
    cout << "  </head>" << '\n';
    cout << "  <body>" << '\n';
    cout << "    <table class=\"table table-dark table-bordered\">" << '\n';
    cout << "      <thead>" << '\n';
    cout << "        <tr>" << '\n';
    for (unsigned long int i = 0; i < clients.size(); i++) {
        cout << "          <th scope=\"col\">" << clients[i].host << ":"
             << clients[i].port << "</th>" << '\n';
    }
    cout << "        </tr>" << '\n';
    cout << "      </thead>" << '\n';
    cout << "      <tbody>" << '\n';
    cout << "        <tr>" << '\n';
    for (unsigned long int i = 0; i < clients.size(); i++) {
        cout << "          <td><pre id=\"s" << i
             << "\" class=\"mb-0\"></pre></td>" << '\n';
    }
    cout << "        </tr>" << '\n';
    cout << "      </tbody>" << '\n';
    cout << "    </table>" << '\n';
    cout << "  </body>" << '\n';
    cout << "</html>" << '\n';
    cout << flush;
}

int main(int argc, char *argv[]) {

    try {
        cout << "Content-type: text/html\r\n\r\n" << flush;

        setEnvVar();
        spiltAndSetClientInfo();
        http();

        boost::asio::io_context io_context;
        client s(io_context, std::atoi(argv[1]));
        io_context.run();
    } catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}