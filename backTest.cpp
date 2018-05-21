#include "backTest.h"

void init();

auto main(int argc, char* argv[]) -> int{
    cout << endl << "Welcome to the C++ Binance Bot. WARNING --> This is still an extremly early alpha build. Use at your own risk." << endl << flush;
    // init();
    backTest.backTest();
    return 0;
}

void init(){
    string input;
    cout << endl << "Choose bot mode: 1 = trading" << endl;
    cin >> input;
    //(input == "1") ? priceData.tradingBot() : algoBot.algoBot(); //TEMPORARY
}

void serverData::setPair(){
    cout << "Select trading pair (ex: LTCBTC, TRXETH, BTCUSDT, etc): " << endl;
    cin >> pair;
}

void serverData::inputPrice(double foundPrice){
    price = foundPrice;
}

void serverData::setPrice(){
  http_client client(U(RESTfulHost)); //The base connection for the RESTful API
  string full_request = "/api/v3/ticker/price?symbol=" + pair; //The rest of the URL we are going to ping to the price
  client.request(methods::GET, full_request).then([](http_response response)-> //Do a RESTful GET request on the full URL for the price
                                                pplx::task<json::value>{ //Result will be in JSON
                                                    if(response.status_code() == status_codes::OK){ //If everything went okay, return the JSON (lambda function)
                                                        return response.extract_json();
                                                    }
                                                    return pplx::task_from_result(json::value());
                                                })
  .then([](pplx::task<json::value> previousTask){
    try{
        json::value const & v = previousTask.get(); //Take the JSON gathered by the GET request and store it into CPP JSON object
        printPrice(v); //pass that object to the print price
    }
    catch (http_exception const & e){ //This is just here to output an error if one occurs.
        wcout << e.what() << endl;
    }
 }).wait(); //Wait for all to finish
}

/* void botData::tradingBot(){
    string choice, amount;
    bot.setPair();
    bot.getPrice();
    cout << "Selected " << pair << ". Last known price of " << pair << ": " << price;

    cout << "Buying or selling? (1 for buying, 0 for selling)" << endl;
    cin >> choice;
    (choice == "0") ? choice = "SELL" : choice = "BUY";

    cout << "Enter the price you would like to buy/sell at." << endl;
    cin >> price;

    cout << "Enter amount you would like to buy/sell." << endl;
    cin >> amount;
    string curlTest = "curl -H \"X-MBX-APIKEY: ";
    // string command = "\" -X POST 'https://api.binance.com/api/v3/order?symbol="+pair+"&side="+choice+"&type=LIMIT&timeInForce=GTC&quantity="+amount+"&price="+price+"&recvWindow=5000&timestamp=";
    getTime();
    //string message = "symbol="+pair+"&side="+choice+"&type=LIMIT&timeInForce=GTC&quantity="+amount+"&price="+price+"&recvWindow=5000&timestamp="+epochTime;
    // HMACsha256(message, bot.secretKey);
    //curlTest = curlTest+APIKEY+command+time+"&signature="+signature+"'";
    cout << endl << endl << curlTest << endl << endl;
    // system(curlTest.c_str());


}*/

void serverData::HMACsha256(string const& message, string const& key) {
    unsigned char result[EVP_MAX_MD_SIZE];
    unsigned result_len = 0;
    HMAC(EVP_sha256(), key.data(), key.size(), reinterpret_cast<unsigned char const*>(message.data()), message.size(), result, &result_len);
    servData.setSignature(binary_to_hex(result, result_len));
}

void serverData::getTime(){
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long long mslong = (long long) tp.tv_sec * 1000L + tp.tv_usec / 1000;
    string time = to_string(mslong);
}

