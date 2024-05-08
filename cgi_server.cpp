#include <boost/algorithm/string/replace.hpp>
#include <boost/asio.hpp>
#include <boost/format.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string.h>
#include <string>
#include <utility>
#include <vector>

using namespace boost::asio;
using namespace boost::asio::ip;
using boost::asio::ip::tcp;
using namespace std;

struct clientInfo {
    string hostName;
    string port;
    string testFile;
};

string header = "Content-type: text/html\r\n\r\n";
string get_panel_html() {
    string panelHead = R"(
	<!DOCTYPE html>
	<html lang="en">
	  <head>
	    <title>NP Project 3 Panel</title>
	    <link
	      rel="stylesheet"
	      href="https://cdn.jsdelivr.net/npm/bootstrap@4.5.3/dist/css/bootstrap.min.css"
	      integrity="sha384-TX8t27EcRE3e/ihU7zmQxVncDAy5uIKz4rEkgIXeMed4M0jlfIDPvg6uqKI2xXr2"
	      crossorigin="anonymous"
	    />
	    <link
	      href="https://fonts.googleapis.com/css?family=Source+Code+Pro"
	      rel="stylesheet"
	    />
	    <link
	      rel="icon"
	      type="image/png"
	      href="https://cdn4.iconfinder.com/data/icons/iconsimple-setting-time/512/dashboard-512.png"
	    />
	    <style>
	      * {
	        font-family: 'Source Code Pro', monospace;
	      }
	    </style>
	  </head>
	  <body class="bg-secondary pt-5">
	  	<form action="console.cgi" method="GET">
	      <table class="table mx-auto bg-light" style="width: inherit">
	        <thead class="thead-dark">
	          <tr>
	            <th scope="col">#</th>
	            <th scope="col">Host</th>
	            <th scope="col">Port</th>
	            <th scope="col">Input File</th>
	          </tr>
	        </thead>
	        <tbody>
	)";

    string panelBody1 = R"(
         <tr>
               <th scope="row" class="align-middle">Session 1</th>
               <td>
                 <div class="input-group">
                   <select name="h0" class="custom-select">
                     <option></option>
                     <option value="nplinux1.cs.nycu.edu.tw">nplinux1</option>
                     <option value="nplinux2.cs.nycu.edu.tw">nplinux2</option>
                     <option value="nplinux3.cs.nycu.edu.tw">nplinux3</option>
                     <option value="nplinux4.cs.nycu.edu.tw">nplinux4</option>
                     <option value="nplinux5.cs.nycu.edu.tw">nplinux5</option>
                     <option value="nplinux6.cs.nycu.edu.tw">nplinux6</option>
                     <option value="nplinux7.cs.nycu.edu.tw">nplinux7</option>
                     <option value="nplinux8.cs.nycu.edu.tw">nplinux8</option>
                     <option value="nplinux9.cs.nycu.edu.tw">nplinux9</option>
                     <option value="nplinux10.cs.nycu.edu.tw">nplinux10</option>
                     <option value="nplinux11.cs.nycu.edu.tw">nplinux11</option>
                     <option value="nplinux12.cs.nycu.edu.tw">nplinux12</option>
                   </select>
                   <div class="input-group-append">
                     <span class="input-group-text">.cs.nycu.edu.tw</span>
                   </div>
                 </div>
               </td>
               <td>
                 <input name="p0" type="text" class="form-control" size="5" />
               </td>
               <td>
                 <select name="f0" class="custom-select">
                   <option></option>
                   <option value="t1.txt">t1.txt</option>
                   <option value="t2.txt">t2.txt</option>
                   <option value="t3.txt">t3.txt</option>
                   <option value="t4.txt">t4.txt</option>
                   <option value="t5.txt">t5.txt</option>
                 </select>
               </td>
         </tr>
       )";

    string panelBody2 = R"(
         <tr>
               <th scope="row" class="align-middle">Session 2</th>
               <td>
                 <div class="input-group">
                   <select name="h1" class="custom-select">
                     <option></option>
                     <option value="nplinux1.cs.nycu.edu.tw">nplinux1</option>
                     <option value="nplinux2.cs.nycu.edu.tw">nplinux2</option>
                     <option value="nplinux3.cs.nycu.edu.tw">nplinux3</option>
                     <option value="nplinux4.cs.nycu.edu.tw">nplinux4</option>
                     <option value="nplinux5.cs.nycu.edu.tw">nplinux5</option>
                     <option value="nplinux6.cs.nycu.edu.tw">nplinux6</option>
                     <option value="nplinux7.cs.nycu.edu.tw">nplinux7</option>
                     <option value="nplinux8.cs.nycu.edu.tw">nplinux8</option>
                     <option value="nplinux9.cs.nycu.edu.tw">nplinux9</option>
                     <option value="nplinux10.cs.nycu.edu.tw">nplinux10</option>
                     <option value="nplinux11.cs.nycu.edu.tw">nplinux11</option>
                     <option value="nplinux12.cs.nycu.edu.tw">nplinux12</option>
                   </select>
                   <div class="input-group-append">
                     <span class="input-group-text">.cs.nycu.edu.tw</span>
                   </div>
                 </div>
               </td>
               <td>
                 <input name="p1" type="text" class="form-control" size="5" />
               </td>
               <td>
                 <select name="f1" class="custom-select">
                   <option></option>
                   <option value="t1.txt">t1.txt</option>
                   <option value="t2.txt">t2.txt</option>
                   <option value="t3.txt">t3.txt</option>
                   <option value="t4.txt">t4.txt</option>
                   <option value="t5.txt">t5.txt</option>
                 </select>
               </td>
         </tr>
       )";

    string panelBody3 = R"(
         <tr>
               <th scope="row" class="align-middle">Session 3</th>
               <td>
                 <div class="input-group">
                   <select name="h2" class="custom-select">
                     <option></option>
                     <option value="nplinux1.cs.nycu.edu.tw">nplinux1</option>
                     <option value="nplinux2.cs.nycu.edu.tw">nplinux2</option>
                     <option value="nplinux3.cs.nycu.edu.tw">nplinux3</option>
                     <option value="nplinux4.cs.nycu.edu.tw">nplinux4</option>
                     <option value="nplinux5.cs.nycu.edu.tw">nplinux5</option>
                     <option value="nplinux6.cs.nycu.edu.tw">nplinux6</option>
                     <option value="nplinux7.cs.nycu.edu.tw">nplinux7</option>
                     <option value="nplinux8.cs.nycu.edu.tw">nplinux8</option>
                     <option value="nplinux9.cs.nycu.edu.tw">nplinux9</option>
                     <option value="nplinux10.cs.nycu.edu.tw">nplinux10</option>
                     <option value="nplinux11.cs.nycu.edu.tw">nplinux11</option>
                     <option value="nplinux12.cs.nycu.edu.tw">nplinux12</option>
                   </select>
                   <div class="input-group-append">
                     <span class="input-group-text">.cs.nycu.edu.tw</span>
                   </div>
                 </div>
               </td>
               <td>
                 <input name="p2" type="text" class="form-control" size="5" />
               </td>
               <td>
                 <select name="f2" class="custom-select">
                   <option></option>
                   <option value="t1.txt">t1.txt</option>
                   <option value="t2.txt">t2.txt</option>
                   <option value="t3.txt">t3.txt</option>
                   <option value="t4.txt">t4.txt</option>
                   <option value="t5.txt">t5.txt</option>
                 </select>
               </td>
         </tr>
       )";

    string panelBody4 = R"(
         <tr>
               <th scope="row" class="align-middle">Session 4</th>
               <td>
                 <div class="input-group">
                   <select name="h3" class="custom-select">
                     <option></option>
                     <option value="nplinux1.cs.nycu.edu.tw">nplinux1</option>
                     <option value="nplinux2.cs.nycu.edu.tw">nplinux2</option>
                     <option value="nplinux3.cs.nycu.edu.tw">nplinux3</option>
                     <option value="nplinux4.cs.nycu.edu.tw">nplinux4</option>
                     <option value="nplinux5.cs.nycu.edu.tw">nplinux5</option>
                     <option value="nplinux6.cs.nycu.edu.tw">nplinux6</option>
                     <option value="nplinux7.cs.nycu.edu.tw">nplinux7</option>
                     <option value="nplinux8.cs.nycu.edu.tw">nplinux8</option>
                     <option value="nplinux9.cs.nycu.edu.tw">nplinux9</option>
                     <option value="nplinux10.cs.nycu.edu.tw">nplinux10</option>
                     <option value="nplinux11.cs.nycu.edu.tw">nplinux11</option>
                     <option value="nplinux12.cs.nycu.edu.tw">nplinux12</option>
                   </select>
                   <div class="input-group-append">
                     <span class="input-group-text">.cs.nycu.edu.tw</span>
                   </div>
                 </div>
               </td>
               <td>
                 <input name="p3" type="text" class="form-control" size="5" />
               </td>
               <td>
                 <select name="f3" class="custom-select">
                   <option></option>
                   <option value="t1.txt">t1.txt</option>
                   <option value="t2.txt">t2.txt</option>
                   <option value="t3.txt">t3.txt</option>
                   <option value="t4.txt">t4.txt</option>
                   <option value="t5.txt">t5.txt</option>
                 </select>
               </td>
         </tr>
       )";

    string panelBody5 = R"(
         <tr>
               <th scope="row" class="align-middle">Session 5</th>
               <td>
                 <div class="input-group">
                   <select name="h4" class="custom-select">
                     <option></option>
                     <option value="nplinux1.cs.nycu.edu.tw">nplinux1</option>
                     <option value="nplinux2.cs.nycu.edu.tw">nplinux2</option>
                     <option value="nplinux3.cs.nycu.edu.tw">nplinux3</option>
                     <option value="nplinux4.cs.nycu.edu.tw">nplinux4</option>
                     <option value="nplinux5.cs.nycu.edu.tw">nplinux5</option>
                     <option value="nplinux6.cs.nycu.edu.tw">nplinux6</option>
                     <option value="nplinux7.cs.nycu.edu.tw">nplinux7</option>
                     <option value="nplinux8.cs.nycu.edu.tw">nplinux8</option>
                     <option value="nplinux9.cs.nycu.edu.tw">nplinux9</option>
                     <option value="nplinux10.cs.nycu.edu.tw">nplinux10</option>
                     <option value="nplinux11.cs.nycu.edu.tw">nplinux11</option>
                     <option value="nplinux12.cs.nycu.edu.tw">nplinux12</option>
                   </select>
                   <div class="input-group-append">
                     <span class="input-group-text">.cs.nycu.edu.tw</span>
                   </div>
                 </div>
               </td>
               <td>
                 <input name="p4" type="text" class="form-control" size="5" />
               </td>
               <td>
                 <select name="f4" class="custom-select">
                   <option></option>
                   <option value="t1.txt">t1.txt</option>
                   <option value="t2.txt">t2.txt</option>
                   <option value="t3.txt">t3.txt</option>
                   <option value="t4.txt">t4.txt</option>
                   <option value="t5.txt">t5.txt</option>
                 </select>
               </td>
         </tr>
       )";

    string panelBody6 = R"(
			<tr>
	          <td colspan="3"></td>
	          <td>
	            <button type="submit" class="btn btn-info btn-block">Run</button>
	          </td>
	        </tr>
	      </tbody>
	    </table>
	  </form>
	  </body>
	  </html>
	)";
    return header + panelHead + panelBody1 + panelBody2 + panelBody3 +
           panelBody4 + panelBody5 + panelBody6;
}

