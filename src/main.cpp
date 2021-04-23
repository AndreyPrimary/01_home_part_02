#include "lib.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <iterator>
#include <algorithm>


using file_text = std::vector<std::string>;
using ip_address = std::string;
using ip_key = uint64_t;
using ip_vector = std::vector<uint8_t>;
using ip_list = std::multimap<ip_key, ip_address>;

template<char delimiter>
class StringDelimiter : public std::string
{};

std::vector<std::string> splitLine(const std::string& text)
{
    std::istringstream iss(text);

    std::vector<std::string> results((std::istream_iterator<StringDelimiter<'\t'>>(iss)),
        std::istream_iterator<StringDelimiter<'\t'>>());

    return results;
}

std::vector<std::string> splitIp(const ip_address& ip)
{
    std::string ip_val;
    std::istringstream iss(ip);

    std::vector<std::string> result;

    while (std::getline(iss, ip_val, '.')) {
        result.push_back(ip_val);
    }

    return result;
}

ip_address toIpAddress(const std::string &line)
{
    auto v = splitLine(line);
    ip_address ip = v.at(0);

    return ip;
}

ip_vector getIpVector(const ip_address &ip)
{
    ip_vector result;

    auto v = splitIp(ip);

    for (const auto &ips : v) {
        result.push_back(std::stoi( ips ));
    }

    return result;
}

ip_key getIpKey(const ip_address &ip)
{
    auto v = getIpVector(ip);
        
    // std::cout << "Before key v.size = " << v.size()
    //     << " v.at(0) :" << v.at(0)
    //     << " ip :" << ip 
    //     << std::endl;
    ip_key key = v.at(0) * 0x1000000 + v.at(1) * 0x10000 
        + v.at(2) * 0x100 + v.at(3); 
    // std::cout << "ip: " << ip
    //     << " key = " << key
    //     << std::endl;

    return key;
}

ip_list getIpList (const file_text &text)
{
    ip_list result;

    for (const auto &line : text) {
        // std::cout << line << std::endl;

        auto ip = toIpAddress(line);
        // DEBUG >>>
        // std::cout << "ip: " << ip << std::endl;
        // <<< DEBUG
        auto key = getIpKey(ip);
        // DEBUG >>>
        // std::cout << "key: " << key << std::endl;
        // <<< DEBUG

        // result[key] = ip;
        result.insert(std::pair <ip_key, ip_address>(key, ip));
    }

    return result;
}

void showIpList (const ip_list &list)
{
    // All list
    for (auto it = list.rbegin(); it != list.rend(); it++) {
        std::cout << it->second << std::endl;   
    }

    // std::cout << "---" << std::endl;

    // first 1
    for (auto it = list.rbegin(); it != list.rend(); it++) {
        
        auto v = getIpVector(it->second);

        if (v.at(0) == 1) {
            std::cout << it->second << std::endl;
        }
    }

    // std::cout << "---" << std::endl;

    // first 46 and second 70
    for (auto it = list.rbegin(); it != list.rend(); it++) {
        
        auto v = getIpVector(it->second);

        if ( (v.at(0) == 46) && (v.at(1) == 70) ) {
            std::cout << it->second << std::endl;
        }
    }

    // std::cout << "---" << std::endl;

    // any 46
    for (auto it = list.rbegin(); it != list.rend(); it++) {
        
        auto v = getIpVector(it->second);

        if ( std::find(v.begin(), v.end(), 46) != v.end() ) {
            std::cout << it->second << std::endl;
        }
    }

}

bool readFile(const std::string &filename, file_text &text)
{
    try {
        std::string line;
        std::ifstream myfile (filename.c_str());

        if (!myfile.is_open()) {
            std::cout << "Unable to open file: " << filename << std::endl;

            return false;
        }

        while ( getline (myfile,line) ) {
            text.push_back(line);
        }

        myfile.close();
    }
    catch(const std::exception &e) {
        std::cout << "Error read file: " << filename << std::endl;

        return false;
    }

    return true;
}

bool readCin(file_text &text)
{
    try {
        for(std::string line; std::getline(std::cin, line);)
        {
            text.push_back(line);
        }
    }
    catch(const std::exception &e) {
        std::cout << "Error read from std::cin: " << std::endl;

        return false;
    }

    return true;
}

void showFile(file_text &text)
{
    std::cout << "---" << std::endl;

    for (const auto &line : text) {
        std::cout << line << std::endl;
    }

    std::cout << "---" << std::endl;
}

int main (int, char **)
{
    // std::cout << "ip_filter Version: " << version() << std::endl;

    file_text text;

    // if (!readFile("ip_filter.tsv", text)) {
    // // if (!readFile("ip_test.tsv", text)) {
    //     return 1;
    // }

    // showFile(text);

    if (!readCin(text) ) {
        return 1;
    }

    try {
        auto list = getIpList (text);
        showIpList (list);
    }
    catch(const std::exception &e) {
        std::cout << "Error parsing ip list: " << e.what() << std::endl;

        return 2;
    }
    return 0;
}
