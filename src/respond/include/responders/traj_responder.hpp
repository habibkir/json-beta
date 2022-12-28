#ifndef RESPONDER_TRAJ_RESPONDER_H
#define RESPONDER_TRAJ_RESPONDER_H

#include<iostream> // per i cout di test
#include<unordered_map>

#include<boost/function.hpp>

#include "../json/json.hpp"
using json = nlohmann::json;

#include "Responder_base.hpp" // and redpilled

class Traj_responder {
public:
    // costruttore prende la path del json
    Traj_responder(std::string s) {
	// metti err2call nel responder
	std::ifstream ifs = std::ifstream(s);

	//
	std::string str;
	std::cout<<"==== BEGIN PGP KEY ===="<<std::endl;
	while(std::getline(ifs, str)) {
	    std::cout<<str<<std::endl;
	}
	std::cout<<"===== END PGP KEY ====="<<std::endl;
	ifs.close();
	ifs.open(s);
	//

	json j = json::parse(ifs);
	r->follow_json(j);
    }
    void respond_to(std::string err_id) {
	std::cout<<"Traj_error responding to "<<err_id<<std::endl;
	r->respond_to(err_id);
    }
    void bind(std::string err_id, std::string fun_id) {
	r->bind(err_id, fun_id);
    }
    ~Traj_responder() {
	delete r;
    }
private:
    Responder_base <Traj_responder>* r =
	new Responder_base <Traj_responder>
	({{"onTrackLost", &Traj_responder::test},
	  {"onCameraFucked", &Traj_responder::test},
	  {"onControlsFucked", &Traj_responder::test},
	  {"haltAndCatchFire", &Traj_responder::test},
	  {"haltAndLinkTheFire", &Traj_responder::test}});

    void test() {
	std::cout<<"Cogito ergo segfault(core dumped)"<<std::endl;
    }
};

#endif
