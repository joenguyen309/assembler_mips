//Edit: 3:26pm 13/12/2020
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <map>
#include <math.h>

using namespace std;
// Delare Variable
map <int,vector<string>> Label; // label jump
map <int,vector<string>> Code;  // instruction
map <int,vector<string>> _Label_for_Data; // Label name for variable.
map <string,string> Address_Label_Data; 
map <string,string> Address_Label; 
map <int,string> _Open_File_Read(); // Open file to read inside and store in map 


// Function
void _Check_MAP (map<int,vector<string>> input_map); // print in console the element of Map

int Count_Space_in_String (string input_string); // Count the space in string

vector <string> Split (string input,string delimiter); // Split string with delimiter

map <int,vector<string>> _Edit_File_Read (map <int,string> file_read); // Return Map that has been removed space and comment

string _Edit_String (string input_string); // Return Code 

map <string,string> _The_Array_of_Label_Data (map <int,vector<string>> _Label_data); // his table will make address for Label

map <string,string> _The_Array_of_Jump_Label  (map <int,vector<string>> _Label);// This table will make address for Jump Label

void _Check_MAP_ver2 (map <string,string> input_map); // This check the map<string,string>

vector<string> Final_Translate (map <int,vector<string>> Input_Code);

vector<string> Convert_Instruction_Code_ASM (vector<string>instruction); // this function will make MC for each instruction.

string Convert_Decimal_to_Binary (int value);

void _Open_File_Write_and_Write_to_File ();

bool Chech_if_I_type (string input_code);

bool Check_if_J_type (string input_code);

string Convert_Regsistor (string input_reg);

string Decode_Regsister (string input_regsister,string instruction);

int main (void){
    // Load file input to map Load_File
    map <int,string> Load_File = _Open_File_Read();
    // Remove space in elem.second of Load_File
    Code = _Edit_File_Read(Load_File); // Remove comment and Space in File. This's code map
    //Test to check vetor<string>
    //cout<< "\nCode\n";
    //_Check_MAP(Label);
    //cout<<endl;
    //_Check_MAP(Code);
    //cout<<endl;
    //_Check_MAP(_Label_for_Data);
    //cout<<endl;
    Address_Label = _The_Array_of_Label_Data(_Label_for_Data); // This's table address of Label (Start at 0x0h)
    //_Check_MAP_ver2(Address_Label); 
    //cout<<endl;
    Address_Label_Data  = _The_Array_of_Jump_Label (Label); // This table will make address for Jump Label
    //_Check_MAP_ver2(Address_Label_Data);    
    _Open_File_Write_and_Write_to_File();
}

map <int,string> _Open_File_Read(){
    ifstream File_Input ("D:\\OneDrive\\Desktop\\File_Test_ASM.asm"); // Your File's Direction
    map <int,string> Load_File;
    string Text;
    int PC,count = 0;
    // Add to Map
    while (getline(File_Input,Text)){
        Load_File.insert(pair<int,string>(PC,Text));
        PC ++;
    }
    File_Input.close();
    return Load_File;
}

void _Open_File_Write_and_Write_to_File () {
    ofstream File_Write ("D:\\OneDrive\\Desktop\\File_MC.txt");
    vector<string> MC = Final_Translate(Code);
    for (auto i = MC.begin(); i != MC.end();++i) {
        File_Write << *i <<endl;
    }
    File_Write.close();
}

void _Check_MAP_ver2 (map <string,string> input_map){
    for(auto elem : input_map)
    {
        cout <<elem.first<< "  " <<elem.second<<endl;
    } 
}

void _Check_MAP (map<int,vector<string>> input_map){
    for(auto elem : input_map)
    {
        cout <<elem.first<< "  ";
        for (auto i = elem.second.begin(); i != elem.second.end(); ++i)
        {
           cout << *i << " ";
        }
        cout<<endl;
    } 
} 

int Count_Space_in_String (string input_string){
    int count = 0;
    char check[0];
    while (input_string[0] == ' '){
        input_string.erase(0,1);
        count++;
    }
    return count;
}

