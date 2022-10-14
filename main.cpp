#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstring>
#include <unordered_map>
#include <map>
#include <stdlib.h>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::istringstream;
using std::ifstream;
using std::ofstream;
using std::getline;
using std::move;
using std::stoi;
using std::stod;
using std::unordered_map;
using std::map;

// Each line of data from PseudoData.csv is read into pseudo_data structure (storing only data)
struct pseudo_data {

    // Data members as per CSV file
    string agent_Breed;
    //string policy_ID; // This is not used in here - kept for consistence. Stored as a string to avoid storing big sized integer/double type variables. Decimal point ignored as was 0.
                     // Alternatively I'd use unsigned long long if we need number and decimal point can be ignored. Otherwise, find double long enough to store it.
    //int age; // Could use to calculate age over 15 years. This is not used in here - kept for consistence. If uint, be mindful or comparisons later with int.
    int social_Grade;
    int payment_at_Purchase;
    double attribute_Brand;
    double attribute_Price;
    double attribute_Promotions;
    bool auto_Renew;
    int inertia_for_Switch;

    // Agent breed data members used to calculate Breed_C lost, gained and regained
    string agent_Breed_Minus_One; // Agent breed year ago
    string agent_Breed_Minus_Two; // Agent breed two years ago
    bool breed_Switch; // True if changed from breed C to NC or from NC to C

    pseudo_data(string _agent_Breed,
               //string _policy_ID,
               //int _age,
               int _social_Grade,
               int _payment_at_Purchase,
               double _attribute_Brand,
               double _attribute_Price,
               double _attribute_Promotions,
               bool _auto_Renew,
               int _inertia_for_Switch)
    {
        agent_Breed = _agent_Breed;
        //policy_ID = _policy_ID;
        //age = _age;
        social_Grade = _social_Grade;
        payment_at_Purchase = _payment_at_Purchase;
        attribute_Brand = _attribute_Brand;
        attribute_Price = _attribute_Price;
        attribute_Promotions = _attribute_Promotions;
        auto_Renew = _auto_Renew;
        inertia_for_Switch = _inertia_for_Switch;
        agent_Breed_Minus_One = "";
        agent_Breed_Minus_Two = "";
        breed_Switch = 0;
   }
};

struct model_output {
    int breed_C;
    int breed_NC;
    int breed_C_Lost; // Switched to Breed_NC
    int breed_C_Gained; // Switch from Breed_NC
    int breed_C_Regained; // Switched to NC, then back to C

    model_output(int _breed_C,
               int _breed_NC,
               int _breed_C_Lost,
               int _breed_C_Gained,
               int _breed_C_Regained)
    {
        breed_C = _breed_C;
        breed_NC = _breed_NC;
        breed_C_Lost = _breed_C_Lost; // Switched to Breed_NC
        breed_C_Gained = _breed_C_Gained; // Switch from Breed_NC
        breed_C_Regained = _breed_C_Regained; // Switched to NC, then back to C
    }
};

/* -----------------------------------------------------------------------------------------
HOW TO RUN
In terminal run these lines:
$ g++ main.cpp -o main
$ ./main arg1 arg2 arg3

There are three input arguments:
1. Brand factor with values ranging from (0.1 - > 2.9).
2. Input data file name.
3. Output data file name.

Example:
$ g++ mainreturn.cpp -o main
$ ./main 2.1 PseudoData.csv OutputData.csv

--------------------------------------------------------------------------------------------*/

