#include <iostream>

class rooms {

    int length;
    std::string mainCommand;
    std::string subCommand;
    std::string argument;

    int initial=2500;

public:

    rooms () {

	length=0;
	head=NULL;
	tail=NULL;
    }
    
    enum class roomType { main_hall, exam_hall, lecture_room, tutorial_room, design_studio, meeting_room };

    struct room {

	roomType type;
	int ID;
	int capacity;
	room* next;
	room* prev;
    };

    void start (std::string);

    room* returnPointer ();

    void make();

    void deleteRoom (int);

    void update (int);

    void show ();
    
    void upload ();

    void load ();

    void help ();

    bool isFull () { return length>=20; }

    bool isRoomIdCorrect (int, roomType&);

    void showByType (std::string);

    roomType textToType (std::string);

    std::string typeToText (roomType);

    std::string modeToOutput (std::string);

    std::string toLower (std::string);
    
    int returnID ();

    roomType returnType ();

    static std::string stringParser (std::string&, std::string&, std::string&);

    static bool stringToInt (std::string, int&);

    static bool validate (int &value);

private:

    room* head;
    room* tail;
};