void backTesting::getTotal(){
    total=0;
    double test = 0;

    if(BTC>0){
    servData.inputPair("BTCUSDT");
    servData.inputPrice(0);
    servData.setPrice();
    test = BTC * servData.getPrice();
    total += test;
    }


    if(LTC>0){
    servData.inputPair("LTCUSDT");
    servData.inputPrice(0);
    servData.setPrice();
    test = LTC * servData.getPrice();
    total += test;
    }

    if(ETH>0){
    servData.inputPair("ETHUSDT");
    servData.inputPrice(0);
    servData.setPrice();
    test = ETH * servData.getPrice();
    total += test;
    }

    if(BCC>0){
    servData.inputPair("BCCUSDT");
    servData.inputPrice(0);
    servData.setPrice();
    test = BCC * servData.getPrice();
    total += test;
    }

    if(BNB>0){
    servData.inputPair("BNBUSDT");
    servData.inputPrice(0);
    servData.setPrice();
    test = BNB * servData.getPrice();
    total += test;
    }

    if(NEO>0){
    servData.inputPair("NEOUSDT");
    servData.inputPrice(0);
    servData.setPrice();
    test = NEO * servData.getPrice();
    total += test;
    }

    if(ADA>0){
    servData.inputPair("ADAUSDT");
    servData.inputPrice(0);
    servData.setPrice();
    test = ADA * servData.getPrice();
    total += test;
    }

    if(QTUM>0){
    servData.inputPair("QTUMUSDT");
    servData.inputPrice(0);
    servData.setPrice();
    test = QTUM * servData.getPrice();
    total += test;
    }

    double temp = total + USDT;
    string input = "\n\nTotal: " + to_string(temp);
    cout << endl <<  input << endl;
    ofstream account("backTest.txt", ofstream::out|ofstream::app);
    account << input;
    account.close();
}

void backTesting::simSell(){
    string append;
    servData.setPrice();
    string pair = servData.getPair();
    double price = servData.getPrice();
    if(pair == "BTCUSDT"){
        this->USDT = this->USDT + this->BTC * price;
        append = "Sold " + to_string(this->BTC) + "BTC for a total of: $" + to_string(BTC*price);
        this->BTC = 0;
    }
    else if(pair == "ETHUSDT"){
        this->USDT = this->USDT + this->ETH * price;
        append = "Sold " + to_string(this->ETH) + "ETH for a total of: $" + to_string(ETH*price);
        this->ETH = 0;
    }
    else if(pair == "BCCUSDT"){
        this->USDT = this->USDT + this->BCC * price;
        append = "Sold " + to_string(this->BCC) + "BCC for a total of: $" + to_string(BCC*price);
        this->BCC = 0;
    }
    else if(pair == "LTCUSDT"){
        this->USDT = this->USDT + this->LTC * price;
        append = "Sold " + to_string(this->LTC) + "LTC for a total of: $" + to_string(LTC*price);
        this->LTC = 0;
    }
    else if(pair == "BNBUSDT"){
        this->USDT = this->USDT + this->BNB * price;
        append = "Sold " + to_string(this->BNB) + "BNB for a total of: $" + to_string(BNB*price);
        this->BNB = 0;
    }
    else if(pair == "NEOUSDT"){
        this->USDT = this->USDT + this->NEO * price;
        append = "Sold " + to_string(this->NEO) + "NEO for a total of: $" + to_string(NEO*price);
        this->NEO = 0;
    }
    else if(pair == "ADAUSDT"){
        this->USDT = this->USDT + this->ADA * price;
        append = "Sold " + to_string(this->ADA) + "ADA for a total of: $" + to_string(ADA*price);
        this->ADA = 0;
    }
    else{
        this->USDT = this->USDT + this->QTUM * price;
        append = "Sold " + to_string(this->QTUM) + "QTUM for a total of: $" + to_string(QTUM*price);
        this->QTUM = 0;
    }

    string trade = "\n\nFound sell. Making sell on pair " + pair + " at price: $" + to_string(price) + ".\n" + append + "\n USDT remaining: " + to_string(USDT);
    cout << endl << endl << trade << endl << endl;

    ofstream account("backTest.txt", ofstream::out|ofstream::app);
    account << trade;
    account.close();
}