int main(int argc, char *argv[]) {
    cout << "Hello World"; // prints Hello World

    if (argc != 4)
    {
        cout << "Incorrect number of arguments. Provide THREE arguments:" << endl;
        cout << "1. Brand_Factor with values ranging from (0.1 -> 2.9)." << endl;
        cout << "2. Input data file name (with directory if required)." << endl;
        cout << "3. Output data file name (with directory if required)." << endl;
        return 0;
    }

    double brand_Factor = atoi(argv[1]);
    string input_Filename{argv[2]};
    string output_Filename{argv[3]};

    // Parse CSV file into map container

    ifstream input{input_Filename};

    if (!input.is_open())
    {
        cerr << "Couldn't read file: " << input_Filename << "\n";
        return 1;
    }

    unordered_map<string,pseudo_data>map_Data;

    for (string line; getline(input, line,'\r');)
    {
        istringstream ss(move(line));
        vector<string> row;

        // Read each row of data into vector of strings
        for (string value; getline(ss, value, ',');)
        {
            row.push_back(move(value));

        }

        if (row[1].compare("Policy_ID") != 0 && row.size() == 10)
        {
            // 0 Agent_Breed 	1 Policy_ID 	2 Age 	3 Social_Grade 	4 Payment_at_Purchase
            // 5 Attribute_Brand 	6 Attribute_Brand 	7 Attribute_Promotions 	8 Auto_Renew 	9 Inertia_for_Switch
            pseudo_data rowData(row[0], stoi(row[3]), stoi(row[4]), stod(row[5]), stod(row[6]), stod(row[7]), stoi(row[8]), stoi(row[9]));
            map_Data.insert({row[1], rowData});
        }
    }

    input.close();

    // Print out the input data
    /*for (const auto rowThis : map_Data)
    {
        cout << std::setw(10) << rowThis.second.agent_Breed << " "
                << rowThis.second.social_Grade << " "
                << rowThis.second.payment_at_Purchase << " "
                << rowThis.second.attribute_Brand << " "
                << rowThis.second.attribute_Promotions << " "
                << rowThis.second.auto_Renew << " "
                << rowThis.second.inertia_for_Switch << endl;
    }*/

    // Calculate breed agent characteristics for the next 15 years ---- Model Run (run for 15 years, in 1 year increments) ----
    map<int,model_output>map_Output;

    for (int i = 1; i < 16; i++)
    {
        int breed_C = 0;
        int breed_NC = 0;
        int breed_C_Lost = 0;
        int breed_C_Gained = 0;
        int breed_C_Regained = 0;
        for (auto key_value : map_Data)
        {
            pseudo_data temp_Data = key_value.second;

            if (temp_Data.auto_Renew)
            {
            // Do nothing, maintain Breed
            }
            else
            {
                double randomNumber = rand() * 3;
                double affinity = (double)temp_Data.payment_at_Purchase / temp_Data.attribute_Price + (randomNumber * temp_Data.attribute_Promotions * (double)temp_Data.inertia_for_Switch);
                bool breed_Switch_old = temp_Data.breed_Switch;

                if (temp_Data.agent_Breed == "Breed_C" && affinity < (temp_Data.social_Grade * temp_Data.attribute_Brand))
                {
                    temp_Data.agent_Breed = "Breed_NC";
                    ++breed_C_Lost;
                    temp_Data.breed_Switch = 1;
                    if (breed_Switch_old)
                      ++breed_C_Regained;
                }
                else if (temp_Data.agent_Breed == "Breed_NC" && affinity < (temp_Data.social_Grade * temp_Data.attribute_Brand * brand_Factor))
                {
                    temp_Data.agent_Breed = "Breed_C";
                    ++breed_C_Gained;
                    temp_Data.breed_Switch = 1;
                }
                else
                {
                    temp_Data.breed_Switch = 0;
                }

                map_Data.at(key_value.first).agent_Breed = temp_Data.agent_Breed;
                map_Data.at(key_value.first).breed_Switch = temp_Data.breed_Switch;
            }
            if (temp_Data.agent_Breed == "Breed_C")
                ++breed_C;
            if (temp_Data.agent_Breed == "Breed_NC")
                ++breed_NC;
        }

        map_Output.insert({i, model_output(breed_C, breed_NC, breed_C_Lost, breed_C_Gained, breed_C_Regained)});
        //cout << std::setw(10) << breed_C << " " << breed_NC << " " << breed_C_Lost << " " << breed_C_Gained << " " << breed_C_Regained << endl;
    }

    // Print out the model output data into console
    for (const auto rowThis : map_Output)
    {
        cout << std::setw(10) << "Year " << rowThis.first << " " << rowThis.second.breed_C << " "
                << rowThis.second.breed_NC << " "
                << rowThis.second.breed_C_Lost << " "
                << rowThis.second.breed_C_Gained << " "
                << rowThis.second.breed_C_Regained << endl;
    }


    // Save model output to CSV file
    ofstream output_Data;
    output_Data.open(output_Filename);
    if( !output_Data )
    {
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }

    output_Data << "Year" << "," << "Breed_C" << "," << "Breed_NC"
        << "," << "Breed_C_Lost" << "," << "Breed_C_Gained" << "," << "Breed_C_Regained" << endl;

    for (const auto rowThis : map_Output)
    {
        output_Data << rowThis.first << ","
                << rowThis.second.breed_C << ","
                << rowThis.second.breed_NC << ","
                << rowThis.second.breed_C_Lost << ","
                << rowThis.second.breed_C_Gained << ","
                << rowThis.second.breed_C_Regained << endl;
    }

    output_Data.close();

    cout << "Model output saved." << endl;

   return 0;
}

/*
Future work and things to consider
- Check input arguments. If correct amount and data type.
- Visually checked, all data appears to be valid. It would be good to have sanity check for values (in they are not missing, if valid, e.g. not negative or above 0 or non 0 like in division by attribute_Price).
- Make loading Brand_Factor foolproof. Need to decide if any restrictions on decimal places/digits in total required and if ranges include edge values.
- Ranges for rand().
- Is Affinity okay to be int?
- Add sanity check if total breed agents = C + NC.
- Any limit on data size? Number of rows?
- Don't display output if no data loaded in or didn't calculate properly.
- Use usize in for loop with small amount of iterations.
- Sort out loading CSV file with various end of line options, e.g. \n or \r\n.
- Investigate end of line options for saving model output CSV file.
- Display results in different form and extract what is important, e.g. plot, key values, trends.
- Consider warning user if overriding existing model output file.

*/

/* --- Code to display output in jupyter notebook ---

import pandas as pd

# get file name from the user
print("Provide file name containing model data:")
outputFilename = input()

# reading the database
data = pd.read_csv(outputFilename)

# printing the results
display(data)


*/
