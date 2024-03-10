#include<iostream>
#include <string>
#include <vector>
#include <map>
#include <bitset>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <fstream>
using namespace std;

struct datafile
{
    string name;
    string type;
    vector<long long> value;
    string value1;
};
struct seg
{
    string name;
    string position;
};

string lstrip(const string& s) {
    size_t start = s.find_first_not_of(" \t\n\v\f\r");
    return (start == string::npos) ? "" : s.substr(start);
}

string rstrip(const string& s) {
    size_t end = s.find_last_not_of(" \t\n\v\f\r");
    return (end == string::npos) ? "" : s.substr(0, end + 1);
}

string strip(const string& s) {
    return rstrip(lstrip(s));
}

vector<string> splitString(const string& input) {
    vector<string> tokens;
    string token = "";
    for (auto id : input) {
        if (id == ' ' or id == ',' or id == '\t') {
            if (token != "") {
                tokens.push_back(token);
            }
            token = "";
        }
        else {
            token += id;
        }
    }
    if (token != "") {
        tokens.push_back(token);
    }
    return tokens;
}

int string_to_int(string s) {
    if (s.size() >= 2 && s[0] == '0' && s[1] == 'x') {
        return stoi(s, nullptr, 16);
    }
    int x = 0;
    int sign = 1;

    if (s[0] == '-') {
        sign = -1;
        s = s.substr(1);
    }

    for (int i = 0; i < s.size(); i++) {
        x = x * 10 + (s[i] - '0');
    }

    return x * sign;
}

long long int bin_to_dec(bitset<32> bin) {
    long long int dec = 0;
    for (int i = 0; i < 32; i++) {
        dec += bin[i] * pow(2, i);
    }
    return dec;
}

string dec_to_hex_1(long long int dec) {
    if (dec < 0)
    {
        dec = 0xFFFFFFFF + dec + 1;
    }
    string hex = "";
    while (dec != 0) {
        int temp = 0;
        temp = dec % 16;
        if (temp < 10) {
            hex.push_back(temp + 48);
        }
        else {
            hex.push_back(temp + 55);
        }
        dec = dec / 16;
    }
    hex = hex + "x0" ;
    reverse(hex.begin(), hex.end());
    return hex;
}

string dec_to_hex(long long int dec) {
    if (dec < 0)
    {
        dec = 0xFFFFFFFF + dec + 1;
    }
    string hex = "";
    while (dec != 0) {
        int temp = dec % 16;
        if (temp < 10) {
            hex.push_back(temp + 48);
        }
        else {
            hex.push_back(temp + 55);
        }
        dec = dec / 16;
    }
    while (hex.size() < 8)
    {
        hex = hex + "0";
    }
    hex = hex + "x0";

    reverse(hex.begin(), hex.end());
    return hex;
}

map<string,vector<string>> encodes_map = {
    {"add", {"0110011", "000", "0000000", "R"}},   // add
    {"and", {"0110011", "111", "0000000", "R"}},   // and
    {"or",  {"0110011", "110", "0000000", "R"}},   // or
    {"sll", {"0110011", "001", "0000000", "R"}},   // sll
    {"slt", {"0110011", "010", "0000000", "R"}},   // slt
    {"sra", {"0110011", "101", "0100000", "R"}},   // sra
    {"srl", {"0110011", "101", "0000000", "R"}},   // srl
    {"sub", {"0110011", "000", "0100000", "R"}},   // sub
    {"xor", {"0110011", "100", "0000000", "R"}},   // xor
    {"mul", {"0110011", "000", "0000001", "R"}},   // mul
    {"div", {"0110011", "100", "0000001", "R"}},   // div
    {"rem", {"0110011", "110", "0000001", "R"}},   // rem
    {"addi", {"0010011", "000", "", "I"}},        // addi
    {"andi", {"0010011", "111", "", "I"}},        // andi
    {"ori",  {"0010011", "110", "", "I"}},        // ori
    {"lb",   {"0000011", "000", "", "L"}},        // lb
    {"ld",   {"0000011", "011", "", "L"}},        // ld
    {"lh",   {"0000011", "001", "", "L"}},        // lh
    {"lw",   {"0000011", "010", "", "L"}},        // lw
    {"jalr", {"1100111", "000", "", "I"}},       // jalr
    {"sb",   {"0100011", "000", "", "S"}},       // sb
    {"sw",   {"0100011", "010", "", "S"}},       // sw
    {"sd",   {"0100011", "011", "", "S"}},       // sd
    {"sh",   {"0100011", "001", "", "S"}},       // sh
    {"beq",  {"1100011", "000", "", "B"}},      // beq
    {"bne",  {"1100011", "001", "", "B"}},      // bne
    {"bge",  {"1100011", "101", "", "B"}},      // bge
    {"blt",  {"1100011", "100", "", "B"}},      // blt
    {"auipc", {"0010111", "000", "", "U"}},     // auipc
    {"lui",   {"0110111", "000", "", "U"}},     // lui
    {"jal",   {"1101111", "000", "", "J"}}     // jal
};

