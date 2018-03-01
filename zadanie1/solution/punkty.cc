/* Autorzy rozwiązania : Michał Radecki, Wojciech Kończewski */

#include<cstdio>
#include<iostream>
#include<string>
#include<fstream>
#include<regex>
#include<set>
#include<map>

using namespace std;

vector<string> split(const std::string &s)
{
    vector<string> elements;
    stringstream ss1, ss2;
    ss1.str(s);
    string str1, str2;
    int counter = 0;
    while(getline(ss1, str1, '/'))
    {
        counter++;
        ss2 << str1;
        if(counter == 3) // grupa1/zadanie1/mr371591+ab123456+ ...  --> part after second '/'
        {
            while(getline(ss2, str2, '+'))
            {
                elements.push_back(str2);
            }
        }
        ss2.str("");
    }
    return elements;
}

int check_number_of_params(int argc, char* argv[])
{
    if(argc != 2)
    {
        cerr << "Usage: " << argv[0] << " file" << endl;
        return 1;
    }
    else return 0;
}

int try_to_open_file(string filename, fstream& my_file_stream)
{
    my_file_stream.open(filename);
    if(my_file_stream.is_open() == false)
    {
        cerr << "Error: problem with file " << filename << endl;
        return 1;
    }
    return 0;
}

void read_given_file_with_identificators(string filename, fstream& my_file_stream, set<string>& identificators)
{
    string reg2_6 = "[[:alpha:]]{2}[[:digit:]]{6}";
    string reg2m_4 = "[[:alpha:]]{2}m-[[:digit:]]{4}";
    string correct_identificator = "(" + reg2_6 + "|" + reg2m_4 + ")";

    string line;
    int line_number = 0;
    regex regular_expression_for_identificators(correct_identificator);

    while(getline(my_file_stream, line))
    {
        line_number++;
        if(line != "")
        {
            bool match = regex_match(line, regular_expression_for_identificators);
            if(match)
            {
                if(identificators.count(line) == 1)
                {
                    cerr << "Error in " << filename << ", line " << line_number << ": " << line << endl;
                }
                else identificators.insert(line);
            }
            else cerr << "Error in " << filename << ", line " << line_number << ": " << line << endl;
        }
    }
}

void find_pairwise_different_identificators(set<string> const& correct_identificators, set<string>& pairwise_different_identificators, vector<string>& identificators_in_group, int line_number)
{
    for(string id : identificators_in_group)
    {
        if(correct_identificators.count(id) == 1 && pairwise_different_identificators.count(id) == 0)
        {
            pairwise_different_identificators.insert(id);
        }
        else cerr << "Error in cin, line " << line_number << ": " << id << endl;
    }
}

void add_pairs_to_map_of_cooperations(set<string>& pairwise_different_identificators, map<pair<string, string>, int>& map_of_cooperations)
{
    map<pair<string, string>, int>::iterator map_it;
    for(string id1 : pairwise_different_identificators)
    {
        for(string id2 : pairwise_different_identificators)
        {
            if(id1 != id2)
            {
                map_it = map_of_cooperations.find(pair<string, string>(id1, id2));
                if(map_it != map_of_cooperations.end())
                {
                    map_it->second += 1;
                }
                else map_of_cooperations.insert(pair<pair<string, string>, int>(pair<string, string>(id1, id2), 1));
            }
        }
    }
}

void check_correctness_of_identificators(set<string> const& correct_identificators, vector<string>& identificators_in_group, int line_number, map<pair<string, string>, int>& map_of_cooperations, string line)
{
    if(identificators_in_group.size() != 2 && identificators_in_group.size() != 3)
    {
        cerr << "Error in cin, line " << line_number << ": " << line << endl;
    }
    else
    {
        set<string> pairwise_different_identificators;
        find_pairwise_different_identificators(correct_identificators, pairwise_different_identificators, identificators_in_group, line_number);
        add_pairs_to_map_of_cooperations(pairwise_different_identificators, map_of_cooperations);
    }
}

void read_cin_input_with_groups_to_tasks(map<pair<string,string>,int>& map_of_cooperations, set<string> const& correct_identificators)
{
    unsigned int line_number = 0;
    string line;

    string reg2_6 = "[[:alpha:]]{2}[[:digit:]]{6}";
    string reg2m_4 = "[[:alpha:]]{2}m-[[:digit:]]{4}";
    string correct_id = "(" + reg2_6 + "|" + reg2m_4 + ")";
    string reg_cin_line = "grupa[1-8]/zadanie[1-6]/" + correct_id + "(\\+" + correct_id + ")*";
    regex my_regular_expression(reg_cin_line);

    while(getline(cin, line))
    {
        line_number++;
        if(line != "")
        {
            bool match = regex_match(line, my_regular_expression);
            if(not match)
            {
                cerr << "Error in cin, line " << line_number << ": " << line << endl;
            }
            else
            {
                vector<string> identificators_in_group = split(line);
                check_correctness_of_identificators(correct_identificators, identificators_in_group, line_number, map_of_cooperations, line);
            }
        }
    }
}

void count_cooperation_numbers(map<pair<string, string>, int>& map_of_cooperations, map<string, int>& map_of_cooperation_numbers)
{
    map<string, int>::iterator map_of_numbers_iterator;
    map<pair<string, string>, int>::iterator map_iterator;
    string id;
    int value_in_map_of_cooperations;
    for(map_iterator = map_of_cooperations.begin(); map_iterator != map_of_cooperations.end(); map_iterator++)
    {
        id = map_iterator->first.first;
        value_in_map_of_cooperations = map_iterator->second;

        int value = (0 + value_in_map_of_cooperations - 1) * value_in_map_of_cooperations;
        value = value/2;

        map_of_numbers_iterator = map_of_cooperation_numbers.find(id);
        if(map_of_numbers_iterator == map_of_cooperation_numbers.end())
        {
            if(value != 0)
            {
                map_of_cooperation_numbers.insert(pair<string, int>(id, value));
            }
        }
        else map_of_numbers_iterator->second += value;
    }
}

void give_answers(map<string, int>& map_of_cooperation_numbers)
{
    multimap<string, int> my_multimap;
    map<string, int>::iterator map_iterator;
    string id;
    int number;

    for(map_iterator = map_of_cooperation_numbers.begin(); map_iterator != map_of_cooperation_numbers.end(); map_iterator++)
    {
        string id = map_iterator->first;
        string id_number = id.substr(2, 6); //mr371591 --> 371591
        int value = map_iterator->second;
        my_multimap.insert(pair<string, int>(id_number, value));
    }

    for(map_iterator = my_multimap.begin(); map_iterator != my_multimap.end(); map_iterator++)
    {
        id = map_iterator->first;
        number = map_iterator->second;
        if(number != 0)
        {
            cout << id << ";" << number << ";" << endl;
        }
    }
}

int main(int argc, char* argv[])
{
    int code = check_number_of_params(argc, argv);
    if(code == 1) return 1;

    fstream my_file_stream;
    string filename(argv[1]);
    code = try_to_open_file(filename, my_file_stream);
    if(code == 1) return 1;

    set<string> correct_identificators;

    string file_name(argv[1]);
    read_given_file_with_identificators(filename, my_file_stream, correct_identificators);

    map<pair<string,string>,int> map_of_cooperations;
    read_cin_input_with_groups_to_tasks(map_of_cooperations, correct_identificators);

    map<string, int> map_of_cooperation_numbers;
    count_cooperation_numbers(map_of_cooperations, map_of_cooperation_numbers);
    give_answers(map_of_cooperation_numbers);

    return 0;
}
