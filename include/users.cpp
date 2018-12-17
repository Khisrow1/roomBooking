#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <limits>
#include "users.h"


bool users::isFull() const {

    return index>10;
}

void users::insert(std::string user, std::string password, std::string phonenumber, std::string emailaddress, accountType accountType) {

   std::string accountTemp;
   std::string decision;
   bool isConfirmed = false;

   if (isFull()) {

       std::cout<<std::endl<<"    No more than 10 users are allowed at the same time"<<std::endl;
       return;
   }
   
   initial++;

   while (isConfirmed==false) {

       data[index].id=initial;
       std::cout<<std::endl<<"    Enter a Username: ";
       std::cin>>data[index].username;
       std::cout<<"    Enter a Password: ";
       std::cin>>data[index].pass;
       std::cout<<"    Enter Phone Number: ";
       std::cin>>data[index].phone;
       validate(data[index].phone);
       std::cout<<"    Enter Email Address: ";
       std::cin>>data[index].email;
       std::cout<<"    Account Type: ";
       std::cin>>accountTemp;
       data[index].type=textToType(accountTemp);

       std::cout<<std::endl<<"    Info -----\n"<<std::endl;
       std::cout<<std::endl<<"       "<<data[index].id<<" : "<<data[index].username<<" : 0"<<data[index].phone<<" : "<<data[index].email<<" : "<<typeName(index)<<std::endl<<std::endl;

       std::cout<<"\n    are you sure? (yes/no): ";
       std::cin>>decision;

       if (decision[0]=='y') {

	   break;
       }
   }
   std::cout<<"\n    Successfully Created!\n\n";
   index++;
}

bool users::deleteuser (int ID) {

    for (int i=0; i<index; i++) {

	if (ID==10000) {

	    std::cout<<"\n    Administrator account cannot be deleted\n\n";
	    return false;
	}
	else if (data[i].id==ID) {

	    for (int j=i; j<index; j++) {

		data[j]=data[j+1];
	    }

	    index--;
	    return true;
	}
    }

    return false;
}

void users::update (int ID) {

    for (int i=0; i<index; i++) {

	if (data[i].id == ID) {

	    if (ID == 10000) {

		std::cout<<"\n    Administrator account cannot be updated\n\n";
		return;
	    }

	    changeAttr(i);
	    return;
	}
    }

    std::cout<<"\n    Account is not in the system\n\n";
}

// abstraction for for showMembers
void users::show () const {

    showMembers (index-1);
}

// recursion for showing all the members
void users::showMembers (int index) const {

    if (index==-1) {

	return;
    }
    else if (index==0) {

	std::cout<<"       ------------------------------------------------------------------"<<std::endl;
	std::cout<<"       "<<data[index].id<<" : "<<data[index].username<<" : 0"<<data[index].phone<<" : "<<data[index].email<<" : "<<typeName(index)<<std::endl;
    }

    else {

	std::cout<<"       "<<data[index].id<<" : "<<data[index].username<<" : 0"<<data[index].phone<<" : "<<data[index].email<<" : "<<typeName(index)<<std::endl;
	showMembers (index-1);
    }
}

// abstraction for the authentication call
bool users::authentication (int ID, std::string password) {

   return auth (ID, password, index);
} 

// recursion for authentication
bool users::auth (int ID, std::string password, int temp) {

    if (temp==-1) {

	return false;
    }
    else if (data[temp].id==ID && data[temp].pass==password) {

	currentUser=data[temp].username;
	currentID=data[temp].id;
	currentPrivilege=data[temp].type;
	return true;
    }

    else {

	return auth (ID, password, temp-1);
    }
}

// account type to string coversion
std::string users::typeName (int index) const {

    switch (data[index].type) {

	case accountType::administrator:
	    return "Administrator";
	    break;
	case accountType::member:
	    return "Member Account";
	    break;
	case accountType::student:
	    return "Student Account";
	    break;
	case accountType::none:
	    return "invalid type";
	    break;
    }
}

