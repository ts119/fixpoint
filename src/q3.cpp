#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <tuple>
#include <sstream>
#include "File.hpp"
#include "Date_calc.hpp"

using vector_st = std::vector<std::string>;


//tuple→{過負荷フラグ,過負荷の発生時間, 直近のpingキュー, 直近のm個の合計ping}
using tuple = std::tuple<bool, std::string, std::queue<int>, int>;


std::vector<vector_st> monitoring(const std::vector<vector_st>& log, const int m, const int t){
    //過負荷状態のサーバーアドレスとその過負荷期間 {サーバーアドレス, 過負荷期間}
    std::vector<vector_st> monitor_;
    //監視対象のサーバーのマップ
    std::map<std::string, tuple> over_map;

    //ログの確認
    for(auto log_line : log){
        auto time = log_line[0];
        auto address = log_line[1];
        auto restime = log_line[2];
        auto itr = over_map.find(address);

        //サーバーの応答なしを発見する
        if(restime=="-"){
            //10000ms待って応答がなかった場合を応答なしとした
            restime = "10000";
        }
        int restime_int = std::stoi(restime);
        if(itr == over_map.end()){
            auto& over_tuple = over_map[address];
            auto& que = std::get<2>(over_tuple);
            std::get<0>(over_tuple) = false;
            if(!que.empty()){
                que.pop();
            }
            que.push(restime_int);
            std::get<3>(over_tuple) = restime_int;
        }else{
            auto& over_tuple = over_map[address];
            auto& flag = std::get<0>(over_tuple);
            auto& start_time = std::get<1>(over_tuple);
            auto& que = std::get<2>(over_tuple);
            auto& sum_m = std::get<3>(over_tuple);
            que.push(restime_int);
            sum_m += restime_int;

            if(que.size()>m){
                sum_m -= que.front();
                que.pop();
            }
        }
        auto& over_tuple = over_map[address];
        auto& flag = std::get<0>(over_tuple);
        auto& start_time = std::get<1>(over_tuple);
        auto& que = std::get<2>(over_tuple);
        auto& sum_m = std::get<3>(over_tuple);
        //サーバーの復帰を確かめる
        if(flag==true){
            if(sum_m<t*m){
                monitor_.push_back({address, start_time, add_ms(time, std::stoi(restime))});
                flag = false;
            }
        }else{
            //過負荷状態になったかを確かめる
            if(que.size()==m && sum_m >= t*m){
                start_time = time;
                flag = true;
            }
        }
    }
    //過負荷状態継続中のもの
    for(auto one: over_map){
        auto flag = std::get<0>(one.second);
        if(flag){
            auto& address = one.first;
            auto& start_time = std::get<1>(one.second);
            monitor_.push_back({address, start_time, "now"});
        }
    }
    return monitor_;
}

int  main(int argc, char* argv[]){
    if(argc != 4){
        std::cerr<<"Wrong command line\n";
        return -1;
    }

    File data_log(argv[1]);
    int m = std::stoi(argv[2]);
    int t = std::stoi(argv[3]);

    //データログを出力
    //data_log.print();

    auto log = data_log.log;

    auto monitor = monitoring(log,m,t);

    std::cout<<"Server Address : start date ~ end date\n";
    for(auto monitor_line : monitor){
        std::cout<<monitor_line[0]<<" : "<<monitor_line[1]<<" ~ "<<monitor_line[2]<<"\n";
    }
    
}