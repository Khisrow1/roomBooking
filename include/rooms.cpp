#include <limits>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdlib.h>
#include "rooms.h"

// this is where the manage section starts
void rooms::start (std::string username) {

    std::cout<<"\nEntering Room Management... \n\n";
    std::string typed;
    int id_place;

    while (1) {

	std::cout<<modeToOutput(username);

	std::string forLower;
	std::getline(std::cin, forLower);
	typed=toLower(forLower);
	mainCommand = stringParser(typed, subCommand, argument);
	
	if (mainCommand=="make") { make(); }
	else if (mainCommand=="show") { show (); }
	else if (mainCommand=="back") { upload(); break; }
	else if (mainCommand=="clear") { system ("clear");}
	else if (mainCommand=="delete") {

	    if (stringToInt(subCommand, id_place)) { deleteRoom(id_place); }
	    else { deleteRoom(0); }
	}
	else if (mainCommand=="update") {
	   if (stringToInt(subCommand, id_place)) { update(id_place); }
	   else { update(0); }
	}
	else if (mainCommand=="help") { help (); }

	else if (mainCommand.length()==0) { }
	else { std::cout<<"\n'"<<mainCommand<<"' is not a valid command, type 'help' for command instructions\n\n"; }

	mainCommand="";
	subCommand="";
    }
}

// create new entry
void rooms::make() {

   std::string accountTemp;
   std::string decision;
   bool isConfirmed = false;

   if (isFull()) {

       std::cout<<std::endl<<"    No more than 20 rooms are allowed at the same time\n"<<std::endl;
       return;
   }

  room* temp = new room(); 

   while (isConfirmed==false) {

       temp->ID=initial;
       std::cout<<std::endl<<"    Capacity: ";
       std::cin>>temp->capacity;
       validate (temp->capacity);
       std::cout<<"    Room Type: ";
       std::cin>>accountTemp;
       std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
       temp->type=textToType(accountTemp);

       std::cout<<std::endl<<"    Info -----\n"<<std::endl;
       std::cout<<std::endl<<"       ID: "<<temp->ID<<" : "<<temp->capacity<<" people : Type :"<<typeToText(temp->type)<<std::endl<<std::endl;

       std::cout<<"\n    are you sure? (yes/no): ";
       std::cin>> decision;

       if (decision[0]=='y') {

	   if (head==NULL && tail==NULL) {

	       head=temp;
	       tail=temp;
	   }
	   else {

	       tail->next=temp;
	       room* prev =tail;
	       tail=temp;
	       tail->prev=prev;
	   }
	   break;
       }

       std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
   }

   initial++;
   length++;
   std::cout<<"\nSuccessfully Created!\n\n";
   std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

}

void rooms::deleteRoom (int ID=0) {

    int id;

    if (ID==0) {
	std::cout<<"\n    Enter ID of the room: ";
	std::cin>>id;
	validate(id);
    } else { id=ID; }

    room* temp = head;
    room* prev;

    while (temp!=NULL) {


	if (temp->ID==id) {

	    if (temp->prev==NULL) {

		if (temp->next==NULL) {

		    head=NULL;
		    tail=NULL;
		    delete temp;
		    length= length-1;
		    std::cout<<"\nSuccessfully Deleted!\n\n";
		    return;
		}

		prev=temp->next;
		prev->prev=NULL;
		head=prev;
		delete temp;
	    }
	    else if (temp->next==NULL) {

		prev=temp->prev;
		prev->next=NULL;
		tail=prev;
		delete temp;
	    }
	    else {

		prev=temp->prev;
		prev->next=temp->next;
		prev=temp->next;
		prev->prev=temp->prev;
		delete temp;
	    }

	    length= length-1;
	    std::cout<<"\nSuccessfully Deleted!\n\n";
	    return;
	}
	else {

	    temp=temp->next;
	}
    }
    std::cout<<"\nNot found in the system\n\n";
}

void rooms::update (int ID=0) {

    int id;
    std::string sTemp;
    std::string decision;
    if (ID==0) {

	std::cout<<"\n    Enter the ID to be updated: ";
	std::cin>>id;
	validate(id);
    } else { id=ID;}

    room* temp = head;

    while (temp!=NULL) {

	if (temp->ID==id) {

	    std::cout<<"\n\n    What would you like to be changed? (ID/Capacity/Type): ";
	    std::cin>>decision;
	    std::string lowered=toLower(decision);

	    if (lowered=="id") {

		std::cout<<"\nID's are Immutable, therefore it cannot be changed\n\n";
		return;
	    }
	    else if (lowered[0]=='c' && lowered[1]=='a') {

		std::cout<<"    Capacity: ";
		std::cin>>temp->capacity;
		validate(temp->capacity);
		std::cout<<"\nSuccessfully Updated!\n\n";
		return;
	    }
	    else if (lowered[0]=='t' && lowered[1]=='y') {

		std::cout<<"    Type: ";
		std::cin>>sTemp;
		temp->type=textToType(sTemp);
		std::cout<<"\nSuccessfully Updated!\n\n";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return;
	    }
	    else {

		std::cout<<"\nWrong Input!\n\n";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return;
	    }
	}
	else {

	    temp=temp->next;
	}
    }
    std::cout<<"\nNot found in the system\n\n";
}

