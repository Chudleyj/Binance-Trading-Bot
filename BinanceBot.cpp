#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/ws_client.h>
#include <openssl/hmac.h>
#include <iostream>
#include <string>
#include <chrono>

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
using namespace web;
using namespace web::websockets::client;
using namespace std;
using namespace std::chrono;

void print_test(json::value const &);
void search();
void test();
string getTime();
string HMACsha256(string);

int main(int argc, char* argv[])
{
    search();
    return 0;
}

void search(){
    
    string coin,target = "/api/v3/ticker/price?symbol=";
   // cout << "Enter coin pairing (EX: LTCBTC)";
   // cin >> coin;
    target = target+coin;
   // cout << target;
    string curlTest = "curl -H \"X-MBX-APIKEY: ";
    string secret = "YOURPRIVATEKEYHERE\" "; //THIS IS THE KEY FOR THE HMAC SHA256
    /* THE MESSAGE for it is:
        'https://api.binance.com/api/v3/order/test?symbol=LTCBTC&side=BUY&type=LIMIT&timeInForce=GTC' -d 'quantity=1&price=0.1&recvWindow=6000000&timestamp=WHATEVERTHECURRENTEPOCHTIMEIS;&signature=69452f1d9ac1ff6cfada72cdd81b025372b3841e52a747a00eb4dfcde7203853'"
     */
    string command = "-X POST 'https://api.binance.com/api/v3/order/test?symbol=LTCBTC&side=BUY&type=LIMIT&timeInForce=GTC' -d 'quantity=1&price=0.1&recvWindow=6000000&timestamp=";
    string time = getTime();
    string sig = "&signature=";
    string message = "symbol=LTCBTC&side=BUY&type=LIMIT&timeInForce=GTC' -d 'quantity=1&price=0.1&recvWindow=6000000&timestamp=" + time;
    string api = HMACsha256(message); //THIS IS THE HMAC SHA256 SIGNATURE. IT NEEDS TO CHANGE EVERY TIME "time" CHANGES
    curlTest = curlTest+secret+command+time+sig+api;
   // cout << endl << endl << curlTest << endl << endl;
    system(curlTest.c_str());
}
string HMACsha256(string message){
    // The secret key for hashing
    const char key[] = "YOURPRIVATEKEYHERE";
    
    // The data that we're going to hash
    char data[] = message;
    
    // Be careful of the length of string with the choosen hash engine. SHA1 needed 20 characters.
    // Change the length accordingly with your choosen hash engine.
    unsigned char* result;
    unsigned int len = 64;
    
    result = (unsigned char*)malloc(sizeof(char) * len);
    
    HMAC_CTX ctx;
    HMAC_CTX_init(&ctx);
    
    HMAC_Init_ex(&ctx, key, strlen(key), EVP_sha256(), NULL);
    HMAC_Update(&ctx, (unsigned char*)&data, strlen(data));
    HMAC_Final(&ctx, result, &len);
    HMAC_CTX_cleanup(&ctx);
    
    printf("HMAC digest: ");
    
    for (int i = 0; i != len; i++)
        printf("%02x", (unsigned int)result[i]);
    
    printf("\n");
    
    free(result);
}
string getTime(){
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long long mslong = (long long) tp.tv_sec * 1000L + tp.tv_usec / 1000;
    auto time = to_string(mslong);
    return time;
}

void test(){
    http_client client(U("https://api.binance.com/"));
    client.request(methods::GET, "/api/v1/time").then([](http_response response)->
                                              pplx::task<json::value>{
                                                  if(response.status_code() == status_codes::OK){
                                                      return response.extract_json();
                                                  }
                                                  return pplx::task_from_result(json::value());
                                              })
    .then([](pplx::task<json::value> previousTask){
        try{
            json::value const & v = previousTask.get();
            print_test(v);
        }
        catch (http_exception const & e){
            wcout << e.what() << endl;
        }
    }).wait();
    
}
void print_test(json::value const & value){
    if(!value.is_null()){
        json::value test = value;
       cout << test["serverTime"] << endl << time;
    }
    
}
