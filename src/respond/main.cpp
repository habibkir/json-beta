#include<iostream>
#include<fstream>
#include<map>
#include<filesystem>

#include<boost/function.hpp>

#include "include/json.hpp"
using json = nlohmann::json;

class Logger {
public:
    explicit Logger(std::string& file) {
	ofs = new std::ofstream(file);
    }
    void log(std::string& msg) {
	*ofs<<msg;
    }
    ~Logger() {
	delete ofs;
    }
private:
    std::ofstream* ofs;
};

class Responder {
public:
    void respond_to(std::string err_id) {
	(azioni[err_id])(this);
    }
    void follow_json(json j) {
	for(json error : j["errors"]) {
	    bind(error["id"], error["fun id"]);
	}
    }
    void bind(std::string err_id, std::string fun_id) {
	azioni[err_id] = funzioni[fun_id];
    }
private:
    void onTrackLost() {
	std::cout<<"non ci vedo"<<std::endl;
    }
    void onCameraFucked() {
	std::cout<<"pessimo modo per non vederci"<<std::endl;
    }
    void onJumanji() {
	std::cout<<"ho finito le battute"<<std::endl;
    }
    /*
     * il campo function del json si riferisce a quanto indicato in questa mappa
     * il (Responder*) nella signature messa nel template è per il puntatore this
     * che è sempre implicitamente passato come parametro a tutti i metodi di una classe
     */
    std::map<std::string, boost::function<void(Responder*)>> funzioni =
	{{"onTrackLost", &Responder::onTrackLost},
	 {"onCameraFucked", &Responder::onCameraFucked},
	 {"onJumanji", &Responder::onJumanji}};

    /*
     * Responder::bind() associerà l'id alla funzione in questa mappa
     * in modo da poter cambiare in modo più flessibile il funzionamento dell'error handler
     * attraverso, per l'appunto, questa mappa
     * (che sarà quella da cui si chiamano le funzioni)
     */
    std::map<std::string, boost::function<void(Responder*)>> azioni;
};

/*
 * da
 *
 * #include<stdio.h>  // per #define FILENAME_MAX
 * #include<unistd.h> // unix standard library
 * char b[FILENAME_MAX];
 * getcwd(b,FILENAME_MAX); // get current working directory
 * std::string str(b);
 * std::cout<<str;
 * 
 * mi dicono che la working directory in cui
 * viene eseguito l'eseguibile è la directory base del workspace
 * in questo caso /home/troia/Ros/json-beta
 * quindi le ifstream devono essere inizializzate rispetto 
 * a questa informazione
 */



int main() {
    // con la absolute path funziona
    std::ifstream ifs("error-files/errors.json");
    json j = json::parse(ifs);
    Responder r;
    r.follow_json(j);

    r.respond_to("track lost");

    return 0;
}
