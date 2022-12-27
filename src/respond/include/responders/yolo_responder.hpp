#ifndef RESPONDER_YOLO_RESPONDER_H
#define RESPONDER_YOLO_RESPONDER_H

#include<iostream> // per i cout di test
#include<unordered_map>

#include<boost/function.hpp>

#include "../json/json.hpp"
using json = nlohmann::json;

#include "Responder_base.hpp" // and redpilled

class Yolo_responder {
public:
    // costruttore prende la path del json
    Yolo_responder(std::string s) {
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
    ~Yolo_responder() {
	delete r;
    }
private:
    Responder_base <Yolo_responder>* r =
	new Responder_base <Yolo_responder>
	({{"onTrackLost", &Yolo_responder::test},
	  {"onCameraFucked", &Yolo_responder::test},
	  {"onTrajFucked", &Yolo_responder::test},
	  {"haltAndCatchFire", &Yolo_responder::test},
	  {"haltAndLinkTheFire", &Yolo_responder::test}});

    void test() {
	std::cout<<"Cogito ergo segfault(core dumped)"<<std::endl;
    }
};

#endif
