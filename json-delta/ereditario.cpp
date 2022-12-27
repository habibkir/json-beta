// visto che il problema era con le signature, ho parametrizzato le signature

// questo funziona con il subtyping, ma lo fa coi template, la visione è sconsigliata
// a chiunque abbia ancora una qualche anima, ho creato un mostro, di nuovo

// base
#include<iostream>

// per il responder
#include<unordered_map>
#include<boost/function.hpp>

// per il json
#include<fstream>
#include"json.hpp"
using json = nlohmann::json;


template<typename T>
class Responder {
public:
    void respond_to(std::string err_id) {
	(err2callback[err_id])(nullptr);
    }
    void bind_fun(std::string err_id, boost::function<void(T*)> fun) {
	(err2callback[err_id]) = fun;
    }
    virtual void bind_id(std::string err_id, std::string fun_id) = 0;

    virtual void follow_json(json j) {
	for(json err : j["errors"]) {
	    bind_id(err["id"], err["fun id"]);
	}
    }
    // visto che virtual, qui bind_id chiamerà la funzione bind_id della sottoclasse
    // qualora la sottoclasse facesse l'override, che grazialcazzo è astratto quel metodo
    // o lo riscrivi o t'attacchi
    virtual ~Responder() = default;

private:
    std::unordered_map<std::string, boost::function<void(T*)>> err2callback;
};

class In_peggio : public Responder<In_peggio> {
public:
    explicit In_peggio(std::string s) {
	follow_json(json::parse(std::ifstream(s)));
    }
    virtual void bind_id(std::string err_id, std::string fun_id) override {
	bind_fun(err_id, id2fun[fun_id]);
    }
private:
    std::unordered_map<std::string, boost::function<void(In_peggio*)>> id2fun =
	{{"la mamma", &In_peggio::puttana},
	 {"cogito ergo", &In_peggio::sium}};
	 
    void puttana() {
	std::cout<<"Laura Poggiolini\n";
    }
    void sium() {
	std::cout<<"SIUM\n";
    }
};

int main() {
    In_peggio* peggio = new In_peggio("jason.json");

    peggio->respond_to("cazzo");
    peggio->respond_to("aiuto");

    return 0;
}
