#include <iostream>

class users {

public:
    enum class accountType { member, student , administrator, none};

    struct userData {

	int id;
	std::string username;
	std::string pass;
	int phone;
	std::string email;
	accountType type;
    };

    accountType currentPrivilege;
    std::string currentUser;
    int currentID;

private:

    userData data[10];
    int index;
    int initial;

public:

    users () {

	index=1;
	
	initial=10000;	

	data[0].id=initial;
	data[0].username="admin";
	data[0].phone=1000000;
	data[0].pass="whatisapass";
	data[0].email="--confidential--";
	data[0].type=accountType::administrator;

	currentUser="none";
	currentID=000;
	currentPrivilege=accountType::none;
    }


   void insert (std::string , std::string , std::string , std::string , accountType );

   bool deleteuser (int id);

   bool isFull () const;
   
   bool authentication (int ID, std::string password);

   void show () const;

   void update (int ID);

   void upload ();

   void load ();

   std::string typeName (int) const;

   static std::string toLower (std::string);

   static bool validate (int&);

   static accountType textToType (std::string);

   static std::string typeToText (accountType);

private:

   bool auth (int ID, std::string user, int);

   void showMembers (int) const ;

   void changeAttr (int);
};
