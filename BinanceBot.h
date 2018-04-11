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
#include <fstream> //File opens/closes
#include <vector>
#include <boost/algorithm/string.hpp> //Boost::erase

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
using namespace std;
using namespace std::chrono;                //Allows us to get epoch time without having to ping Binance server for it

#define RESTfulHost "https://api.binance.com/"

class botData{
    double price,pastPrice,sellPercent,RSI;
    vector<long> openTime,closeTime,numTrades;
    vector <long double> open, high, low, close, volume, quoteAssetVolume,takerBuyAssetVol,takerBuyQuoteAssetVol,Ignore;
    bool algoCheck,algoBuy;
    string pair;
    string epochTime;
    string signature; //THIS IS THE HMAC SHA256 SIGNATURE. USES MESSAGE AND YOUR SECRET API KEY
    string secretKey = "YOUR SECRET KEY HERE"; //THIS IS THE KEY FOR THE HMAC SHA256
    string APIKEY = "YOUR KEY HERE";

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
    void getHistoricalPrices();
    void formatHistoricalPrices(json::value const &);
    void calcRSI();
    
    
}bot;








#endif /* BinanceBot_h */
