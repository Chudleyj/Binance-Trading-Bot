#ifndef BinanceBot_h
#define BinanceBot_h

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/ws_client.h>
#include <openssl/hmac.h>
#include <iostream>
#include <string>
#include <chrono>
#include <sstream>
#include <stdio.h>
#include <iomanip> //Allows one to see prices of small satoshi's worth coins.

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
using namespace std;
using namespace std::chrono;                //Allows us to get epoch time without having to ping Binance server for it

#define RESTfulHost "https://api.binance.com/"

class botData{
    double price,pastPrice,sellPercent;
    string pair;
    string epochTime;
    string signature; //THIS IS THE HMAC SHA256 SIGNATURE. USES MESSAGE AND YOUR SECRET API KEY
    bool algoCheck,algoBuy;
public:
    void setPair();
    void setPrice(double foundPrice);
    void setSellPercent();
    void init();
    void tradingBot();
    void algoBot();
    void getPrice(const string);
    void formatPrice(json::value const &);
    void getTime(); //Get epoch time (It will be needed for the signature)
    void HMACsha256(string const&, string const&);
    void checkBuy();
    void checkSell();
    
    
    
}bot;









#endif /* BinanceBot_h */
