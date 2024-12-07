#include <string>
#include <iostream>

class StateMachine{
public:
	virtual void Handle(std::string& action,StateMachine* SM)=0;
};

class Start : public StateMachine{
public:
	void Handle(std::string& action,StateMachine* SM) override{
		if(action=="x1"){
			SM=new Left();
		}
		if(action=="x2"){
			SM=new Right();
		}
	}
};

class Left : public StateMachine{
public:
	void Handle(std::string& action,StateMachine* SM) override{
	}
};

class Right : public StateMachine{
public:
	void Handle(std::string& action,StateMachine* SM) override{
	}
};

