//
//  main.cpp
//  Thinker
//
//  Created by Dustin on 9/12/13.
//

#include <iostream>
#include <pthread.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <map>
#include <sstream>
#include <time.h>
#include <cmath>
#include <regex>

using namespace std;

int chances = 3;
/**
 *  0:  thinker / Dissector
 *  1:  part 1
 *  2:  part 2
 *  3:  part 3
 */
int semaphore = 0;
//  all parts received adiquate parameters
bool all_good = true;
//vector<string> the_messages;
//map<string, vector<string> > message_map;

void * part_1(void * sections);

void * part_2(void * sections);

void * part_3(void * sections);

void * thinker(void * sections);

void * disector(void * section);

// gives age, return -1 if no correct input
int get_age(string b_day);

//  returns true if the size is 6 and satisfies "[a-z].*[0-9].*" regex
bool password_regex_match(string password);

int main(int argc, const char * argv[])
{
    
    pthread_t thinker_id;
    string message = "";
    
    
    int result;
    //  create thinker thread
    result = pthread_create(&thinker_id, NULL, thinker, (void *) message.c_str());
    if (result == 0) {
        printf("<Thinker Thread Created>\n");

    }
    else{
        printf("<Thinker Thread Not Created>\n");
        return -1;
    }
    
    result = pthread_join(thinker_id, NULL);
    if (result  == 0) {
        printf("<Thinker Thread Joined>\n");
    }
    else{
        printf("<Thinker Thread Not Joined>\n");
        return -1;
    }
    return 0;
}

int get_age(string b_day)
{   if (b_day.size() == 6) {
    
        /**
         *  0:  day
         *  1:  month
         *  2:  year
         */
        int b_day_array[3];
        
        
        string b_day_part;
        // get parts of birthday
        for (int i = 0; i < b_day.size() - 1; i += 2) {
            b_day_part      = b_day[i];
            b_day_part      += b_day[i + 1];
            b_day_array[i / 2]   = atoi(b_day_part.c_str());
        }
        b_day_array[2] += 1900;
        
        if (b_day_array[1] < 12 ||
            b_day_array[1] > 1 ||
            b_day_array[0] > 1 ||
            b_day_array[0] < 31) {
            time_t the_time = time(NULL);
            struct tm* a_time = localtime(&the_time);
            //  get current date
            int day = a_time->tm_mday,
                month = a_time->tm_mon + 1,
                year = a_time->tm_year + 1900;
            //  Calculate Age
            if (year == b_day_array[2]) {
                return 0;
            }
            else{
                if (month > b_day_array[1]) {
                    return year - b_day_array[2];
                }
                else if(month < b_day_array[1]){
                    return year - b_day_array[2] - 1;
                }
                else{
                    if (day >= b_day_array[0]) {
                        return year - b_day_array[2];
                    }
                    else{
                        return year - b_day_array[2] - 1;
                    }
                }
            }
        }
        else{return -1;}
    }
    else{return -1;}
}

bool password_regex_match(string password)
{
    regex rex_password("[a-z].*[0-9].*");
    bool match = false;
    if (regex_match(password, rex_password))
    {
        match = true;
    }
    return match;
}

void * part_1(void * sections)
{   string message, sub_message, b_day_message;
    regex   rex_name("[a-zA-Z]*"),
            rex_initial("[A-Z]"),
            rex_num("[0-9]*[ ]*");
    
    /**
     *  0:  Last name
     *  1:  First name
     *  2:  Middle initial
     *  3:  Age
     */
    string  info_list[4],
            detail[4] = {   "Last Name: ",
                            "First Name: ",
                            "Middle Initial: ",
                            "Age: "
            };
    if (sections)
    {   message = (char*) sections;
        istringstream ss(message);
        while (ss)
        {   getline(ss, sub_message, ' ');
            if (sub_message.size() != 0)
            {   switch (sub_message.at(0))
                {   case 'F':
                    {   sub_message.erase(sub_message.begin());
                        info_list[1] = regex_match(sub_message, rex_name) ? sub_message : "";
                    }
                        break;
                    case 'M':
                    {   sub_message.erase(sub_message.begin());
                        info_list[2] = regex_match(sub_message, rex_initial) ? sub_message : "";
                    }
                        break;
                    case 'L':
                    {   sub_message.erase(sub_message.begin());
                        info_list[0] = regex_match(sub_message, rex_name) ? sub_message : "";
                    }
                        break;
                    case 'B':
                    {   sub_message.erase(sub_message.begin());
                        if (regex_match(sub_message, rex_num)) {
                            int age = get_age(sub_message);
                            if (age >= 0)
                            {   info_list[3] = to_string(age);
                            }
                        }
                    }
                        break;
                    default:
                        break;
                }
            }
            
        }
        // print out the information
        string s;
        int i;
        while(1)
        {
            if (semaphore == 1)
            {   cout << "Part 1:" << endl;
                if( info_list[0] == "" &&
                    info_list[1] == "" &&
                    info_list[2] == "" &&
                    info_list[3] == "")
                {
                    cout << "\tInvalid Input" << endl;
                    semaphore++;
                    all_good = false;
                    return NULL;
                }
                
                for (i = 0; i < 4; i++)
                {   s = detail[i];
                    //  checks if the key has a value
                    if (info_list[i].size() == 0)
                    {   s += "missing value";
                    }
                    else
                    {   s += info_list[i];
                    }
                    printf("\t%s\n", s.c_str());
                }
                semaphore++;
                break;
            }
        }
    }
    return NULL;
}

