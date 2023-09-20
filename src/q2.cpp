#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <utility>
#include "File.hpp"
#include "Date_calc.hpp"

using vector_st = std::vector<std::string>;



std::vector<vector_st> monitoring(const std::vector<vector_st>& log, const int N){
    //故障状態のサーバーアドレスとその故障期間(サーバーアドレス, 故障期間)
    std::vector<vector_st> monitor_;
    //故障中のサーバーのmap
    //pair→{タイムアウト回数, エラー時間}
    std::map<std::string, std::pair<int, std::string>> error_map;
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
                error_map[address].first = 1;
                error_map[address].second = time;
            }else{
                error_map[address].first += 1;
            }
        //サーバーの復帰を確かめる
        }else if(itr != error_map.end() && error_map[address].first>=N){
            monitor_.push_back({address, error_map[address].second, add_ms(time, std::stoi(restime))});
            error_map.erase(address);
        }
    }

    //サブネットの故障が継続中のもの
    for(auto error : error_map){
        auto address = error.first;
        auto error_time = error.second.first;
        auto start_time = error.second.second;
        if(error_time>=N){
            monitor_.push_back({address, start_time, "now"});
        }
    }

    return monitor_;
}

int  main(int argc, char* argv[]){
    if(argc != 3){
        std::cout<<argc<<"\n";
        std::cerr<<"Wrong command line\n";
        return -1;
    }
    File data_log(argv[1]);
    int N = std::stoi(argv[2]);

    //データログを出力
    //data_log.print();

    auto log = data_log.log;

    auto monitor = monitoring(log, N);

    std::cout<<"Server Address : start date ~ end date\n";
    for(auto monitor_line : monitor){
        std::cout<<monitor_line[0]<<" : "<<monitor_line[1]<<" ~ "<<monitor_line[2]<<"\n";
    }
    
}