void rooms::show () {

    if (length==0) {

	std::cout<<"\nThere are no rooms registered\n\n";
	return;
    }
    room* temp = head;
    std::cout<<"\n    Rooms----\n\n";

    while (temp!=NULL) {

	std::cout<<"    ID- "<<temp->ID<<" : "<<temp->capacity<<" people : Type- "<<typeToText(temp->type)<<"\n\n";
	temp=temp->next;
    }
}

// show the commands
void rooms::help () {

    std::cout<<std::endl<<"    List of commands ---\n"<<std::endl;
    std::cout<<"    show        Show all the rooms"<<std::endl;
    std::cout<<"    make        Create a new entry for a room"<<std::endl;
    std::cout<<"    update [p]  Update an existing room"<<std::endl;
    std::cout<<"    delete [p]  Delete an existing room"<<std::endl;
    std::cout<<"    back        Go back to main mode"<<std::endl;
    std::cout<<std::endl;
}

// validate the existence of a room using id as a query
bool rooms::validate (int &value) {

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
	    
	    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	    return true;
	    break;
	}
    }
}

// string to room type coversion
rooms::roomType rooms::textToType (std::string text) {

    std::string lowCase = toLower(text);

    if (lowCase[0]=='m' && lowCase[1]=='a') {

	return roomType::main_hall;
    }
    else if(lowCase[0]=='e' && lowCase[1]=='x') {

	return roomType::exam_hall;
    }
    else if (lowCase[0]=='d' && lowCase[1]=='e') {

	return roomType::design_studio;
    }
    else if (lowCase[0]=='l' && lowCase[1]=='e') {

	return roomType::lecture_room;
    }
    else if (lowCase[0]=='m' && lowCase[1]=='e') {

	return roomType::meeting_room;
    }
    else {

	return roomType::tutorial_room;
    }
}

// room type to string coversion
std::string rooms::typeToText (roomType type) {

    switch (type) {

	case roomType::design_studio:
	    return "Design Studio";
	    break;
	case roomType::exam_hall:
	    return "Exam Hall";
	    break;
	case roomType::lecture_room:
	    return "Lecture Room";
	    break;
	case roomType::main_hall:
	    return "Main Hall";
	    break;
	case roomType::meeting_room:
	    return "Meeting Room";
	    break;
	case roomType::tutorial_room:
	    return "Tutorial Room";
	    break;
    }
}

// shell prompt using ANSI escape code
std::string  rooms::modeToOutput (std::string username) {

    return std::string("\033[1;94mAU\033[1;92mAF\033[1;97m-\033[1;31mSERVICE \033[1;93m[")+username+std::string("] \033[1;97m(manage): \033[0m");
}

std::string rooms::toLower (std::string string) {

    std::string lowered=string;
    std::transform (lowered.begin(), lowered.end(), lowered.begin(), ::tolower);

    return lowered;
}

void rooms::upload () {

    std::ofstream write ("/home/abid/roomBooking/rooms.txt");

    if (write.is_open()) {

	room* temp =head;
	while (temp!=NULL) {

	    write<<temp->ID<<":"<<temp->capacity<<":"<<typeToText(temp->type)<<"\n";
	    temp=temp->next;
	}
    }
}

void rooms::load () {

    std::ifstream read ("/home/abid/roomBooking/rooms.txt");
    std::string sTemp;
    std::stringstream string_to_id;
    std::string line;

    while (std::getline(read, line)) {

	room* temp = new room();

	std::stringstream eachLine (line);

	std::getline (eachLine, sTemp,':');
	string_to_id.str (sTemp);
	string_to_id>>temp->ID;
	initial=temp->ID+1;
	string_to_id.clear();

	std::getline (eachLine, sTemp,':');
	string_to_id.str (sTemp);
	string_to_id>>temp->capacity;
	string_to_id.clear();

	std::getline (eachLine, sTemp,':');
	temp->type=textToType(sTemp);
	
	if (head==NULL && tail==NULL) {

	   head=temp;
	   tail=temp;
        }
        else {

	   tail->next=temp;
	   room* prev =tail;
	   tail=temp;
	   tail->prev=prev;
        }

	length++;
    }
}

// parses the typed command into main and sub command
std::string rooms::stringParser (std::string& input, std::string& sub, std::string& arg) {

    std::string main;
    std::string delimiter = " ";
    std::size_t pos =0;

    main = input.substr(0, input.find(delimiter));
    input.erase(0, input.find(delimiter) + delimiter.length());
    sub = input.substr(0, input.find(delimiter));
    input.erase(0, input.find(delimiter) + delimiter.length());
    arg = input.substr(0, input.find(delimiter));

    return main;
}

// checks if a typed sstring is a number or no, if yes then returns that number
bool rooms::stringToInt (std::string input, int& output) {

    int number;
    std::stringstream stream (input);

    stream>>number;

    if (stream.fail()) {

	return false;
    }
    else {

	output = number;
	return true;
    }
}

bool rooms::isRoomIdCorrect (int ID, roomType& type) {

    room* temp = head;

    while (temp!=NULL) {

	if (temp->ID==ID) {

	    type=temp->type;
	    return true;
	}
	else {

	    temp=temp->next;
	}
    }

    show();
    return false;
}

void rooms::showByType (std::string text) {

    roomType type=textToType(text);
    room* temp = head;

    std::cout<<"\n    Rooms----\n\n";

    while (temp!=NULL) {

	if (temp->type==type) {

	    std::cout<<"    ID- "<<temp->ID<<" : "<<temp->capacity<<" people : Type- "<<typeToText(temp->type)<<"\n\n";
	temp=temp->next;
	}
	else {

	    temp=temp->next;
	}
    }
}