int reg(string register_name)
{
   char first_char = register_name[0];
   if(first_char == 'x')
    {
        return string_to_int(register_name.substr(1));
    }
    else if(first_char == 'z')
    {
        return 0;
    }
    else if(first_char == 'r')
    {
        return 1 ;
    }
    else if(register_name == "sp")
    {
        return 2;
    }
    else if (register_name == "s0" || register_name == "fp")
    {
        return 8;
    }
    else  if (first_char == 's')
    {
        return 16 + string_to_int(register_name.substr(1));
    }
    else if(first_char == 'g')
    {
        return 3;
    }
    else if(register_name == "tp")
    {
        return 4;
    }
    else if(first_char == 't')
    {
        int a = string_to_int(register_name.substr(1));
        if(a < 3)
        {
            return string_to_int(register_name.substr(1)) + 5;
        }
        else
        {
            return string_to_int(register_name.substr(1)) + 25;
        }
    }
    else
    {
        return 10 + string_to_int(register_name.substr(1));
    }
}

string encode_in_r(vector<string> tokens) {
    bitset<32> machine_code ;
    bitset<32> opcode(encodes_map[tokens[0]][0]);
    bitset<32> rd(reg(tokens[1]));
    rd = rd << 7;
    bitset<32> funct3(encodes_map[tokens[0]][1]);
    funct3 = funct3 << 12;
    bitset<32> rs1(reg(tokens[2]));
    rs1 = rs1 << 15;
    bitset<32> rs2(reg(tokens[3]));
    rs2 = rs2 << 20;
    bitset<32> funct7(encodes_map[tokens[0]][2]);
    funct7 = funct7 << 25;
    machine_code = (opcode | rd | funct3 | rs1 | rs2 | funct7);
    return dec_to_hex(bin_to_dec(machine_code));
}

string encode_in_i(vector<string> tokens)
{
    bitset<32> machine_code ;
    bitset<32> opcode(encodes_map[tokens[0]][0]);
    bitset<32> rd(reg(tokens[1]));
    rd = rd << 7;
    bitset<32> funct3(encodes_map[tokens[0]][1]);
    funct3 = funct3 << 12;
    bitset<32> rs1(reg(tokens[2]));
    rs1 = rs1 << 15;
    bitset<32> imm(string_to_int(tokens[3]));
    imm = imm << 20;

    machine_code = (opcode | rd | funct3 | rs1 | imm);
    return dec_to_hex(bin_to_dec(machine_code));
}

string encode_in_s(vector<string> tokens)
{
    bitset<32> machine_code ;
    bitset<32> opcode(encodes_map[tokens[0]][0]);
    bitset<32> rs2(reg(tokens[1]));
    rs2 = rs2 << 20;
    bitset<32> funct3(encodes_map[tokens[0]][1]);
    funct3 = funct3 << 12;
    int index = tokens[2].find('(');
    string imm_str = tokens[2].substr(0, index);
    bitset<32> imm(string_to_int(imm_str));
    bitset<32> rs1(reg(tokens[2].substr(index + 1, tokens[2].size() - index - 2)));
    rs1 = rs1 << 15;
    bitset<32> last_5_bits = imm &  bitset<32>(0x1F);
    last_5_bits = last_5_bits << 7;
    imm = imm << 20;
    bitset<32> first_7_bits = imm &  bitset<32>(0xFE000000);
    machine_code = (opcode | rs2 | funct3 | rs1 | last_5_bits | first_7_bits);
    return dec_to_hex(bin_to_dec(machine_code));
}

string encode_in_lw(vector<string> tokens, int pc )
{
    bitset<32> machine_code ;
    bitset<32> opcode(encodes_map[tokens[0]][0]);
    bitset<32> rd(reg(tokens[1]));
    rd = rd << 7;
    bitset<32> funct3(encodes_map[tokens[0]][1]);
    funct3 = funct3 << 12;
    int index = tokens[2].find('(');
    string imm_str = tokens[2].substr(0, index);
    bitset<32> imm(string_to_int(imm_str));
    imm = imm << 20;
    bitset<32> rs1(reg(tokens[2].substr(index + 1, tokens[2].size() - index - 2)));
    rs1 = rs1 << 15;
    machine_code = (opcode | rd | funct3 | rs1 | imm);
    return dec_to_hex(bin_to_dec(machine_code));
}

