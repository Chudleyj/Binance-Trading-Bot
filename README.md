# Binance-Trading-Bot
A C++ bot that uses Bianance's RESTful API alongside Microsoft's C++ RESTsdk

So far this bot has only been confirmed to connect to Binance's RESTful API. 
The implementation of the HMAC SHA256 algorithim to generate a signiture works, but the rest of the program is not yet fitted to run with it as I wrote it in C and the rest is in C++. 

Binance API: https://github.com/binance-exchange/binance-official-api-docs

Prerequisites: 

https://github.com/Microsoft/cpprestsdk
C++ 11 or higher

To Run:

g++ -std=c++11 BinanceBot.cpp -stdlib=libc++ -lcpprest -lssl -lcrypto -t/openssl/lib -I/usr/local/opt/openssl/include-mt -L/usr/local/opt
