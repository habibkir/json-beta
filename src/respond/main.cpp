#include<iostream>
#include<fstream> // per leggere i file json

#include<algorithm> // per std::replace

#include<boost/function.hpp>

#include "ros/ros.h"
#include "std_msgs/String.h"

#include "include/json.hpp"
using json = nlohmann::json;



class Logger {
public:
    Logger () {
	ofs = new std::ofstream(log_file_name());
    }
    void log(std::string& msg) {
	(*ofs)<<msg<<" time : "<<curr_time_c_str();
    }

    ~Logger() {
	delete ofs;
    }
private:
    time_t _time = time(NULL);
    struct curr_time * curtime;
    std::ofstream* ofs;
    std::string log_file_name() {
	std::string date = std::string(curr_time_c_str());
	// evitimao di farci uccidere da qualche filesystem
	std::replace(date.begin(), date.end(), ' ', '-');
	std::replace(date.begin(), date.end(), ':', '_');

	return "log-files/" + date + "-errors.log";
    }
    char* curr_time_c_str() {
	return asctime(localtime(&_time));
    }
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
    void onControlsFucked() {
	std::cout<<"non riesco a muovermi molto bene"<<std::endl;
    }

    void haltAndCatchFire() {
	std::cout<<"GAME OVER YEEEEEEEAAAAAAH!!!!!!"<<std::endl;
    }

    void haltAndLinkTheFire () {
	std::cout<<"plin plin plon, plin plin plin, plin plon"<<std::endl;
    }
    /*
     * il campo function del json si riferisce
     * a quanto indicato in questa mappa
     * il (Responder*) nella signature messa nel template
     * è per il puntatore this
     * che è sempre implicitamente passato come parametro
     * a tutti i metodi di una classe
     */
    std::map<std::string, boost::function<void(Responder*)>> funzioni =
	{{"onTrackLost", &Responder::onTrackLost},
	 {"onCameraFucked", &Responder::onCameraFucked},
	 {"onControlsFucked", &Responder::onControlsFucked},
	 {"haltAndCatchFire", &Responder::haltAndCatchFire},
	 {"haltAndLinkTheFire", &Responder::haltAndLinkTheFire}};

    /*
     * Responder::bind() associerà l'id alla funzione in questa mappa
     * in modo da poter cambiare in modo più flessibile
     * il funzionamento dell'error handler
     * attraverso, per l'appunto, questa mappa
     * (che sarà quella da cui si chiamano le funzioni nel respond_to)
     */
    std::map<std::string, boost::function<void(Responder*)>> azioni;
};

/*
 * da
 *
 * #include<stdio.h>  // per #define FILENAME_MAX
 * #include<unistd.h> // unix standard library
 * char b[FILENAME_M.AX];
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

// variabili globali, peccato contro l'umanità et al
// trova il modo di toglierle
Logger* log4j = new Logger();
Responder* r = new Responder();

void error_callback(const std_msgs::String::ConstPtr& msg) {
    std::string str = msg->data;
    log4j->log(str);
    r->respond_to(str);
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "respond");
    ros::NodeHandle nh;

    std::ifstream ifs("error-files/errors.json");
    json j = json::parse(ifs);
    r->follow_json(j);

    ros::Subscriber error_sub = nh.subscribe("errori", 100, error_callback);

    ros::spin();

    return 0;
}
