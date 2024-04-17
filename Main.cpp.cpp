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
vector<int> registers(32, 0);
struct datafile
{
    string name;
    string type;
    vector <long long> value;
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

// Function to remove trailing whitespace characters
string rstrip(const string& s) {
    size_t end = s.find_last_not_of(" \t\n\v\f\r");
    return (end == string::npos) ? "" : s.substr(0, end + 1);
}

// Function to remove leading and trailing whitespace characters
string strip(const string& s) {
    return rstrip(lstrip(s));
}



vector<string> splitString(const string& input) {
    vector<string> tokens;
    string token = "" ;
    for(auto id : input) 
    {   
        if(id == ' ' or id == ',' or id == '\t') 
        {
            if(token != "") 
            {
                
                tokens.push_back(token);
                
            }
            token = "" ;
        }
        else
        { 
          token += id ;
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

    for(int i = 0; i < s.size(); i++) {
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
        } else {
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
        } else {
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


std::map<string,vector<std::string>> encodes_map = {
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
    else if (register_name == "s0" or register_name == "fp")
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
    {   int a = string_to_int(register_name.substr(1));
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
    if(tokens[0] == "add") registers[reg(tokens[1])] = registers[reg(tokens[2])] + registers[reg(tokens[3])];
    else if (tokens[0] == "and") registers[reg(tokens[1])] = registers[reg(tokens[2])] + registers[reg(tokens[3])];
    else if (tokens[0] == "or") registers[reg(tokens[1])] = registers[reg(tokens[2])] | registers[reg(tokens[3])];
    else if( tokens[0] == "sll") registers[reg(tokens[1])] = registers[reg(tokens[2])] << registers[reg(tokens[3])];
    else if( tokens[0] == "slt") 
        {registers[reg(tokens[1])] = registers[reg(tokens[2])] + registers[reg(tokens[3])];
        if(registers[reg(tokens[2])] < registers[reg(tokens[3])])
        {
            registers[reg(tokens[1])] = 1;
        }
        else
        {
            registers[reg(tokens[1])] = 0;
        }
        }
    else if( tokens[0] == "sra") registers[reg(tokens[1])] = registers[reg(tokens[2])] >> registers[reg(tokens[3])];
    else if( tokens[0] == "srl") registers[reg(tokens[1])] = registers[reg(tokens[2])] >> registers[reg(tokens[3])];
    else if( tokens[0] == "sub") registers[reg(tokens[1])] = registers[reg(tokens[2])] - registers[reg(tokens[3])];
    else if( tokens[0] == "xor") registers[reg(tokens[1])] = registers[reg(tokens[2])] ^ registers[reg(tokens[3])];
    else if( tokens[0] == "mul") registers[reg(tokens[1])] = registers[reg(tokens[2])] * registers[reg(tokens[3])];
    else if( tokens[0] == "div") registers[reg(tokens[1])] = registers[reg(tokens[2])] / registers[reg(tokens[3])];
    else if( tokens[0] == "rem") registers[reg(tokens[1])] = registers[reg(tokens[2])] % registers[reg(tokens[3])];
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
    return dec_to_hex(bin_to_dec(machine_code)) ; // Added return statement
}

string encode_in_i(vector<string> tokens) 
{   
    if (tokens[0] == "addi") registers[reg(tokens[1])] = registers[reg(tokens[2])] + string_to_int(tokens[3]);
    else if (tokens[0] == "andi") registers[reg(tokens[1])] = registers[reg(tokens[2])] & string_to_int(tokens[3]);
    else registers[reg(tokens[1])] = registers[reg(tokens[2])] | string_to_int(tokens[3]);
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
    return dec_to_hex(bin_to_dec(machine_code)) ; // Added return statement
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
    return dec_to_hex(bin_to_dec(machine_code)) ; // Added return statement
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
    // cout << opcode << " " << rd << " " << funct3 << " " << rs1 << " " << imm << endl;
    // cout << machine_code << endl;
    
    return dec_to_hex(bin_to_dec(machine_code)) ; // Added return statement
}


map<string, int> labels;
int pc1 = 0x0;
void processLabels(const string& line) {
    if (line.find(':') != string::npos  and line.find('.') == string::npos){
        string label = strip(line.substr(0, line.find(':')));
        labels[label] = pc1;
        if (splitString(line).size() > 2){
            pc1 += 4 ;
        }
    }
    else if (line.find('.') == string::npos){
        pc1 += 4;
    }
}
void loadLabels(const string& filename) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        else
        {
            processLabels(line);
        }
    }
    file.close();
}

string encode_in_sb(vector<string> tokens, int pc)
{
    bitset<32> machine_code ;
    bitset<32> opcode(encodes_map[tokens[0]][0]);
    bitset<32> rs1(reg(tokens[1]));
    rs1 = rs1 << 15;
    bitset<32> rs2(reg(tokens[2]));
    rs2 = rs2 << 20;
    bitset<32> func3(encodes_map[tokens[0]][1]);
    func3 = func3 << 12;
    int imm  = labels[tokens[3]] - pc;
    // cout << labels[tokens[3]] << endl ;
    // cout << imm <<endl;
    bitset<32> imm_11(imm & 0x400);
    bitset<32> imm_12(imm & 0x800);
    bitset<32> imm_10to5(imm & 0x7E0);
    bitset<32> imm_4to1(imm & 0x1E);

    bitset<32> imm_final = (imm_12 << 19) | (imm_10to5 <<20) | (imm_4to1 << 7) | (imm_11 >> 4);

    machine_code = opcode | rs1 | rs2 | func3 | imm_final;
    // cout << machine_code << endl;
    return dec_to_hex(bin_to_dec(machine_code)) ;
}

string encode_in_u(vector<string> tokens) {
    bitset<32> machine_code ;
    bitset<32> opcode(encodes_map[tokens[0]][0]);
    bitset<32> rd(reg(tokens[1]));
    rd = rd << 7;
    bitset<32> imm(string_to_int(tokens[2]));
    imm = imm << 12;
    machine_code = (opcode | rd | imm);
    return dec_to_hex(bin_to_dec(machine_code)) ; // Added return statement

}

string encode_in_uj(vector<string> tokens, int pc) 
{
    bitset<32> machine_code ;
    bitset<32> opcode(encodes_map[tokens[0]][0]);
    bitset<32> rd(reg(tokens[1]));
    rd = rd << 7;
    int imm = labels[tokens[2]] - pc ;
    // cout << imm << endl;
    // cout << labels[tokens[2]] << endl;
    bitset<32> bit_20(imm >> 1 & 0x80000);
    bitset<32> bit_10to1(imm << 8 & 0x7FE00);
    bitset<32> bit_11(imm >> 2 & 0x100);
    bitset<32> bit_19to12(imm >> 12 & 0xFF);
    bitset<32> imm_final = (bit_20 | bit_19to12 | bit_11 | bit_10to1);
    machine_code = opcode | rd | imm_final << 12;
    return dec_to_hex(bin_to_dec(machine_code)) ;
}
string inc_hex(string address, int bytes) {
    unsigned long long int_value;
    stringstream ss;
    ss << hex << address;
    ss >> int_value;
    int_value += bytes;
    stringstream result;
    result << hex << int_value;
    return result.str();
}

typedef pair<string,string> MemoryPair;
MemoryPair datamemory; // pair of data memory with its size in bytes
vector<MemoryPair> datamemoryList; // vector of data memory pairs with their addresses and values
vector<seg> datalabels; // vector of data labels with its position in memory
const int START =0;


void read_data() {
    ifstream file("assemblycode.asm");
    string line;
    vector<datafile> stored;
    int start = 0;

    if (file.is_open()) {
        while (getline(file, line)) {
            vector<string> tokens = splitString(line);

            if (line.empty()) 
            {
                continue;
            }
            if (start > 1) 
            {
                continue;
            }
            else if (start == 1) 
            {
                if (tokens[0] == ".text:" || tokens[0] == ".text") 
                { // Data part ends
                    start = 2;
                }
                else 
                {
                    datafile temp;
                    int flag = 0;
                    int index = -1;

                    for (int i = 0; i < tokens.size(); i++) 
                    {
                        if (tokens[i].find(":") != string::npos && tokens[i+1].find(".") != string::npos)
                        {
                            flag = 1;
                            index = i;
                            break;
                        }
                    }

                    if (flag == 1) 
                    {
                        string nameT = tokens[index].substr(0, tokens[index].size() - 1);
                        string typeT = tokens[index + 1].substr(1);
                        temp.name = nameT;
                        temp.type = typeT;
                        if (typeT == "asciiz") 
                        {
                            for (int i = index + 2; i < tokens.size(); i++) 
                            {
                                string a = tokens[i];
                                for (int j = 1; j < a.size()-1; j++) 
                                {
                                    temp.value1.push_back(a[j]);
                                }
                                // temp.value1.push_back(0);
                            }
                        } 
                        else 
                        {
                            for (int i = index + 2; i < tokens.size(); i++) 
                            {
                                int a = string_to_int(tokens[i]);
                
                                temp.value.push_back(a);
                            }
                        }
                    } 
                    // else 
                    // {
                    //     temp.name = tokens[0];
                    //     temp.type = tokens[1].substr(1);
                        
                    //     for (int i = 2; i < tokens.size(); i++) 
                    //     {
                    //         int a = string_to_int(tokens[i]);
                    //         temp.value.push_back(a);
                    //     }
                    // }
                    stored.push_back(temp);
                }
            } 
            else if (start == 0) 
            {
                if (tokens[0] == ".data:" || tokens[0] == ".data") 
                { // Data part starts
                    start = 1;
                }
            }
        }
    }

    file.close();

    int pos = 0;
    string address = "0x10000000";
    int update = string_to_int(address);

    for (int i = 0; i < stored.size(); i++) {

        seg temp;
        
        if (stored[i].type == "byte") 
        {
            temp.name = stored[i].name;
            temp.position = address;
            for (int j = 0; j < stored[i].value.size(); j++) 
            {
                MemoryPair data;
                data.first = address;
                int val = stored[i].value[j];
                string hex = dec_to_hex_1(val);
                data.second = hex;
                datamemoryList.push_back(data);
                pos++;
                update += 1;
                address = dec_to_hex(update);
            }
        } 
        else if (stored[i].type == "word") 
        {
            temp.name = stored[i].name;
            temp.position = address;
            for (int j = 0; j < stored[i].value.size(); j++) 
            {
                MemoryPair data;
                data.first = address;
                int val = stored[i].value[j];
                string hex = dec_to_hex_1(val);

                data.second = hex;
                datamemoryList.push_back(data);
                pos += 4;
                update += 4;
                address = dec_to_hex(update);
            }
        } 
        else if (stored[i].type == "half") 
        {
            temp.name = stored[i].name;
            temp.position = address;
            for (int j = 0; j < stored[i].value.size(); j++) 
            {
                MemoryPair data;
                data.first = address;
                int val = stored[i].value[j];
                string hex = dec_to_hex_1(val);
                data.second = hex;
                datamemoryList.push_back(data);
                pos += 2;
                update += 2;
                address = dec_to_hex(update);
                
            }
        }
        else if (stored[i].type == "double")
        {
            temp.name = stored[i].name;
            temp.position = address;
            for (int j = 0; j < stored[i].value.size(); j++) 
            {
                MemoryPair data;
                data.first = address;
                int val = stored[i].value[j];
                string hex = dec_to_hex_1(val);
                data.second = hex;
                datamemoryList.push_back(data);
                pos += 8;
                update += 8;
                address = dec_to_hex(update);
            }
        }
        else if (stored[i].type == "asciiz")
        {
            temp.name = stored[i].name;
            temp.position = address;
            for (int j = 0; j < stored[i].value1.size(); j++) 
            {
                MemoryPair data;
                data.first = address;
                data.second.push_back(stored[i].value1[j]);
                datamemoryList.push_back(data);
                pos++;
                update += 1;
                address = dec_to_hex(update);
            }
        
        }
        datalabels.push_back(temp);
    }
    cout << endl;
     

    ofstream mc("machinecode.mc", ios::app);
    if (mc.is_open())
    {
        for (const auto &memory : datamemoryList) 
        {  
        mc << memory.first << " " ;
            for (auto value : memory.second)  
            {
                mc << value ;
            }
        mc << endl;
        }
    }
}
map<int, string> instructions;

int main() {   
    // map <string,int> labels;
    ifstream file("assemblycode.asm");
    int pc = 0x0 ;
    loadLabels("assemblycode.asm");
    ofstream mc("machinecode.mc");
    string line;
    while (getline(file, line)) {
        if (line.empty())
        {
            continue;
        }
        if (line.find('.') == string::npos){   
            if(line.find(':') != string::npos) 
            {
                if(splitString(line).size() > 2)
                {
                line = strip(line.substr(line.find(':') + 1));
                }
                else
                {
                    continue;
                }
            }
            vector<string> tokens = splitString(line);
            string instruction = tokens[0];
            char format = encodes_map[instruction][3][0];
            switch(format) {
                case 'R':    
                    mc << dec_to_hex_1(pc) << " " <<  encode_in_r(tokens) << endl;
                    instructions[pc] = line;
                    break;
                case 'I':
                    mc << dec_to_hex_1(pc) << " "  << encode_in_i(tokens) << endl;
                    instructions[pc] = line;
                    break;
                case 'S':
                    mc << dec_to_hex_1(pc) << " " << encode_in_s(tokens) << endl;
                    instructions[pc] = line;
                    break;
                case 'B':
                    mc << dec_to_hex_1(pc) << " " << encode_in_sb(tokens,pc) << endl;
                    instructions[pc] = line;
                    break;
                case 'U':
                    mc << dec_to_hex_1(pc) << " "  << encode_in_u(tokens) << endl;
                    instructions[pc] = line;
                    break;
                case 'J':
                    mc << dec_to_hex_1(pc) << " "  << encode_in_uj(tokens,pc) << endl;
                    instructions[pc] = line;
                    break;
                case 'L':
                    mc << dec_to_hex_1(pc) << " "  << encode_in_lw(tokens, pc) << endl;
                    instructions[pc] = line;
                    break;

            }
            pc += 4 ;
        }
    }
    mc << "--------------------------------------------------------"<< endl;

    read_data();
    mc.close();
    for (auto it : instructions) {
        cout << it.first << " " << it.second << endl;
    }
    return 0;
}
