# Binance-Trading-Bot
A C++ bot that uses Bianance's RESTful API alongside Microsoft's C++ RESTsdk

Back testing of this bot is now up and running. It is using a simple algorithm but works 100%. So far, the algorithm has shown 5% profits over a 30 day period. Algorithm definetly needs massive improvements and far more technical indictaors, but the framework of this bot works. 

Binance API: https://github.com/binance-exchange/binance-official-api-docs

Prerequisites: 

https://github.com/Microsoft/cpprestsdk

C++ 11 or higher

To Run:
g++ -std=c++11 BinanceBot.cpp -stdlib=libc++ -lcpprest -lssl -lcrypto -t/openssl/lib -I/usr/local/opt/openssl/include-mt -L/usr/local/opt