// user validation
bool users::validate (int &value) {

    while (1) {

	std::string sValue = std::to_string(value);

	if (std::cin.fail()) {

	    std::cin.clear();

	    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	    std::cout<<"    Your input is wrong"<<std::endl;
	    std::cout<<std::endl;
	    std::cout<<"    Try Again: ";
	    std::cin>>value;

	}

	else if (sValue[0]=='0') {

	    std::cin.clear();
	    
	    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	    std::cout<<"    You cannot have zero as initial"<<std::endl;
	    std::cout<<std::endl;
	    std::cout<<"    Try Again: ";
	    std::cin>>value;

	}
	else {
	    
	    return true;
	    break;
	}
    }
}

// string to user type conversion
users::accountType users::textToType (std::string text) {

    std::string lowCase = toLower(text);

    if (lowCase[0]=='a' && lowCase[1]=='d' && lowCase[2]== 'm') {

	return accountType::administrator;
    }
    
    else if (lowCase[0]=='m' && lowCase[1]=='e' && lowCase[2]== 'm') {

	return accountType::member;
    }
    else {

	return accountType::student;
    }
}

// string user if file upload
std::string users::typeToText (accountType type) {

    switch (type) {

	case accountType::member:
	    return "mem";
	    break;
	case accountType::student:
	    return "stu";
	    break;
	default:
	    return "none";
	    break;
    }
}

std::string users::toLower (std::string string) {

    std::string lowered=string;
    std::transform (lowered.begin(), lowered.end(), lowered.begin(), ::tolower);

    return lowered;
}

// attribute change, used in user update function
void users::changeAttr (int index) {

    int decision;
    std::string value;
    std::cout<<"\n    Which column would you like to change? (1/2/3/4/5): ";
    std::cin>>decision;
    
    switch (decision) {

	case 1:
	    std::cout<<"\n    ID of an account is immutable\n";
	    changeAttr(index);
	    break;
	case 2:
	    std::cout<<"\n    Enter a new username: ";
	    std::cin>>data[index].username;
	    break;
	case 3:
	    std::cout<<"\n    Enter a new phone number: ";
	    std::cin>>data[index].phone;
	    validate(data[index].phone);
	    break;
	case 4:
	    std::cout<<"\n    Enter a new email: ";
	    std::cin>>data[index].email;
	    break;
	case 5: 
	    std::cout<<"\n    Enter the new account type: ";
	    std::cin>>value;
	    data[index].type=textToType(value);
	    break;
	default:
	    std::cout<<"\nWrong Input\n\n";
	    break;
    }

    std::cout<<"\nSuccessfully updated!\n\n";
}

// file outstreaming
void users::upload () {

    std::ofstream write ("/home/abid/roomBooking/users.txt");

    if (write.is_open()) {

	for (int i=1; i<index; i++) {

	    write<<data[i].id<<":"<<data[i].username<<":"<<data[i].pass<<":"<<data[i].phone<<":"<<data[i].email<<":"<<typeToText(data[i].type)<<"\n";
	}
    }
}

//file instreaming
void users::load () {

    std::ifstream read ("/home/abid/roomBooking/users.txt");
    std::string temp;
    std::stringstream string_to_id;
    std::string line;

    while (std::getline(read, line)) {

	std::stringstream eachLine (line);

	if (index>9) {

	    return;
	}

	std::getline (eachLine, temp,':');
	string_to_id.str (temp);
	string_to_id>>data[index].id;
	string_to_id.clear();

	std::getline (eachLine, data[index].username,':');
	std::getline (eachLine, data[index].pass,':');

	std::getline (eachLine, temp,':');
	string_to_id.str (temp);
	string_to_id>>data[index].phone;
	string_to_id.clear();

	std::getline (eachLine, data[index].email,':');

	std::getline (eachLine, temp,':');
	data[index].type=textToType(temp);
	initial++;
	index++;
    }
}