void * part_2(void * sections)
{   string  input = (char*)sections,
            s_number, outlier;
    double stan_dev = 0, avg = 0;
    istringstream ss(input);
    vector<double> numbers, outliers;
    
    if (!regex_match(input, regex("[0-9, ]*"))) {
        while (1) {
            if(semaphore == 2){
                cout << "Part 2:" << endl;
                cout << "\tInvalid Input\n"<< endl;
                all_good = false;
                semaphore++;
                return NULL;
            }
        }
    }
    while (getline(ss, s_number, ','))
    {   double num = atoi(s_number.c_str());
        numbers.push_back(num);
        //  sumation of all numbers
        avg += num;
    }
    
    //  average found
    avg /= numbers.size();
    
    // find Standard deviation
    for (vector<double>::iterator it = numbers.begin(); it != numbers.end(); it++)
    {   double d = pow(avg - *it, 2);
        stan_dev += d;
    }
    stan_dev /= numbers.size();
    stan_dev = sqrt(abs(stan_dev));
    
    //  find outliers
    double outlier_test;
    for (vector<double>::iterator it = numbers.begin(); it != numbers.end(); it++)
    {   outlier_test = abs(*it - avg);
        outlier_test /= stan_dev;
        if (outlier_test > 2) {
            outliers.push_back(*it);
        }
    }
    while(1)
    {   if (semaphore == 2)
    {   cout << "Part 2:" << endl;
            //  print the numbers
            printf("\tNumbers:\n");
            for (vector<double>::iterator it = numbers.begin(); it != numbers.end(); it++)
                printf("\t\t%s\n", to_string(*it).c_str());
            
            //  average
            printf("\tAverage: %f\n", avg);
            //  Standard Deviation
            printf("\tStandard Deviation: %f\n", stan_dev);
            //  print outliers:
            printf("\tOutliers:\n");
            for (vector<double>::iterator it = outliers.begin(); it != outliers.end(); it++)
                printf("\t\t%s", to_string(*it).c_str());
        cout << "\n" << endl;
            semaphore++;
            break;
        }
    }
    return NULL;
}

