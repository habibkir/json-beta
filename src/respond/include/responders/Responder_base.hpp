#ifndef RESPONDER_RESPONDER_BASE_H
#define RESPONDER_RESPONDER_BASE_H

// base
#include<iostream>

// per il responder
#include<unordered_map>
#include<boost/function.hpp>

// per il json
#include<fstream>
#include"../json/json.hpp"
using json = nlohmann::json;

// questo è solo "zuccherino sintattico" per il costruttore
#include<initializer_list>

// Responder è fatto per essere parte di un'altro oggetto
// bene o male il mr.callback di una classe a cui serve fare callback
// quindi fatto apposta per essere un wrapped
//
// Responder<wrapper> sarà quindi un coso che chiama metodi di wrapper
// l'organo che gestisce le callback
template<typename wrapper>
class Responder_base {
public:
    // questo è per fare sì che il Responder abbia un constructor un pochino
    // meno massacrante per gli occhi

    // magia nera
    Responder_base(std::initializer_list
	      <std::pair<const std::string,
	      boost::function<void(wrapper*)>>> init): id2fun(init) {}

    void respond_to(std::string err_id) {
	(err2call[err_id])(w);
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
    wrapper* w;
    
    // inizializzata nel costruttore
    std::unordered_map<std::string, boost::function<void(wrapper*)>> id2fun;

    // modificata con i metodi pubblici
    std::unordered_map<std::string, boost::function<void(wrapper*)>> err2call;
};


#endif
