#ifndef RESPONDER_BASE_RESPONDER_H
#define RESPONDER_BASE_RESPONDER_H

#include<iostream> // per i cout di test
#include<fstream> // per i cout di test
#include<unordered_map>

#include<boost/function.hpp>

#include "../json/json.hpp"
using json = nlohmann::json;

class Responder {
public:
    /*
    Responder(std::string s) {
	std::ifstream ifs = std::ifstream(s);
	json j = json::parse(ifs);
	this->follow_json(j);
    }
    */
	
    void respond_to(std::string err_id) {
	(azioni[err_id])(this);
    }

    virtual void follow_json(json j) {
    	for(json error : j["errors"]) {
	    azioni[error["id"]] = fun_from_id(j["fun id"]);
	}
    }
    // virtual void bind(std::string err_id, std::string fun_id) = 0;
    virtual boost::function<void(Responder*)>fun_from_id(std::string s)=0;
private:
    std::unordered_map<std::string,
		       boost::function<void(Responder*)>> azioni;
};

#endif