vector <string> Split (string input,string delimiter){
    vector <string> result;
    if (input.length() !=0){
        input = input + delimiter;
    }
    size_t pos = 0;
    string token;
    while ((pos = input.find(delimiter)) != std::string::npos ){
        token = input.substr(0,pos);
        if (token != " "){
            result.push_back(token);
        }
        input.erase(0,pos+delimiter.length());
    }   
    return result; 
}

string _Edit_String (string input_string) {
    string result ="";
    int flag = 0;
    for (int i = 0; i < input_string.size(); i++){
        if (input_string[i] != '#' && flag !=1){
            result += input_string[i];
        }
        else {
            flag = 1;
        }
    }
    return result;
}

vector <string> Comment_Edit (vector <string> input){
    vector <string> Result;
    int size = input.size();
    for (auto i = input.begin(); i != input.end(); ++i)
        {
            if ((*i).find("#") !=0){
              Result.push_back((*i));
            }
        }
    return Result;
}

map <int,vector<string>> _Edit_File_Read (map <int,string> file_read)
{   
    map <int,string> Temp = file_read;
    map <int,vector<string>> Result;
    for(auto elem : Temp){
        string temp = _Edit_String(elem.second); // Remove comment
        temp.erase(0,Count_Space_in_String(temp)); //Delete space
        vector<string> label = Split(temp ," "); // Start split t
        if (label .size() >1 && label.size() <=2){ // Check Is it label is a code ??
            Result.insert(pair <int,vector<string>>(elem.first+1,label));  // just add none-null line and none-label line
        }
        else if (label.size() ==1){            
            for (auto i = label.begin(); i != label.end(); ++i)
            {
                if ((*i) == "syscall") { //syscall also a code
                    Result.insert(pair <int,vector<string>>(elem.first+1,label));
                }
                else { // Take a Label
                    Label.insert(pair <int,vector<string>>(elem.first+1,label)); // add to Label 
                }
            }
        }
        else if(label.size() >4){
            _Label_for_Data.insert(pair <int,vector<string>>(elem.first+1,label));
        }
    }
    return Result;
}

int _Find_Position_in_String (string input, char find){
    int position=0;
    for (int i=0;i<input.length();i++){
        if (input[i] != find){
            position++;
        }
    }
    return position;
}

string Convert_Decimal_to_Binary (int value){
    int temp_value = value;
    string Result= "";
    char Hexa_Table[] {'0','1'};
    int temp;
    if (value >0) {
        while (temp_value >0){
        temp = temp_value % 2;
        Result = Hexa_Table[temp] + Result;
        temp_value /= 2;
        }
    }
    else if (value <0)
    {
        temp_value = -temp_value;
        temp_value--;
        while (temp_value >0){
        temp = temp_value % 2;
        Result = Hexa_Table[temp] + Result;
        temp_value /= 2;
        } 
    } 
    else if (value==0) {
        Result = "0";
    }
    int Size_result =32 - Result.size();
    for (int i=0; i<(Size_result); i++) {
        Result = "0" + Result; 
    }
    if (value <0) {
        for (int j =0; j<(Result.length());j++) {
            if (Result[j] == '0') {
                Result[j] = '1';
            }
            else {
                Result[j] = '0';
            }
        }
    }
    //Result = "0x"+Result + "b";
    return Result;
}

map <string,string> _The_Array_of_Label_Data (map <int,vector<string>> _Label){
    map <string,string> Result;
    int Address = 500;
    string Name_Label;

    for(auto elem : _Label)
    {   
        auto i = elem.second.begin();
        Name_Label = *i;
        Name_Label.erase(Name_Label.length()-1,Name_Label.length());
        Result.insert(pair <string,string>(Convert_Decimal_to_Binary(Address),Name_Label));
        Address +=4;
    } 
    return Result;
}

map <string,string> _The_Array_of_Jump_Label  (map <int,vector<string>> _Label) {
    map<string,string> Result;
    string Name_Label;
    int Address=0;
    for(auto elem : _Label)
    {   
        for (auto i = elem.second.begin(); i != elem.second.end(); ++i)
        {
            if (*i != ".data" && *i != ".text" ) 
            {
                Address = elem.first *4; // Address =  line in text * 4;
                Name_Label = elem.second.front();
                Name_Label.erase(Name_Label.length()-1,Name_Label.length());
                Result.insert(pair <string,string>(Convert_Decimal_to_Binary(Address),Name_Label));
            }
        }    
    } 
    return Result;
}

