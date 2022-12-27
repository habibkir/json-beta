#ifndef RESPONDER_CONTROLS_RESPONDER_H
#define RESPONDER_CONTROLS_RESPONDER_H

#include<iostream> // per i cout di test
#include<unordered_map>

#include<boost/function.hpp>

#include "Responder_base.hpp"
#include "../json/json.hpp"
using json = nlohmann::json;

class Controls_responder {
public:
    // costruttore prende la path del json
    Controls_responder(std::string s) {
	// metti err2call nel responder
	std::ifstream ifs = std::ifstream(s);
	json j = json::parse(ifs);
	r->follow_json(j);
    }
    void respond_to(std::string err_id) {
	r->respond_to(err_id);
    }
    void bind(std::string err_id, std::string fun_id) {
	r->bind(err_id, fun_id);
    }
    ~Controls_responder() {
	delete r;
    }
private:
    Responder_base <Controls_responder>* r =
	new Responder_base <Controls_responder>
	({{"onTrackLost", &Controls_responder::onTrackLost},
	  {"onCameraFucked", &Controls_responder::onCameraFucked},
	  {"onControlsFucked", &Controls_responder::onControlsFucked},
	  {"haltAndCatchFire", &Controls_responder::haltAndCatchFire},
	  {"haltAndLinkTheFire", &Controls_responder::haltAndLinkTheFire}});

    void onTrackLost() {
	std::cout<<"non ci vedo"<<std::endl;
    }
    void onCameraFucked() {
	std::cout<<"pessimo modo per non vederci"<<std::endl;
    }
    void onControlsFucked() {
	std::cout<<"non riesco a muovermi molto bene"<<std::endl;
    }

    void haltAndCatchFire() {
	std::cout<<"GAME OVER YEEEEEEEAAAAAAH!!!!!!"<<std::endl;
    }

    void haltAndLinkTheFire () {
	std::cout<<"plin plin plon, plin plin plin, plin plon"<<std::endl;
    }

};

#endif
