#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include "File.hpp"
#include "Date_calc.hpp"

using vector_st = std::vector<std::string>;



std::vector<vector_st> monitoring(const std::vector<vector_st>& log){
    //故障状態のサーバーアドレスとその故障期間(サーバーアドレス, 故障期間)
    std::vector<vector_st> monitor_;
    //故障中のサーバーのmap
    std::map<std::string, std::string> error_map;
    //ログの確認
    for(auto log_line : log){
        auto time = log_line[0];
        auto address = log_line[1];
        auto restime = log_line[2];
        auto itr = error_map.find(address);
        //サーバーの故障を発見する
        if(restime=="-"){
            //既に故障を発見していないかを確かめる
            if(itr == error_map.end()){
                error_map[address] = time;
            }
        }else{
            //サーバーの復帰を確かめる
            if(itr != error_map.end()){
                monitor_.push_back({address, error_map[address], add_ms(time, std::stoi(restime))});
                error_map.erase(address);
            }
        }
        
    }

    for(auto error : error_map){
        monitor_.push_back({error.second, error.first, ""});
    }

    return monitor_;
}

int  main(int argc, char* argv[]){
    if(argc != 2){
        std::cerr<<"Wrong command line\n";
        return -1;
    }
    File data_log(argv[1]);

    data_log.print();

    auto log = data_log.log;

    auto monitor = monitoring(log);

    std::cout<<"Server Address : start date ~ end date\n";
    for(auto monitor_line : monitor){
        std::cout<<monitor_line[0]<<" : "<<monitor_line[1]<<" ~ "<<monitor_line[2]<<"\n";
    }
    
}