bool Check_if_R_type (string input_code){
    return ((input_code == "add")   || 
            (input_code == "addu")  || 
            (input_code == "and")   ||
            (input_code == "jr")    ||
            (input_code == "sll")   ||
            (input_code == "sub")   ||
            (input_code == "slt")   ||
            (input_code == "ja")    ||
            (input_code == "move")  ||
            (input_code == "syscall") 
            ) ? true:false;
}

bool Chech_if_I_type (string input_code){
    return ((input_code == "addi")  || 
            (input_code == "addiu") || 
            (input_code == "andi")  ||
            (input_code == "beq")   ||
            (input_code == "bne")   ||
            (input_code == "lbu")   ||
            (input_code == "lhu")   ||
            (input_code == "lui")   ||
            (input_code == "lw")    ||
            (input_code == "sw")    ||
            (input_code == "li")    ||
            (input_code == "la")    ||
            (input_code == "bge")   ||
            (input_code == "blt")   ||
            (input_code == "subi")
            ) ? true:false;
}

bool Check_if_J_type (string input_code) {

    return ((input_code == "j")   || 
            (input_code == "jal")  
            ) ? true:false;
}

string Convert_R_type (string input_r_type) {
    string result = "";
        if (input_r_type =="add" ) {
            result = "100000";
        }
        else if (input_r_type =="addu") {
            result = "100001";
        }
        else if (input_r_type =="and") {
            result = "100100";
        }
        else if (input_r_type == "jr") {
            result = "001000";
        }
        else if (input_r_type =="sll") {
            result = "000000";
        }
        else if (input_r_type =="sub") {
            result = "100010";
        }
        else if (input_r_type =="jr") {
            result = "001000";
        }
        else if (input_r_type =="slt") {
            result = "101010";
        }
        
    return result;
}

string Convert_I_type (string input_i_type){    
    string result = "";
        if (input_i_type =="addi"){
            result = "001000";
        }
        else if (input_i_type =="addiu") {
            result = "001001";
            
        }
        else if (input_i_type =="andi") {
            result = "001100";
            
        }
        else if (input_i_type =="beq") {
            result = "000100";
        }
        else if (input_i_type =="lui") {
            result = "001111";
        }
        else if (input_i_type =="ori"){
            result = "001101";
        }
        else if (input_i_type =="bne") {
            result = "000101";
        }
        else if (input_i_type =="lbu") {
            result = "100100";
        }
        else if (input_i_type =="lhu") {
            result = "100101";
        }
        else if (input_i_type =="lui") {
            result = "001111";
        }
        else if (input_i_type =="lw") {
            result = "100011";
        }
        else if (input_i_type =="sw") {
            result = "101011";
        }
    return result;
}

string Convert_J_type (string input_j_type) {
    string result = "";
        if (input_j_type =="j") {
            result = "000010";
        }
        else if (input_j_type =="jal") {
            result = "000011";
        }
    return result; 
}

string Convert_Regsistor (string input_reg) {
    string result = "";
    if (input_reg =="$zero"){
            result = "00000";
        }
        else if (input_reg =="$at") {
            result = "00001";
            
        }
        else if (input_reg =="$v0") {
            result = "00010";
            
        }
        else if (input_reg =="$v1") {
            result = "00011";
        }
        else if (input_reg =="$a0") {
            result = "00100";
        }
        else if (input_reg =="$a1") {
            result = "00101";
        }
        else if (input_reg =="$a2") {
            result = "00110";
        }
        else if (input_reg =="$a3") {
            result = "00111";
        }
        else if (input_reg =="$t0") {
            result = "01000";
        }
        else if (input_reg =="$t1") {
            result = "01001";
        }
        else if (input_reg =="$t2") {
            result = "01010";
        }
        else if (input_reg =="$t3") {
            result = "01011";
        }
        else if (input_reg =="$t4") {
            result = "01100";
        }
        else if (input_reg =="$t5") {
            result = "01101";
        }
        else if (input_reg =="$t6") {
            result = "01110";
        }
        else if (input_reg =="$t7") {
            result = "01111";
        }else if (input_reg =="$s0") {
            result = "10000";
        }
        else if (input_reg =="$s1") {
            result = "10001";
        }
        else if (input_reg =="$s2") {
            result = "10010";
        }else if (input_reg =="$s3") {
            result = "10011";
        }
        else if (input_reg =="$s4") {
            result = "10100";
        }
        else if (input_reg =="$s5") {
            result = "10101";
        }
        else if (input_reg =="$s6") {
            result = "10110";
        }
        else if (input_reg =="$s7") {
            result = "10111";
        }
        else if (input_reg =="$t8") {
            result = "11000";
        }
        else if (input_reg =="$t9") {
            result = "11001";
        }
        else if (input_reg =="$k0") {
            result = "11010";
        }
        else if (input_reg =="$k1") {
            result = "11011";
        }
        else if (input_reg =="$gp") {
            result = "11100";
        }
        else if (input_reg =="$sp") {
            result = "11101";
        }
        else if (input_reg =="$fp") {
            result = "11110";
        }
        else if (input_reg =="$ra") {
            result = "11111";
        }
    return result;
}

