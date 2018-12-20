#include <sstream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include "shell.h"
#include "users.h"

users user;
rooms manage;

int main () {

    shell start;
    start.mainShell();
}

// starting point of the application
void shell::mainShell () {

    user.load();
    manage.load();
    load();
    std::string typed;
    int id_place;

    while (users::toLower(mainCommand)!="exit") {

	std::cout<<modeToOutput(currentMode);
	std::string forLower;
	std::getline(std::cin, forLower);
	typed=users::toLower(forLower);
	mainCommand = rooms::stringParser(typed, subCommand, argument);

	if (mainCommand=="help") { help(); }
	else if (mainCommand=="clear") { system("clear"); }
	else if (mainCommand=="login") { 

	    if (rooms::stringToInt(subCommand, id_place)) { login(id_place); }
	    else { login(0); }
	}
	else if (mainCommand=="exit") { user.upload(); std::cout<<"\nshutting down services...\n"; system("killall -9 bash"); }
	else if (mainCommand=="logout") { logout(); }
	else if (mainCommand=="signup") { signUp (); }
	else if (mainCommand=="delete") { 
	    if (rooms::stringToInt(subCommand, id_place)) { deleteMember (id_place); }
	    else { deleteMember(0); }
	}
	else if (mainCommand=="show") { displayMembers(); }
	else if (mainCommand=="update") { 
	    if (rooms::stringToInt(subCommand, id_place)) { updateMember(id_place); }
	    else { updateMember(0); }
	}
	else if (mainCommand=="book") { start(); std::cout<<"\n"; }
	else if (mainCommand=="manage") { roomManagement (); std::cout<<"\n"; }
	else if (mainCommand=="") { }
	else { std::cout<<"\n'"<<mainCommand<<"' is not a valid command, type 'help' for command instructions\n\n"; }
    }
}