string get_console_html(vector<clientInfo> cnt) {
    string consoleHead = R"(
		<!DOCTYPE html>
		<html lang="en">
		  <head>
		    <meta charset="UTF-8" />
		    <title>NP Project 3 Sample Console</title>
		    <link
		      rel="stylesheet"
		      href="https://cdn.jsdelivr.net/npm/bootstrap@4.5.3/dist/css/bootstrap.min.css"
		      integrity="sha384-TX8t27EcRE3e/ihU7zmQxVncDAy5uIKz4rEkgIXeMed4M0jlfIDPvg6uqKI2xXr2"
		      crossorigin="anonymous"
		    />
		    <link
		      href="https://fonts.googleapis.com/css?family=Source+Code+Pro"
		      rel="stylesheet"
		    />
		    <link
		      rel="icon"
		      type="image/png"
		      href="https://cdn0.iconfinder.com/data/icons/small-n-flat/24/678068-terminal-512.png"
		    />
		    <style>
		      * {
		        font-family: 'Source Code Pro', monospace;
		        font-size: 1rem !important;
		      }
		      body {
		        background-color: #212529;
		      }
		      pre {
		        color: #cccccc;
		      }
		      b {
		        color: #01b468;
		      }
		    </style>
		  </head>
		  <body>
		    <table class="table table-dark table-bordered">
		      <thead>
		        <tr>
	)";
    string consoleBody1;
    for (int i = 0; i < cnt.size(); i++) {
        if (cnt[i].hostName != "" && cnt[i].port != "" &&
            cnt[i].testFile != "") {
            consoleBody1 += "<th scope=\"col\">" + cnt[i].hostName + ":" +
                            cnt[i].port + "</th>\r\n";
        }
    }

    string consoleBody2 = R"(
				</tr>
		      </thead>
		      <tbody>
		        <tr>
	)";

    string consoleBody3;
    for (int i = 0; i < cnt.size(); i++) {
        if (cnt[i].hostName != "" && cnt[i].port != "" &&
            cnt[i].testFile != "") {
            consoleBody3 += "<td><pre id=\"s" + to_string(i) +
                            "\" class=\"mb-0\"></pre></td>\r\n";
        }
    }
    string consoleBody4 = R"(
		        </tr>
		      </tbody>
		    </table>
		  </body>
		</html>
	)";

    return header + consoleHead + consoleBody1 + consoleBody2 + consoleBody3 +
           consoleBody4;
}

