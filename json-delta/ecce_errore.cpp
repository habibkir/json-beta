// questo file non compila per gli stessi errori visti a mezzanotte ieri/ il 23

// base
#include<iostream>

// per il responder
#include<unordered_map>
#include<boost/function.hpp>

class Responder {
public:
    void respond_to(std::string err_id) { // oooh, upfront
	(err2callback[err_id])(this);
    }
    void bind_fun(std::string err_id, boost::function<void(Responder*)> fun) {
	(err2callback[err_id]) = fun;
    }

private:
    std::unordered_map<std::string, boost::function<void(Responder*)>> err2callback =
	{{"errore", &Responder::urla}};
    // qui ci vannno gli errori comuni a tutti

    void urla() {
	std::cout<<"MA CHE OOOOOH!\n";
    }
};

class Subclass_responder : public Responder {
public:
    void bind_id(std::string err_id, std::string fun_id) {
	bind_fun(err_id, id2fun[fun_id]);
    }
private:
    // l'errore è che qui, &Subclass_responder::metodo() ha signature
    // void(Subclass_responder*)
    // (per il puntatore this viene passato come parametro implicito a tutti i metodi,
    // come il self di python),
    // questo non è compatibile con la signature
    // void(Responder*) che serve nel template affinchè possa integrarsi con
    // le signature dei metodi di responder, quindi boh, non fa, c'est la vie
    std::unordered_map<std::string, boost::function<void(Responder*)>> id2fun =
	{{"la mamma", &Subclass_responder::puttana},
	 {"cogito ergo", &Subclass_responder::sium},
	 {"plin plin", &Subclass_responder::plon}};

    void puttana() {
	std::cout<<"C'ha la mamma puttana\n";
    }

    void sium() {
	std::cout<<"SIUM\n";
    }

    void plon() {
	std::cout<<"In the age of aincinets\n";
    }
};


int main() {
    Responder* r = new Responder();
    Subclass_responder* s = new Subclass_responder();

    r->respond_to("errore");

    s->respond_to("la mamma");
    s->respond_to("cogito ergo");
    s->respond_to("plin plin");

    return 0;
}
