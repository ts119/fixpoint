#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <utility>
#include "File.hpp"
#include "Date_calc.hpp"

using vector_st = std::vector<std::string>;

unsigned int pow_2(int n){
    if(n==0){
        return 1;
    }
    return 2*pow_2(n-1);
}

std::vector<unsigned int> bit_gen(int prefix){
    int n = prefix/8;
    int a = prefix%8;

    std::vector<unsigned int> ans(4, 0);

    int i = 0;
    for(;i<n;i++){
        ans[i] = pow_2(8)-1;
    }
    if(a==0){
        ans[i] = pow_2(a)-1;
    }

    return ans;
    
}

std::pair<std::string, int> split_address(std::string row_address/*,std::map<std::string, std::pair<int, std::string>> subnet_map*/ ){
    std::stringstream row_address_stream(row_address);
    std::string element;
    std::vector<std::string> address_string;
    std::vector<int> address_v;
    while(std::getline(row_address_stream, element, '/')){
        address_string.push_back(element);
    }

    int prefix = std::stoi(address_string[1]);
    std::stringstream row_server_address(address_string[0]);

    while(std::getline(row_server_address, element, '.')){
        address_v.push_back(std::stoi(element));
    }
    
    auto mask = bit_gen(prefix);

    for(int i=0;i<4;i++){
        address_v[i] = address_v[i] & mask[i];
        
    }
    std::string server_address;

    server_address = std::to_string(address_v[0]) + "." 
                   + std::to_string(address_v[1]) + "." 
                   + std::to_string(address_v[2]) + "." 
                   + std::to_string(address_v[3]);

    return {server_address, prefix};

}


std::vector<vector_st> monitoring(const std::vector<vector_st>& log, const int N){
    //故障状態のサーバーアドレスとその故障期間(サーバーアドレス, 故障期間)
    std::vector<vector_st> monitor_;
    //故障中のサーバーのmap
    //pair→{タイムアウト回数, エラー時間}
    std::map<std::string, std::pair<int, std::string>> error_map;

    //各サブネットごとのサーバー名, 監視時間, restimeを把握
    std::string subnet_s = "-1";
    std::string subnet_start_time;
    int subnet_restime = 10000;

    bool flag_subnet_error = 0;
    //ログの確認
    for(auto log_line : log){
        auto time = log_line[0];
        auto address = log_line[1];
        auto restime = log_line[2];

        bool flag1 = false;

        auto address_pair = split_address(address);

        //監視するサブネットが変わったかどうかを確認
        if(subnet_s == "-1"){
            subnet_s = address_pair.first;
            flag_subnet_error = 1;
        }else if(subnet_s != address_pair.first){
            auto itr = error_map.find(subnet_s);
            if(flag_subnet_error){
                flag1 = true;
                if(itr == error_map.end()){
                    error_map[subnet_s].first = 1;
                    error_map[subnet_s].second = subnet_start_time;
                }else{
                    error_map[subnet_s].first += 1;
                }
            }else{
                if(itr != error_map.end() && error_map[subnet_s].first>=N){
                monitor_.push_back({subnet_s, error_map[subnet_s].second, add_ms(time, subnet_restime)});
                error_map.erase(subnet_s);
                }
            }
            subnet_s = address_pair.first;
            subnet_start_time = time;
            subnet_restime = 10000;
            flag_subnet_error = 1;
        }
        //サーバーの故障を発見する
        if(restime=="-"){
            //サブネット内の他のサーバが故障していたかを確認する
            flag_subnet_error = 1*flag_subnet_error;

        //サーバーの復帰を確かめる
        }else{ 
            flag_subnet_error = 0;
            int restime_int = std::stoi(restime);
            if(subnet_restime>restime_int){
                subnet_restime = restime_int;
            }
        }
            
    }

    auto itr = error_map.find(subnet_s);
    if(flag_subnet_error){
        if(itr == error_map.end()){
            error_map[subnet_s].first = 1;
            error_map[subnet_s].second = subnet_start_time;
        }else{
            error_map[subnet_s].first += 1;
        }
    }else{
        if(itr != error_map.end() && error_map[subnet_s].first>=N){
            monitor_.push_back({subnet_s, error_map[subnet_s].second, add_ms(subnet_start_time, subnet_restime)});
            error_map.erase(subnet_s);
        }
    }
    
    //サブネットの故障が継続中のもの
    for(auto error : error_map){
        auto address = error.first;
        auto error_time = error.second.first;
        auto start_time = error.second.second;
        if(error_time>=N){
            monitor_.push_back({address,  start_time, "now"});
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