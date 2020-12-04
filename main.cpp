#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

class Passport {
    private:
        vector<string> needed_keys;
        vector<string> optional_keys;

    public:
        map<string, string> data;

        ///Sets all of the needed keys to an empty string
        Passport(vector<string> keys, vector<string> optional) {
            needed_keys = keys;
            optional_keys = optional;

            Reset_Data();
        }

        ///clears the data, and then sets all of the needed keys to an empty string
        void Reset_Data() {
            string key;

            data.clear();
            for (int k = 0; k < needed_keys.size(); k++) {
                key = needed_keys[k];
                data[key] = "";
            }
        }
};

///check to see if a string (int) is in a range
bool In_Range(string str_val, int min, int max) {
    int value;
    stringstream value_s(str_val);
    value_s >> value;

    if (value >= min && value <= max) {
        return true;
    }
    else {
        return false;
    }
}

///Checks a string to see if it's all in hex format
bool Is_Hex(string hex) {
    char chr;

    for_each(hex.begin(), hex.end(), [](char chr){
        //if this character isn't in hex format
        if (!(chr >= '0' && chr <= '9' || chr >= 'a' && chr <= 'f')) {
                return false;
             }
    });

    return true;
}

bool Valid_Eye(string eye_color) {
    vector<string> valid_eyes = {
        "amb",
        "blu",
        "brn",
        "gry",
        "grn",
        "hzl",
        "oth"
    };
    bool valid = false;

    for_each(valid_eyes.begin(), valid_eyes.end(), [eye_color, &valid](string valid_color){
                        if (eye_color == valid_color) {
                            valid = true;
                          }
                          });

    return valid;
}

///check to see if a string of text is a number
bool Is_Num(string data) {
    for (int d = 0; d < data.size(); d++) {
        if (data[d] < '0' || data[d] > '9') {
            return false;
        }
    }

    return true;
}

///check to see if a passport has all of the valid keys
bool Valid(Passport passport, vector<string> needed_keys, vector<string> optional_keys) {
    string key;
    string value;

    for (int k = 0; k < needed_keys.size(); k++) {
        key = needed_keys[k];

        //return false if there is no data at all for the key
        if (passport.data[key] == "") {
            return false;
        }

        else {
            value = passport.data[key];

            if (key == "byr") {
                if (!In_Range(value, 1920, 2002)) {
                    return false;
                }
            }
            else if (key == "iyr") {
                if (!In_Range(value, 2010, 2020)) {
                    return false;
                }
            }
            else if (key == "eyr") {
                if (!In_Range(value, 2020, 2030)) {
                    return false;
                }
            }
            else if (key == "hgt") {
                //if cm
                if ((value.find('cm') < value.size())) {
                    if (!In_Range(value.substr(0, value.find('cm')-1), 150, 193)) {
                        return false;
                    }
                }
                //if in
                else if ((value.find('in') < value.size())) {
                    if (!In_Range(value.substr(0, value.find('in')-1), 59, 76)) {
                        return false;
                    }
                }
                //invalid
                else {
                    return false;
                }
            }
            else if (key == "hcl") {
                //has to start with # to be valid
                if (value[0] == '#') {
                    //check to see if the data is in hex format
                    value = value.substr(1);
                    if (value.size() != 6 || !Is_Hex(value)) {
                        return false;
                    }
                }
                else {
                    return false;
                }
            }
            else if (key == "ecl") {
                if (!Valid_Eye(value)) {
                    return false;
                }
            }
            else if (key == "pid") {
                if (value.size() != 9 || !Is_Num(value)) {
                    return false;
                }
            }
        }
    }


    return true;
}

void Parse_Line(Passport *passport, string line) {
    vector<string> data;
    stringstream stream(line);
    string key_value;
    string key;
    string value;

    while (stream >> key_value) {
        key = key_value.substr(0, key_value.find(':'));
        value = key_value.substr(key_value.find(':')+1);

        passport->data[key] = value;
    }
}

///gets the number of valid passports - needed keys in the passport and optional keys
int Valid_Passports(vector<string> needed_keys, vector<string> optional_keys) {
    ifstream file;
    int valid_count = 0;
    int total_count = 0;
    string line;
    Passport passport(needed_keys, optional_keys);

    file.open("passports.txt");

    getline(file, line);
    while (file) {
        if (line == "") {
            if (Valid(passport, needed_keys, optional_keys)) {
                valid_count++;
            }

            passport.Reset_Data();
        }
        //store the data from the current line
        Parse_Line(&passport, line);

        //get the next line
        getline(file, line);
    }

    //validate the last entry (it wasnt validated because there wasn't an empty line after it)
    if (Valid(passport, needed_keys, optional_keys)) {
        valid_count++;
    }

    file.close();

    return valid_count;
}

int main()
{
    int valid_count;

    valid_count = Valid_Passports({"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"}, {"cid"});

    cout << "There are " << valid_count << " valid passports" << endl;

    return 0;
}