void backTesting::simBuy(){
    if(USDT > 0){
        string append;
        double buy;
        double purchaseAmt = USDT * 25 / 100;
        USDT = USDT - purchaseAmt;
        servData.setPrice();
        string pair = servData.getPair();
        double price = servData.getPrice();

        if(pair == "BTCUSDT"){
            buy = purchaseAmt/price;
            BTC = buy + BTC;
            append = "Purchased " + to_string(buy) + "BTC for a total of: $" + to_string(purchaseAmt);
        }
        else if(pair == "ETHUSDT"){
            buy = purchaseAmt/price;
            ETH = buy + ETH;
            append = "Purchased " + to_string(buy)+ "ETH for a total of: $" + to_string(purchaseAmt);
        }
        else if(pair == "BCCUSDT"){
            buy = purchaseAmt/price;
            BCC = buy + BCC;
            append = "Purchased " + to_string(buy) + "BCC for a total of: $" + to_string(purchaseAmt);
        }
        else if(pair == "LTCUSDT"){
            buy = purchaseAmt/price;
            LTC = buy + LTC;
            append = "Purchased " + to_string(buy) + "LTC for a total of: $" + to_string(purchaseAmt);
        }
        else if(pair == "BNBUSDT"){
            buy = purchaseAmt/price;
            BNB = buy + BNB;
            append = "Purchased " + to_string(buy) + "BNB for a total of: $" + to_string(purchaseAmt);
        }
        else if(pair == "NEOUSDT"){
            buy = purchaseAmt/price;
            NEO = buy + NEO;
            append = "Purchased " + to_string(buy) + "NEO for a total of: $" + to_string(purchaseAmt);
        }
        else if(pair == "ADAUSDT"){
            buy = purchaseAmt/price;
            ADA = buy + ADA;
            append = "Purchased " + to_string(buy) + "ADA for a total of: $" + to_string(purchaseAmt);
        }
        else{
            buy = purchaseAmt/price;
            QTUM = buy + QTUM;
            append = "Purchased " + to_string(buy) + "QTUM for a total of: $" + to_string(purchaseAmt);
        }

        string trade = "\n\nFound buy. Making buy on pair " + pair + " at price: $" + to_string(price) + ".\n" + append + "\n USDT remaining: " + to_string(USDT);;
        cout << endl << endl << trade << endl << endl;
        ofstream account("backTest.txt", ofstream::out|ofstream::app);
        account << trade;
        account.close();
    }
}

void backTesting::backTest(){
    USDT = 1000;
    double total=0;
    int count = 0;
    long double RSIcheck;
    while(1){
        servData.inputPair("BTCUSDT");
        algoBot.checkBuy();
        RSIcheck = TechnicalAnalysisCalcs.getRSIback();
        if(RSIcheck < 40.0 && BTC == 0)
            simBuy();

        servData.inputPair("LTCUSDT");
        algoBot.checkBuy();
        RSIcheck = TechnicalAnalysisCalcs.getRSIback();
        if(RSIcheck < 40.0 && LTC == 0)
            simBuy();

        servData.inputPair("ETHUSDT");
        algoBot.checkBuy();
        RSIcheck = TechnicalAnalysisCalcs.getRSIback();
        if(RSIcheck < 40.0 && ETH == 0)
            simBuy();

        servData.inputPair("BCCUSDT");
        algoBot.checkBuy();
        RSIcheck = TechnicalAnalysisCalcs.getRSIback();
        if(RSIcheck < 40.0 && BCC == 0)
            simBuy();

        servData.inputPair("BNBUSDT");
        algoBot.checkBuy();
        RSIcheck = TechnicalAnalysisCalcs.getRSIback();
        if(RSIcheck < 40.0 && BNB == 0)
            simBuy();

        servData.inputPair("NEOUSDT");
        algoBot.checkBuy();
        RSIcheck = TechnicalAnalysisCalcs.getRSIback();
        if(RSIcheck < 40.0 && NEO == 0)
            simBuy();

        servData.inputPair("ADAUSDT");
        algoBot.checkBuy();
        RSIcheck = TechnicalAnalysisCalcs.getRSIback();
        if(RSIcheck < 40.0 && ADA == 0)
            simBuy();

        servData.inputPair("QTUMUSDT");
        algoBot.checkBuy();
        RSIcheck = TechnicalAnalysisCalcs.getRSIback();
        if(RSIcheck < 40.0 && QTUM == 0)
            simBuy();
        //---------------//

        servData.inputPair("BTCUSDT");
        algoBot.checkBuy();
        RSIcheck = TechnicalAnalysisCalcs.getRSIback();
        if(RSIcheck > 60.0 && BTC > 0)
            simSell();

        servData.inputPair("LTCUSDT");
        algoBot.checkBuy();
        RSIcheck = TechnicalAnalysisCalcs.getRSIback();
        if(RSIcheck > 60.0 && LTC > 0)
            simSell();

        servData.inputPair("ETHUSDT");
        algoBot.checkBuy();
        RSIcheck = TechnicalAnalysisCalcs.getRSIback();
        if(RSIcheck > 60.0 && ETH > 0)
            simSell();

        servData.inputPair("BCCUSDT");
        algoBot.checkBuy();
        RSIcheck = TechnicalAnalysisCalcs.getRSIback();
        if(RSIcheck > 60.0 && BCC > 0)
            simSell();

        servData.inputPair("BNBUSDT");
        algoBot.checkBuy();
        RSIcheck = TechnicalAnalysisCalcs.getRSIback();
        if(RSIcheck > 60.0 && BNB > 0)
            simSell();

        servData.inputPair("NEOUSDT");
        algoBot.checkBuy();
        RSIcheck = TechnicalAnalysisCalcs.getRSIback();
        if(RSIcheck > 60.0 && NEO > 0)
            simSell();

        servData.inputPair("ADAUSDT");
        algoBot.checkBuy();
        RSIcheck = TechnicalAnalysisCalcs.getRSIback();
        if(RSIcheck > 60.0 && ADA > 0)
            simSell();

        servData.inputPair("QTUMUSDT");
        algoBot.checkBuy();
        RSIcheck = TechnicalAnalysisCalcs.getRSIback();
        if(RSIcheck > 60.0 && QTUM > 0)
            simSell();
        (count == 50) ? (getTotal(), count = 0) : (count++);
    }
}