string encode_in_sb(vector<string> tokens , int pc)
{
    bitset<32> machine_code ;
    bitset<32> opcode(encodes_map[tokens[0]][0]);
    bitset<32> rs1(reg(tokens[1]));
    rs1 = rs1 << 15;
    bitset<32> rs2(reg(tokens[2]));
    rs2 = rs2 << 20;
    bitset<32> funct3(encodes_map[tokens[0]][1]);
    funct3 = funct3 << 12;
    long long int a = string_to_int(tokens[3]);
    long long int b = pc + 4;
    long long int offset = a - b;
    offset = offset >> 1;
    bitset<32> imm((offset & 0x1E) | ((offset & 0x1000) >> 4) | ((offset & 0x7E0) << 20) | ((offset & 0x800) << 19));
    imm = imm << 25;
    bitset<32> last_7_bits = imm &  bitset<32>(0xFE000000);
    last_7_bits = last_7_bits >> 25;
    bitset<32> first_5_bits = imm &  bitset<32>(0x00000F80);
    first_5_bits = first_5_bits >> 7;
    machine_code = (opcode | rs1 | rs2 | funct3 | first_5_bits | last_7_bits);
    return dec_to_hex(bin_to_dec(machine_code));
}

string encode_in_u(vector<string> tokens)
{
    bitset<32> machine_code ;
    bitset<32> opcode(encodes_map[tokens[0]][0]);
    bitset<32> rd(reg(tokens[1]));
    rd = rd << 7;
    bitset<32> imm(string_to_int(tokens[2]));
    imm = imm << 12;
    machine_code = (opcode | rd | imm);
    return dec_to_hex(bin_to_dec(machine_code));
}

string encode_in_uj(vector<string> tokens , int pc)
{
    bitset<32> machine_code ;
    bitset<32> opcode(encodes_map[tokens[0]][0]);
    bitset<32> rd(reg(tokens[1]));
    rd = rd << 7;
    long long int a = string_to_int(tokens[2]);
    long long int b = pc + 4;
    long long int offset = a - b;
    offset = offset >> 1;
    bitset<32> imm((offset & 0x3FF) | ((offset & 0x400) >> 10) | ((offset & 0x7F800) << 11) | ((offset & 0x80000) << 20));
    imm = imm << 12;
    machine_code = (opcode | rd | imm);
    return dec_to_hex(bin_to_dec(machine_code));
}

int main()
{
    ifstream inFile;
    inFile.open("input.txt");
    ofstream outFile;
    outFile.open("output.mc");
    vector<datafile> ins;
    string line;
    while(getline(inFile, line))
    {
        stringstream ss(line);
        datafile temp;
        ss >> temp.name;
        if(temp.name[0] == '#' || temp.name == "") continue;
        ss >> temp.type;
        if(temp.type == "data")
        {
            string temp2;
            ss >> temp2;
            temp2 = strip(temp2);
            temp.value.push_back(string_to_int(temp2));
        }
        else
        {
            string temp2;
            ss >> temp2;
            temp2 = strip(temp2);
            temp.value1 = temp2;
            while(ss >> temp2)
            {
                temp2 = strip(temp2);
                temp.value.push_back(string_to_int(temp2));
            }
        }
        ins.push_back(temp);
    }
    int pc = 0;
    for(auto i : ins)
    {
        if(i.type == "data")
        {
            outFile << i.name << " ";
            for(auto j : i.value)
            {
                outFile << dec_to_hex(j) << " ";
            }
            outFile << endl;
        }
        else
        {
            vector<string> tokens = splitString(i.value1);
            if(encodes_map[tokens[0]][3] == "R")
            {
                outFile << dec_to_hex(pc) << " " << encode_in_r(tokens) << endl;
            }
            else if(encodes_map[tokens[0]][3] == "I")
            {
                outFile << dec_to_hex(pc) << " " << encode_in_i(tokens) << endl;
            }
            else if(encodes_map[tokens[0]][3] == "S")
            {
                outFile << dec_to_hex(pc) << " " << encode_in_s(tokens) << endl;
            }
            else if(encodes_map[tokens[0]][3] == "B")
            {
                outFile << dec_to_hex(pc) << " " << encode_in_sb(tokens, pc) << endl;
            }
            else if(encodes_map[tokens[0]][3] == "U")
            {
                outFile << dec_to_hex(pc) << " " << encode_in_u(tokens) << endl;
            }
            else if(encodes_map[tokens[0]][3] == "J")
            {
                outFile << dec_to_hex(pc) << " " << encode_in_uj(tokens, pc) << endl;
            }
            pc = pc + 4;
        }
    }
    return 0;
}
