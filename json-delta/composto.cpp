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

// questo è solo "zuccherino sintattico" per il costruttore
#include<initializer_list>

// Responder è fatto per essere parte di un'altro oggetto
// bene o male il mr.callback di una classe a cui serve fare callback
// quindi fatto apposta per essere un wrapped
//
// Responder<wrapper> sarà quindi un coso che chiama metodi di wrapper
template<typename wrapper>
class Responder {
public:
    // questo è per fare sì che il Responder abbia un constructor un pochino
    // meno massacrante per gli occhi

    // ci vorrebbero cinque/dieci minuti solo a spiegare la sigature, lascia fare per ora
    // magari dopo natale
    Responder(std::initializer_list
	      <std::pair<const std::string,
	      boost::function<void(wrapper*)>>> init) : id2fun(init) {}

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
    // entrambe queste mappe sono gestite alla costruzione del Ken_combo/wrapper 
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
    // un'espressione come questa viene chiamata alla costruzione
    // come fare un
    // private ArrayList<Integer> a = new ArrayList<> ();
    // nella definizione di una classe java
    Responder<Ken_combo>* r = new Responder<Ken_combo>
	     ({{"wombo", &Ken_combo::wombo},
	       {"combo", &Ken_combo::combo}});

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