void shell::login (int id) {

    int ID;
    std::string password;
    int attempt=3;
    bool isLogged=false;
    bool oneTime =false;
     if(currentMode==modes::loggedIn) {

	 std::cout<<"\nYou have to logout first\n\n";
	 return;
     }

    while (attempt>0 && isLogged==false && oneTime==false) {

	if (id==0) {
	    std::cout<<std::endl;
	    std::cout<<"    Enter your ID: ";
	    std::cin>>ID;
	    users::validate(ID);	
	    std::cout<<"    Enter your Password: \033[30;40m";
	    std::cin>>password;
	    std::cout<<"\033[0m";
	}
	else { 
	    ID=id;
	    std::cout<<"\n    Enter your Password: \033[30;40m";
	    std::cin>>password;
	    oneTime=true;
	    std::cout<<"\033[0m";
	}

    	if(user.authentication(ID, password)) {
	
	    //system ("clear");
	    std::cout<<std::endl<<"Logged in Successfully!"<<std::endl;
	    std::cout<<std::endl;
	    isLogged=true;
	    currentMode= modes::loggedIn;
    	}
    	else {

	    std::cout<<std::endl;
	    std::cout<<"Credentials are not found in the system\n"<<std::endl;
	    attempt=attempt-1;
	    if (id==0) {

		if (attempt==0) { std::cout<<"Attempts exhausted"<<std::endl; }
		else { std::cout<<"Attempts left: "<<attempt<<std::endl; }
	    }
    	}
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// goes to manage mode
void shell::roomManagement (){

    if (user.currentPrivilege==users::accountType::administrator) {

	manage.start(user.currentUser);
    }
    else {

	std::cout<<"\nYou do not have the privileges\n";
	return;
    }
}

void shell::logout () {

    if (user.currentPrivilege==users::accountType::none) {

	std::cout<<"\nThere are no accounts logged in\n\n";
	return;
    }

    user.currentUser="none";
    currentMode= modes::loggedOut;
    user.currentPrivilege= users::accountType::none;
    std::cout<<std::endl<<"Logged Out!"<<std::endl<<std::endl;
}

void shell::signUp () {

    if (user.currentPrivilege!=users::accountType::administrator) {

	std::cout<<"\nYou do not have the privileges\n\n";
	return;
    }
    user.insert("","","","", users::accountType::student);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void shell::updateMember (int id) {

    int ID;

    if (user.currentPrivilege!=users::accountType::administrator) {

	std::cout<<"\nYou do not have the privileges\n\n";
	return;
    }

    if(id==0) {

	std::cout<<"\n\n    Enter the ID of account being updated: ";
	std::cin>>ID;
    } else { ID=id; }

    user.update(ID);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void shell::deleteMember (int id) {

    if (user.currentPrivilege!=users::accountType::administrator) {

	std::cout<<"\nYou do not have the privileges\n\n";
	return;
    }

    int ID;
    std::string decision;
  
    if (id==0) {
	std::cout<<"\n\n    Enter ID to be deleted: ";
	std::cin>>ID;
	users::validate(ID);
    } else {ID=id; }
    std::cout<<"\n    Are you sure? (yes/no): ";
    std::cin>>decision;

    if (decision[0]=='y') {

	if (user.deleteuser(ID)) {
	    
	    std::cout<<"\nSuccessfully Deleted!\n\n";
	}
	else { std::cout<<"\nUser not found in the system\n\n"; }
    }
    else {

	std::cout<<"\n    Operation cancelled!\n\n";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void shell::displayMembers () {

    if (user.currentPrivilege!=users::accountType::administrator) {

	std::cout<<"\nYou do not have the privileges\n\n";
	return;
    }

    std::cout<<"\n\n";
    user.show();
    std::cout<<"\n\n";
}
    
void shell::help () {

    if (user.currentPrivilege==users::accountType::none) {

	std::cout<<"\nYou do not have the privileges, you have to login first\n\n";
	return;
    }
    std::cout<<std::endl<<"    List of commands ---\n"<<std::endl;
    std::cout<<"    show        Show all the members"<<std::endl;
    std::cout<<"    login [p]   Login using predetermined accounts"<<std::endl;
    std::cout<<"    logout      Logout from the current session"<<std::endl;
    std::cout<<"    signup      Sign Up a new account for new members"<<std::endl;
    std::cout<<"    update [p]  Update an existing account"<<std::endl;
    std::cout<<"    delete [p]  Delete an existing account"<<std::endl;
    std::cout<<"    manage      Enter room management"<<std::endl;
    std::cout<<"    book        Enter booking mode"<<std::endl;
    std::cout<<"    exit        Exit the service"<<std::endl;
    std::cout<<std::endl;
}

// shell prompt
std::string shell::modeToOutput (modes current) {

    switch (current) {

	case modes::loggedOut:

	    return "\033[1;94mAU\033[1;92mAF-\033[1;31mSERVICE \033[1;93m[] \033[1;97m~: \033[0m";
	    break;
	case modes::loggedIn:

	    return std::string("\033[1;94mAU\033[1;92mAF\033[1;97m-\033[1;31mSERVICE \033[1;93m[")+user.currentUser+std::string("] \033[1;97m~: \033[0m");
	    break;
    }
}

//===============================================================================================================

// starting point of booking mode
void shell::start () {

    if (currentMode==modes::loggedOut) {

	std::cout<<"\nYou have to login first\n\n";
	return;
    }

    std::cout<<"\nEntering Booking ... \n\n";
    std::string typed;
    int id_place;

    while (1) {

	std::cout<<modeToOutput(user.currentUser);

	std::string forLower;
	std::getline(std::cin, forLower);
	typed=users::toLower(forLower);
	mainCommand = rooms::stringParser(typed, subCommand, argument);
	
	if (mainCommand=="make") { make(); }
	else if (mainCommand=="back") { upload(); break;}
	else if (mainCommand=="clear") { system ("clear");}
	else if (mainCommand=="delete") {
	    time_t time;
	    if (dateParser(subCommand, time)) { deletebooking(subCommand); }
	    else {deletebooking("none"); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); }
	}
	else if (mainCommand=="help") { book_help(); }
	else if (mainCommand=="show") { 

	    if (subCommand=="user") { showByUser(argument); }
	    else if (subCommand=="room") { showByRoom(argument); }
	    else if (subCommand=="date") { showByDate (argument); }
	    else { show(); }
       	}
	else if (mainCommand=="update") { 
	    
	    if (rooms::stringToInt(subCommand, id_place)) {

		update(id_place);
	    }else { update(0); }
	}
	else if (mainCommand=="help") { }

	else if (mainCommand.length()==0) { }
	else { std::cout<<"\n'"<<mainCommand<<"' is not a valid command, type 'help' for command instructions\n\n"; }

	mainCommand="";
	subCommand="";
    }
}

void shell::make () {

   int userID=user.currentID, roomID, numPeople, start_hour, start_minute, end_hour, end_minute;
   std::string organizer, start_daytime, end_daytime, sType;
   rooms::roomType roomType;

   bool isConfirmed = false;

   if (isFull()) {

       std::cout<<std::endl<<"    No more than 20 booking records are allowed at the same time\n"<<std::endl;
       return;
   }

   while (isConfirmed==false) {

       std::cout<<"\n    What type of room do you want: ";
       std::cin>>sType;
       manage.showByType(sType);
       std::cout<<"\n    Enter Room ID: ";
       std::cin>>roomID;
       users::validate(roomID);
       roomIDcheck(roomID);

       std::cout<<"\n    Number of people: ";
       std::cin>>numPeople;
       users::validate(numPeople);
       std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

       timeParser("\n    Enter Starting Time: ", "", start_hour, start_minute,start_daytime, true);
       timeParser("\n    Enter Ending Time: ", "", end_hour, end_minute,end_daytime, true);

       std::cout<<"\n    The Organizer: ";
       std::cin>>organizer;
       std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

       std::cout<<std::endl<<"    Info -----\n"<<std::endl;
       std::cout<<std::endl<<"       User ID- "<<userID<<" : Room ID- "<<roomID<<" : "<<numPeople<<" participants : Start- "<<start_hour<<":"<<start_minute<<" "<<start_daytime<<" : End- "<<end_hour<<":"<<end_minute<<" "<<end_daytime;

       std::cout<<"\n\n    are you sure? (yes/no): ";
       std::string decision;
       std::cin>> decision;

       if (decision[0]=='y') {

	   insert(userID, roomID, numPeople, start_hour, start_minute, start_daytime, end_hour, end_minute, end_daytime, organizer);
	   break;
       }

       std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
   }

   initial++;
   length++;
   std::cout<<"\nSuccessfully Created!\n\n";
   std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void shell::update (int ID=0) {

    int id;
    rooms::roomType roomType;
    std::string sTemp;
    std::string decision;
    if (ID==0) {

	std::cout<<"\n    Enter log ID:  ";
	std::cin>>id;
	users::validate(id);
    } else { id=ID;}

    booking* temp = head;

    while (temp!=NULL) {

	if (temp->bookID==id) {

	    std::cout<<"\n\n    What would you like to be changed? (RoomID/Participants/Start/End): ";
	    std::cin>>decision;
	    std::string lowered=users::toLower(decision);
	    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	    if (lowered[0]=='r' && lowered[1]=='o' && lowered[2]=='o') {

		std::cout<<"\n    RoomID: ";
		std::cin>>id;
		roomIDcheck(id);
		temp->roomID=id;
		temp->userID=user.currentID;
		std::cout<<"\nSuccessfully Updated!\n\n";
		return;
	    }
	    else if (lowered[0]=='p' && lowered[1]=='a' && lowered[2]=='r') {

		std::cout<<"    Participants: ";
		std::cin>>temp->numPeople;
		users::validate(temp->numPeople);
		temp->userID=user.currentID;
		std::cout<<"\nSuccessfully Updated!\n\n";
		return;
	    }
	    else if (lowered[0]=='s' && lowered[1]=='t' && lowered[2]=='a') {

		timeParser("\n    Starting Time: ", "", temp->start_hour, temp->start_minute,temp->start_daytime, true);
		temp->userID=user.currentID;
		std::cout<<"\nSuccessfully Updated!\n\n";
		return;
	    }
	    else if (lowered[0]=='e' && lowered[1]=='n' && lowered[2]=='d') {

	        timeParser("\n    Enter Ending Time: ", "", temp->end_hour, temp->end_minute, temp->end_daytime, true);
		temp->userID=user.currentID;
		std::cout<<"\nSuccessfully Updated!\n\n";
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

void shell::deletebooking (std::string text) {

    if (!(user.currentPrivilege==users::accountType::administrator)) { std::cout<<"\nYou do not have the privileges\n\n"; return; }
    std::string sTemp;

    if (text=="none") {
    std::cout<<"\n    Delete Until (dd/mm/yyyy): ";
    std::cin>>sTemp;
    } else { sTemp = text; }

    time_t time;

    booking* temp = head;
    booking* prev;

    if (dateParser(sTemp, time)) {

	while (temp!=NULL) {

	    tm* sTime=localtime(&temp->time);
	    sTime->tm_hour=0; sTime->tm_min=0; sTime->tm::tm_sec=0;
	    time_t time2 = mktime(sTime);

	    if (time>time2) {

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

		temp=temp->next;
	    }
	    else {

		temp=temp->next;
	    }
	}
	std::cout<<"\nDeleted Successfully!\n\n";
    }
    else {

	std::cout<<"\nPlease check your date format (dd/mm/yyyy)\n\n";
    }
}

void shell::book_help () {

    std::cout<<std::endl<<"    List of commands ---\n"<<std::endl;
    std::cout<<"    show [p]      Show all the bookings"<<std::endl;
    std::cout<<"       -room [p]  Show by room"<<std::endl;
    std::cout<<"       -user [p]  Show by type"<<std::endl;
    std::cout<<"       -date [p]  Show by date"<<std::endl;
    std::cout<<"    make          Make a new room booking"<<std::endl;
    std::cout<<"    update [p]    Update an existing booking"<<std::endl;
    std::cout<<"    delete [p]    Delete an existing booking"<<std::endl;
    std::cout<<"    back          Go back to main mode"<<std::endl;
    std::cout<<std::endl;
}
void shell::show () {

    booking* temp = head;

    if (length!=0) {

	std::cout<<std::endl<<"    Info -----\n"<<std::endl;
	while (temp!=NULL) {

	    std::cout<<"       Log("<<temp->bookID<<") User- "<<temp->userID<<" : Room ID- "<<temp->roomID<<" : "<<temp->numPeople<<" participants : Start- "<<temp->start_hour<<":"<<temp->start_minute<<" "<<temp->start_daytime<<" : End- "<<temp->end_hour<<":"<<temp->end_minute<<" "<<temp->end_daytime<<" : "<<temp->organizer<<" : RegDate- "<<std::ctime(&temp->time)<<"\n\n";

	    temp=temp->next;
	}
    }
    else {

	std::cout<<"\nThere are no booking records\n\n";
    }
}

void shell::showByUser (std::string arg) {

    if (!(user.currentPrivilege==users::accountType::administrator)) { std::cout<<"\nYou do not have the privileges\n\n"; return; }
    int iTemp;
    if (!rooms::stringToInt(arg, iTemp)) {
	std::cout<<"\n    Enter User ID: ";
	std::cin>>iTemp;
	users::validate(iTemp);
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    if (length==0) { std::cout<<"\nThere are no booking records\n\n"; return; }

    else {

	std::cout<<"\n    Search Result -----\n\n";
	booking* temp =head;
	while (temp!=NULL) {

	    if (temp->userID==iTemp) {

		std::cout<<"       Log("<<temp->bookID<<") User- "<<temp->userID<<" : Room ID- "<<temp->roomID<<" : "<<temp->numPeople<<" participants : Start- "<<temp->start_hour<<":"<<temp->start_minute<<" "<<temp->start_daytime<<" : End- "<<temp->end_hour<<":"<<temp->end_minute<<" "<<temp->end_daytime<<" : "<<temp->organizer<<" : RegDate- "<<std::ctime(&temp->time)<<"\n\n";
		temp=temp->next;
	    }
	    else {

		temp=temp->next;
	    }
	}
    }    
}

void shell::showByRoom (std::string arg) {

    if (!(user.currentPrivilege==users::accountType::administrator)) { std::cout<<"\nYou do not have the privileges\n\n"; return; }
    int iTemp;
    if (!rooms::stringToInt (arg, iTemp)) {
	std::cout<<"\n    Enter Room ID: ";
	std::cin>>iTemp;
	users::validate(iTemp);
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    if (length==0) { std::cout<<"\nThere are no booking records\n\n"; return; }

    else {
	std::cout<<"\n    Search Result -----\n\n";
	booking* temp =head;
	while (temp!=NULL) {

	    if (temp->roomID==iTemp) {

		std::cout<<"       Log("<<temp->bookID<<") User- "<<temp->userID<<" : Room ID- "<<temp->roomID<<" : "<<temp->numPeople<<" participants : Start- "<<temp->start_hour<<":"<<temp->start_minute<<" "<<temp->start_daytime<<" : End- "<<temp->end_hour<<":"<<temp->end_minute<<" "<<temp->end_daytime<<" : "<<temp->organizer<<" : RegDate- "<<std::ctime(&temp->time)<<"\n\n";
		temp=temp->next;
	    }
	    else {

		temp=temp->next;
	    }
	}
    }
}

void shell::showByDate (std::string arg) {

    time_t time;
    std::string sTemp;

    if (!dateParser(arg, time)) { 
	std::cout<<"\n    Date (dd/mm/yyyy): ";
	std::cin>>sTemp; 
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } else { sTemp=arg; }

    if (dateParser(sTemp, time)) {

	std::cout<<"\n    Search Result -----\n\n";
	booking* temp=head;
	while (temp!=NULL) {

	    tm* sTime=localtime(&temp->time);
	    sTime->tm_hour=0; sTime->tm_min=0; sTime->tm::tm_sec=0;
	    time_t time2 = mktime(sTime);
	    if (time==time2) {

		std::cout<<"       Log("<<temp->bookID<<") User- "<<temp->userID<<" : Room ID- "<<temp->roomID<<" : "<<temp->numPeople<<" participants : Start- "<<temp->start_hour<<":"<<temp->start_minute<<" "<<temp->start_daytime<<" : End- "<<temp->end_hour<<":"<<temp->end_minute<<" "<<temp->end_daytime<<" : "<<temp->organizer<<" : RegDate- "<<std::ctime(&temp->time)<<"\n\n";
		temp=temp->next;
	    }
	    else {

		temp=temp->next;
	    }
	}
    }
    else {

	std::cout<<"\nPlease check your date format\n\n";
    }
}

// takes data int the form of (dd/mm/yy) and parses it into time_t
bool shell::dateParser (std::string string, time_t& time) {

    int day, month, year;
    std::string delimiter="/", capture=string;

    if (!rooms::stringToInt( capture.substr(0, capture.find(delimiter)), day)) { return false; }
    capture.erase(0, capture.find(delimiter) + delimiter.length());

    if (!rooms::stringToInt( capture.substr(0, capture.find(delimiter)), month)) { return false; }
    capture.erase(0, capture.find(delimiter) + delimiter.length());

    if (!rooms::stringToInt(capture, year)) { return false; }

    tm* sTime = new tm();

    sTime->tm_year= year-1900;
    sTime->tm_mon=month-1;
    sTime->tm_mday=day;
    sTime->tm_hour=0;
    sTime->tm_min=0;
    sTime->tm_sec=0;

    time = mktime(sTime);
    return true;
}

// shell prompt
std::string  shell::modeToOutput (std::string username) {

    return std::string("\033[1;94mAU\033[1;92mAF\033[1;97m-\033[1;31mSERVICE \033[1;93m[")+username+std::string("] \033[1;97m(booking): \033[0m");
}

// insertion used for make function
void shell::insert (int userID, int roomID, int numPeople, int start_hour, int start_minute, std::string start_daytime, int end_hour, int end_minute, std::string end_daytime, std::string organizer) {

   booking* temp = new booking ();

   temp->bookID=initial;
   temp->userID=userID;
   temp->roomID=roomID;
   temp->numPeople=numPeople;
   temp->start_hour=start_hour;
   temp->start_minute=start_minute;
   temp->start_daytime=start_daytime;
   temp->end_hour=end_hour;
   temp->end_minute=end_minute;
   temp->end_daytime=end_daytime;
   temp->organizer=organizer;
   temp->time=time(0);

   if (head==NULL && tail==NULL) {

       head=temp;
       tail=temp;
   }
   else {

       tail->next=temp;
       booking* prev =tail;
       tail=temp;
       tail->prev=prev;
   }
}

// check if room exist and whether it can be booked
void shell::roomIDcheck (int& roomID) {

    rooms::roomType roomType;

   while (1) {

       if (!manage.isRoomIdCorrect(roomID,roomType)) {

	   std::cout<<"\nRoom ID is not correct\n\n";

	   std::cout<<std::endl<<"    Enter Room ID again: ";
	   std::cin>>roomID;
	   users::validate(roomID);
       }
       else if (user.currentPrivilege==users::accountType::student) {

	   if (roomType==rooms::roomType::main_hall || roomType==rooms::roomType::lecture_room || roomType==rooms::roomType::tutorial_room){

	       break;
	   }
	   else {

		std::cout<<"\nYou cannot book "<<manage.typeToText(roomType)<<"s\n\n";
		std::cout<<std::endl<<"    Enter another Room ID: ";
		std::cin>>roomID;
		users::validate(roomID);
	   }
       } 
       else { break; }
   }
}

// taking data from stream and giving it to number, used in load function
void shell::fileTonumber (std::stringstream& eachLine, int& number, char delimiter) {

    std::stringstream string_to_id;
    std::string sTemp;

    std::getline (eachLine, sTemp,delimiter);
    string_to_id.str (sTemp);
    string_to_id>>number;
    string_to_id.clear();
}

// takes value as string and parses hour : minute and daytime
void shell::timeParser (std::string message, std::string time, int& hours, int& minutes, std::string& daytime, bool isPrompt) {

    std::string capture, sHours, sMinutes, period, time_delimiter=":", period_delimeter=" ";

    if (isPrompt==true) {

	std::cout<<message;
	std::getline(std::cin, capture);
    }
    else {

	capture=time;
    }

    while (1) {
	
	sHours = capture.substr(0, capture.find(time_delimiter));
	capture.erase(0, capture.find(time_delimiter) + time_delimiter.length());
	sMinutes = capture.substr(0, capture.find(period_delimeter));
	capture.erase(0, capture.find(period_delimeter) + period_delimeter.length());
	period =users::toLower(capture);

	if (isPrompt==true) {
	    if (!rooms::stringToInt (sHours, hours) || !rooms::stringToInt(sMinutes, minutes)) {

		std::cout<<"\nWrong Input\n\n";
		std::cout<<"    Try Again: ";
		getline(std::cin, capture);
	    }
	    else if (period[0]!='a' && period[0]!='p') {

		std::cout<<"\nWrong Period Input\n\n";
		std::cout<<"    Try Again: ";
	    getline(std::cin, capture);
	    }
	    else { 
		
		if (period[0] =='a') { daytime = "AM"; }
		else { daytime = "PM"; }
		break;
	    }
	}
	else {

	    rooms::stringToInt (sHours, hours);
	    rooms::stringToInt (sMinutes, minutes);
	    if (period[0]=='a') { daytime = "AM"; }
	    else { daytime = "PM"; }
	    break;
	}
    }
}

void shell::upload () {

    std::ofstream write ("/home/abid/roomBooking/records.txt");

    if (write.is_open()) {

	booking* temp =head;
	while (temp!=NULL) {

	    write<<temp->bookID<<"-"<<temp->userID<<"-"<<temp->roomID<<"-"<<temp->numPeople<<"-"<<temp->start_hour<<":"<<temp->start_minute<<" "<<temp->start_daytime<<"-"<<temp->end_hour<<":"<<temp->end_minute<<" "<<temp->end_daytime<<"-"<<temp->organizer<<"-"<<temp->time<<"\n";

	    temp=temp->next;
	}
    }
}

void shell::load () {

    std::ifstream read ("/home/abid/roomBooking/records.txt");
    std::string sTemp, line;
    std::stringstream string_to_id;

    while (std::getline(read, line)) {

	booking* temp = new booking();

	std::stringstream eachLine (line);

	fileTonumber(eachLine, temp->bookID, '-');
	initial=temp->bookID+1;

	fileTonumber(eachLine, temp->userID, '-');

	fileTonumber(eachLine, temp->roomID, '-');

	fileTonumber(eachLine, temp->numPeople, '-');

	std::getline (eachLine, sTemp, '-');
	timeParser("", sTemp, temp->start_hour, temp->start_minute, temp->start_daytime, false);

	std::getline (eachLine, sTemp, '-');
	timeParser("", sTemp, temp->end_hour, temp->end_minute, temp->end_daytime, false);
	
	std::getline(eachLine, sTemp, '-');
	temp->organizer=sTemp;

	std::getline(eachLine, sTemp);
	string_to_id.str(sTemp);
	string_to_id>>temp->time;
	string_to_id.clear();

	if (head==NULL && tail==NULL) {

	   head=temp;
	   tail=temp;
        }
        else {

	   tail->next=temp;
	   booking* prev =tail;
	   tail=temp;
	   tail->prev=prev;
        }
	length++;
    }
}

