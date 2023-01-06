#ifndef RESPONDER_RESPONDER_BASE_H
#define RESPONDER_RESPONDER_BASE_H

#include<iostream>

// per il responder
#include<unordered_map>
#include<boost/function.hpp>

// per il json
#include<fstream>
#include"../json/json.hpp"
using json = nlohmann::json;

// migliora la sintassi del costruttore
#include<initializer_list>

// Responder è fatto per essere parte di un'altro oggetto
// bene o male il gestore dei callback di una classe a cui serve fare callback
// quindi fatto apposta per essere un wrapped

class Responder_base {
public:
    Responder_base(std::initializer_list
	      <std::pair<const std::string,
	      boost::function<void()>>> init): id2fun(init) {}

    void respond_to(std::string err_id) {
	(err2call[err_id])();
    }
    void bind(std::string err_id, std::string fun_id) {
	err2call[err_id] = id2fun[fun_id];
    }

    void follow_json(json j) {
	for(json error : j["errors"]) {
	    bind(error["id"], error["fun id"]);
	}
    }
private:
    // inizializzata nel costruttore
    std::unordered_map<std::string, boost::function<void()>> id2fun;

    // modificata con i metodi pubblici
    std::unordered_map<std::string, boost::function<void()>> err2call;
};


#endif