void * part_3(void * sections)
{   /**
     *  Rules for Password:
     *      1.  Six Characters long
     *      2.  Characters are only lowercase or digits
     *      3.  Starts with a lowercase abc... 
     *      4.  Includes atleast one digit
     *      5.  Each alphabet letter is at beginning of a token
     *      6.  Each Digit is spelled-out
     *      7.  tokens that represent a digit, or start with
     *          capital cannot be used for abc letters
     *      8.  If length of descrambled > 6, use first 6 
     *          characters, unless it produces an invalid 
     *          password, then use last 6 characters.
     *      9.  If both produce an invalid password, then it is
     *          Invalid. 
     *      10. print the result, indicating whether it is valid
     */
    //  strings sent to console
    vector<string> console_outs;
    if (sections)
    {
        //  map between spelling of number with actual number
        typedef map<string, char> Map;
        const Map::value_type x[] =
            {   make_pair("zero",   '0'),
                make_pair("one",    '1'),
                make_pair("two",    '2'),
                make_pair("three",  '3'),
                make_pair("four",   '4'),
                make_pair("five",   '6'),
                make_pair("six",    '7'),
                make_pair("seven",  '8'),
                make_pair("nine",   '9'),
            };
        const Map num_map(x, x + sizeof x / sizeof x[0]);
        
        
        regex   rex_init ("[a-z]+"); //  regex allowing only lowercase letters
        //  @message holds passed paramenter
        string  message = (char *) sections,
                token_heads = "", //  holds all the heads[a-z] and [0-9] of each token
                l_token_heads = "",
                r_token_heads = "",
                token,
                out_message;
        istringstream ss(message);

        //  parse
        while (getline(ss, token, ' '))
        {
            if (regex_match(token, rex_init))
            {
                
                char th = num_map.find(token) != num_map.end()
                        ? num_map.find(token)->second : token[0];
                token_heads += th;
            }
        }
        if (token_heads.size() > 6)
        {
            l_token_heads = token_heads.substr(0, 6);
            r_token_heads = token_heads.substr(token_heads.size() - 6, 6);
            if (password_regex_match(l_token_heads))
            {   //  print password; say Valid
                out_message = "Valid:\t";
                out_message += l_token_heads;
                console_outs.push_back(out_message);
            }
            else
            {   //  print password; say not Valid
                out_message = "Invalid :\t";
                out_message += l_token_heads;
                console_outs.push_back(out_message);
                
            }
            if (password_regex_match(r_token_heads))
            {   //  print password; say Valid
                out_message = "Valid:\t";
                out_message += r_token_heads;
                console_outs.push_back(out_message);
            }
            else
            {   out_message = "Invalid :\t";
                out_message += r_token_heads;
                console_outs.push_back(out_message);
            }
        }
        else if (token_heads.size() == 6)
        {
            if(password_regex_match(token_heads))
            {   // print the password, say it's Valid
                out_message = "Valid:\t";
            }
            else
            {   //  print it, say it's not Valid
                out_message = "Invalid :\t";
            }
            out_message += token_heads;
            console_outs.push_back(out_message);
            
        }
        else if(token_heads.size() < 6 && token_heads.size() > 0)
        {   //  print password, say not Valid
            out_message = "Invalid :\t";
            out_message += token_heads;
            console_outs.push_back(out_message);
            console_outs.push_back("Above is invalid (#valid tokens < 6)");
            all_good = false;
            
        }
        else
        {
            console_outs.push_back("Invalid Input");
            all_good = false;
        }
        
        
    }
    else
    {   console_outs.push_back("\tInvalid input in part 3");
    }
    while (1)
    {   if (semaphore == 3)
    {   cout << "Part 3:" << endl;
            for (vector<string>::iterator it = console_outs.begin(); it != console_outs.end(); it++)
                printf("\t%s\n", it->c_str());
            
            semaphore++;
            break;
        }
    }
    return NULL;
}

void * thinker(void * sections)
{   string message;
    int orig_chances = chances,
        result;
    pthread_t dissector_id;
    if (!all_good) {
        chances--;
    }
    all_good = true;
    printf("[%d chances remaining]\nEnter a character string(press 'Return' to complete):\n", chances);
    //  get get console input
    getline(cin, message);
        if (message.find("****") == string::npos)
        {   string input_element = message;
            //  Check for ***
            int location = (int)message.find("***");
            if (location != string::npos)
            {   //  remove the *** found at location
                input_element.replace(location, 3, "");
                //  check if the string still contains *** after removal
                if (input_element.find("***") != string::npos)
                {   chances--;}
                else
                {   //  Check for **
                    location = (int)input_element.find("**");
                    if (location != string::npos)
                    {   //  remove the ** found at location
                        input_element.replace(location, 2, "");
                        //  check if the string still contains ** after removal
                        if (input_element.find("**") != string::npos)
                        {   chances--;}
                        else
                        {   //  Check for *
                            location = (int)input_element.find("*");
                            if (location != string::npos)
                            {   //  remove the * found at location
                                input_element.replace(location, 2, "");
                                //  check if the string still contains * after removal
                                if (input_element.find("*") != string::npos)
                                {   chances--;}
                                else
                                {   //  reset chances
                                    chances = 3;
                                    //  Create Dissector thread
                                    result = pthread_create(&dissector_id, NULL, disector, (void*) message.c_str());
                                    if (result == 0)
                                    {   printf("<Dissector Thread Created>\n");
                                        //  join dissector thread
                                        result = pthread_join(dissector_id, NULL);
                                        if (result == 0)
                                        {   printf("<Dissector Thread Joined>\n");}
                                        else
                                        {   printf("<Dissector Thread Not Joined>\n");}
                                    }
                                    else
                                    {   printf("<Dissector Thread Not Created>\n");}
                                }
                            }
                            else
                            {   chances--;}
                        }
                    }
                    else
                    {   chances--; }
                }
            }
            else
            {   chances--;}
            
           
            
            

        }
        else
        {chances--;
        }
    //  check of chances is less that orginal chances
    if (chances < orig_chances)
    {   if (chances == 0)
        {   cout << "Sorry, Mate. You have ran out of chances" << endl;
            return NULL;
        }
        int i = 0;
        while (i < 5) {
            cout << "|-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-|" << endl;
            i++;
        }
        cout << "Sorry, Mate. You have inadequate input. Try again" << endl;
        semaphore = 0;
        thinker(sections);
    }
    else
    {   chances = 3;
        semaphore = 0;
        int i = 0;
        while (i < 5) {
            cout << "|-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-|" << endl;
            i++;
        }

        thinker(sections);
        
    }
    return NULL;
}


