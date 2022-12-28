#include<iostream>
#include<fstream> // per leggere i file json

#include<sstream> // per stampare sta cazzo di data
#include<ctime>
#include<iomanip>

#include<algorithm> // per std::replace

#include<boost/function.hpp>

#include "ros/ros.h"
#include "std_msgs/String.h"

#include "include/json/json.hpp"
using json = nlohmann::json;

#include "include/responders/controls_responder.hpp"
#include "include/responders/yolo_responder.hpp"
#include "include/responders/slam_responder.hpp"
#include "include/responders/traj_responder.hpp"

// l'implementazione corrente è di un inefficiente spaventoso
// si veda poi come farla decente
class Time_formatter {
public:
    static std::string curr_time_str() {
	auto t = std::time(nullptr);
	auto tm = *(std::localtime(&t));
	std::stringstream ss;
	ss<<std::put_time(&tm, "%d-%m-%Y-%H_%M_%S");
	return ss.str();
    }
};

class Logger {
public:
    Logger(std::string base_dir) {
	std::string s =
	    base_dir + "/"
	    + Time_formatter::curr_time_str()
	    + "-errors.log";

	std::cout<<"begin nome del file del logger"<<std::endl;
	std::cout<<s<<std::endl;
	std::cout<<"end nome del file del logger"<<std::endl;
	ofs = std::ofstream(s);
    }
    void log(std::string msg) {
	std::cout<<"logging "<<msg<<std::endl;
	ofs<<msg<<", tempo : " + Time_formatter::curr_time_str()<<std::endl;
	// std::endl dovrebbe fare il flush, quindi se non si vede il file non so che dovrebbe voler dire
    }
    ~Logger() {
	ofs.close();
    }
private:
    std::ofstream ofs;
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

Logger* logger;
Traj_responder* t;

void error_callback(const std_msgs::String::ConstPtr& msg) {
    std::string str = msg->data;
    std::cout<<"begin error_callback"<<std::endl;
    std::cout<<str<<std::endl;
    logger->log(str);
    t->respond_to(str);
    std::cout<<"end error_callback"<<std::endl;
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "respond");
    ros::NodeHandle nh;

    std::string logs_dir;
    nh.getParam("/respond_node/logs_dir", logs_dir);
    std::cout<<logs_dir<<std::endl;
    logger = new Logger(logs_dir);

    std::string err;
    if(nh.getParam("/respond_node/error_topic", err)) {
	std::cout<<"l'error topic del responder è il seguente"<<std::endl;
	std::cout<<err<<std::endl;
    }
    else {
	std::cout<<"error_topic non s'ha da fare"<<std::endl;
    }

    std::string err_json;
    if(nh.getParam("/respond_node/error_json", err_json)) {
	std::cout<<"l'error json del responder è il seguente"<<std::endl;
	std::cout<<err_json<<std::endl;
    }
    else {
	std::cout<<"error_json non s'ha da fare"<<std::endl;
    }

    std::vector<std::string> params;
    nh.getParamNames(params);
    for(auto a : params) {
	std::cout<<a<<" : ";
	std::string param_val;
	nh.getParam(a,param_val);
	std::cout<<param_val<<std::endl;
    }

    t = new Traj_responder(err_json);
    std::cout<<"begin quindi"<<std::endl;
    std::cout<<"error topic "<<err<<std::endl;
    std::cout<<"error json "<<err_json<<std::endl;
    std::cout<<"end quindi"<<std::endl;
    ros::Subscriber error_sub = nh.subscribe(err, 100, error_callback);

    ros::spin();

    return 0;
}