void algorithmBot::setSellPercent(){
    cout << "Enter target gains percentage" << endl;
    cin >> sellPercent;
}

void algorithmBot::algoBot(){
    algoCheck = false;
    algoBuy = true;
    servData.setPair();
    // bot.setSellPercent();
    cout << "Gathering data on pair...This may take some time..." << endl;
    servData.setPrice();
    //cout << "PRICE: " << setprecision(8) << fixed << algoBot.getPrice() << endl;
    //pastPrice = servData.getPrice();
    //while(algoCheck == false)
    //    (algoBuy == true) ? checkBuy() : checkSell();
}

void algorithmBot::checkBuy(){
    priceData.getHistoricalPrices();
    TechnicalAnalysisCalcs.calcRSI();
    // algoBot.calcStochRSI();
}

/*void algorithmBot::checkSell(){
    double currentPrice = servData.getPrice();
    double difference;
    cout << endl << "PRICE: " << setprecision(8) << fixed << currentPrice << endl;
    if(price > pastPrice){
        difference = ((currentPrice - pastPrice)/price)*100;
        cout << endl << "DIFF: " << difference << endl;

        if(difference >= sellPercent){
            cout << "We should sell." << endl;
        }
    }
}*/

void TechnicalAnalysis::calcStochRSI(){
    vector<long double> highest, lowest;
    long double temp=0;
    int startingPoint = 0;


    for(int i = 0; i < RSI.size()-14; i++){
        vector<long double>::iterator lowIt=min_element(RSI.begin()+i,RSI.begin()+i+13);
        vector<long double>::iterator highIt=max_element(RSI.begin()+i,RSI.begin()+i+13);

        highest.push_back(*highIt);
        lowest.push_back(*lowIt);
        i++;
    }

    for(int i =0; i < highest.size(); i++){
        temp = (RSI[i+13]-lowest[i])/(highest[i]-lowest[i]);
        stochRSI.push_back(temp);
        temp=0;
    }

    cout << endl << "STOCH RSI: " << stochRSI.back() << endl;

}

void TechnicalAnalysis::calcRSI(){
    RSI.clear();
    vector<long double> gain, loss, change, avgGain, avgLoss, RS;
    vector<long double> currentPeriod(priceData.close.end()-250, priceData.close.end());
    double sumGain = 0, sumLoss = 0;

    for(int i = 1; i < currentPeriod.size(); i++)
        change.push_back(currentPeriod[i] - currentPeriod[i-1]);

    for(int i = 0; i < change.size(); i++){
        change[i] > 0 ? gain.push_back(change[i]) : gain.push_back(0);
        change[i] < 0 ? loss.push_back(abs(change[i])) : loss.push_back(0);
        sumGain += gain[i];
        sumLoss += loss[i];
    }

    avgGain.push_back(sumGain/14);
    avgLoss.push_back(sumLoss/14);

    for(int i = 14, j = 1; i < gain.size(); i++){
        avgGain.push_back(((avgGain[j-1] * 13)+ gain[i])/14);
        avgLoss.push_back(((avgLoss[j-1] * 13)+ loss[i])/14);
        j++;
    }

    for(int i = 0; i < avgGain.size(); i++){
        RS.push_back(avgGain[i]/avgLoss[i]);
        avgLoss[i] == 0 ? RSI.push_back(100) : RSI.push_back(100 - (100/(1+RS[i])));
    }
}

//Clear all vectors of old coin pair or old data to allow most up to date data.
void pricingData::clearVecs(){
    priceData.openTime.clear();
    priceData.open.clear();
    priceData.high.clear();
    priceData.low.clear();
    priceData.close.clear();
    priceData.volume.clear();
    priceData.closeTime.clear();
    priceData.quoteAssetVolume.clear();
    priceData.numTrades.clear();
    priceData.takerBuyAssetVol.clear();
    priceData.takerBuyQuoteAssetVol.clear();
    priceData.ignore.clear();
}