void * disector(void * section)
{   string  message,
            sub_message_1,
            sub_message_2,
            sub_message_3,
            temp_message;
    int result;
    //  vector holding pthread ids
    vector<pthread_t> threads;
    //  vector holding all lines going to output
    vector<string> console_outs;
    
    if (section)
    {   message = (char*) section;
        temp_message = message;
        message.erase(message.begin(), message.begin() + message.find('*'));
        
        // Part 3
        int star_3 = (int)message.find("***"),
            star_2,
            star_1;
        temp_message.replace(star_3, 3, "___");
        
        star_2 = (int)temp_message.find("**");
        temp_message.replace(star_2, 2, "__");
        
        star_1 = (int)temp_message.find("*");
        if (regex_match(message, regex("[* ]*"))) {
            cout <<"Invalid Input; you only Entered asterisks and spaces" << endl;
            chances --;
            return NULL;
        }
        if (star_3 == 0)
        {   message.erase(message.begin(), message.begin() + 3);
            sub_message_3 = message.substr(0, message.find('*'));
            message.erase(message.begin(), message.begin() + message.find('*'));
        }
        else if (star_3 < star_2 || star_3 < star_1)
        {   message.erase(message.begin() + star_3, message.begin() + star_3 + 3);
            sub_message_3 = message.substr(star_3, message.find('*', star_3) - star_3);
            message.erase(message.begin() + star_3, message.begin() + message.find('*', star_3));
        }
        else
        {   message.erase(message.begin() + star_3, message.end());
            sub_message_3 = message.substr(star_3, message.size());
            message.erase(message.begin() + star_3, message.end());
        }
        if (sub_message_3[0] == ' ')
        {   sub_message_3.erase(0, 1);
        }
        pthread_t part_3_id;
        result = pthread_create(&part_3_id, NULL, part_3, (void *) sub_message_3.c_str());
        threads.push_back(part_3_id);
        if (result == 0)
            printf("<thread %d, %s, created>\n", 3, to_string((long)part_3_id).c_str());
        else
        {   printf("<thread %d not created>\n",3);}
        
        // Part 2
        star_2 = (int)message.find("**");
        temp_message = message;
        temp_message.replace(star_2, star_2 + 2, "__");
        
        star_1 = (int)temp_message.find("*");
        
        if (star_2 == 0)
        {  message.erase(message.begin(), message.begin()+ 2);
            sub_message_2 = message.substr(0, message.find('*'));
            message.erase(message.begin(), message.begin() + message.find('*'));
        }
        else
        {   message.erase(message.begin() + star_2, message.begin() + star_2 + 2);
            sub_message_2 = message.substr(star_2, message.size());
            message.erase(message.begin() + star_2, message.end());
        }
        if (sub_message_2[0] == ' ')
        {   sub_message_2.erase(0, 1);
        }

        pthread_t part_2_id;
        result = pthread_create(&part_2_id, NULL, part_2, (void *) sub_message_2.c_str());
        threads.push_back(part_2_id);
        if (result == 0)
        {   printf("<thread %d, %s, created>\n", 2, to_string((long)part_2_id).c_str());
        }
        else
        {   printf("<thread %d not created>\n", 2);
        }
        
        //  Part 1
        message.erase(message.begin(), message.begin()+1);
        sub_message_1 = message;
        if (sub_message_1[0] == ' ')
        {   sub_message_1.erase(0, 1);
        }
        pthread_t part_1_id;
        result = pthread_create(&part_1_id, NULL, part_1, (void *) sub_message_1.c_str());
        threads.push_back(part_1_id);
        if (result == 0)
        {   printf("<thread %d, %s, created>\n", 1, to_string((long)part_3_id).c_str());
        }
        else
        {   printf("<thread %d not created>\n",1);
        }
            semaphore++;
        
        
        //  Join the created Threads
        for (vector<pthread_t>::iterator it = threads.begin(); it != threads.end(); it++) {
            result = pthread_join(*it, NULL);
            if (result == 0) { printf("<thread %s joined>\n", to_string((long)*it).c_str());}
            else{printf("<thread %s not joined>\n", to_string((long)*it).c_str());}
        }
    }
    return NULL;
}