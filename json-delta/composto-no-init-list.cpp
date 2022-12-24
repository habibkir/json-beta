// qui provo a fare un classico "composition over inheritance" visto che l'ereditarietà
// c++ è l'ereditarietà in c++, guarda da te il file

// visto che il problema era di nuovo nella signature
// ho parametrizzato di nuovo la signature

// base
#include<iostream>

// per il responder
#include<unordered_map>
#include<boost/function.hpp>

// per il json
#include<fstream>
#include"json.hpp"
using json = nlohmann::json;

// Responder è fatto per essere parte di un'altro oggetto
// bene o male il mr.callback di una classe a cui serve fare callback
// quindi

// Responder<wrapper> sarà quindi un coso che chiama metodi di wrapper
template<typename wrapper>
class Responder {
public:
    Responder
    (std::unordered_map <std::string, boost::function<void(wrapper*)>> id2fun)
	: id2fun(id2fun) {};
    // (poi lo facciamo col json)
    void respond_to(std::string err_id) {
	(err2call[err_id])(nullptr);
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
    std::unordered_map<std::string, boost::function<void(wrapper*)>> id2fun;
    std::unordered_map<std::string, boost::function<void(wrapper*)>> err2call;
};

// questo fa per composzione e forwarding dal responder
class Ken_combo {
public:
    Ken_combo(std::string s) {
	// metti err2call nel responder
	std::ifstream ifs = std::ifstream(s);
	json j = json::parse(ifs);
	r->follow_json(j);
    }
    void respond_to(std::string err_id) {
	r->respond_to(err_id); // le forwarding has arrived
    }
    void bind(std::string err_id, std::string fun_id) {
	r->bind(err_id, fun_id);
    }
    ~Ken_combo() {
	delete r;
    }

private:
    // metti id2fun nel responder
    // questo viene chiamato alla costruzione, come fare un
    // ArrayList<Integer> a = new ArrayList<> ();
    // come membro di una classe
    Responder<Ken_combo>* r = new Responder<Ken_combo>
	    (std::unordered_map<std::string, boost::function<void(Ken_combo*)>>
	     ({{"wombo", &Ken_combo::wombo},
	       {"combo", &Ken_combo::combo}}));

    void wombo() {
	std::cout<<"wombo\n";
    }

    void combo() {
	std::cout<<"combo\n";
    }
};

int main() {
    Ken_combo* ken = new Ken_combo("argo.json");

    ken->respond_to("not falco");
    ken->respond_to("ken");

    return 0;
}
