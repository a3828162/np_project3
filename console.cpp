#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <utility>
#include <vector>

using boost::asio::ip::tcp;
using namespace std;

struct clientInfo {
    string hostName;
    string port;
    string testFile;
};

const string env_Variables[9] = {
    "REQUEST_METHOD",  "REQUEST_URI", "QUERY_STRING",
    "SERVER_PROTOCOL", "HTTP_HOST",   "SERVER_ADDR",
    "SERVER_PORT",     "REMOTE_ADDR", "REMOTE_PORT"};

map<string, string> env;
vector<clientInfo> clients(5);

class shellClient : public std::enable_shared_from_this<shellClient> {
  public:
    shellClient(boost::asio::io_context &io_context, int index)
        : resolver(io_context), socket_(io_context), index(index) {}

    void start() { do_resolve(); }

  private:
    void do_resolve() {
        auto self(shared_from_this());
        resolver.async_resolve(
            clients[index].hostName, clients[index].port,
            [this, self](boost::system::error_code ec,
                         tcp::resolver::results_type result) {
                if (!ec) {
                    memset(data_, '\0', sizeof(data_));
                    
                    do_connect(result);
                } else {
                    cerr << "resolv error code: " << ec.message() << '\n';
                }
            });
    }

    void do_connect(tcp::resolver::results_type &result) {
        auto self(shared_from_this());
        boost::asio::async_connect(
            socket_, result,
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
                    cerr << "connect error code: " << ec.message() << '\n';
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
                    string tr_msg = transform_http_type(msg);
                    cout << "<script>document.getElementById('s" << index
                        << "').innerHTML += '" << tr_msg << "';</script>\n"
                        << flush;
                    if (msg.find("% ") != string::npos) {
                        do_write();
                    } else {
                        do_read();
                    }
                } else {
                    cerr << "read error code: " << ec.message() << '\n';
                    socket_.close();
                }
            });
    }

    void do_write() {
        auto self(shared_from_this());
        string cmd;
        getline(in, cmd);
        cmd.push_back('\n');
        string tr_cmd = transform_http_type(cmd);
        cout << "<script>document.getElementById('s" << index
             << "').innerHTML += '<b>" << tr_cmd << "</b>';</script>\n"
             << flush;
        boost::asio::async_write(
            socket_, boost::asio::buffer(cmd, cmd.size()),
            [this, self, cmd](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    cmd == "exit\n" ? socket_.close() : do_read();
                }
            });
    }

    string transform_http_type(string &input) {
        string output = "";
        for(auto &s : input){
            if(s == '&') output+="&amps";
            else if(s == '\r') output+="";
            else if(s== '\n') output+="<br>";
            else if(s=='\'') output+="\\'";
            else if(s=='<') output+="&lt;";
            else if(s=='>') output+='&gt;';
            else output+=s;
        }

        return output;
    }
    tcp::resolver resolver;
    tcp::socket socket_;
    int index;
    //tcp::resolver::results_type endpoint_;
    ifstream in;
    enum { max_length = 4096 };
    char data_[max_length];
};

void setEnvVar() {
    for (auto &s : env_Variables) {
        env[s] = string(getenv(s.c_str()));
    }
}

void setClientInfo() {
    string query = env["QUERY_STRING"];
    stringstream ss(query);
    string token;
    int i = 0;
    while (getline(ss, token, '&')) {
        clients[i].hostName =
            token.substr(token.find("=") + 1, token.size() - 1);
        getline(ss, token, '&');
        clients[i].port = token.substr(token.find("=") + 1, token.size() - 1);
        getline(ss, token, '&');
        clients[i].testFile =
            token.substr(token.find("=") + 1, token.size() - 1);
        ++i;
    }
}

void printhttp() {
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
    for (int i = 0; i < clients.size(); i++) {
        cout << "          <th scope=\"col\">" << clients[i].hostName << ":"
             << clients[i].port << "</th>" << '\n';
    }
    cout << "        </tr>" << '\n';
    cout << "      </thead>" << '\n';
    cout << "      <tbody>" << '\n';
    cout << "        <tr>" << '\n';
    for (int i = 0; i < clients.size(); i++) {
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

int main() {
    try {
        cout << "Content-type: text/html\r\n\r\n" << flush;
        setEnvVar();
        setClientInfo();
        printhttp();
        boost::asio::io_context io_context;
        for (int i = 0; i < clients.size(); i++) {
            std::make_shared<shellClient>(io_context, i)->start();
        }
        io_context.run();
    } catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}