const string env_Variables[9] = {
    "REQUEST_METHOD",  "REQUEST_URI", "QUERY_STRING",
    "SERVER_PROTOCOL", "HTTP_HOST",   "SERVER_ADDR",
    "SERVER_PORT",     "REMOTE_ADDR", "REMOTE_PORT"};

vector<clientInfo> clients(5);

class shellClient : public std::enable_shared_from_this<shellClient> {
  public:
    shellClient(boost::asio::io_context &io_context,
                tcp::socket remoteServerSocket,
                shared_ptr<tcp::socket> shared_client, int c_index)
        : resolver(io_context), socket_(move(remoteServerSocket)),
          shared_client_(shared_client) {
        index = c_index;
        clientPtr = &clients[index];
    }

    void start() {
        cout << "Start Remote Server\n";
        cmds.clear();
        readCommandFromFile();
        do_resolve();
    }

  private:
    void do_resolve() {
        auto self(shared_from_this());
        cout << "Resolving " << clientPtr->hostName << ":" << clientPtr->port
             << endl;
        tcp::resolver::query query(clientPtr->hostName, clientPtr->port);
        resolver.async_resolve(
            query, [this, self](boost::system::error_code ec,
                                tcp::resolver::results_type results) {
                if (!ec) {
                    memset(data_, '\0', sizeof(data_));
                    do_connect(results);
                } else {
                    cerr << "resolv error code: " << ec.message() << '\n';
                }
            });
    }

