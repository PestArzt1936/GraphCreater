#include <string>
#include <iostream>

class StateMachine{
public:
	virtual void Handle(std::string& action,StateMachine* SM)=0;
};

class Closed : public StateMachine{
public:
	void Handle(std::string& action,StateMachine* SM) override{
		if(action=="Open"){
			SM=new Open();
		}
		if(action=="Lock"){
			SM=new Locked();
		}
	}
};

class Open : public StateMachine{
public:
	void Handle(std::string& action,StateMachine* SM) override{
		if(action=="x4"){
			SM=new Closed();
		}
	}
};

class Locked : public StateMachine{
public:
	void Handle(std::string& action,StateMachine* SM) override{
		if(action=="x3"){
			SM=new Closed();
		}
	}
};

