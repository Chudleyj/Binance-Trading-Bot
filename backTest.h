#ifndef backTest_h
#define backTest_h

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
#include <stdlib.h> //abs val

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
using namespace std;
using namespace std::chrono;                //Allows us to get epoch time without having to ping Binance server for it

#define RESTfulHost "https://api.binance.com/"

class serverData{
  double price;
  string pair;
  string epochTime;
  string signature; //THIS IS THE HMAC SHA256 SIGNATURE. USES MESSAGE AND YOUR SECRET API KEY
  string secretKey = "YOUR SECRET KEY HERE"; //THIS IS THE KEY FOR THE HMAC SHA256
  string APIKEY = "YOUR KEY HERE";
 public:
  void setPair();
  void inputPair(const string input){this->pair = input;}
  void setPrice();
  void inputPrice(double foundPrice);
  void getTime(); //Get epoch time (It will be needed for the signature)
  void HMACsha256(string const&, string const&);
  void setSignature(const string sig){this->signature=sig;}
  auto getPrice()->double{return this->price;}
  auto getPair()->string{return this->pair;}
}servData;

class backTesting{
  double USDT, BTC=0, LTC=0, ETH=0, BCC=0, BNB=0, NEO=0, ADA=0, QTUM=0, total;
  double pastPrice;
 public:
  void backTest();//temp
  void simBuy();
  void simSell();
  void getTotal();
}backTest;

class algorithmBot{
  double sellPercent;
  bool algoCheck,algoBuy;
 public:
    void setSellPercent();
    void checkBuy();
    void checkSell();
    void algoBot();
}algoBot;

class TechnicalAnalysis{
  vector<long double> RSI, stochRSI;
  public:
    void calcRSI();
    void calcStochRSI();
    auto getRSIback()->long double{return RSI.back();}
}TechnicalAnalysisCalcs;

class pricingData{
    vector<long> openTime,closeTime,numTrades;
    vector <long double> open, high, low, close, volume, quoteAssetVolume,takerBuyAssetVol,takerBuyQuoteAssetVol,ignore;
 public:
    friend class TechnicalAnalysis;
    void tradingBot();
    void formatPrice(json::value const &);
    void getHistoricalPrices();
    void formatHistoricalPrices(json::value const &);
    void clearVecs();
}priceData;

#endif /* BinanceBot_h */
