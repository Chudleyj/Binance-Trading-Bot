#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/ws_client.h>
#include <openssl/hmac.h>
#include <iostream>
#include <string>
#include <chrono>
#include <sstream>
#include <stdio.h>

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
using namespace std;
using namespace std::chrono;                //Allows us to get epoch time without having to ping Binance server for it

string getTime(); //Get epoch time (It will be needed for the signature)
string HMACsha256(string const&, string const&); //Generate HMACsha256 signature
void init();
void tradingBot();
void getPrice(const string);
void formatPrice(json::value const &);

int main(int argc, char* argv[])
{
    cout << endl << "Welcome to the C++ Binance Bot. WARNING --> This is still an extremly early alpha build. Use at your own risk." << endl;
    init();
   // search();
    return 0;
}

void init(){
    string input;
    cout << endl << "Choose bot mode: 1 = trading" << endl;
    cin >> input;
    (input == "1") ? tradingBot() : init();
}

void tradingBot(){
    string pair, choice, price, amount;
    
    cout << "Select trading pair (e.g. LTCBTC, ETHBTC,TRXETH, etc.)" << endl;
    cin >> pair;
    
    cout << "Selected " << pair << ". Last known price of " << pair << ": ";
    getPrice(pair);
    
    cout << "Buying or selling? (1 for buying, 0 for selling)" << endl;
    cin >> choice;
    (choice == "0") ? choice = "SELL" : choice = "BUY";
    
    cout << "Enter the price you would like to buy/sell at." << endl;
    cin >> price;
    
    cout << "Enter amount you would like to buy/sell." << endl;
    cin >> amount;
    
    string curlTest = "curl -H \"X-MBX-APIKEY: ";
    string secret = "YOURSECRETKEY"; //THIS IS THE KEY FOR THE HMAC SHA256
    string APIKEY = "YOURPUBLICKEY";
    string command = "\" -X POST 'https://api.binance.com/api/v3/order?symbol="+pair+"&side="+choice+"&type=LIMIT&timeInForce=GTC&quantity="+amount+"&price="+price+"&recvWindow=5000&timestamp=";
    string time = getTime();
    string message = "symbol="+pair+"&side="+choice+"&type=LIMIT&timeInForce=GTC&quantity="+amount+"&price="+price+"&recvWindow=5000&timestamp="+time;
    auto api = HMACsha256(message, secret); //THIS IS THE HMAC SHA256 SIGNATURE. USES MESSAGE AND YOUR SECRET API KEY
    curlTest = curlTest+APIKEY+command+time+"&signature="+api+"'";
    cout << endl << endl << curlTest << endl << endl;
    system(curlTest.c_str());
    
    
}

void printPrice(json::value const & value){
    if(!value.is_null()){
        json::value test = value;
        cout << test["price"] << endl;
    }
    
}

void getPrice(const string pair){
    http_client client(U("https://api.binance.com/"));
    string full_request = "/api/v3/ticker/price?symbol=" + pair;
    client.request(methods::GET, full_request).then([](http_response response)->
                                                      pplx::task<json::value>{
                                                          if(response.status_code() == status_codes::OK){
                                                              return response.extract_json();
                                                          }
                                                          return pplx::task_from_result(json::value());
                                                      })
    .then([](pplx::task<json::value> previousTask){
        try{
            json::value const & v = previousTask.get();
            printPrice(v);
        }
        catch (http_exception const & e){
            wcout << e.what() << endl;
        }
    }).wait();
}

inline char binary_to_hex_digit(unsigned a) {
    return a + (a < 10 ? '0' : 'a' - 10);
}

std::string binary_to_hex(unsigned char const* binary, unsigned binary_len) {
    string r(binary_len * 2, '\0');
    for(unsigned i = 0; i < binary_len; ++i) {
        r[i * 2] = binary_to_hex_digit(binary[i] >> 4);
        r[i * 2 + 1] = binary_to_hex_digit(binary[i] & 15);
    }
    return r;
}

string HMACsha256(string const& message, string const& key) {
    unsigned char result[EVP_MAX_MD_SIZE];
    unsigned result_len = 0;
    HMAC(EVP_sha256(), key.data(), key.size(), reinterpret_cast<unsigned char const*>(message.data()), message.size(), result, &result_len);
    return binary_to_hex(result, result_len);
}



string getTime(){
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long long mslong = (long long) tp.tv_sec * 1000L + tp.tv_usec / 1000;
    auto time = to_string(mslong);
    return time;
}
