#include <string>
#include "rooms.h"

class shell {

    enum class modes { loggedOut, loggedIn };

    std::string mainCommand;
    std::string subCommand;
    std::string argument;
    modes currentMode;

public:

    shell () {

	mainCommand="default";
	subCommand="none";
	currentMode=modes::loggedOut;

	// Booking Attributes initialization
	length=0;
	initial=3500;
	head=NULL;
	tail=NULL;

    }

public:

    void mainShell ();

    void login (int);

    void logout ();

    void signUp ();

    void updateMember (int);

    void deleteMember (int);

    void help ();
    
    void displayMembers ();

    void roomManagement ();

    std::string modeToOutput (modes);

    std::string getpass(const char *prompt, bool);

//=================================================================================================

    struct booking {

	int bookID;
	int userID;
	int roomID;
	int numPeople;
	int start_hour;
	int start_minute;
	int end_hour;
	int end_minute;
	time_t time;

	std::string organizer;
	std::string start_daytime;
	std::string end_daytime;
	booking* prev;
	booking* next;
    };

    booking* head;
    booking* tail;

    int length;
    int initial;

    std::string book_mainCommand;
    std::string book_subCommand;

public:

    void start ();

    void make ();

    void update (int);

    void search (std::string);

    void show ();

    void book_help ();

    void deletebooking (std::string);

    void load ();

    void upload ();

    bool isFull () { return length>=50; }

    std::string modeToOutput (std::string username);

    void insert (int, int, int, int, int, std::string, int, int, std::string, std::string);

    void timeParser (std::string, std::string, int&, int&, std::string&, bool);

    void roomIDcheck (int&);

    static void fileTonumber (std::stringstream&, int&, char);

private:

    void showByUser (std::string);

    void showByRoom (std::string);

    void showByDate (std::string);

    bool dateParser (std::string, time_t&);
};