    void do_connect(tcp::resolver::results_type results) {
        auto self(shared_from_this());
        boost::asio::async_connect(
            socket_, results,
            [this, self](boost::system::error_code ec, tcp::endpoint) {
                if (!ec) {
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
            [this, self](boost::system::error_code ec, size_t length) {
                if (!ec) {
                    data_[length] = '\0';
                    string msg(data_);
                    cout << "Read from " << clientPtr->hostName << ":"
                         << clientPtr->port << " : " << msg << endl;
                    memset(data_, '\0', sizeof(data_));

                    string outputTmp = htmlEscape(msg);
                    string output = "<script>document.getElementById(\'s" +
                                    to_string(index) + "\').innerHTML += \'" +
                                    outputTmp + "\';</script>";
                    do_writeClient(output);
                    if (msg.find("%") != string::npos) {
                        if (cmds.size() > 0) {
                            string cmd = cmds.front();
                            cmds.erase(cmds.begin());

                            string commandTmp = htmlEscape(cmd);
                            string command =
                                "<script>document.getElementById(\'s" +
                                to_string(index) + "\').innerHTML += \'<b>" +
                                commandTmp + "</b>\';</script>";
                            do_writeClient(command);
                            do_writeRemoteServer(cmd);
                        }
                    } else {
                        do_read();
                    }
                }
            });
    }

    void do_writeClient(string output) {
        auto self(shared_from_this());
        boost::asio::async_write(
            *shared_client_,
            boost::asio::buffer(output.c_str(), output.length()),
            [this, self](boost::system::error_code ec, size_t) {
                if (!ec) {
                    cout << "Write to client OK" << endl;
                }
            });
    }

    void do_writeRemoteServer(string command) {
        auto self(shared_from_this());
        boost::asio::async_write(
            socket_, boost::asio::buffer(command.c_str(), command.length()),
            [this, self, command](boost::system::error_code ec, size_t) {
                if (!ec) {
                    cout << "Write to " << clientPtr->hostName << ":"
                         << clientPtr->port << " OK" << endl;
                    if (command.find("exit") == string::npos) {
                        do_read();

                    } else {
                        socket_.close();
                        cout << "Remote closed!!\n";
                    }
                }
            });
    }

    string htmlEscape(string str) {
        boost::replace_all(str, "&", "&amp;");
        boost::replace_all(str, ">", "&gt;");
        boost::replace_all(str, "<", "&lt;");
        boost::replace_all(str, "\"", "&quot;");
        boost::replace_all(str, "\'", "&apos;");
        boost::replace_all(str, "\n", "&NewLine;");
        boost::replace_all(str, "\r", "");
        return str;
    }

    void readCommandFromFile() {
        std::ifstream ifs("test_case/" + clientPtr->testFile);
        string line;
        while (getline(ifs, line)) {
            cmds.push_back(line + "\n");
            // cout << "line: " << line << endl;
        }
    }

    vector<string> cmds;                    // store commands
    tcp::resolver resolver;                 // resolve host name to ip address
    tcp::socket socket_;                    // socket to remote np server
    shared_ptr<tcp::socket> shared_client_; // share ptr to browser socket
    clientInfo *clientPtr;                  // pointer to clientInfo
    int index;                              // index of client in clients vector
    enum { max_length = 4096 };             // max length of data
    char data_[max_length];                 // data buffer
};

class session : public std::enable_shared_from_this<session> {
  public:
    session(tcp::socket socket, boost::asio::io_context &io_context)
        : socket_(std::move(socket)), io_context_(io_context) {}

    void start() { do_read(); }

  private:
    void do_read() {
        auto self(shared_from_this());

        socket_.async_read_some(
            boost::asio::buffer(data_, max_length),
            [this, self](boost::system::error_code ec, std::size_t length) {
                if (!ec) {

                    data_[length] = '\0';
                    http_request_parser();
                    memset(data_, '\0', length);
                    cgi_handler();
                }
            });
    }

    void cgi_handler() {

        auto self(shared_from_this());
        strcpy(data_, "HTTP/1.1 200 OK\r\n");
        boost::asio::async_write(
            socket_, boost::asio::buffer(data_, strlen(data_)),
            [this, self](boost::system::error_code ec, size_t s) {
                string cgiFileName = env[env_Variables[1]].substr(
                    0, env[env_Variables[1]].find("?"));
                cout << cgiFileName << endl;
                if (cgiFileName == "/panel.cgi") {
                    panel_handler();
                } else if (cgiFileName == "/console.cgi") {
                    clients.clear();
                    clients.assign(5, clientInfo{});
                    setClientInfo();
                    console_handler();
                    shared_ptr<tcp::socket> shared_client_ =
                        make_shared<tcp::socket>(move(socket_));
                    for (int i = 0; i < clients.size(); i++) {
                        if (clients[i].hostName != "" &&
                            clients[i].port != "" &&
                            clients[i].testFile != "") {
                            tcp::socket remoteClient_(io_context_);
                            make_shared<shellClient>(io_context_,
                                                     move(remoteClient_),
                                                     shared_client_, i)
                                ->start();
                        }
                    }
                } else {
                    socket_.close();
                }
            });
    }

    void panel_handler() {
        auto self(shared_from_this());
        string panel_page = get_panel_html();

        boost::asio::async_write(
            socket_,
            boost::asio::buffer(panel_page.c_str(), panel_page.length()),
            [this, self](boost::system::error_code ec, size_t) {
                if (!ec) {
                    cout << "Write Panel.cgi OK" << endl;
                }
            });
    }

    void console_handler() {
        auto self(shared_from_this());
        string console_page = get_console_html(clients);

        boost::asio::async_write(
            socket_,
            boost::asio::buffer(console_page.c_str(), console_page.length()),
            [this, self](boost::system::error_code ec, size_t) {
                if (!ec) {
                    cout << "Write Console.cgi OK" << endl;
                }
            });
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
            clients[i].port =
                token.substr(token.find("=") + 1, token.size() - 1);
            getline(ss, token, '&');
            clients[i].testFile =
                token.substr(token.find("=") + 1, token.size() - 1);
            ++i;
        }

        /*for (int i = 0; i < clients.size(); i++) {
            cout << "hostName: " << clients[i].hostName << endl;
            cout << "port: " << clients[i].port << endl;
            cout << "testFile: " << clients[i].testFile << endl;
        }*/
    }

    void http_request_parser() {
        stringstream ss;
        ss << string(data_);
        string token;
        vector<string> tmp;
        while (ss >> token)
            tmp.push_back(token);

        env[env_Variables[0]] = tmp[0];
        env[env_Variables[1]] = tmp[1];
        env[env_Variables[3]] = tmp[2];
        env[env_Variables[4]] = tmp[4];
        env[env_Variables[5]] = socket_.local_endpoint().address().to_string();
        env[env_Variables[6]] = to_string(socket_.local_endpoint().port());
        env[env_Variables[7]] = socket_.remote_endpoint().address().to_string();
        env[env_Variables[8]] = to_string(socket_.remote_endpoint().port());

        if (env[env_Variables[1]].find("?") != string::npos) {
            env[env_Variables[2]] = env[env_Variables[1]].substr(
                env[env_Variables[1]].find("?") + 1);
        } else {
            env[env_Variables[2]] = "";
        }

        for (auto &[key, value] : env) {
            // cout << key << " : " << value << endl;
        }
    }

    boost::asio::io_context &io_context_;
    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
    map<string, string> env;
};

class server {
  public:
    server(boost::asio::io_context &io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
          io_context_(io_context) {
        do_accept();
    }

  private:
    void do_accept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::cout << "Server: Accept...\n";
                    std::make_shared<session>(std::move(socket), io_context_)
                        ->start();
                }

                do_accept();
            });
    }

    boost::asio::io_context &io_context_;
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