void pricingData::formatHistoricalPrices(json::value const &value){ //temp
    priceData.clearVecs();
    if(!value.is_null()){
        json::value historicalData = value;
        ofstream outputFile;
        outputFile.open("test.txt",ofstream::out | ofstream::trunc); //output to file
        outputFile << historicalData; //store JSON into file
        outputFile.close();
        ifstream inputFile("test.txt"); //input from file
        string historicalDataString="";
        string appendTemp;
        while(!inputFile.eof()){
            getline(inputFile,appendTemp); //store value from file into string
            historicalDataString.append(appendTemp);
        }
        boost::erase_first(historicalDataString, "["); //formatting for parsing
        historicalDataString.append("\"]"); //formatting for parsing
        int count = 0;
        string tempHolder;
        for (string::iterator it = historicalDataString.begin(); it < historicalDataString.end(); it++){
            if(*it != '[' && *it != '"' && *it !=']'){
                if(*it != ',')
                    tempHolder.append(1,*it);
                else{
                    switch (count){
                        case 0:
                            priceData.openTime.push_back(stof(tempHolder.c_str()));
                            break;
                        case 1:
                            priceData.open.push_back(stof(tempHolder.c_str()));
                            break;
                        case 2:
                            priceData.high.push_back(stof(tempHolder.c_str()));
                            break;
                        case 3:
                            priceData.low.push_back(stof(tempHolder.c_str()));
                            break;
                        case 4:
                            priceData.close.push_back(stof(tempHolder.c_str()));
                            break;
                        case 5:
                            priceData.volume.push_back(stof(tempHolder.c_str()));
                            break;
                        case 6:
                            priceData.closeTime.push_back(stof(tempHolder.c_str()));
                            break;
                        case 7:
                            priceData.quoteAssetVolume.push_back(stof(tempHolder.c_str()));
                            break;
                        case 8:
                            priceData.numTrades.push_back(stof(tempHolder.c_str()));
                            break;
                        case 9:
                            priceData.takerBuyAssetVol.push_back(stof(tempHolder.c_str()));
                            break;
                        case 10:
                            priceData.takerBuyQuoteAssetVol.push_back(stof(tempHolder.c_str()));
                            break;
                        case 11:
                            priceData.ignore.push_back(stof(tempHolder.c_str()));
                            break;
                        default:
                            break;
                    }//end of switch
                    (count == 11) ? count = 0 : count++;
                    tempHolder = "";
                }//end of else
            }//end of 2nd if
        } //end of for loop
    }//end of first if
}//End of function

void pricingData::getHistoricalPrices(){ //Get all price data since 1/1/2017 over an interval
    //https://api.binance.com/api/v1/klines?symbol=ETHBTC&interval=1h&startTime=1523059200
    http_client client(U(RESTfulHost));
    string interval, timestamp = "1483243199000";
    interval = "5m";
    string full_request = "/api/v1/klines?symbol="+servData.getPair()+"&interval="+interval;
    client.request(methods::GET, full_request).then([](http_response response)-> //Do a RESTful GET request on the full URL for the price
                                                    pplx::task<json::value>{ //Result will be in JSON
                                                        if(response.status_code() == status_codes::OK){ //If everything went okay, return the JSON (lambda function)
                                                            return response.extract_json();
                                                        }
                                                        return pplx::task_from_result(json::value());
                                                    })
    .then([](pplx::task<json::value> previousTask){
        try{
            json::value const & v = previousTask.get(); //Take the JSON gathered by the GET request and store it into CPP JSON object
            priceData.formatHistoricalPrices(v);
        }
        catch (http_exception const & e){ //This is just here to output an error if one occurs.
            wcout << e.what() << endl;
        }
    }).wait(); //Wait for all to finish
}

void printPrice(json::value const &value){ //temp
    if(!value.is_null()){
        json::value test = value;
        string v = test["price"].as_string();
        double price = stof(v.c_str());
        servData.inputPrice(price);
        // cout << test["price"] << endl << v << endl;
    }

}

auto binary_to_hex_digit(unsigned a) -> char{
    return a + (a < 10 ? '0' : 'a' - 10);
}

auto binary_to_hex(unsigned char const* binary, unsigned binary_len) -> string {
    string r(binary_len * 2, '\0');
    for(unsigned i = 0; i < binary_len; ++i) {
        r[i * 2] = binary_to_hex_digit(binary[i] >> 4);
        r[i * 2 + 1] = binary_to_hex_digit(binary[i] & 15);
    }
    return r;
}