string Decode_Regsister (string input_regsister,string instruction) {
    vector <string> Regsister_Array = Split(input_regsister,","); 
    string Result,Rd ="";
    int number;
    if (Check_if_R_type(instruction)){
        Result = "000000";
        if (instruction == "syscall") {
            Result ="00000000000000000000000000001100";     
        }
        else if (instruction == "jr") {
            Result +=  Convert_Regsistor(Regsister_Array[0]) + "000000000000000" +  Convert_R_type(instruction);
        }
        else if (instruction == "sll")
        {
            int number = stoi (Regsister_Array[2]);
            string convert_num = Convert_Decimal_to_Binary(number);
            convert_num.erase(0,27);
            Result = Result + "00000" + Convert_Regsistor(Regsister_Array[1]) + Convert_Regsistor(Regsister_Array[0]) +convert_num + Convert_R_type(instruction);       
        }    
        else if (instruction == "move") {
            Result = Result  + Convert_Regsistor(Regsister_Array[1]) + Convert_Regsistor("$zero") +Convert_Regsistor(Regsister_Array[0]) +  + "00000" + Convert_R_type("add");
        } 
        else  {
            for (auto i = Regsister_Array.begin(); i != Regsister_Array.end();++i) 
            {
                if (i == Regsister_Array.begin()){
                    Rd = Convert_Regsistor(*i);
                }
                else {
                    Result += Convert_Regsistor(*i);
                }
            }
            Result = Result +Rd +"00000" + Convert_R_type(instruction);
        }
    }
    else if (Chech_if_I_type(instruction)) {
        string label = "";
        string address_label = "111111111111111111111111111111111111111111111111";
        Result += Convert_I_type(instruction);
        if(instruction == "beq" || instruction == "bne") {
            // Decode 2 regsister
            Result  = Convert_I_type(instruction);
            for (auto i = Regsister_Array.begin(); i != Regsister_Array.end(); ++i) {
                if (i!=Regsister_Array.end()) 
                {
                    Result += Convert_Regsistor (*i);
                }
            }
            // find the address in Jump Label
            for (auto j : Address_Label_Data) {

                if (j.second == Regsister_Array.back()){
                    address_label = j.first;
                }
            }
            address_label.erase(0,16);
            Result += address_label;
        }
        else if (instruction == "lw" || instruction == "sw") {

            vector <string> Reg_split_p1 = Split(input_regsister,",");
            vector <string> Reg_split_p2 = Split(Reg_split_p1.back(), "(");
            vector <string> Reg_split_p3 = Split(Reg_split_p2.back(), ")");
            string reg_i = Reg_split_p3[0];
            number = stoi(Reg_split_p2[0]);
            string convert_num = Convert_Decimal_to_Binary(number);

            convert_num.erase(0,16);

            Result = Convert_I_type(instruction) + Convert_Regsistor(reg_i)+Convert_Regsistor(Reg_split_p1[0]) +convert_num;
        }
        else if (instruction == "li") {
            number = stoi (Regsister_Array[1]); 
            string number_binary = Convert_Decimal_to_Binary(number);
            string temp_number = Convert_Decimal_to_Binary(number);

            string highbit_number = number_binary.erase(16,number_binary.length());
            string lowbit_number = temp_number.erase(0,16);
            Result = Convert_I_type("lui") +"00000"+ Convert_Regsistor(Regsister_Array[0]) + highbit_number + " ->>>" + Convert_I_type("ori") +Convert_Regsistor(Regsister_Array[0])+ Convert_Regsistor(Regsister_Array[0]) + lowbit_number;
        }
        else if (instruction == "la") {      
            string temp_label;
            vector <string> Check_label = Split(Regsister_Array[1],"(");
            if (Check_label.size() == 1) {
                for (auto i : Address_Label) {
                    if (i.second == Regsister_Array[1])
                    {
                    address_label = i.first;
                    temp_label = address_label;
                    }
                }
                string highbit_label = address_label.erase(16,address_label.length());
                string lowbit_label = temp_label.erase(0,16);
                Result = Convert_I_type("lui") +"00000"+ Convert_Regsistor(Regsister_Array[0]) + highbit_label + " ->>>" + Convert_I_type("ori") + Convert_Regsistor(Regsister_Array[0])+Convert_Regsistor(Regsister_Array[0]) + lowbit_label;
            }
        }
        else if (instruction == "subi") {
            int number = stoi (Regsister_Array[2]);
            number = - number;
            string num_convert = Convert_Decimal_to_Binary(number);
            num_convert.erase(0,16);
            Result = Convert_I_type("addi") + Convert_Regsistor(Regsister_Array[1]) + Convert_Regsistor(Regsister_Array[0]) +num_convert;
        }
        else if (instruction == "bge" || instruction == "bgt" ) {

            Result = "000000" + Convert_Regsistor(Regsister_Array[0]) + Convert_Regsistor(Regsister_Array[1]) +Convert_Regsistor("$at") + "00000" + Convert_R_type("slt");
            for (auto elem : Address_Label_Data) {
                if (elem.second == Regsister_Array[2]) {
                    address_label = elem.first;
                }
            }
            address_label.erase(0,16);
            if (instruction == "bge") {
                Result = Result + " ->>>" + Convert_I_type ("beq") + Convert_Regsistor("$at") +Convert_Regsistor ("$zero") +address_label;
            }  
            else {
                Result = Result + " ->>>" + Convert_I_type ("bne") + Convert_Regsistor("$at") +Convert_Regsistor ("$zero") +address_label;
            }
        }
        else if (instruction == "blt" || instruction == "ble" ) {
             Result = "000000" + Convert_Regsistor(Regsister_Array[1]) + Convert_Regsistor(Regsister_Array[0]) +Convert_Regsistor("$at") + "00000" + Convert_R_type("slt");
            for (auto elem : Address_Label_Data) {
                if (elem.second == Regsister_Array[2]) {
                    address_label = elem.first;
                }
            }
            address_label.erase(0,16);
           if (instruction == "blt") {
                Result = Result + " ->>>" + Convert_I_type ("bne") + Convert_Regsistor("$at") +Convert_Regsistor ("$zero") +address_label;
           } 
           else {
               Result = Result + " ->>>" + Convert_I_type ("beq") + Convert_Regsistor("$at") +Convert_Regsistor ("$zero") +address_label;
           }
        }
        else {
            for (auto i = Regsister_Array.begin(); i != Regsister_Array.end(); ++i) {
                if (*i >= "0" && *i<="9") 
                {
                    number = stoi (*i);
                }
                else {
                    Result += Convert_Regsistor (*i);
                }
            }           
            string convert_num = Convert_Decimal_to_Binary(number);
            convert_num.erase(0,16);
            Result += convert_num;
        }
    }
    else if (Check_if_J_type(instruction)) {
        Result += Convert_J_type(instruction);
        string address_label = "111111111111111111111111111111111111111111111111";
        for (auto j : Address_Label_Data) {
            if (j.second == Regsister_Array.back()){
                address_label = j.first;
            }
        }
        address_label.erase(0,6);
        Result += address_label;      
    }
    return Result;
}

vector<string> Final_Translate (map <int,vector<string>> Input_Code) {
    string Result = "";
    vector <string> MC;
    for (auto elem : Input_Code) {
        Result = Decode_Regsister(elem.second.back(),elem.second.front());
        MC.push_back(Result);
    }
    return MC;   
}










